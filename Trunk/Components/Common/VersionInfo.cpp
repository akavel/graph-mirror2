/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Config.h"
#include "VersionInfo.h"
#include <sstream>
#include <vector>
//---------------------------------------------------------------------------
unsigned TVersion::PackedVersion() const
{
  return ((unsigned)Major << 24) | ((unsigned)Minor << 16) | ((unsigned)Release << 8) | Build;
}
//---------------------------------------------------------------------------
TVersionInfo::TVersionInfo()
{
  LoadVersionInfo();
}
//---------------------------------------------------------------------------
TVersionInfo::TVersionInfo(const std::string &FileName) : FFileName(FileName)
{
  LoadVersionInfo();
}
//---------------------------------------------------------------------------
void TVersionInfo::SetFileName(const std::string &FileName)
{
  FFileName = FileName;
  LoadVersionInfo();
}
//---------------------------------------------------------------------------
std::string TVersion::Text() const
{
  std::ostringstream Stream;
  Stream << Major << '.' << Minor;
  if(Release || Build)
    Stream << '.' << Release;
  if(Build)
    Stream << '.' << Build;
  return Stream.str();
}
//---------------------------------------------------------------------------
void TVersionInfo::LoadVersionInfo()
{
  FInfoAvailable = false;
  if(FFileName.empty())
    FFileName = Application->ExeName.c_str();

  unsigned int puLen;//Length of file info structure
  DWORD Dummy;

  //Get size of Resource data
  int DataSize = GetFileVersionInfoSize(const_cast<char*>(FFileName.c_str()), &Dummy);
  if(DataSize == 0)
    return; //Return on error

  //Allocate memory to data
  std::vector<char> Data(DataSize);

  //Get version info from file
  if(GetFileVersionInfo(const_cast<char*>(FFileName.c_str()), 0, Data.size(), &Data[0]))
  {
    //Get pointer to file info structure
    VS_FIXEDFILEINFO *FileInfo;//Pointer to file info structure
    if(VerQueryValue(&Data[0],"\\", &(void*)FileInfo, &puLen))
    {
      FFileVersion.VersionMS = FileInfo->dwFileVersionMS;
      FFileVersion.VersionLS = FileInfo->dwFileVersionLS;
      FProductVersion.VersionMS = FileInfo->dwProductVersionMS;
      FProductVersion.VersionLS = FileInfo->dwProductVersionLS;

      FFileFlags = ffNone;
      if(FileInfo->dwFileFlags & VS_FF_DEBUG)
        FFileFlags |= ffDebug;
      if(FileInfo->dwFileFlags & VS_FF_PATCHED)
        FFileFlags |= ffPatched;
      if(FileInfo->dwFileFlags & VS_FF_PRERELEASE)
        FFileFlags |= ffPrerelease;
      if(FileInfo->dwFileFlags & VS_FF_PRIVATEBUILD)
        FFileFlags |= ffPrivateBuild;
      if(FileInfo->dwFileFlags & VS_FF_SPECIALBUILD)
        FFileFlags |= ffSpecialBuild;
      FFileType = FileInfo->dwFileType;
      FFileSubtype = FileInfo->dwFileSubtype;
    }
  }
  
  FInfoAvailable = true;
}
//---------------------------------------------------------------------------
std::string TVersionInfo::StringValue(const std::string &Ident) const
{
  unsigned int puLen;//Length of file info structure
  VS_FIXEDFILEINFO *FileInfo;//Pointer to file info structure
  DWORD Dummy;

  //Get size of Resource data
  int DataSize = GetFileVersionInfoSize(const_cast<char*>(FFileName.c_str()), &Dummy);
  if(DataSize == 0)
    return ""; //Return on error

  //Allocate memory to data
  std::vector<char> Data(DataSize);
  //Get version info from file
  if(!GetFileVersionInfo(const_cast<char*>(FFileName.c_str()), 0, Data.size(), &Data[0]))
    return "";

  // *** Now the Character translation tables ***
  struct TRANSLATION
  {
    WORD langID;
    WORD charset;
  }m_translation;

  if(!VerQueryValue(&Data[0], "VarFileInfo\\Translation", &(void*)FileInfo, &puLen))
    return "";

  m_translation = *(TRANSLATION*)FileInfo;
  // *** Now we are ready to Build Queries ***
  AnsiString Query = "StringFileInfo\\" + IntToHex(m_translation.langID, 4) + IntToHex(m_translation.charset, 4) + "\\" + Ident.c_str();
  if(!VerQueryValue(&Data[0], Query.c_str(), &(void*)FileInfo, &puLen))
    return "";

  std::string Str = reinterpret_cast<char*>(FileInfo);
  return Str;
}
//---------------------------------------------------------------------------
void TVersion::Init(const char *Str)
{
  Major = Minor = Release = Build = 0;

  std::stringstream Stream(Str);
  Stream >> *this;
}
//---------------------------------------------------------------------------
bool operator<(const TVersion &Ver1, const TVersion &Ver2)
{
  if(Ver1.Major == Ver2.Major)
    if(Ver1.Minor == Ver2.Minor)
      if(Ver1.Release == Ver2.Release)
        return Ver1.Build < Ver2.Build;
      else return Ver1.Release < Ver2.Release;
    else return Ver1.Minor < Ver2.Minor;
  else return Ver1.Major < Ver2.Major;
}
//---------------------------------------------------------------------------
bool operator==(const TVersion &Ver1, const TVersion &Ver2)
{
  return Ver1.Major == Ver2.Major && Ver1.Minor == Ver2.Minor &&
    Ver1.Release == Ver2.Release && Ver1.Build == Ver2.Build;
}
//---------------------------------------------------------------------------
bool operator!=(const TVersion &Ver1, const TVersion &Ver2)
{
  return !(Ver1 == Ver2);
}
//---------------------------------------------------------------------------
bool operator>(const TVersion &Ver1, const TVersion &Ver2)
{
  return !(Ver1 < Ver2) && !(Ver1 == Ver2);
}
//---------------------------------------------------------------------------
bool operator<=(const TVersion &Ver1, const TVersion &Ver2)
{
  return Ver1 < Ver2 || Ver1 == Ver2;
}
//---------------------------------------------------------------------------
bool operator>=(const TVersion &Ver1, const TVersion &Ver2)
{
  return !(Ver1 < Ver2);
}
//---------------------------------------------------------------------------
std::string GetWindowsVersion()
{
  OSVERSIONINFO VerInfo;
  VerInfo.dwOSVersionInfoSize = sizeof(VerInfo);
  std::ostringstream Stream;
  if(GetVersionEx(&VerInfo))
  {
    switch(VerInfo.dwPlatformId)
    {
      case VER_PLATFORM_WIN32s:
        Stream << "Win32s ";
        break;

      case VER_PLATFORM_WIN32_WINDOWS:
        if(VerInfo.dwMajorVersion == 4 && VerInfo.dwMinorVersion == 10)
          Stream << "Windows 98 ";
        else if(VerInfo.dwMajorVersion == 4 && VerInfo.dwMinorVersion == 90)
          Stream << "Windows Me ";
        else
          Stream << "Windows 95 ";
        break;

      case VER_PLATFORM_WIN32_NT:
        if(VerInfo.dwMajorVersion == 5 && VerInfo.dwMinorVersion == 0)
          Stream << "Windows 2000 ";
        else if(VerInfo.dwMajorVersion == 5 && VerInfo.dwMinorVersion == 1)
          Stream << "Windows XP ";
        else
          Stream << "Windows NT ";
        break;
    }

    Stream << VerInfo.dwMajorVersion << '.' << VerInfo.dwMinorVersion << ' ' << VerInfo.szCSDVersion;
  }
  return Stream.str();
}
//---------------------------------------------------------------------------
int TVersionInfo::GetLanguage() const
{
  unsigned int puLen;//Length of file info structure
  DWORD Dummy;

  //Get size of Resource data
  int DataSize = GetFileVersionInfoSize(const_cast<char*>(FFileName.c_str()), &Dummy);
  if(!DataSize)
    return 0; //Return on error

  //Allocate memory to data
  std::vector<char> Data(DataSize);

  //Get version info from file
  if(!GetFileVersionInfo(const_cast<char*>(FFileName.c_str()), 0, DataSize, &Data.front()))
    return 0;

  // *** Now the Character translation tables ***
  struct TRANSLATION
  {
    WORD langID;
    WORD charset;
  }m_translation;

  VS_FIXEDFILEINFO *FileInfo;//Pointer to file info structure
  VerQueryValue(&Data.front(), "VarFileInfo\\Translation", &(void*)FileInfo, &puLen);
  m_translation = *(TRANSLATION*)FileInfo;
  return m_translation.langID;
}
//---------------------------------------------------------------------------
std::string TVersionInfo::GetLanguageName() const
{
  int Lang = GetLanguage();
  if(Lang == 0)
    return "";

  char Buffer[100];
  if(GetLocaleInfo(Lang, LOCALE_SENGLANGUAGE, Buffer, sizeof(Buffer)))
    return std::string(Buffer);
  return "";
}
//---------------------------------------------------------------------------
std::ostream& operator<<(std::ostream &Stream, const TVersion &Ver)
{
  Stream << Ver.Text();
  return Stream;
}
//---------------------------------------------------------------------------
std::istream& operator>>(std::istream &Stream, TVersion &Ver)
{
  char Ch;
  Stream >> Ver.Major >> Ch >> Ver.Minor;
  if(!Stream)
    return Stream;

  //The last bit is optional  
  Stream >> Ch >> Ver.Release >> Ch >> Ver.Build;
  Stream.clear();
  return Stream;
}
//---------------------------------------------------------------------------

