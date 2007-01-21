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
#include "GuiUtil.h"
#include "PointSelect.h"
#include <ValueEdit.hpp>
#include <set>
#include <TntControls.hpp>
#include "TntExtCtrls.hpp"
#include <boost/format.hpp>
#include <MultiMon.hpp>
//---------------------------------------------------------------------------
void DrawComboBoxEndPoint(TCanvas *Canvas, int Index, const TRect &Rect)
{
  //Clear drawing area
  Canvas->Brush->Color = clWhite;
  Canvas->FillRect(Rect);
  Canvas->Pen->Color = clRed;
  Canvas->Pen->Width = 2;

  TPoint Pos(Rect.Left/2 + Rect.Right/2, Rect.Top/2 + Rect.Bottom/2);
  switch(Index)
  {
    case 1:
      TPointSelect::DrawPoint(Canvas, Pos, 0, clRed, clWhite, 5);
      break;

    case 2:
      TPointSelect::DrawPoint(Canvas, Pos, 0, clRed, clRed, 5);
      break;

    case 3:
      Canvas->MoveTo(Pos.x + 6, Pos.y);
      Canvas->LineTo(Pos.x - 5, Pos.y - 6);
      Canvas->MoveTo(Pos.x + 6, Pos.y);
      Canvas->LineTo(Pos.x - 5, Pos.y + 6);
      break;

    case 4:
      Canvas->Arc(Pos.x - 11, Pos.y - 6, Pos.x + 5, Pos.y + 7, Pos.x - 3, Pos.y + 7, Pos.x - 3, Pos.y - 7);
      break;

    case 5:
      Canvas->Arc(Pos.x + 11, Pos.y - 6, Pos.x - 5, Pos.y + 7, Pos.x + 3, Pos.y - 7, Pos.x + 3, Pos.y + 7);
      break;
  }

  //This is necesarry to use white as brush before the VCL can draw the focus
  //rectangle correct; I don't know why.
  TRect Rect1(0,0,0,0);
  Canvas->Brush->Color = clWhite;
  Canvas->FillRect(Rect1);
}
//---------------------------------------------------------------------------
void ScaleComponent(TComponent *Component, unsigned Scale)
{
  if(TComboBox *ComboBox = dynamic_cast<TComboBox*>(Component))
    ComboBox->ItemHeight = (ComboBox->ItemHeight * Scale) / 100;
  else if(TColorBox *ColorBox = dynamic_cast<TColorBox*>(Component))
    ColorBox->ItemHeight = (ColorBox->ItemHeight * Scale) / 100;
  else if(TValueListEditor *Editor = dynamic_cast<TValueListEditor*>(Component))
    Editor->DefaultRowHeight = (Editor->DefaultRowHeight * Scale) / 100;
  for(int I = 0; I < Component->ComponentCount; I++)
    ScaleComponent(Component->Components[I], Scale);
}
//---------------------------------------------------------------------------
void ScaleForm(TForm *Form)
{
  //Change font for form. All components should have ParentFont=true
  Form->Font->Name = "MS Shell Dlg";

  //Set FontScale to 100 to disable scaling
  int FontScale = Form1->Data.Property.FontScale;
  if(FontScale != 100)
  {
    Form->ScaleBy(FontScale, 100);
    ScaleComponent(Form, FontScale);
  }
}
//---------------------------------------------------------------------------
void ResizeControl(TControl *Control, int Left)
{
  int Width = Control->Width - (Left - Control->Left);
  Control->Left = Left;
  Control->Width = Width;
}
//---------------------------------------------------------------------------
void ResizeControl(TControl *Control, TCustomLabel *Label1, TCustomLabel *Label2, TCustomLabel *Label3)
{
  int MaxRight = Label1->Left + Label1->Width;
  if(Label2) MaxRight = std::max(MaxRight, Label2->Left + Label2->Width);
  if(Label3) MaxRight = std::max(MaxRight, Label3->Left + Label3->Width);
  ResizeControl(Control, MaxRight + 5);
}
//---------------------------------------------------------------------------
void MoveControl(TControl *Control, TCustomLabel *Label)
{
  Control->Left = Label->Left + Label->Width + 5;
}
//---------------------------------------------------------------------------
void MoveLabel(TControl *Control, TCustomLabel *Label)
{
  Label->Left = Control->Left - Label->Width - 5;
}
//---------------------------------------------------------------------------
void AddAccelerator(TControl *Control, std::set<wchar_t> &Accelerators)
{
  //Don't add accelerator to OK buttons, Cancel buttons or Help buttons
  if(TButton *Button = dynamic_cast<TButton*>(Control))
    if(Button->Default || Button->Cancel || Button->Caption == _("Help"))
   return;

  //Only add accelerators to labels if FocusCotrol is set
  //Dont add to TEdit
  if(TLabel *Label = dynamic_cast<TLabel*>(Control))
    if(Label->FocusControl == NULL)
      return;
  if(TTntLabel *Label = dynamic_cast<TTntLabel*>(Control))
    if(Label->FocusControl == NULL)
      return;
  if(dynamic_cast<TCustomEdit*>(Control) != NULL || dynamic_cast<TCustomComboBox*>(Control) != NULL)
    return;

  WideString Caption = TntControl_GetText(Control);
  WideString LowerCaption = WideLowerCase(Caption);

  for(int I = 1; I <= LowerCaption.Length(); I++)
  {
    wchar_t Ch = LowerCaption[I];
    if(Accelerators.count(Ch))
    {
      Accelerators.erase(Ch);
      Caption.Insert(WideString(L'&'), I);
      TntControl_SetText(Control, Caption);
      break;
    }
  }
}
//---------------------------------------------------------------------------
void SetAccelerators(TWinControl *WinControl, std::set<wchar_t> &Accelerators)
{
  std::vector<TWinControl*> PostProcess;

  //Now loop through controls with children
  for(int I = 0; I < WinControl->ControlCount; I++)
  {
    TControl *Control = WinControl->Controls[I];
    TWinControl *WinControl2 = dynamic_cast<TWinControl*>(Control);

    if(WinControl2 && WinControl2->ControlCount)
    {
      if(dynamic_cast<TPageControl*>(WinControl2) ||
         dynamic_cast<TFrame*>(WinControl2))
        PostProcess.push_back(WinControl2);
      else
        SetAccelerators(WinControl2, Accelerators);
    }
    else
      AddAccelerator(Control, Accelerators);
  }

  for(unsigned I = 0; I < PostProcess.size(); I++)
    for(int J = 0; J < PostProcess[I]->ControlCount; J++)
      if(TWinControl *WinControl2 = dynamic_cast<TWinControl*>(PostProcess[I]->Controls[J]))
      {
        //Don't update Accelerators from page controls
        std::set<wchar_t> Temp(Accelerators);
        SetAccelerators(WinControl2, Temp);
      }
}
//---------------------------------------------------------------------------
void SetAccelerators(TWinControl *WinControl)
{
  const wchar_t Accelerators[] = L"abcdefghijklmnopqrstuvwxyz0123456789";
  std::set<wchar_t> Temp(Accelerators, Accelerators + sizeof(Accelerators) - 1);
  SetAccelerators(WinControl, Temp);
}
//---------------------------------------------------------------------------
void TranslateStrings(TStrings *Strings)
{
  for(int I = 0; I < Strings->Count; I++)
    Strings->Strings[I] = gettext(Strings->Strings[I]);
}
//---------------------------------------------------------------------------
void TranslateStrings(TTntStrings *Strings)
{
  for(int I = 0; I < Strings->Count; I++)
    Strings->Strings[I] = gettext(Strings->Strings[I]);
}
//---------------------------------------------------------------------------
WideString LoadRes(short Ident, WideString Par1, WideString Par2, WideString Par3)
{
  WideString ResStr = LoadRes(Ident);
  if(ResStr.IsEmpty())
  {
    MessageBox("Resource " + AnsiString(Ident) + " not found!", "Error", MB_ICONSTOP);
    return "";
  }

  for(int I = 1; I <= Par1.Length(); I++)
    if(Par1[I] == L'&')
      Par1.Delete(I, 1);

  for(int I = 1; I <= Par2.Length(); I++)
    if(Par2[I] == L'&')
      Par2.Delete(I, 1);

  if(!Par1.IsEmpty() && Par1[Par1.Length()] == L':')
    Par1.Delete(Par1.Length(), 1);

  boost::wformat fmter(ToWString(ResStr));
  fmter.exceptions(boost::io::all_error_bits ^ boost::io::too_many_args_bit);
  return WideString((fmter % ToWString(Par1) % ToWString(Par2) % ToWString(Par3)).str().c_str());
}
//---------------------------------------------------------------------------
WideString LoadRes(short Ident, const std::string &Par1, const std::string &Par2, const std::string &Par3)
{
  return LoadRes(Ident, WideString(Par1.c_str()), Par2.c_str(), Par3.c_str());
}
//---------------------------------------------------------------------------
WideString LoadRes(short Ident, const AnsiString &Par1, const AnsiString &Par2, const AnsiString &Par3)
{
  return LoadRes(Ident, WideString(Par1), WideString(Par2), WideString(Par3));
}
//---------------------------------------------------------------------------
WideString LoadRes(short Ident)
{
  if(!IsWinNT)
    return LoadStr(Ident);
  wchar_t Temp[512];
  LoadStringW(HInstance, Ident, Temp, sizeof(Temp)/sizeof(Temp[0]));
  return gettext(Temp);
}
//---------------------------------------------------------------------------
std::wstring LoadString(unsigned Ident)
{
  return LoadRes(Ident).c_bstr();
}
//---------------------------------------------------------------------------
int MessageBox(const char *Text, const char *Caption, int Flags)
{
  return Application->MessageBox(Text, Caption, Flags);
}
//---------------------------------------------------------------------------
//Unicode version of Application::MessageBox
int MessageBox(const WideString &Text, const WideString &Caption, int Flags)
{
  HWND ActiveWindow = (HWND)GetActiveWindow();
  HMONITOR MBMonitor = MonitorFromWindow(ActiveWindow, MONITOR_DEFAULTTONEAREST);
  HMONITOR AppMonitor = MonitorFromWindow(Application->Handle, MONITOR_DEFAULTTONEAREST);
  TRect Rect;
  if(MBMonitor != AppMonitor)
  {
    MONITORINFO MonInfo;
    MonInfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(MBMonitor, &MonInfo);
    GetWindowRect(Application->Handle, &Rect);
    SetWindowPos(Application->Handle, 0,
      MonInfo.rcMonitor.left + ((MonInfo.rcMonitor.right - MonInfo.rcMonitor.left) / 2),
      MonInfo.rcMonitor.top + ((MonInfo.rcMonitor.bottom - MonInfo.rcMonitor.top) / 2),
      0, 0, SWP_NOACTIVATE | SWP_NOREDRAW | SWP_NOSIZE | SWP_NOZORDER);
  }
  void *WindowList = DisableTaskWindows(0);
  TFocusState FocusState = SaveFocusState;
  if(Application->UseRightToLeftReading())
    Flags = Flags | MB_RTLREADING;

  int Result = MessageBoxW(Application->Handle, Text, Caption, Flags);
  if(MBMonitor != AppMonitor)
    SetWindowPos(Application-> Handle, 0,
      Rect.Left + Rect.Width() / 2,
      Rect.Top + Rect.Height() / 2,
      0, 0, SWP_NOACTIVATE | SWP_NOREDRAW | SWP_NOSIZE | SWP_NOZORDER);
  EnableTaskWindows(WindowList);
  SetActiveWindow(ActiveWindow);
  RestoreFocusState(FocusState);
  return Result;
}
//---------------------------------------------------------------------------
int MessageBox(const std::string &Text, const std::string &Caption, int Flags)
{
  return Application->MessageBox(Text.c_str(), Caption.c_str(), Flags);
}
//---------------------------------------------------------------------------
class TCustomTranslator
{
  void __fastcall TranslateTntStrings(TObject *Object)
  {
    if(TTntStrings *Strings = dynamic_cast<TTntStrings*>(Object))
      TranslateStrings(Strings);
  }

public:
  TCustomTranslator()
  {
    TP_GlobalHandleClass(__classid(TTntStrings), TranslateTntStrings);
  }
};
TCustomTranslator CustomTranslator;
//---------------------------------------------------------------------------
class TInputQueryForm : public TForm
{
  TEdit *Edit;
  TLabel *Label;
  TButton *Button1;
  TButton *Button2;
  int &Value;

public:
  __fastcall TInputQueryForm(const AnsiString &ACaption, const AnsiString &APrompt, int &AValue)
    : TForm(NULL, 0), Value(AValue)
  {
    Caption = ACaption;
    BorderStyle = bsDialog;
    Position = poMainFormCenter;
    ClientHeight = 70;

    Label = new TLabel(this);
    Label->Caption = APrompt;
    Label->Parent = this;
    Label->Left = 8;
    Label->Top = 8;

    Edit = new TEdit(this);
    Edit->Text = Value;
    Edit->Parent = this;
    Edit->Left = Label->Left + Label->Width + 5;
    Edit->Top = 8;
    Edit->Width = std::max(180 - Edit->Left - 10, 50);
    Width = Edit->Left + Edit->Width + 10;

    Button1 = new TButton(this);
    Button1->Parent = this;
    Button1->Caption = _("OK");
    Button1->Left = Width / 2 - Button1->Width - 5;
    Button1->Top = 38;
    Button1->Default = true;
    Button1->OnClick = InputQueryClick;

    Button2 = new TButton(this);
    Button2->Parent = this;
    Button2->Caption = _("Cancel");
    Button2->Left = 50;
    Button2->Left = Width / 2 + 5;
    Button2->Top = 38;
    Button2->Cancel = true;
    Button2->ModalResult = mrCancel;
  }

  void __fastcall InputQueryClick(TObject *Sender)
  {
    Value = Edit->Text.ToInt(); //Verify that a valid integer is entered
    ModalResult = mrOk;
  }
};
//---------------------------------------------------------------------------
bool InputQuery(const AnsiString &Caption, const AnsiString &Prompt, int &Value)
{
  std::auto_ptr<TForm> Form(new TInputQueryForm(Caption, Prompt, Value));
  return Form->ShowModal() == mrOk;
}
//---------------------------------------------------------------------------
AnsiString GetKeyName(UINT Key)
{
  UINT ScanCode = MapVirtualKeyA(Key, 0) << 16;
  if(ScanCode != 0)
  {
    if(Key > VK_ESCAPE && Key < 'A' /*VK_A*/) //Not sure about this range
      ScanCode |= 0x01000000;
    char KeyName[20];
    GetKeyNameTextA(ScanCode, KeyName, sizeof(KeyName));
    AnsiStrLower(&KeyName[1]);
    return AnsiString(KeyName);
  }
  return AnsiString();              
}
//---------------------------------------------------------------------------
WideString GetWideKeyName(UINT Key)
{
  UINT ScanCode = MapVirtualKeyW(Key, 0) << 16;
  if(ScanCode != 0)
  {
    if(Key > VK_ESCAPE && Key < 'A' /*VK_A*/) //Not sure about this range
      ScanCode |= 0x01000000;
    wchar_t KeyName[20];
    GetKeyNameTextW(ScanCode, KeyName, sizeof(KeyName) / sizeof(KeyName[0]));
    CharLowerW(&KeyName[1]);
    return WideString(KeyName);
  }
  return WideString();
}
//---------------------------------------------------------------------------
namespace Menus
{
  AnsiString __fastcall ShortCutToText(TShortCut ShortCut)
  {
    static const AnsiString Plus('+');
    AnsiString Str;
    if(ShortCut & scShift)
      Str += GetKeyName(VK_SHIFT) + Plus;
    if(ShortCut & scCtrl)
      Str += GetKeyName(VK_CONTROL) + Plus;
    if(ShortCut & scAlt)
      Str += GetKeyName(VK_MENU) + Plus;
    Str += GetKeyName(ShortCut & 0xFF);
    return Str;
  }
}
//---------------------------------------------------------------------------
//Replaces the function WideShortCutToText() in TntMenus.pas
//This function asks the keyboard driver for the key names instead of using hardcoded names
//We better make sure this also works on a Chinese system
namespace Tntmenus
{
  WideString __fastcall WideShortCutToText(TShortCut ShortCut)
  {
    static const WideString Plus(L'+');
    WideString Str;
    if(ShortCut & scShift)
      Str += GetWideKeyName(VK_SHIFT) + Plus;
    if(ShortCut & scCtrl)
      Str += GetWideKeyName(VK_CONTROL) + Plus;
    if(ShortCut & scAlt)
      Str += GetWideKeyName(VK_MENU) + Plus;
    Str += GetWideKeyName(ShortCut & 0xFF);
    return Str;
  }
}
//---------------------------------------------------------------------------

