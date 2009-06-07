/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
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
#include "VersionInfo.h"
//---------------------------------------------------------------------------
#pragma link "SymbolDialog"              
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
  String TranslatorString = LinkLabel3->Caption;
  LinkLabel3->Width = Comments->Width;
  TranslateProperties(this);
  Animate1->ResName = "FLAG";
  reinterpret_cast<TEdit*>(Animate1)->OnDblClick = ProgramIconDblClick; //Nasty hack. Don't do this at home kids

  TVersionInfo Info;
  //Make string with file version information
  if(Info.InfoAvailable())
  {
    TVersion FileVersion = Info.FileVersion();
    Label3->Caption = LoadRes(RES_BUILD, FileVersion.Build);
    FileVersion.Build = 0;
    Version->Caption = ToUString(LoadRes(RES_VERSION, FileVersion.Text()));
    if(Info.FileFlags() & ffDebug)
      Version->Caption = Version->Caption + " Beta";
    Copyright->Caption = Info.StringValue(L"LegalCopyright").c_str();
  }

  const wchar_t *EmailStr = L"<a href=\"mailto:" EMAIL L"\">" EMAIL L"</a>";
  LinkLabel2->Caption = FormatStr(LinkLabel2->Caption, EmailStr);
  const wchar_t *LinkStr = L"<a href=\"" HOMEPAGE L"\">" HOMEPAGE L"</a>";
  LinkLabel1->Caption = FormatStr(LinkLabel1->Caption, LinkStr);

  if(LinkLabel3->Caption != TranslatorString)
  {
    LinkLabel3->Visible = true;
    Height = Height + LinkLabel3->Height + 10;
  }

  //Don't scale until we have made all adjustments.
  //Disable AutoSize before we scale as it sometimes create problems.
  LinkLabel3->AutoSize = false;
  LinkLabel3->Width = Panel1->ClientWidth - 5 - LinkLabel3->Left;
  ScaleForm(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ProgramIconDblClick(TObject *Sender)
{
  Animate1->Visible = !Animate1->Visible;
  ProgramIcon->Visible = !ProgramIcon->Visible;
  Animate1->Active = !Animate1->Active;
}
//---------------------------------------------------------------------------
void __fastcall TForm2::LinkLabel1LinkClick(TObject *Sender, const UnicodeString Link,
          TSysLinkType LinkType)
{
  ShellExecute(Handle, NULL, Link.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);
}
//---------------------------------------------------------------------------
void __fastcall TForm2::LinkLabel2LinkClick(TObject *Sender, const UnicodeString Link,
          TSysLinkType LinkType)
{
  std::wstring Str = L"Mailto:";
  Str += EMAIL;
  Str += L"?Subject=Bug report/suggestions for Graph " + TVersionInfo().StringValue(L"ProductVersion");
  ShellExecute(Handle, NULL, Str.c_str(), NULL, NULL, SW_SHOWDEFAULT);
}
//---------------------------------------------------------------------------

