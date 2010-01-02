/* Copyright 2007 Ivan Johansen
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
void TSymbolList::Add(const std::wstring &Key)
{
  std::wstring Name = ToLower(Key);
  if(!IsValidName(Name))
    throw EFuncError(ecIllegalName, Name);
  List[Name];
}
//---------------------------------------------------------------------------
void TSymbolList::Add(const std::wstring &Key, const std::wstring &Str, const std::vector<std::wstring> &Args)
{
  std::wstring Name = ToLower(Key);
  if(!IsValidName(Name))
    throw EFuncError(ecIllegalName, Name);
  List[Name].reset(new TCustomFunc(Str, Args));
}
//---------------------------------------------------------------------------
void TSymbolList::Add(const std::wstring &Key, const boost::shared_ptr<TBaseCustomFunc> &CustomFunc)
{
  std::wstring Name = ToLower(Key);
  if(!IsValidName(Name))
    throw EFuncError(ecIllegalName, Key);
  List[Name] = CustomFunc;
}
//---------------------------------------------------------------------------
boost::shared_ptr<TBaseCustomFunc> TSymbolList::Get(const std::wstring &Key) const
{
  TConstIterator Iter = List.find(ToLower(Key));
  if(Iter == List.end())
    return boost::shared_ptr<TCustomFunc>();
  return Iter->second;
}
//---------------------------------------------------------------------------
bool TSymbolList::Exists(const std::wstring &Key) const
{
  return List.count(ToLower(Key));
}
//---------------------------------------------------------------------------
void TSymbolList::Erase(const std::wstring &Key)
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


