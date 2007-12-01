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
#include "PythonBind.h"

//Workaround for name mangling bug in GetPropList()
#pragma alias "@Typinfo@GetPropList$qqrp14System@TObjectrpp17Typinfo@TPropInfo"\
="@Typinfo@GetPropList$qqrp14System@TObjectrpa16380$p17Typinfo@TPropInfo"

#define NAME_VALUE_ENTRY(x) {#x, (TFastcallFunction)x}

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
        PyTuple_SetItem(Args, I, PyInt_FromLong(Arg3));
      else if(Type == "Char")
        PyTuple_SetItem(Args, I, PyString_FromFormat("%c", *(char*)Arg3));
    }
    PyObject *Result = PyObject_CallObject(Callback, Args);
    if(Result == NULL)
      PyErr_Print();
    Py_XDECREF(Result);
  }
};
//---------------------------------------------------------------------------
static PyObject* VclCreateObject(PyObject *Self, PyObject *Args)
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
    PyErr_SetString(PyVclException, E.Message.c_str());
    return NULL;
  }

}
//---------------------------------------------------------------------------
static PyObject* VclSetProperty(PyObject *Self, PyObject *Args)
{
  try
  {
    TControl *Control;
    const char *Name;
    PyObject *Value = NULL;
    PyObject *Sender = NULL;
    if(!PyArg_ParseTuple(Args, "isO|O", &Control, &Name, &Value, &Sender))
      return NULL;

    AnsiString ClassName = Name;
    if(ClassName == "Parent")
    {
      Control->Parent = reinterpret_cast<TWinControl*>(PyInt_AsLong(Value));
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
      case tkInteger:
      case tkChar:
      case tkClass:
      {
        int Long = PyInt_AsLong(Value);
        if(!PyErr_Occurred())
          SetOrdProp(Control, PropInfo, Long);
        break;
      }

      case tkEnumeration:
      {
        int Long = PyInt_AsLong(Value);
        if(!PyErr_Occurred())
          SetOrdProp(Control, PropInfo, Long);
        else
        {
          PyErr_Clear();
          if(const char *Str = PyString_AsString(Value))
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
        PTypeData TypeData = GetTypeData(*PropInfo->PropType);
        Py_INCREF(Value);
        TPythonCallback *PythonCallback = new TPythonCallback(Value, Sender, TypeData);
        TNotifyEvent Event = &PythonCallback->Method;
        SetMethodProp(Control, PropInfo, reinterpret_cast<TMethod&>(Event));
        break;
      }
    }
    return PyErr_Occurred() ? NULL : (Py_INCREF(Py_None), Py_None);
  }
  catch(Exception &E)
  {
    PyErr_SetString(PyVclException, E.Message.c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* VclGetProperty(PyObject *Self, PyObject *Args)
{
  try
  {
    TControl *Control;
    const char *Name;
    int Index = 0;
    if(!PyArg_ParseTuple(Args, "is|i", &Control, &Name, &Index))
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
      PyErr_SetString(PyPropertyException, (AnsiString("Property ") + Name + " does not exist in class " + Control->ClassName()).c_str());
      return NULL;
    }

    int Kind = (*PropInfo->PropType)->Kind;
    switch(Kind)
    {
      case tkInteger:
      case tkChar:
      case tkClass:
        return Py_BuildValue("ii", GetOrdProp(Control, PropInfo), Kind);

      case tkEnumeration:
        return Py_BuildValue("si", GetEnumProp(Control, PropInfo).c_str(), Kind);

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
        PyObject *Result = PythonCallback ? PythonCallback->Callback : Py_None;
        return Py_BuildValue("Oi", Result, Kind);
      }

      default:
        Py_RETURN_NONE;
    }
  }
  catch(Exception &E)
  {
    PyErr_SetString(PyVclException, E.Message.c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
static PyObject* VclGetPropertyList(PyObject *Self, PyObject *Args)
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
static PyObject* VclCallMethod(PyObject *Self, PyObject *Args)
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
    Py_RETURN_NONE;
  }
  catch(Exception &E)
  {
    PyErr_SetString(PyVclException, E.Message.c_str());
    return NULL;
  }
}
//---------------------------------------------------------------------------
typedef int __fastcall (*TFastcallFunction)(int,int);
struct TFunctionEntry
{
  const char *Name;
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
    const char *Name;
    PyObject *Arg1;
    PyObject *ResultType;
    if(!PyArg_ParseTuple(Args, "sOO", &Name, &ResultType, &Arg1))
      return NULL;
    TFastcallFunction Function = NULL;
    AnsiString FunctionName = Name;
    for(int I = 0; I < sizeof(FunctionList) / sizeof(FunctionList[0]); I++)
      if(FunctionName == FunctionList[I].Name)
      {
        Function = FunctionList[I].Function;
        break;
      }
      
    if(Function == NULL)
      Py_RETURN_NONE;

    int pArg1 = 0, pArg2 = 0, pResult = 0;
    AnsiString Str1, Str2;
    WideString WStr1, WStr2;
    if(PyString_Check(Arg1))
    {
      Str1 = PyString_AsString(Arg1);
      pArg1 = (int)Str1.data();
    }
    else if(PyUnicode_Check(Arg1))
    {
      WStr1 = (wchar_t*)PyUnicode_AsUnicode(Arg1);
      pArg1 = (int)&WStr1;
    }
    else if(PyInt_Check(Arg1))
      pArg1 = (int)PyInt_AsLong(Arg1);
    else
      Py_RETURN_NONE;

    if(ResultType == (PyObject*)&PyString_Type)
      pArg2 = (int)&Str2;
    else if(ResultType == (PyObject*)&PyUnicode_Type)
      pArg2 = (int)&WStr2;
    else if(ResultType == (PyObject*)&PyInt_Type)
      ;//pResult = (int)&Int;
    else
      Py_RETURN_NONE;

//    Int = TextToShortCut(Str1);
//    Str1 = ShortCutToText(Int);
    pResult = Function(pArg1, pArg2);

    if(ResultType == (PyObject*)&PyString_Type)
      return PyString_FromString(Str2.c_str());
    else if(ResultType == (PyObject*)&PyUnicode_Type)
      return PyUnicode_FromWideChar(WStr2.c_bstr(), WStr2.Length());
    else if(ResultType == (PyObject*)&PyInt_Type)
      return PyInt_FromLong(pResult);
    else
      Py_RETURN_NONE;
  }
  catch(Exception &E)
  {
    PyErr_SetString(PyVclException, E.Message.c_str());
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
void InitPyVcl()
{
  RegisterClass(__classid(TForm));
  RegisterClass(__classid(TEdit));
  RegisterClass(__classid(TLabel));
  RegisterClass(__classid(TButton));
  RegisterClass(__classid(TTntAction));
  RegisterClass(__classid(TTntMenuItem));

  PyObject *PyVclModule = Py_InitModule("PyVcl", PyVclMethods);

  PyPropertyException = PyErr_NewException("PyVcl.PropertyError", NULL, NULL);
  Py_INCREF(PyPropertyException);
  PyModule_AddObject(PyVclModule, "PropertyError", PyPropertyException);

  PyVclException = PyErr_NewException("PyVcl.VclException", NULL, NULL);
  Py_INCREF(PyVclException);
  PyModule_AddObject(PyVclModule, "VclError", PyVclException);
}
//---------------------------------------------------------------------------
}

















