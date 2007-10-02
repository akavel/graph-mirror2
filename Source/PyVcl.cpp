/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Graph.h"
#pragma hdrstop
#include "PyVcl.h"
#undef _DEBUG
#include <python.h>

//Workaround for name mangling bug in GetPropList()
#pragma alias "@Typinfo@GetPropList$qqrp14System@TObjectrpp17Typinfo@TPropInfo"\
="@Typinfo@GetPropList$qqrp14System@TObjectrpa16380$p17Typinfo@TPropInfo"
//---------------------------------------------------------------------------
static PyObject *PyPropertyException = NULL;
static PyObject *PyVclException = NULL;

struct TPythonCallback : public TObject
{
  PyObject *Callback;
  TPythonCallback(PyObject *ACallback) : Callback(ACallback) {}
    
  void __fastcall Method(TObject *Sender)
  {
    PyObject *Result = PyObject_CallObject(Callback, NULL);
    if(Result == NULL)
      PyErr_Print();
    Py_XDECREF(Result);
  }
};
//---------------------------------------------------------------------------
PyObject* PyNone()
{
  static PyObject *None = Py_BuildValue("");
  return None;
}
//---------------------------------------------------------------------------
PyObject* PyReturnNone()
{
  Py_INCREF(PyNone());
  return PyNone();
}
//---------------------------------------------------------------------------
static PyObject* CreateObject(PyObject *Self, PyObject *Args)
{
  try
  {
    const char *Name;
    if(!PyArg_ParseTuple(Args, "s", &Name))
      return NULL;

    TMetaClass *MetaClass = FindClass(Name);
    TComponent *Component;
    AnsiString ClassName = Name;
    Application->CreateForm(MetaClass, &Component);

    return Py_BuildValue("i", Component); //Return pointer to new object, NULL on failure
  }
  catch(Exception &E)
  {
    PyErr_SetString(PyVclException, E.Message.c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* DeleteObject(PyObject *Self, PyObject *Args)
{
  try
  {
    TObject *Object;
    if(!PyArg_ParseTuple(Args, "i", &Object))
      return NULL;

    delete Object;
    return PyReturnNone();
  }
  catch(Exception &E)
  {
    PyErr_SetString(PyVclException, E.Message.c_str());
    return NULL;
  }

}
//---------------------------------------------------------------------------
static PyObject* SetProperty(PyObject *Self, PyObject *Args)
{
  try
  {
    TControl *Control;
    const char *Name;
    PyObject *Value = NULL;
    if(!PyArg_ParseTuple(Args, "isO", &Control, &Name, &Value))
      return NULL;

    AnsiString ClassName = Name;
    if(ClassName == "Parent")
    {
      Control->Parent = reinterpret_cast<TWinControl*>(PyInt_AsLong(Value));
      return PyReturnNone();
    }

    TPropInfo *PropInfo = GetPropInfo(Control, Name);
    if(PropInfo == NULL)
    {
      PyErr_SetString(PyPropertyException, ("Property " + ClassName + " does not exist in class " + Control->ClassName()).c_str());
      return NULL;
    }

    switch((*PropInfo->PropType)->Kind)
    {
      case tkInteger:
      case tkEnumeration:
      case tkChar:
      case tkClass:
      {
        int Long = PyInt_AsLong(Value);
        if(!PyErr_Occurred())
          SetOrdProp(Control, PropInfo, Long);
        break;
      }

      case tkFloat:
      {
        double Double = PyFloat_AsDouble(Value);
        if(!PyErr_Occurred())
          SetFloatProp(Control, PropInfo, Double);
        break;
      }

      case tkLString:
      case tkString:
      {
        const char *Str = PyString_AsString(Value);
        if(Str == NULL)
          return NULL;
        SetStrProp(Control, PropInfo, Str);
        break;
      }

      case tkWString:
      {
        PyObject *Object = PyUnicode_FromObject(Value);
        if(Object == NULL)
          return NULL;

        const wchar_t *Str = reinterpret_cast<wchar_t*>(PyUnicode_AsUnicode(Object));
        if(Str == NULL)
          return NULL;
        SetWideStrProp(Control, PropInfo, Str);
        Py_DECREF(Object);
        break;
      }

      case tkSet:
      {
        const char *Str = PyString_AsString(Value);
        if(Str)
          SetSetProp(Control, PropInfo, Str);
        break;
      }

      case tkMethod:
      {
        Py_INCREF(Value);
        TPythonCallback *PythonCallback = new TPythonCallback(Value);
        TNotifyEvent Event = &PythonCallback->Method;
        SetMethodProp(Control, PropInfo, reinterpret_cast<TMethod&>(Event));
        break;
      }
    }
    return PyReturnNone();
  }
  catch(Exception &E)
  {
    PyErr_SetString(PyVclException, E.Message.c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* GetProperty(PyObject *Self, PyObject *Args)
{
  try
  {
    TControl *Control;
    const char *Name;
    if(!PyArg_ParseTuple(Args, "is", &Control, &Name))
      return NULL;

    AnsiString ClassName = Name;
    if(ClassName == "Parent")
      return PyInt_FromLong(reinterpret_cast<long>(Control->Parent));

    TPropInfo *PropInfo = GetPropInfo(Control, Name);
    if(PropInfo == NULL)
    {
      PyErr_SetString(PyPropertyException, (AnsiString("Property ") + Name + " does not exist in class " + Control->ClassName()).c_str());
      return NULL;
    }

    int Kind = (*PropInfo->PropType)->Kind;
    switch(Kind)
    {
      case tkInteger:
      case tkEnumeration:
      case tkChar:
      case tkClass:
        return Py_BuildValue("ii", GetOrdProp(Control, PropInfo), Kind);

      case tkFloat:
        return Py_BuildValue("di", GetFloatProp(Control, PropInfo), Kind);

      case tkLString:
      case tkString:
        return Py_BuildValue("si", GetStrProp(Control, PropInfo).c_str(), Kind);

      case tkWString:
      {
        WideString Str = GetWideStrProp(Control, PropInfo);
        return Py_BuildValue("ui", Str ? Str.c_bstr() : L"", Kind);
      }

      case tkSet:
        return Py_BuildValue("si", GetSetProp(Control, PropInfo).c_str(), Kind);

      case tkMethod:
      {
        void *Data = GetMethodProp(Control, PropInfo).Data;
        TPythonCallback *PythonCallback = dynamic_cast<TPythonCallback*>(static_cast<TObject*>(Data));
        PyObject *Result = PythonCallback ? PythonCallback->Callback : PyNone();
        return Py_BuildValue("Oi", Result, Kind);
      }

      default:
        return PyReturnNone();
    }
  }
  catch(Exception &E)
  {
    PyErr_SetString(PyVclException, E.Message.c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* GetPropertyList(PyObject *Self, PyObject *Args)
{
  TObject *Object = reinterpret_cast<TObject*>(PyInt_AsLong(Args));
  if(PyErr_Occurred())
    return NULL;

  PPropList PropList;
  int Count = GetPropList(Object, PropList);
  PyObject *List = PyList_New(Count);
  for(int I = 0; I < Count; I++)
    PyList_SetItem(List, I, PyString_FromString(AnsiString(PropList[I]->Name).c_str()));
  return List;
}
//---------------------------------------------------------------------------
static PyObject* CallMethod(PyObject *Self, PyObject *Args)
{
  try
  {
    TObject *Object;
    const char *Name;
    if(!PyArg_ParseTuple(Args, "is", &Object, &Name))
      return NULL;

    if(TForm *Form = dynamic_cast<TForm*>(Object))
    {
      AnsiString Method = Name;
      if(Method == "ShowModal")
        return PyInt_FromLong(Form->ShowModal());
      else if(Method == "Close")
        Form->Close();
    }
    return PyReturnNone();
  }
  catch(Exception &E)
  {
    PyErr_SetString(PyVclException, E.Message.c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyMethodDef PyVclMethods[] = {
  {"CreateObject", CreateObject, METH_VARARGS, ""},
  {"DeleteObject", DeleteObject, METH_VARARGS, ""},
  {"SetProperty", SetProperty, METH_VARARGS, ""},
  {"GetProperty", GetProperty, METH_VARARGS, ""},
  {"CallMethod", CallMethod, METH_VARARGS, ""},
  {"GetPropertyList", GetPropertyList, METH_O, ""},
  {NULL, NULL, 0, NULL}
};
//---------------------------------------------------------------------------
void InitPyVcl()
{
  RegisterClass(__classid(TForm));
  RegisterClass(__classid(TEdit));
  RegisterClass(__classid(TLabel));
  RegisterClass(__classid(TButton));
  PyObject *PyVclModule = Py_InitModule("PyVcl", PyVclMethods);

  PyPropertyException = PyErr_NewException("PyVcl.PropertyError", NULL, NULL);
  Py_INCREF(PyPropertyException);
  PyModule_AddObject(PyVclModule, "PropertyError", PyPropertyException);

  PyVclException = PyErr_NewException("PyVcl.VclException", NULL, NULL);
  Py_INCREF(PyVclException);
  PyModule_AddObject(PyVclModule, "VclError", PyVclException);
}
//---------------------------------------------------------------------------












