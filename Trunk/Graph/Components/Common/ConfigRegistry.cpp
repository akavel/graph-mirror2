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
#include <fstream>
#include <iomanip>
//---------------------------------------------------------------------------
/** Check the result of a registry operation.
 *  \param ErrorCode: The result to check.
 *  \throw ERegistryError if the ErrorCode is not ERROR_SUCCESS.
 */
void CheckRegistryResult(DWORD ErrorCode)
{
  if(ErrorCode != ERROR_SUCCESS)
  {
    char Str[100];
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, ErrorCode, 0, Str, sizeof(Str), NULL);
    throw ERegistryError(ErrorCode, Str);
  }
}
//---------------------------------------------------------------------------
/** Create new TConfigRegistry.
 *  \param Key: Name of key to open.
 *  \param RootKey: Root key. Defaults to HKEY_CURRENT_USER.
 *  \param Create: If true, attempts to create the key if doesn't exist.
 *  \throw ERegistryError if Create is true and the key could not be created.
 */
TConfigRegistry::TConfigRegistry(const std::wstring &Key, HKEY RootKey, bool Create)
  : Handle(NULL)
{
  if(Create)
    CheckRegistryResult(RegCreateKeyEx(RootKey, GetKey(Key).c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &Handle, NULL));
  else
    RegOpenKeyEx(RootKey, GetKey(Key).c_str(), 0, KEY_READ, &Handle);
}
//---------------------------------------------------------------------------
/** Replaces / with \ in key name.
 */
std::wstring TConfigRegistry::GetKey(const std::wstring &Key)
{
  //Replace '/' in Key with '\\'
  std::wstring Result = Key;
  std::replace(Result.begin(), Result.end(), L'/', L'\\');
  return Result;
}
//---------------------------------------------------------------------------
/** Create a new sub key.
 *  \param Key: Name of new sub key.
 *  \param RootKey: Root key. Defaults to HKEY_CURRENT_USER.
 *  \return true if the key was created or already existed. false on error.
 */
bool TConfigRegistry::CreateKey(const std::wstring &Key, HKEY RootKey)
{
  CloseKey();
  return RegCreateKeyEx(RootKey, GetKey(Key).c_str(), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &Handle, NULL) == ERROR_SUCCESS;
}
//---------------------------------------------------------------------------
/** Open a sub key.
 *  \param Key: Name of sub key to open.
 *  \param RootKey: Root key. Defaults to HKEY_CURRENT_USER.
 *  \param ReadOnly: If true (default), the key is opened with read only access.
 */
bool TConfigRegistry::OpenKey(const std::wstring &Key, HKEY RootKey, bool ReadOnly)
{
  CloseKey();
  return RegOpenKeyEx(RootKey, GetKey(Key).c_str(), 0, ReadOnly ? KEY_READ : KEY_ALL_ACCESS, &Handle) == ERROR_SUCCESS;
}
//---------------------------------------------------------------------------
/** Close the key handle if it is open.
 */
void TConfigRegistry::CloseKey()
{
  if(Handle)
    RegCloseKey(Handle);
  Handle = NULL;
}
//---------------------------------------------------------------------------
/** Write a value to the open key.
 *  \param Name: Value name.
 *  \param Value: Value to set.
 */
void TConfigRegistry::Write(const std::string &Name, const std::string &Value)
{
  RegSetValueExA(Handle, Name.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(Value.c_str()), Value.size() + 1);
}
//---------------------------------------------------------------------------
/** Write a value to the open key.
 *  \param Name: Value name.
 *  \param Value: Value to set.
 */
void TConfigRegistry::Write(const std::wstring &Name, const std::wstring &Value)
{
  RegSetValueEx(Handle, Name.c_str(), 0, REG_SZ, reinterpret_cast<const BYTE*>(Value.c_str()), 2*(Value.size() + 1));
}
//---------------------------------------------------------------------------
/** Write a value to the open key.
 *  \param Name: Value name.
 *  \param Value: Value to set.
 */
void TConfigRegistry::Write(const std::wstring &Name, int Value)
{
  RegSetValueEx(Handle, Name.c_str(), 0, REG_DWORD, reinterpret_cast<const BYTE*>(&Value), sizeof(Value));
}
//---------------------------------------------------------------------------
/** Write a value to the open key.
 *  \param Name: Value name.
 *  \param Value: Value to set.
 */
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
    throw ERegistryError(0, "Registry value is of wrong type.");
  int Result;
  DWORD Size = sizeof(Result);
  CheckRegistryResult(RegQueryValueEx(Handle, Name.c_str(), 0, NULL, reinterpret_cast<BYTE*>(&Result), &Size));
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
    throw ERegistryError(0, "Registry value is of wrong type.");
  int Result;
  DWORD Size = sizeof(Result);
  CheckRegistryResult(RegQueryValueExA(Handle, Name.c_str(), 0, NULL, reinterpret_cast<BYTE*>(&Result), &Size));
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
long long TConfigRegistry::Read(const std::wstring &Name, long long Default) const
{
  return Read(Name, static_cast<unsigned long long>(Default));
}
//---------------------------------------------------------------------------
unsigned long long TConfigRegistry::Read(const std::wstring &Name, unsigned long long Default) const
{
  if(GetValueSize(Name, REG_QWORD) == 0)
    return Default;
  unsigned long long Result;
  DWORD Size = sizeof(Result);
  CheckRegistryResult(RegQueryValueEx(Handle, Name.c_str(), 0, NULL, reinterpret_cast<BYTE*>(&Result), &Size));
  return Result;
}
//---------------------------------------------------------------------------
void TConfigRegistry::Read(const std::string &Name, std::vector<BYTE> &Data) const
{
  DWORD Size;
  CheckRegistryResult(RegQueryValueExA(Handle, Name.c_str(), 0, NULL, NULL, &Size));
  Data.resize(Size);
  CheckRegistryResult(RegQueryValueExA(Handle, Name.c_str(), 0, NULL, &Data[0], &Size));
}
//---------------------------------------------------------------------------
void TConfigRegistry::Read(const std::wstring &Name, std::vector<BYTE> &Data) const
{
  DWORD Size;
  CheckRegistryResult(RegQueryValueEx(Handle, Name.c_str(), 0, NULL, NULL, &Size));
  Data.resize(Size);
  CheckRegistryResult(RegQueryValueEx(Handle, Name.c_str(), 0, NULL, &Data[0], &Size));
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
//Delete key and all sub keys
void TConfigRegistry::DeleteKey(const std::wstring &Key)
{
  TConfigRegistry SubRegistry;
  if(SubRegistry.OpenKey(Key, Handle, false))
  {
    unsigned SubKeys = SubRegistry.NumSubKeys();
    if(SubKeys > 0)
    {
      for(int I = SubKeys-1; I >= 0; I--)  //Count down to prevent looping through deleted keys
        SubRegistry.DeleteKey(SubRegistry.SubKey(I));
    }
    SubRegistry.CloseKey();
    CheckRegistryResult(RegDeleteKey(Handle, Key.c_str()));
  }
}
//---------------------------------------------------------------------------
unsigned TConfigRegistry::NumSubKeys() const
{
  DWORD SubKeys;
  CheckRegistryResult(RegQueryInfoKey(Handle, NULL, NULL, NULL, &SubKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL));
  return SubKeys;
}
//---------------------------------------------------------------------------
unsigned TConfigRegistry::NumValues() const
{
  DWORD Values;
  CheckRegistryResult(RegQueryInfoKey(Handle, NULL, NULL, NULL, NULL, NULL, NULL, &Values, NULL, NULL, NULL, NULL));
  return Values;
}
//---------------------------------------------------------------------------
std::wstring TConfigRegistry::SubKey(unsigned Index)
{
  wchar_t Str[100];
  DWORD Size = sizeof(Str)/sizeof(Str[0]);
  CheckRegistryResult(RegEnumKeyEx(Handle, Index, Str, &Size, NULL, NULL, NULL, NULL));
  return std::wstring(Str, Size);
}
//---------------------------------------------------------------------------
std::wstring TConfigRegistry::ValueName(unsigned Index, TRegistryValueType &ValueType)
{
  wchar_t Str[100];
  DWORD Size = sizeof(Str)/sizeof(Str[0]);
  DWORD Type;
  CheckRegistryResult(RegEnumValue(Handle, Index, Str, &Size, NULL, &Type, NULL, NULL));
  ValueType = static_cast<TRegistryValueType>(Type);
  return std::wstring(Str, Size);
}
//---------------------------------------------------------------------------
//Copyed from http://stackoverflow.com/questions/937044/determine-path-to-registry-key-from-hkey-handle-in-c
std::wstring TConfigRegistry::GetKeyPath() const
{
  std::wstring keyPath;
  HKEY Key = Handle;
  if(Key != NULL)
  {
    HMODULE dll = LoadLibrary(L"ntdll.dll");
    if(dll != NULL)
    {
      typedef DWORD (__stdcall *NtQueryKeyType)(
          HANDLE  KeyHandle,
          int KeyInformationClass,
          PVOID  KeyInformation,
          ULONG  Length,
          PULONG  ResultLength);

      NtQueryKeyType func = reinterpret_cast<NtQueryKeyType>(::GetProcAddress(dll, "NtQueryKey"));
      if(func != NULL)
      {
        DWORD size = 0;
        DWORD result = 0;
        result = func(Key, 3, 0, 0, &size);
        if(result == 0xC0000023 /* STATUS_BUFFER_TOO_SMALL */)
        {
          size = size + 2;
          wchar_t* buffer = new wchar_t[size/sizeof(wchar_t)]; // size is in bytes
          result = func(Key, 3, buffer, size, &size);
          if (result == 0 /* STATUS_SUCCESS */)
          {
            buffer[size / sizeof(wchar_t)] = L'\0';
            keyPath = std::wstring(buffer + 2);
          }
          delete[] buffer;
        }
      }
      FreeLibrary(dll);
    }
  }
  return keyPath;
}
//---------------------------------------------------------------------------
const char* KeyToName(HKEY Key)
{
  if(Key == NULL) return "NULL";
  else if(Key == HKEY_CLASSES_ROOT) return "HKEY_CLASSES_ROOT";
  else if(Key == HKEY_CURRENT_USER) return "HKEY_CURRENT_USER";
  else if(Key == HKEY_LOCAL_MACHINE) return "HKEY_LOCAL_MACHINE";
  else if(Key == HKEY_USERS) return "HKEY_USERS";
  else if(Key == HKEY_PERFORMANCE_DATA) return "HKEY_PERFORMANCE_DATA";
  else if(Key == HKEY_PERFORMANCE_TEXT) return "HKEY_PERFORMANCE_TEXT";
  else if(Key == HKEY_PERFORMANCE_NLSTEXT) return "HKEY_PERFORMANCE_NLSTEXT";
  else if(Key == HKEY_CURRENT_CONFIG) return "HKEY_CURRENT_CONFIG";
  else if(Key == HKEY_DYN_DATA) return "HKEY_DYN_DATA";
  else if(Key == HKEY_CURRENT_USER_LOCAL_SETTINGS) return "HKEY_CURRENT_USER_LOCAL_SETTINGS";
  return "Unknown";
}
//---------------------------------------------------------------------------
void ExportRegistryKey(const std::wstring &Key, HKEY RootKey, const std::wstring &FileName)
{
  std::ofstream File(FileName.c_str());
  if(!File)
    throw ERegistryError(-1, "Failed to create file \"" + std::string(FileName.begin(), FileName.end()) + "\"");
  File << "REGEDIT4" << std::endl << std::endl;
  ExportRegistryKey(Key, RootKey, File);
}
//---------------------------------------------------------------------------
std::string Encode(const std::wstring &Str)
{
  std::string Result;
  for(unsigned I = 0; I < Str.size(); I++)
    switch(Str[I])
    {
      case '\"':
        Result += "\\\"";
        break;
      case '\\':
        Result += "\\\\";
        break;
      default:
        Result += Str[I];
        break;
    }
  return Result;
}
//---------------------------------------------------------------------------
void StreamBinary(std::ostream &Stream, const std::vector<BYTE> &Data)
{
  Stream << std::hex << std::setfill('0');
  for(unsigned I = 0; I < Data.size() - 1; I++)
    Stream << std::setw(2) << static_cast<unsigned>(Data[I]) << ',';
  Stream << std::setw(2) << static_cast<unsigned>(Data.back());
}
//---------------------------------------------------------------------------
void ExportRegistryKey(const std::wstring &Key, HKEY RootKey, std::ostream &Stream)
{
  Stream << "[" << KeyToName(RootKey) << '\\' << std::string(Key.begin(), Key.end()) << "]" << std::endl;

  TConfigRegistry Registry(Key, RootKey);
  unsigned Count = Registry.NumValues();
  for(unsigned I = 0; I < Count; I++)
  {
    TRegistryValueType ValueType;
    std::wstring Name = Registry.ValueName(I, ValueType);
    if(Name.empty())
      Stream << "@=";
    else
      Stream << "\"" << std::string(Name.begin(), Name.end()) << "\"=";
    switch(ValueType)
    {
      case regDWord:
        Stream << "dword:" << std::hex << std::setfill('0') << std::setw(8) << Registry.Read(Name, 0);
        break;
      case regSZ:
        Stream << "\"" << Encode(Registry.Read(Name, L"")) << "\"";
        break;
      case regQWord:
      case regBinary:
      case regExpandSZ:
      case regMultiSZ:
      {
        std::vector<BYTE> Data;
        Registry.Read(std::string(Name.begin(), Name.end()), Data); //REGEDIT4 format uses ANSI
        if(ValueType == regBinary)
          Stream << "hex:";
        else
          Stream << "hex(" << std::hex << ValueType << "):";
        StreamBinary(Stream, Data);
        break;
      }
      case regLink:
      case regDWordBigEndian:
      case regNone:
      default:
        throw ERegistryError(ValueType, "ExportRegistryKey does not support the given value type.");
    }
    Stream << std::endl;
  }
  Stream << std::endl;
  unsigned SubKeys = Registry.NumSubKeys();
  for(unsigned I = 0; I < SubKeys; I++)
    ExportRegistryKey(Key + L'\\' + Registry.SubKey(I), RootKey, Stream);
}
//---------------------------------------------------------------------------
//Remove Key and all subkeys from the registry
void RemoveRegistryKey(const std::wstring &Key, HKEY RootKey)
{
  size_t Pos = Key.find(L'\\');
  std::wstring ParentKey = Key.substr(0, Pos == std::wstring::npos ? 0 : Pos);
  std::wstring KeyToDelete = Key.substr(Pos + 1, std::wstring::npos);
  TConfigRegistry Registry;
  if(Registry.OpenKey(ParentKey, RootKey, false))
    Registry.DeleteKey(KeyToDelete);
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
void SetRegValue(const std::wstring &Key, const std::wstring &ValueName, HKEY RootKey, const std::wstring &Value)
{
  try
  {
    TConfigRegistry Registry(Key, RootKey, true);
    Registry.Write(ValueName, Value);
  }
  catch(ERegistryError &E)
  {
  }
}
//---------------------------------------------------------------------------
void SetRegValue(const std::wstring &Key, const std::wstring &ValueName, HKEY RootKey, unsigned Value)
{
  try
  {
    TConfigRegistry Registry(Key, RootKey, true);
    Registry.Write(ValueName, Value);
  }
  catch(ERegistryError &E)
  {
  }
}
//---------------------------------------------------------------------------


