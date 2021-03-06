/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#define _XKEYCHECK_H	//Disable check for defining keywords
#define private public  //Nasty hack to get access to TMethodImplementation::FUserData
#include <Rtti.hpp>
#undef private
#include "Platform.h"
#pragma hdrstop
#include "Python.hpp"
#include "PyVclConvert.h"
#include "PyVclObject.h"
#include "PyVclMethod.h"
#include "PyVclClosure.h"
#include "PyVclRef.h"
//---------------------------------------------------------------------------
namespace Python
{
TRttiContext Context;
PyObject *PyVclException = NULL;
//---------------------------------------------------------------------------
String GetTypeName(PyObject *O)
{
  TPyObjectPtr Type(PyObject_GetAttrString(reinterpret_cast<PyObject*>(O->ob_type), "__name__"), false);
  if(Type.get())
    return FromPyObject<String>(Type.get());
  PyErr_Clear();
  return "";
}
//---------------------------------------------------------------------------
TValue ToValue(PyObject *O)
{
  if(PyUnicode_Check(O))
    return TValue::From(FromPyObject<String>(O));
	throw EPyVclError("Cannot convert Python object of type '" + GetTypeName(O) + "' to 'TValue'");
}
//---------------------------------------------------------------------------
/** Convert a PyObject to a TValue as used when calling functions and setting properties in a generic way through Delphi RTTI.
 *  \param O: PyObject to convert
 *  \param TypeInfo: The expected return type
 *  \return A TValue with a value of the type given by TypeInfo.
 *  \throw EPyVclError on conversion errors.
 */
TValue ToValue(PyObject *O, TTypeInfo *TypeInfo)
{
  if(VclRef_Check(O))
    return VclRef_AsValue(O);

	TValue Result;
	switch(TypeInfo->Kind)
	{
		case tkClass:
			if(VclObject_Check(O))
				Result = TValue::From(VclObject_AsObject(O));
			else if(O == Py_None)
				Result = TValue::From((TObject*)NULL);
			else
				throw EPyVclError("Cannot convert Python object of type '" + GetTypeName(O) + "' to '" + AnsiString(TypeInfo->Name) + "'");
			break;

		case tkEnumeration:
			if(PyLong_Check(O))
				TValue::Make(PyLong_AsLong(O), TypeInfo, Result);
      else
				TValue::Make(GetEnumValue(TypeInfo, FromPyObject<String>(O)), TypeInfo, Result);
			break;

		case tkSet:
    {
      String SetStr;
      if(PySet_Check(O))
      {
        TPyObjectPtr Sep = ToPyObjectPtr(",");
        TPyObjectPtr Str(PyUnicode_Join(Sep.get(), O), false);
        SetStr = FromPyObject<String>(Str.get());
      }
      else
        SetStr = FromPyObject<String>(O);
			TValue::Make(StringToSet(TypeInfo, SetStr), TypeInfo, Result);
			break;
    }
		case tkInteger:
			Result = TValue::From(PyLong_AsUnsignedLongMask(O));
			break;

		case tkUString:
		case tkString:
		case tkLString:
		case tkWString:
			Result = TValue::From(FromPyObject<String>(O));
			break;

		case tkChar:
		case tkWChar:
			if(PyUnicode_GetSize(O) != 1)
				throw EPyVclError("Expected string with one character");
			Result = TValue::From(FromPyObject<String>(O)[1]);
			break;

		case tkFloat:
			Result = TValue::From(PyFloat_AsDouble(O));
			break;

		case tkRecord:
      if(AnsiString(TypeInfo->Name) == "TValue") //Can this be optimized?
      {
        TValue V = ToValue(O);
  			TValue::Make(&V, TypeInfo, Result);
      }
      else
      {
        TRttiType *Type = Context.GetType(TypeInfo);
        std::vector<BYTE> Data(Type->TypeSize);
        DynamicArray<TRttiField*> Fields = Type->GetFields();
        if(PyTuple_Size(O) != Fields.Length)
          throw EPyVclError("Expected tuple with " + IntToStr(Fields.Length) + " elements");
        for(int I = 0; I < Fields.Length; I++)
          Fields[I]->SetValue(&Data[0], ToValue(PyTuple_GetItem(O, I), Fields[I]->FieldType->Handle));
        TValue::Make(&Data[0], TypeInfo, Result);
      }
			break;

		case tkInt64:
			Result = TValue::From(PyLong_AsLongLong(O));
			break;

		case tkPointer:
//      if(AnsiString(TypeInfo->Name) == "PWideChar")
//    		TValue::Make(reinterpret_cast<int>(PyUnicode_AsUnicode(O)), TypeInfo, Result);
//      else
    		throw EPyVclError("Cannot convert Python object of type '" + GetTypeName(O) + "' to '" + AnsiString(TypeInfo->Name) + "'");
      break;

		case tkClassRef:
		case tkVariant:
		case tkArray:
		case tkInterface:
		case tkDynArray:
		case tkProcedure:
		case tkUnknown:
		case tkMethod:
		default:
			throw EPyVclError("Cannot convert Python object of type '" + GetTypeName(O) + "' to '" + AnsiString(TypeInfo->Name) + "'");
	}
	if(PyErr_Occurred())
 		throw EPyVclError("Cannot convert Python object of type '" + GetTypeName(O) + "' to '" + AnsiString(TypeInfo->Name) + "'");
	return Result;
}
//---------------------------------------------------------------------------
/** Convert the content for a tuple to a vector of TValue that can be passed to the Delphi RTTI system.
 *  \param O: This must be a tuple. Not checked.
 *  \param Values: vector filled with the return values.
 *  \param Parameters: This must be the expected types for the converted values. The number of elements 
 *         must be the same as the number of items in the tuple.
 */
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
  return PyUnicode_FromWideChar(&Value, 1);
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(double Value)
{
	return PyFloat_FromDouble(Value);
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(const std::wstring &Str)
{
  return PyUnicode_FromWideChar(Str.c_str(), Str.size());
}
//---------------------------------------------------------------------------
PyObject* ToPyObject(const String &Str)
{
#ifdef _Windows
	return PyUnicode_FromWideChar(Str.c_str(), Str.Length());
#else
	return PyUnicode_DecodeUTF16(reinterpret_cast<const char*>(Str.c_str()), Str.Length(), NULL, NULL);
#endif
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
PyObject* ToPyObject(const Variant &V)
{
  return ToPyObject(Rtti::TValue::FromVariant(V));
}
//---------------------------------------------------------------------------
/** Convert a TValue from the Delphi RTTI system to a PyObject*.
 *  \param V: The value to convert.
 *  \return New reference to a converted object.
 *  \throw EPyVclError if the conversion fails.
 */
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
      AnsiString Name = Value.TypeInfo->Name;
			if(Name == "Boolean" || Name == "bool")
				return ToPyObject(static_cast<bool>(Value.AsOrdinal()));
			TRttiEnumerationType *Type = static_cast<TRttiEnumerationType*>(Context.GetType(Value.TypeInfo));
			int Ordinal = Value.AsOrdinal();
			if(Ordinal >= Type->MinValue && Ordinal <= Type->MaxValue)
			  //GetEnumName() lacks range check
				return ToPyObject(GetEnumName(Value.TypeInfo, Ordinal));
			return ToPyObject("0x" + IntToHex(Ordinal, 2));
		}

		case tkClass:
			return VclObject_Create(Value.AsObject(), false);

		case tkSet:
    {
      String Str = SetToString(Value.TypeInfo, *static_cast<int*>(Value.GetReferenceToRawData()), false);
      if(Str.Length() > 0)
      {
        TPyObjectPtr SetStr = ToPyObjectPtr(Str);
        TPyObjectPtr Sep = ToPyObjectPtr(",");
        TPyObjectPtr List(PyUnicode_Split(SetStr.get(), Sep.get(), -1), false);
        return PySet_New(List.get());
      }
      return PySet_New(NULL);
    }
		case tkChar:
			return ToPyObject(Value.AsType<char>());

		case tkWChar:
			return ToPyObject(Value.AsType<wchar_t>());

		case tkFloat:
			return ToPyObject(Value.AsExtended());

		case tkRecord:
		{
			void *Data = Value.GetReferenceToRawData();
      if(AnsiString(Value.TypeInfo->Name) == "TValue") //It should be possible to optimize this!
        return ToPyObject(*static_cast<TValue*>(Data));
			TRttiType *Type = Context.GetType(Value.TypeInfo);
			DynamicArray<TRttiField*> Fields = Type->GetFields();
			PyObject *Tuple = PyTuple_New(Fields.Length);
			for(int I = 0; I < Fields.Length; I++)
        if(Fields[I]->FieldType != NULL)
  				PyTuple_SetItem(Tuple, I, ToPyObject(Fields[I]->GetValue(Data)));
        else
  				PyTuple_SetItem(Tuple, I, (Py_INCREF(Py_None), Py_None));
			return Tuple;
		}

		case tkMethod: //Event
		{
			TMethod Method;
			Value.ExtractRawDataNoCopy(&Method);
			if(Method.Code == NULL)
				Py_RETURN_NONE;
			TObject *Object = static_cast<TObject*>(Method.Data);
			if(TMethodImplementation *Impl = dynamic_cast<TMethodImplementation*>(Object))
			{
				PyObject *Result = static_cast<PyObject*>(Impl->FUserData);
				Py_INCREF(Result);
				return Result;
			}
      return VclClosure_Create(Value);
		}

		case tkInt64:
			return PyLong_FromLongLong(Value.AsInt64());

		case tkArray:
    {
      PyObject *O = PyList_New(Value.GetArrayLength());
      if(O != NULL)
      {
        Py_ssize_t Size = PyList_Size(O);
        for(int I = 0; I < Size; I++)
          PyList_SetItem(O, I, ToPyObject(Value.GetArrayElement(I)));
      }
      return O;
    }
		case tkPointer:
		case tkVariant:
		case tkInterface:
		case tkDynArray:
		case tkClassRef:
		case tkProcedure:
		case tkUnknown:
		default:
			throw EPyVclError("Unable to convert type '" + AnsiString(Value.TypeInfo->Name) + "'");
	}
}
//---------------------------------------------------------------------------
template<> double FromPyObject<double>(PyObject *O)
{
  double Result;
	if(PyComplex_Check(O))
  {
    if(PyComplex_ImagAsDouble(O) != 0)
      throw EPyVclError("Complex number has an imaginary part");
		Result = PyComplex_RealAsDouble(O);
  }
  else
    Result = PyFloat_AsDouble(O);
	if(PyErr_Occurred())
  {
    PyErr_Clear();
		throw EPyVclError("Cannot convert Python object of type '" + GetTypeName(O) + "' to 'double'");
  }
  return Result;
}
//---------------------------------------------------------------------------
template<> std::wstring FromPyObject<std::wstring>(PyObject *O)
{
  Py_ssize_t Size = PyUnicode_GetSize(O);
  if(Size == -1)
  {
    PyErr_Clear();
		throw EPyVclError("Cannot convert Python object of type '" + GetTypeName(O) + "' to 'wstring'");
  }
  std::wstring Str(Size, ' ');
  if(Str.size() == 0)
    return Str;
  PyUnicode_AsWideChar(O, &Str[0], Str.size());
	return Str;
}
//---------------------------------------------------------------------------
template<> String FromPyObject<String>(PyObject *O)
{
  String Str;
#ifdef _Windows
  Py_ssize_t Size = PyUnicode_GetSize(O);
  if(Size == -1)
  {
    PyErr_Clear();
		throw EPyVclError("Cannot convert Python object of type '" + GetTypeName(O) + "' to 'String'");
  }
  Str.SetLength(Size);
  if(Str.Length() == 0)
	  return Str;
  PyUnicode_AsWideChar(O, &Str[1], Str.Length());
#else
	TPyObjectPtr UTF16(PyUnicode_AsUTF16String(O), false);
  if(UTF16)
  {
    Py_ssize_t Size;
    char *Data;
    if(PyBytes_AsStringAndSize(UTF16.get(), &Data, &Size) != -1)
    {
      Str.SetLength(Size / 2);
      memcpy(&Str[1], Data, Size);
    }
  }
#endif
	return Str;
}
//---------------------------------------------------------------------------
template<> int FromPyObject<int>(PyObject *O)
{
	int Result = PyLong_AsLong(O);
	if(PyErr_Occurred())
  {
    PyErr_Clear();
		throw EPyVclError("Cannot convert Python object of type '" + GetTypeName(O) + "' to 'double'");
  }
  return Result;
}
//---------------------------------------------------------------------------
/** Exception handling helper function. Called from a catch(...) section and converts an active
 *  C++/Delphi exception to a Python exception.
 */
PyObject* PyVclHandleException()
{
	try
	{
		throw;
	}
  //WARNING: The 64 bit compiler in C++ Builder XE4 does not handle catch of exceptions
  //derived from Exception correctly. See QC #116246
  catch(EAbort &E)
  { //This is used when a Python exception has already been set. Mostly useful for debugging.
  }
	catch(EStringListError &E)
	{
		SetErrorString(PyExc_IndexError, E.Message);
	}
	catch(EListError &E)
	{
		SetErrorString(PyExc_IndexError, E.Message);
	}
  catch(DynArrayOutOfRange &E)
  {
		PyErr_SetString(PyVclException, "Invalid number of indexes in dynamic array");
  }
  catch(DynArrayException &E)
  {
		PyErr_SetString(PyVclException, "Unknown dynamic array exception");
  }
  catch(EPyVclError &E)
  {
		SetErrorString(PyVclException, E.Message);
  }
	catch(Exception &E)
	{
		SetErrorString(PyVclException, E.ClassName() + ": " + E.Message);
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
