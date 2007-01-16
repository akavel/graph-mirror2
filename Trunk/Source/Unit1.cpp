/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Graph.h"
#pragma hdrstop
#include "Unit2.h"
#include "Unit3.h"
#include "Unit4.h"
#include "Unit5.h"
#include "Unit6.h"
#include "Unit7.h"
#include "Unit9.h"
#include "Unit10.h"
#include "Unit11.h"
#include "Unit12.h"
#include "Unit13.h"
#include "Unit14.h"
#include "Unit15.h"
#include "Unit16.h"
#include "Unit17.h"
#include "Unit18.h"
#include "Unit19.h"
#include <Clipbrd.hpp>
#include <Registry.hpp>
#include <Printers.hpp>
#include <limits>
#include <cmath>
#include <float.h>
#include "IGraphic.h"
#include "Clipboard.h"
#include "VersionInfo.h"
#include "GuiHelper.h"
#include "BMGLibPNG.h"                     //Used to save png files
#include "OleServerImpl.h"
#include "PrintForm.h" //Needed to access TPrintFrm for translation
#include <Menus.hpp>
#include <fstream>
#include <iomanip>
#include <jpeg.hpp>
#include "IThread.h"
#include "Debug.h"
#include <iostream>
#include "PngHelper.h"
#include "StackTrace.h"
#include "pdflib.hpp"
#include "ConfigFile.h"
#include "ConfigRegistry.h"
#include <TypInfo.hpp>
#include "OleObjectElem.h"
#include "PythonBind.h"
#include "Encode.h"
//---------------------------------------------------------------------------
#pragma link "TRecent"
#pragma link "Cross"
#pragma link "MySplitter"
#pragma link "FocusPanel"
#pragma link "IRichEdit"
#pragma link "IPolygon"
#pragma link "IColorSelect"
#pragma link "IPageSetup"
#pragma link "OpenPreviewDialog"
#pragma link "IPrintDialog"
#pragma link "SaveDialogEx"
#pragma link "TntComCtrls"
#pragma link "TntActnList"
#pragma link "TntMenus"

#pragma link "Wininet.lib" //Used for InternetGetConnectedState()
#pragma link "../../BMGLib/BMGLibPNG.lib"  //Used to save png files
#pragma link "PDFlib.lib"
#pragma resource "*.dfm"
TForm1 *Form1;
const TCursor crMoveHand1 = 1;                       
const TCursor crMoveHand2 = 2;
TMutex GlobalMutex("Graph running"); //Global Mutex object indicating Graph is running (Checked by installation program)
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: IsResizing(0), TTntForm(Owner), Updating(0), StatusIcon(-1), CursorState(csIdle),
    FixedImages(ImageList1->Count), Draw(Image1->Canvas, &Data, false, "DrawThread"), AbortPrinting(false)
{
#ifdef _DEBUG
  std::ofstream out(ChangeFileExt(Application->ExeName, ".log").c_str(), std::ios_base::app);
  if(out)
  {
    out << std::endl << std::endl;
    out << "PID: 0x" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << GetCurrentProcessId() << std::endl;
    out << "Date: " << DateTimeToStr(Now()).c_str() << std::endl;
    out << "CmdLine: " << CmdLine << std::endl;
  }
 
  SetApplicationExceptionHandler(true); //Log all exceptions
  InitDebug();
#else
  SetApplicationExceptionHandler(GetRegValue(REGISTRY_KEY, "LogExceptions", HKEY_CURRENT_USER, false));
#endif

  Font->Name = "MS Shell Dlg";
  Data.SetAbortUpdateEvent(&Draw.AbortUpdate);

  //Set "AllowOLE" to 1 in the Registry to use OLE with instances not started with "-Embedding"
  if(!FindCmdLineSwitch("EMBEDDING") && !GetRegValue(REGISTRY_KEY, "AllowOLE", HKEY_CURRENT_USER, 0))
    //Prevent OLE instantiation if not started for use with OLE
    _Module.RevokeClassObjects();
                                                                  
  SetThreadName("Main");
  DefaultInstance->OnDebugLine = DebugLine;

  Initialize();                        

  //Don't update when system settings are changed (especially DecimalSeparator)
  Application->UpdateFormatSettings = false;

  //Use dot as decimal separator
  DecimalSeparator = '.';

  //Set bitmap to screen size; Maximum possible size
  Image1->Picture->Bitmap->Height = Screen->Height;
  Image1->Picture->Bitmap->Width = Screen->Width;

  //Save list of buttons on toolbar before they are loaded from registry
  DefaultToolBar = GetToolBar();

  //Accept draged files from explorer
  DragAcceptFiles(Handle, true);

  //Maximum time in milliseconds hint is shown
  Application->HintHidePause = 10000;

  LoadSettings();
  ScaleForm(this);
  ActionToolBar1->ActionClient->Items->SmallIcons = Data.Property.FontScale < 150;

  //Don't create Form9 before settings are loaded. Scaling and other settings are needed in the constructor.
  Form9.reset(new TForm9(NULL));

  //Center main window on the working area
  CenterForm(this);

  //Set shortcuts that depends on the locale settings
  ZoomInAction->ShortCut = ShortCut(VkKeyScan('+') & 0xFF, TShiftState() << ssCtrl);
  ZoomOutAction->ShortCut = ShortCut(VkKeyScan('-') & 0xFF, TShiftState() << ssCtrl);

  Screen->Cursors[crMoveHand2] = LoadCursor(HInstance, "MOVECURSOR1");
  Screen->Cursors[crMoveHand1] = LoadCursor(HInstance, "MOVECURSOR2");
  BOOST_ASSERT(Screen->Cursors[crMoveHand1]);
  BOOST_ASSERT(Screen->Cursors[crMoveHand2]);
  BOOST_ASSERT(TreeView->Items->Count == 0);               
}                                  
//---------------------------------------------------------------------------
void __fastcall TForm1::FormShow(TObject *Sender)
{
  //Load file passed as the first argument to the program
  //This cannot be done from the constructor, because the main form must be fully created.
  //When loading labels TIRichEdit are using the mainform to find the size
  //For this to work Form1->Visible must be set to False at design time
  BOOST_ASSERT(Application->MainForm);

  //Form9 must be created before it can be docked
  //Wait with docking until the form is shown. Else the COM client will blink in the title when we are running as COM server
  if(GetRegValue(REGISTRY_KEY, "DockCalcForm", HKEY_CURRENT_USER, true))
  {
    if(Form9)
      Form9->ManualDock(Panel4);
    Panel1->Height = 0;
  }

  //Do not initialize data when OLE is used. This is done through InitNew() and Load() in IPersistStorage
  if(!FindCmdLineSwitch("EMBEDDING"))
  {
    if(!ParamCount() || ParamStr(1)[1] == '/' || ParamStr(1)[1] == '-' || !LoadFromFile(ParamStr(1)))
      LoadDefault();
    Redraw();
  }

  if(Data.Property.CheckForUpdate)
  {
    ShowStatusError(LoadRes(520), clGreen, 0);
    CallFromThread(&CheckForUpdate, true, &CancelStatusError);
  }
  PostMessage(Handle, WM_USER, 0, 0);

  TreeView->SetFocus();
}
//---------------------------------------------------------------------------
void TForm1::Initialize()
{
  Application->BiDiMode = static_cast<TBiDiMode>(GetRegValue(REGISTRY_KEY, "BiDiMode", HKEY_CURRENT_USER, bdLeftToRight));

  //Prevent flicker from Image1 and Panel1 when the form is resized
  ControlStyle = ControlStyle << csOpaque;

  //Prevent flicker
  Panel2->ControlStyle = Panel2->ControlStyle << csOpaque;
  Panel2->DoubleBuffered = true;
  TreeView->ControlStyle = TreeView->ControlStyle << csOpaque;
  TreeView->DoubleBuffered = true;

  //Prevent flicker in status bar
  StatusBar1->DoubleBuffered = true;

  Recent1->RegistryKey = REGISTRY_KEY; //Set key for data in registry

  //Add degree symbols (°) so we get 270° in the rotation menus
  for(int I = 0; I < Label_Placement->Count; I++)
  {
    Label_Rotation->Items[I]->Caption = Label_Rotation->Items[I]->Caption + WideString(L'\xB0');
    Tree_Rotation->Items[I]->Caption = Tree_Rotation->Items[I]->Caption + WideString(L'\xB0');
  }

  //Label rotation doesn't work under Windows 9x
  Label_Rotation->Visible = IsWinNT;

  //Be careful about using TreeView->Items before OnShow. It has probably not been streamed yet
}
//---------------------------------------------------------------------------
  struct TComponentInfo
  {
    TComponent *Component;
    AnsiString Caption;
    AnsiString Hint;
    TComponentInfo(TComponent *AComponent)
      : Component(AComponent), Caption(GetWideStrProp(AComponent, "Caption")), Hint(GetWideStrProp(AComponent, "Hint")) {}
  };

void TForm1::Translate()
{
  static std::vector<TComponentInfo> TranslateList;
  if(TranslateList.empty())
  {
    for(int I = 0; I < MainMenu->Items->Count; I++)
      TranslateList.push_back(MainMenu->Items->Items[I]);

    TranslateList.push_back(File_Import);

    TranslateList.push_back(Tree_Export);
    TranslateList.push_back(Tree_Visible);
    TranslateList.push_back(Tree_ShowInLegend);
    TranslateList.push_back(Tree_Placement);
    TranslateList.push_back(Tree_Above);
    TranslateList.push_back(Tree_Below);
    TranslateList.push_back(Tree_Left);
    TranslateList.push_back(Tree_Right);

    TranslateList.push_back(Label_Edit);
    TranslateList.push_back(Label_Delete);
    TranslateList.push_back(Label_Placement);
    TranslateList.push_back(Label_Rotation);
    TranslateList.push_back(Label_Above);
    TranslateList.push_back(Label_Below);
    TranslateList.push_back(Label_Left);
    TranslateList.push_back(Label_Right);

    TranslateList.push_back(ToolBar_Reset);
    TranslateList.push_back(ToolBar_Customize);

    TranslateList.push_back(Legend_Show);
    TranslateList.push_back(Legend_Font);
    TranslateList.push_back(Legend_Placement);
    TranslateList.push_back(Legend_TopRight);
    TranslateList.push_back(Legend_BottomRight);
    TranslateList.push_back(Legend_TopLeft);
    TranslateList.push_back(Legend_BottomLeft);

    //Translate actions
    for(int I = 0; I < ActionManager->ActionCount; I++)
      TranslateList.push_back(ActionManager->Actions[I]);
  }

  //As a workaround on a bug in Windows XP set MainMenu->Images to NULL while the menu is changed.
  //Else the menu will change color from grey to white.
  //Warning: Graph will throw EMenuError at the line "MainMenu->Images = NULL;" under startup when run in Wine under Linux.
  //I have no ide why. It looks like we can detect Wine by looking for the registry key HKCU\Software\Wine.
  if(!RegKeyExists("Software\\Wine", HKEY_CURRENT_USER))
    MainMenu->Images = NULL;

  for(unsigned I = 0; I < TranslateList.size(); I++)
  {
    TComponent *Component = TranslateList[I].Component;
    SetWideStrProp(Component, GetPropInfo(Component, "Caption"), gettext(StripHotkey(TranslateList[I].Caption)));
    if(!TranslateList[I].Hint.IsEmpty())
      SetWideStrProp(Component, GetPropInfo(Component, "Hint"), gettext(TranslateList[I].Hint));
  }
  MainMenu->Images = ImageList2; //Remember to restore the images

  UpdateTreeView(); //Translate "Axes"

  Recent1->Hint = LoadRes(RES_OPEN_FILE);
  SaveDialogEx1->HelpCaption = LoadRes(RES_SAVE_OPTIONS); //Translate the Options dialog button
  if(Form9)
    Form9->Translate();
}
//---------------------------------------------------------------------------
__fastcall TForm1::~TForm1()
{
  //WARNING: DefaultActionBars is not destroyed by the VCL
  //Delete it to prevent Code Guard from reporting a lot of memory leaks
  delete ActionManager->DefaultActionBars;
}
//---------------------------------------------------------------------------
void TForm1::Redraw(void)
{
  //Check if we are able to perform an update
  if(Application->Terminated || Image1->Width == 0)
    return;

  Draw.AbortUpdate();
  //Set width and height
  Draw.SetSize(Image1->Width, Image1->Height);
  //Make sure background is drawn
  Image1->Canvas->Brush->Style = bsSolid;
  //Set background color
  Image1->Canvas->Brush->Color = Data.Axes.BackgroundColor;
  //Clear area
  Image1->Canvas->FillRect(Rect(0,0,Image1->Width,Image1->Height));
  Draw.DrawAll();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Image1MouseDown(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
  if(Shift.Contains(ssDouble))
    return;

  switch(Button)
  {
    case mbLeft:
      Panel2->SetFocus();
      switch(CursorState)
      {
        case csAddLabel:
        {
          SetCursorState(csIdle);
          std::auto_ptr<TForm6> Form6(new TForm6(Application, Data.Property.DefaultLabelFont, NAME, Data.GetFileName().c_str()));
          if(Form6->ShowModal() == mrOk && !Form6->IsEmpty())
          {
            Data.Property.DefaultLabelFont = Form6->GetFont();
            boost::shared_ptr<TTextLabel> Label(new TTextLabel(Form6->GetText().c_str(), lpUserPos, Draw.xyCoord(X, Y), Form6->GetBackgroundColor(), 0));
            Data.Add(Label);
            Label->Update();
            Redraw();
            UndoList.Push(TUndoAdd(Data.Back()));
            UpdateTreeView();
            UpdateMenu();
            Data.SetModified();
          }
          break;
        }

        case csZoomWindow:
          Shape1->SetBounds(X, Y + ActionToolBar1->Height, 0, 0);
          Shape1->Visible = true;
          xZoom = X;
          yZoom = Y;
          break;

        case csMove:
          UndoList.Push(TUndoAxes());
          MovePos = Func32::TDblPoint(Draw.xCoord(X), Draw.yCoord(Y));
          SetCursorState(csMoving);
          break;

        case csIdle:
          if(Shift.Contains(ssCtrl))
          {
            boost::shared_ptr<TGraphElem> Elem = GetGraphElem(TreeView->Selected);
            if(TPointSeries *Series = dynamic_cast<TPointSeries*>(Elem.get()))
            {
              Series->AddPoint(Draw.xyCoord(X, Y));
              Data.SetModified();
              Redraw(); //We need to redraw everything; Smooth lines may have changed
            }
          }
          else
          {
            boost::shared_ptr<TTextLabel> Label = Data.FindLabel(X, Y);
            if(Label)
            {
              Draw.AbortUpdate();
              SetCursorState(csMoveLabel);
              MovingLabel = Label;
              MovingLabelPlacement = Label->GetPlacement();

              Image2->Top = Label->GetRect().Top;
              Image2->Left = Label->GetRect().Left;
              Image2->Picture->Assign(Label->GetImage());
              Image2->Visible = true;
              ImagePos = TPoint(Image2->Left, Image2->Top);

              Label->Visible = false;
              Redraw();
              break;
            }

            if(Draw.InsideLegend(X, Y))
            {
              Draw.AbortUpdate();
              SetCursorState(csMoveLegend);
              TRect Rect = Draw.GetLegendRect();

              Image2->SetBounds(Rect.Left, Rect.Top, Rect.Width(), Rect.Height());
              Image2->Picture->Bitmap->Width = Rect.Width();
              Image2->Picture->Bitmap->Height = Rect.Height();
              Image2->Canvas->CopyMode = cmSrcCopy;
              Image2->Canvas->CopyRect(Image2->ClientRect, Image1->Canvas, Rect);
              Image2->Transparent = false;
              Image2->Visible = true;
              ImagePos = TPoint(Image2->Left, Image2->Top);
              LegendPlacement = Data.Axes.LegendPlacement;

              Data.Axes.ShowLegend = false;
              Redraw();
            }

            if(EvalAction->Checked)
              SetCross(X, Y);
            else if(AreaAction->Checked || PathAction->Checked)
              Form9->StartValueChanged(X, Y);
          }
          break;
      }
      break;

    case mbRight:
      if(CursorState != csMoveLabel && CursorState != csMoveLegend)
      {
        TPoint Pos = Image1->ClientToScreen(TPoint(X, Y));
        if(Data.FindLabel(X, Y))
          PopupMenu3->Popup(Pos.x, Pos.y);
        else if(Draw.InsideLegend(X, Y))
          PopupMenu4->Popup(Pos.x, Pos.y);

        SetCursorState(csIdle);
      }
      break;

    case mbMiddle:
      //Move coordinate system to the center without changing the scale
      Zoom(0, 0, 0.5, 0.5, false);
      break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Image1MouseMove(TObject *Sender, TShiftState Shift,
	int X, int Y)
{
  if(LastMousePos == TPoint(X, Y))
    return;

  switch(CursorState)
  {
    case csZoomWindow:
      if(Shift.Contains(ssLeft))
      {
        //Check if we are moving outside canvas
        if(X >= Image1->Width)
          X = Image1->Width-1;
        else if(X < 0)
          X = 0;
        if(Y >= Image1->Height)
          Y = Image1->Height-1;
        else if(Y < 0)
          Y = 0;
        Shape1->Width = abs(X-xZoom);
        Shape1->Height = abs(Y-yZoom);
        if(X < xZoom)
          Shape1->Left = X;
        else
          Shape1->Left = xZoom;
        if(Y < yZoom)
          Shape1->Top = Y;
        else
          Shape1->Top = yZoom;
        StatusBar1->Panels->Items[0]->Text = "(" + RoundToStr(Draw.xCoord(xZoom), Data) + " ; " +
          RoundToStr(Draw.yCoord(yZoom), Data) + ")->(" + RoundToStr(Draw.xCoord(X), Data) + " ; " +
          RoundToStr(Draw.yCoord(Y), Data) + ")";
        Application->ProcessMessages();
      }
      else if(X >= 0 && X < Image1->Width && Y >= 0 && Y < Image1->Height)
        StatusBar1->Panels->Items[0]->Text = "(" + RoundToStr(Draw.xCoord(X), Data) + " ; " +
          RoundToStr(Draw.yCoord(Y), Data) + ")";
      break;

    case csMoving:
    {
      TAxes &Axes = Data.Axes;
      double xMin = Axes.xAxis.Min;
      double xMax = Axes.xAxis.Max;
      double yMin = Axes.yAxis.Min;
      double yMax = Axes.yAxis.Max;
      if(Axes.xAxis.LogScl)
      {
        double xScale = MovePos.x / Draw.xCoord(X);
        xMin *= xScale;
        xMax *= xScale;
      }
      else
      {
        double DeltaX = MovePos.x - Draw.xCoord(X);
        xMin += DeltaX,
        xMax += DeltaX;
      }

      if(Axes.yAxis.LogScl)
      {
        double yScale = MovePos.y / Draw.yCoord(Y);
        yMin *= yScale;
        yMax *= yScale;
      }
      else
      {
        double DeltaY = MovePos.y - Draw.yCoord(Y);
        yMin += DeltaY;
        yMax += DeltaY;
      }
      ZoomWindow(xMin, xMax, yMin, yMax, true, false);
      break;
    }

    case csIdle:
      if(Shift.Contains(ssLeft))
      {
        if(EvalAction->Checked)
          SetCross(X,Y);
        else if(AreaAction->Checked || PathAction->Checked)
          Form9->EndValueChanged(X, Y);
      }
      else
        if(!!Data.FindLabel(X, Y) || Draw.InsideLegend(X, Y))
          Panel2->Cursor = crSizeAll;
        else
          Panel2->Cursor = crDefault;
      break;

    case csMoveLabel:
      MoveAndSnapLabel(X - LastMousePos.x, Y - LastMousePos.y, !Shift.Contains(ssCtrl));
      break;

    case csMoveLegend:
      MoveAndSnapLegend(X - LastMousePos.x, Y - LastMousePos.y, !Shift.Contains(ssCtrl));
      break;
  }

  LastMousePos.x = X;
  LastMousePos.y = Y;

  int xDigits = 4 - std::log10(std::abs(Data.Axes.xAxis.Max - Data.Axes.xAxis.Min));
  int xPrecision = xDigits > 0 ? 100 : 4;
  if(xDigits < 0)
    xDigits = 0;
  int yDigits = 4 - std::log10(std::abs(Data.Axes.yAxis.Max - Data.Axes.yAxis.Min));
  int yPrecision = yDigits > 0 ? 100 : 4;
  if(yDigits < 0)
    yDigits = 0;

  WideString Str = "x = " + FloatToStrF(Draw.xCoord(X), ffFixed, xPrecision, xDigits) + "    y = " +
    FloatToStrF(Draw.yCoord(Y), ffFixed, yPrecision, yDigits);
  for(int I = 1; I <= Str.Length(); I++)
    if(Str[I] == '-')
      Str[I] = 0x2212; //Replace Hyphen-Minus with Minus Sign
  StatusBar1->Panels->Items[1]->Text = Str;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Image1MouseUp(TObject *Sender, TMouseButton Button,
	TShiftState Shift, int X, int Y)
{
  if(Shift.Contains(ssDouble))
    return;

  if(Button == mbLeft)
    switch(CursorState)
    {
      case csMoving:
        SetCursorState(csMove);
        break;

      case csMoveLabel:
      {
        Image2->Visible = false;
        MovingLabel->Visible = true; //Works because we can only move a visible label

        //Only if label was actually moved
        if(Image2->Left != MovingLabel->GetRect().Left || Image2->Top != MovingLabel->GetRect().Top)
        {
          UndoList.Push(TUndoChange(MovingLabel, Data.GetIndex(MovingLabel)));
          boost::shared_ptr<TTextLabel> NewLabel(new TTextLabel(MovingLabel->GetText(), MovingLabelPlacement, Draw.xyCoord(Image2->Left, Image2->Top), MovingLabel->GetBackgroundColor(), MovingLabel->GetRotation()));
          Data.Replace(Data.GetIndex(MovingLabel), NewLabel);
          NewLabel->Update();
          NewLabel->UpdateRect(Image2->Left, Image2->Top); //Needed so we don't have to wait for label to be redrawn
          MovingLabel.reset();
        }

        Data.SetModified();
        UpdateMenu();
        Redraw();
        SetCursorState(csIdle);
        break;
      }

      case csMoveLegend:
        Image2->Visible = false;
        Data.Axes.ShowLegend = true;
        UndoList.Push(TUndoAxes());
        Data.Axes.LegendPlacement = LegendPlacement;
        Data.Axes.LegendPos = Draw.xyCoord(Image2->Left, Image2->Top);
        Data.SetModified();
        Redraw();
        SetCursorState(csIdle);
        break;

      case csZoomWindow:
        SetCursorState(csIdle);
        if(X != xZoom && Y != yZoom)
          //Zoom to user selected window;don't change coordinates
          ZoomWindow(Draw.xCoord(Shape1->Left), Draw.xCoord(Shape1->Left+Shape1->Width), Draw.yCoord(Shape1->Top+Shape1->Height), Draw.yCoord(Shape1->Top));
        break;
    }
}
//---------------------------------------------------------------------------
bool TForm1::AskSave(void)
{
  bool Result = true;
  if(Data.IsModified())
  {
    WideString Str = Data.GetFileName().empty() ? LoadRes(RES_SAVE_CHANGES) : LoadRes(RES_SAVE_CHANGES_IN, Data.GetFileName());
    switch(MessageBox(Str, NAME, MB_YESNOCANCEL | MB_ICONEXCLAMATION))
    {
      case IDYES:
        Result = DoSave(Data.GetFileName().c_str(), true);
        break;
      case IDNO:
        Result = true;
        break;
      case IDCANCEL:
        Result = false;
        break;
    }
  }
  if(Result)
    Draw.AbortUpdate();
  return Result;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  DWORD SaveOption = OLECLOSE_SAVEIFDIRTY;

  //Some other programs may ask us to terminate more than once
  //When runing as OLE server only save when using linked object
  //Don't save when running as automation object
  if(!Application->Terminated && (!OleServerRunning() || !Data.GetFileName().empty()))
  {
    CanClose = AskSave();
    SaveOption = OLECLOSE_NOSAVE; //Don't save again
  }

  if(OleServerRunning() && CanClose)
  {
    OleServerImpl->AddRef();
    OleServerImpl->Close(SaveOption);
    OleServerImpl->Release();
  }
}
//---------------------------------------------------------------------------
void TForm1::LoadSettings(void)
{
  TConfigRegistry Registry;

  Registry.OpenKey(REGISTRY_KEY);
  Recent1->MaxFiles = Registry.Read("Recents", 4);
  Application->ShowHint = Registry.Read("ShowHint", true);
  Width = Registry.Read("Width", Width); //Note: Calls FormConstrainedResize()
  StdWidth = Width;

  Height = Registry.Read("Height", Height); //Note: Calls FormConstrainedResize()
  StdHeight = Height;

  Panel3->Width = Registry.Read("SplitterPos", Panel3->Width);
  Panel1->Width = Panel3->Width;

  WindowState = Registry.Read("Maximized", false) ? wsMaximized : wsNormal;
  IPrintDialog1->LeftMargin = Registry.Read("LeftMargin", IPrintDialog1->LeftMargin);
  IPrintDialog1->TopMargin = Registry.Read("TopMargin", IPrintDialog1->TopMargin);
  IPrintDialog1->PrintOutput = Registry.Read("ForceBlackOnPrint", false) ? poBlackWhite : poColor;
  IPrintDialog1->CenterOnPage = Registry.Read("CenterOnPage", true);
  IPrintDialog1->Scale = Registry.Read("PrinterScale", 100);
  IPrintDialog1->Orientation = Registry.ReadEnum("Orientation", poPortrait);
  UndoList.SetMaxUndo(Registry.Read("MaxUndo", 50));

  InitPlugins();

  if(Registry.ValueExists("ToolBar"))
    CreateToolBar(Registry.Read("ToolBar", "").c_str());

  if(Registry.ValueExists("Language"))
    ChangeLanguage(Registry.Read("Language", "English").c_str());
  else
    ChangeLanguage(GetRegValue(REGISTRY_KEY, "Language", HKEY_LOCAL_MACHINE, "English"));

  if(Registry.KeyExists(REGISTRY_KEY "\\Property"))
    Registry.OpenKey(REGISTRY_KEY "\\Property");
  Data.Property.Read(Registry);

  StartToolBar = GetToolBar(); //Save shown toolbar
  Data.LoadUserModels();
}
//---------------------------------------------------------------------------
void TForm1::SaveSettings(void)
{
  TConfigRegistry Registry;
  if(Registry.CreateKey(REGISTRY_KEY))
  {
    Registry.Write("Recents", Recent1->MaxFiles);
    Registry.Write("ShowHint", Application->ShowHint);

    Registry.Write("LeftMargin", IPrintDialog1->LeftMargin);
    Registry.Write("TopMargin", IPrintDialog1->TopMargin);
    Registry.Write("ForceBlackOnPrint", IPrintDialog1->PrintOutput == poBlackWhite);
    Registry.Write("CenterOnPage", IPrintDialog1->CenterOnPage);
    Registry.Write("PrinterScale", IPrintDialog1->Scale);
    Registry.Write("Orientation", IPrintDialog1->Orientation);

    Registry.Write("DockCalcForm", Panel4->DockClientCount);
    if(Data.Property.SavePos)
    {
      Registry.Write("Maximized", WindowState == wsMaximized);
      if(WindowState == wsNormal)
      {
        StdWidth = Width;
        StdHeight = Height;
      }
      Registry.Write("Width", StdWidth);
      Registry.Write("Height", StdHeight);
      Registry.Write("SplitterPos", TreeView->Width);
    }

    Registry.Write("MaxUndo", UndoList.GetMaxUndo());

    AnsiString ToolBar = GetToolBar();
    if(StartToolBar != GetToolBar())
      Registry.Write("ToolBar", ToolBar.c_str());

    if(Registry.CreateKey(REGISTRY_KEY "\\Property"))
      Data.Property.Write(Registry);
  }

  Data.SaveUserModels();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Panel2KeyDown(TObject *Sender, WORD &Key,
    TShiftState Shift)
{
  switch(Key)
  {
    case VK_RIGHT:
      MoveRightAction->Execute();
      break;
    case VK_LEFT:
      MoveLeftAction->Execute();
      break;
    case VK_UP:
      MoveUpAction->Execute();
      break;
    case VK_DOWN:
      MoveDownAction->Execute();
      break;
  }
}
//---------------------------------------------------------------------------
void TForm1::UpdateMenu()
{
  //It may be called after the destructor
  if(Application->Terminated)
    return;

  boost::shared_ptr<TGraphElem> Elem = GetGraphElem(TreeView->Selected);

  if(Elem)
  {
    EditAction->Enabled = true;
    DeleteAction->Enabled = true;
  }
  else
  {
    EditAction->Enabled = false;
    DeleteAction->Enabled = false;
  }

  if(dynamic_cast<TBaseFuncType*>(Elem.get()) && !dynamic_cast<TTan*>(Elem.get()))
  {
    InsertTangentAction->Enabled = true;
    InsertShadeAction->Enabled = true;
    InsertDifAction->Enabled = true;
    TableAction->Enabled = true;
    Tree_InsTan->Visible = true;
    Tree_InsDiff->Visible = true;
    Tree_InsShade->Visible = true;
  }
  else
  {
    InsertTangentAction->Enabled = false;
    InsertShadeAction->Enabled = false;
    InsertDifAction->Enabled = false;
    TableAction->Enabled = false;
    Tree_InsTan->Visible = false;
    Tree_InsDiff->Visible = false;
    Tree_InsShade->Visible = false;
  }

  if(dynamic_cast<TPointSeries*>(Elem.get()))
  {
    InsertTrendlineAction->Enabled = true;
    Tree_InsTrend->Visible = true;
    Tree_Export->Visible = true;
  }
  else
  {
    InsertTrendlineAction->Enabled = false;
    Tree_InsTrend->Visible = false;
    Tree_Export->Visible = false;
  }

  if(dynamic_cast<TBaseFuncType*>(Elem.get()) || dynamic_cast<TPointSeries*>(Elem.get()) || dynamic_cast<TTextLabel*>(Elem.get()) || dynamic_cast<TRelation*>(Elem.get()))
  {
    CutAction->Enabled = true;
    CopyAction->Enabled = true;
  }
  else
  {
    CutAction->Enabled = false;
    CopyAction->Enabled = false;
  }

  if(Elem)
  {
    Tree_Visible->Checked = Elem->Visible;
    Tree_ShowInLegend->Checked = Elem->ShowInLegend;
  }

  if(dynamic_cast<TTextLabel*>(Elem.get()) || dynamic_cast<TAxesView*>(Elem.get()) ||
     dynamic_cast<TOleObjectElem*>(Elem.get()))
    Tree_ShowInLegend->Visible = false;
  else
    Tree_ShowInLegend->Visible = true;

  Tree_Placement->Visible = dynamic_cast<TTextLabel*>(Elem.get());

  if(dynamic_cast<TAxesView*>(Elem.get()))
  {
    Tree_Visible->Visible = false;
    DeleteAction->Enabled = false;
  }
  else
  {
    Tree_Visible->Visible = true;
    DeleteAction->Enabled = true;
  }

  //Enable animation if there are any constants defined
  AnimateAction->Enabled = false;
  for(TCustomFunctions::ConstIterator Iter = Data.CustomFunctions.Begin(); Iter != Data.CustomFunctions.End(); ++Iter)
    if(Iter->Arguments.empty())
    {
      AnimateAction->Enabled = true;
      break;
    }

  UndoAction->Enabled = UndoList.CanUndo();
  RedoAction->Enabled = UndoList.CanRedo();
  PasteAction->Enabled = GraphClipboard.HasData();
  ZoomFitAction->Enabled = dynamic_cast<TBaseFuncType*>(Elem.get()) || dynamic_cast<TPointSeries*>(Elem.get()) || dynamic_cast<TRelation*>(Elem.get());
//  ZoomAllPointsAction->Enabled = !Data.PointList.empty();
  ZoomSquareAction->Enabled = Data.Axes.xAxis.LogScl == Data.Axes.yAxis.LogScl;

#ifdef LIMITED_EDITION
  InsertDifAction->Enabled = false;
#endif
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
  SaveSettings();

  if(Printer()->Printing)
  {
    if(MessageBox(LoadRes(521), NAME, MB_OKCANCEL | MB_ICONEXCLAMATION) == IDCANCEL)
      Action = caNone;
    else
      AbortPrinting = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Math_MonotoniClick(TObject *Sender)
{
  MessageBox(Handle,"Denne funktion er endnu ikke implementeret!","Function not implemented",MB_OK|MB_ICONSTOP);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormConstrainedResize(TObject *Sender,
      int &MinWidth, int &MinHeight, int &MaxWidth, int &MaxHeight)
{
  if(WindowState == wsNormal && Top > 0)
  {
    StdTop = Top;
    StdLeft = Left;
    StdWidth = Width;
    StdHeight = Height;
  }
}
//---------------------------------------------------------------------------
//This function sets the min, max and scl values
bool TForm1::ZoomWindow(double xMin, double xMax, double yMin, double yMax, bool Update, bool SaveUndo)
{
  //Check for out of range
  if(xMax-xMin < 1E-10 || yMax-yMin < 1E-10)
  {
    ShowStatusError(LoadRes(RES_MAX_ZOOM));
    return false;
  }

  //Check if we have zoomed so much out that log10(xMin/xMax) can no longer be evaluated
  if(xMax>1E15 || yMax>1E15 || xMin < -1E15 || yMin < -1E15)
  {
    ShowStatusError(LoadRes(RES_MIN_ZOOM));
    return false;
  }

  Cross->Hide();

  Draw.AbortUpdate();
  if(SaveUndo)
    UndoList.Push(TUndoAxes());

  if(Data.Axes.ZoomSquare && Data.Axes.xAxis.LogScl == Data.Axes.yAxis.LogScl)
  {
    if(Data.Axes.xAxis.LogScl)
    {
      double yMiddle = std::exp((std::log(yMax) + std::log(yMin)) / 2);
      double dy = std::exp(Draw.GetScaledYAxis() * (std::log(xMax) - std::log(xMin)) / 2);
      yMin = yMiddle / dy;
      yMax = yMiddle * dy;
    }
    else
    {
      double yMiddle = (yMin + yMax) /2;
      double dy = Draw.GetScaledYAxis() * (xMax - xMin);
      yMin = yMiddle - dy / 2;
      yMax = yMiddle + dy / 2;
    }
  }
  else
    Data.Axes.ZoomSquare = false;

  //Set min and max for axes
  Data.Axes.xAxis.Min = xMin;
  Data.Axes.xAxis.Max = xMax;
  Data.Axes.yAxis.Min = yMin;
  Data.Axes.yAxis.Max = yMax;

  Data.ClearCache();
  if(Update)
  {
    Data.SetModified();
    Redraw();
    if(!MoveAction->Checked)
      UpdateEval();
    UpdateMenu();
  }
  return true;
}
//---------------------------------------------------------------------------
//This function is called then a file is dragged from explorer
void __fastcall TForm1::WMDropFiles(TMessage &Message)
{
  //Get needed size for filename
  int BufSize = DragQueryFile(HDROP(Message.WParam), 0, NULL, 0) + 1;

  //Allocate memory for filename
  std::vector<char> FileName(BufSize);
  DragQueryFile(HDROP(Message.WParam), 0, &FileName[0], BufSize);

  //Release memory
  DragFinish(HDROP(Message.WParam));

  if(AskSave())
  {
    LoadFromFile(&FileName[0]);
    Redraw();
  }
}
//---------------------------------------------------------------------------
//This function is called then the form, and therefore the
//satusbar is resized
void __fastcall TForm1::StatusBar1Resize(TObject *Sender)
{
  StatusBar1->Panels->Items[0]->Width = StatusBar1->Width - StatusBar1->Panels->Items[1]->Width - 40;
  StatusBar1->Invalidate();
}
//---------------------------------------------------------------------------
//This function is called then the statusbar should be redrawn
void __fastcall TForm1::StatusBar1DrawPanel(TStatusBar *StatusBar,
      TStatusPanel *Panel, const TRect &Rect)
{
  if(Panel == StatusBar->Panels->Items[0])
  {
    StatusBar1->Canvas->FillRect(Rect);
    StatusBar1->Canvas->Font->Color = StatusErrorColor;
    StatusBar1->Canvas->TextOut(5, 4, StatusError);
  }
  else if(Panel == StatusBar->Panels->Items[2])
    if(StatusIcon != -1)
      //Draw icon number StatusIcon from the image list on the statusbar
      ImageList1->Draw(StatusBar->Canvas, Rect.Left+2, Rect.Top, StatusIcon, true);
}
//---------------------------------------------------------------------------
//This constructor shows a message in the stausbar
void TForm1::ShowStatusMessage(const AnsiString &Str)
{
  //Do not disable timer; Warnings may not be overwritten

  //Don't show hints if Str not empty
  StatusBar1->AutoHint = Str.IsEmpty();
  //Show message
  StatusBar1->Panels->Items[0]->Text = Str;
  StatusBar1->Update();
}
//---------------------------------------------------------------------------
//Show error message in status bar. This overwrite a normal message.
//Timeout indicates how long the message is shown
//Timeout=0 will show the message until CancelStatusError() is called
void TForm1::ShowStatusError(const AnsiString &Str, TColor Color, unsigned Timeout)
{
  //Set statusbar to owner drawn; don't show hints, show error message
  StatusBar1->Panels->Items[0]->Style = psOwnerDraw;
  //Save message; written by Statusbar1DrawPanel()
  StatusError = Str;
  StatusErrorColor = Color;
  Timer1->Enabled = false;
  Timer1->Interval = Timeout;
  if(Timeout)
    Timer1->Enabled = true; //Enable timer
  StatusBar1->Update(); //Update message in status bar
}
//---------------------------------------------------------------------------
void TForm1::CancelStatusError()
{
  StatusBar1->Panels->Items[0]->Style = psText;
  Timer1->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1Timer(TObject *Sender)
{
  CancelStatusError();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ApplicationEventsShowHint(AnsiString &HintStr,
      bool &CanShow, THintInfo &HintInfo)
{
  //Maximum length in pixels of hint text before line wrap
  HintInfo.HintMaxWidth = 200;

  if(Application->HintShortCuts)
    if(TCustomAction *CustomAction = dynamic_cast<TCustomAction*>(HintInfo.HintControl->Action))
    {
      if(CustomAction->ShortCut != 0)
        HintStr += " (" + ShortCutToText(CustomAction->ShortCut) + ")";
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::WMEnterSizeMove(TMessage &Message)
{
  IsResizing++;
  TForm::Dispatch(&Message);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::WMExitSizeMove(TMessage &Message)
{
  IsResizing--;
  if(Reseized)
  {
    Draw.AbortUpdate();
    Data.ClearCache();
    SendOleAdvise(acDataChanged);
    Redraw(); //Activates thread; must be done after OLE update
    Reseized = false;
  }
  TForm::Dispatch(&Message);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TreeViewCollapsing(TObject *Sender,
      TTreeNode *Node, bool &AllowCollapse)
{
  AllowCollapse = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Panel2Resize(TObject *Sender)
{
  //Check if bitmap exists
  if(Image1->Picture->Bitmap && Visible && !Application->Terminated)
  {
    if(IsResizing)
    {
      IPolygon1->Visible = false;
      Draw.AbortUpdate();
      if(Data.Axes.ZoomSquare)
        ZoomWindow(Data.Axes.xAxis.Min, Data.Axes.xAxis.Max, Data.Axes.yAxis.Min, Data.Axes.yAxis.Max, false, false);
      //Set width and height
      Draw.SetSize(Image1->Width,Image1->Height);
      //Make sure background is drawn
      Image1->Canvas->Brush->Style = bsSolid;
      //Set background color
      Image1->Canvas->Brush->Color = Data.Axes.BackgroundColor;
      //Clear area
      Image1->Canvas->FillRect(Rect(0,0,Image1->Width,Image1->Height));
      //Only draw the axes, not the functions.
      Draw.RedrawAxes();
      Draw.DrawAxes();
      Reseized = true;
      Cross->Visible = false;
    }
    else
    {
      Draw.AbortUpdate();
      Data.ClearCache();
      Redraw();
      UpdateEval();
    }

    if(OleServerImpl)
      OleServerImpl->SetSize(Image1->Width, Image1->Height);
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TreeViewContextPopup(TObject *Sender,
      TPoint &MousePos, bool &Handled)
{
  if(TreeView->Selected == NULL)
    Handled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TreeViewDblClick(TObject *Sender)
{
  if(TreeView->Selected)
    Tree_Properties->OnClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TreeViewMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if(Button == mbRight)
    TreeView->Selected = TreeView->GetNodeAt(X,Y);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseWheelUp(TObject *Sender,
      TShiftState Shift, const TPoint &MousePos, bool &Handled)
{
  //Check for mouse inside Image1
  if(MousePos.x < Image1->ClientOrigin.x || MousePos.x > Image1->ClientOrigin.x + Image1->Width ||
     MousePos.y < Image1->ClientOrigin.y || MousePos.y > Image1->ClientOrigin.y + Image1->Height)
       return;

  //Change screen cooridnates to coordinates on Image1
  TPoint ClientPos = Image1->ScreenToClient(MousePos);

  //New window is 0.75 times the current
  Zoom(ClientPos, 0.4330127, false);

  Handled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormMouseWheelDown(TObject *Sender,
      TShiftState Shift, const TPoint &MousePos, bool &Handled)
{
  //Check for mouse inside Image1
  if(MousePos.x < Image1->ClientOrigin.x || MousePos.x > Image1->ClientOrigin.x + Image1->Width ||
     MousePos.y < Image1->ClientOrigin.y || MousePos.y > Image1->ClientOrigin.y + Image1->Height)
       return;

  //New window is 2 times the current
  Zoom(0.7071067812, false);
  Handled = true;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SplitterDblClick(TObject *Sender)
{
  int MaxWidth = Splitter->MinSize;
  for(int I = 0; I < TreeView->Items->Count; I++)
  {
    TTreeNode *Node = TreeView->Items->Item[I];
    int StrWidth = TreeView->Canvas->TextWidth(Node->Text) + TreeView->Indent * Node->Level;
    if(StrWidth > MaxWidth)
      MaxWidth = StrWidth;
  }
  Panel3->Width = MaxWidth + 30;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TreeViewChange(TObject *Sender, TTreeNode *Node)
{
  UpdateMenu();

  //Necessary because Form9 may not have been loaded yet
  if(Form9)
    Form9->FuncChanged(GetGraphElem(Node));
}
//---------------------------------------------------------------------------
void TForm1::ChangeLanguage(const AnsiString &Lang)
{
  if(Lang.IsEmpty())
    return;

  AnsiString Path = ExtractFilePath(Application->ExeName);
  UseLanguage(Lang); //dxGetText will not update translation unless langugae has been changed
  DefaultInstance->bindtextdomainToFile("default", Path + "locale\\" + Lang + ".mo");
  AnsiString HelpFile = Path + "Help\\" + Lang + ".hlp";
  if(FileExists(HelpFile))
    Application->HelpFile = HelpFile;
  else
    Application->HelpFile = Path + "Help\\English.hlp";

  //Special handling for Chinese
  MainMenu->AutoHotkeys = Lang == "Chinese" ? maManual : maAutomatic;

  if(Lang != Data.Property.Language)
    Translate();

  Data.Property.Language = Lang;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  switch(Key)
  {
    case VK_F5:
      Draw.AbortUpdate();
      Data.ClearCache();
      Data.Update();
      UpdateTreeView();
      SendOleAdvise(acDataChanged);
      Redraw(); //Activates thread; must be done after OLE update
      break;

    case VK_SHIFT:
      if(CursorState == csIdle)
        SetCursorState(csMove);
      break;
  }

  if(Shift.Contains(ssCtrl))
  {
    if(Shift.Contains(ssShift))
    {
      const char* Languages[] = {"Dutch", "Greek", "Finnish", "Chinese", "Mongolian", "Swedish"};
      if(Key >= '1' && Key <='6')
        ChangeLanguage(Languages[Key - '1']);
    }
    else if(Key >= '0' && Key <='9')
    {
      const char* Languages[] = {"Russian", "English", "Danish", "German", "Spanish", "Portuguese", "Italian", "Slovenian", "French", "Hungarian"};
      ChangeLanguage(Languages[Key - '0']);
    }
    switch(Key)
    {
      case VK_RIGHT:
        MoveRightAction->Execute();
        Key = 0;
        break;
      case VK_LEFT:
        MoveLeftAction->Execute();
        Key = 0;
        break;
      case VK_UP:
        MoveUpAction->Execute();
        Key = 0;
        break;
      case VK_DOWN:
        MoveDownAction->Execute();
        Key = 0;
        break;
      case VK_ADD:
        ZoomInAction->Execute();
        break;
      case VK_SUBTRACT:
        ZoomOutAction->Execute();
        break;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyPress(TObject *Sender, char &Key)
{
  switch(Key)
  {
    case 27:
      Draw.AbortUpdate(); //Abort updating
      Key = 0;

      //If text label is being moved: Hide image and redraw with the old label visible
      if(CursorState == csMoveLabel)
      {
        Image2->Visible = false;
        MovingLabel->Visible = true;
        Redraw();
      }
      else if(CursorState == csMoveLegend)
      {
        Image2->Visible = false;
        Data.Axes.ShowLegend = true;
        Redraw();
      }
      SetCursorState(csIdle);
      break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TreeViewMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  if(StatusBar1->AutoHint)
  {
    TTreeNode *Node = TreeView->GetNodeAt(X,Y);
    boost::shared_ptr<TGraphElem> GraphElem = GetGraphElem(Node);
    if(GraphElem)
    {
      std::wstring Str = GraphElem->MakeText();
      std::wstring LegendText = GraphElem->GetLegendText();
      if(!LegendText.empty() && Str != LegendText)
        Str += L"    \"" + LegendText + L'\"';
      StatusBar1->Panels->Items[0]->Text = Str.c_str();
    }
    else
      StatusBar1->Panels->Items[0]->Text = "";
  }
}
//---------------------------------------------------------------------------
void TForm1::UpdateTreeView(const boost::shared_ptr<TGraphElem> &Selected)
{
  int Index = TreeView->Selected ? TreeView->Selected->AbsoluteIndex : -1;
  TreeView->Items->BeginUpdate();

  TreeView->Selected = NULL;
  TreeView->Items->Clear();

  while(ImageList1->Count > FixedImages)
    ImageList1->Delete(ImageList1->Count-1);

  for(unsigned I = 0; I < Data.ElemCount(); I++)
    Data.GetElem(I)->Accept(TAddView());

  if(Selected)
    TreeView->Selected = GetNode(Selected);
  else if(Index != -1 && TreeView->Items->Count)
    TreeView->Selected = TreeView->Items->Item[Index < TreeView->Items->Count ? Index : TreeView->Items->Count-1];
  TreeViewChange(TreeView, TreeView->Selected);

  TreeView->FullExpand();
  TreeView->Items->EndUpdate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TreeViewKeyPress(TObject *Sender, char &Key)
{
  switch(Key)
  {
    case 13:
      Tree_Properties->OnClick(Sender);
      Key = 0;
      break;
  }
  Key = 0;
}
//---------------------------------------------------------------------------
void TForm1::SetCross(int X, int Y)
{
  if(!Draw.Updating() && !!Form9)
    Form9->StartValueChanged(X, Y);
}
//---------------------------------------------------------------------------
void TForm1::SetCrossPos(double x, double y)
{
  Cross->SetPos(Draw.xPoint(x), Draw.yPoint(y));
  Cross->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpdateEval()
{
  if(Form9)
    Form9->Edit1Change(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
  if(Panel2->Width < Splitter->MinSize)
    TreeView->Width = TreeView->Width - (Splitter->MinSize - Panel2->Width);
  ActionToolBar1->Align = alTop; //Necessary to avoid double line toolbar without reason when maximized
}
//---------------------------------------------------------------------------
void __fastcall TForm1::BeginUpdate()
{
  Updating++;
  if(Panel2->Cursor == crDefault)
    Screen->Cursor = crAppStart;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::EndUpdate()
{
  if(--Updating == 0)
    Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ApplicationEventsActivate(TObject *Sender)
{
  UpdateMenu(); 
}
//---------------------------------------------------------------------------
//Use to zoom in at a given pixel position
bool TForm1::Zoom(TPoint Pos, double ZoomRate, bool ChangeUnits)
{
  return Zoom(Draw.xCoord(Pos.x), Draw.yCoord(Pos.y), ZoomRate, ZoomRate, ChangeUnits);
}
//---------------------------------------------------------------------------
//Use to zoom in and center at current center
bool TForm1::Zoom(double ZoomRate, bool ChangeUnits)
{
  return Zoom(ZoomRate, ZoomRate, ChangeUnits);
}
//---------------------------------------------------------------------------
bool TForm1::Zoom(double xZoomRate, double yZoomRate, bool ChangeUnits)
{
  const TAxes &Axes = Data.Axes;
  double x, y;
  if(Axes.xAxis.LogScl)
    x = std::exp((std::log(Axes.xAxis.Max) + std::log(Axes.xAxis.Min)) / 2);
  else
    x = (Axes.xAxis.Max + Axes.xAxis.Min) / 2;

  if(Axes.yAxis.LogScl)
    y = std::exp((std::log(Axes.yAxis.Max) + std::log(Axes.yAxis.Min)) / 2);
  else
    y = (Axes.yAxis.Max + Axes.yAxis.Min) / 2;

  return Zoom(x, y, xZoomRate, yZoomRate, ChangeUnits);
}
//---------------------------------------------------------------------------
//Use this function to zoom in at a given position
//ZoomRate = sqrt(Z/4), where is the relative window size
//If you want the new window to be 1/4 of the current:  Z=0.25 => ZoomRate=0.25
//If you want the new window to be 4 times the current: Z=4    => ZoomRate=1
bool TForm1::Zoom(double x, double y, double xZoomRate, double yZoomRate, bool ChangeUnits)
{
  TAxes &Axes = Data.Axes;
  double xMin,xMax,yMin,yMax;

  if(Axes.xAxis.LogScl)
  {
    xMin = std::exp(std::log(x) - std::log(Axes.xAxis.Max / Axes.xAxis.Min) * xZoomRate);
    xMax = std::exp(std::log(x) + std::log(Axes.xAxis.Max / Axes.xAxis.Min) * xZoomRate);
  }
  else
  {
    xMin = x - (Axes.xAxis.Max - Axes.xAxis.Min) * xZoomRate;
    xMax = x + (Axes.xAxis.Max - Axes.xAxis.Min) * xZoomRate;
  }
  if(Axes.yAxis.LogScl)
  {
    yMin = std::exp(std::log(y) - std::log(Axes.yAxis.Max / Axes.yAxis.Min) * yZoomRate);
    yMax = std::exp(std::log(y) + std::log(Axes.yAxis.Max / Axes.yAxis.Min) * yZoomRate);
  }
  else
  {
    yMin = y - (Axes.yAxis.Max - Axes.yAxis.Min) * yZoomRate;
    yMax = y + (Axes.yAxis.Max - Axes.yAxis.Min) * yZoomRate;
  }

  if(ZoomWindow(xMin, xMax, yMin, yMax, false))
  {
    if(ChangeUnits)
    {
      Axes.xAxis.TickUnit = Axes.xAxis.LogScl ? std::pow(Axes.xAxis.TickUnit, 2*xZoomRate) : Axes.xAxis.TickUnit * 2*xZoomRate;
      Axes.yAxis.TickUnit = Axes.yAxis.LogScl ? std::pow(Axes.yAxis.TickUnit, 2*yZoomRate) : Axes.yAxis.TickUnit * 2*yZoomRate;
      Axes.xAxis.GridUnit = Axes.xAxis.LogScl ? std::pow(Axes.xAxis.GridUnit, 2*xZoomRate) : Axes.xAxis.GridUnit * 2*xZoomRate;
      Axes.yAxis.GridUnit = Axes.yAxis.LogScl ? std::pow(Axes.yAxis.GridUnit, 2*yZoomRate) : Axes.yAxis.GridUnit * 2*yZoomRate;
    }

    Data.SetModified();
    Redraw();
    UpdateMenu();

    if(!MoveAction->Checked)
      UpdateEval();
    return true;
  }
  return false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Panel2MouseLeave(TObject *Sender)
{
  //Clear mouse cursor coordinate in status bar if mouse is outside Panel2.
  //This event may also happen if we are over another control inside Panel2, eg. Image2
  //Don't use Mouse->CursorPos. A user (Veet) has reported it to fail sometimes)
  DWORD P = GetMessagePos();
  POINTS Points = MAKEPOINTS(P);
  TPoint Pos = Panel2->ScreenToClient(TPoint(Points.x, Points.y));
  if(!InsideRect(Panel2->ClientRect, Pos))
    StatusBar1->Panels->Items[1]->Text = "";
}
//---------------------------------------------------------------------------
//Makes a copy of the image with index Index in ImageList1 and replaces clRed with Color
int TForm1::AddImage(int Index, TColor Color)
{
  std::auto_ptr<Graphics::TBitmap> Bitmap1(new Graphics::TBitmap);
  std::auto_ptr<Graphics::TBitmap> Bitmap2(new Graphics::TBitmap);
  ImageList1->GetBitmap(Index, Bitmap1.get());
  Bitmap2->Canvas->Brush->Color = Color;
  Bitmap2->Width = Bitmap1->Width;
  Bitmap2->Height = Bitmap1->Height;
  TRect Rect(0, 0, Bitmap1->Width, Bitmap1->Height);
  Bitmap2->Canvas->BrushCopy(Rect, Bitmap1.get(), Rect, clRed);
  return ImageList1->Add(Bitmap2.get(), NULL);
}
//---------------------------------------------------------------------------
int TForm1::AddImage(TColor Color, TBrushStyle Style)
{
  std::auto_ptr<Graphics::TBitmap> Bitmap(new Graphics::TBitmap);
  Bitmap->Width = 16;
  Bitmap->Height = 16;
  Bitmap->Canvas->Brush->Color = Color;
  Bitmap->Canvas->Brush->Style = Style;
  Bitmap->Canvas->Pen->Style = psClear;
  Bitmap->Canvas->Rectangle(0, 0, Bitmap->Width, Bitmap->Height);
  return ImageList1->Add(Bitmap.get(), NULL);
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::ApplicationEventsHelp(WORD Command, int Data,
      bool &CallHelp)
{
  //This function is called from the error dialog shown at EFuncError exceptions
  //and when Help|Contents is selected
  if(Command != HELP_FINDER && HelpError)
    Application->HelpJump(AnsiString().sprintf("ERROR_%02d", HelpError));
  return false;
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::Recent1LoadFile(TRecent *Sender,
      const AnsiString &FileName)
{
  if(AskSave())
  {
    LoadFromFile(FileName);
    if(Form9)
      Form9->Clear();

    if(TreeView->Items->Count)
    {
      TreeView->Select(TreeView->Items->Item[0]);
      Form9->FuncChanged(Data.GetElem(0));
    }
    else
      Form9->FuncChanged(boost::shared_ptr<TGraphElem>());
    Redraw();
    return true;
  }
  return false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::xPageSetupDialogPaintText(
      TIPageSetupDialog *Sender, TCanvas *PageCanvas,
      const TRect &UpdateRect, const TRect &PaperSize)
{
  int x=(UpdateRect.Right+UpdateRect.Left)/2;
  int y=(UpdateRect.Bottom+UpdateRect.Top)/2;
  PageCanvas->Pen->Width=2;
  PageCanvas->Pen->Color=clBlue;
  PageCanvas->MoveTo(x,UpdateRect.Top);
  PageCanvas->LineTo(x,UpdateRect.Bottom);
  PageCanvas->MoveTo(UpdateRect.Left,y);
  PageCanvas->LineTo(UpdateRect.Right,y);

  PageCanvas->Pen->Width=2;
  PageCanvas->MoveTo(UpdateRect.Right-7,y-7);
  PageCanvas->LineTo(UpdateRect.Right,y);
  PageCanvas->LineTo(UpdateRect.Right-7,y+7);
  PageCanvas->MoveTo(x-7,UpdateRect.Top+7);
  PageCanvas->LineTo(x,UpdateRect.Top);
  PageCanvas->LineTo(x+7,UpdateRect.Top+7);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::NewActionExecute(TObject *Sender)
{
  if(AskSave())
  {
    if(Form9)
      Form9->Clear();
    LoadDefault();
    Redraw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OpenActionExecute(TObject *Sender)
{
  if(AskSave())
  {
    if(Form9)
      Form9->Clear();
    OpenPreviewDialog1->Filter = LoadRes(RES_GRAPH_FILTER);
    if(OpenPreviewDialog1->Execute())
    {
      LoadFromFile(OpenPreviewDialog1->FileName);
      if(TreeView->Items->Count)
        TreeView->Items->Item[0]->Selected = true;
      Redraw();
    }
  }
}
//---------------------------------------------------------------------------
bool TForm1::DoSave(const AnsiString &FileName, bool Remember)
{
  bool Result = false;
  if(FileName.IsEmpty())
  {
    SaveDialog->FileName = "";
    SaveDialog->DefaultExt = "grf";
    SaveDialog->Filter = LoadRes(RES_FILE_FILTER);
    if(SaveDialog->Execute())
    {
      Result = Data.Save(SaveDialog->FileName.c_str(), Remember);
      if(Result)
        Recent1->FileUsed(SaveDialog->FileName);
    }
  }
  else
  {
    Result = Data.Save(FileName.c_str(), Remember);
    if(Result)
      Recent1->FileUsed(FileName);
  }
  return Result;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SaveActionExecute(TObject *Sender)
{
  DoSave(Data.GetFileName().c_str(), true);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SaveAsActionExecute(TObject *Sender)
{
  DoSave("", true);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SaveAsImageActionExecute(TObject *Sender)
{
  //Remove file extention
  SaveDialogEx1->FileName = ChangeFileExt(Data.GetFileName().c_str(), "");
  SaveDialogEx1->DefaultExt = "emf";
  SaveDialogEx1->Filter =
    "Windows Enhanced Meta File [*.emf]|*.emf|"
//    "Encapsulated PostScript [*.eps]|*.eps|"
//    "Scaleable Vector Graphic [*.svg]|*.svg|"
    "Windows Bitmap [*.bmp]|*.bmp|"
    "Portable Network Graphics [*.png]|*.png|"
    "Joint Photographic Experts Group [*.jpg,*.jpeg]|*.jpg;*.jpeg|"
    "Portable Document Format [*.pdf]|*.pdf";

  ImageOptions.reset(new TImageOptions(Image1->Width, Image1->Height));
  ImageOptions->LoadSettings();
  if(SaveDialogEx1->Execute())
  {
    AnsiString FileName = SaveDialogEx1->FileName; //SaveAsImage() calls ProcessMessages() and may change SaveDialogEx1
    switch(SaveAsImage(FileName, SaveDialogEx1->FilterIndex))
    {
      case seFileAccess:
        MessageBox(LoadRes(RES_FILE_ACCESS, FileName), "Error writing file", MB_OK | MB_ICONSTOP);
        break;

      case seOutOfResources:
        MessageBox(LoadRes(RES_OUT_OF_RESOURCES), "Error writing file", MB_OK | MB_ICONSTOP);
        break;

      case sePdfError:
        break;
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PrintActionExecute(TObject *Sender)
{
  IPrintDialog1->ImageWidth = Image1->Width;
  IPrintDialog1->ImageHeight = Image1->Height;

  //Throws EPrinter on error
  try
  {
    if(!IPrintDialog1->Execute())
      return; //User pressed Cancel
  }
  catch(EPrinter &E)
  {
    MessageBox(E.Message, "Printer error", MB_ICONSTOP);
    return;
  }

  SetStatusIcon(iiPrint);  //Show printer icon in statusbar
  TCallOnRelease Dummy(&SetStatusIcon, -1);
  Printer()->Title = Caption; //Set document title
  Printer()->BeginDoc();      //Start sending to print spooler

  HDC ScreenDC = GetDC(NULL);
  double xMM = GetDeviceCaps(ScreenDC, HORZSIZE); //Get horizontal width of screen in mm
  double yMM = GetDeviceCaps(ScreenDC, VERTSIZE); //Get vertical width of screen in mm
  double xScreenResolution = Screen->Width * 25.4 / xMM; //Get horizontal resolution of screen in pixels per inch
  double yScreenResolution = Screen->Height * 25.4 / yMM; //Get horizontal resolution of screen in pixels per inch
  ReleaseDC(NULL, ScreenDC);
  double xPrinterResolution = GetDeviceCaps(Printer()->Handle, LOGPIXELSX); //Horizontal pixels per inch on printer
  double yPrinterResolution = GetDeviceCaps(Printer()->Handle, LOGPIXELSY); //Vertical pixels per inch on printer
  double xSizeMul = xPrinterResolution / xScreenResolution;
  double ySizeMul = yPrinterResolution / yScreenResolution;

  bool ForceBlack = IPrintDialog1->PrintOutput == poBlackWhite;
  unsigned Copies = IPrintDialog1->Copies;
  TRect Area = IPrintDialog1->DrawingArea;

  Draw.AbortUpdate();
  TData PrintData(Data);
  TDraw PrintDraw(Printer()->Canvas, &PrintData, ForceBlack, "Printing DrawThread");
  PrintDraw.SetArea(Area);  //Set drawing area
  PrintDraw.SetSizeMul(xSizeMul, ySizeMul);

  AbortPrinting = false;
  for(unsigned I = 0; I < Copies && !AbortPrinting; I++)
  {
    if(I > 0)
      Printer()->NewPage();
    PrintDraw.DrawAll();           //Draw graphs
    while(PrintDraw.Updating())
    { //Process messages while waiting for draw thread to finish
      Sleep(100);
      Application->ProcessMessages();
      //OnClose set AbortPrinting to signal that we want to close the application
      if(AbortPrinting)
        PrintDraw.AbortUpdate();
    }
  }

  if(AbortPrinting)
    Printer()->Abort();
  else
    Printer()->EndDoc();             //End printing
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ExitActionExecute(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UndoActionExecute(TObject *Sender)
{
  Draw.AbortUpdate();
  UndoList.Undo();
  Data.SetModified();
  UpdateTreeView();
  UpdateMenu();
  Redraw();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::RedoActionExecute(TObject *Sender)
{
  Draw.AbortUpdate();
  UndoList.Redo();
  Data.SetModified();
  UpdateTreeView();
  UpdateMenu();
  Redraw();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CutActionExecute(TObject *Sender)
{
  //Copy from the edit control if it has focus
  if(TEdit *Edit = dynamic_cast<TEdit*>(ActiveControl))
  {
    Edit->CutToClipboard();
    return;
  }

  if(!TreeView->Selected)
    return;

  boost::shared_ptr<TGraphElem> GraphElem = GetGraphElem(TreeView->Selected);
  if(TBaseFuncType *Func = dynamic_cast<TBaseFuncType*>(GraphElem.get()))
    GraphClipboard.Copy(Func);
  else if(TPointSeries *Series = dynamic_cast<TPointSeries*>(GraphElem.get()))
    GraphClipboard.Copy(Series);
  else if(TTextLabel *Label = dynamic_cast<TTextLabel*>(GraphElem.get()))
    GraphClipboard.Copy(Label);
  else if(TRelation *Relation = dynamic_cast<TRelation*>(GraphElem.get()))
    GraphClipboard.Copy(Relation);

  DeleteGraphElem(GraphElem);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CopyActionExecute(TObject *Sender)
{
  //Copy from the edit control if it has focus
  if(TCustomEdit *Edit = dynamic_cast<TCustomEdit*>(ActiveControl))
  {
    Edit->CopyToClipboard();
    return;
  }

  boost::shared_ptr<TGraphElem> Elem = GetGraphElem(TreeView->Selected);
  if(TBaseFuncType *Func = dynamic_cast<TBaseFuncType*>(Elem.get()))
    GraphClipboard.Copy(Func);
  else if(TPointSeries *Series = dynamic_cast<TPointSeries*>(Elem.get()))
    GraphClipboard.Copy(Series);
  else if(TTextLabel *Label = dynamic_cast<TTextLabel*>(Elem.get()))
    GraphClipboard.Copy(Label);
  else if(TRelation *Relation = dynamic_cast<TRelation*>(Elem.get()))
    GraphClipboard.Copy(Relation);
  else if(TOleObjectElem *OleObjectElem = dynamic_cast<TOleObjectElem*>(Elem.get()))
    OleObjectElem->Copy();
  UpdateMenu();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PasteActionExecute(TObject *Sender)
{
  Draw.AbortUpdate();
  GraphClipboard.Paste(Data);
  Data.SetModified();
  UpdateTreeView();
  UpdateMenu();
  UpdateEval();
  Redraw();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CopyImageActionExecute(TObject *Sender)
{
  IDataObject *Dummy = NULL;
  OleCheck(OleInitialize(NULL));
  if(!OleServerRunning())
    OleCheck(TOleServerImpl::_CreatorClass::CreateInstance(NULL, IID_IDataObject, &(void*&)Dummy));

  if(FAILED(OleSetClipboard(OleServerImpl)))
    CopyAsImageToClipboard(); //Only copy bitmap and metafile
  else
    OleCheck(OleFlushClipboard());

  if(Dummy)
    OleCheck(Dummy->Release());

  OleUninitialize();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AxesActionExecute(TObject *Sender)
{
  if(CreateForm<TForm3>(Data)->ShowModal() == mrOk)
  {
    Draw.AbortUpdate();
    Cross->Hide();
    Data.ClearCache();
    Data.SetModified();
    Redraw();
    UpdateEval();
    //Zoom|Square needs to be updated
    UpdateMenu();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OptionsActionExecute(TObject *Sender)
{
  if(CreateForm<TForm4>(Data)->ShowModal()==mrOk)
  {
    Draw.AbortUpdate();
    Data.ClearCache();
    Redraw();
    UpdateMenu();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::InsertFunctionActionExecute(TObject *Sender)
{
  if(CreateForm<TForm5>(Data)->ShowModal() == mrOk)
  {
    UpdateTreeView();
    TreeView->Items->Item[TreeView->Items->Count-1]->Selected = true;
    TreeView->SetFocus();
    Data.SetModified();
    Redraw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::InsertTangentActionExecute(TObject *Sender)
{
  boost::shared_ptr<TBaseFuncType> Func = boost::dynamic_pointer_cast<TBaseFuncType>(GetGraphElem(TreeView->Selected));
  if(Func)
    if(CreateForm<TForm12>(Data)->InsertTan(Func) == mrOk)
    {
      UpdateTreeView();
      TreeView->Items->Item[TreeView->Items->Count-1]->Selected = true;
      Data.SetModified();
      Redraw();
      UpdateMenu();
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::InsertShadeActionExecute(TObject *Sender)
{
  boost::shared_ptr<TBaseFuncType> Func = boost::dynamic_pointer_cast<TBaseFuncType>(GetGraphElem(TreeView->Selected));
  if(Func)
    if(CreateForm<TForm16>(Data)->InsertShade(Func) == mrOk)
    {
      UpdateTreeView();
      TreeView->Items->Item[TreeView->Items->Count-1]->Selected = true;
      Data.SetModified();
      Redraw();
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::InsertPointsActionExecute(TObject *Sender)
{
  if(CreateForm<TForm14>(Data)->ShowModal() == mrOk)
  {
    UpdateTreeView();
    TreeView->Items->Item[TreeView->Items->Count-1]->Selected = true;
    Data.SetModified();
    Redraw();
    UpdateMenu();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::InsertTrendlineActionExecute(TObject *Sender)
{
  boost::shared_ptr<TPointSeries> Series = boost::dynamic_pointer_cast<TPointSeries>(GetGraphElem(TreeView->Selected));
  if(Series)
    if(CreateForm<TForm13>(Data)->InsertTrendline(Series) == mrOk)
    {
      UpdateTreeView();
      TreeView->Items->Item[TreeView->Items->Count-1]->Selected = true;
      Data.SetModified();
      Redraw();
      UpdateMenu();
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::EditActionExecute(TObject *Sender)
{
  boost::shared_ptr<TGraphElem> Item = GetGraphElem(TreeView->Selected);

  TModalResult Result = mrNone;
  if(boost::shared_ptr<TTan> Tan = boost::dynamic_pointer_cast<TTan>(Item))
    Result = CreateForm<TForm12>(Data)->EditTan(Tan->ParentFunc(), TreeView->Selected->Index);
  else if(boost::shared_ptr<TBaseFuncType> Func = boost::dynamic_pointer_cast<TBaseFuncType>(Item))
    Result = CreateForm<TForm5>(Data)->EditFunc(Func);
  else if(boost::shared_ptr<TPointSeries> PointSeries = boost::dynamic_pointer_cast<TPointSeries>(Item))
    Result = CreateForm<TForm14>(Data)->EditPointSeries(PointSeries);
  else if(boost::shared_ptr<TShade> Shade = boost::dynamic_pointer_cast<TShade>(Item))
    Result = CreateForm<TForm16>(Data)->EditShade(Shade);
  else if(boost::shared_ptr<TTextLabel> Label = boost::dynamic_pointer_cast<TTextLabel>(Item))
    EditLabel(Label);
  else if(boost::shared_ptr<TRelation> Relation = boost::dynamic_pointer_cast<TRelation>(Item))
    Result = CreateForm<TForm11>(Data)->EditRelation(Relation);
  else if(boost::shared_ptr<TAxesView> AxesView = boost::dynamic_pointer_cast<TAxesView>(Item))
    AxesAction->Execute();
  else if(boost::shared_ptr<TOleObjectElem> OleObjectElem = boost::dynamic_pointer_cast<TOleObjectElem>(Item))
    OleObjectElem->Edit();

  if(Result == mrOk)
  {
    Data.SetModified();
    UpdateTreeView();
    Redraw();
  }
}
//---------------------------------------------------------------------------
void TForm1::DeleteGraphElem(const boost::shared_ptr<TGraphElem> &GraphElem)
{
  UndoList.BeginMultiUndo();
  if(dynamic_cast<const TBaseFuncType*>(GraphElem.get()))
    //Delete all shades bound to the function
    for(unsigned I = 0; I < Data.ElemCount(); I++)
      for(unsigned J = 0; J < Data.GetElem(I)->ChildList.size(); J++)
        if(boost::shared_ptr<TShade> Shade = boost::dynamic_pointer_cast<TShade>(Data.GetElem(I)->ChildList[J]))
          if(Shade->Func2 == GraphElem)
          {
            UndoList.Push(TUndoDel(Shade, I));
            Data.Delete(Shade);
          }

  GraphElem->ClearCache();
  UndoList.Push(TUndoDel(GraphElem, Data.GetIndex(GraphElem)));
  UndoList.EndMultiUndo();

  Data.Delete(GraphElem);
  UpdateTreeView();
  UpdateMenu();
  Data.SetModified();
  Redraw();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DeleteActionExecute(TObject *Sender)
{
  TTreeNode *Node = TreeView->Selected;
  if(MessageBox(LoadRes(RES_DELETE, Node->Text), LoadRes(RES_CAPTION_DELETE), MB_ICONQUESTION|MB_YESNO) == IDYES)
    DeleteGraphElem(GetGraphElem(Node));
}
//---------------------------------------------------------------------------
void __fastcall TForm1::InsertDifActionExecute(TObject *Sender)
{
  if(boost::shared_ptr<TBaseFuncType> Func = boost::dynamic_pointer_cast<TBaseFuncType>(GetGraphElem(TreeView->Selected)))
    if(CreateForm<TForm7>(Data)->InsertDif(Func) == mrOk)
    {
      UpdateTreeView();
      TreeView->Items->Item[TreeView->Items->Count-1]->Selected = true;
      Data.SetModified();
      Redraw();
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ZoomInActionExecute(TObject *Sender)
{
  //Zoom in; New window is 1/4 of the original
  Zoom(0.25, false);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ZoomOutActionExecute(TObject *Sender)
{
  //Zoom out; New window is the doubble of the original
  Zoom(1, false);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ZoomWindowActionExecute(TObject *Sender)
{
  SetCursorState(csZoomWindow);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ZoomStandardActionExecute(TObject *Sender)
{
  TAxis xAxis, yAxis;

  AnsiString Str = GetRegValue(REGISTRY_KEY, "DefaultAxes", HKEY_CURRENT_USER, "");
  TConfigFile ConfigFile;
  ConfigFile.LoadFromString(Str.c_str());
  xAxis.ReadFromIni(ConfigFile, "x");
  yAxis.ReadFromIni(ConfigFile, "y");

  xAxis.Label = Data.Axes.xAxis.Label;
  yAxis.Label = Data.Axes.yAxis.Label;

  Data.Axes.xAxis = xAxis;
  Data.Axes.yAxis = yAxis;

  Data.ClearCache();
  Data.SetModified();
  Redraw();
  if(!MoveAction->Checked)
    UpdateEval();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PathActionExecute(TObject *Sender)
{
  if(PathAction->Checked)
    Form9->SetEvalType(etArc);
  else
    Form9->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AreaActionExecute(TObject *Sender)
{
  if(AreaAction->Checked)
    Form9->SetEvalType(etArea);
  else
    Form9->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::EvalActionExecute(TObject *Sender)
{
  if(EvalAction->Checked)
  {
    Form9->SetEvalType(etEval);
    Form9->FuncChanged(GetGraphElem(TreeView->Selected));
  }
  else
    Form9->Visible = false;

  if(MoveAction->Checked)
    MoveAction->Execute();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TableActionExecute(TObject *Sender)
{
  if(TBaseFuncType *Func = dynamic_cast<TBaseFuncType*>(GetGraphElem(TreeView->Selected).get()))
    CreateForm<TForm15>()->ShowTable(Func);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ContentsActionExecute(TObject *Sender)
{
  Application->HelpCommand(HELP_FINDER,0);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ListActionExecute(TObject *Sender)
{
  Application->HelpJump("FUNCTIONS");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::HomePageActionExecute(TObject *Sender)
{
  ShellExecute(Handle, NULL, HOMEPAGE, NULL, NULL, SW_SHOWMAXIMIZED);
}
//---------------------------------------------------------------------------
void TForm1::CheckForUpdate(bool StartupCheck)
{
  unsigned long Flags;
  InternetGetConnectedState(&Flags, 0);
  if((Flags & INTERNET_CONNECTION_MODEM) && StartupCheck)
    return; //If user has a modem, he will be asked if he will connect to the Internet if we continue

  try
  {
    TVersionInfo Info;
    std::auto_ptr<TMemoryStream> Stream(new TMemoryStream);
    std::auto_ptr<TIdHTTP> IdHTTP(new TIdHTTP(NULL));

    IdHTTP->Request->UserAgent = ("Mozilla/3.0 (compatible; Graph " + Info.StringValue("ProductVersion") + ')').c_str();
    IdHTTP->HandleRedirects = true;

    AnsiString Url = GetRegValue(REGISTRY_KEY, "InfFile", HKEY_CURRENT_USER, INF_FILE);
    Url += AnsiString("?Version=") + AnsiString(Info.StringValue("ProductVersion").c_str());
    IdHTTP->Get(Url, Stream.get());

    Stream->Position = 0;
    std::auto_ptr<TStringList> StringList(new TStringList);
    StringList->LoadFromStream(Stream.get());
    std::auto_ptr<TMemIniFile> IniFile(new TMemIniFile(""));
    IniFile->SetStrings(StringList.get());
    int Major = IniFile->ReadInteger("Graph", "Major", 0);
    int Minor = IniFile->ReadInteger("Graph", "Minor", 0);
    int Release = IniFile->ReadInteger("Graph", "Release", 0);
    AnsiString DownloadPage = IniFile->ReadString("Graph", "DownloadPage", "http:\/\/www.padowan.dk");
    std::auto_ptr<TStringList> LanguageSection(new TStringList);

    AnsiString Section = IniFile->SectionExists(GetCurrentLanguage()) ? GetCurrentLanguage() : AnsiString("English");
    IniFile->ReadSectionValues(Section, LanguageSection.get());

    AnsiString NewFeatures;
    for(int I = 0; I < LanguageSection->Count; I++)
      if(LanguageSection->Names[I] == "NewFeatures")
      {
        AnsiString Line = LanguageSection->Strings[I];
        if(!NewFeatures.IsEmpty())
          NewFeatures += '\n';
        NewFeatures += StringReplace(Line.SubString(Line.Pos('=') + 1, 0x7FFFFFFF), "\\n", "\r\n", TReplaceFlags() << rfReplaceAll); //Replace all "\\n" with '\n'
      }

    //Check if a newer version is available, or if this is a debug version one with the same version
    if(Info.InfoAvailable())
    {
      TVersion Version = Info.ProductVersion();
      if(Major > Version.Major || (Major == Version.Major && (Minor > Version.Minor ||
        (Minor == Version.Minor && (Release > Version.Release ||
        (Release == Version.Release && (Info.FileFlags() & ffDebug)))))))
      {
        AnsiString VersionString = AnsiString(Major) + '.' + Minor;
        if(Release)
          VersionString += "." + AnsiString(Release);
        AnsiString Str = LoadRes(541, VersionString, NewFeatures);
        if(StartupCheck)
          Str += LoadRes(550);
        if(MessageBox(Str, LoadRes(540), MB_YESNO) == ID_YES)
          ShellExecute(Handle, NULL, DownloadPage.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);
      }
      else if(!StartupCheck)
        MessageBox(LoadRes(543), LoadRes(542), MB_ICONINFORMATION);
    }
  }
  catch(EIdSocketError &E)
  {
    if(!StartupCheck)
      MessageBox(LoadRes(544), LoadRes(545, E.LastError), MB_ICONSTOP);
  }
  catch(EIdProtocolReplyError &E)
  {
    if(!StartupCheck)
      MessageBox(LoadRes(546, E.Message), LoadRes(547, E.ReplyErrorCode), MB_ICONSTOP);
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::UpdateActionExecute(TObject *Sender)
{
  TWaitCursor Dummy;
  ShowStatusError(LoadRes(520), clGreen, 0);
  CheckForUpdate(false);
  CancelStatusError();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AboutActionExecute(TObject *Sender)
{
  ShowForm<TForm2>();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MoveRightActionExecute(TObject *Sender)
{
  const TAxes &Axes = Data.Axes;
  double StepSize = GetKeyState (VK_SHIFT) & 0x8000 ? 0.1 : 0.01;
  double MoveWidth=(Axes.xAxis.Max-Axes.xAxis.Min) * StepSize;
  if(Axes.xAxis.LogScl)
  {
    double xMul = std::pow(Axes.xAxis.Max/Axes.xAxis.Min, StepSize);
    ZoomWindow(Axes.xAxis.Min*xMul, Axes.xAxis.Max*xMul, Axes.yAxis.Min, Axes.yAxis.Max);
  }
  else
    ZoomWindow(Axes.xAxis.Min+MoveWidth, Axes.xAxis.Max+MoveWidth, Axes.yAxis.Min, Axes.yAxis.Max);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MoveLeftActionExecute(TObject *Sender)
{
  const TAxes &Axes = Data.Axes;
  double StepSize = GetKeyState (VK_SHIFT) & 0x8000 ? 0.1 : 0.01;
  double MoveWidth=(Axes.xAxis.Max - Axes.xAxis.Min) * StepSize;
  if(Axes.xAxis.LogScl)
  {
    double xDiv = std::pow(Axes.xAxis.Max/Axes.xAxis.Min, StepSize);
    ZoomWindow(Axes.xAxis.Min/xDiv, Axes.xAxis.Max/xDiv, Axes.yAxis.Min, Axes.yAxis.Max);
  }
  else
    ZoomWindow(Axes.xAxis.Min-MoveWidth, Axes.xAxis.Max-MoveWidth, Axes.yAxis.Min, Axes.yAxis.Max);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MoveUpActionExecute(TObject *Sender)
{
  const TAxes &Axes = Data.Axes;
  double StepSize = GetKeyState (VK_SHIFT) & 0x8000 ? 0.1 : 0.01;
  double MoveHeight=(Axes.yAxis.Max-Axes.yAxis.Min) * StepSize;
  if(Axes.yAxis.LogScl)
  {
    double yMul = std::pow(Axes.yAxis.Max/Axes.yAxis.Min, StepSize);
    ZoomWindow(Axes.xAxis.Min, Axes.xAxis.Max, Axes.yAxis.Min*yMul, Axes.yAxis.Max*yMul);
  }
  else
    ZoomWindow(Axes.xAxis.Min, Axes.xAxis.Max, Axes.yAxis.Min+MoveHeight, Axes.yAxis.Max+MoveHeight);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MoveDownActionExecute(TObject *Sender)
{
  const TAxes &Axes = Data.Axes;
  double StepSize = GetKeyState (VK_SHIFT) & 0x8000 ? 0.1 : 0.01;
  double MoveHeight=(Axes.yAxis.Max - Axes.yAxis.Min) * StepSize;
  if(Axes.yAxis.LogScl)
  {
    double yDiv = std::pow(Axes.yAxis.Max / Axes.yAxis.Min, StepSize);
    ZoomWindow(Axes.xAxis.Min, Axes.xAxis.Max, Axes.yAxis.Min / yDiv, Axes.yAxis.Max / yDiv);
  }
  else
    ZoomWindow(Axes.xAxis.Min, Axes.xAxis.Max, Axes.yAxis.Min - MoveHeight, Axes.yAxis.Max - MoveHeight);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SeparatorActionUpdate(TObject *Sender)
{
  for(int I = 0; I < ActionToolBar1->ActionClient->Items->Count;  I++)
  {
    TActionClientItem *Item = ActionToolBar1->ActionClient->Items->ActionClients[I];
    if(Item->Action == SeparatorAction)
    {
      Item->Action = NULL;
      Item->Caption = "-";
    }
  }
}
//---------------------------------------------------------------------------
void TForm1::CreateToolBar(AnsiString Str)
{
  int Pos;
  AnsiString LastName;
  ActionToolBar1->ActionClient->Items->Clear();
  while((Pos = Str.Pos(',')) != 0)
  {
    AnsiString ButtonName = Str.SubString(1, Pos - 1);
    Str.Delete(1, Pos);
    if(ButtonName == '-')
    {
      if(LastName != '-')
        ActionToolBar1->ActionClient->Items->Add()->Action = SeparatorAction;
    }
    else
      for(int I = 0; I < ActionManager->ActionCount; I++)
        if(ButtonName == ActionManager->Actions[I]->Name)
        {
          TActionClientItem *Item = ActionToolBar1->ActionClient->Items->Add();
          Item->Action = ActionManager->Actions[I];
          break;
        }
    LastName = ButtonName;
  }
}
//---------------------------------------------------------------------------
AnsiString TForm1::GetToolBar()
{
  AnsiString Str;
  for(int I = 0; I < ActionToolBar1->ActionClient->Items->Count; I++)
  {
    TContainedAction *Action = ActionToolBar1->ActionClient->Items->ActionClients[I]->Action;
    if(Action == NULL)
      Str += "-,";
    else
      Str += Action->Name + ',';
  }
  return Str;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::MoveActionExecute(TObject *Sender)
{
  SetCursorState(MoveAction->Checked ? csMove : csIdle);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ToolBar_CustomizeClick(TObject *Sender)
{
  CustomizeDlg->Show();
  TCustomizeFrm *Form = CustomizeDlg->CustomizeForm;
  TranslateProperties(Form);
  Form->Tabs->Pages[1]->Caption = LoadRes(552);
  Form->ActionsActionsLbl->Caption = LoadRes(552) + AnsiString(':');
  Form->CatList->Items->Strings[Form->CatList->Items->Count-1] = LoadRes(553);
  Form->InfoLbl->Caption = LoadRes(554);
  Form->ActionsCatLbl->Caption = LoadRes(557);
  Form->ToolbarsTab->TabVisible = false;
  Form->OptionsTab->TabVisible = false;
  SetAccelerators(Form);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ToolBar_ResetClick(TObject *Sender)
{
  CreateToolBar(DefaultToolBar);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CustomizeDlgClose(TObject *Sender)
{
  UpdateMenu();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ApplicationEventsSettingChange(TObject *Sender,
      int Flag, const AnsiString Section, int &Result)
{
  //If local options has changed
  if(Flag == 0)
  {
    DecimalSeparator = '.';
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ZoomAllPointsActionExecute(TObject *Sender)
{
  TZoomFit ZoomFit(Data, Draw);
  bool Found = false;

  for(std::vector<boost::shared_ptr<TGraphElem> >::const_iterator Iter = Data.Begin();
    Iter != Data.End(); ++Iter)
  {
    TPointSeries *Series = dynamic_cast<TPointSeries*>(Iter->get());
    //Ignore non visible point series
    if(Series && Series->Visible)
    {
      Series->Accept(ZoomFit);
      Found = true;
    }
  }

  if(!Found)
    return; //Ignore if no Point series found

  double xMin = ZoomFit.xMin;
  double xMax = ZoomFit.xMax;
  double yMin = ZoomFit.yMin;
  double yMax = ZoomFit.yMax;

  double xMin2 = Data.Axes.xAxis.LogScl ? xMin * std::pow(xMin / xMax, 0.1) : xMin - (xMax - xMin) / 10;
  double xMax2 = Data.Axes.xAxis.LogScl ? xMax * std::pow(xMax / xMin, 0.1) : xMax + (xMax - xMin) / 10;
  double yMin2 = Data.Axes.yAxis.LogScl ? yMin * std::pow(yMin / yMax, 0.1) : yMin - (yMax - yMin) / 10;
  double yMax2 = Data.Axes.yAxis.LogScl ? yMax * std::pow(yMax / yMin, 0.1) : yMax + (yMax - yMin) / 10;

  ZoomWindow(xMin2, xMax2, yMin2, yMax2);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FaqActionExecute(TObject *Sender)
{
  Application->HelpJump("FAQ");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Tree_VisibleClick(TObject *Sender)
{
  boost::shared_ptr<TGraphElem> GraphElem = GetGraphElem(TreeView->Selected);
  if(GraphElem)
  {
    GraphElem->Visible = Tree_Visible->Checked;
    Data.SetModified();
    Redraw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Panel4DockDrop(TObject *Sender,
      TDragDockObject *Source, int X, int Y)
{
  Panel1->Height = Form9->VisibleFrame ? Form9->VisibleFrame->Height+10 : 100;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Panel4UnDock(TObject *Sender, TControl *Client,
      TWinControl *NewTarget, bool &Allow)
{
  Panel1->Height = 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Panel4DockOver(TObject *Sender,
      TDragDockObject *Source, int X, int Y, TDragState State,
      bool &Accept)
{
  // Modify the DockRect to preview dock area.
  int FormHeight = Form9->VisibleFrame ? Form9->VisibleFrame->Height+10 : 100;
  TPoint TopLeft = Panel1->ClientToScreen(TPoint(0, -FormHeight));
  TPoint BottomRight = Panel1->ClientToScreen(TPoint(Panel1->Width, Panel1->Height));
  Source->DockRect = TRect(TopLeft, BottomRight);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ZoomFitActionExecute(TObject *Sender)
{
  if(!TreeView->Selected)
    return;
  boost::shared_ptr<TGraphElem> Elem = GetGraphElem(TreeView->Selected);

  TZoomFit ZoomFit(Data, Draw);
  Elem->Accept(ZoomFit);

  double xMin = ZoomFit.xMin;
  double xMax = ZoomFit.xMax;
  double yMin = ZoomFit.yMin;
  double yMax = ZoomFit.yMax;

  if(_finite(xMin) && _finite(xMax) && _finite(yMin) && _finite(yMax))
  {
    double xMin2 = Data.Axes.xAxis.LogScl ? xMin * std::pow(xMin / xMax, 0.1) : xMin - (xMax - xMin) / 10;
    double xMax2 = Data.Axes.xAxis.LogScl ? xMax * std::pow(xMax / xMin, 0.1) : xMax + (xMax - xMin) / 10;
    double yMin2 = Data.Axes.yAxis.LogScl ? yMin * std::pow(yMin / yMax, 0.1) : yMin - (yMax - yMin) / 10;
    double yMax2 = Data.Axes.yAxis.LogScl ? yMax * std::pow(yMax / yMin, 0.1) : yMax + (yMax - yMin) / 10;

    ZoomWindow(xMin2, xMax2, yMin2, yMax2);
  }
}
//---------------------------------------------------------------------------
//Workaround for bug in VCL
//Without this, a menu item is sometimes shown in the status bar when it is double buffered,
//See http://groups.google.com/groups?hl=en&lr=&ie=UTF-8&oe=UTF-8&selm=3caf8e6a_1%40dnews&rnum=4
void __fastcall TForm1::WndProc(Messages::TMessage &Message)
{
  if(Message.Msg == WM_DRAWITEM)
    if(StatusBar1->Handle == reinterpret_cast<HWND>(Message.WParam))
      reinterpret_cast<DRAWITEMSTRUCT*>(Message.LParam)->CtlType = ODT_STATIC;
  TForm::WndProc(Message);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ZoomSquareActionExecute(TObject *Sender)
{
  TAxes &Axes = Data.Axes;
  if(Axes.xAxis.LogScl != Axes.yAxis.LogScl)
    return; //Invalid

  UndoList.Push(TUndoAxes());
  Axes.ZoomSquare = !Axes.ZoomSquare;

  //Don't save undo info. We have already done that
  if(Axes.ZoomSquare)
    ZoomWindow(Axes.xAxis.Min, Axes.xAxis.Max, Axes.yAxis.Min, Axes.yAxis.Max, true, false);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Panel2KeyPress(TObject *Sender, char &Key)
{
  switch(Key)
  {
    case '-': ZoomOutAction->Execute(); break;
    case '+': ZoomInAction->Execute(); break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Tree_ShowInLegendClick(TObject *Sender)
{
  if(TreeView->Selected)
  {
    boost::shared_ptr<TGraphElem> GraphElem = GetGraphElem(TreeView->Selected);
    GraphElem->ShowInLegend = Tree_ShowInLegend->Checked;
    Data.SetModified();
    Redraw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::InsertLabelActionExecute(TObject *Sender)
{
  SetCursorState(csAddLabel);
}
//---------------------------------------------------------------------------
void TForm1::SetCursorState(TCursorState State)
{
  if(State != csMove && State != csMoving)
    MoveAction->Checked = false;
  if(State != csZoomWindow)
    ZoomWindowAction->Checked = false;
  switch(State)
  {
    case csIdle:
    {
      MoveAction->Checked = false;
      Shape1->Visible = false;
      if(CursorState != csIdle)
        StatusBar1->Panels->Items[0]->Text = ""; //Only if state has changed
      TPoint Pos = Image1->ScreenToClient(Mouse->CursorPos);
      if(!!Data.FindLabel(Pos.x, Pos.y) || Draw.InsideLegend(Pos.x, Pos.y))
        Panel2->Cursor = crSizeAll;
      else
        Panel2->Cursor = crDefault;
      break;
    }

    case csAddLabel:
      Panel2->Cursor = crIBeam;
      break;

    case csMoveLabel:
    case csMoveLegend:
      Panel2->Cursor = crSizeAll;
      break;

    case csZoomWindow:
      Panel2->Cursor = crCross;
      break;

    case csMove:
      Panel2->Cursor = crMoveHand1;
      break;

    case csMoving:
      Panel2->Cursor = crMoveHand2;
      break;
  }

  CursorState = State;

  //Necesarry, else cursor is not changed when captured; It will also change the cursor instantly
  if(Screen->Cursor == crDefault)
    ::SetCursor(Screen->Cursors[Panel2->Cursor]);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Image1DblClick(TObject *Sender)
{
  EditLabel(Data.FindLabel(LastMousePos.x, LastMousePos.y));
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Label_EditClick(TObject *Sender)
{
  EditLabel(Data.FindLabel(LastMousePos.x, LastMousePos.y));
}
//---------------------------------------------------------------------------
void TForm1::EditLabel(const boost::shared_ptr<TTextLabel> &Label)
{
  if(Label)
  {
    std::auto_ptr<TForm6> Form6(new TForm6(Application, Data.Property.DefaultLabelFont, NAME, Data.GetFileName().c_str()));
    Form6->SetText(Label->GetText().c_str());
    Form6->SetBackgroundColor(Label->GetBackgroundColor());
    if(Form6->ShowModal() == mrOk)
    {
      //If text is empty, remove label
      if(Form6->IsEmpty())
      {
        UndoList.Push(TUndoDel(Label, Data.GetIndex(Label)));
        Data.Delete(Label);
      }
      else
      {
        Data.Property.DefaultLabelFont = Form6->GetFont();
        UndoList.Push(TUndoChange(Label, Data.GetIndex(Label)));
        boost::shared_ptr<TTextLabel> NewLabel(new TTextLabel(Form6->GetText().c_str(), Label->GetPlacement(), Label->GetPos(), Form6->GetBackgroundColor(), Label->GetRotation()));
        Data.Replace(Data.GetIndex(Label), NewLabel);
        NewLabel->Update();
      }
      UpdateTreeView();
      Data.SetModified();
      UpdateMenu();
      Redraw();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Label_DeleteClick(TObject *Sender)
{
  boost::shared_ptr<TTextLabel> Label = Data.FindLabel(LastMousePos.x, LastMousePos.y);
  if(Label)
  {
    UndoList.Push(TUndoDel(Label, Data.GetIndex(Label)));
    Data.Delete(Label);
    Data.SetModified();
    UpdateTreeView();
    Redraw();
    UpdateMenu();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DonateActionExecute(TObject *Sender)
{
  ShellExecute(Handle, NULL, DONATE_PAGE, NULL, NULL, SW_SHOWMAXIMIZED);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TipActionExecute(TObject *Sender)
{
  ShowForm<TForm10>(Data.Property);
}
//---------------------------------------------------------------------------
//Called when message loop is up and running
void __fastcall TForm1::WMUser(TMessage &Message)
{
  if(Data.Property.ShowTipsAtStartup)
    ShowForm<TForm10>(Data.Property);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  switch(Key)
  {
    case VK_SHIFT:
      if(CursorState == csMove || CursorState == csMoving)
        if(!MoveAction->Checked)
          SetCursorState(csIdle);
      break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SaveCopyAsActionExecute(TObject *Sender)
{
  DoSave("", false);
}
//---------------------------------------------------------------------------
//Disables menu items that shouldn't be available in the OLE server
void TForm1::ActivateOleUserInterface()
{
  NewAction->Enabled = false;
  OpenAction->Enabled = false;
  if(Data.GetFileName().empty())
    SaveAction->Enabled = false;
  SaveAsAction->Enabled = false;
  File_SaveCopyAs->Visible = true;
  Recent1->Enabled = false;
}
//---------------------------------------------------------------------------
TSaveError SaveAsPdf(const AnsiString &FileName, Graphics::TBitmap *Bitmap, const std::string &Title, const std::string &Subject, TPrinterOrientation Orientation)
{
  std::ostringstream Stream;
  Bitmap->PixelFormat = pf8bit;
  SaveBitmapToPngStream(Bitmap->Handle, Stream);
  std::string Str = Stream.str();
  double Width = Orientation == poPortrait ? a4_width : a4_height;
  double Height = Orientation == poPortrait ? a4_height : a4_width;

  try
  {
    PDFlib p;
    if (p.begin_document(FileName.c_str(), "") == -1)
      return seFileAccess;

    std::string Creator = NAME " " + TVersionInfo().StringValue("ProductVersion");
    p.set_info("Creator", Creator);
    p.set_info("Author", Creator);
    p.set_info("Title", Title); 
    p.set_info("Subject", Subject);
    p.begin_page_ext(Width, Height, "");
    int image;
    p.create_pvf("/pvf/image/Temp.png" , Str.data(), Str.size(), "");
    if((image = p.load_image("auto", "/pvf/image/Temp.png", "")) == -1)
    {
      MessageBox("Error: Couldn't read image file.", "PDF error");
      return sePdfError;
    }

    p.fit_image(image, Width*0.05, Height*0.05, "boxsize {" + ToString(Width*0.9) + " " + ToString(Height*0.9) + "} position 50 fitmethod meet");
    p.close_image(image);
    p.end_page_ext("");
    p.end_document("");
    return seNoError;
  }
  catch(EDllLoadError&)
  {
    return seDllError;
  }
  catch(PDFlib::Exception &ex)
  {
    MessageBox("PDFlib exception: [" + ToString(ex.get_errnum()) + "] " + ex.get_apiname() + ": " + ex.get_errmsg(), "PDFlib exception");
    return sePdfError;
  }
}

TSaveError TForm1::SaveAsImage(const AnsiString &FileName, int ImageFileType)
{
  try
  {
    bool SameSize = !ImageOptions->UseCustomSize;

    //Show save icon in status bar
    SetStatusIcon(iiSave);
    TCallOnRelease Dummy(&SetStatusIcon, -1);
    Draw.Wait();

    if(ImageFileType == ifMetafile/* || ImageFileType == ifPostScript*/)
    {
      std::auto_ptr<TMetafile> Metafile(new TMetafile);
      Metafile->Width = ImageOptions->CustomWidth;
      Metafile->Height = ImageOptions->CustomHeight;

      std::auto_ptr<TMetafileCanvas> Meta(new TMetafileCanvas(Metafile.get(), 0));
      TData MetaData(Data);
      TDraw FileDraw(Meta.get(), SameSize ? &Data : &MetaData, false, "Metafile DrawThread");
      FileDraw.SetArea(TRect(0, 0, ImageOptions->CustomWidth, ImageOptions->CustomHeight));  //Set drawing area

      FileDraw.DrawAll();
      if(SameSize)
        FileDraw.Wait(); //Wait if using Data object
      else
        while(FileDraw.Updating())
        { //Process messages while waiting for draw thread to finish
          Sleep(100);
          Application->ProcessMessages();
        }
      Meta.reset();
/*      if(ImageFileType == ifPostScript)
      {
        TEmfToEps EmfToEps(reinterpret_cast<HENHMETAFILE>(Metafile->Handle));
        EmfToEps.SaveToFile(FileName.c_str());
      }
      else*/
        Metafile->SaveToFile(FileName);
    }
    else
    {
      std::auto_ptr<Graphics::TBitmap> Bitmap(new Graphics::TBitmap);
      Bitmap->Width = ImageOptions->CustomWidth;
      Bitmap->Height = ImageOptions->CustomHeight;
      TRect Rect(0, 0, ImageOptions->CustomWidth, ImageOptions->CustomHeight);
      if(SameSize)
        Bitmap->Canvas->CopyRect(Rect, Image1->Picture->Bitmap->Canvas, Rect);
      else
      {
        //Make sure background is drawn
        Bitmap->Canvas->Brush->Style = bsSolid;
        Bitmap->Canvas->Brush->Color = Data.Axes.BackgroundColor;
        Bitmap->Canvas->FillRect(Rect);

        TData FileData(Data);
        TDraw FileDraw(Bitmap->Canvas, &FileData, false, "Save as image DrawThread");
        FileDraw.SetArea(Rect);  //Set drawing area
        FileDraw.DrawAll();
        while(FileDraw.Updating())
        {
          Sleep(100);
          Application->ProcessMessages();
        }
      }

      switch(ImageFileType)
      {
        case ifBitmap:
          SaveCompressedBitmap(Bitmap.get(), Rect, FileName);
          break;

        case ifPng:
          //Warning: There seems to be a bug in BMGlib that may crash the program if we do not use 8 bit bitmaps.
          //But there is no reason to use a higher color depth anyway.
          Bitmap->PixelFormat = pf8bit; //Change bitmap to 8 bit
          return SaveBitmapToPNGFile(Bitmap->Handle, FileName.c_str()) == 0 ? seNoError : seFileAccess;

        case ifJpeg:
        {
          std::auto_ptr<TJPEGImage> Image(new TJPEGImage);
          Image->Assign(Bitmap.get());
          Image->CompressionQuality = ImageOptions->Jpeg.Quality;
          Image->ProgressiveEncoding = ImageOptions->Jpeg.ProgressiveEncoding;
          Image->Compress();
          Image->SaveToFile(FileName);
          break;
        }

        case ifPdf:
          return SaveAsPdf(FileName, Bitmap.get(), Data.GetFileName(), ToString(Data.Axes.Title), ImageOptions->Pdf.Orientation);
      }
    }
  }
  catch(EOutOfResources &E)
  {
    return seOutOfResources;
  }
  catch(...)
  {
    return seFileAccess;
  }
  return seNoError;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TreeViewKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  TTreeNode *Node = TreeView->Selected;
  if(Node && Shift.Contains(ssAlt))
    switch(Key)
    {
      //Move selected item up
      case VK_UP:
      {
        Key = 0;
        TTreeNode *PrevNode = Node->getPrevSibling();
        if(!PrevNode)
          return; //Node already at top

        TGraphElemPtr Elem = GetGraphElem(Node);
        UndoList.Push(TUndoMove(Elem, Node->Index));
        Data.Delete(Elem);
        Data.Insert(Elem, PrevNode->Index);
        UpdateTreeView(GetGraphElem(PrevNode));
        Redraw();
        break;
      }

      //Move selected item down
      case VK_DOWN:
      {
        Key = 0;
        TTreeNode *NextNode = Node->getNextSibling();
        if(!NextNode)
          return; //Node already at bottom

        TGraphElemPtr Elem = GetGraphElem(Node);
        UndoList.Push(TUndoMove(Elem, Node->Index));
        Data.Delete(Elem);
        Data.Insert(Elem, NextNode->Index);
        UpdateTreeView(GetGraphElem(NextNode));
        Redraw();
        break;
      }
    }
    else if(Shift.Contains(ssCtrl))
      switch(Key)
      {
        case 'C':
          CopyAction->Execute();
          break;

        case 'X':
          CutAction->Execute();
          break;

        case 'V':
          PasteAction->Execute();
          break;
      }
}
//---------------------------------------------------------------------------
bool TForm1::LoadFromFile(const AnsiString &FileName, bool AddToRecent, bool ShowErrorMessages)
{
  Draw.AbortUpdate();
  if(!Data.LoadFromFile(FileName.c_str(), ShowErrorMessages))
  {
    //Parts of Data might have changed. Better change this to Strong guarantee!!
    UpdateTreeView();
    return false;
  }

  UpdateTreeView();
  Caption = AnsiString(NAME) + " - " + FileName;
  if(AddToRecent)
    Recent1->FileUsed(FileName);
  UndoList.Clear();

  Application->Title = AnsiString(NAME) + " - " + ExtractFileName(FileName);
  UpdateMenu();
  return true;
}
//---------------------------------------------------------------------------
bool __fastcall TForm1::OpenPreviewDialog1PreviewFile(
      TOpenPreviewDialog *Dialog, const AnsiString &FileName,
      TCanvas *Canvas, const TRect &Rect)
{
  PreviewDraw->AbortUpdate();

  TConfigFile IniFile;
  if(IniFile.LoadFromFile(FileName.c_str()))
  {
    std::string PngStr = IniFile.Read("Image", "Png", "");
    if(!PngStr.empty())
    {
      std::vector<BYTE> Data((PngStr.size()*3)/4);
      Base64Decode(PngStr, &Data[0]);
      std::auto_ptr<Graphics::TBitmap> Bitmap(new Graphics::TBitmap);
      Bitmap->Handle = CreateBitmapFromPNGMemory(&Data[0], Data.size());
      if(Bitmap->Handle != NULL)
      {
        Canvas->CopyRect(Rect, Bitmap->Canvas, TRect(0, 0, Bitmap->Width, Bitmap->Height));
        return true;
      }
    }
  }

  //Make sure background is drawn
  Canvas->Brush->Style = bsSolid;
  //Set background color
  Canvas->Brush->Color = PreviewData.Axes.BackgroundColor;
  //Clear area
  Canvas->FillRect(Rect);

  if(PreviewData.LoadFromFile(FileName.c_str(), false) && !!PreviewDraw)
  {
    PreviewData.Axes.ShowLegend = false; //Always disable legend for preview

    //Make sure background is drawn
    Canvas->Brush->Style = bsSolid;
    //Set background color
    Canvas->Brush->Color = PreviewData.Axes.BackgroundColor;
    //Clear area
    Canvas->FillRect(Rect);

    PreviewDraw->SetCanvas(Canvas);
    PreviewDraw->SetSize(Rect.Width(), Rect.Height());
    PreviewDraw->DrawAll();
  }
  return true;
}
//---------------------------------------------------------------------------
void TForm1::LoadDefault()
{
  std::auto_ptr<TRegistry> Registry(new TRegistry);
  AnsiString Str;
  if(Registry->OpenKeyReadOnly(REGISTRY_KEY))
    if(Registry->ValueExists("DefaultAxes"))
      Str = Registry->ReadString("DefaultAxes");

  if(Str.IsEmpty() || !Data.LoadFromString(Str.c_str()))
    Data.LoadDefault();

  UndoList.Clear();
  UpdateTreeView();
  Caption = NAME;
  Application->Title = NAME;
  UpdateMenu();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OpenPreviewDialog1Show(TObject *Sender)
{
  PreviewDraw.reset(new TDraw(NULL, &PreviewData, false, "Preview DrawThread"));
}
//---------------------------------------------------------------------------
void __fastcall TForm1::OpenPreviewDialog1Close(TObject *Sender)
{
  PreviewDraw.reset();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Tree_ExportClick(TObject *Sender)
{
  if(!TreeView->Selected)
    return;

  if(TPointSeries *Series = dynamic_cast<TPointSeries*>(GetGraphElem(TreeView->Selected).get()))
  {
    SaveDialog->Filter = LoadRes(RES_EXPORT_DATA_FILTER);
    if(SaveDialog->Execute())
      if(!ExportPointSeries(Series, SaveDialog->FileName.c_str(), SaveDialog->FilterIndex == 1 ? ';' : '\t'))
        MessageBox(LoadRes(RES_FILE_ACCESS, SaveDialog->FileName), LoadRes(RES_WRITE_FAILED), MB_ICONSTOP);
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::IPrintDialog1PaintSample(TIPrintDialog *Sender,
      TCanvas *PageCanvas, const TRect &PaintRect, const TRect &PaperSize)
{
  int x = (PaintRect.Right + PaintRect.Left) / 2;
  int y = (PaintRect.Bottom + PaintRect.Top) / 2;
  PageCanvas->Pen->Width = 2;
  PageCanvas->Pen->Color = clBlue;
  PageCanvas->MoveTo(x, PaintRect.Top);
  PageCanvas->LineTo(x, PaintRect.Bottom);
  PageCanvas->MoveTo(PaintRect.Left, y);
  PageCanvas->LineTo(PaintRect.Right, y);

  PageCanvas->MoveTo(PaintRect.Right-7, y-7);
  PageCanvas->LineTo(PaintRect.Right, y);
  PageCanvas->LineTo(PaintRect.Right-7, y+7);
  PageCanvas->MoveTo(x-7, PaintRect.Top+7);
  PageCanvas->LineTo(x, PaintRect.Top);
  PageCanvas->LineTo(x+7, PaintRect.Top+7);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::IPrintDialog1Show(TObject *Sender)
{
  if(IPrintDialog1->PrintForm)
  {
    TPrintFrm *Form = IPrintDialog1->PrintForm;
    ScaleForm(Form);
    TranslateProperties(Form);
    SetAccelerators(Form);
  }
}
//---------------------------------------------------------------------------
boost::shared_ptr<TGraphElem> TForm1::GetGraphElem(TTreeNode *Node)
{
  if(Node == NULL)
    return boost::shared_ptr<TGraphElem>();

  if(Node->Level == 1) //Tangent or shade
    return Data.GetElem(Node->Parent->Index)->ChildList[Node->Index];
  return Data.GetElem(Node->Index);
}
//---------------------------------------------------------------------------
TTntTreeNode* TForm1::GetRootNode(unsigned Index)
{
  TTntTreeNode *Node = TreeView->Items->GetFirstNode();
  for(unsigned I = 0; I < Index; I++)
    Node = Node->getNextSibling();
  return Node;
}
//---------------------------------------------------------------------------
TTntTreeNode* TForm1::GetNode(const boost::shared_ptr<const TGraphElem> &Elem)
{
  //WARNING: TreeView->Items->Item conatins *ALL* nodes, not just the root nodes
  if(!Elem)
    throw Exception("No element given");
  boost::shared_ptr<TBaseFuncType> Parent = Elem->ParentFunc();
  if(Parent)
    return GetRootNode(Data.GetIndex(Parent))->Item[Data.GetIndex(Elem)];
  return GetRootNode(Data.GetIndex(Elem));
}
//---------------------------------------------------------------------------
//Copy metafile and bitmap to clipboard; Only used when not possible to register
//as OLE server
void TForm1::CopyAsImageToClipboard()
{
  Clipboard()->Open();

  //Copy metafile to clipboard
  std::auto_ptr<TMetafile> Metafile(new TMetafile());
  Metafile->Width = Image1->Width;
  Metafile->Height = Image1->Height;
  std::auto_ptr<TMetafileCanvas> MetaCanvas(new TMetafileCanvas(Metafile.get(), 0));
  TDraw DrawMeta(MetaCanvas.get(), &Data, false, "Metafile DrawThread");

  //Set width and height
  DrawMeta.SetSize(Image1->Width, Image1->Height);
  DrawMeta.DrawAll();
  DrawMeta.Wait();
  MetaCanvas.reset();
  Clipboard()->Assign(Metafile.get());

  //Copy bitmap to clipboard
  std::auto_ptr<Graphics::TBitmap> Bitmap(new Graphics::TBitmap);
  Bitmap->Width = Image1->Width;
  Bitmap->Height = Image1->Height;
  Bitmap->Canvas->CopyRect(TRect(0,0,Image1->Width,Image1->Height), Image1->Canvas,Image1->ClientRect);
  Clipboard()->Assign(Bitmap.get());

  std::stringstream Stream;
  Bitmap->PixelFormat = pf8bit; //Change bitmap to 8 bit
  if(SaveBitmapToPngStream(Bitmap->Handle, Stream))
    GraphClipboard.CopyPngData(Stream.str());

  Clipboard()->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TreeViewDragOver(TObject *Sender, TObject *Source,
      int X, int Y, TDragState State, bool &Accept)
{
  Accept = false;
  TTreeNode *Node= TreeView->GetNodeAt(X, Y);
  if(Node && TreeView->Selected)
    Accept = Node->Parent == TreeView->Selected->Parent;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TreeViewDragDrop(TObject *Sender, TObject *Source,
      int X, int Y)
{
  if(TreeView->Selected)
  {
    TTreeNode *Node = TreeView->GetNodeAt(X, Y);
    boost::shared_ptr<TGraphElem> Elem = GetGraphElem(TreeView->Selected);
    Draw.AbortUpdate();
    UndoList.Push(TUndoMove(Elem, Data.GetIndex(Elem)));
    Data.Delete(Elem);
    Data.Insert(Elem, Node->Index);
    UpdateTreeView(Elem);
    Redraw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::LegendPlacementClick(TObject *Sender)
{
  if(TMenuItem *MenuItem = dynamic_cast<TMenuItem*>(Sender))
  {
    MenuItem->Checked = true;
    Data.Axes.LegendPlacement = static_cast<TLegendPlacement>(MenuItem->MenuIndex + 1);
    Data.SetModified();
    Redraw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Legend_ShowClick(TObject *Sender)
{
  Data.Axes.ShowLegend = Legend_Show->Checked;
  Data.SetModified();
  Redraw();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PopupMenu4Popup(TObject *Sender)
{
  Legend_Show->Checked = Data.Axes.ShowLegend;

  if(Data.Axes.LegendPlacement > lpCustom && Data.Axes.LegendPlacement <= Legend_Placement->Count)
    Legend_Placement->Items[Data.Axes.LegendPlacement - 1]->Checked = true;
  else //Remove all check marks
    for(int I = 0; I < Legend_Placement->Count; I++)
      Legend_Placement->Items[I]->Checked = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SupportActionExecute(TObject *Sender)
{
  ShellExecute(Handle, NULL, SUPPORT_PAGE, NULL, NULL, SW_SHOWMAXIMIZED);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ZoomXInActionExecute(TObject *Sender)
{
  //Zoom in on x-axis only
  Zoom(0.25, 0.5);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ZoomXOutActionExecute(TObject *Sender)
{
  //Zoom out on x-axis only
  Zoom(1, 0.5);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ZoomYInActionExecute(TObject *Sender)
{
  //Zoom in on y-axis only
  Zoom(0.5, 0.25);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ZoomYOutActionExecute(TObject *Sender)
{
  //Zoom out on y-axis only
  Zoom(0.5, 1);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::InsertRelationActionExecute(TObject *Sender)
{
  if(CreateForm<TForm11>(Data)->ShowModal() == mrOk)
  {
    TreeView->SetFocus();
    UpdateTreeView(Data.Back());
    UpdateMenu();
    Redraw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::CustomFunctionsActionExecute(TObject *Sender)
{
  CreateForm<TForm17>(Data)->ShowModal();
  UpdateMenu();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::SaveDialogEx1Help(TObject *Sender)
{
  CreateForm<TForm18>()->EditOptions(*ImageOptions, static_cast<TImageFormat>(SaveDialogEx1->FilterIndex), Image1->Width, Image1->Height);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::LabelPlacementClick(TObject *Sender)
{
  if(TMenuItem *MenuItem = dynamic_cast<TMenuItem*>(Sender))
  {
    MenuItem->Checked = true;
    Draw.AbortUpdate();
    boost::shared_ptr<TTextLabel> TextLabel = Data.FindLabel(LastMousePos.x, LastMousePos.y);
    if(!TextLabel)
      return;

    UndoList.Push(TUndoChange(TextLabel, Data.GetIndex(TextLabel)));
    boost::shared_ptr<TTextLabel> NewLabel(new TTextLabel(TextLabel->GetText(), static_cast<TLabelPlacement>(MenuItem->MenuIndex + 1), Func32::MakeCoord(0.0, 0.0), TextLabel->GetBackgroundColor(), TextLabel->GetRotation()));
    Data.Replace(Data.GetIndex(TextLabel), NewLabel);
    NewLabel->Update();

    Data.SetModified();
    UpdateMenu();
    Redraw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PopupMenu3Popup(TObject *Sender)
{
  TPoint Pos = Image1->ScreenToClient(PopupMenu3->PopupPoint);
  if(boost::shared_ptr<TTextLabel> TextLabel = Data.FindLabel(Pos.x, Pos.y))
  {
    if(TextLabel->GetPlacement() > lpUserPos && TextLabel->GetPlacement() <= Label_Placement->Count)
      Label_Placement->Items[TextLabel->GetPlacement() - 1]->Checked = true;
    else //Remove all check marks
      for(int I = 0; I < Label_Placement->Count; I++)
        Label_Placement->Items[I]->Checked = false;

    if(TextLabel->GetRotation() % 90 == 0)
      Label_Rotation->Items[TextLabel->GetRotation() / 90]->Checked = true;
    else
      Label_Rotation->Items[4]->Checked = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PopupMenu1Popup(TObject *Sender)
{
  //Warning: RadioItem does not work when Images are assigned
  if(boost::shared_ptr<TTextLabel> TextLabel = boost::dynamic_pointer_cast<TTextLabel>(GetGraphElem(TreeView->Selected)))
  {
    for(int I = 0; I < Tree_Placement->Count; I++)
      if(TextLabel->GetPlacement() == I + 1)
        Tree_Placement->Items[I]->ImageIndex = iiBullet;
      else //Remove check mark
        Tree_Placement->Items[I]->ImageIndex = -1;

    if(TextLabel->GetRotation() % 90 == 0)
      Tree_Rotation->Items[TextLabel->GetRotation() / 90]->Checked = true;
    else
      Tree_Rotation->Items[4]->Checked = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Tree_LabelPlacementClick(TObject *Sender)
{
  //Warning RadioItem doesn't work when Images is assigned
  if(TMenuItem *MenuItem = dynamic_cast<TMenuItem*>(Sender))
  {
    Draw.AbortUpdate();
    boost::shared_ptr<TTextLabel> TextLabel = boost::dynamic_pointer_cast<TTextLabel>(GetGraphElem(TreeView->Selected));
    if(!TextLabel)
      return;

    UndoList.Push(TUndoChange(TextLabel, Data.GetIndex(TextLabel)));
    boost::shared_ptr<TTextLabel> NewLabel(new TTextLabel(TextLabel->GetText(), static_cast<TLabelPlacement>(MenuItem->MenuIndex + 1), Func32::MakeCoord(0.0, 0.0), TextLabel->GetBackgroundColor(), TextLabel->GetRotation()));
    Data.Replace(Data.GetIndex(TextLabel), NewLabel);
    NewLabel->Update();

    Data.SetModified();
    UpdateMenu();
    Redraw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ApplicationEventsException(TObject *Sender,
      Exception *E)
{
  LogUncaughtException(Sender, E);
}
//---------------------------------------------------------------------------
void TForm1::MoveAndSnapLegend(int dx, int dy, bool Snap)
{
  ImagePos.x += dx;
  ImagePos.y += dy;
  Image2->Left = ImagePos.x;
  Image2->Top = ImagePos.y;
  unsigned SnapDist = 10;
  const TRect &Rect = Draw.GetAxesRect();

  if(!Snap)
  {
    LegendPlacement = lpCustom;
    Image2->Left = ImagePos.x;
    Image2->Top = ImagePos.y;
    return;
  }

  unsigned LeftDist = std::abs(Image2->Left - (int)Rect.Left);
  unsigned TopDist = std::abs(Image2->Top - (int)Rect.Top);
  unsigned RightDist = std::abs(Image2->Left + Image2->Width - (int)Rect.Right);
  unsigned BottomDist = std::abs(Image2->Top + Image2->Height - (int)Rect.Bottom);

  //Check for snap to upper left corner
  if(LeftDist < SnapDist && TopDist < SnapDist)
  {
    Image2->Left = Rect.Left;
    Image2->Top = Rect.Top;
    LegendPlacement = lpTopLeft;
  }

  //Check for snap to upper right corner
  else if(RightDist < SnapDist && TopDist < SnapDist)
  {
    Image2->Left = Rect.Right - Image2->Width;
    Image2->Top = Rect.Top;
    LegendPlacement = lpTopRight;
  }

  //Check for snap to lower left corner
  else if(LeftDist < SnapDist && BottomDist < SnapDist)
  {
    Image2->Left = Rect.Left;
    Image2->Top = Rect.Bottom - Image2->Height;
    LegendPlacement = lpBottomLeft;
  }

  //Check for snap to lower right corner
  else if(RightDist < SnapDist && BottomDist < SnapDist)
  {
    Image2->Left = Rect.Right - Image2->Width;
    Image2->Top = Rect.Bottom - Image2->Height;
    LegendPlacement = lpBottomRight;
  }

  // Legend has a custom position
  else
    LegendPlacement = lpCustom;
}
//---------------------------------------------------------------------------
void TForm1::MoveAndSnapLabel(int dx, int dy, bool Snap)
{
  ImagePos.x += dx;
  ImagePos.y += dy;
  Image2->Left = ImagePos.x;
  Image2->Top = ImagePos.y;
  int SnapDist = 15;
  const TRect &Rect = Draw.GetAxesRect();

  if(!Snap)
  {
    MovingLabelPlacement = lpUserPos;
    Image2->Left = ImagePos.x;
    Image2->Top = ImagePos.y;
    return;
  }

  int RightDist = std::abs(Image2->Left + Image2->Width - Rect.Width()) + 2;
  int TopDist = std::abs(Image2->Top - (int)Rect.Top + 1);
  int xAxesCoord = Draw.xPoint(Data.Axes.yAxis.AxisCross);
  int yAxesCoord = Draw.yPoint(Data.Axes.xAxis.AxisCross);

  //Check for label above x-axis
  if(RightDist < SnapDist && std::abs(Image2->Top + Image2->Height -4 - yAxesCoord) < SnapDist)
  {
    Image2->Left = Rect.Width() - Image2->Width + 2;
    Image2->Top = yAxesCoord - Image2->Height - 4;
    MovingLabelPlacement = lpAboveX;
  }

  //Check for label below x-axis
  else if(RightDist < SnapDist && std::abs(Image2->Top - yAxesCoord) < SnapDist)
  {
    Image2->Left = Rect.Width() - Image2->Width + 2;
    Image2->Top = yAxesCoord;
    MovingLabelPlacement = lpBelowX;
  }

  //Check for label left of y-axis
  else if(TopDist < SnapDist && std::abs(Image2->Left + Image2->Width - xAxesCoord) < SnapDist)
  {
    Image2->Left = xAxesCoord - Image2->Width;
    Image2->Top = Rect.Top + 1;
    MovingLabelPlacement = lpLeftOfY;
  }

  //Check for label right of y-axis
  else if(TopDist < SnapDist && std::abs(Image2->Left + 12 - xAxesCoord) < SnapDist)
  {
    Image2->Left = xAxesCoord + 12;
    Image2->Top = Rect.Top + 1;
    MovingLabelPlacement = lpRightOfY;
  }
  else
  {
    //Snap to nearest point in point series
/*    unsigned MinDist = MAXINT;
    std::vector<TGraphElemPtr>::const_iterator End = Data.End();
    TPointSeries *MinSeries = NULL;
    TPoint MinPoint;
    for(std::vector<TGraphElemPtr>::const_iterator Iter = Data.Begin(); Iter != End; ++Iter)
      if(TPointSeries *Series = dynamic_cast<TPointSeries*>(Iter->get()))
        for(unsigned I = 0; I < Series->PointList.size(); I++)
        {
          TPoint Point;
          Point.x = Draw.xPoint(Series->PointList[I].x.Value);
          Point.y = Draw.yPoint(Series->PointList[I].y.Value);
          unsigned Dist = Sqr(ImagePos.x - Point.x) + Sqr(ImagePos.y - Point.y);
          if(Dist < MinDist)
          {
            MinDist = Dist;
            MinSeries = Series;
            MinPoint = Point;
          }
        }

    if(MinSeries && MinDist <= 150)
    {
      Image2->Left = MinPoint.x;
      Image2->Top = MinPoint.y;
    }*/

    MovingLabelPlacement = lpUserPos;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::RotationClick(TObject *Sender)
{
  if(TMenuItem *MenuItem = dynamic_cast<TMenuItem*>(Sender))
  {
    MenuItem->Checked = true;
    Draw.AbortUpdate();
    boost::shared_ptr<TTextLabel> TextLabel;
    if(MenuItem->Parent == Label_Rotation)
      TextLabel = Data.FindLabel(LastMousePos.x, LastMousePos.y);  //Right click on label menu
    else
      TextLabel = boost::dynamic_pointer_cast<TTextLabel>(GetGraphElem(TreeView->Selected)); //Context menu in function list

    if(!TextLabel)
      return;

    int Rotation = TextLabel->GetRotation();
    if(MenuItem->MenuIndex == 4)
    {
      if(!InputQuery(LoadStr(RES_ROTATION), LoadStr(RES_ROTATION) + ":", Rotation))
        return;
    }
    else
      Rotation = MenuItem->MenuIndex * 90;

    UndoList.Push(TUndoChange(TextLabel, Data.GetIndex(TextLabel)));
    boost::shared_ptr<TTextLabel> NewLabel(new TTextLabel(TextLabel->GetText(), TextLabel->GetPlacement(), TextLabel->GetPos(), TextLabel->GetBackgroundColor(), Rotation));
    Data.Replace(Data.GetIndex(TextLabel), NewLabel);
    NewLabel->Update();

    Data.SetModified();
    UpdateMenu();
    Redraw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::DebugLine(System::TObject* Sender, const AnsiString Line, bool &Discard)
{
  OutputDebugStringA(Line.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AnimateActionExecute(TObject *Sender)
{
  CreateForm<TForm19>(Data)->ShowModal();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ImportGraphFileActionExecute(TObject *Sender)
{
  OpenPreviewDialog1->Filter = LoadRes(RES_GRAPH_FILTER);
  if(OpenPreviewDialog1->Execute())
  {
    unsigned Count = Data.ElemCount();

    if(Data.Import(OpenPreviewDialog1->FileName.c_str()))
    {
      UndoList.BeginMultiUndo();
      for(unsigned I = Count; I < Data.ElemCount(); I++)
        UndoList.Push(TUndoAdd(Data.GetElem(I)));
      UndoList.EndMultiUndo();
      UpdateTreeView();
      UpdateMenu();
      Redraw();
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ImportPointSeriesActionExecute(TObject *Sender)
{
  OpenDialog->Filter = LoadRes(RES_DATA_FILTER);
  if(OpenDialog->Execute())
  {
    for(int I = 0; I < OpenDialog->Files->Count; I++)
      if(!Data.ImportData(OpenDialog->Files->Strings[I].c_str()))
        return;

    UpdateTreeView();
    UpdateMenu();
    Redraw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Legend_FontClick(TObject *Sender)
{
  std::auto_ptr<TFontDialog> FontDialog(new TFontDialog(NULL));
  FontDialog->Options = FontDialog->Options << fdForceFontExist;
  FontDialog->Font->Assign(Data.Axes.LegendFont);
  if(FontDialog->Execute())
  {
    Data.Axes.LegendFont->Assign(FontDialog->Font);
    Data.SetModified();
    Redraw();
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::InsertObjectActionExecute(TObject *Sender)
{
  Func32::TDblPoint Pos(Image1->Width/2, Image1->Height/2);
  boost::shared_ptr<TOleObjectElem> OleObject(new TOleObjectElem(Pos));
  if(!OleObject->InsertObjectDialog())
    return;

  UndoList.Push(TUndoAdd(OleObject));
  Data.Add(OleObject);
  UpdateTreeView();
  TreeView->Items->Item[TreeView->Items->Count-1]->Selected = true;
  Data.SetModified();
  Redraw();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ZoomSquareActionUpdate(TObject *Sender)
{
  ZoomSquareAction->Checked = Data.Axes.ZoomSquare;   
}
//---------------------------------------------------------------------------

