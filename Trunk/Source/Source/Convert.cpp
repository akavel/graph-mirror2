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
#include "Unit1.h"
#include "Convert.h"
#include "Grid.h"
#include "GuiUtil.h"
#include <boost/spirit/phoenix/operators.hpp>
#include <boost/spirit/phoenix/primitives.hpp>
using phoenix::arg1;
//---------------------------------------------------------------------------
double MakeFloat(TCustomEdit *Edit, const String &ErrorStr, const boost::function1<bool, double> &Interval)
{
  AnsiString Text = Edit->Text;
  if(Text.IsEmpty())
  {
    SetGlobalFocus(Edit);
    MessageBox(ErrorStr.IsEmpty() ? LoadRes(RES_VALUE_NEEDED) : ErrorStr, LoadRes(RES_ERROR));
    throw EAbort("");
  }

  try
  {
    double Number = Form1->Data.Calc(Text.c_str());

    if(!Interval(Number))
    {
      SetGlobalFocus(Edit);
      if(ErrorStr.IsEmpty())
        MessageBox("This error should not occur. Please report this to the programmer.", LoadRes(RES_ERROR), MB_ICONSTOP);
      else
        MessageBox(ErrorStr, LoadRes(RES_ERROR_IN_VALUE));
      throw EAbort("");
    }
    return Number;
  }
  catch(Func32::EFuncError &Error)
  {
    SetGlobalFocus(Edit);
    ShowErrorMsg(Error, Edit);
    throw EAbort("");
  }
}
//---------------------------------------------------------------------------
double MakeFloat(TCustomEdit *Edit, const String &ErrorStr, double Min, double Max)
{
  return MakeFloat(Edit, ErrorStr, arg1 >= Min && arg1 <= Max);
}
//---------------------------------------------------------------------------
int MakeInt(TCustomEdit *Edit, const String &Caption)
{
  String Text = Edit->Text;
  if(Text.IsEmpty())
  {
    MessageBox(LoadRes(RES_VALUE_NEEDED), LoadRes(RES_ERROR));
    Edit->SetFocus();
    throw EAbort("");
  }

  try
  {
    double Number = Form1->Data.Calc(Text.c_str());
    int Value = Number;
    if(Number <= 0 || static_cast<double>(Value) != Number)
    {
      MessageBox(LoadRes(RES_INT_GREATER_ZERO, Caption), LoadRes(RES_ERROR));
      SetGlobalFocus(Edit);
      throw EAbort("");
    }
    return Number;
  }
  catch(Func32::EFuncError &Error)
  {
    SetGlobalFocus(Edit);
    ShowErrorMsg(Error, Edit);
    throw EAbort("");
  }
}
//---------------------------------------------------------------------------
//This function writes the complex number into the RichEdit
void ComplexToRTF(Func32::TComplex C, TRichEdit *RichEdit)
{
  const TData &Data = Form1->Data;
  //Create rtf string to everything to become bold
  //Font f0: MS Sans Serif
  //Font f1: Symbol
  AnsiString Str = "{\\rtf1\\ansi\\deff0{\\fonttbl{\\f0\\fswiss\\fprq2\\fcharset0 MS Sans Serif;}{\\f1\\froman\\fprq2\\fcharset2 Symbol;}}\n\\f0\\fs18 ";

  switch(Property.ComplexFormat)
  {
  case cfReal:
    //Check for an imaginary part
    if(std::abs(C.imag()) < MIN_ZERO)
      Str += RoundToStr(C.real(), Data);
    break;
  case cfRectangular:
  {
    //Round numbers to chosen number of decimals
    AnsiString Real = RoundToStr(C.real(), Data);
    AnsiString Imag = RoundToStr(std::abs(C.imag()), Data);

    if(Imag == "0" || std::abs(C.imag()) < MIN_ZERO)  //(-1.50)^2 = 2.25+2.4395E-19i
      Str += Real;
    else //If there is a imaginary part
    {
      if(Real != "0")
        Str += Real + (C.imag() < 0 ? '-' : '+');
      //Add imaginary part to string
      if(Imag != "1")
        Str += Imag;
      Str += "\\b i";
    }
    break;
  }
  case cfPolar: //Polar format
    //The complex number 0 does not have an angle
    if(C.real() == 0 && C.imag() == 0)
    {
      Str += '0';
      break;
    }

    Str += RoundToStr(abs(C), Data); //Get numeric value as a string
    //Add angle symbol to text in Symbol font
    Str += "\\f1\\fs20 \xD0\\f0\\fs18 ";
    if(Data.Axes.Trigonometry == Func32::Degree)
      //Add degree symbol, if angle is in degree
      Str += RoundToStr(std::arg(C) * 180 / M_PI, Data) + "\\f1\\fs20 \xB0";
    else
      Str += RoundToStr(arg(C), Data);
  }

  Str += "\n}";
  //Use stream to put data into RichEdit; Only thing that works under Win9x
  std::auto_ptr<TStringStream> Stream(new TStringStream(Str));
  RichEdit->Lines->LoadFromStream(Stream.get());
}
//---------------------------------------------------------------------------
String ComplexToString(const Func32::TComplex &C)
{
  const TData &Data = Form1->Data;
  String Str;

  switch(Property.ComplexFormat)
  {
    case cfReal:
      //Check for an imaginary part
      if(std::abs(C.imag()) < MIN_ZERO)
        Str += RoundToStr(C.real(), Data);
      break;

    case cfRectangular:
    {
      //Round numbers to chosen number of decimals
      String Real = RoundToStr(C.real(), Data);
      String Imag = RoundToStr(std::abs(C.imag()), Data);

      if(Imag == "0" || std::abs(C.imag()) < MIN_ZERO)  //(-1.50)^2 = 2.25+2.4395E-19i
        Str += Real;
      else //If there is a imaginary part
      {
        if(Real != "0")
          Str += Real + (C.imag() < 0 ? '-' : '+');
        //Add imaginary part to string
        if(Imag != "1")
          Str += Imag;
        Str += "i";
      }
      break;
    }
    case cfPolar: //Polar format
      //The complex number 0 does not have an angle
      if(C.real() == 0 && C.imag() == 0)
      {
        Str += "0";
        break;
      }

      Str += RoundToStr(abs(C), Data); //Get numeric value as a string
      //Add angle symbol to text in Symbol font
      Str += L'\x2220';
      if(Data.Axes.Trigonometry == Func32::Degree)
        //Add degree symbol, if angle is in degree
        Str += RoundToStr(std::arg(C) * 180 / M_PI, Data) + L'\xB0';
      else
        Str += RoundToStr(arg(C), Data);
  }

  return Str;
}
//---------------------------------------------------------------------------
//This function converts a floating point number to a string
//The number of decimals will be determined by Property.RoundTo
String DoubleToStr(long double Number)
{
  return DoubleToStr(Number, Property.RoundTo);
}
//---------------------------------------------------------------------------
//This function converts a floating point number to a string
//Decimals indicate the fixed number of decimals in the string
String DoubleToStr(long double Number,int Decimals)
{
  if(std::abs(Number) < MIN_ZERO)
    return 0;
  if(Number >= 10000 || Number <= -10000)
    return FloatToStrF(Number,ffExponent,Decimals,9999);
  return FloatToStrF(Number,ffFixed,9999,Decimals);
}
//---------------------------------------------------------------------------
//This function returns the number of decimals in a number
int GetDecimals(double Number)
{
  double Dummy;
  int Decimals;
  for(Decimals = 0; std::modf(Number,&Dummy); Decimals++)
    Number *= 10;
  return Decimals;
}
//---------------------------------------------------------------------------
//Coverts a TFont objet to a AnsiString
std::wstring FontToStr(TFont *Font)
{
  //String with Name,Size,Color
  std::wstring Str = (Font->Name + "," + Font->Size + "," + ColorToString(Font->Color)).c_str();

  if(!Font->Style.Empty())
    Str += L",";

  //Add B, I or U acording to the font style
  if(Font->Style.Contains(fsBold))
    Str += L"B";
  if(Font->Style.Contains(fsItalic))
    Str += L"I";
  if(Font->Style.Contains(fsUnderline))
    Str += L"U";
  return Str;
}
//---------------------------------------------------------------------------
//Converts a AnsiString into a TFont object
//Format: Name,Size,Color,Style
void StrToFont(const std::wstring &Str, TFont *Font)
{
  if(Str.empty())
    return;

  unsigned Pos = Str.find(L',');
  //Get font name from string
  Font->Name = Str.substr(0, Pos).c_str();

  unsigned Pos2 = Str.find(L',', Pos+1);
  //Get font size from string
  Font->Size = ToIntDef(Str.substr(Pos+1, Pos2-Pos-1), 10);

  unsigned Pos3 = Str.find(L',', Pos2+1);
  //Get font color from string
  Font->Color = StringToColor(Str.substr(Pos2+1, Pos3-Pos2-1).c_str());
  Font->Style = Font->Style.Clear(); //Clear font style

  //Loop through the rest of the characters in string
  if(Pos3 != std::wstring::npos)
    for(unsigned N = Pos3+1; N < Str.size(); N++)
      switch(Str[N])
      {
        case L',': return; //Return if a ',' is found
        case L'B': //Add Bold to font style
        case L'b': Font->Style = Font->Style << fsBold; break;
        case L'I': //Add Italic to font style
        case L'i': Font->Style = Font->Style << fsItalic; break;
        case L'U': //Add Underlined to font style
        case L'u': Font->Style = Font->Style << fsUnderline;
      }

  //If font does not exists
  if(Screen->Fonts->IndexOf(Font->Name) == -1 && Font->Name.CompareIC(DEFAULT_FONT) != 0)
  {
    Form1->ShowStatusError(LoadRes(RES_UNKNOWN_FONT, Font->Name));
    Font->Name = DEFAULT_FONT;
  }
}
//---------------------------------------------------------------------------
//Coverts a string to a double
//Takes care of INF, -INF and +INF
double StringToDouble(const AnsiString &Str)
{
  //Chek for INF or +INF
  if(!Str.AnsiCompareIC("INF") || !Str.AnsiCompareIC("+INF"))
    return std::numeric_limits<double>::infinity();
  //Check for -INF
  if(!Str.AnsiCompareIC("-INF"))
    return -std::numeric_limits<double>::infinity();
  //Convert number to double
  return Str.ToDouble();
}
//---------------------------------------------------------------------------
bool CheckLimit(TWinControl *Control, String Str, int Min, int Max)
{
  bool Error = false;
  try
  {
    int Value = GetControlText(Control).ToInt();
    if(Value < Min || Value > Max)
      Error = true;
  }
  catch(...)
  {
    Error = true;
  }
  if(Error)
  {
    MessageBox(Str, LoadRes(RES_ERROR_IN_VALUE));
    Control->SetFocus();
    return false;
  }
  return true;
}
//---------------------------------------------------------------------------
String RoundToStr(long double Number, unsigned Decimals)
{
  if(std::_isnanl(Number))
    return "NAN";
  if(!std::_finitel(Number))
    return "INF";
  if(std::abs(Number) < MIN_ZERO)
    return 0;

  String Str;
  if(std::abs(Number) >= 10000 || std::abs(Number) <= 1E-4)
  {
    Str = FloatToStrF(Number, ffExponent, Decimals + 1, 0);
    int N = Str.Pos('E');
    while(Str[N-1] == '0' && Str[N-2] != '.')
      Str.Delete(--N, 1);
    return Str.c_str();
  }
  Str = FloatToStrF(Number,ffFixed, 9999, Decimals);

  //Returns if no decimal separator found
  if(!Str.Pos(DecimalSeparator))
    return Str.c_str();

  //Remove trailing zeros
  while(Str[Str.Length()] == '0')
    Str.Delete(Str.Length(), 1);
  if(Str[Str.Length()] == DecimalSeparator)
    Str.Delete(Str.Length(), 1);
  return Str;
}
//---------------------------------------------------------------------------
String RoundToStr(long double Number, const TData &Data)
{
  return RoundToStr(Number, Property.RoundTo);
}
//---------------------------------------------------------------------------
double CellToDouble(TGrid *Grid, int Col, int Row)
{
  try
  {
    //First try to convert as a number as optimization. If it fails parse using Calc()
    double Result;
    AnsiString Str = Grid->Cells[Col][Row];
    if(Str.Pos("e") == -1 && TryStrToFloat(Str, Result))
      return Result;
    return Form1->Data.Calc(Str.c_str());
  }
  catch(Func32::EParseError &E)
  {
    Grid->SetFocus();
    Grid->Col = Col;
    Grid->Row = Row;
    Grid->SetCursorPos(E.ErrorPos);
    ShowErrorMsg(E);
    throw EAbort("");
  }
  catch(Func32::EFuncError &E)
  {
    return NAN;
  }
}
//---------------------------------------------------------------------------
//Converts a float number to a fraction. F.ex: 0.66666666 = 2/3
std::pair<int, int> FloatToFract(double f, double Epsilon)
{
  double t = f;
  while(std::abs(t - static_cast<int>(t + (t < 0 ? -0.5 : 0.5))) > Epsilon)
    t += f;
  int n = t + (t < 0 ? -0.5 : 0.5);
  int d = n/f + (n/f < 0 ? -0.5 : 0.5);
  return std::pair<int, int>(n, d);
}
//---------------------------------------------------------------------------
std::string RtfToPlainText(const std::string &Str)
{
  //We need a parent window; just use main form
  std::auto_ptr<TIRichEdit> RichEdit(new TIRichEdit(Application->MainForm));
  RichEdit->Visible = false;
  RichEdit->Parent = Application->MainForm;
  RichEdit->WordWrap = false;
  RichEdit->SetRichText(Str.c_str());
  AnsiString Text = RichEdit->GetPlainText();
  AnsiString Result;
  for(int I = 1; I <= Text.Length(); I++)
    if(Text[I] == '\n')
      Result += ' ';
    else if(Text[I] != '\r' && Text[I] != '\v')
      Result += Text[I];
  return Result.Trim().c_str();
}
//---------------------------------------------------------------------------
std::string ToString(int Value)
{
  return AnsiString(Value).c_str();
}
//---------------------------------------------------------------------------
std::string ToString(unsigned Value)
{
  return AnsiString(Value).c_str();
}
//---------------------------------------------------------------------------
std::string ToString(long double Value)
{
  return AnsiString(FloatToStr(Value)).c_str();
}
//---------------------------------------------------------------------------
std::string ToString(const std::wstring &Str)
{
  return AnsiString(String(Str.c_str())).c_str();
}
//---------------------------------------------------------------------------
std::string ToString(const String &Str)
{
  return AnsiString(Str).c_str();
}
//---------------------------------------------------------------------------
std::wstring ToWString(int Value)
{
  return String(Value).c_str();
}
//---------------------------------------------------------------------------
std::wstring ToWString(long double Value)
{
  return FloatToStr(Value).c_str();
}
//---------------------------------------------------------------------------
std::wstring ToWString(const std::string &Str)
{
  return String(Str.c_str()).c_str();
}
//--------------------------------------------------------------------------
std::wstring ToWString(const String &Str)
{
  return Str.c_str();
}
//--------------------------------------------------------------------------
std::wstring ToWString(const char *Str)
{
  return String(Str).c_str();
}
//--------------------------------------------------------------------------
String ToUString(const std::wstring &Str)
{
  return Str.c_str();
}
//--------------------------------------------------------------------------
String ToUString(const std::string &Str)
{
  return Str.c_str();
}
//--------------------------------------------------------------------------
void Trim(std::wstring &Str)
{
  unsigned Pos1 = Str.find_first_not_of(L" ");
  unsigned Pos2 = Str.find_last_not_of(L" ");
  if(!Str.empty() && (Pos1 != 0 || Pos2 != Str.size()-1))
    Str.replace(0, std::wstring::npos, Str, Pos1, Pos2 - Pos1 + 1);
}
//--------------------------------------------------------------------------
std::string ToLower(const std::string &Str)
{
  std::string Result;
  for(unsigned I = 0; I < Str.size(); I++)
    Result += std::tolower(Str[I]);
  return Result;
}
//---------------------------------------------------------------------------



