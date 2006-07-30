/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2005 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Graph.h"
#pragma hdrstop
#include "ConfigRegistry.h"
//---------------------------------------------------------------------------
std::string TConfigRegistry::GetKey(const std::string &Key)
{
  //Replace '/' in Key with '\\'
  std::string Result = Key;
  std::replace(Result.begin(), Result.end(), '/', '\\');
  return Result;
}
//---------------------------------------------------------------------------
bool TConfigRegistry::CreateKey(const std::string &Key, HKEY RootKey)
{
  CloseKey();
  return RegCreateKeyEx(RootKey, GetKey(Key).c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &Handle, NULL) == ERROR_SUCCESS;
}
//---------------------------------------------------------------------------
bool TConfigRegistry::OpenKey(const std::string &Key, HKEY RootKey)
{
  CloseKey();
  return RegOpenKeyEx(RootKey, GetKey(Key).c_str(), 0, KEY_ALL_ACCESS, &Handle) == ERROR_SUCCESS;
}
//---------------------------------------------------------------------------
void TConfigRegistry::CloseKey()
{
  if(Handle)
    RegCloseKey(Handle);
  Handle = NULL;
}
//---------------------------------------------------------------------------
void TConfigRegistry::Write(const std::string &Name, const std::string &Value)
{
  RegSetValueEx(Handle, Name.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(Value.c_str()), Value.size() + 1);
}
//---------------------------------------------------------------------------
void TConfigRegistry::Write(const std::string &Name, int Value)
{
  RegSetValueEx(Handle, Name.c_str(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&Value), sizeof(Value));
}
//---------------------------------------------------------------------------
unsigned TConfigRegistry::GetValueSize(const std::string &Name, unsigned ValueType) const
{
  if(Handle == NULL)
    return 0;
  DWORD TempValueType;
  DWORD Size;
  if(RegQueryValueEx(Handle, Name.c_str(), 0, &TempValueType, NULL, &Size) != ERROR_SUCCESS)
    return 0;
  if(ValueType != REG_NONE && ValueType != TempValueType)
    return 0;
  return Size;
}
//---------------------------------------------------------------------------
std::string TConfigRegistry::Read(const std::string &Name, const std::string &Default) const
{
  DWORD Size = GetValueSize(Name, REG_SZ);
  if(Size == 0)
    return Default;
  std::vector<char> Result(Size);
  if(RegQueryValueEx(Handle, Name.c_str(), 0, NULL, reinterpret_cast<BYTE*>(&Result[0]), &Size) != ERROR_SUCCESS)
    return Default;
  return &Result[0];
}
//---------------------------------------------------------------------------
int TConfigRegistry::Read(const std::string &Name, int Default) const
{
  if(GetValueSize(Name, REG_DWORD) == 0)
    return Default;
  int Result;
  DWORD Size = sizeof(Result);
  LONG Error = RegQueryValueEx(Handle, Name.c_str(), 0, NULL, reinterpret_cast<BYTE*>(&Result), &Size);
  if(Error != ERROR_SUCCESS)
    return Default;
  return Result;
}
//---------------------------------------------------------------------------
bool TConfigRegistry::KeyExists(const std::string &Key, HKEY RootKey)
{
  HKEY TempHandle;
  LONG Result = RegOpenKeyEx(RootKey, GetKey(Key).c_str(), 0, KEY_ALL_ACCESS, &TempHandle);
  if(Result == ERROR_SUCCESS)
    RegCloseKey(TempHandle);
  return Result == ERROR_SUCCESS;
}
//---------------------------------------------------------------------------


