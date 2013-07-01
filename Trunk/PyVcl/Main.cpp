//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#undef _DEBUG
#include "Python.hpp"
#include "PyVcl.h"
#include "PythonBind.h"
#include <float.h>
//---------------------------------------------------------------------------
#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
  return 1;
}
//---------------------------------------------------------------------------
PyMODINIT_FUNC __stdcall PyInit_PyVcl(void)
{
  _control87(DEFAULT_FPU_CONTROL, FPU_MASK);
  Application->Initialize();
  PyObject *Result = Python::InitPyVcl();
	_control87(PYTHON_FPU_CONTROL, FPU_MASK); //Set the FPU Control Word to what Python expects
  return Result;
}
//---------------------------------------------------------------------------
/* Replacement for TWinControl::DestroyWindowHandle() in Controls.pas.
 * This does not throw an exception if the window handle cannot be destroyed.
 * This prevents a crash when the console is closed by pressing Ctrl+C or using the
 * button with a cross in top right corner.
 */
void __fastcall TWinControl::DestroyWindowHandle()
{
  if(Touch->GestureEngine != NULL)
    Touch->GestureEngine->Active = false;
  ControlState = ControlState << csDestroyingHandle;
  DestroyWindow(WindowHandle);
  ControlState = ControlState >> csDestroyingHandle;
  WindowHandle = 0;
}
//---------------------------------------------------------------------------

