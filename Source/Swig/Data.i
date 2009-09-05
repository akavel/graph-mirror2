%module Data
%import "std_wstring.i"

%begin %{
#include "Graph.h"
#include "Unit1.h"
#include "PythonBind.h"
#undef _DEBUG
#include "Python.h"
#pragma warn -8060
PyObject* SWIG_init2(PyObject*, PyObject*);
namespace Python
{
%}

%init %{
  SWIG_init2(d, m);
  return m;
}
} //namespace Python
using namespace Python;

static PyObject* SWIG_init2(PyObject *d, PyObject *m)
{
%}

