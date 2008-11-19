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
#include "OleObjectElem.h"
#include "ConfigFile.h"
#include "Encode.h"
//---------------------------------------------------------------------------
////////////////////
// TOleObjectElem //
////////////////////
TOleObjectElem::TOleObjectElem()
  : OleContainer(new TOleContainer((TComponent*)NULL))
{
  OleContainer->Parent = Application->MainForm;
  OleContainer->AllowInPlace = false;
  OleContainer->Left = 200;
  OleContainer->Top = 100;
  SetShowInLegend(false);
}
//---------------------------------------------------------------------------
TOleObjectElem::TOleObjectElem(const Func32::TDblPoint &APos)
  : Pos(APos), OleContainer(new TOleContainer((TComponent*)NULL))
{
//  OleContainer->Parent = Form1->Panel2;
  OleContainer->AllowInPlace = false;
  OleContainer->Left = 200;
  OleContainer->Top = 100;
  SetShowInLegend(false);
}
//---------------------------------------------------------------------------
bool TOleObjectElem::InsertObjectDialog()
{
  if(!OleContainer->InsertObjectDialog())
    return false;
  if(OleContainer->NewInserted)
    OleContainer->DoVerb(0);
  return true;
}
//---------------------------------------------------------------------------
bool TOleObjectElem::Edit()
{
  OleContainer->Modified = false;
  OleContainer->DoVerb(0);
  return OleContainer->Modified;
}
//---------------------------------------------------------------------------
std::wstring TOleObjectElem::MakeText() const
{
  LPOLESTR Str = NULL;
  String Result;
  if(OleContainer->OleObjectInterface->GetUserType(USERCLASSTYPE_SHORT, &Str) == S_OK)
    Result = Str;
  return ToWString(Result);
}
//---------------------------------------------------------------------------
void TOleObjectElem::WriteToIni(TConfigFileSection &Section) const
{
  std::auto_ptr<TMemoryStream> Stream(new TMemoryStream);
  OleContainer->SaveToStream(Stream.get());
//  Section.Write(L"Pos", Pos);
//  Section.Write(L"Object", Base64Encode(Stream->Memory, Stream->Size));
}
//---------------------------------------------------------------------------
void TOleObjectElem::ReadFromIni(const TConfigFileSection  &Section)
{
  std::auto_ptr<TMemoryStream> Stream(new TMemoryStream);
  std::wstring Str = Section.Read(L"Object", L"");
  Stream->Size = (Str.size() * 3) / 4;
//  Stream->Size = Base64Decode(Str, Stream->Memory);
  OleContainer->LoadFromStream(Stream.get());
//  Pos = Section.Read(L"Pos", Func32::TDblPoint(0,0));
}
//---------------------------------------------------------------------------
void TOleObjectElem::Copy()
{
  OleContainer->Copy();
}
//---------------------------------------------------------------------------


