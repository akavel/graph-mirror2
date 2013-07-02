//---------------------------------------------------------------------------
#ifndef PyVclRefH
#define PyVclRefH
#include "Python.hpp"
#include <Rtti.hpp>
//---------------------------------------------------------------------------
namespace Python
{
	extern PyTypeObject VclRefType;
	PyObject* VclRef_Create(TValue *Value);
}
#endif
