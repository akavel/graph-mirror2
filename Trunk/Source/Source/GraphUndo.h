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
class TUndoAdd
{
	TGraphElemPtr GraphElem;
public:
	TUndoAdd(const TGraphElemPtr &AGraphElem)
    : GraphElem(AGraphElem) {}
  void operator()(TUndoList &UndoList) const;
};

class TUndoChange
{
  TGraphElemPtr OldElem;
	TGraphElemPtr NewElem;
public:
	TUndoChange(const TGraphElemPtr &AOldElem, const TGraphElemPtr &ANewElem)
		: OldElem(AOldElem), NewElem(ANewElem)
	{
		OldElem->ClearCache();
	}

	void operator()(TUndoList &UndoList) const;
};

class TUndoDel
{
	int Index;
	TGraphElemPtr Elem;
	TGraphElemPtr Parent;
public:
	TUndoDel(const TGraphElemPtr &AElem, const TGraphElemPtr &AParent, int AIndex)
		: Index(AIndex), Elem(AElem), Parent(AParent)  { }
	void operator()(TUndoList &UndoList) const;
};

template<class T>
class TUndoObject
{
  T &OrgObject;
	T Object;
public:
  TUndoObject(T &AOrgObject, const T &AObject) : OrgObject(AOrgObject), Object(AObject) {}
  void operator()(TUndoList &UndoList)
  {
    UndoList.Push(TUndoObject<T>(OrgObject, OrgObject));
    OrgObject = Object;
  }
};

class TUndoAxes
{
  TAxes Axes;
  TData &Data;
public:
  TUndoAxes(TData &AData) : Data(AData), Axes(AData.Axes) {}
  void operator()(TUndoList &UndoList);
};

class TUndoCustomFunctions
{
  boost::shared_ptr<TCustomFunctions> CustomFunctions;
  TData &Data;
public:
  TUndoCustomFunctions(TData &AData) //This will steal the custom functions from AData
    : Data(AData), CustomFunctions(new TCustomFunctions)
  {
    CustomFunctions->Swap(AData.CustomFunctions);
  }
	void operator()(TUndoList &UndoList);
};

class TUndoMove
{
  TData &Data;
  TGraphElemPtr Elem;
  unsigned Index;
public:
  TUndoMove(TData &AData, const TGraphElemPtr &AElem, unsigned AIndex)
    : Data(AData), Elem(AElem), Index(AIndex) {}
	void operator()(TUndoList &UndoList);
};

class TUndoAddPoint
{
	TPointSeriesPtr Series;
public:
	TUndoAddPoint(TPointSeriesPtr ASeries) : Series(ASeries) {}
	void operator()(TUndoList &UndoList);
};

class TUndoDelPoint
{
	TPointSeriesPtr Series;
	TPointSeriesPoint Point;
public:
	TUndoDelPoint(TPointSeriesPtr ASeries, const TPointSeriesPoint &APoint)
		: Series(ASeries), Point(APoint) {}
	void operator()(TUndoList &UndoList);
};

template<class T>
TUndoObject<T> MakeUndoObject(T &OrgObject)
{
  return TUndoObject<T>(OrgObject, OrgObject);
}

class TUndoChangeVisible
{
  TGraphElemPtr Elem;
  int Visible;
public:
  TUndoChangeVisible(const TGraphElemPtr &AElem)
    : Elem(AElem), Visible(AElem->GetVisible()) {}
  void operator()(TUndoList &UndoList);
};

class TUndoChangeShowInLegend
{
  TGraphElemPtr Elem;
  bool ShowInLegend;
public:
  TUndoChangeShowInLegend(const TGraphElemPtr &AElem)
    : Elem(AElem), ShowInLegend(AElem->GetShowInLegend()) {}
  void operator()(TUndoList &UndoList);
};

extern TUndoList UndoList;
//---------------------------------------------------------------------------
} //namespace Graph
#endif
