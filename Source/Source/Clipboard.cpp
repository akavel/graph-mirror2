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
#include <PngImage.hpp>
#include <Clipbrd.hpp>
#include "Clipboard.h"
#include "VersionInfo.h"
#include "ConfigFile.h"
//---------------------------------------------------------------------------
TGraphClipboard GraphClipboard;
//---------------------------------------------------------------------------
TGraphClipboard::TGraphClipboard()
{
  ClipboardFormat = RegisterClipboardFormat(CLIPBOARDFORMAT);
  PngFormat = RegisterClipboardFormat(L"PNG");
}
//---------------------------------------------------------------------------
bool TGraphClipboard::HasData()
{
  return Clipboard()->HasFormat(ClipboardFormat);
}
//---------------------------------------------------------------------------
void TGraphClipboard::Copy(const TConfigFile &ConfigFile)
{
  std::wstring Str = ConfigFile.GetAsString();
  SetClipboardData(ClipboardFormat, Str.c_str(), Str.size()*sizeof(wchar_t));
}
//---------------------------------------------------------------------------
  void TGraphClipboard::Copy(const TBaseFuncType *Func)
{
  TConfigFile IniFile;
  TData::WriteInfoToIni(IniFile);
  IniFile.Section(L"Data").Write(L"FuncCount", 1);
  Func->WriteToIni(IniFile.Section(L"Func1"));
  Copy(IniFile);
}
//---------------------------------------------------------------------------
void TGraphClipboard::Copy(const TPointSeries *PointSeries)
{
  TConfigFile IniFile;
  TData::WriteInfoToIni(IniFile);
  IniFile.Section(L"Data").Write(L"PointSeriesCount", 1);
  PointSeries->WriteToIni(IniFile.Section(L"PointSeries1"));
  Copy(IniFile);
}
//---------------------------------------------------------------------------
void TGraphClipboard::Copy(const TTextLabel *Label)
{
  TConfigFile IniFile;
  TData::WriteInfoToIni(IniFile);
  IniFile.Section(L"Data").Write(L"LabelCount", 1);
  Label->WriteToIni(IniFile.Section(L"Label1"));
  Copy(IniFile);
}
//---------------------------------------------------------------------------
void TGraphClipboard::Copy(const TRelation *Relation)
{
  TConfigFile IniFile;
  TData::WriteInfoToIni(IniFile);
  IniFile.Section(L"Data").Write(L"RelationCount", 1);
  Relation->WriteToIni(IniFile.Section(L"Relation1"));
  Copy(IniFile);
}
//---------------------------------------------------------------------------
void TGraphClipboard::Paste(TData &Data)
{
  try
  {
    if(!HasData())
      return;

    int DataSize = GetClipboardDataSize(ClipboardFormat);
    std::wstring Str(DataSize/sizeof(wchar_t), 0);
    GetClipboardData(ClipboardFormat, &Str[0], DataSize);
    TConfigFile IniFile;
    IniFile.LoadFromString(Str);

    unsigned ElemNo = Data.ElemCount();

    if(Data.CheckIniInfo(IniFile))
      Data.LoadData(IniFile);

    UndoList.BeginMultiUndo();
    for(unsigned I = ElemNo; I < Data.ElemCount(); I++)
      UndoList.Push(TUndoAdd(Data, Data.GetElem(I)));
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
void TGraphClipboard::CopyPngData(Graphics::TBitmap *Bitmap)
{
  std::auto_ptr<TMemoryStream> Stream(new TMemoryStream);
  Bitmap->PixelFormat = pf8bit; //Change bitmap to 8 bit
  std::auto_ptr<TPngImage> PngImage(new TPngImage);
  PngImage->Assign(Bitmap);
  PngImage->SaveToStream(Stream.get());
  SetClipboardData(PngFormat, Stream->Memory, Stream->Size);
}
//---------------------------------------------------------------------------


















