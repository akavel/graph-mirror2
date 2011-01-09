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
#undef _DEBUG
#include <Python.h>
#include "PyVclConvert.h"
#define private public
#include <Rtti.hpp>
#undef private
#include "PyVclObject.h"
#include "PyVclMethod.h"
//---------------------------------------------------------------------------
namespace Python
{
TRttiContext Context;
PyObject *PyVclException = NULL;
//---------------------------------------------------------------------------
TValue ToValue(PyObject *O, TTypeInfo *TypeInfo)
{
	TValue Result;
	switch(TypeInfo->Kind)
	{
		case tkClass:
			if(VclObject_Check(O))
				Result = TValue::From(VclObject_AsObject(O));
			else if(O == Py_None || PyLong_Check(O))
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

		case tkChar:
		case tkWChar:
			if(PyUnicode_GetSize(O) != 1)
				throw EPyVclError("Expected string with one character");
			Result = TValue::From(PyUnicode_AsUnicode(O)[0]);
			break;

		case tkFloat:
			Result = TValue::From(PyFloat_AsDouble(O));
			break;

		case tkRecord:
		{
			TRttiType *Type = Context.GetType(TypeInfo);
			std::vector<BYTE> Data(Type->TypeSize);
			DynamicArray<TRttiField*> Fields = Type->GetFields();
			for(int I = 0; I < Fields.Length; I++)
				Fields[I]->SetValue(&Data[0], ToValue(PyTuple_GetItem(O, I), Fields[I]->FieldType->Handle));
			TValue::Make(&Data[0], TypeInfo, Result);
			break;
		}

		case tkInt64:
			Result = TValue::From(PyLong_AsLongLong(O));
			break;

		case tkVariant:
		case tkArray:
		case tkInterface:
		case tkDynArray:
		case tkClassRef:
		case tkPointer:
		case tkProcedure:
		case tkUnknown:
		case tkMethod:
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
PyObject* ToPyObject(bool Value)
{
	return PyBool_FromLong(Value);
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(int Value)
{
	return PyLong_FromLong(Value);
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(long long Value)
{
	return PyLong_FromLongLong(Value);
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(wchar_t Value)
{
  return PyUnicode_FromUnicode(&Value, 1);
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(double Value)
{
	return PyFloat_FromDouble(Value);
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(const std::wstring &Str)
{
  return PyUnicode_FromUnicode(Str.c_str(), Str.size());
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(const String &Str)
{
	return PyUnicode_FromUnicode(Str.c_str(), Str.Length());
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(const Func32::TComplex &Value)
{
	return Value.imag() ? PyComplex_FromDoubles(Value.real(), Value.imag()) : PyFloat_FromDouble(Value.real());
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(TObject *Object)
{
	return VclObject_Create(Object, false);
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(const char *Str)
{
	return PyUnicode_DecodeASCII(Str, strlen(Str), NULL);
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(const wchar_t *Str)
{
  return PyUnicode_FromWideChar(Str, -1);
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(const Rtti::TValue &V)
{
	Rtti::TValue &Value = const_cast<Rtti::TValue&>(V);
	if(Value.IsEmpty)
		Py_RETURN_NONE;
	switch(Value.Kind)
	{
		case tkInteger:
			return ToPyObject(Value.AsInteger());

		case tkUString:
		case tkString:
		case tkLString:
		case tkWString:
			return ToPyObject(Value.AsString());

		case tkEnumeration:
		{
			if(AnsiString(Value.TypeInfo->Name) == "Boolean")
				return ToPyObject(static_cast<bool>(Value.AsOrdinal()));
			return ToPyObject(GetEnumName(Value.TypeInfo, Value.AsOrdinal()));
		}

		case tkClass:
			return VclObject_Create(Value.AsObject(), false);

		case tkSet:
			return ToPyObject(SetToString(Value.TypeInfo, *static_cast<int*>(Value.GetReferenceToRawData()), false));

		case tkChar:
			return ToPyObject(Value.AsType<char>());

		case tkWChar:
			return ToPyObject(Value.AsType<wchar_t>());

		case tkFloat:
			return ToPyObject(Value.AsExtended());

		case tkRecord:
		{
			void *Data = Value.GetReferenceToRawData();
			TRttiType *Type = Context.GetType(Value.TypeInfo);
			DynamicArray<TRttiField*> Fields = Type->GetFields();
			PyObject *Tuple = PyTuple_New(Fields.Length);
			for(int I = 0; I < Fields.Length; I++)
				PyTuple_SET_ITEM(Tuple, I, ToPyObject(Fields[I]->GetValue(Data)));
			return Tuple;
		}

		case tkMethod:
		{
			TMethod Method;
			Value.ExtractRawDataNoCopy(&Method);
			if(Method.Code == NULL)
				Py_RETURN_NONE;
			TObject *Object = static_cast<TObject*>(Method.Data);
			if(TMethodImplementation *Impl = dynamic_cast<TMethodImplementation*>(Object))
			{
				PyObject *Result = static_cast<PyObject*>(Impl->FUserData);
				Py_IncRef(Result);
				return Result;
			}
			String Name = Object->MethodName(Method.Code);
			TRttiType *Type = Context.GetType(Object->ClassType());
			return VclMethod_Create(Object, Type->GetMethods(Name));
		}

		case tkInt64:
			return PyLong_FromLongLong(Value.AsInt64());

		case tkVariant:
		case tkArray:
		case tkInterface:
		case tkDynArray:
		case tkClassRef:
		case tkPointer:
		case tkProcedure:
		case tkUnknown:
		default:
			throw EPyVclError("Unable to convert type '" + AnsiString(Value.TypeInfo->Name) + "'");
	}
}
//---------------------------------------------------------------------------
template<> double FromPyObject<double>(PyObject *O)
{
  if(PyComplex_Check(O))
    if(PyComplex_ImagAsDouble(O) != 0)
      PyErr_SetString(PyExc_TypeError, "complex number has an imaginary part");
		else
      return PyComplex_RealAsDouble(O);
  return PyFloat_AsDouble(O);
}
//---------------------------------------------------------------------------
template<> Func32::TComplex FromPyObject<Func32::TComplex>(PyObject *O)
{
  Py_complex V = PyComplex_AsCComplex(O);
	return Func32::TComplex(V.real, V.imag);
}
//---------------------------------------------------------------------------
template<> std::wstring FromPyObject<std::wstring>(PyObject *O)
{
	return PyUnicode_AsUnicode(O);
}
//---------------------------------------------------------------------------
template<> int FromPyObject<int>(PyObject *O)
{
	return PyLong_AsLong(O);
}
//---------------------------------------------------------------------------
PyObject* PyVclHandleException()
{
	try
	{
		throw;
	}
	catch(EListError &E)
	{
		PyErr_SetString(PyExc_IndexError, AnsiString(E.Message).c_str());
	}
	catch(Exception &E)
	{
		PyErr_SetString(PyVclException, AnsiString(E.Message).c_str());
	}
	catch(std::exception &E)
	{
		PyErr_SetString(PyVclException, E.what());
	}
	catch(...)
	{
		PyErr_SetString(PyVclException, "Unknown exception");
	}
	return NULL;
}
//---------------------------------------------------------------------------
} //namespace Python