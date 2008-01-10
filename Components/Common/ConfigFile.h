/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef ConfigFileH
#define ConfigFileH
//---------------------------------------------------------------------------
#include <string>
#include <deque>
#include <iosfwd>
#include <iomanip>
#include <algorithm>

class TConfigFile
{
  typedef std::deque<std::pair<std::string, std::string> > TSection;
  typedef std::deque<std::pair<std::string, TSection> > TConfigData;
  TConfigData ConfigData;
  std::string Comment;

  bool ReadString(const std::string &Section, const std::string &Key, std::string &Result) const;
  static std::string TrimString(const std::string &Str);

  struct TCmpString
  {
    const std::string &Str;
    TCmpString(const std::string &AStr) : Str(AStr) {}
    template<class T>
    bool operator()(const std::pair<std::string, T> &Pair) const {return Str == Pair.first;}
  };

public:
  typedef TSection::const_iterator TSectionIterator;

  TConfigFile() {}
  TConfigFile(const std::string &FileName);
  bool LoadFromFile(const std::string &FileName);
  bool SaveToFile(const std::string &FileName) const;
  void LoadFromStream(std::istream &Stream);
  void SaveToStream(std::ostream &Stream) const;
  void LoadFromString(const std::string &Str);
  std::string GetAsString() const;
  void SetComment(const std::string &Str);
  void Clear() {ConfigData.clear();}
  void DeleteKey(const std::string &Section, const std::string &Key);
  void DeleteSection(const std::string &Section);
  bool SectionExists(const std::string &Section) const;
  bool KeyExists(const std::string &Section, const std::string &Key) const;
  void Write(const std::string &Section, const std::string &Key, const std::string &Value);

  template<class T>
  void Write(const std::string &Section, const std::string &Key, const T &Value)
  {
    std::ostringstream Stream;
    Stream << std::setprecision(15) << std::uppercase << Value;
    Write(Section, Key, Stream.str());
  }

  template<class T>
  void Write(const std::string &Section, const std::string &Key, const T &Value, const T &Default)
  {
    if(Value == Default)
      DeleteKey(Section, Key);
    else
      Write(Section, Key, Value);
  }

  template<class T>
  T Read(const std::string &Section, const std::string &Key, const T &Default) const
  {
    try
    {
      TConfigData::const_iterator Iter = std::find_if(ConfigData.begin(), ConfigData.end(), TCmpString(Section));
      if(Iter != ConfigData.end())
      {
        //Section found
        TSection::const_iterator Iter2 = std::find_if(Iter->second.begin(), Iter->second.end(), TCmpString(Key));
        if(Iter2 != Iter->second.end())
        {
          //Key found
          std::istringstream Stream(Iter2->second);
          T Value;
          if(Stream >> Value)
            return Value;
        }
      }
    }
    //Just return default if any exceptions occur
    catch(...)
    {
    }
    return Default;
  }

  template<>
  std::string Read(const std::string &Section, const std::string &Key, const std::string &Default) const
  {
    TConfigData::const_iterator Iter = std::find_if(ConfigData.begin(), ConfigData.end(), TCmpString(Section));
    if(Iter != ConfigData.end())
    {
      //Section found
      TSection::const_iterator Iter2 = std::find_if(Iter->second.begin(), Iter->second.end(), TCmpString(Key));
      if(Iter2 != Iter->second.end())
        return Iter2->second;
    }
    return Default;
  }

  //Return a std::string if instantiated with a char* as default
  std::string Read(const std::string &Section, const std::string &Key, const char* Default) const
  {
    return Read<std::string>(Section, Key, Default);
  }

  //This will read as an int and cast to T, which should be an enum
  template<typename T>
  T ReadEnum(const std::string &Section, const std::string &Key, T Default) const
  {
    return static_cast<T>(Read<int>(Section, Key, Default));
  }

  unsigned SectionCount() const {return ConfigData.size();}
  const std::string& GetSection(unsigned Index) const {return ConfigData.at(Index).first;}
  std::pair<TSectionIterator, TSectionIterator> GetSectionData(const std::string &Section) const;
};

#endif
