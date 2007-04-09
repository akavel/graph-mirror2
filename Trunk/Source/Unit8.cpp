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
__fastcall TForm8::TForm8(TComponent* Owner, TData &AData, int AItemIndex)
  : TTntForm(Owner), Data(AData), ItemIndex(AItemIndex)
{
  ScaleForm(this);
  TranslateProperties(this);
  Label1->Caption = "&" + Label1->Caption;
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
    for(unsigned I = 0; I < Data.UserModels.size(); I++)
      if(Data.UserModels[I].Name == Name && static_cast<int>(I) != ItemIndex)
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

