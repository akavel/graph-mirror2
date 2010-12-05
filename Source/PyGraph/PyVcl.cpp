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
#include "ExtColorBox.h"
#include "FindClass.hpp"
#include <OleCtrls.hpp>
#include <clipbrd.hpp>
#define private public
#include <Rtti.hpp>
#undef private

namespace Python
{
static TRttiContext Context;
//---------------------------------------------------------------------------
PyObject *PyVclException = NULL;
TValue ToValue(PyObject *O, TTypeInfo *TypeInfo);
//---------------------------------------------------------------------------
template<class T> T VclCast(TObject *Object)
{
	if(T O = dynamic_cast<T>(Object))
		return O;
	throw EConvertError(AnsiString().sprintf("Cannot convert object of type '%s' to '%s'",
		AnsiString(Object->ClassName()).c_str(), typeid(T).name()));
}
//---------------------------------------------------------------------------
struct TPythonCallback : public TCppInterfacedObject<TMethodImplementationCallback>
{
	void __fastcall Invoke(void * UserData, const System::DynamicArray<TValue> Args, TValue &Result);
};
TPythonCallback *PythonCallback = new TPythonCallback;
//---------------------------------------------------------------------------
void __fastcall TPythonCallback::Invoke(void * UserData, const System::DynamicArray<TValue> Args, TValue &Result)
{
  TLockGIL Dummy;
	PyObject *Object = static_cast<PyObject*>(UserData);
	int Count = Args.get_length() - 1;
	PyObject *PyArgs = Count != 0 ? PyTuple_New(Count) : NULL;
	for(int I = 0; I < Count; I++)
		PyTuple_SET_ITEM(PyArgs, I, ToPyObject(Args[I + 1]));
	PyObject *PyResult = PyObject_CallObject(Object, PyArgs);
	Py_XDECREF(PyArgs);
	if(PyResult != NULL && PyResult != Py_None)
		Result = ToValue(PyResult, NULL); //Bug: Type of result missing
	Py_XDECREF(PyResult);
	if(PyResult == NULL)
	  PyErr_Print();
}
//---------------------------------------------------------------------------
TMethodImplementation::TInvokeInfo* CreateInvokeInfo(TTypeInfo *TypeInfo)
{
	TTypeData *TypeData = reinterpret_cast<TTypeData*>(&TypeInfo->Name[TypeInfo->Name[0]+1]);
	int Index = 0;
	for(int I = 0; I < TypeData->ParamCount; I++)
	{
		Index++;
		Index += TypeData->ParamList[Index] + 1;
		Index += TypeData->ParamList[Index] + 1;
	}
	if(TypeData->MethodKind == mkFunction)
	{
		Index += TypeData->ParamList[Index] + 1;
		Index += 4;
	}
	Typinfo::TCallConv CallConv = static_cast<Typinfo::TCallConv>(TypeData->ParamList[Index]);
	Index++;
	TMethodImplementation::TInvokeInfo *InvokeInfo = new TMethodImplementation::TInvokeInfo(CallConv, true);
	InvokeInfo->AddParameter(__delphirtti(TObject), false);
	for(int I = 0; I < TypeData->ParamCount; I++)
	{
		TTypeInfo *ParamType = **(TTypeInfo***)&TypeData->ParamList[Index];
		InvokeInfo->AddParameter(ParamType, false);
		Index += 4;
	}
	InvokeInfo->Seal();
	return InvokeInfo;
}
//---------------------------------------------------------------------------
TValue ToValue(PyObject *O, TTypeInfo *TypeInfo)
{
	TValue Result;
	switch(TypeInfo->Kind)
	{
		case tkClass:
			if(O == Py_None || PyLong_Check(O))
				Result = TValue::From((TObject*)(O == Py_None ? NULL : PyLong_AsLong(O)));
			else
				throw EPyVclError("Cannot convert Python object of type '" + String(O->ob_type->tp_name) + "' to '" + AnsiString(TypeInfo->Name) + "'");
			break;

		case tkEnumeration:
			if(PyUnicode_Check(O))
				TValue::Make(GetEnumValue(TypeInfo, PyUnicode_AsUnicode(O)), TypeInfo, Result);
			if(PyLong_Check(O))
				TValue::Make(PyLong_AsLong(O), TypeInfo, Result);
			break;

		case tkSet:
			TValue::Make(StringToSet(TypeInfo, PyUnicode_AsUnicode(O)), TypeInfo, Result);
			break;

		case tkInteger:
			Result = TValue::From(PyLong_AsLong(O));
			break;

		case tkUString:
		case tkString:
		case tkLString:
		case tkWString:
			Result = TValue::From(String(PyUnicode_AsUnicode(O)));
			break;

		case tkMethod:
		{
			if(O == Py_None)
				return TValue::From<TObject*>(NULL);
			Py_INCREF(O);
			TMethodImplementation *Implementation = new TMethodImplementation(O, CreateInvokeInfo(TypeInfo), PythonCallback);
			TMethod Method = {Implementation->CodeAddress, NULL};
			TValue::Make(&Method, TypeInfo, Result);
			break;
		}
		case tkChar:
		case tkWChar:
			if(PyUnicode_GetSize(O) != 1)
				throw EPyVclError("Expected string with one character");
			Result = TValue::From(PyUnicode_AsUnicode(O)[0]);
			break;

		case tkFloat:
			Result = TValue::From(PyFloat_AsDouble(O));
			break;

		case tkVariant:
		case tkArray:
		case tkRecord:
		case tkInterface:
		case tkInt64:
		case tkDynArray:
		case tkClassRef:
		case tkPointer:
		case tkProcedure:
		case tkUnknown:
		default:
			throw EPyVclError("Cannot convert Python object of type '" + String(O->ob_type->tp_name) + "' to '" + AnsiString(TypeInfo->Name) + "'");
	}
	if(PyErr_Occurred())
		throw EPyVclError("Cannot convert Python object of type '" + String(O->ob_type->tp_name) + "' to '" + AnsiString(TypeInfo->Name) + "'");
	return Result;
}
//---------------------------------------------------------------------------
void TupleToValues(PyObject *O, std::vector<TValue> &Values, const DynamicArray<TRttiParameter*> &Parameters)
{
	Values.clear();
	unsigned Size = PyTuple_Size(O);
	for(unsigned I = 0; I < Size; I++)
		Values.push_back(ToValue(PyTuple_GetItem(O, I), Parameters[I]->ParamType->Handle));
}
//---------------------------------------------------------------------------
static PyObject* VclFindClass(PyObject *Self, PyObject *Args)
{
	try
	{
		const wchar_t *Name;
		if(!PyArg_ParseTuple(Args, "u", &Name))
			return NULL;

		TMetaClass *Class = GetClass(Name);
		TRttiType *Type;
		if(Class != NULL)
			Type = Context.GetType(Class);
		else
		  Type = Context.GetType(LookUpClass(Name)); //Throws exception on failure
		return Py_BuildValue("i", Type); //Return pointer to meta class, NULL on failure
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
		TObject *Object;
		const wchar_t *Name;
		PyObject *Value = NULL;
		if(!PyArg_ParseTuple(Args, "iuO", &Object, &Name, &Value))
			return NULL;
		TRttiType *Type = Context.GetType(Object->ClassType());
		TRttiProperty *Property = Type->GetProperty(Name);
		if(Property == NULL)
			throw EPyVclError("Property " + String(Name) + " not found in " + Type->Name);
		Property->SetValue(Object, ToValue(Value, Property->PropertyType->Handle));
		Py_RETURN_NONE;
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
		TObject *Object;
		const wchar_t *Name;
		int Index = -1;
		if(!PyArg_ParseTuple(Args, "iu|i", &Object, &Name, &Index))
			return NULL;

		String PropertyName = Name;
		TRttiType *Type = Context.GetType(Object->ClassType());
		if(Type == NULL)
			throw EPyVclError("Type not found.");
		TRttiProperty *Property = Type->GetProperty(Name);
		if(Property == NULL)
		{
			TRttiMethod *Method = Type->GetMethod(Name);
			if(Method == NULL)
			{
				TRttiField *Field = Type->GetField(Name);
				if(Field == NULL || Field->Visibility != mvPublic)
				{
					if(PropertyName == "Items")
					{
						if(TCollection *Collection = dynamic_cast<TCollection*>(Object))
							return Py_BuildValue("ii", Index == -1 ? 0 : Collection->Items[Index], Index == -1 ? 20 : tkClass);
					}
					else if(PropertyName == "Strings")
					{
						if(TStrings *Strings = dynamic_cast<TStrings*>(Object))
							return Py_BuildValue("iu", Index == -1 ? NULL : Strings->Strings[Index].c_str(), Index == -1 ? 20 : tkString);
					}
					else if(PropertyName == "Forms")
					{
						if(TScreen *Screen = dynamic_cast<TScreen*>(Object))
							return Py_BuildValue("ii", Index == -1 ? 0 : Screen->Forms[Index], Index == -1 ? 20 : tkClass);
					}
					else if(PropertyName == "Components")
					{
						if(TComponent *Component = dynamic_cast<TComponent*>(Object))
							return Py_BuildValue("ii", Index == -1 ? 0 : Component->Components[Index], Index == -1 ? 20 : tkClass);
					}
					else if(PropertyName == "Controls")
					{
						if(TWinControl *Control = dynamic_cast<TWinControl*>(Object))
							return Py_BuildValue("ii", Index == -1 ? 0 : Control->Controls[Index], Index == -1 ? 20 : tkClass);
					}
					throw EPyVclError("Property " + String(Name) + " not found");
				}
				TValue Result = Field->GetValue(Object);
				return Py_BuildValue("Ni", ToPyObject(Result), Result.Kind);
			}
			return Py_BuildValue("ii", Method, 10);
		}
		TValue Result = Property->GetValue(Object);
		return Py_BuildValue("Ni", ToPyObject(Result), Result.Kind);
	}
	catch(EListError &E)
	{
		PyErr_SetString(PyExc_IndexError, AnsiString(E.Message).c_str());
		return NULL;
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

	TRttiType *Type = Context.GetType(Object->ClassType());
	DynamicArray<TRttiProperty*> Properties = Type->GetProperties();
	int Count = Properties.Length;
	PyObject *List = PyList_New(Count);
	for(int I = 0; I < Count; I++)
		PyList_SetItem(List, I, ToPyObject(Properties[I]->Name));
	return List;
}
//---------------------------------------------------------------------------
static PyObject* VclCallMethod(PyObject *Self, PyObject *Args)
{
	try
	{
		TObject *Object;
		const wchar_t *Name;
		PyObject *Tuple;
		if(!PyArg_ParseTuple(Args, "iuO", &Object, &Name, &Tuple))
			return NULL;

		TRttiType *Type = dynamic_cast<TRttiType*>(Object);
		if(Type == NULL)
			Type = Context.GetType(Object->ClassType());
		TRttiMethod *Method = Type->GetMethod(Name);
		if(Method == NULL)
		  throw EPyVclError(String(Type->Name) + " has no method '" + Name + "'");
		DynamicArray<TRttiParameter*> ParameterTypes = Method->GetParameters();
		std::vector<TValue> Parameters;
		int ParamCount = PyTuple_Size(Tuple);
		if(ParamCount != ParameterTypes.get_length())
		  throw EPyVclError(String(Name) + "() takes exactly " + ParameterTypes.get_length() + " arguments (" + ParamCount + " given)");
		TupleToValues(Tuple, Parameters, ParameterTypes);
		TValue Result;
		if(Method->IsConstructor)
		{
			Result = Method->Invoke(Type->AsInstance->MetaclassType, Parameters.size() == 0 ? NULL : &Parameters[0], Parameters.size()-1);
			return Py_BuildValue("ii", Result.AsObject(), Result.Kind);
		}
		else
		{
			if(Method->IsClassMethod)
				Result = Method->Invoke(Object->ClassType(), Parameters.size() == 0 ? NULL : &Parameters[0], Parameters.size()-1);
			else
				Result = Method->Invoke(Object, Parameters.size() == 0 ? NULL : &Parameters[0], Parameters.size()-1);
			return Py_BuildValue("Ni", ToPyObject(Result), Result.Kind);
		}

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
const int MaxArgCount = 1;
struct TFunctionEntry
{
	const wchar_t *Name;
	void *Address;
	TTypeInfo *Result;
	TTypeInfo *Args[MaxArgCount];
};

TFunctionEntry FunctionList[] =
{
	{L"ShortCutToText", ShortCutToText, __delphirtti(String), __delphirtti(TShortCut)},
	{L"TextToShortCut", TextToShortCut, __delphirtti(TShortCut), __delphirtti(String)},
};
//---------------------------------------------------------------------------
static PyObject* VclCallFunction(PyObject *Self, PyObject *Args)
{
	try
	{
		const wchar_t *Name;
		PyObject *FuncArgs;
		if(!PyArg_ParseTuple(Args, "uO!", &Name, &PyTuple_Type, &FuncArgs))
			return NULL;
		String FunctionName = Name;
		TFunctionEntry *Function = NULL;
		for(unsigned I = 0; I < sizeof(FunctionList) / sizeof(FunctionList[0]); I++)
			if(FunctionName == FunctionList[I].Name)
			{
				Function = &FunctionList[I];
				break;
			}

		if(Function == NULL)
			throw EPyVclError("Function " + FunctionName + " not found!");

		DynamicArray<TValue> Arguments;

		int Count;
		for(Count=0; Count < MaxArgCount && Function->Args[Count] != NULL; Count++);
		Arguments.Length = Count;
		if(Count != PyTuple_Size(FuncArgs))
			throw EPyVclError("Function argument count mismatch!");

		for(int I = 0; I < Count; I++)
			 Arguments[I] = ToValue(PyTuple_GET_ITEM(FuncArgs, I), Function->Args[I]);

		TValue Result = Invoke(Function->Address, Arguments, ccReg, Function->Result);
		return ToPyObject(Result);
	}
	catch(Exception &E)
	{
		PyErr_SetString(PyVclException, AnsiString(E.Message).c_str());
		return NULL;
	}
}
//---------------------------------------------------------------------------
static PyMethodDef PyVclMethods[] = {
	{"FindClass", VclFindClass, METH_VARARGS, ""},
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
	PyObject *PyVclModule = PyModule_Create(&PyVclModuleDef);

	PyVclException = PyErr_NewException("PyVcl.VclException", NULL, NULL);
	Py_INCREF(PyVclException);
	PyModule_AddObject(PyVclModule, "VclError", PyVclException);
	PyModule_AddIntConstant(PyVclModule, "Application", reinterpret_cast<int>(Application));
	PyModule_AddIntConstant(PyVclModule, "Clipboard", reinterpret_cast<int>(Clipboard));
	PyModule_AddIntConstant(PyVclModule, "Mouse", reinterpret_cast<int>(Mouse));
	PyModule_AddIntConstant(PyVclModule, "Screen", reinterpret_cast<int>(Screen));
	return PyVclModule;
}
//---------------------------------------------------------------------------
}

