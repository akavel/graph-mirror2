/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit8H
#define Unit8H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ValEdit.hpp>
#include "TntStdCtrls.hpp"
//---------------------------------------------------------------------------
class TForm8 : public TTntForm
{
__published:	// IDE-managed Components
  TTntEdit *Edit1;
  TTntLabel *Label1;
  TValueListEditor *ValueListEditor1;
  TTntButton *Button1;
  TTntButton *Button2;
  void __fastcall ValueListEditor1KeyPress(TObject *Sender, char &Key);
  void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
  TData &Data;
  bool AllowReplace;

public:		// User declarations
  __fastcall TForm8(TComponent* Owner, TData &AData);
  bool EditModel(std::wstring &Name);
  bool AddModel(const std::string &model, std::wstring &Name);
};
//---------------------------------------------------------------------------
#endif
