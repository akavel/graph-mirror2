/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef ConvertH
#define ConvertH
#include <boost/function.hpp>
#include <math.hpp>
//---------------------------------------------------------------------------
double MakeFloat(TCustomEdit *Edit, const WideString &ErrorStr=WideString(), double Min=-MaxDouble, double Max=MaxDouble);
double MakeFloat(TCustomEdit *Edit, const WideString &ErrorStr, const boost::function1<bool, double> &Interval);
int MakeInt(TCustomEdit *Edit, const AnsiString &Caption);
void ComplexToRTF(const Func32::TComplex &C, TRichEdit *RichEdit);
WideString ComplexToWideString(const Func32::TComplex &C);
AnsiString RoundToStr(long double Number, unsigned Decimals);
AnsiString RoundToStr(long double Number, const TData &Data);
AnsiString DoubleToStr(long double Number);
AnsiString DoubleToStr(long double Number,int Decimals);
double CellToDouble(class TGrid *Grid, int Col, int Row);
int GetDecimals(double Number);
std::string FontToStr(TFont *Font);
void StrToFont(const std::string &Str, TFont *Font);
double StringToDouble(const AnsiString &Str);
bool CheckLimit(TWinControl *Control, AnsiString Str, int Min=-MaxInt, int Max=MaxInt);
std::pair<int, int> FloatToFract(double f, double Epsilon = 0.01);
inline std::string RoundToString(long double Number, unsigned Decimals) {return RoundToStr(Number, Decimals).c_str();}
inline std::string RoundToString(long double Number, const TData &Data) {return RoundToStr(Number, Property.RoundTo).c_str();}
std::string RtfToPlainText(const std::string &Str);
void Trim(std::string &Str);

inline int ToInt(const WideString &Str) {return AnsiString(Str).ToInt();}
inline int ToIntDef(const std::string &Str, int Default) {return StrToIntDef(Str.c_str(), Default);}
inline int ToIntDef(const WideString &Str, int Default) {return StrToIntDef(Str, Default);}

std::string ToString(int Value);
std::string ToString(unsigned Value);
std::string ToString(long double Value);
std::string ToString(const std::wstring &Str);
std::string ToString(const WideString &Str);

std::wstring ToWString(int Value);
std::wstring ToWString(const std::string &Str);
std::wstring ToWString(const WideString &Str);

WideString ToWideString(const char *Str);
WideString ToWideString(const std::wstring &Str);
WideString ToWideString(const std::string &Str);
WideString ToWideString(const AnsiString &Str);

std::string ToLower(const std::string &Str);

#endif
