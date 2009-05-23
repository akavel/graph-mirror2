/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef GraphUndoH
#define GraphUndoH
#include "Undo.h"

namespace Graph
{
struct TUndoAdd
{
  TData &Data;
  boost::shared_ptr<TGraphElem> GraphElem;

  TUndoAdd(TData &AData, const boost::shared_ptr<TGraphElem> &AGraphElem)
    : Data(AData), GraphElem(AGraphElem) {}
  void operator()(TUndoList &UndoList) const;
};

struct TUndoChange
{
  TData &Data;
  boost::shared_ptr<TGraphElem> Elem;
  unsigned Index;

  TUndoChange(TData &AData, const boost::shared_ptr<TGraphElem> &AElem, unsigned AIndex)
    : Data(AData), Elem(AElem), Index(AIndex) {Elem->ClearCache();}
  void operator()(TUndoList &UndoList) const
  {
    UndoList.Push(TUndoChange(Data, Data.Replace(Index, Elem), Index));
  }
};

//Special class for use with TBaseFuncBase changes; Use this instead of TUndoChange
struct TUndoChangeFunc
{
  TData &Data;
  boost::shared_ptr<TBaseFuncType> OldFunc;
  boost::shared_ptr<TBaseFuncType> NewFunc;

  TUndoChangeFunc(TData &AData, const boost::shared_ptr<TBaseFuncType> &AOldFunc, const boost::shared_ptr<TBaseFuncType> &ANewFunc)
    : Data(AData), OldFunc(AOldFunc), NewFunc(ANewFunc)
  {
    OldFunc->ClearCache();
  }

  void operator()(TUndoList &UndoList) const
  {
    UndoList.Push(TUndoChangeFunc(Data, NewFunc, OldFunc));
    Data.Replace(Data.GetIndex(NewFunc), OldFunc);
  }
};

struct TUndoDel
{
  TData &Data;
  int Index;
  boost::shared_ptr<TGraphElem> GraphElem;

  TUndoDel(TData &AData, const boost::shared_ptr<TGraphElem> &AGraphElem, int AIndex)
    : Data(AData), Index(AIndex), GraphElem(AGraphElem)  { }
  void operator()(TUndoList &UndoList) const
  {
    UndoList.Push(TUndoAdd(Data, GraphElem));
    Data.Insert(GraphElem, Index);
  }
};

inline void TUndoAdd::operator()(TUndoList &UndoList) const
{
  UndoList.Push(TUndoDel(Data, GraphElem, Data.GetIndex(GraphElem)));
  Data.Delete(GraphElem);
}


template<class T>
struct TUndoObject
{
  T &OrgObject;
  T Object;
  TUndoObject(T &AOrgObject, const T &AObject) : OrgObject(AOrgObject), Object(AObject) {}
  void operator()(TUndoList &UndoList)
  {
    UndoList.Push(TUndoObject<T>(OrgObject, OrgObject));
    OrgObject = Object;
  }
};

struct TUndoAxes
{
  TAxes Axes;
  TData &Data;
  TUndoAxes(TData &AData) : Data(AData), Axes(AData.Axes) {}
  void operator()(TUndoList &UndoList)
  {
    UndoList.Push(TUndoAxes(Data));
    Data.Axes = Axes;
    Data.ClearCache();
    Data.Update(); //In case trigonmetry has changed
  }
};

struct TUndoCustomFunctions
{
  TCustomFunctions CustomFunctions;
  TData &Data;
  TUndoCustomFunctions(TData &AData)
    : Data(AData), CustomFunctions(AData.CustomFunctions) {}
  void operator()(TUndoList &UndoList)
  {
    Data.CustomFunctions.Swap(CustomFunctions);
    UndoList.Push(*this);
    Data.Update();
    Data.ClearCache();
  }
};

struct TUndoMove
{
  TData &Data;
  TGraphElemPtr Elem;
  unsigned Index;

  TUndoMove(TData &AData, const TGraphElemPtr &AElem, unsigned AIndex)
    : Data(AData), Elem(AElem), Index(AIndex) {}
  void operator()(TUndoList &UndoList)
  {
    UndoList.Push(TUndoMove(Data, Elem, Data.GetIndex(Elem)));
    Data.Delete(Elem);
    Data.Insert(Elem, Index);
  }
};

template<class T>
TUndoObject<T> MakeUndoObject(T &OrgObject)
{
  return TUndoObject<T>(OrgObject, OrgObject);
}

extern TUndoList UndoList;
//---------------------------------------------------------------------------
} //namespace Graph
#endif
