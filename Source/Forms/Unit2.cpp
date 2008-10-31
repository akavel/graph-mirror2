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
  Label6->Width = Comments->Width;
  TranslateProperties(this);                                     
  Animate1->ResName = "FLAG";
  reinterpret_cast<TEdit*>(Animate1)->OnDblClick = ProgramIconDblClick; //Nasty hack. Don't do this at home kids
  Label2->Left = Label1->Left + Label1->Width + 5;

  TVersionInfo Info;
  //Make string with file version information
  if(Info.InfoAvailable())
  {
    TVersion FileVersion = Info.FileVersion();
    Label3->Caption = LoadRes(RES_BUILD, FileVersion.Build);
    FileVersion.Build = 0;
    Version->Caption = LoadRes(RES_VERSION, FileVersion.Text());
    if(Info.FileFlags() & ffDebug)
      Version->Caption = Version->Caption + " beta";
    Copyright->Caption = Info.StringValue("LegalCopyright").c_str();
  }
  Label2->Caption = EMAIL;
  Label5->Caption = HOMEPAGE;

  if(Form1->Data.Property.Language != "English")
  {
    Label6->Visible = true;
    Label6->Top = Label6->Top + 25;
    Height = Height + Label6->Height + 5;
  }

  //Don't scale until we have made all adjustments.
  //Disable AutoSize before we scale as it sometimes create problems.
  Label6->AutoSize = false;
  Label6->Width = Panel1->ClientWidth - 5 - Label6->Left;
  ScaleForm(this);
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Label2Click(TObject *Sender)
{
  std::wstring Str = L"Mailto:";
  Str += EMAIL;
  Str += L"?Subject=Bug report/suggestions for Graph " + ToWString(TVersionInfo().StringValue("ProductVersion"));
  ShellExecute(Handle, NULL, Str.c_str(), NULL, NULL, SW_SHOWDEFAULT);
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Label5Click(TObject *Sender)
{
  ShellExecute(Handle, NULL, HOMEPAGE, NULL, NULL, SW_SHOWMAXIMIZED);
}
//---------------------------------------------------------------------------
void __fastcall TForm2::ProgramIconDblClick(TObject *Sender)
{
  Animate1->Visible = !Animate1->Visible;
  ProgramIcon->Visible = !ProgramIcon->Visible;
  Animate1->Active = !Animate1->Active;
}
//---------------------------------------------------------------------------







