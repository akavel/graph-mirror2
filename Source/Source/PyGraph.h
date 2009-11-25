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
    peAnimate
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
  PyObject* ToPyObject(double Value);
  PyObject* ToPyObject(const std::wstring &Str);
  PyObject* ToPyObject(const TVariant &Variant);
}
#endif
