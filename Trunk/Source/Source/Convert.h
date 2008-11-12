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
double MakeFloat(TCustomEdit *Edit, const String &ErrorStr=String(), double Min=-MaxDouble, double Max=MaxDouble);
double MakeFloat(TCustomEdit *Edit, const String &ErrorStr, const boost::function1<bool, double> &Interval);
int MakeInt(TCustomEdit *Edit, const String &Caption);
void ComplexToRTF(const Func32::TComplex &C, TRichEdit *RichEdit);
String ComplexToString(const Func32::TComplex &C);
String RoundToStr(long double Number, unsigned Decimals);
String RoundToStr(long double Number, const TData &Data);
String DoubleToStr(long double Number);
String DoubleToStr(long double Number,int Decimals);
double CellToDouble(class TGrid *Grid, int Col, int Row);
int GetDecimals(double Number);
std::wstring FontToStr(TFont *Font);
void StrToFont(const std::wstring &Str, TFont *Font);
double StringToDouble(const String &Str);
bool CheckLimit(TWinControl *Control, String Str, int Min=-MaxInt, int Max=MaxInt);
std::pair<int, int> FloatToFract(double f, double Epsilon = 0.01);
inline std::wstring RoundToString(long double Number, unsigned Decimals) {return RoundToStr(Number, Decimals).c_str();}
inline std::wstring RoundToString(long double Number, const TData &Data) {return RoundToStr(Number, Property.RoundTo).c_str();}
std::string RtfToPlainText(const std::string &Str);
void Trim(std::wstring &Str);

inline int ToIntDef(const std::wstring &Str, int Default) {return StrToIntDef(Str.c_str(), Default);}
inline int ToIntDef(const String &Str, int Default) {return StrToIntDef(Str, Default);}

std::string ToString(int Value);
std::string ToString(unsigned Value);
std::string ToString(long double Value);
std::string ToString(const std::wstring &Str);
std::string ToString(const String &Str);

std::wstring ToWString(int Value);
std::wstring TOWString(long double Value);
std::wstring ToWString(const std::string &Str);
std::wstring ToWString(const String &Str);
std::wstring ToWString(const char *Str);

String ToUString(const std::wstring &Str);
String ToUString(const std::string &Str);

std::string ToLower(const std::string &Str);

#endif
