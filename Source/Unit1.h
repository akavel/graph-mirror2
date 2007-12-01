/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <TRecent.h>
#include <ImgList.hpp>
#include <AppEvnts.hpp>
#include "IPageSetup.h"
#include "IColorSelect.h"
#include "Cross.h"
#include "FocusPanel.h"
#include "MySplitter.h"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <ActnCtrls.hpp>
#include <ActnList.hpp>
#include <ActnMan.hpp>
#include <CustomizeDlg.hpp>
#include <ToolWin.hpp>
#include <StdActns.hpp>
#include "IColorSelect.h"
#include "IPageSetup.h"
#include "IPolygon.h"
#include "IRichEdit.h"
#include <IdAntiFreeze.hpp>
#include <IdAntiFreezeBase.hpp>
#include <OleCtnrs.hpp>
#include <ExtDlgs.hpp>
#include "OpenPreviewDialog.h"
#include "IPrintDialog.h"
#include "SaveDialogEx.h"
#include <ListActns.hpp>
#include "TntComCtrls.hpp"
#include "TntForms.hpp"
#include "TntMenus.hpp"
#include "TntActnList.hpp"
#include <stack>
#include <boost/scoped_ptr.hpp>
//---------------------------------------------------------------------------
enum TCursorState {csIdle, csAddLabel, csMoveLabel, csMoveLegend, csZoomWindow, csMove, csMoving};
enum TSaveError {seNoError, seFileAccess, seOutOfResources, sePdfError, seDllError, seUnknownFileType};

enum TIconIndex
{
  iiSave, //Position of save icon used in StatusBar in image list
  iiPrint,
  iiFuncNode,
  iiTanNode,
  iiLabelNode,
  iiAxesNode,
  iiBullet,
  iiNormalNode,
  iiOleNode,
  iiUnChecked,
  iiChecked,
  iiGrayed
};

class TForm1 : public TTntForm
{
__published:	// IDE-managed Components
  TOpenDialog *OpenDialog;
  TTntMainMenu *MainMenu;
  TTntMenuItem *File_;
  TTntMenuItem *Edit_;
  TSaveDialog *SaveDialog;
  TRecent *Recent1;
  TTntPopupMenu *PopupMenu1;
  TTntMenuItem *Tree_Delete;
  TTntMenuItem *Tree_Properties;
  TApplicationEvents *ApplicationEvents;
  TTimer *Timer1;
  TImageList *ImageList1;
  TFocusPanel *Panel2;
  TImage *Image1;
  TCross *Cross;
  TShape *Shape1;
  TTntMenuItem *Tree_Copy;
  TTntMenuItem *Tree_Cut;
  TTntMenuItem *Tree_Paste;
  TTntMenuItem *N8;
  TTntMenuItem *N10;
  TTntMenuItem *Tree_InsTan;
  TTntMenuItem *Tree_InsDiff;
  TTntMenuItem *Tree_InsTrend;
  TTntMenuItem *Tree_InsShade;
  TTntMenuItem *Internet_HomePage;
  TTntMenuItem *Internet_Update;
  TImageList *ImageList2;
  TCustomizeDlg *CustomizeDlg;
  TActionManager *ActionManager;
  TTntAction *NewAction;
  TActionToolBar *ActionToolBar1;
  TTntAction *OpenAction;
  TTntAction *SaveAction;
  TTntAction *SaveAsAction;
  TTntAction *SaveAsImageAction;
  TTntAction *PrintAction;
  TTntAction *ExitAction;
  TTntAction *UndoAction;
  TTntAction *RedoAction;
  TTntAction *CutAction;
  TTntAction *CopyAction;
  TTntAction *PasteAction;
  TTntAction *CopyImageAction;
  TTntAction *AxesAction;
  TTntAction *OptionsAction;
  TTntAction *InsertFunctionAction;
  TTntAction *InsertTangentAction;
  TTntAction *InsertShadeAction;
  TTntAction *InsertPointsAction;
  TTntAction *InsertTrendlineAction;
  TTntAction *EditAction;
  TTntAction *DeleteAction;
  TTntAction *InsertDifAction;
  TTntAction *ZoomInAction;
  TTntAction *ZoomOutAction;
  TTntAction *ZoomWindowAction;
  TTntAction *ZoomStandardAction;
  TTntAction *MoveUpAction;
  TTntAction *MoveDownAction;
  TTntAction *MoveLeftAction;
  TTntAction *MoveRightAction;
  TTntAction *MoveAction;
  TTntAction *PathAction;
  TTntAction *AreaAction;
  TTntAction *EvalAction;
  TTntAction *TableAction;
  TTntAction *ContentsAction;
  TTntAction *ListAction;
  TTntAction *HomePageAction;
  TTntAction *UpdateAction;
  TTntAction *AboutAction;
  TTntAction *SeparatorAction;
  TTntPopupMenu *PopupMenu2;
  TTntMenuItem *ToolBar_Reset;
  TTntMenuItem *ToolBar_Customize;
  TTntMenuItem *N11;
  TTntAction *ZoomFitAllAction;
  TTntAction *FaqAction;
  TMySplitter *Splitter1;
  TPanel *Panel3;
  TTntTreeView *TreeView;
  TPanel *Panel1;
  TPanel *Panel4;
  TTntAction *ZoomFitAction;
  TTntAction *ZoomSquareAction;
  TTntMenuItem *Tree_ShowInLegend;
  TTntMenuItem *N15;
  TIPolygon *IPolygon1;
  TTntMenuItem *File_Import_PointSeries;
  TTntMenuItem *File_Import_GraphFile;
  TTntAction *InsertLabelAction;
  TImage *Image2;
  TTntPopupMenu *PopupMenu3;
  TTntMenuItem *Label_Edit;
  TTntMenuItem *Label_Delete;
  TTntMenuItem *Internet_Donate;
  TTntAction *DonateAction;
  TTntAction *TipAction;
  TTntAction *SaveCopyAsAction;
  TOpenPreviewDialog *OpenPreviewDialog1;
  TTntMenuItem *Tree_Export;
  TIPrintDialog *IPrintDialog1;
  TTntPopupMenu *PopupMenu4;
  TTntMenuItem *Legend_Show;
  TTntMenuItem *Legend_Placement;
  TTntMenuItem *Legend_TopRight;
  TTntMenuItem *Legend_BottomRight;
  TTntMenuItem *Legend_TopLeft;
  TTntMenuItem *Legend_BottomLeft;
  TTntAction *SupportAction;
  TTntMenuItem *Internet_Support;
  TTntAction *ZoomXInAction;
  TTntAction *ZoomXOutAction;
  TTntAction *ZoomYInAction;
  TTntAction *ZoomYOutAction;
  TTntAction *InsertRelationAction;
  TTntAction *CustomFunctionsAction;
  TSaveDialogEx *SaveDialogEx1;
  TTntMenuItem *Label_Placement;
  TTntMenuItem *Label_Above;
  TTntMenuItem *Label_Below;
  TTntMenuItem *Label_Left;
  TTntMenuItem *Label_Right;
  TTntMenuItem *Tree_Placement;
  TTntMenuItem *Tree_Right;
  TTntMenuItem *Tree_Left;
  TTntMenuItem *Tree_Below;
  TTntMenuItem *Tree_Above;
  TTntMenuItem *Label_Rotation;
  TTntMenuItem *Rotation_90;
  TTntMenuItem *Rotation_180;
  TTntMenuItem *Rotation_270;
  TTntMenuItem *Rotation_0;
  TTntMenuItem *File_Exit;
  TTntMenuItem *N2;
  TTntMenuItem *File_Print;
  TTntMenuItem *N4;
  TTntMenuItem *File_Import;
  TTntMenuItem *File_SaveAsPicture;
  TTntMenuItem *N1;
  TTntMenuItem *File_SaveCopyAs;
  TTntMenuItem *File_SaveAs;
  TTntMenuItem *File_Save;
  TTntMenuItem *File_Open;
  TTntMenuItem *File_New;
  TTntMenuItem *Help_;
  TTntMenuItem *Math_;
  TTntMenuItem *Zoom_;
  TTntMenuItem *Function_;
  TTntMenuItem *Edit_Property;
  TTntMenuItem *Axes1;
  TTntMenuItem *N3;
  TTntMenuItem *Edit_CopyImage;
  TTntMenuItem *N9;
  TTntMenuItem *Edit_Paste;
  TTntMenuItem *Edit_Copy;
  TTntMenuItem *Edit_Cut;
  TTntMenuItem *N5;
  TTntMenuItem *Edit_Redo;
  TTntMenuItem *Edit_Undo;
  TTntMenuItem *Customfunctions1;
  TTntMenuItem *Function_Delete;
  TTntMenuItem *Function_Edit;
  TTntMenuItem *N7;
  TTntMenuItem *Insertlabel1;
  TTntMenuItem *Insertinequation1;
  TTntMenuItem *N16;
  TTntMenuItem *Function_InsTrend;
  TTntMenuItem *Function_InsPoints;
  TTntMenuItem *N6;
  TTntMenuItem *Function_InsertDiff;
  TTntMenuItem *Function_InsShade;
  TTntMenuItem *Function_InsTan;
  TTntMenuItem *Function_InsFunc;
  TTntMenuItem *Allpoints1;
  TTntMenuItem *Fit1;
  TTntMenuItem *N13;
  TTntMenuItem *Movesystem1;
  TTntMenuItem *N14;
  TTntMenuItem *Zoom_Standard;
  TTntMenuItem *Square1;
  TTntMenuItem *Zoom_Window;
  TTntMenuItem *Zoom_Out;
  TTntMenuItem *Zoom_In;
  TTntMenuItem *Math_Table;
  TTntMenuItem *Math_Eval;
  TTntMenuItem *Math_Area;
  TTntMenuItem *Math_Arc;
  TTntMenuItem *Help_About;
  TTntMenuItem *Help_Internet;
  TTntMenuItem *N12;
  TTntMenuItem *Help_Tip;
  TTntMenuItem *Help_Faq;
  TTntMenuItem *Help_Functions;
  TTntMenuItem *Help_Contents;
  TTntMenuItem *Math_Animate;
  TTntAction *AnimateAction;
  TTntAction *ImportGraphFileAction;
  TTntAction *ImportPointSeriesAction;
  TTntMenuItem *Legend_Font;
  TTntAction *InsertObjectAction;
  TTntMenuItem *Insertobject1;
  TTntMenuItem *Label_Rotation_Custom;
  TTntMenuItem *Tree_Rotation;
  TTntMenuItem *Tree_Rotation_0;
  TTntMenuItem *Tree_Rotation_90;
  TTntMenuItem *Tree_Rotation_180;
  TTntMenuItem *Tree_Rotation_270;
  TTntMenuItem *Tree_Rotation_Custom;
  TTntMenuItem *Plugins_;
  TTntMenuItem *Tree_Placement_Custom;
  TTntMenuItem *Label_Placement_Custom;
  TPanel *Panel5;
  TSplitter *Splitter2;
  TTntStatusBar *StatusBar1;
  TPanel *Panel6;
  TTimer *Timer2;
  void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift, int X,int Y);
  void __fastcall Image1MouseUp(TObject *Sender, TMouseButton Button,	TShiftState Shift, int X, int Y);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall Math_MonotoniClick(TObject *Sender);
  void __fastcall FormConstrainedResize(TObject *Sender,
  int &MinWidth, int &MinHeight, int &MaxWidth, int &MaxHeight);
  void __fastcall StatusBar1Resize(TObject *Sender);
  void __fastcall StatusBar1DrawPanel(TStatusBar *StatusBar,
  TStatusPanel *Panel, const TRect &Rect);
  void __fastcall Timer1Timer(TObject *Sender);
  void __fastcall ApplicationEventsShowHint(AnsiString &HintStr,
  bool &CanShow, THintInfo &HintInfo);
  void __fastcall TreeViewCollapsing(TObject *Sender,
  TTreeNode *Node, bool &AllowCollapse);
  void __fastcall Panel2Resize(TObject *Sender);
  void __fastcall TreeViewContextPopup(TObject *Sender,
  TPoint &MousePos, bool &Handled);
  void __fastcall TreeViewDblClick(TObject *Sender);
  void __fastcall TreeViewMouseDown(TObject *Sender,
  TMouseButton Button, TShiftState Shift, int X, int Y);
  void __fastcall FormMouseWheelUp(TObject *Sender,
  TShiftState Shift, const TPoint &MousePos, bool &Handled);
  void __fastcall FormMouseWheelDown(TObject *Sender,
  TShiftState Shift, const TPoint &MousePos, bool &Handled);
  void __fastcall Splitter1DblClick(TObject *Sender);
  void __fastcall TreeViewChange(TObject *Sender, TTreeNode *Node);
  void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
    TShiftState Shift);
  void __fastcall FormKeyPress(TObject *Sender, char &Key);
  void __fastcall TreeViewMouseMove(TObject *Sender,
    TShiftState Shift, int X, int Y);
  void __fastcall TreeViewKeyPress(TObject *Sender, char &Key);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall ApplicationEventsActivate(TObject *Sender);
  void __fastcall Panel2MouseLeave(TObject *Sender);
  bool __fastcall ApplicationEventsHelp(WORD Command, int Data,
    bool &CallHelp);
  bool __fastcall Recent1LoadFile(TRecent *Sender,
    const AnsiString &FileName);
  void __fastcall xPageSetupDialogPaintText(TIPageSetupDialog *Sender,
    TCanvas *PageCanvas, const TRect &UpdateRect,
    const TRect &PaperSize);
  void __fastcall NewActionExecute(TObject *Sender);
  void __fastcall OpenActionExecute(TObject *Sender);
  void __fastcall SaveActionExecute(TObject *Sender);
  void __fastcall SaveAsActionExecute(TObject *Sender);
  void __fastcall SaveAsImageActionExecute(TObject *Sender);
  void __fastcall PrintActionExecute(TObject *Sender);
  void __fastcall ExitActionExecute(TObject *Sender);
  void __fastcall UndoActionExecute(TObject *Sender);
  void __fastcall RedoActionExecute(TObject *Sender);
  void __fastcall CutActionExecute(TObject *Sender);
  void __fastcall CopyActionExecute(TObject *Sender);
  void __fastcall PasteActionExecute(TObject *Sender);
  void __fastcall CopyImageActionExecute(TObject *Sender);
  void __fastcall AxesActionExecute(TObject *Sender);
  void __fastcall OptionsActionExecute(TObject *Sender);
  void __fastcall InsertFunctionActionExecute(TObject *Sender);
  void __fastcall InsertTangentActionExecute(TObject *Sender);
  void __fastcall InsertShadeActionExecute(TObject *Sender);
  void __fastcall InsertPointsActionExecute(TObject *Sender);
  void __fastcall InsertTrendlineActionExecute(TObject *Sender);
  void __fastcall EditActionExecute(TObject *Sender);
  void __fastcall DeleteActionExecute(TObject *Sender);
  void __fastcall InsertDifActionExecute(TObject *Sender);
  void __fastcall ZoomInActionExecute(TObject *Sender);
  void __fastcall ZoomOutActionExecute(TObject *Sender);
  void __fastcall ZoomWindowActionExecute(TObject *Sender);
  void __fastcall ZoomStandardActionExecute(TObject *Sender);
  void __fastcall PathActionExecute(TObject *Sender);
  void __fastcall AreaActionExecute(TObject *Sender);
  void __fastcall EvalActionExecute(TObject *Sender);
  void __fastcall TableActionExecute(TObject *Sender);
  void __fastcall ContentsActionExecute(TObject *Sender);
  void __fastcall ListActionExecute(TObject *Sender);
  void __fastcall HomePageActionExecute(TObject *Sender);
  void __fastcall UpdateActionExecute(TObject *Sender);
  void __fastcall AboutActionExecute(TObject *Sender);
  void __fastcall MoveRightActionExecute(TObject *Sender);
  void __fastcall MoveLeftActionExecute(TObject *Sender);
  void __fastcall MoveUpActionExecute(TObject *Sender);
  void __fastcall MoveDownActionExecute(TObject *Sender);
  void __fastcall SeparatorActionUpdate(TObject *Sender);
  void __fastcall MoveActionExecute(TObject *Sender);
  void __fastcall ToolBar_CustomizeClick(TObject *Sender);
  void __fastcall ToolBar_ResetClick(TObject *Sender);
  void __fastcall CustomizeDlgClose(TObject *Sender);
  void __fastcall ApplicationEventsSettingChange(TObject *Sender, int Flag,
    const AnsiString Section, int &Result);
  void __fastcall ZoomFitAllActionExecute(TObject *Sender);
  void __fastcall FaqActionExecute(TObject *Sender);
  void __fastcall Panel4DockDrop(TObject *Sender, TDragDockObject *Source,
    int X, int Y);
  void __fastcall Panel4UnDock(TObject *Sender, TControl *Client,
    TWinControl *NewTarget, bool &Allow);
  void __fastcall Panel4DockOver(TObject *Sender, TDragDockObject *Source,
    int X, int Y, TDragState State, bool &Accept);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall ZoomFitActionExecute(TObject *Sender);
  void __fastcall ZoomSquareActionExecute(TObject *Sender);
  void __fastcall Tree_ShowInLegendClick(TObject *Sender);
  void __fastcall InsertLabelActionExecute(TObject *Sender);
  void __fastcall Image1DblClick(TObject *Sender);
  void __fastcall Label_EditClick(TObject *Sender);
  void __fastcall Label_DeleteClick(TObject *Sender);
  void __fastcall DonateActionExecute(TObject *Sender);
  void __fastcall TipActionExecute(TObject *Sender);
  void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
  void __fastcall SaveCopyAsActionExecute(TObject *Sender);
  void __fastcall TreeViewKeyDown(TObject *Sender, WORD &Key,
    TShiftState Shift);
  bool __fastcall OpenPreviewDialog1PreviewFile(TOpenPreviewDialog *Dialog,
    const AnsiString &FileName, TCanvas *Canvas, const TRect &Rect);
  void __fastcall OpenPreviewDialog1Show(TObject *Sender);
  void __fastcall OpenPreviewDialog1Close(TObject *Sender);
  void __fastcall Tree_ExportClick(TObject *Sender);
  void __fastcall IPrintDialog1PaintSample(TIPrintDialog *Sender,
    TCanvas *PageCanvas, const TRect &PaintRect,
    const TRect &PaperSize);
  void __fastcall IPrintDialog1Show(TObject *Sender);
  void __fastcall TreeViewDragOver(TObject *Sender, TObject *Source, int X,
          int Y, TDragState State, bool &Accept);
  void __fastcall TreeViewDragDrop(TObject *Sender, TObject *Source, int X,
          int Y);
  void __fastcall LegendPlacementClick(TObject *Sender);
  void __fastcall Legend_ShowClick(TObject *Sender);
  void __fastcall PopupMenu4Popup(TObject *Sender);
  void __fastcall SupportActionExecute(TObject *Sender);
  void __fastcall ZoomXInActionExecute(TObject *Sender);
  void __fastcall ZoomXOutActionExecute(TObject *Sender);
  void __fastcall ZoomYInActionExecute(TObject *Sender);
  void __fastcall ZoomYOutActionExecute(TObject *Sender);
  void __fastcall InsertRelationActionExecute(TObject *Sender);
  void __fastcall CustomFunctionsActionExecute(TObject *Sender);
  void __fastcall SaveDialogEx1Help(TObject *Sender);
  void __fastcall PlacementClick(TObject *Sender);
  void __fastcall PopupMenu3Popup(TObject *Sender);
  void __fastcall PopupMenu1Popup(TObject *Sender);
  void __fastcall ApplicationEventsException(TObject *Sender,
          Exception *E);
  void __fastcall RotationClick(TObject *Sender);
  void __fastcall AnimateActionExecute(TObject *Sender);
  void __fastcall ImportGraphFileActionExecute(TObject *Sender);
  void __fastcall ImportPointSeriesActionExecute(TObject *Sender);
  void __fastcall Legend_FontClick(TObject *Sender);
  void __fastcall InsertObjectActionExecute(TObject *Sender);
  void __fastcall ZoomSquareActionUpdate(TObject *Sender);
  void __fastcall ZoomActionUpdate(TObject *Sender);
  void __fastcall Panel6UnDock(TObject *Sender, TControl *Client,
          TWinControl *NewTarget, bool &Allow);
  void __fastcall Panel6DockDrop(TObject *Sender, TDragDockObject *Source,
          int X, int Y);
  void __fastcall Panel6DockOver(TObject *Sender, TDragDockObject *Source,
          int X, int Y, TDragState State, bool &Accept);
  void __fastcall Panel6GetSiteInfo(TObject *Sender, TControl *DockClient,
          TRect &InfluenceRect, TPoint &MousePos, bool &CanDock);
  void __fastcall Panel4GetSiteInfo(TObject *Sender, TControl *DockClient,
          TRect &InfluenceRect, TPoint &MousePos, bool &CanDock);
  void __fastcall Timer2Timer(TObject *Sender);
  void __fastcall TreeViewEndDrag(TObject *Sender, TObject *Target, int X,
          int Y);

private:	// User declarations
  friend class TAddView;

  //Form pos in standard size
  int StdTop, StdLeft, StdWidth, StdHeight;
  WideString StatusError;
  TColor StatusErrorColor;
  int IsResizing;     // >0 when the window is being resized or dragged
  bool Reseized;      // true if windo was resized
  int Updating;
  Func32::TDblPoint MovePos; //Position when moving system with mouse; Coordinate where left button was pressed
  TPoint LastMousePos; //Last coordiante for mouse
  AnsiString DefaultToolBar; //List of tool bar buttons from design time
  AnsiString StartToolBar;   //The toolbar shown a startup; Not necessarily the one saved
  boost::shared_ptr<TTextLabel> MovingLabel; //Label being moved
  TLabelPlacement MovingLabelPlacement; //Used while moving a text label
  TLegendPlacement LegendPlacement; //Used while moving the legend
  TCursorState CursorState;
  boost::scoped_ptr<class TForm9> Form9;
  TData PreviewData;
  boost::scoped_ptr<TDraw> PreviewDraw; //Only availble while open dialog is shown
  const int FixedImages; //Number of fixed images in ImageList1 (The rest are dynamically added and deleted)
  boost::scoped_ptr<struct TImageOptions> ActionImageOptions; //Used when saving as image from menu

  int HelpError; //Errorcode used for showing help file
  int StatusIcon;
  int xZoom, yZoom;
  bool AbortPrinting; //Set in OnClose to signal that we should abort printing
  TPoint ImagePos;   //The actual Position of Image2. The real position may vary from this when it snaps

  bool AskSave();
  void LoadSettings();
  void SaveSettings();
  void CreateToolBar(AnsiString Str);
  AnsiString GetToolBar();
  void Initialize();
  void SetCursorState(TCursorState State);
  void EditLabel(const boost::shared_ptr<TTextLabel> &Label);
  void CheckForUpdate(bool StartupCheck);
  void Translate();
  void DeleteGraphElem(const boost::shared_ptr<TGraphElem> &GraphElem);
  boost::shared_ptr<TGraphElem> GetGraphElem(TTreeNode *Node);
  TTntTreeNode* GetNode(const boost::shared_ptr<const TGraphElem> &Elem);
  void CopyAsImageToClipboard();
  TTntTreeNode* GetRootNode(unsigned Index);
  bool DoSave(const AnsiString &FileName, bool Remember);
  void MoveAndSnapLegend(int dx, int dy, bool Snap);
  void MoveAndSnapLabel(int dx, int dy, bool Snap);
  void MoveLabel(int X, int Y, TLabelPlacement Placement, bool Snap);
  void __fastcall DebugLine(System::TObject* Sender, const AnsiString Line, bool &Discard); //Write debug text from dxGetText
  void ChangeVisible(boost::shared_ptr<TGraphElem> GraphElem);
  void HandleCommandLine();

  int AddImage(int Index, TColor Color);
  int AddImage(TColor Color, TBrushStyle Style);

  //Declare message handlers
  void __fastcall WMDropFiles(TMessage &Message);
  void __fastcall WMEnterSizeMove(TMessage &Message);
  void __fastcall WMExitSizeMove(TMessage &Message);
  void __fastcall WMUser(TMessage &Message);
  void __fastcall ExecuteFunction(TMessage &Message);

  //Workaround for bug in VCL
  void __fastcall WndProc(TMessage &Message);

BEGIN_MESSAGE_MAP
  VCL_MESSAGE_HANDLER(WM_DROPFILES, TMessage, WMDropFiles)
  VCL_MESSAGE_HANDLER(WM_ENTERSIZEMOVE,TMessage, WMEnterSizeMove)
  VCL_MESSAGE_HANDLER(WM_EXITSIZEMOVE,TMessage, WMExitSizeMove)
  VCL_MESSAGE_HANDLER(WM_USER, TMessage, WMUser)
  VCL_MESSAGE_HANDLER(WM_USER+1, TMessage, ExecuteFunction);
END_MESSAGE_MAP(TForm)

public:		// User declarations
  TData Data;
  TDraw Draw;

  __fastcall TForm1(TComponent* Owner);
  __fastcall ~TForm1();
  void Redraw();
  void UpdateMenu();
  bool ZoomWindow(double xMin, double xMax, double yMin, double yMax, bool Update=true, bool SaveUndo=true);
  bool Zoom(TPoint Pos, double ZoomRate, bool ChangeUnits=true);
  bool Zoom(double x, double y, double xZoomRate, double yZoomRate, bool ChangeUnits=true);
  bool Zoom(double ZoomRate, bool ChangeUnits=true);
  bool Zoom(double xZoomRate, double yZoomRate, bool ChangeUnits=true);
  void ShowStatusError(const WideString &Str, TColor = clRed, unsigned Timeout = 5000);
  void CancelStatusError();
  void ShowStatusMessage(const AnsiString &Str);
  void SetHelpError(int AHelpError) {HelpError = AHelpError;}
  void SetStatusIcon(int AStatusIcon) {StatusIcon = AStatusIcon; StatusBar1->Repaint();}

  void UpdateTreeView(const boost::shared_ptr<TGraphElem> &Selected = boost::shared_ptr<TGraphElem>());

  void SetCross(int X, int Y);
  void SetCrossPos(long double x, long double y);
  void ChangeLanguage(const AnsiString &Ext);
  void ActivateOleUserInterface();

  void __fastcall UpdateEval();
  void __fastcall BeginUpdate();
  void __fastcall EndUpdate();

  TSaveError SaveAsImage(const AnsiString &FileName, const TImageOptions &ImageOptions);
  TSaveError SaveAsImage(const AnsiString &FileName, int ImageFileType, const TImageOptions &ImageOptions);
  bool LoadFromFile(const AnsiString &FileName, bool AddToRecent = true, bool ShowErrorMessages = true);
  void LoadDefault();
};

//---------------------------------------------------------------------------
extern TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
