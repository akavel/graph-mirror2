//---------------------------------------------------------------------------
#ifndef PyVclRefH
#define PyVclRefH
#include "Python.hpp"
#include <Rtti.hpp>
//---------------------------------------------------------------------------
namespace Python
{
	extern PyTypeObject VclRef_Type;
	PyObject* VclRef_Create(TValue *Value);
}
#endif
