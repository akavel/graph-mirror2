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
#define private public   //Because TMethodImplementation::FInvokeInfo is private in XE
#define protected public //Because TMethodImplementation::FInvokeInfo is protected in XE4
#include <Rtti.hpp>
#undef private
#undef protected
#include "Platform.h"
#pragma hdrstop
#include "Python.hpp"
#include "PyVclObject.h"
#include "PyVclMethod.h"
#include "PyVcl.h"
#include "PyVclConvert.h"
#include "PyVclIndexedProperty.h"
#include "PyVclRef.h"
#include <deque>
#include <algorithm>

namespace Python
{
PyTypeObject *VclObject_Type = NULL;
//---------------------------------------------------------------------------
/** This object is created when needed and owned by a TComponent object which should
 *  be monitored for destruction. Delphi objects not derived from TComponent cannot
 *  be monitored. Only one TObjectDeleteHandler is needed for every object monitored,
 *  and it can always be found by its name "ObjectDeleteHandler".
 *
 *  VclObject objects will register themself to be notified when the referred object
 *  is destroyed. Only one VCL object is registered. This object is reused instead of creating
 *  more proxy objects to the same instance.
 *  Events that call Python functions are also tracked for cleanup when
 *  the monitored object is destroyed.
 */
class TObjectDeleteHandler : public TComponent
{
	TVclObject* VclObject;
  std::deque<TMethodImplementation*> MethodImplementations;
public:
	__fastcall TObjectDeleteHandler(TComponent *AOwner) : TComponent(AOwner)
	{
		Name = L"ObjectDeleteHandler";
	}

	__fastcall ~TObjectDeleteHandler()
	{
		if(VclObject)
		{
			VclObject->Instance = NULL;
			VclObject->Owned = false;
    }

		for(std::deque<TMethodImplementation*>::iterator Iter = MethodImplementations.begin(); Iter != MethodImplementations.end(); ++Iter)
		{
      Py_DECREF(static_cast<PyObject*>((*Iter)->FUserData));
		  delete (*Iter)->FInvokeInfo;
   	  delete *Iter;
    }
	}

  TVclObject* GetVclObject() {return VclObject;}
	void SetVclObject(TVclObject *AVclObject) {VclObject = AVclObject;}
  void Register(TMethodImplementation *Impl) {MethodImplementations.push_back(Impl);}
	void Unregister(TMethodImplementation *Impl)
	{
		std::deque<TMethodImplementation*>::iterator Iter = std::find(MethodImplementations.begin(), MethodImplementations.end(), Impl);
		if(Iter != MethodImplementations.end())
    {
      Py_DECREF(static_cast<PyObject*>(Impl->FUserData));
		  delete Impl->FInvokeInfo;
   	  delete Impl;
		  MethodImplementations.erase(Iter);
    }
	}
};
//---------------------------------------------------------------------------
/** Find and optionally create a TObjectDeleteHandler associated to a TComponent.
 */
TObjectDeleteHandler* FindDeleteHandler(TObject *Object, bool Create)
{
	if(TComponent *Component = dynamic_cast<TComponent*>(Object))
	{
		TObjectDeleteHandler *DeleteHandler =
			dynamic_cast<TObjectDeleteHandler*>(Component->FindComponent("ObjectDeleteHandler"));
		if(DeleteHandler == NULL && Create)
			DeleteHandler = new TObjectDeleteHandler(Component);
		return DeleteHandler;
	}
  return NULL;
}
//---------------------------------------------------------------------------
/** Class with function used to assign all events that are python objects.
 *  There is only one global instance, in fact the instance is not used but is
 *  necesarry because a closure is needed.
 */
struct TPythonCallback : public TCppInterfacedObject<TMethodImplementationCallback>
{
  TPythonCallback() {AddRef();} //Always keep a global reference; Ref count should never be 0
	void __fastcall Invoke(void * UserData, System::DynamicArray<TValue> Args, TValue &Result);
};
TPythonCallback *PythonCallback = new TPythonCallback;
//---------------------------------------------------------------------------
/** Function called for all events assigned to Python functions.
 *  The function will convert the arguments passed with the event to Python objects and
 *  call the Python callback function.
 */
void __fastcall TPythonCallback::Invoke(void * UserData, System::DynamicArray<TValue> Args, TValue &Result)
{
	TLockGIL Dummy;
	TMethodImplementation *Impl = static_cast<TMethodImplementation*>(Args[0].AsObject());
	TMethodImplementation::TInvokeInfo *InvokeInfo = Impl->FInvokeInfo;
	DynamicArray<TMethodImplementation::TParamLoc> Params = InvokeInfo->GetParamLocs();
	PyObject *Object = static_cast<PyObject*>(UserData);
	int Count = Args.get_length() - 1;
	PyObject *PyArgs = Count != 0 ? PyTuple_New(Count) : NULL;
	for(int I = 0; I < Count; I++)
		PyTuple_SetItem(PyArgs, I, Params[I+1].FByRefParam ? VclRef_Create(&Args[I+1]) : ToPyObject(Args[I+1]));
	PyObject *PyResult = PyObject_CallObject(Object, PyArgs);

  //Invalidate all VclRef objects used as arguments
	for(int I = 0; I < Count; I++)
  {
    PyObject *Arg = PyTuple_GetItem(PyArgs, I);
    if(VclRef_Check(Arg))
      VclRef_Invalidate(Arg);
  }

	Py_XDECREF(PyArgs);
	if(PyResult != NULL && PyResult != Py_None)
		Result = ToValue(PyResult, NULL); //Bug: Type of result missing
	Py_XDECREF(PyResult);
	if(PyResult == NULL)
  {
    if(PyErr_ExceptionMatches(PyExc_SystemExit))
    {
      if(Application->MainForm)
        Application->MainForm->Close();
      else
        Application->Terminate();
      PyErr_Clear();
    }
    else
	    PyErr_Print();
  }
}
//---------------------------------------------------------------------------
/** Create TInvokeInfo object with parameter information for a Delphi event.
 *  This is used to decode the parameters inside the generic event handler.
 */
TMethodImplementation::TInvokeInfo* CreateInvokeInfo(TTypeInfo *TypeInfo)
{
	TTypeData *TypeData = reinterpret_cast<TTypeData*>(&TypeInfo->Name[TypeInfo->Name[0]+1]);
	int Index = 0;
	std::vector<Typinfo::TParamFlags> ParamFlags;
	for(int I = 0; I < TypeData->ParamCount; I++)
	{
		ParamFlags.push_back(reinterpret_cast<Typinfo::TParamFlags&>(TypeData->ParamList[Index++]));
		Index += TypeData->ParamList[Index] + 1;
		Index += TypeData->ParamList[Index] + 1;
	}
	if(TypeData->MethodKind == mkFunction)
	{
		Index += TypeData->ParamList[Index] + 1;
		Index += sizeof(void*);
	}
	Typinfo::TCallConv CallConv = static_cast<Typinfo::TCallConv>(TypeData->ParamList[Index]);
	Index++;
	TMethodImplementation::TInvokeInfo *InvokeInfo = new TMethodImplementation::TInvokeInfo(CallConv, true);
	InvokeInfo->AddParameter(__delphirtti(TObject), false);
	for(int I = 0; I < TypeData->ParamCount; I++)
	{
		TTypeInfo *ParamType = **(TTypeInfo***)&TypeData->ParamList[Index];
		InvokeInfo->AddParameter(ParamType, ParamFlags[I].Contains(Typinfo::pfVar));
		Index += sizeof(void*);
	}
	InvokeInfo->Seal();
	return InvokeInfo;
}
//---------------------------------------------------------------------------
/** This creates a weak reference to a callable object. If Func is a bound method,
 *  this will return a bound method weakproxy Func.__self__ to Func.__func__.
 *  Otherwise a normal strong reference is stored to Func to make sure the object is
 *  not destroyed if there are no references, e.g. lambda function.
 *
 *  We use a weak reference to avoid a circular reference like this case:
 *  class FooClass:
 *    def OnShow(self, Sender): pass
 *    def __init__(self):
 *      self.Form = vcl.TForm(None, OnShow=self.OnShow)
 *  Foo = FooClass()
 *  del Foo
 *
 *  In this case Foo contains a reference to Foo.Form. But Foo.Form.OnShow contains
 *  a bound method with a reference Foo. This will prevent Foo from being destroyed.
 *
 *  We cannot use a weak reference to the bound method as the bound method will loose
 *  its strong reference as soon as the weak reference is created.
 *
 *  We cannot change the bound method to use a weak reference as the __self__ attribute
 *  is read only.
 *  \param Func: Callable object used as event.
 *  \return New reference to event object. NULL if a Python exception has been set.
 */
 PyObject *GlobalObj;
PyObject* CreateWeakEvent(PyObject* Func)
{
  if(!PyCallable_Check(Func))
    return SetErrorString(PyExc_TypeError, "Event must be callable.");

  //Foo.OnShow.__self__ is a reference to Foo and Foo.OnShow.__func__ is a reference to FooClass.OnShow
  if(!PyObject_HasAttrString(Func, "__self__") || !PyObject_HasAttrString(Func, "__func__"))
  {
    Py_INCREF(Func);
    return Func;
  }

  TPyObjectPtr Obj(PyObject_GetAttrString(Func, "__self__"), false);
  TPyObjectPtr Fn(PyObject_GetAttrString(Func, "__func__"), false);
  if(!Obj || !Fn)
    return NULL; //Should never happen. We already checked that the attributes exist
GlobalObj=Obj.get();
  TPyObjectPtr Proxy(PyWeakref_NewProxy(Obj.get(), NULL), false);
  if(!Proxy)
    return NULL;
  //Instead of using PyMethod_New(Fn, Proxy), which doesn't exist in the limited API,
  //we create a new method object using the type passing (Fn, Proxy) to the constructor.
  return PyObject_Call(reinterpret_cast<PyObject*>(Func->ob_type), PyTuple_Pack(2, Fn.get(), Proxy.get()), NULL);
}
//---------------------------------------------------------------------------
/** Returns a Python string that represent the VclObject.
 */
static PyObject *VclObject_Repr(TVclObject* self)
{
	try
	{
		String Str;
		TComponent *Component = dynamic_cast<TComponent*>(self->Instance);
		if(self->Instance == NULL)
			Str = "<Reference to released object>";
		else if(Component != NULL)
			Str = "<object '" + Component->Name + "' of type '" + Component->ClassName() + "'>";
		else
			Str = "<object of type '" + self->Instance->ClassName() + "'>";
		return ToPyObject(Str);
	}
	catch(...)
	{
		return PyVclHandleException();
	}
}
//---------------------------------------------------------------------------
/** Returns a list of all properties, methods and public fields of the object.
 */
static PyObject* VclObject_Dir(TVclObject *self, PyObject *arg)
{
	PyObject *List = PyList_New(0);
	if(self->Instance != NULL)
	{
		TRttiType *Type = Context.GetType(self->Instance->ClassType());
		DynamicArray<TRttiProperty*> Properties = Type->GetProperties();
		DynamicArray<TRttiMethod*> Methods = Type->GetMethods();
		DynamicArray<TRttiField*> Fields = Type->GetFields();
		int PropertyCount = Properties.Length;
		int MethodCount = Methods.Length;
		int FieldCount = Fields.Length;
		PyObject *Value;
		for(int I = 0; I < PropertyCount; I++)
		{
			Value = ToPyObject(Properties[I]->Name);
			if(!PySequence_Contains(List, Value))
				PyList_Append(List, Value);
			Py_DECREF(Value);
		}
		for(int I = 0; I < MethodCount; I++)
		{
			Value = ToPyObject(Methods[I]->Name);
			if(!PySequence_Contains(List, Value))
				PyList_Append(List, Value);
			Py_DECREF(Value);
		}
		for(int I = 0; I < FieldCount; I++)
			if(Fields[I]->Visibility == mvPublic)
			{
				Value = ToPyObject(Fields[I]->Name);
				PyList_Append(List, Value);
				Py_DECREF(Value);
			}
	}
	return List;
}
//---------------------------------------------------------------------------
/** Destructor for the VclObject. If the instance is owned, first remove all objects
 *  that have the instance as parent, and then free the owned instance.
 *  If not owned, unregister from the delete handler.
 */
static void VclObject_Dealloc(TVclObject* self)
{
	if(self->Owned)
	{
#if !FIREMONKEY
		if(TWinControl *Control = dynamic_cast<TWinControl*>(self->Instance))
			while(Control->ControlCount)
				Control->Controls[Control->ControlCount-1]->Parent = NULL;
#endif
		delete self->Instance;
	}
	else
    if(TObjectDeleteHandler *DeleteHandler = FindDeleteHandler(self->Instance, false))
	  	DeleteHandler->SetVclObject(NULL);

  //It is okay to call PyObject_Del() directly as the object was created with PyObject_New() and cannot be subclassed.
  PyObject_Del(self);
}
//---------------------------------------------------------------------------
/** Retrieve a method, property or field. If a property or field is retrieved,
 *  we try to convert it to a built-in type, else a new VclObject is created to
 *  refference it without owning it.
 *  If a method is retrieved, a TVclMethod object is returned, which may be used
 *  to call the method.
 */
static PyObject* VclObject_GetAttro(TVclObject *self, PyObject *attr_name)
{
	try
	{
		if(self->Instance == NULL)
			throw EPyVclError("Referenced object has been deleted.");

		TObject *Object = self->Instance;
		String Name = FromPyObject<String>(attr_name);
		TRttiType *Type = Context.GetType(Object->ClassType());
		if(Type == NULL)
			throw EPyVclError("Type not found.");
		TRttiProperty *Property = Type->GetProperty(Name);
		if(Property == NULL)
		{
			DynamicArray<TRttiMethod*> Methods = Type->GetMethods(Name);
			if(Methods.Length != 0 && !Methods[0]->IsDestructor)
				return VclMethod_Create(self, self->Instance, Methods);

			TRttiField *Field = Type->GetField(Name);
			if(Field == NULL || Field->Visibility != mvPublic)
			{
				PyObject *ArrayProperty = VclIndexedProperty_Create(Object, Name);
				if(ArrayProperty != NULL)
					return ArrayProperty;
				PyObject *Result = PyObject_GenericGetAttr(reinterpret_cast<PyObject*>(self), attr_name);
				if(Result == NULL)
					SetErrorString(PyExc_AttributeError, "'" + Type->Name + "' object has no attribute '" + Name + "'");
				return Result;
			}
			TValue Result = Field->GetValue(Object);
			return ToPyObject(Result);
		}
		TValue Result = Property->GetValue(Object);
		return ToPyObject(Result);
	}
	catch(...)
	{
		return PyVclHandleException();
	}
}
//---------------------------------------------------------------------------
/** Set the value of a public property or field in a VclObject. If an event is
 *  set, we use a generic event, which will convert the parameters and call the
 *  Python event. If the Python event is a bound method, we store a weak reference
 *  to the instance of the bound method to avoid circular references.
 */
static int VclObject_SetAttro(TVclObject *self, PyObject *attr_name, PyObject *v)
{
	try
	{
		if(self->Instance == NULL)
			throw EPyVclError("Referenced object has been deleted.");

		String Name = FromPyObject<String>(attr_name);
		TRttiType *Type = Context.GetType(self->Instance->ClassType());
		TRttiProperty *Property = Type->GetProperty(Name);
		if(Property == NULL)
		{
			int Result = PyObject_GenericSetAttr(reinterpret_cast<PyObject*>(self), attr_name, v);
			if(Result == -1)
				SetErrorString(PyExc_AttributeError, "'" + Type->Name + "' object has no attribute '" + Name + "'");
			return Result;
		}

		TTypeInfo *TypeInfo = Property->PropertyType->Handle;
		TValue Value;
		if(TypeInfo->Kind == tkMethod)
		{
      //Free exisitng event if it is a PyObject
      Value = Property->GetValue(self->Instance);
      TMethod Method;
			Value.ExtractRawDataNoCopy(&Method);
			TObject *Object = static_cast<TObject*>(Method.Data);
			if(TMethodImplementation *Impl = dynamic_cast<TMethodImplementation*>(Object))
			{
				if(TObjectDeleteHandler *DeleteHandler = FindDeleteHandler(self->Instance, false))
					DeleteHandler->Unregister(Impl);
			}

			if(v == Py_None)
				Value = TValue::From<TObject*>(NULL);
			else
			{
				TMethodImplementation::TInvokeInfo *InvokeInfo = CreateInvokeInfo(TypeInfo);
        PyObject *Func = CreateWeakEvent(v);
        if(Func == NULL)
          return -1;
				TMethodImplementation *Implementation = new TMethodImplementation(Func, InvokeInfo, PythonCallback);
				TMethod Method = {Implementation->CodeAddress, Implementation}; //Pass InvokeInfo in this, which can be used as another data pointer
				TValue::Make(&Method, TypeInfo, Value);
				if(TObjectDeleteHandler *DeleteHandler = FindDeleteHandler(self->Instance, true))
					DeleteHandler->Register(Implementation);
			}
		}
		else
			Value = ToValue(v, TypeInfo);
		Property->SetValue(self->Instance, Value);
		return 0;
	}
	catch(...)
	{
		PyVclHandleException();
		return -1;
	}
}
//---------------------------------------------------------------------------
/** Compares two VCL proxy objects. Unordered compare (<, <=, >, >=) is not
 *  supported. Two proxy objects are equal if they refer to the same VCL object.
 *  \param self: The VCL proxy object.
 *  \param other: VCL proxy object to compare with.
 *  \return True or False depending on the compare operator.
 */
static PyObject* VclObject_RichCompare(TVclObject *self, PyObject *other, int op)
{
  if(op == Py_EQ)
  {
    if(!VclObject_Check(other))
      Py_RETURN_FALSE;
    if(self->Instance == reinterpret_cast<TVclObject*>(other)->Instance)
      Py_RETURN_TRUE;
    Py_RETURN_FALSE;
  }

  if(op == Py_NE)
  {
    if(!VclObject_Check(other))
      Py_RETURN_TRUE;
    if(self->Instance == reinterpret_cast<TVclObject*>(other)->Instance)
      Py_RETURN_FALSE;
    Py_RETURN_TRUE;
  }

  return PyErr_Format(PyExc_TypeError, "Unorderable types: VclObject() < %s()", AnsiString(GetTypeName(other)).c_str());
}
//---------------------------------------------------------------------------
/** Convert the object to True or False.
 *  \param self: The VCL proxy object.
 *  \return True if it is bound to an instance.
 */
static int VclObject_Bool(TVclObject *self)
{
  return self->Instance != NULL;
}
//---------------------------------------------------------------------------
/** Helper function for VclObject_Subscript() and VclObject_SetSubscript().
 *  \param self: VCL object to get default indexed property for.
 *  \return The default indexed property for the object if one exists.
 *  \throw EPyVclError on any error.
 */
#if __BCPLUSPLUS__ >= 0x640
static TRttiIndexedProperty* GetDefaultIndexedProeprty(TVclObject *self)
{
  if(self->Instance == NULL)
    throw EPyVclError("Referenced object has been deleted.");
  TRttiType *Type = Context.GetType(self->Instance->ClassType());
  System::DynamicArray<TRttiIndexedProperty*>  Properties = Type->GetIndexedProperties();
  for(int I = 0; I < Properties.Length; I++)
    if(Properties[I]->IsDefault)
      return Properties[I];
  throw EPyVclError("Instance of class '" + String(Type->Name) + "' has no default indexed property.");
}
#endif
//---------------------------------------------------------------------------
/** Get an element of the default indexed property if it exists.
 *  \param self: The VCL proxy object.
 *  \param key: The subscription index. May be a tuple.
 *  \return New reference to the retrieved object.
 */

 PyObject* Test(TRttiIndexedProperty *Property, TVclObject *self, std::vector<TValue> &Parameters)
 {
  try
  {
    return ToPyObject(Property->GetValue(self->Instance, &Parameters[0], Parameters.size()-1));
  }
  catch(Exception &E)
  {
		return PyVclHandleException();
  }
 }
 static PyObject* VclObject_Subscript(TVclObject *self, PyObject *key)
{
#if __BCPLUSPLUS__ >= 0x640
	try
	{
    TRttiIndexedProperty *Property = GetDefaultIndexedProeprty(self);
    if(!Property->IsReadable)
      throw EPyVclError("Default indexed property is not readable.");
    DynamicArray<TRttiParameter*> ParameterTypes = Property->ReadMethod->GetParameters();
    std::vector<TValue> Parameters;
    if(PyTuple_Check(key))
      TupleToValues(key, Parameters, ParameterTypes);
    else
      Parameters.push_back(ToValue(key, ParameterTypes[0]->ParamType->Handle));
    return ToPyObject(Property->GetValue(self->Instance, &Parameters[0], Parameters.size()-1));
//    return Test(Property, self, Parameters);
	}
	catch(...)
	{
		return PyVclHandleException();
	}
#else
  return SetErrorString(PyExc_NotImplementedError, "Not supported by this compiler");
#endif
}
//---------------------------------------------------------------------------
/** Set an element of the default indexed property if it exists.
 *  \param self: The VCL proxy object.
 *  \param key: The subscription index. May be a tuple.
 *  \param v: The value to set. This is checked for corret type.
 *  \return 0 on success and -1 on failure.
 */
static int VclObject_SetSubscript(TVclObject *self, PyObject *key, PyObject *v)
{
#if __BCPLUSPLUS__ >= 0x640
	try
	{
    TRttiIndexedProperty *Property = GetDefaultIndexedProeprty(self);
    if(!Property->IsWritable)
      throw EPyVclError("Property is not writable.");
    DynamicArray<TRttiParameter*> ParameterTypes = Property->WriteMethod->GetParameters();
    std::vector<TValue> Parameters;
    if(PyTuple_Check(key))
      TupleToValues(key, Parameters, ParameterTypes);
    else
      Parameters.push_back(ToValue(key, ParameterTypes[0]->ParamType->Handle));
    Property->SetValue(
      self->Instance,
      &Parameters[0], Parameters.size()-1,
      ToValue(v, Property->PropertyType->Handle));
    return 0;
	}
	catch(...)
	{
		PyVclHandleException();
		return -1;
	}
#else
  SetErrorString(PyExc_NotImplementedError, "Not supported by this compiler");
  return -1;
#endif
}
//---------------------------------------------------------------------------
static PyMemberDef VclObject_Members[] =
{
	{(char*)"_owned", T_BOOL, offsetof(TVclObject, Owned), 0, (char*)"Indicates if the " PROJECT_NAME " object is freed when the proxy is destroyed"},
	{NULL, 0, 0, 0, NULL}
};
//---------------------------------------------------------------------------
static PyMethodDef VclObject_Methods[] =
{
	{"__dir__", (PyCFunction)VclObject_Dir, METH_NOARGS, ""},
	{NULL, NULL, 0, NULL}
};
//---------------------------------------------------------------------------
/** Proxy object for a VCL object. If _owned is set to True, the VCL object is
 *  owned by the proxy and destroyed when the proxy is destroyed.
 *  If possible the VCL object will own a delete handler, which will clear the
 *  reference in the proxy if the VCL object is destroyed before the proxy.
 */
static PyType_Slot VclObject_Slots[] =
{
  {Py_tp_doc,	(void*) PROJECT_NAME " object"},
  {Py_tp_dealloc, VclObject_Dealloc},
  {Py_tp_repr, VclObject_Repr},
  {Py_tp_getattro, VclObject_GetAttro},
  {Py_tp_setattro, VclObject_SetAttro},
  {Py_tp_richcompare, VclObject_RichCompare},
  {Py_nb_bool, VclObject_Bool},
  {Py_mp_subscript, VclObject_Subscript},
  {Py_mp_ass_subscript, VclObject_SetSubscript},
	{Py_tp_methods, VclObject_Methods},
	{Py_tp_members, VclObject_Members},
  {0, NULL}
};

static PyType_Spec VclObject_Spec =
{
  GUI_TYPE "Object",   //name
  sizeof(TVclObject),  //basicsize
  0,                    //itemsize
  Py_TPFLAGS_DEFAULT,   //flags
  VclObject_Slots,     //slots
};
//---------------------------------------------------------------------------
/** Initialize the VclObject_Type type object.
 *  \return true on success
 */
bool VclObject_InitType()
{
  VclObject_Type = reinterpret_cast<PyTypeObject*>(PyType_FromSpec(&VclObject_Spec));
  return VclObject_Type != NULL;
}
//---------------------------------------------------------------------------
/** Create new VclObject.
 *  \param Instance: VCL instance, which the VclObject will be a proxy for.
 *  \param Owned: If True, Instance will be owned by the proxy and destroyed
 *                when the proxy is destroyed.
 *  \return New reference to VclObject.
 */
PyObject* VclObject_Create(TObject *Instance, bool Owned)
{
  TObjectDeleteHandler *DeleteHandler = FindDeleteHandler(Instance, true);
	TVclObject *VclObject = NULL;
  if(DeleteHandler)
    VclObject = DeleteHandler->GetVclObject();
  if(VclObject != NULL)
  {
    if(Owned)
      VclObject->Owned = true;
    Py_INCREF(VclObject);
  }
  else
  {
    VclObject = PyObject_New(TVclObject, VclObject_Type);
	  VclObject->Instance = Instance;
	  VclObject->Owned = Owned;
    if(DeleteHandler)
      DeleteHandler->SetVclObject(VclObject);
  }
  return reinterpret_cast<PyObject*>(VclObject);
}
//---------------------------------------------------------------------------
/** Return true if O is a VclObject.
 */
bool VclObject_Check(PyObject *O)
{
	return O->ob_type == VclObject_Type;
}
//---------------------------------------------------------------------------
/** Return the VCL object, which the given VclObject is a proxy for.
 *  \param O: Pointer to a VclObject instance.
 *  \return VCL object refered to by O. This may be NULL if the object has been destroyed
 *  \throw EPyVclError if O is not a VclObject.
 */
TObject* VclObject_AsObject(PyObject *O)
{
	if(O->ob_type != VclObject_Type)
		throw EPyVclError("Object is not a " GUI_TYPE "Object type");
	return reinterpret_cast<TVclObject*>(O)->Instance;
}
//---------------------------------------------------------------------------
} //namespace Python
