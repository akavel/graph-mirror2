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
//---------------------------------------------------------------------------
struct _object;
namespace Python
{
  enum TPluginEvent
  {
    peNew,
    peLoad,
    peSelect,
    peClose,
    peEdit
  };

  extern _object *PyEFuncError;
  extern _object *PyEGraphError;

  void InitPlugins();
  bool ExecutePythonCommand(const String &Command);
  void ShowPythonConsole(bool Visible);
  bool ExecutePluginEvent(TPluginEvent PluginEvent, _object *Param=NULL);
  bool ExecutePluginEvent(TPluginEvent PluginEvent, const TGraphElemPtr &Elem);
}
#endif
