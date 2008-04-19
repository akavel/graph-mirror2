/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef GuiUtilH
#define GuiUtilH
//---------------------------------------------------------------------------
namespace Tntextctrls
{
  class TTntRadioGroup;
}

namespace Tntclasses
{
  class TTntStrings;
}

void DrawComboBoxEndPoint(TCanvas *Canvas, int Index, const TRect &Rect);

void ResizeControl(TControl *Control, int Left);
void ResizeControl(TControl *Control, TCustomLabel *Label1, TCustomLabel *Label2 = NULL, TCustomLabel *Label3 = NULL);
void MoveLabel(TControl *Control, TCustomLabel *Label);
void MoveControl(TControl *Control, TCustomLabel *Label);

void SetAccelerators(TWinControl *WinControl);

void ScaleComponent(TComponent *Component, unsigned Scale);
void ScaleForm(TForm *Form, bool Flip=true);

void TranslateStrings(TStrings *Strings);
void TranslateStrings(Tntclasses::TTntStrings *Strings);
inline WideString _(const char* Str) {return _(AnsiString(Str));}

WideString LoadRes(short Ident, WideString Par1, WideString Par2 = WideString(), WideString Par3 = WideString());
WideString LoadRes(short Ident, const std::string &Par1, const std::string &Par2 = std::string(), const std::string &Par3 = std::string());
WideString LoadRes(short Ident, const AnsiString &Par1, const AnsiString &Par2 = AnsiString(), const AnsiString &Par3 = AnsiString());
WideString LoadRes(short Ident);
std::wstring LoadString(unsigned Ident);

int MessageBox(const char *Text, const char *Caption, int Flags = MB_OK | MB_ICONWARNING);
int MessageBox(const WideString &Text, const WideString &Caption, int Flags = MB_OK | MB_ICONWARNING);
int MessageBox(const std::string &Text, const std::string &Caption, int Flags = MB_OK | MB_ICONWARNING);

bool InputQuery(const AnsiString &Caption, const AnsiString &Prompt, int &Value);

#endif
