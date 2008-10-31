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
//Trim spaces around string
std::string TConfigFile::TrimString(const std::string &Str)
{
  unsigned First = Str.find_first_not_of(" \t");
  unsigned Last = Str.find_last_not_of(" \t\r"); //If Line feed is \r\n, \r may stay and only the \n is detected as end of line
  if(First == std::string::npos)
    return std::string();
  return std::string(Str, First, Last + 1 - First);
}
//---------------------------------------------------------------------------
TConfigFile::TConfigFile(const std::string &FileName)
{
  LoadFromFile(FileName);
}
//---------------------------------------------------------------------------
bool TConfigFile::LoadFromFile(const std::string &FileName)
{
  std::ifstream File(FileName.c_str());
  if(!File)
    return false;
  LoadFromStream(File);
  return true;
}
//---------------------------------------------------------------------------
void TConfigFile::LoadFromStream(std::istream &Stream)
{
  ConfigData.clear();
  std::string Line;

  while(std::getline(Stream, Line))
  {
    Line = TrimString(Line);
    if(Line.empty() || Line[0] == ';') //Ignore empty lines and lines with comments
      continue;

    if(Line[0] == '[' && Line[Line.size() - 1] == ']')
    {
      ConfigData.push_back(std::make_pair(Line.substr(1, Line.size() - 2), TSection()));
      continue;
    }

    unsigned Pos = Line.find('=');
    if(Pos == std::string::npos || ConfigData.empty())
      continue; //Ignore Lies with errors

    std::string Key = TrimString(Line.substr(0, Pos));
    std::string Value = TrimString(Line.substr(Pos + 1));

    //Ignore empty keys
    if(!Key.empty())
      ConfigData.back().second.push_back(std::make_pair(Key, Value));
  }
}
//---------------------------------------------------------------------------
bool TConfigFile::SaveToFile(const std::string &FileName) const
{
  std::ofstream File(FileName.c_str());
  if(!File)
    return false;
  SaveToStream(File);
  return true;
}
//---------------------------------------------------------------------------
void TConfigFile::SaveToStream(std::ostream &Stream) const
{
  Stream << Comment << std::endl;
  for(TConfigData::const_iterator Iter = ConfigData.begin(); Iter != ConfigData.end(); ++Iter)
  {
    Stream << '[' << Iter->first << ']' << std::endl;
    for(TSection::const_iterator Iter2 = Iter->second.begin(); Iter2 != Iter->second.end(); ++Iter2)
      Stream << Iter2->first << " = " << Iter2->second << std::endl;
    Stream << std::endl;
  }
}
//---------------------------------------------------------------------------
//Set comment string, and add a ';' in front of each line
void TConfigFile::SetComment(const std::string &Str)
{
  Comment = Str;
  if(!Comment.empty())
  {
    unsigned Pos = -1;
    do
    {
      Comment.insert(Pos+1, ";");
      Pos = Comment.find('\n', Pos+2);
    } while(Pos != std::string::npos);
  }
}
//---------------------------------------------------------------------------
void TConfigFile::LoadFromString(const std::string &Str)
{
  LoadFromStream(std::stringstream(Str));
}
//---------------------------------------------------------------------------
std::string TConfigFile::GetAsString() const
{
  std::stringstream Stream;
  SaveToStream(Stream);
  return Stream.str();
}
//---------------------------------------------------------------------------
void TConfigFile::DeleteKey(const std::string &Section, const std::string &Key)
{
  TConfigData::iterator Iter = std::find_if(ConfigData.begin(), ConfigData.end(), TCmpString(Section));
  if(Iter != ConfigData.end())
  {
    //Section found
    TSection::iterator Iter2 = std::find_if(Iter->second.begin(), Iter->second.end(), TCmpString(Key));
    if(Iter2 != Iter->second.end())
      //Key found;
      Iter->second.erase(Iter2);
  }
}
//---------------------------------------------------------------------------
void TConfigFile::DeleteSection(const std::string &Section)
{
  TConfigData::iterator Iter = std::find_if(ConfigData.begin(), ConfigData.end(), TCmpString(Section));
  if(Iter != ConfigData.end())
    //Section found
    ConfigData.erase(Iter);
}
//---------------------------------------------------------------------------
bool TConfigFile::SectionExists(const std::string &Section) const
{
  return std::find_if(ConfigData.begin(), ConfigData.end(), TCmpString(Section)) != ConfigData.end();
}
//---------------------------------------------------------------------------
bool TConfigFile::KeyExists(const std::string &Section, const std::string &Key) const
{
  TConfigData::const_iterator Iter = std::find_if(ConfigData.begin(), ConfigData.end(), TCmpString(Section));
  if(Iter != ConfigData.end())
    //Section found
    return  std::find_if(Iter->second.begin(), Iter->second.end(), TCmpString(Key)) != Iter->second.end();
  return false;
}
//---------------------------------------------------------------------------
void TConfigFile::Write(const std::string &Section, const std::string &Key, const std::string &Value)
{
  TConfigData::iterator Iter = std::find_if(ConfigData.begin(), ConfigData.end(), TCmpString(Section));
  if(Iter == ConfigData.end())
    Iter = ConfigData.insert(ConfigData.end(), std::make_pair(Section, TSection()));

  TSection::iterator Iter2 = std::find_if(Iter->second.begin(), Iter->second.end(), TCmpString(Key));
  if(Iter2 == Iter->second.end())
    Iter->second.push_back(std::make_pair(Key, Value));
  else
    Iter2->second = Value;
}
//---------------------------------------------------------------------------
std::pair<TConfigFile::TSectionIterator, TConfigFile::TSectionIterator> TConfigFile::GetSectionData(const std::string &Section) const
{
  TConfigData::const_iterator Iter = std::find_if(ConfigData.begin(), ConfigData.end(), TCmpString(Section));
  if(Iter != ConfigData.end())
    //Section found
    return make_pair(Iter->second.begin(), Iter->second.end());
  throw std::runtime_error("Section " + Section + " not found!");
}
//---------------------------------------------------------------------------

