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
#include "Python.h"
#include "PythonBind.h"
#include "PyVclObject.h"
#include "PyVcl.h"

namespace Python
{
//---------------------------------------------------------------------------
struct TVclArrayProperty
{
	PyObject_HEAD
	TObject *Instance;
	int PropertyId;
};
//---------------------------------------------------------------------------
static PyObject *VclArrayProperty_Repr(TVclArrayProperty* self)
{
	const wchar_t *Names[] = {L"Strings", L"Forms", L"Components", L"Controls"};
	String Str = "<Array property '";
	Str += Names[self->PropertyId];
	TComponent *Component = dynamic_cast<TComponent*>(self->Instance);
	if(Component != NULL)
		Str += "' of <object '" + Component->Name + "' of type '" + Component->ClassName() + "'>>";
	else
		Str += "' of <object of type '" + self->Instance->ClassName() + "'>>";
	return PyUnicode_FromUnicode(Str.c_str(), Str.Length());
}
//---------------------------------------------------------------------------
Py_ssize_t VclArrayProperty_Length(TVclArrayProperty *self)
{
	switch(self->PropertyId)
	{
		case 0:
			return static_cast<TCollection*>(self->Instance)->Count;
		case 1:
			return static_cast<TStrings*>(self->Instance)->Count;
		case 2:
			return static_cast<TScreen*>(self->Instance)->FormCount;
		case 3:
			return static_cast<TComponent*>(self->Instance)->ComponentCount;
		case 4:
			return static_cast<TWinControl*>(self->Instance)->ControlCount;
	}
	return 0;
}
//---------------------------------------------------------------------------
PyObject* VclArrayProperty_Item(TVclArrayProperty *self, Py_ssize_t i)
{
	switch(self->PropertyId)
	{
		case 0:
			return ToPyObject(static_cast<TCollection*>(self->Instance)->Items[i]);
		case 1:
			return ToPyObject(static_cast<TStrings*>(self->Instance)->Strings[i]);
		case 2:
			return ToPyObject(static_cast<TScreen*>(self->Instance)->Forms[i]);
		case 3:
			return ToPyObject(static_cast<TComponent*>(self->Instance)->Components[i]);
		case 4:
			return ToPyObject(static_cast<TWinControl*>(self->Instance)->Controls[i]);
	}
	return NULL;
}
//---------------------------------------------------------------------------
int VclArrayProperty_SetItem(TVclArrayProperty *self, Py_ssize_t i, PyObject *v)
{
	try
	{
		switch(self->PropertyId)
		{
			case 0:
			{
				TObject *Object = VclObject_AsObject(v);
				if(TCollectionItem *Item = dynamic_cast<TCollectionItem*>(Object))
					static_cast<TCollection*>(self->Instance)->Items[i] = Item;
				else
					throw EPyVclError("Only TCollectionItem can be assigned to a TCollection");
				break;
			}
			case 1:
				if(!PyUnicode_Check(v))
					throw EPyVclError("Only strings can be assigned to a TStrings");
				static_cast<TStrings*>(self->Instance)->Strings[i] = PyUnicode_AsUnicode(v);
				break;

			default:
				SetErrorString(PyVclException, "Property is read only");
				return -1;
		}
		return  0;
	}
	catch(Exception &E)
	{
		SetErrorString(PyVclException, E.Message);
		return -1;
	}
}
//---------------------------------------------------------------------------
PySequenceMethods VclArrayPropertySequence =
{
	(lenfunc)VclArrayProperty_Length, /* sq_length */
	0, /* sq_concat */
	0, /* sq_repeat */
	(ssizeargfunc)VclArrayProperty_Item, /* sq_item */
	0, /* was_sq_slice */
	(ssizeobjargproc)VclArrayProperty_SetItem, /* sq_ass_item */
	0, /* was_sq_ass_slice */
	0, /* sq_contains */
	0, /* sq_inplace_concat */
	0, /* sq_inplace_repeat */
};

PyTypeObject VclArrayPropertyType =
{
	PyObject_HEAD_INIT(NULL)
	"vcl.VclArrayProperty",  	 /* tp_name */
	sizeof(TVclArrayProperty), /* tp_basicsize */
	0,                         /* tp_itemsize */
	0, 												 /* tp_dealloc */
	0,                         /* tp_print */
	0,                         /* tp_getattr */
	0,                         /* tp_setattr */
	0,                         /* tp_compare */
	(reprfunc)VclArrayProperty_Repr,  /*tp_repr */
	0, 												 /* tp_as_number */
	&VclArrayPropertySequence, /* tp_as_sequence */
	0,                         /* tp_as_mapping */
	0,                         /* tp_hash */
	0, 												 /* tp_call */
	0,                         /* tp_str */
	0, 												 /* tp_getattro */
	0, 												 /* tp_setattro */
	0,                         /* tp_as_buffer */
	Py_TPFLAGS_DEFAULT, 			 /* tp_flags */
	"VCL array property",			 /* tp_doc */
	0,		                     /* tp_traverse */
	0,		                     /* tp_clear */
	0,		                     /* tp_richcompare */
	0,		                     /* tp_weaklistoffset */
	0,		                     /* tp_iter */
	0,		                     /* tp_iternext */
	0, 								         /* tp_methods */
	0,                         /* tp_members */
	0,       									 /* tp_getset */
	0,                         /* tp_base */
	0,                         /* tp_dict */
	0,                         /* tp_descr_get */
	0,                         /* tp_descr_set */
	0,                         /* tp_dictoffset */
	0,                         /* tp_init */
	0,                         /* tp_alloc */
	0,						             /* tp_new */
};
//---------------------------------------------------------------------------
PyObject* VclArrayProperty_Create(TObject *Instance, const String &Name)
{
	int PropertyId = -1;
	if(Name == "Items" && dynamic_cast<TCollection*>(Instance))
		PropertyId = 0;
	else if(Name == "Strings" && dynamic_cast<TStrings*>(Instance))
		PropertyId = 1;
	else if(Name == "Forms" && dynamic_cast<TScreen*>(Instance))
		PropertyId = 2;
	else if(Name == "Components" && dynamic_cast<TComponent*>(Instance))
		PropertyId = 3;
	else if(Name == "Controls" && dynamic_cast<TWinControl*>(Instance))
		PropertyId =4;
	else
		return NULL;
	TVclArrayProperty *VclProperty = PyObject_New(TVclArrayProperty, &VclArrayPropertyType);
	VclProperty->Instance = Instance;
	VclProperty->PropertyId = PropertyId;
	return reinterpret_cast<PyObject*>(VclProperty);
}
//---------------------------------------------------------------------------
} //namespace Python
