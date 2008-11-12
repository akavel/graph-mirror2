/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Config.h"
#pragma hdrstop
#include <fstream>
#include "ConfigFile.h"
#include <sstream>
//---------------------------------------------------------------------------
////////////////////////
// TConfigFileSection //
////////////////////////
void TConfigFileSection::DeleteKey(const std::wstring &Key)
{
  TSection::iterator Iter = std::find_if(Section.begin(), Section.end(), TCmpString(Key));
  if(Iter != Section.end())
    //Key found;
    Section.erase(Iter);
}
//---------------------------------------------------------------------------
bool TConfigFileSection::KeyExists(const std::wstring &Key) const
{
  return  std::find_if(Section.begin(), Section.end(), TCmpString(Key)) != Section.end();
}
//---------------------------------------------------------------------------
void TConfigFileSection::Write(const std::wstring &Key, const std::wstring &Value)
{
  TSection::iterator Iter = std::find_if(Section.begin(), Section.end(), TCmpString(Key));
  if(Iter == Section.end())
    Section.push_back(std::make_pair(Key, Value));
  else
    Iter->second = Value;
}
//---------------------------------------------------------------------------
void TConfigFileSection::Write(const std::wstring &Key, const std::wstring &Value, const std::wstring &Default)
{
  if(Value == Default)
    DeleteKey(Key);
  else
    Write(Key, Value);
}
//---------------------------------------------------------------------------
/////////////////
// TConfigFile //
/////////////////
//Trim spaces around string
std::wstring TConfigFile::TrimString(const std::wstring &Str)
{
  unsigned First = Str.find_first_not_of(L" \t");
  unsigned Last = Str.find_last_not_of(L" \t\r"); //If Line feed is \r\n, \r may stay and only the \n is detected as end of line
  if(First == std::wstring::npos)
    return std::wstring();
  return std::wstring(Str, First, Last + 1 - First);
}
//---------------------------------------------------------------------------
TConfigFile::TConfigFile(const std::wstring &FileName)
{
  LoadFromUtf8File(FileName);
}
//---------------------------------------------------------------------------
bool TConfigFile::LoadFromAnsiFile(const std::wstring &FileName)
{
/*  std::ifwstream File(FileName.c_str());
  if(!File)
    return false;
  LoadFromStream(File);
  */
  return true;
}
//---------------------------------------------------------------------------
bool TConfigFile::LoadFromUtf8File(const std::wstring &FileName)
{
  return false;
}
//---------------------------------------------------------------------------
void TConfigFile::LoadFromStream(std::wistream &Stream)
{
  ConfigData.clear();
  std::wstring Line;

  while(std::getline(Stream, Line))
  {
    Line = TrimString(Line);
    if(Line.empty() || Line[0] == ';') //Ignore empty lines and lines with comments
      continue;

    if(Line[0] == '[' && Line[Line.size() - 1] == ']')
    {
      //Add new empty section
      ConfigData.push_back(TConfigFileSection(Line.substr(1, Line.size() - 2)));
      continue;
    }

    unsigned Pos = Line.find('=');
    if(Pos == std::wstring::npos || ConfigData.empty())
      continue; //Ignore Lies with errors

    std::wstring Key = TrimString(Line.substr(0, Pos));
    std::wstring Value = TrimString(Line.substr(Pos + 1));

    //Ignore empty keys
    if(!Key.empty())
      ConfigData.back().Section.push_back(std::make_pair(Key, Value));
  }
}
//---------------------------------------------------------------------------
bool TConfigFile::SaveToUtf8File(const std::wstring &FileName) const
{
/*
  std::ofstream File(FileName.c_str());
  if(!File)
    return false;
  SaveToStream(File);
*/
  return true;
}
//---------------------------------------------------------------------------
void TConfigFile::SaveToStream(std::wostream &Stream) const
{
  Stream << Comment << std::endl;
  for(TConfigData::const_iterator Iter = ConfigData.begin(); Iter != ConfigData.end(); ++Iter)
  {
    Stream << '[' << Iter->Name << ']' << std::endl;
    for(TConfigFileSection::TSection::const_iterator Iter2 = Iter->Section.begin(); Iter2 != Iter->Section.end(); ++Iter2)
      Stream << Iter2->first << " = " << Iter2->second << std::endl;
    Stream << std::endl;
  }
}
//---------------------------------------------------------------------------
//Set comment string, and add a ';' in front of each line
void TConfigFile::SetComment(const std::wstring &Str)
{
  Comment = Str;
  if(!Comment.empty())
  {
    unsigned Pos = -1;
    do
    {
      Comment.insert(Pos+1, L";");
      Pos = Comment.find('\n', Pos+2);
    } while(Pos != std::wstring::npos);
  }
}
//---------------------------------------------------------------------------
void TConfigFile::LoadFromString(const std::wstring &Str)
{
  LoadFromStream(std::wstringstream(Str));
}
//---------------------------------------------------------------------------
void TConfigFile::LoadFromUtf8String(const std::string &Str)
{
}
//---------------------------------------------------------------------------
std::wstring TConfigFile::GetAsString() const
{
  std::wstringstream Stream;
  SaveToStream(Stream);
  return Stream.str();
}
//---------------------------------------------------------------------------
void TConfigFile::DeleteSection(const std::wstring &Section)
{
  TConfigData::iterator Iter = std::find_if(ConfigData.begin(), ConfigData.end(), TCmpString(Section));
  if(Iter != ConfigData.end())
    //Section found
    ConfigData.erase(Iter);
}
//---------------------------------------------------------------------------
bool TConfigFile::SectionExists(const std::wstring &Section) const
{
  return std::find_if(ConfigData.begin(), ConfigData.end(), TCmpString(Section)) != ConfigData.end();
}
//---------------------------------------------------------------------------
TConfigFileSection& TConfigFile::Section(const std::wstring &ASection)
{
  TConfigData::iterator Iter = std::find_if(ConfigData.begin(), ConfigData.end(), TCmpString(ASection));
  if(Iter == ConfigData.end())
  {
    //Create new empty section
    ConfigData.push_back(TConfigFileSection(ASection));
    return ConfigData.back();
  }
  return *Iter;
}
//---------------------------------------------------------------------------
  const TConfigFileSection& TConfigFile::Section(const std::wstring &ASection) const
{
  TConfigData::const_iterator Iter = std::find_if(ConfigData.begin(), ConfigData.end(), TCmpString(ASection));
  if(Iter != ConfigData.end())
    //Section found
    return *Iter;

  //If no section exist, return an empty section
  static const TConfigFileSection EmptySection;
  return EmptySection;
}
//---------------------------------------------------------------------------

