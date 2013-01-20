/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2013 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Graph.h"
#pragma hdrstop
#include "GraphUndo.h"
#include "PyGraph.h"
//---------------------------------------------------------------------------
namespace Graph
{
TUndoList UndoList(50);
//---------------------------------------------------------------------------
void TUndoChange::operator()(TUndoList &UndoList) const
{
  UndoList.Push(TUndoChange(NewElem, OldElem));
  TData::Replace(NewElem, OldElem);
  Python::ExecutePluginEvent(Python::peChanged, OldElem);
}
//---------------------------------------------------------------------------
void TUndoDel::operator()(TUndoList &UndoList) const
{
  UndoList.Push(TUndoAdd(Elem));
  Parent->InsertChild(Elem, Index);
  Python::ExecutePluginEvent(Python::peNewElem, Elem);
}
//---------------------------------------------------------------------------
void TUndoAdd::operator()(TUndoList &UndoList) const
{
  Python::ExecutePluginEvent(Python::peDelete, GraphElem);
  UndoList.Push(TUndoDel(GraphElem, GraphElem->GetParent(), TData::GetIndex(GraphElem)));
  TData::Delete(GraphElem);
}
//---------------------------------------------------------------------------
void TUndoAxes::operator()(TUndoList &UndoList)
{
  UndoList.Push(TUndoAxes(Data));
  Data.Axes = Axes;
  Data.ClearCache();
  Python::ExecutePluginEvent(Python::peAxesChanged);
  Data.Update(); //In case trigonmetry has changed
}
//---------------------------------------------------------------------------
void TUndoCustomFunctions::operator()(TUndoList &UndoList)
{
  Data.CustomFunctions.Swap(*CustomFunctions);
  UndoList.Push(*this);
  Python::ExecutePluginEvent(Python::peCustomFunctionsChanged);
  Data.Update();
  Data.ClearCache();
}
//---------------------------------------------------------------------------
void TUndoMove::operator()(TUndoList &UndoList)
{
  const TGraphElemPtr &Parent = Elem->GetParent();
  int OldIndex = Parent->GetChildIndex(Elem);
  UndoList.Push(TUndoMove(Data, Elem, OldIndex));
  Parent->RemoveChild(OldIndex);
  Parent->InsertChild(Elem, Index);
  Python::ExecutePluginEvent(Python::peMoved, Elem);
}
//---------------------------------------------------------------------------
void TUndoDelPoint::operator()(TUndoList &UndoList)
{
  Series->InsertPoint(Point);
  UndoList.Push(TUndoAddPoint(Series));
  Python::ExecutePluginEvent(Python::peChanged, TGraphElemPtr(Series));
}
//---------------------------------------------------------------------------
void TUndoAddPoint::operator()(TUndoList &UndoList)
{
	UndoList.Push(TUndoDelPoint(Series, Series->GetPoint(Series->PointCount()-1)));
	Series->DeletePoint(Series->PointCount()-1);
  Python::ExecutePluginEvent(Python::peChanged, TGraphElemPtr(Series));
}
//---------------------------------------------------------------------------
} //namespace Graph
