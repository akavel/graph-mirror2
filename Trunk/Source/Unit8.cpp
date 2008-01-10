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
#include "Unit8.h"
//---------------------------------------------------------------------------
#pragma link "TntStdCtrls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TForm8::TForm8(TComponent* Owner, TData &AData)
  : TTntForm(Owner), Data(AData), AllowReplace(false)
{
  ScaleForm(this);
  TranslateProperties(this);
  Label1->Caption = "&" + Label1->Caption;
}
//---------------------------------------------------------------------------
bool TForm8::EditModel(std::wstring &Name)
{
  Caption = LoadRes(503);
  AllowReplace = true;
  Edit1->Text = Name.c_str();
  TUserModel UserModel = Data.UserModels[Name];
  for(unsigned I = 0; I < UserModel.Defaults.size(); I++)
    ValueListEditor1->Values[UserModel.Defaults[I].first.c_str()] = UserModel.Defaults[I].second;

  if(ShowModal() == mrOk)
  {
    for(unsigned I = 0; I < UserModel.Defaults.size(); I++)
      UserModel.Defaults[I].second = ValueListEditor1->Values[UserModel.Defaults[I].first.c_str()].ToDouble();

    Data.UserModels.erase(Name);
    Name = ToWString(Edit1->Text);
    Data.UserModels[Name] = UserModel;
    return true;
  }
  return false;
}
//---------------------------------------------------------------------------
bool TForm8::AddModel(const std::string &Model, std::wstring &ModelName)
{
  TUserModel UserModel;
  UserModel.Model = Model;
  std::vector<std::string> Unknowns = Func32::FindUnknowns(Model);

  if(Unknowns.empty())
  {
    MessageBox(LoadRes(529), LoadRes(RES_ERROR));
    return false;
  }

  Unknowns.insert(Unknowns.begin(), "x");

  Func32::TCustomFunc TempFunc(Model, Unknowns, Data.CustomFunctions.SymbolList, Data.Axes.Trigonometry);

  for(unsigned I = 1; I < Unknowns.size(); I++)
    ValueListEditor1->Values[Unknowns[I].c_str()] = 1;

  if(ShowModal() == mrOk)
  {
    for(std::vector<std::string>::const_iterator Iter = Unknowns.begin() + 1; Iter != Unknowns.end(); ++Iter)
      UserModel.Defaults.push_back(std::make_pair(*Iter, ValueListEditor1->Values[Iter->c_str()].ToDouble()));

    ModelName = ToWString(Edit1->Text);
    Data.UserModels[ModelName] = UserModel;
    return true;
  }
  return false;
}
//---------------------------------------------------------------------------
void __fastcall TForm8::ValueListEditor1KeyPress(TObject *Sender,
      char &Key)
{
  if(Key == ',')
    Key = '.';
  else if(!isdigit(Key) && Key != '\b' && Key != '-' && Key != '.')
    Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TForm8::Button1Click(TObject *Sender)
{
  //Make sure that all numbers are valid
  int I;
  try
  {
    std::wstring Name = ToWString(Edit1->Text);
    //Empty names are not allowed
    if(Name.empty())
    {
      MessageBox(LoadRes(516), LoadRes(RES_ERROR));
      return;
    }

    //Two models with the same name cannot exist
    if(!AllowReplace)
    for(unsigned I = 0; I < Data.UserModels.size(); I++)
      if(Data.UserModels.count(Name) != 0)
      {
        MessageBox(LoadRes(517), LoadRes(RES_ERROR));
        return;
      }

    for(I = 1; I < ValueListEditor1->RowCount; I++)
      ValueListEditor1->Cells[1][I].ToDouble();
    ModalResult = mrOk;
  }
  catch(...)
  {
    MessageBox(LoadRes(RES_NOT_VALID_NUMBER, ValueListEditor1->Cells[1][I]), LoadRes(RES_ERROR_IN_VALUE));
  }
}
//---------------------------------------------------------------------------

