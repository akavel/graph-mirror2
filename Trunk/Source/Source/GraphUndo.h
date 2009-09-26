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
  TGraphElemPtr OldElem;
  TGraphElemPtr NewElem;

  TUndoChange(TData &AData, const TGraphElemPtr &AOldElem, const TGraphElemPtr &ANewElem)
    : Data(AData), OldElem(AOldElem), NewElem(ANewElem)
  {
    OldElem->ClearCache();
  }

  void operator()(TUndoList &UndoList) const
  {
    UndoList.Push(TUndoChange(Data, NewElem, OldElem));
    Data.Replace(OldElem, NewElem);
  }
};

struct TUndoDel
{
  TData &Data;
  int Index;
  TGraphElemPtr Elem;
  TGraphElemPtr Parent;

  TUndoDel(TData &AData, const TGraphElemPtr &AElem, const TGraphElemPtr &AParent, int AIndex)
    : Data(AData), Index(AIndex), Elem(AElem), Parent(AParent)  { }
  void operator()(TUndoList &UndoList) const
  {
    UndoList.Push(TUndoAdd(Data, Elem));
    if(Parent)
      Parent->InsertChild(Elem, Index);
    else
      Data.Insert(Elem, Index);
  }
};

inline void TUndoAdd::operator()(TUndoList &UndoList) const
{
  UndoList.Push(TUndoDel(Data, GraphElem, GraphElem->GetParent(), Data.GetIndex(GraphElem)));
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
