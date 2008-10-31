/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2005 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef ConfigRegistryH
#define ConfigRegistryH
//---------------------------------------------------------------------------
#include <string>

struct ERegistryError : std::exception
{
  const char* what() const throw() {return "Registry read error!";}
};

class TConfigRegistry
{
  HKEY Handle;

  static std::string GetKey(const std::string &Key);
  unsigned GetValueSize(const std::string &Name, unsigned ValueType = REG_NONE) const;

public:
  TConfigRegistry() : Handle(NULL) {}
  ~TConfigRegistry() {CloseKey();}
  bool CreateKey(const std::string &Key, HKEY RootKey = HKEY_CURRENT_USER);
  bool OpenKey(const std::string &Key, HKEY RootKey = HKEY_CURRENT_USER);
  void CloseKey();
  bool IsOpen() const {return Handle;}
  void Write(const std::string &Name, const std::string &Value);
  void Write(const std::string &Name, int Value);
  std::string Read(const std::string &Name, const std::string &Default) const;
  std::string Read(const std::string &Name, const char *Default) const {return Read(Name, std::string(Default));}
  int Read(const std::string &Name) const;
  int Read(const std::string &Name, int Default) const;
  bool Read(const std::string &Name, bool Default) const {return Read(Name, static_cast<int>(Default));}
  bool Read(const std::string &Name, unsigned Default) const {return Read(Name, static_cast<int>(Default));}
  template<typename T>
  T ReadEnum(const std::string &Name, const T &Default) const {return Read(Name, static_cast<int>(Default));}
  bool ValueExists(const std::string &Name) const {return GetValueSize(Name) != 0;}
  static bool KeyExists(const std::string &Key, HKEY RootKey = HKEY_CURRENT_USER);
};

#endif
