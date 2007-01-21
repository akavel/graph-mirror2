/* Copyright 2006 Ivan Johansen
 *
 * Func32 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Func32.h"
#include "Func32Impl.h"
#pragma hdrstop
namespace Func32
{
//---------------------------------------------------------------------------
void TSymbolList::Add(const std::string &Key)
{
  std::string Name = ToLower(Key);
  if(!IsValidName(Name))
    throw EFuncError(ecIllegalName, Name);
  List[Name];
}
//---------------------------------------------------------------------------
void TSymbolList::Add(const std::string &Key, const std::string &Str, const std::vector<std::string> &Args)
{
  std::string Name = ToLower(Key);
  if(!IsValidName(Name))
    throw EFuncError(ecIllegalName, Name);
  List[Name].FuncData.reset(new TFuncData(Str, Args));
  List[Name].Args = Args;
}
//---------------------------------------------------------------------------
void TSymbolList::Add(const std::string &Key, const TCustomFunc &CustomFunc)
{
  std::string Name = ToLower(Key);
  if(!IsValidName(Name))
    throw EFuncError(ecIllegalName, Key);
  List[Name] = CustomFunc;
}
//---------------------------------------------------------------------------
const TCustomFunc& TSymbolList::Get(const std::string &Key) const
{
  TConstIterator Iter = List.find(ToLower(Key));
  if(Iter == List.end())
  {
    static TCustomFunc Empty;
    return Empty;
  }
  return Iter->second;
}
//---------------------------------------------------------------------------
bool TSymbolList::Exists(const std::string &Key) const
{
  return List.count(ToLower(Key));
}
//---------------------------------------------------------------------------
void TSymbolList::Erase(const std::string &Key)
{
  List.erase(ToLower(Key));
}
//---------------------------------------------------------------------------
void TSymbolList::Clear()
{
  List.clear();
}
//---------------------------------------------------------------------------
} //namespace Func32


