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
	const char *ClassName;
	TMetaClass *Class;
};

TClassList ClassList[] =
{
	{"TForm", __classid(TForm)},
};

TMetaClass* FindClass(const std::string &Name)
{
	unsigned Count = sizeof(ClassList)/sizeof(ClassList[0]);
	for(int I = 0; I < Count; I++)
		if(ClassList[I].ClassName == Name)
			return ClassList[I].Class;
	return NULL;
}
} //namespace Python
