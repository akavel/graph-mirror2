/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
// GRAPHSERVERIMPL.H : Declaration of the TGraphServerImpl

#ifndef GraphServerImplH
#define GraphServerImplH

#define ATL_APARTMENT_THREADED

#include "Graph_TLB.H"
#include <boost/shared_ptr.hpp>

extern class TGraphServerImpl *GraphServerImpl;

/////////////////////////////////////////////////////////////////////////////
// TGraphServerImpl     Implements IGraphServer, default interface of GraphServer
// ThreadingModel : Apartment
// Dual Interface : TRUE
// Event Support  : FALSE
// Default ProgID : Graph.GraphServer
// Description    :
/////////////////////////////////////////////////////////////////////////////
class ATL_NO_VTABLE TGraphServerImpl :
  public CComObjectRootEx<CComSingleThreadModel>,
  public CComCoClass<TGraphServerImpl, &CLSID_GraphServer>,
  public IDispatchImpl<IGraphServer, &IID_IGraphServer, &LIBID_Graph>
{
public:
  TGraphServerImpl();
  ~TGraphServerImpl();

  // Data used when registering Object
  //
  DECLARE_THREADING_MODEL(otApartment);
  DECLARE_PROGID("Graph.GraphServer");
  DECLARE_DESCRIPTION("");

  // Function invoked to (un)register object
  //
  static HRESULT WINAPI UpdateRegistry(BOOL bRegister)
  {
    TTypedComServerRegistrarT<TGraphServerImpl>
    regObj(GetObjectCLSID(), GetProgID(), GetDescription());
    return regObj.UpdateRegistry(bRegister);
  }


BEGIN_COM_MAP(TGraphServerImpl)
  COM_INTERFACE_ENTRY(IGraphServer)
  COM_INTERFACE_ENTRY2(IDispatch, IGraphServer)
END_COM_MAP()

// IGraphServer
public:

  STDMETHOD(Show());
  STDMETHOD(Hide());
  STDMETHOD(Load(BSTR FileName));
  STDMETHOD(ImportPointSeries(BSTR FileName));
  STDMETHOD(Save(BSTR FileName, VARIANT_BOOL Remember));
  STDMETHOD(SaveAsImage(BSTR FileName, TImageFileType ImageFileType));
  STDMETHOD(ZoomAllPoints());
  STDMETHOD(Refresh());
  STDMETHOD(get_Axes(TAxesRecord* Value));
  STDMETHOD(set_Axes(TAxesRecord Value));
  STDMETHOD(AddFunc(TFuncRecord Func));
};

#endif //GraphServerImplH
