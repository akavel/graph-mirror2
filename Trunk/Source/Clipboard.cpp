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
#include "Clipbrd.hpp"
#include "Clipboard.h"
#include "VersionInfo.h"
#include "ConfigFile.h"
//---------------------------------------------------------------------------
TGraphClipboard GraphClipboard;
//---------------------------------------------------------------------------
TGraphClipboard::TGraphClipboard()
{
  ClipboardFormat = RegisterClipboardFormat(CLIPBOARDFORMAT);
  PngFormat = RegisterClipboardFormat("PNG");
}
//---------------------------------------------------------------------------
bool TGraphClipboard::HasData()
{
  return Clipboard()->HasFormat(ClipboardFormat);
}
//---------------------------------------------------------------------------
void TGraphClipboard::Copy(const TBaseFuncType *Func)
{
  TConfigFile IniFile;
  TData::WriteInfoToIni(IniFile);
  IniFile.Write("Data", "FuncCount", 1);
  Func->WriteToIni(IniFile, "Func1");

  std::string Str = IniFile.GetAsString();
  SetClipboardData(ClipboardFormat, Str.c_str(), Str.size());
}
//---------------------------------------------------------------------------
void TGraphClipboard::Copy(const TPointSeries *PointSeries)
{
  TConfigFile IniFile;
  TData::WriteInfoToIni(IniFile);
  IniFile.Write("Data", "PointSeriesCount", 1);
  PointSeries->WriteToIni(IniFile, "PointSeries1");

  std::string Str = IniFile.GetAsString();
  SetClipboardData(ClipboardFormat, Str.c_str(), Str.size());
}
//---------------------------------------------------------------------------
void TGraphClipboard::Copy(const TTextLabel *Label)
{
  TConfigFile IniFile;
  TData::WriteInfoToIni(IniFile);
  IniFile.Write("Data", "LabelCount", 1);
  Label->WriteToIni(IniFile, "Label1");

  std::string Str = IniFile.GetAsString();
  SetClipboardData(ClipboardFormat, Str.c_str(), Str.size());
}
//---------------------------------------------------------------------------
void TGraphClipboard::Copy(const TRelation *Relation)
{
  TConfigFile IniFile;
  TData::WriteInfoToIni(IniFile);
  IniFile.Write("Data", "RelationCount", 1);
  Relation->WriteToIni(IniFile, "Relation1");

  std::string Str = IniFile.GetAsString();
  SetClipboardData(ClipboardFormat, Str.c_str(), Str.size());
}
//---------------------------------------------------------------------------
void TGraphClipboard::Paste(TData &Data)
{
  try
  {
    if(!HasData())
      return;

    int DataSize = GetClipboardDataSize(ClipboardFormat);
    std::vector<char> Str(DataSize+1);  //It looks like we need a zero termination
    GetClipboardData(ClipboardFormat, &Str[0], DataSize);
    TConfigFile IniFile;
    IniFile.LoadFromString(&Str[0]);

    unsigned ElemNo = Data.ElemCount();

    if(Data.CheckIniInfo(IniFile))
      Data.LoadData(IniFile);

    UndoList.BeginMultiUndo();
    for(unsigned I = ElemNo; I < Data.ElemCount(); I++)
      UndoList.Push(TUndoAdd(Data.GetElem(I)));
    UndoList.EndMultiUndo();
  }
  catch(Func32::EFuncError &Error)
  {
    ShowErrorMsg(Error);
  }
}
//---------------------------------------------------------------------------
void TGraphClipboard::SetClipboardData(unsigned Format, const void *Data, unsigned DataSize)
{
  HGLOBAL DataHandle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, DataSize);
  if(DataHandle == 0)
    return;
  void *Ptr = GlobalLock(DataHandle);
  memcpy(Ptr, Data, DataSize);
  GlobalUnlock(DataHandle);
  Clipboard()->SetAsHandle(Format, (unsigned)DataHandle);
}
//---------------------------------------------------------------------------
bool TGraphClipboard::GetClipboardData(unsigned Format, void *Data, unsigned DataSize)
{
  Clipboard()->Open();
  HGLOBAL DataHandle = (void*)Clipboard()->GetAsHandle(Format);
  if(DataHandle == NULL || GlobalSize(DataHandle) > DataSize)
  {
    Clipboard()->Close();
    return false;
  }
  void *Ptr = GlobalLock(DataHandle);
  memcpy(Data, Ptr, DataSize);
  GlobalUnlock(DataHandle);
  Clipboard()->Close();
  return true;
}
//---------------------------------------------------------------------------
int TGraphClipboard::GetClipboardDataSize(unsigned Format)
{
  Clipboard()->Open();
  HGLOBAL DataHandle = (void*)Clipboard()->GetAsHandle(Format);
  int Size = GlobalSize(DataHandle);
  Clipboard()->Close();
  return Size;
}
//---------------------------------------------------------------------------
void TGraphClipboard::CopyPngData(const std::string &Str)
{
  SetClipboardData(PngFormat, Str.c_str(), Str.size());
}
//---------------------------------------------------------------------------








