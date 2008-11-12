/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2005 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Config.h"
#pragma hdrstop
#include <Windows.h>
#include <algorithm>
#include <vector>
#include "ConfigRegistry.h"
//---------------------------------------------------------------------------
TConfigRegistry::TConfigRegistry(const std::wstring &Key, HKEY RootKey, bool ReadOnly)
  : Handle(NULL)
{
  if(ReadOnly)
    OpenKey(Key, RootKey);
  else
    CreateKey(Key, RootKey);
}
//---------------------------------------------------------------------------
std::wstring TConfigRegistry::GetKey(const std::wstring &Key)
{
  //Replace '/' in Key with '\\'
  std::wstring Result = Key;
  std::replace(Result.begin(), Result.end(), L'/', L'\\');
  return Result;
}
//---------------------------------------------------------------------------
bool TConfigRegistry::CreateKey(const std::wstring &Key, HKEY RootKey)
{
  CloseKey();
  return RegCreateKeyEx(RootKey, GetKey(Key).c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &Handle, NULL) == ERROR_SUCCESS;
}
//---------------------------------------------------------------------------
bool TConfigRegistry::OpenKey(const std::wstring &Key, HKEY RootKey)
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
  RegSetValueExA(Handle, Name.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(Value.c_str()), Value.size() + 1);
}
//---------------------------------------------------------------------------
void TConfigRegistry::Write(const std::wstring &Name, const std::wstring &Value)
{
  RegSetValueEx(Handle, Name.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(Value.c_str()), 2*(Value.size() + 1));
}
//---------------------------------------------------------------------------
void TConfigRegistry::Write(const std::wstring &Name, int Value)
{
  RegSetValueEx(Handle, Name.c_str(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&Value), sizeof(Value));
}
//---------------------------------------------------------------------------
void TConfigRegistry::Write(const std::string &Name, int Value)
{
  RegSetValueExA(Handle, Name.c_str(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&Value), sizeof(Value));
}
//---------------------------------------------------------------------------
unsigned TConfigRegistry::GetValueSize(const std::wstring &Name, unsigned ValueType) const
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
unsigned TConfigRegistry::GetValueSize(const std::string &Name, unsigned ValueType) const
{
  if(Handle == NULL)
    return 0;
  DWORD TempValueType;
  DWORD Size;
  if(RegQueryValueExA(Handle, Name.c_str(), 0, &TempValueType, NULL, &Size) != ERROR_SUCCESS)
    return 0;
  if(ValueType != REG_NONE && ValueType != TempValueType)
    return 0;
  return Size;
}
//---------------------------------------------------------------------------
std::wstring TConfigRegistry::Read(const std::wstring &Name, const std::wstring &Default) const
{
  DWORD Size = GetValueSize(Name, REG_SZ);
  if(Size == 0)
    return Default;
  std::vector<wchar_t> Result(Size/2);
  if(RegQueryValueEx(Handle, Name.c_str(), 0, NULL, reinterpret_cast<BYTE*>(&Result[0]), &Size) != ERROR_SUCCESS)
    return Default;
  return &Result[0];
}
//---------------------------------------------------------------------------
int TConfigRegistry::Read(const std::wstring &Name) const
{
  if(GetValueSize(Name, REG_DWORD) == 0)
    throw ERegistryError();
  int Result;
  DWORD Size = sizeof(Result);
  LONG Error = RegQueryValueEx(Handle, Name.c_str(), 0, NULL, reinterpret_cast<BYTE*>(&Result), &Size);
  if(Error != ERROR_SUCCESS)
    throw ERegistryError();
  return Result;
}
//---------------------------------------------------------------------------
int TConfigRegistry::Read(const std::wstring &Name, int Default) const
{
  try
  {
    return Read(Name);
  }
  catch(ERegistryError &E)
  {
    return Default;
  }
}
//---------------------------------------------------------------------------
int TConfigRegistry::Read(const std::string &Name) const
{
  if(GetValueSize(Name, REG_DWORD) == 0)
    throw ERegistryError();
  int Result;
  DWORD Size = sizeof(Result);
  LONG Error = RegQueryValueExA(Handle, Name.c_str(), 0, NULL, reinterpret_cast<BYTE*>(&Result), &Size);
  if(Error != ERROR_SUCCESS)
    throw ERegistryError();
  return Result;
}
//---------------------------------------------------------------------------
int TConfigRegistry::Read(const std::string &Name, int Default) const
{
  try
  {
    return Read(Name);
  }
  catch(ERegistryError &E)
  {
    return Default;
  }
}
//---------------------------------------------------------------------------
bool TConfigRegistry::Read(const std::wstring &Name, bool Default) const
{
  return Read(Name, static_cast<int>(Default));
}
//---------------------------------------------------------------------------
unsigned TConfigRegistry::Read(const std::wstring &Name, unsigned Default) const
{
  return Read(Name, static_cast<int>(Default));
}
//---------------------------------------------------------------------------
bool TConfigRegistry::KeyExists(const std::wstring &Key, HKEY RootKey)
{
  HKEY TempHandle;
  LONG Result = RegOpenKeyEx(RootKey, GetKey(Key).c_str(), 0, KEY_ALL_ACCESS, &TempHandle);
  if(Result == ERROR_SUCCESS)
    RegCloseKey(TempHandle);
  return Result == ERROR_SUCCESS;
}
//---------------------------------------------------------------------------
//Remove Key and all subkeys from the registry
void RemoveRegistryKey(const std::wstring &Key, HKEY RootKey)
{
  //To be implemented
/*
  TConfigRegistry Registry;
  int Pos = Key.find(L'\\');
  if(Pos == std::wstring::npos)
    Pos = 0;
  std::wstring ParentKey = Key.substr(0, Pos - 1);
  std::wstring KeyToDelete = Key.substr(Pos, Key.size());
  if(Registry.OpenKey(ParentKey, RootKey))
    Registry.DeleteKey(KeyToDelete);*/
}
//---------------------------------------------------------------------------
std::wstring GetRegValue(const std::wstring &Key, const std::wstring &ValueName, HKEY RootKey, const std::wstring &Default)
{
  return TConfigRegistry(Key, RootKey).Read(ValueName, Default);
}
//---------------------------------------------------------------------------
unsigned GetRegValue(const std::wstring &Key, const std::wstring &ValueName, HKEY RootKey, unsigned Default)
{
  return TConfigRegistry(Key, RootKey).Read(ValueName, Default);
}
//---------------------------------------------------------------------------
bool RegKeyExists(const std::wstring &Key, HKEY RootKey)
{
  return TConfigRegistry().OpenKey(Key, RootKey);
}
//---------------------------------------------------------------------------


