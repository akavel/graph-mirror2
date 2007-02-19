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

struct TUndoAdd
{
  boost::shared_ptr<TGraphElem> GraphElem;

  TUndoAdd(const boost::shared_ptr<TGraphElem> &AGraphElem) : GraphElem(AGraphElem) {}
  void operator()(TUndoList &UndoList) const;
};

struct TUndoChange
{
  boost::shared_ptr<TGraphElem> Elem;
  unsigned Index;

  TUndoChange(const boost::shared_ptr<TGraphElem> &AElem, unsigned AIndex)
    : Elem(AElem), Index(AIndex) {Elem->ClearCache();}
  void operator()(TUndoList &UndoList) const
  {
    UndoList.Push(TUndoChange(Form1->Data.Replace(Index, Elem), Index));
  }
};

//Special class for use with TBaseFuncBase changes; Use this instead of TUndoChange
struct TUndoChangeFunc
{
  boost::shared_ptr<TBaseFuncType> OldFunc;
  boost::shared_ptr<TBaseFuncType> NewFunc;

  TUndoChangeFunc(const boost::shared_ptr<TBaseFuncType> &AOldFunc, const boost::shared_ptr<TBaseFuncType> &ANewFunc)
    : OldFunc(AOldFunc), NewFunc(ANewFunc)
  {
    OldFunc->ClearCache();
  }

  void operator()(TUndoList &UndoList) const
  {
    UndoList.Push(TUndoChangeFunc(NewFunc, OldFunc));
    Form1->Data.Replace(Form1->Data.GetIndex(NewFunc), OldFunc);
  }
};

struct TUndoDel
{
  int Index;
  boost::shared_ptr<TGraphElem> GraphElem;

  TUndoDel(const boost::shared_ptr<TGraphElem> &AGraphElem, int AIndex)
    : Index(AIndex), GraphElem(AGraphElem)  { }
  void operator()(TUndoList &UndoList) const
  {
    UndoList.Push(TUndoAdd(GraphElem));
    Form1->Data.Insert(GraphElem, Index);
  }
};

inline void TUndoAdd::operator()(TUndoList &UndoList) const
{
  UndoList.Push(TUndoDel(GraphElem, Form1->Data.GetIndex(GraphElem)));
  Form1->Data.Delete(GraphElem);
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
  TUndoAxes() : Axes(Form1->Data.Axes) {}
  void operator()(TUndoList &UndoList)
  {
    UndoList.Push(TUndoAxes());
    Form1->Data.Axes = Axes;
    Form1->Data.ClearCache();
    Form1->Data.Update(); //In case trigonmetry has changed
  }
};

struct TUndoCustomFunctions
{
  TCustomFunctions CustomFunctions;
  TUndoCustomFunctions(const TCustomFunctions &ACustomFunctions) : CustomFunctions(ACustomFunctions) {}
  void operator()(TUndoList &UndoList)
  {
    Form1->Data.CustomFunctions.Swap(CustomFunctions);
    UndoList.Push(*this);
    Form1->Data.Update();
    Form1->Data.ClearCache();
  }
};

struct TUndoMove
{
  TGraphElemPtr Elem;
  unsigned Index;

  TUndoMove(const TGraphElemPtr &AElem, unsigned AIndex) : Elem(AElem), Index(AIndex) {}
  void operator()(TUndoList &UndoList)
  {
    UndoList.Push(TUndoMove(Elem, Form1->Data.GetIndex(Elem)));
    Form1->Data.Delete(Elem);
    Form1->Data.Insert(Elem, Index);
  }
};

template<class T>
TUndoObject<T> MakeUndoObject(T &OrgObject)
{
  return TUndoObject<T>(OrgObject, OrgObject);
}

extern TUndoList UndoList;
//---------------------------------------------------------------------------
#endif
