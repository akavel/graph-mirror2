//---------------------------------------------------------------------------
#include <basepch.h>
#pragma hdrstop
#include "OpenPreviewDialog.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TOpenPreviewDialog *)
{
  new TOpenPreviewDialog(NULL);
}
//---------------------------------------------------------------------------
namespace Openpreviewdialog
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TOpenPreviewDialog)};
     RegisterComponents("IComp", classes, 0);
  }
}
//---------------------------------------------------------------------------
__fastcall TOpenPreviewDialog::TOpenPreviewDialog(TComponent* Owner)
  : TOpenDialog(Owner), FOnPreviewFile(NULL), Image(new TImage(this)), Panel(new TPanel(this))
{
  Panel->BevelInner = bvRaised;
  Panel->BevelOuter = bvLowered;

  Image->Parent = Panel;
  Image->Align = alClient;

  //Prevent flicker
  Panel->ControlStyle = Panel->ControlStyle << csOpaque;
  Panel->DoubleBuffered = true;
}
//---------------------------------------------------------------------------
void __fastcall TOpenPreviewDialog::DoSelectionChange()
{
  DoPreviewFile();

  TOpenDialog::DoSelectionChange();
}
//---------------------------------------------------------------------------
void __fastcall TOpenPreviewDialog::DoFolderChange()
{
  DoPreviewFile();

  TOpenDialog::DoFolderChange();
}
//---------------------------------------------------------------------------
void TOpenPreviewDialog::DoPreviewFile()
{
  if(FOnPreviewFile)
    FOnPreviewFile(this, FileName, Image->Canvas, TRect(0, 0, Image->Width, Image->Height));
}
//---------------------------------------------------------------------------
void __fastcall TOpenPreviewDialog::DoShow()
{
  const unsigned Border = 35;

  HWND Hwnd = GetParent(Handle);
  Panel->ParentWindow = Hwnd;

  TRect ClientRect;
  TRect WindowRect;
  if(!GetClientRect(Hwnd, &ClientRect))
    RaiseLastOSError();
  if(!GetWindowRect(Hwnd, &WindowRect))
    RaiseLastOSError();

  Panel->Top = ClientRect.Top + Border;
  Panel->Left = ClientRect.Right + 5;
  Panel->Width = ClientRect.Bottom - 2*Border;
  Panel->Height = ClientRect.Bottom - 2*Border;

  Image->Picture->Bitmap->Width = Image->Width;
  Image->Picture->Bitmap->Height = Image->Height;

  //Resize common dialog window
  if(!SetWindowPos(Hwnd, HWND_TOP, 0, 0, WindowRect.Width() + Panel->Width + 10, WindowRect.Height(), SWP_NOMOVE))
    RaiseLastOSError();

  TOpenDialog::DoShow();
}
//---------------------------------------------------------------------------

