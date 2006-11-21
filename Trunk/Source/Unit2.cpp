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
#include "VersionInfo.h"
//---------------------------------------------------------------------------
#pragma link "TntStdCtrls"
#pragma link "SymbolDialog"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TTntForm(Owner)
{
  ScaleForm(this);
  Label6->Width = Comments->Width;
  TranslateProperties(this);
  Animate1->ResName = "FLAG";
  reinterpret_cast<TEdit*>(Animate1)->OnDblClick = ProgramIconDblClick; //Nasty hack. Don't do this at home kids

  TVersionInfo Info;
  //Make string with file version information
  if(Info.InfoAvailable())
  {
    TVersion FileVersion = Info.FileVersion();
    Label3->Caption = "Build " + AnsiString(FileVersion.Build);
    FileVersion.Build = 0;
    Version->Caption = "Version " + AnsiString(FileVersion.Text().c_str());
    if(Info.FileFlags() && ffDebug)
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
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Label2Click(TObject *Sender)
{
  std::string Str = "Mailto:";
  Str += EMAIL;
  Str += "?Subject=Bug report/suggestions for Graph " + TVersionInfo().StringValue("ProductVersion");
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



