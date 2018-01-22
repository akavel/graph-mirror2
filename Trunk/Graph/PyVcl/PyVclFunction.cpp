/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Platform.h"
#pragma hdrstop
#include "Python.hpp"
#include "PyVclConvert.h"
#include "PyVcl.h"
#include "FindClass.hpp"

namespace Python
{
PyTypeObject *VclFunction_Type = NULL;
//---------------------------------------------------------------------------
const int MaxArgCount = 5;
struct TFunctionEntry
{
	const wchar_t *Name;
	void *Address;
	TTypeInfo *Result;
	TTypeInfo *Args[MaxArgCount];
};
//---------------------------------------------------------------------------
template <typename T, bool IsDelphiClass>
struct TDelphiRttiHelper
{
	static PTypeInfo GetTypeInfo()	{return __delphirtti (T);}
};

template <typename T>
struct TDelphiRttiHelper<T, true>
{
	static PTypeInfo GetTypeInfo() {return static_cast<PTypeInfo>(__classid(T)->ClassInfo());}
};

template <typename T> inline PTypeInfo Rtti()
{
	return TDelphiRttiHelper<T, __is_base_of(System::TObject, T)>::GetTypeInfo();
}
//---------------------------------------------------------------------------
/** List of global VCL function made available to PyVcl.
 *  \Warning: Don't use __delphirtti() on classes. It may make it impossible to access
 *  properties on that class through RTTI. See QC #90773
 */
static const TFunctionEntry FunctionList[] =
{
#if !FIREMONKEY
	{L"ShortCutToText", ShortCutToText, __delphirtti(String), __delphirtti(TShortCut)},
	{L"TextToShortCut", TextToShortCut, __delphirtti(TShortCut), __delphirtti(String)},
  {L"CreateMessageDialog", (TForm* __fastcall (*)(const UnicodeString, TMsgDlgType, TMsgDlgButtons, TMsgDlgBtn))&CreateMessageDialog, Rtti<Forms::TForm>(), __delphirtti(String), Rtti<TMsgDlgType>(), Rtti<TMsgDlgButtons>(), Rtti<TMsgDlgBtn>()},
#endif
	{L"ReadComponentResFile", ReadComponentResFile, Rtti<TComponent>(), __delphirtti(String), Rtti<TComponent>()},
	{L"ObjectTextToBinary", (void __fastcall (*)(TStream*, TStream*))ObjectTextToBinary, NULL, Rtti<TStream>(), Rtti<TStream>()},
  {L"ShowMessage", ShowMessage, NULL, __delphirtti(String)},
  {L"MessageDlg", (int __fastcall (*)(const UnicodeString, TMsgDlgType, TMsgDlgButtons, int, TMsgDlgBtn))&MessageDlg, __delphirtti(int), __delphirtti(String), Rtti<TMsgDlgType>(), Rtti<TMsgDlgButtons>(), __delphirtti(int), Rtti<TMsgDlgBtn>()},
};
//---------------------------------------------------------------------------
struct TVclFunction
{
	PyObject_HEAD
	const TFunctionEntry *Function;
};
//---------------------------------------------------------------------------
/** Return string representation of the given VclFunction.
 */
static PyObject *VclFunction_Repr(TVclFunction* self)
{
	String Str = (self->Function->Result ? "<Function " : "<procedure ") + String(self->Function->Name) + L"(";
	for(int I = 0; I < MaxArgCount && self->Function->Args[I] != NULL; I++)
		Str += AnsiString(self->Function->Args[I]->Name) + ";";
	Str[Str.Length()] = ')';
	if(self->Function->Result)
		Str += ":" + AnsiString(self->Function->Result->Name);
	Str += ">";
	return ToPyObject(Str);
}
//---------------------------------------------------------------------------
/** Call a global VCL function through the VclFunction object.
 */
static PyObject *VclFunction_Call(TVclFunction* self, PyObject *args, PyObject *keywds)
{
	try
	{
		if(keywds != NULL)
			throw EPyVclError("Function does not accept keyword arguments");
		DynamicArray<TValue> Arguments;

		int Count = 0;
		while(Count < MaxArgCount && self->Function->Args[Count] != NULL)
      Count++;
		Arguments.Length = Count;
		if(Count != PyTuple_Size(args))
			throw EPyVclError("Function argument count mismatch!");

		for(int I = 0; I < Count; I++)
			 Arguments[I] = ToValue(PyTuple_GetItem(args, I), self->Function->Args[I]);

		TValue Result = Invoke(self->Function->Address, Arguments, ccReg, self->Function->Result);
		return ToPyObject(Result);
	}
	catch(...)
	{
		return PyVclHandleException();
	}
}
//---------------------------------------------------------------------------
/** A VclFunction is a proxy of a global VCL function made availabe to PyVcl.
 */
static PyType_Slot VclFunction_Slots[] =
{
  {Py_tp_doc,	(void*) PROJECT_NAME " global function object"},
  {Py_tp_repr, VclFunction_Repr},
  {Py_tp_call, VclFunction_Call},
  {0, NULL}
};

static PyType_Spec VclFunction_Spec =
{
  GUI_TYPE "Function",  //name
  sizeof(TVclFunction), //basicsize
  0,                    //itemsize
  Py_TPFLAGS_DEFAULT,   //flags
  VclFunction_Slots,     //slots
};
//---------------------------------------------------------------------------
/** Initialize the VclClosure_Type type object.
 *  \return true on success
 */
bool VclFunction_Init()
{
  VclFunction_Type = reinterpret_cast<PyTypeObject*>(PyType_FromSpec(&VclFunction_Spec));
  return VclFunction_Type != NULL;
}
//---------------------------------------------------------------------------
/** Create a new VclFunction object from a name.
 *  \param Name: The name of the function. Must be case sensitive.
 *  \return New reference to a VCLFunction object.
 */
PyObject* VclFunction_Create(const String &Name)
{
	TFunctionEntry *Function = NULL;
	for(unsigned I = 0; I < sizeof(FunctionList) / sizeof(FunctionList[0]); I++)
		if(Name == FunctionList[I].Name)
		{
			TVclFunction *VclFunction = PyObject_New(TVclFunction, VclFunction_Type);
			VclFunction->Function = &FunctionList[I];
      //We need to increment refcnt for the type as it is decremented by the default tp_dealloc when the object is destroyed
      Py_INCREF(VclFunction_Type);
			return reinterpret_cast<PyObject*>(VclFunction);
		}

	return NULL;
}
//---------------------------------------------------------------------------
/** Retrieve number of global VCL functions in the list. 
 */
unsigned GetVclFunctionCount()
{
	return sizeof(FunctionList)/sizeof(FunctionList[0]);
}
//---------------------------------------------------------------------------
/** Retrieve name of VCL global by index.
 */
const wchar_t* GetVclFunctionName(unsigned Index)
{
	if(Index < sizeof(FunctionList)/sizeof(FunctionList[0]))
		return FunctionList[Index].Name;
	return NULL;
}
//---------------------------------------------------------------------------
} //namespace Python
