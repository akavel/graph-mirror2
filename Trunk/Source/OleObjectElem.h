/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
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
//---------------------------------------------------------------------------
class TOleObjectElem : public TGraphElem
{
  boost::shared_ptr<TOleContainer> OleContainer;
  Func32::TDblPoint Pos;

public:
  TOleObjectElem() : OleContainer(new TOleContainer((TComponent*)NULL)) {}
  TOleObjectElem(const Func32::TDblPoint &APos) : Pos(APos), OleContainer(new TOleContainer((TComponent*)NULL)) {}
  std::wstring MakeText() const {return L"";}
  void WriteToIni(class TConfigFile &IniFile, const std::string &Section) const;
  void ReadFromIni(const TConfigFile &IniFile, const std::string &Section);
  void Accept(TGraphElemVisitor &v) {/*v.Visit(*this);*/}
  boost::shared_ptr<TGraphElem> Clone() const {return boost::shared_ptr<TGraphElem>(new TOleObjectElem(*this));}
  bool InsertObjectDialog() {return OleContainer->InsertObjectDialog();}
};
#endif
