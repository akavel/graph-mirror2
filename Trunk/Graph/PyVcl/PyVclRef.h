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
  bool VclRef_Check(PyObject *O);
  const TValue& VclRef_AsValue(PyObject *O);
  void VclRef_Invalidate(PyObject *O);
}
#endif
