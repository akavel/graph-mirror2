/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef OleObjectElemH
#define OleObjectElemH
#include "GraphElem.h"
#include <OleCtnrs.hpp>
//---------------------------------------------------------------------------
class TOleObjectElem : public TGraphElem
{
  boost::shared_ptr<TOleContainer> OleContainer;
  Func32::TDblPoint Pos;

public:
  TOleObjectElem();
  TOleObjectElem(const Func32::TDblPoint &APos);
  std::wstring MakeText() const;
  void WriteToIni(class TConfigFileSection &IniFile) const;
  void ReadFromIni(const TConfigFileSection &IniFile);
  void Accept(TGraphElemVisitor &v) {v.Visit(*this);}
  boost::shared_ptr<TGraphElem> Clone() const {return boost::shared_ptr<TGraphElem>(new TOleObjectElem(*this));}
  bool InsertObjectDialog();
  bool Edit();
  void Copy();
};
#endif
