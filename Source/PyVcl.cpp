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
//---------------------------------------------------------------------------
static PyObject *PyPropertyException = NULL;

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
  Py_INCREF(None);
  return None;
}
//---------------------------------------------------------------------------
PyObject* ThrowPythonException(const Exception &E)
{
  static PyObject *VclException = PyErr_NewException("PyVcl.VclException", NULL, NULL);
  PyErr_SetString(VclException, E.Message.c_str());
  return NULL;
}
//---------------------------------------------------------------------------
PyObject* ThrowPropertyError(const AnsiString &Str)
{
  PyErr_SetString(PyPropertyException, Str.c_str());
  return NULL;
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
    return ThrowPythonException(E);
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
    return PyNone();
  }
  catch(Exception &E)
  {
    return ThrowPythonException(E);
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
      return PyNone();
    }

    TPropInfo *PropInfo = GetPropInfo(Control, Name);
    if(PropInfo == NULL)
      return ThrowPropertyError(AnsiString("Property ") + Name + " does not exist in class " + Control->ClassName());

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
    return PyNone();
  }
  catch(Exception &E)
  {
    return ThrowPythonException(E);
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
      return ThrowPropertyError(AnsiString("Property ") + Name + " does not exist in class " + Control->ClassName());

    switch((*PropInfo->PropType)->Kind)
    {
      case tkInteger:
      case tkEnumeration:
      case tkChar:
      case tkClass:
        return PyInt_FromLong(GetOrdProp(Control, PropInfo));

      case tkFloat:
        return PyFloat_FromDouble(GetFloatProp(Control, PropInfo));

      case tkLString:  
      case tkString:
        return PyString_FromString(GetStrProp(Control, PropInfo).c_str());

      case tkWString:
      {
        WideString Str = GetWideStrProp(Control, PropInfo);
        return PyUnicode_FromWideChar(Str ? Str.c_bstr() : L"", Str.Length());
      }

      case tkSet:
        return PyString_FromString(GetSetProp(Control, PropInfo).c_str());

      case tkMethod:
      {
        void *Data = GetMethodProp(Control, PropInfo).Data;
        TPythonCallback *PythonCallback = dynamic_cast<TPythonCallback*>(static_cast<TObject*>(Data));
        PyObject *Result = PythonCallback ? PythonCallback->Callback : PyNone();
        Py_INCREF(Result);
        return Result;
      }
      
      default:
        return PyNone();
    }
  }
  catch(Exception &E)
  {
    return ThrowPythonException(E);
  }
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
    return PyNone();  
  }
  catch(Exception &E)
  {
    return ThrowPythonException(E);
  }
}
//---------------------------------------------------------------------------
static PyMethodDef PyVclMethods[] = {
  {"CreateObject", CreateObject, METH_VARARGS, ""},
  {"DeleteObject", DeleteObject, METH_VARARGS, ""},
  {"SetProperty", SetProperty, METH_VARARGS, ""},
  {"GetProperty", GetProperty, METH_VARARGS, ""},
  {"CallMethod", CallMethod, METH_VARARGS, ""},
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
  PyModule_AddObject(PyVclModule, "PropertyError", PyPropertyException);
}
//---------------------------------------------------------------------------

