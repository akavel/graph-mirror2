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
#include <ActnMan.hpp>
#undef _DEBUG
#include <python.h>
#include "PythonBind.h"

#define NAME_VALUE_ENTRY(x) {_TEXT(#x), (TFastcallFunction)x}

namespace Python
{
//---------------------------------------------------------------------------
static PyObject *PyPropertyException = NULL;
static PyObject *PyVclException = NULL;
//---------------------------------------------------------------------------
template<class T> T VclCast(TObject *Object)
{
  if(T O = dynamic_cast<T>(Object))
    return O;
  throw EConvertError(AnsiString().sprintf("Cannot convert object of type '%s' to '%s'",
    AnsiString(Object->ClassName()).c_str(), typeid(T).name()));  
}
//---------------------------------------------------------------------------
struct TPythonCallback : public TObject
{
  PyObject *Callback;
  PTypeData TypeData;
  PyObject *PySender;
  TPythonCallback(PyObject *ACallback, PyObject *APySender, PTypeData ATypeData)
    : Callback(ACallback), PySender(APySender), TypeData(ATypeData)
  {
  }

  void __fastcall Method(TObject *Sender)
  {
    int Arg3 = _ECX;
    AllocGIL();
    PyObject *Args = PyTuple_New(TypeData->ParamCount);
    PyTuple_SetItem(Args, 0, PySender);
    char *Ptr = TypeData->ParamList;
    Ptr++;
    Ptr += *Ptr + 1;
    Ptr += *Ptr + 1;

    for(int I = 1; I < TypeData->ParamCount; I++)
    {
      TParamFlags &Flags = reinterpret_cast<TParamFlags&>(*(Ptr++));
      const ShortString &ParamName = reinterpret_cast<const ShortString&>(*Ptr);
      Ptr += *Ptr + 1;
      const ShortString &TypeName = reinterpret_cast<const ShortString&>(*Ptr);
      Ptr += *Ptr + 1;
      AnsiString Type = TypeName;
      if(Type == "Word")
        PyTuple_SetItem(Args, I, PyLong_FromLong(Arg3));
      else if(Type == "Char")
        PyTuple_SetItem(Args, I, PyUnicode_FromFormat("%c", *(char*)Arg3));
    }
    PyObject *Result = PyObject_CallObject(Callback, Args);
    if(Result == NULL)
      PyErr_Print();
    Py_XDECREF(Result);
    FreeGIL();
  }
};
//---------------------------------------------------------------------------
static PyObject* VclCreateObject(PyObject *Self, PyObject *Args)
{
  try
  {
    const wchar_t *Name;
    if(!PyArg_ParseTuple(Args, "u", &Name))
      return NULL;

    TMetaClass *MetaClass = FindClass(Name);
    TComponent *Component;
    Application->CreateForm(MetaClass, &Component);

    return Py_BuildValue("i", Component); //Return pointer to new object, NULL on failure
  }
  catch(Exception &E)
  {
    PyErr_SetString(PyVclException, AnsiString(E.Message).c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* VclDeleteObject(PyObject *Self, PyObject *Args)
{
  try
  {
    TObject *Object;
    if(!PyArg_ParseTuple(Args, "i", &Object))
      return NULL;

    delete Object;
    Py_RETURN_NONE;
  }
  catch(Exception &E)
  {
    PyErr_SetString(PyVclException, AnsiString(E.Message).c_str());
    return NULL;
  }

}
//---------------------------------------------------------------------------
static PyObject* VclSetProperty(PyObject *Self, PyObject *Args)
{
  try
  {
    TControl *Control;
    const wchar_t *Name;
    PyObject *Value = NULL;
    PyObject *Sender = NULL;
    if(!PyArg_ParseTuple(Args, "iuO|O", &Control, &Name, &Value, &Sender))
      return NULL;

    AnsiString ClassName = Name;
    if(ClassName == "Parent")
    {
      long LongValue = Value == Py_None ? 0 : PyLong_AsLong(Value);
      Control->Parent = reinterpret_cast<TWinControl*>(LongValue);
      Py_RETURN_NONE;
    }

    TPropInfo *PropInfo = GetPropInfo(Control, Name);
    if(PropInfo == NULL)
    {
      PyErr_SetString(PyPropertyException, ("Property " + ClassName + " does not exist in class " + Control->ClassName()).c_str());
      return NULL;
    }

    switch((*PropInfo->PropType)->Kind)
    {
      case tkClass:
        if(AnsiString((*PropInfo->PropType)->Name) == "TStrings")
        {
          ((TStrings*)GetOrdProp(Control, PropInfo))->Text = PyUnicode_AsUnicode(Value);
          break;
        }
        //Fall through
      case tkInteger:
      case tkChar:
      {
        int Long = PyLong_AsLong(Value);
        if(!PyErr_Occurred())
          SetOrdProp(Control, PropInfo, Long);
        break;
      }

      case tkEnumeration:
      {
        int Long = PyLong_AsLong(Value);
        if(!PyErr_Occurred())
          SetOrdProp(Control, PropInfo, Long);
        else
        {
          PyErr_Clear();
          if(const wchar_t *Str = PyUnicode_AsUnicode(Value))
            SetEnumProp(Control, PropInfo, Str);
        }
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
      case tkWString:
      case tkUString:
      {
        const wchar_t *Str = PyUnicode_AsUnicode(Value);
        if(Str == NULL)
          return NULL;
        SetStrProp(Control, PropInfo, Str);
        break;
      }

      case tkSet:
      {
        const wchar_t *Str = PyUnicode_AsUnicode(Value);
        if(Str)
          SetSetProp(Control, PropInfo, Str);
        break;
      }

      case tkMethod:
      {
        PTypeData TypeData = GetTypeData(*PropInfo->PropType);
        Py_INCREF(Value);
        Py_INCREF(Sender);
        TPythonCallback *PythonCallback = new TPythonCallback(Value, Sender, TypeData);
        TNotifyEvent Event = &PythonCallback->Method;
        SetMethodProp(Control, PropInfo, reinterpret_cast<TMethod&>(Event));
        break;
      }

      default:
        PyErr_SetString(PyPropertyException, AnsiString("Unknown type of property " + ClassName + " in class " + Control->ClassName()).c_str());
        return NULL;
    }
    return PyErr_Occurred() ? NULL : (Py_INCREF(Py_None), Py_None);
  }
  catch(Exception &E)
  {
    PyErr_SetString(PyVclException, AnsiString(E.Message).c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* VclGetProperty(PyObject *Self, PyObject *Args)
{
  try
  {
    TControl *Control;
    const wchar_t *Name;
    int Index = 0;
    if(!PyArg_ParseTuple(Args, "iu|i", &Control, &Name, &Index))
      return NULL;

    AnsiString ClassName = Name;
    if(ClassName == "Parent")
      return Py_BuildValue("ii", Control->Parent, tkClass);
    else if(ClassName == "ComponentCount")
      return Py_BuildValue("ii", Control->ComponentCount, tkInteger);
    else if(ClassName == "Components")
      return Py_BuildValue("ii", Control->Components[Index], tkClass);
    else if(ClassName == "ClassName")
      return Py_BuildValue("si", AnsiString(Control->ClassName()).c_str(), tkString);
    else if(ClassName == "ActionCount")
      return Py_BuildValue("ii", VclCast<TActionManager*>(Control)->ActionCount, tkInteger);
    else if(ClassName == "Actions")
      return Py_BuildValue("ii", VclCast<TActionManager*>(Control)->Actions[Index], tkClass);

    TPropInfo *PropInfo = GetPropInfo(Control, Name);
    if(PropInfo == NULL)
    {
      PyErr_SetString(PyPropertyException, ("Property " + ClassName + " does not exist in class " + Control->ClassName()).c_str());
      return NULL;
    }

    int Kind = (*PropInfo->PropType)->Kind;
    switch(Kind)
    {
      case tkInteger:
      case tkChar:
        return Py_BuildValue("ii", GetOrdProp(Control, PropInfo), Kind);

      case tkClass:
        if(AnsiString((*PropInfo->PropType)->Name) == "TStrings")
          return Py_BuildValue("ui", ((TStrings*)GetOrdProp(Control, PropInfo))->Text.c_str(), -1);
        return Py_BuildValue("ii", GetOrdProp(Control, PropInfo), Kind);

      case tkEnumeration:
        if(AnsiString((*PropInfo->PropType)->Name) == "Boolean")
          return Py_BuildValue("Ni", PyBool_FromLong(GetOrdProp(Control, PropInfo)), Kind);
        return Py_BuildValue("ui", GetEnumProp(Control, PropInfo).c_str(), Kind);

      case tkFloat:
        return Py_BuildValue("di", GetFloatProp(Control, PropInfo), Kind);

      case tkLString:
      case tkString:
      case tkWString:
      case tkUString:
        return Py_BuildValue("ui", GetStrProp(Control, PropInfo).c_str(), Kind);

      case tkSet:
        return Py_BuildValue("ui", GetSetProp(Control, PropInfo).c_str(), Kind);

      case tkMethod:
      {
        void *Data = GetMethodProp(Control, PropInfo).Data;
        TPythonCallback *PythonCallback = dynamic_cast<TPythonCallback*>(static_cast<TObject*>(Data));
        PyObject *Result = PythonCallback ? PythonCallback->Callback : Py_None;
        return Py_BuildValue("Oi", Result, Kind);
      }

      default:
        PyErr_SetString(PyPropertyException, AnsiString("Unknown type of property " + ClassName + " in class " + Control->ClassName()).c_str());
        return NULL;
    }
  }
  catch(Exception &E)
  {
    PyErr_SetString(PyVclException, AnsiString(E.Message).c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* VclGetPropertyList(PyObject *Self, PyObject *Args)
{
  TObject *Object = reinterpret_cast<TObject*>(PyLong_AsLong(Args));
  if(PyErr_Occurred())
    return NULL;

  PPropList PropList;
  int Count = GetPropList(Object, PropList);
  PyObject *List = PyList_New(Count);
  for(int I = 0; I < Count; I++)
    PyList_SetItem(List, I, PyUnicode_FromString(AnsiString((*PropList)[I]->Name).c_str()));
  return List;
}
//---------------------------------------------------------------------------
static PyObject* VclCallMethod(PyObject *Self, PyObject *Args)
{
  try
  {
    TObject *Object;
    const wchar_t *Name;
    if(!PyArg_ParseTuple(Args, "iu", &Object, &Name))
      return NULL;

    if(TForm *Form = dynamic_cast<TForm*>(Object))
    {
      AnsiString Method = Name;
      if(Method == "ShowModal")
      {
        FreeGIL();
        TModalResult Result = Form->ShowModal();
        AllocGIL();
        return PyLong_FromLong(Result);
      }
      else if(Method == "Close")
        Form->Close();
    }
    else if(TAction *Action = dynamic_cast<TAction*>(Object))
      Action->Execute();
    Py_RETURN_NONE;
  }
  catch(Exception &E)
  {
    PyErr_SetString(PyVclException, AnsiString(E.Message).c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
typedef int __fastcall (*TFastcallFunction)(int,int);
struct TFunctionEntry
{
  const wchar_t *Name;
  TFastcallFunction Function;
};

TFunctionEntry FunctionList[] =
{
  NAME_VALUE_ENTRY(ShortCutToText),
  NAME_VALUE_ENTRY(TextToShortCut),
};
//---------------------------------------------------------------------------
static PyObject* VclCallFunction(PyObject *Self, PyObject *Args)
{
  try
  {
    const wchar_t *Name;
    PyObject *Arg1;
    PyObject *ResultType;
    if(!PyArg_ParseTuple(Args, "uOO", &Name, &ResultType, &Arg1))
      return NULL;
    TFastcallFunction Function = NULL;
    String FunctionName = Name;
    for(unsigned I = 0; I < sizeof(FunctionList) / sizeof(FunctionList[0]); I++)
      if(FunctionName == FunctionList[I].Name)
      {
        Function = FunctionList[I].Function;
        break;
      }

    if(Function == NULL)
      Py_RETURN_NONE;

    int pArg1 = 0, pArg2 = 0, pResult = 0;
    String Str1, Str2;
    if(PyUnicode_Check(Arg1))
    {
      Str1 = PyUnicode_AsUnicode(Arg1);
      pArg1 = (int)Str1.data();
    }
    else if(PyLong_Check(Arg1))
      pArg1 = PyLong_AsLong(Arg1);
    else
      Py_RETURN_NONE;

    if(ResultType == (PyObject*)&PyUnicode_Type)
      pArg2 = (int)&Str2;
    else if(ResultType == (PyObject*)&PyLong_Type)
      ;//pResult = (int)&Int;
    else
      Py_RETURN_NONE;

//    Int = TextToShortCut(Str1);
//    Str1 = ShortCutToText(Int);
    pResult = Function(pArg1, pArg2);

    if(ResultType == (PyObject*)&PyUnicode_Type)
      return PyUnicode_FromWideChar(Str2.c_str(), Str2.Length());
    else if(ResultType == (PyObject*)&PyLong_Type)
      return PyLong_FromLong(pResult);
    else
      Py_RETURN_NONE;
  }
  catch(Exception &E)
  {
    PyErr_SetString(PyVclException, AnsiString(E.Message).c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyMethodDef PyVclMethods[] = {
  {"CreateObject", VclCreateObject, METH_VARARGS, ""},
  {"DeleteObject", VclDeleteObject, METH_VARARGS, ""},
  {"SetProperty", VclSetProperty, METH_VARARGS, ""},
  {"GetProperty", VclGetProperty, METH_VARARGS, ""},
  {"CallMethod", VclCallMethod, METH_VARARGS, ""},
  {"GetPropertyList", VclGetPropertyList, METH_O, ""},
  {"CallFunction", VclCallFunction, METH_VARARGS, ""},
  {NULL, NULL, 0, NULL}
};
//---------------------------------------------------------------------------
static PyModuleDef PyVclModuleDef =
{
  PyModuleDef_HEAD_INIT,
  "PyVcl",
  NULL,
  -1,
  PyVclMethods,
  NULL,
  NULL,
  NULL,
  NULL,
};
//---------------------------------------------------------------------------
PyObject* InitPyVcl()
{
  RegisterClass(__classid(TForm));
  RegisterClass(__classid(TEdit));
  RegisterClass(__classid(TLabel));
  RegisterClass(__classid(TMemo));
  RegisterClass(__classid(TButton));
  RegisterClass(__classid(TAction));
  RegisterClass(__classid(TMenuItem));
  RegisterClass(__classid(TPanel));

  PyObject *PyVclModule = PyModule_Create(&PyVclModuleDef);

  PyPropertyException = PyErr_NewException("PyVcl.PropertyError", NULL, NULL);
  Py_INCREF(PyPropertyException);
  PyModule_AddObject(PyVclModule, "PropertyError", PyPropertyException);

  PyVclException = PyErr_NewException("PyVcl.VclException", NULL, NULL);
  Py_INCREF(PyVclException);
  PyModule_AddObject(PyVclModule, "VclError", PyVclException);
  return PyVclModule;
}
//---------------------------------------------------------------------------
}
















































