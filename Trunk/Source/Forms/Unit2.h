/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Unit2H
#define Unit2H
//---------------------------------------------------------------------------
#include <vcl\Classes.hpp>
#include <vcl\Controls.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ComCtrls.hpp>
#include "SymbolDialog.h"
#include <pngimage.hpp>
//---------------------------------------------------------------------------
class TForm2 : public TForm
{
__published:	// IDE-managed Components
  TButton *Button1;
	TPanel *Panel1;
  TLabel *ProductName;
  TLabel *Version;
  TLabel *Copyright;
  TLabel *Comments;
  TLabel *Label3;
  TAnimate *Animate1;
  TImage *ProgramIcon;
  TLinkLabel *LinkLabel1;
  TLinkLabel *LinkLabel2;
  TLinkLabel *LinkLabel3;
  void __fastcall ProgramIconDblClick(TObject *Sender);
  void __fastcall LinkLabel1LinkClick(TObject *Sender, const UnicodeString Link,
          TSysLinkType LinkType);
  void __fastcall LinkLabel2LinkClick(TObject *Sender, const UnicodeString Link,
          TSysLinkType LinkType);
private:	// User declarations
public:		// User declarations
	__fastcall TForm2(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
