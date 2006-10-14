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
#include <Registry.hpp>
#include <StrUtils.hpp>
#include "Common.h"
#include "Unit4.h"
#include "Unit9.h"
//---------------------------------------------------------------------------
#pragma link "TntStdCtrls"
#pragma link "TntExtCtrls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner, TData &AData)
	: TTntForm(Owner), Data(AData)
{
  ScaleForm(this);
  TranslateProperties(this);
  SetAccelerators(this);
  int Left = Label1->Left + TMaxWidth(Label1)(Label2)(Label3) + 5;
  int DeltaWidth = Canvas->TextWidth(CheckBox7->Caption) + 20 - CheckBox7->Width;
  Width = Width + TMaxWidth(Left - Edit1->Left)(DeltaWidth);

  const TProperty &Property = Data.Property;
  CheckBox1->Checked = CheckAssocation(".grf", "GraphFile");
  CheckBox2->Checked = Application->ShowHint;
  CheckBox4->Checked = Property.SavePos;
  CheckBox7->Checked = Property.CheckForUpdate;
  UpDown1->Position = Property.RoundTo;
  UpDown2->Position = Form1->Recent1->MaxFiles;
  UpDown3->Position = UndoList.GetMaxUndo();
  RadioGroup1->ItemIndex = Property.ComplexFormat;

  //Put the language codes into a ComboBox
  GetLanguageList(ComboBox1->Items->AnsiStrings);
  OldLanguageIndex = ComboBox1->Items->IndexOf(Data.Property.Language);
  ComboBox1->ItemIndex = OldLanguageIndex;
  ComboBox2->Text = AnsiString(Data.Property.FontScale) + "%";
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button1Click(TObject *Sender)
{
  TProperty &Property = Data.Property;
  if(!CheckLimit(Edit1, LoadRes(RES_LIMIT_ZERO, Label1->Caption),0))
    return;
  if(!CheckLimit(Edit2, LoadRes(510), 0, 9))
    return;
  if(!CheckLimit(Edit3, LoadRes(530, Label3->Caption),0,1000))
    return;

  int FontScale = AnsiReplaceStr(ComboBox2->Text, "%", "").ToInt();
  if(FontScale < 75 || FontScale > 200)
  {
    ComboBox2->SetFocus();
    MessageBox(LoadRes(RES_VALUE_RANGE, Label5->Caption, 75, 200), LoadRes(RES_ERROR_IN_VALUE));
    return;
  }

  Data.AbortUpdate();
  UndoList.Push(MakeUndoObject(Property));

  Property.RoundTo = ToInt(Edit1->Text);
  Form1->Recent1->MaxFiles = ToInt(Edit2->Text);
  UndoList.SetMaxUndo(ToInt(Edit3->Text));
  Property.ComplexFormat = (TComplexFormat)RadioGroup1->ItemIndex;;
  if(CheckBox1->Checked)
  {
    bool AllUsers = GetRegValue(REGISTRY_KEY, "InstallAllUsers", HKEY_CURRENT_USER, 0) || !IsWinNT;
    AssociateExt(".grf", "", "GraphFile", "Graph system", Application->ExeName + ",1", AllUsers);
  }
  else
    RemoveAsociation(".grf", "GraphFile");
  Application->ShowHint = CheckBox2->Checked;
  Property.SavePos = CheckBox4->Checked;
  Property.CheckForUpdate = CheckBox7->Checked;

  //Load new resource dll if the setting was changed
  if(OldLanguageIndex != ComboBox1->ItemIndex && ComboBox1->ItemIndex != -1)
  {
    Form1->ChangeLanguage(ComboBox1->Text);
    CreateRegKey(REGISTRY_KEY, "Language", ComboBox1->Text, reinterpret_cast<unsigned>(HKEY_CURRENT_USER));
  }

  if(FontScale != Property.FontScale)
  {
    Form1->ScaleBy(FontScale, Property.FontScale);
    ScaleComponent(Form1, FontScale / Property.FontScale);
    Property.FontScale = FontScale;
    Form1->ActionToolBar1->ActionClient->Items->SmallIcons = FontScale < 150;
  }
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Button3Click(TObject *Sender)
{
  Application->HelpContext(HelpContext);
}
//---------------------------------------------------------------------------
void __fastcall TForm4::ComboBox2KeyPress(TObject *Sender, char &Key)
{
  if(!std::isdigit(Key) && Key != VK_BACK && Key != '%')
    Key = 0;
}
//---------------------------------------------------------------------------

