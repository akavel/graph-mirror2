/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2010 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Graph.h"
#pragma hdrstop
#include "ClassList.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
namespace Python
{
struct TClassList
{
	const wchar_t *ClassName;
	TMetaClass *Class;
};

TClassList ClassList[] =
{
	{L"TForm", __classid(TForm)},
	{L"TLabel", __classid(TLabel)},
	{L"TEdit", __classid(TEdit)},
	{L"TButton", __classid(TButton)},
	{L"TAction", __classid(TAction)},
};

TMetaClass* LookUpClass(const String &Name)
{
	unsigned Count = sizeof(ClassList)/sizeof(ClassList[0]);
	for(unsigned I = 0; I < Count; I++)
		if(Name == ClassList[I].ClassName)
			return ClassList[I].Class;
	throw Exception("Class " + Name + " not found.");
}
} //namespace Python
