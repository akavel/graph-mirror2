//---------------------------------------------------------------------------
#include "Platform.h"
#pragma hdrstop
#include "Python.hpp"
#include "PyVcl.h"
#include "PythonBind.h"
#include <Math.hpp>
//---------------------------------------------------------------------------
#pragma argsused
/*int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
  return 1;
} */
//---------------------------------------------------------------------------
#pragma argsused
extern "C" int _libmain(unsigned long reason)
{
	return 1;
}
//---------------------------------------------------------------------------
PyMODINIT_FUNC __stdcall INIT_FUNC(void)
{
  //We need to mask the InvalidOp SSE exception to prevent math.sqrt(-1) in Python from
  //raising a C++ exception. It only seems to be a problem with 64 bit Python.
  SetExceptionMask(GetExceptionMask() << exInvalidOp);
  Application->Initialize();
  return Python::InitPyVcl();
}
//---------------------------------------------------------------------------
#ifndef FIREMONKEY
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
#endif
//---------------------------------------------------------------------------
#ifdef FIREMONKEY
/** Replacement for Fmx::Forms::ApplicationState in FMX.Forms.pas.
 *  The cursor is not shown in a TEdit or TMemo before Application.Run() is called.
 *  To show the cursor ApplicationState() must return Running, which it does when Application->FRunning is true.
 *  This workarounds the problem by forcing ApplicationState() to always return true.
 *  NOTICE: You might get an Acdess Violation in TControl.RefreshInheritedCursorForChildren() when
 *  closing the application if all forms have not been deleted.
 */
extern DELPHI_PACKAGE TApplicationState __fastcall Fmx::Forms::ApplicationState(void)
{
   return TApplicationState::Running;
}
#endif
//---------------------------------------------------------------------------

