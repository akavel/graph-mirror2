/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef PyVclH
#define PyVclH
#include <Rtti.hpp>
//---------------------------------------------------------------------------
struct _object;
namespace Python
{
	class EPyVclError : public Exception
	{
	public:
		EPyVclError(const String &Str) : Exception(Str) {}
  };

	extern TRttiContext Context;
	extern _object *PyPropertyException;
  extern _object *PyVclException;
	_object* InitPyVcl();
	TValue ToValue(_object *O, TTypeInfo *TypeInfo);
	void TupleToValues(_object *O, std::vector<TValue> &Values, const DynamicArray<TRttiParameter*> &Parameters);
}
#endif
