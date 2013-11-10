/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "CompilerWorkaround.h"
#include <fstream>
#include <stdio.h>
#include <ActnMan.hpp>
#include <Themes.hpp>
#include <Rtti.hpp>
//---------------------------------------------------------------------------
/* Workaround for problem with support for Unicode filenames in std::fstream in CB2009.
 * It looks like the file fiopen.cpp is not compiled for Unicode support.
 * The following code is copied from Source\cpprtl\Source\Dinkumware\source\fiopen.cpp for wchar_t* instead of char*
 */
#undef _SYSCH
#define _SYSCH(x) L##x
#undef _Sysch_t
#define _Sysch_t wchar_t
#undef _CSTD
#define _CSTD
#define _Xfopen	_wfopen
namespace std
{
_CRTIMP2 FILE *_Fiopen(const _CSTD _Sysch_t *filename,
	ios_base::openmode mode, int)	// protection currently unused
	{	// open a file with native name
	static const _CSTD _Sysch_t *mods[] =
		{	// fopen mode strings corresponding to valid[i]
		_SYSCH("r"), _SYSCH("w"), _SYSCH("w"), _SYSCH("a"),
		_SYSCH("rb"), _SYSCH("wb"), _SYSCH("wb"), _SYSCH("ab"),
		_SYSCH("r+"), _SYSCH("w+"), _SYSCH("a+"),
		_SYSCH("r+b"), _SYSCH("w+b"), _SYSCH("a+b"),
		0};

	static const int valid[] =
		{	// valid combinations of open flags
		ios_base::in,
		ios_base::out,
		ios_base::out | ios_base::trunc,
		ios_base::out | ios_base::app,
		ios_base::in | ios_base::binary,
		ios_base::out | ios_base::binary,
		ios_base::out | ios_base::trunc | ios_base::binary,
		ios_base::out | ios_base::app | ios_base::binary,
		ios_base::in | ios_base::out,
		ios_base::in | ios_base::out | ios_base::trunc,
		ios_base::in | ios_base::out | ios_base::app,
		ios_base::in | ios_base::out | ios_base::binary,
		ios_base::in | ios_base::out | ios_base::trunc
			| ios_base::binary,
		ios_base::in | ios_base::out | ios_base::app
			| ios_base::binary,
		0};

	FILE *fp = 0;
	int n;
	ios_base::openmode atendflag = mode & ios_base::ate;
	ios_base::openmode norepflag = mode & ios_base::_Noreplace;

	if (mode & ios_base::_Nocreate)
		mode |= ios_base::in;	// file must exist
	if (mode & ios_base::app)
		mode |= ios_base::out;	// extension -- app implies out

	mode &= ~(ios_base::ate | ios_base::_Nocreate | ios_base::_Noreplace);
	for (n = 0; valid[n] != 0 && valid[n] != mode; ++n)
		;	// look for a valid mode

	if (valid[n] == 0)
		return (0);	// no valid mode
	else if (norepflag && mode & (ios_base::out || ios_base::app)
		&& (fp = _Xfopen(filename, _SYSCH("r"))) != 0)
		{	// file must not exist, close and fail
		fclose(fp);
		return (0);
		}
	else if (fp != 0 && fclose(fp) != 0)
		return (0);	// can't close after test open
// should open with protection here, if other than default
	else if ((fp = _Xfopen(filename, mods[n])) == 0)
		return (0);	// open failed

	if (!atendflag || fseek(fp, 0, SEEK_END) == 0)
		return (fp);	// no need to seek to end, or seek succeeded

	fclose(fp);	// can't position at end
	return (0);
	}
}
//---------------------------------------------------------------------------
//Workaround for bug in TCustomActionPopupMenuEx.LoadMenu() in ActnPopup.pas
//Hint is not assigned in the loop.
//This function is a copy of TCustomActionControl.SetSelected() in ActnMan.pas but
//compensates for the bug in TCustomActionPopupMenuEx.LoadMenu().
void __fastcall Actnman::TCustomActionControl::SetSelected(bool Value)
{
  if(Value != FSelected)
  {
    FSelected = Value;
    if(Value)
      UpdateSelection();
    Invalidate();
    if(Value && ActionClient->Action != NULL)
    {
      TCustomAction *Action = static_cast<TCustomAction*>(ActionClient->Action);
      if(Action->Hint == "" && ActionClient->Tag != 0)
        Action->Hint = reinterpret_cast<TMenuItem*>(ActionClient->Tag)->Hint;
      Application->Hint = GetLongHint(Action->Hint);
		}
		else
			Application->Hint = ""; //CancelHint();
		if(Value)
			NotifyWinEvent(EVENT_OBJECT_FOCUS, Parent->Handle, OBJID_CLIENT, ActionClient->Index + 1);
	}
}
//---------------------------------------------------------------------------
//Fix for minor bug in DrawCloseButton() inside TDockTree::PaintDockFrame().
//twCloseButtonNormal will draw a button where the cross is not scaled under Windows 7,
//which makes it look ugly. Instead we use twSmallCloseButtonNormal when the button is small.
//TDockTree::FGrabberSize can only be changed through RTTI.
//TDockTree::AdjustDockRect() is using the GrabberSize constant instead of the FGrabberSize member.
class TFixedDockTree : public TDockTree
{
	void DrawThemedCloseButton(TCanvas *Canvas, int Left, int Top, int Size);
  void DrawThemedGrabber(TCanvas *Canvas, TThemedRebar GripperType, int Left, int Top, int Right, int Bottom);
protected:
	void __fastcall PaintDockFrame(TCanvas *TCanvas, TControl *Control, const TRect &Rect);
  void __fastcall AdjustDockRect(TControl *Control, TRect &ARect);
public:
  __fastcall TFixedDockTree(TWinControl *DockSite);
};

__fastcall TFixedDockTree::TFixedDockTree(TWinControl *DockSite)
  : TDockTree(DockSite)
{
  TRttiContext Context;
  TRttiType *DockTreeType = Context.GetType(__classid(TDockTree));
  TRttiField *GrabberSize = DockTreeType->GetField("FGrabberSize");
  GrabberSize->SetValue(this, TValue::From((Screen->PixelsPerInch * 12) / 96));
}

void TFixedDockTree::DrawThemedCloseButton(TCanvas *Canvas, int Left, int Top, int Size)
{
	TRect DrawRect(Left, Top, Left+Size, Top+Size);
	TThemedWindow ButtonType = Size < 15 ? twSmallCloseButtonNormal : twCloseButtonNormal;
	TThemedElementDetails Details = ThemeServices()->GetElementDetails(ButtonType);
	ThemeServices()->DrawElement(Canvas->Handle, Details, DrawRect, NULL);
}

void TFixedDockTree::DrawThemedGrabber(TCanvas *Canvas, TThemedRebar GripperType, int Left, int Top, int Right, int Bottom)
{
  TRect DrawRect(Left, Top, Right, Bottom);
  TThemedElementDetails Details = ThemeServices()->GetElementDetails(GripperType);
  ThemeServices()->DrawElement(Canvas->Handle, Details, DrawRect);
}

void __fastcall TFixedDockTree::PaintDockFrame(TCanvas *Canvas, TControl *Control, const TRect &Rect)
{
	if(ThemeServices()->ThemesEnabled)
  {
    int GrabberSize = (Screen->PixelsPerInch * 12) / 96;
    int GrabberWidth = (Screen->PixelsPerInch * 10) / 96;
		if(DockSite->Align == alTop || DockSite->Align == alBottom)
    {
			DrawThemedCloseButton(Canvas, Rect.Left+1, Rect.Top+1, GrabberWidth);
      DrawThemedGrabber(Canvas, trGripperVert, Rect.Left+1, Rect.Top+GrabberSize+1, Rect.Left+GrabberWidth, Rect.Bottom-2);
    }
		else
    {
			DrawThemedCloseButton(Canvas, Rect.Right-GrabberWidth-1, Rect.Top+1, GrabberWidth);
      DrawThemedGrabber(Canvas, trGripper, Rect.Left+2, Rect.Top+1,  Rect.Right-GrabberSize-2, Rect.Top+GrabberWidth);
    }
  }
  else
  	TDockTree::PaintDockFrame(Canvas, Control, Rect);
}

void __fastcall TFixedDockTree::AdjustDockRect(TControl *Control, TRect &ARect)
{
  int GrabberSize = (Screen->PixelsPerInch * 12) / 96;
//Allocate room for the caption on the left if docksite is horizontally
//oriented, otherwise allocate room for the caption on the top. }
  if(DockSite->Align == alTop || DockSite->Align == alBottom)
    ARect.Left += GrabberSize;
  else
    ARect.Top += GrabberSize;
}
//---------------------------------------------------------------------------
class TInitWorkaround
{
public:
	TInitWorkaround()
	{
		DefaultDockTreeClass = __classid(TFixedDockTree);
	}
} InitWorkaround;
//---------------------------------------------------------------------------
extern "C" int _RTLENTRY  _EXPFUNC __iswctype(int c, int type);
static int IsType(wchar_t c, int Type)
{
	if(c >= 256)
	{
		WORD d;
		GetStringTypeW(CT_CTYPE1, (LPCWSTR)&c, 1, &d);
		return d & Type;
	}
	return __iswctype(c, Type);
}

int _RTLENTRY _EXPFUNC std::iswspace(wchar_t c)
{
	return IsType(c, _IS_SP);
}

int _RTLENTRY _EXPFUNC std::iswdigit(wchar_t c)
{
	return IsType(c, _IS_DIG);
}
//---------------------------------------------------------------------------

