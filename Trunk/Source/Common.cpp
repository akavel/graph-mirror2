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
#include <complex>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "VersionInfo.h"
#include "ConfigFile.h"
#include "TntGraphics.hpp"

const bool IsWinNT = Win32Platform == VER_PLATFORM_WIN32_NT;
//Do not use std::numeric_limits<double>::quiet_NaN(), which makes the program crash
const double NAN = 0.0/0.0;
const double INF = 1.0/0.0;
//---------------------------------------------------------------------------
//Centers a form on the working area
void CenterForm(TForm *Form)
{
  TRect WorkArea;
  if(!SystemParametersInfo(SPI_GETWORKAREA,0,&WorkArea,0))
    return;
  Form->Left=WorkArea.Left+(WorkArea.Width()-Form->Width)/2;
  Form->Top=WorkArea.Top+(WorkArea.Height()-Form->Height)/2;
}
//---------------------------------------------------------------------------
//This function makes an association between a file type and a program
//Ext:          The file type. Ex. ".doc"
//ProgramName:  The file path and name of the program file. Ex. "D:\\WINWORD.EXE"
//              If ProgramName is empty when the applications name is used.
//Ident:        An identifier used to create the asociation. Ex. "docfile"
//Description:  A description of the file type
//Icon:         Path and name of icon file and the number of icon in the file
//              Ex. "C:\\WINWORD.EXE,0"
void AssociateExt(AnsiString Ext, AnsiString ProgramName, AnsiString Ident, AnsiString Description, AnsiString Icon, bool AllUsers)
{
  try
  {
    //Make sure there is a dot before the extention
    if(Ext[1] != '.')
      Ext = AnsiString('.') + Ext;
    //If no program name specified then use the application name
    if(ProgramName.IsEmpty())
      ProgramName = Application->ExeName;
    //If no identifier when use the exe name without extention
    if(Ident.IsEmpty())
      Ident = Application->ExeName.SubString(1,Application->ExeName.AnsiPos("."));
    //If no icon when use the icon from ProgramName
    if(Icon.IsEmpty())
      Icon = ProgramName + ",0";
    //Change ProgramName to format: "Name.ext" "%1"
    ProgramName = "\"" + ProgramName + "\" \"%1\"";

    DWORD RootKey = reinterpret_cast<DWORD>(AllUsers ? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER);

    CreateRegKey("Software\\Classes\\" + Ext, "", Ident, RootKey);
    CreateRegKey("Software\\Classes\\" + Ident, "", Description, RootKey);
    CreateRegKey("Software\\Classes\\" + Ident + "\\DefaultIcon", "", Icon, RootKey);
    CreateRegKey("Software\\Classes\\" + Ident + "\\shell\\open\\command", "", ProgramName, RootKey);

    //Tell the shell that a file association has been changed
    SHChangeNotify(SHCNE_ASSOCCHANGED, 0, NULL, NULL);
  }
  catch(...)
  {
  }
}
//---------------------------------------------------------------------------
//This function removes an association between a file type and a program
//Ext is the file type. Ex. ".doc"
//Ident is the identifier used when the association was made. Ex. "docfile"
void RemoveAsociation(AnsiString Ext, AnsiString Ident)
{
  //Make sure there is a dot before the extention
  if(Ext[1] != '.')
    Ext = AnsiString('.') + Ext;

  if(GetRegStringValue(Ext, "") == Ident)
    DeleteRegKey(Ext);

  DeleteRegKey(Ident);

  //Tell the shell that a file association has been changed
  SHChangeNotify(SHCNE_ASSOCCHANGED, 0, NULL, NULL);
}
//---------------------------------------------------------------------------
//This function checks if a file type is associated with a program
//Ext:   The file extention. Ex. ".doc"
//Ident: The identifier used when the asocation was made. Ex. "docfile"
bool CheckAssocation(AnsiString Ext,AnsiString Ident)
{
  bool Result=false;
  TRegistry *Registry=new TRegistry();
  Registry->RootKey=HKEY_CLASSES_ROOT;
  if(Registry->OpenKey(AnsiString('\\')+Ext,false))
    if(Registry->ReadString("")==Ident)
      Result=true;
  delete Registry;
  return Result;
}
//---------------------------------------------------------------------------
WideString GetErrorMsg(const Func32::EFuncError &Error)
{
  WideString ErrorStr = LoadRes(Error.ErrorCode+100, Error.Str.c_str());
  if(ErrorStr.IsEmpty())
    ErrorStr = "Unknown error code.\nCould not find error message for this error.";
  return ErrorStr;
}
//---------------------------------------------------------------------------
WideString GetErrorMsg(Func32::TErrorCode ErrorCode)
{
  return GetErrorMsg(Func32::EFuncError(ErrorCode));
}
//---------------------------------------------------------------------------
//Shows error message corresponding to ErrorCode in Func
//If Edit parameter is suported the Edit box gets focus and
//the cursor position is set to where the error ocoured
void ShowErrorMsg(const Func32::EFuncError &Error, TCustomEdit *Edit)
{
  AnsiString str = LoadRes(RES_ERROR) + " " + AnsiString(Error.ErrorCode);
  Form1->SetHelpError(Error.ErrorCode);
  MessageBox(GetErrorMsg(Error), str, MB_ICONWARNING | MB_HELP);
  Form1->SetHelpError(0);
  if(Edit)
  {
    SetGlobalFocus(Edit);
    if(const Func32::EParseError *ParseError = dynamic_cast<const Func32::EParseError*>(&Error))
      Edit->SelStart = ParseError->ErrorPos;
  }
}
//---------------------------------------------------------------------------
void ShowErrorMsg(const ECustomFunctionError &Error, TCustomEdit *Edit)
{
  MessageBox(LoadRes(Error.ErrorCode + 200, Error.Text), LoadRes(RES_ERROR), MB_ICONWARNING);
  if(Edit)
  {
    SetGlobalFocus(Edit);
    Edit->SelStart = Error.ErrorPos;
  }
}
//---------------------------------------------------------------------------
void ShowErrorMsg(const EGraphError &Error, TCustomEdit *Edit)
{
  MessageBox(LoadRes(Error.ErrorCode + 210), LoadRes(RES_ERROR), MB_ICONWARNING);
  if(Edit)
    SetGlobalFocus(Edit);
}
//---------------------------------------------------------------------------
//Draws a curved line between a series of points
void DrawCurve(TCanvas *Canvas, const std::vector<TPoint> &Points)
{
  std::vector<TPoint> List;
  List.reserve(Points.size()*3+1);
  List.push_back(Points.front());
  for(unsigned I = 1; I < Points.size()-2; I++)
  {
    List.push_back(Points[I]);
    List.push_back(Points[I]);
    List.push_back(TPoint((Points[I].x + Points[I+1].x)/2, (Points[I].y + Points[I+1].y)/2));
  }
  List.push_back(Points[Points.size()-2]);
  List.push_back(Points[Points.size()-2]);
  List.push_back(Points.back());
  Canvas->PolyBezier(&List[0], List.size()-1);
}
//---------------------------------------------------------------------------
void InvertBitmap(Graphics::TBitmap *Bitmap)
{
  InvertRect(Bitmap->Canvas->Handle, &TRect(0, 0, Bitmap->Width, Bitmap->Height));
}
//---------------------------------------------------------------------------
//Returns number rounded up to nearest a*10^k, where a={1,2,5} and k is an integer
double AdjustUnit(double Number)
{
  double Exponent = std::floor(std::log10(Number));
  double Factor = std::pow10(Exponent);

  //Normalise; Number = [1;10[
  Number /= Factor;
  if(Number > 5)
    Number = 10;
  else if(Number > 2)
    Number = 5;
  else
    Number = 2;
  return Number * Factor;
}
//---------------------------------------------------------------------------
//Set focus to the control, even though the parent doesn't have focus
void SetGlobalFocus(TWinControl *Control)
{
  if(Control == NULL || dynamic_cast<TForm*>(Control))
    return;

  if(TTabSheet *TabSheet = dynamic_cast<TTabSheet*>(Control))
    TabSheet->PageControl->ActivePage = TabSheet;

  SetGlobalFocus(Control->Parent);
  if(Control->CanFocus())
    Control->SetFocus();
}
//---------------------------------------------------------------------------
//Math error handler
//Called on any math errors;
int _matherr(_exception *a)
{
  //Bug in RTL (cosl.asm) cosl() will call _matherr() instead of _matherrl() on error
  //Because of this a->arg1 is also wrong
  if(std::string(a->name) == "cosl")
  {
    using namespace std;
    a->retval = 0;//NAN gives problems with log(-0)
    errno = a->type;
    return 1;
  }

  //We are only allowed to show VCL dialogs from main thread
  //We should probably handle this differently
  if(GetCurrentThreadId() != MainThreadID)
    return 1;

  try
  {
    static bool IgnoreAll = false;
    if(IgnoreAll)
      return 1;

    AnsiString Error;
    switch(a->type)
    {
      case DOMAIN:    Error = "DOMAIN"; break;
      case SING:      Error = "SING"; break;
      case OVERFLOW:  Error = "OVERFLOW"; break;
      case UNDERFLOW: Error = "UNDERFLOW"; break;
      case TLOSS:     Error = "TLOSS"; break;
      default:        Error = "Unknown"; break;
    }

    AnsiString Str = "Internal error! Please report this to the programmer with steps to reproduce:\n" + AnsiString(a->name) + '(' + a->arg1 + ',' + a->arg2 + "): " + Error + " error\nContinue?";

    int Result = MessageDlg(Str, mtError, TMsgDlgButtons() << mbYes << mbYesToAll << mbAbort, 0);
    if(Result == mrYesToAll)
      IgnoreAll = true;
    else if(Result == mrAbort)
      abort();

    a->retval = NAN;
  }
  catch(Exception &E)
  {
    Application->ShowException(&E);
  }
  return 1;
}
//---------------------------------------------------------------------------
template<typename T1, typename T2>
LessFirstPair(const std::pair<T1, T2> &Pair1, const std::pair<T1, T2> &Pair2)
{
  return Pair1.first < Pair2.first;
}
//---------------------------------------------------------------------------
//List is filled with langugae names, which are the same as the file names
void GetLanguageList(TStrings *List)
{
  List->Clear();
  List->Add("English"); //We always have English

  TSearchRec SearchRec;
  AnsiString Path = ExtractFilePath(Application->ExeName) + "locale\\";
  int Result = FindFirst(Path + "*.mo", faReadOnly | faArchive, SearchRec);
  while(Result == 0)
  {
    List->Add(SearchRec.Name.SubString(1, SearchRec.Name.Length() - 3));
    Result = FindNext(SearchRec);
  }
  FindClose(SearchRec);
}
//---------------------------------------------------------------------------
//Remove Key and all subkeys from the registry
void RemoveRegistryKey(const AnsiString &Key, HKEY RootKey)
{
  std::auto_ptr<TRegistry> Registry(new TRegistry);
  int Pos = Key.Pos('\\');
  if(Pos == -1)
    Pos = 0;
  AnsiString ParentKey = Key.SubString(1, Pos - 1);
  AnsiString KeyToDelete = Key.SubString(Pos + 1, Key.Length());
  Registry->RootKey = RootKey;
  if(Registry->OpenKey(ParentKey, false))
    Registry->DeleteKey(KeyToDelete);
}
//---------------------------------------------------------------------------
AnsiString GetRegValue(const AnsiString &Key, const AnsiString &ValueName, HKEY RootKey, const AnsiString &Default)
{
  HKEY RegKey;
  AnsiString Result = Default;

  if(RegOpenKeyEx(RootKey, Key.c_str(), 0, KEY_QUERY_VALUE, &RegKey) == ERROR_SUCCESS)
  {
    DWORD Size;
    DWORD Type;
    if(RegQueryValueEx(RegKey, ValueName.c_str(), NULL, &Type, NULL, &Size) == ERROR_SUCCESS && Type == REG_SZ)
    {
      std::vector<BYTE> Data(Size);
      if(RegQueryValueEx(RegKey, ValueName.c_str(), NULL, NULL, &Data[0], &Size) == ERROR_SUCCESS)
        Result = reinterpret_cast<char*>(&Data[0]);
    }
    RegCloseKey(RegKey);
  }
  return Result;
}
//---------------------------------------------------------------------------
unsigned GetRegValue(const AnsiString &Key, const AnsiString &ValueName, HKEY RootKey, unsigned Default)
{
  HKEY RegKey;
  DWORD Result = Default;

  if(RegOpenKeyEx(RootKey, Key.c_str(), 0, KEY_QUERY_VALUE, &RegKey) == ERROR_SUCCESS)
  {
    DWORD Type;
    RegQueryValueEx(RegKey, ValueName.c_str(), NULL, &Type, NULL, NULL);
    if(Type == REG_DWORD)
    {
      DWORD Data;
      DWORD Size = sizeof(Data);
      if(RegQueryValueEx(RegKey, ValueName.c_str(), NULL, NULL, reinterpret_cast<unsigned char*>(&Data), &Size) == ERROR_SUCCESS)
        Result = Data;
    }
    RegCloseKey(RegKey);
  }
  return Result;
}
//---------------------------------------------------------------------------
bool RegKeyExists(const AnsiString &Key, HKEY RootKey)
{
  HKEY RegKey;
  DWORD Result = false;

  if(RegOpenKeyEx(RootKey, Key.c_str(), 0, KEY_QUERY_VALUE, &RegKey) == ERROR_SUCCESS)
  {
    Result = true;
    RegCloseKey(RegKey);
  }
  return Result;
}
//---------------------------------------------------------------------------
//WARNING don't inline this function. It gives problems in release mode
std::istream& operator>>(std::istream &Stream, TColor &Color)
{
  std::string Str;
  Stream >> Str;
  Color = StringToColor(Str.c_str());
  return Stream;
}
//---------------------------------------------------------------------------
std::wstring ReduceString(const std::wstring &Str, unsigned MaxLength)
{
  if(Str.size() > MaxLength)
    return Str.substr(0, MaxLength-4) + L" ...";
  return Str;
}
//---------------------------------------------------------------------------
TMaxWidth::TMaxWidth(TControl *Control)
{
  Value = WideCanvasTextWidth(Form1->Canvas, TntControl_GetText(Control));
}
//---------------------------------------------------------------------------
AnsiString GetTempPath()
{
  unsigned Length = GetTempPath(0, NULL);
  std::vector<char> Result(Length);
  Win32Check(GetTempPath(Length, &Result[0]));
  return &Result[0];
}
//---------------------------------------------------------------------------
AnsiString GetTempFileName(const AnsiString &Prefix, const AnsiString &Ext)
{
  AnsiString FileName;
  int Count = 0;
  do
  {
    FileName = GetTempPath() + Prefix + Count + "." + Ext;
    Count++;
  }
  while(FileExists(FileName));
  
  return FileName;
}
//---------------------------------------------------------------------------






