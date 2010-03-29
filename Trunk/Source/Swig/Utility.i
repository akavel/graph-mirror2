%module Utility

%include "Types.i"

%begin %{
#include "Graph.h"
#undef _DEBUG
#include <Python.h>
#define WRAP_PYOBJECTS
#include "PythonBind.h"
#include "PyGraph.h"
#include "PythonBind.h"
#pragma warn -8060
%}

