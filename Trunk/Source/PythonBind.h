/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef PythonBindH
#define PythonBindH
//---------------------------------------------------------------------------
namespace PythonBind
{
  enum TPluginEvent
  {
    peNew,
    peLoad,
    peSelect
  };

  void InitPlugins();
  bool ExecutePythonCommand(const AnsiString &Command);
  void ShowPythonConsole();
  void ExecutePluginEvent(TPluginEvent PluginEvent);
}
#endif
