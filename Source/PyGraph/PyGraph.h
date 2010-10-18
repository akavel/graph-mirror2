/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef PyGraphH
#define PyGraphH
#include "boost/variant/variant.hpp"
//---------------------------------------------------------------------------
struct _object;
typedef _object PyObject;
namespace Python
{
  enum TPluginEvent
  {
    peNew,
    peLoad,
    peSelect,
    peClose,
    peEdit,
    peAnimate,
    peDelete,
    peAxesChanged,
    peZoom,
    peOptionsChanged,
    peCustomFunctionsChanged,
    peNewElem,
    peChanged,
    peMoved
  };

  extern PyObject *PyEFuncError;
  extern PyObject *PyEGraphError;
  typedef boost::variant<int, double, std::wstring, PyObject*> TVariant;

  void InitPlugins();
  bool ExecutePythonCommand(const String &Command);
  void ShowPythonConsole(bool Visible);
  bool ExecutePluginEvent(TPluginEvent PluginEvent, PyObject *Param=NULL);
  bool ExecutePluginEvent(TPluginEvent PluginEvent, const TGraphElemPtr &Elem);
  bool ExecutePluginEvent(TPluginEvent PluginEvent, TVariant V1);
  bool ExecutePluginEvent(TPluginEvent PluginEvent, TVariant V1, TVariant V2);
  bool ExecutePluginEvent(TPluginEvent PluginEvent, TVariant V1, TVariant V2, TVariant V3);

  PyObject* ToPyObject(int Value);
  PyObject* ToPyObject(unsigned Value) {return ToPyObject(static_cast<int>(Value));}
  PyObject* ToPyObject(double Value);
  PyObject* ToPyObject(long double Value) {return ToPyObject(static_cast<double>(Value));}
  PyObject* ToPyObject(const std::wstring &Str);
  PyObject* ToPyObject(const Func32::TComplex &Value);
  PyObject* ToPyObject(const TVariant &Variant);

  template<typename T> T FromPyObject(PyObject *O);
  template<> int FromPyObject<int>(PyObject *O);
  template<> double FromPyObject<double>(PyObject *O);
  template<> unsigned FromPyObject<unsigned>(PyObject *O) {return FromPyObject<int>(O);}
  template<> TColor FromPyObject<TColor>(PyObject *O) {return static_cast<TColor>(FromPyObject<int>(O));}
  template<> long double FromPyObject<long double>(PyObject *O) {return FromPyObject<double>(O);}
  template<> Func32::TComplex FromPyObject<Func32::TComplex>(PyObject *O);
  template<> std::wstring FromPyObject<std::wstring>(PyObject *O);
  template<typename T> bool FromPyObject(PyObject *O, T &Value) {Value = FromPyObject<T>(O); return !PyErr_Occurred();}

  template<typename T1, typename T2>
  PyObject* CreateTuple(const T1 &V1, const T2 &V2)
  {
    return Py_BuildValue("NN", ToPyObject(V1), ToPyObject(V2));
  }

  template<typename T1, typename T2>
  bool FromTuple(PyObject *O, T1 &V1, T2 &V2)
  {
    PyObject *O1=NULL, *O2=NULL;
    if(!PyArg_ParseTuple(O, "OO", &O1, &O2)) return false;
    return FromPyObject(O1, V1) && FromPyObject(O2, V2);
  }

  template<typename T1, typename T2, typename T3>
  PyObject* CreateTuple(const T1 &V1, const T2 &V2, const T3 &V3)
  {
    return Py_BuildValue("NNN", ToPyObject(V1), ToPyObject(V2), ToPyObject(V3));
  }

  template<typename T1, typename T2, typename T3>
  bool FromTuple(PyObject *O, T1 &V1, T2 &V2, T3 &V3)
  {
    PyObject *O1=NULL, *O2=NULL, *O3=NULL;
    if(!PyArg_ParseTuple(O, "OOO", &O1, &O2, &O3)) return false;
    return FromPyObject(O1, V1) && FromPyObject(O2, V2) && FromPyObject(O3, V3);
  }

  void ConvertException();
}
#endif
