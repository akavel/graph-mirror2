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
#include <delayimp.h>
//---------------------------------------------------------------------------
class EDllLoadError : public Exception
{
public:
  EDllLoadError(const String &Str) : Exception(Str) {}
};
//---------------------------------------------------------------------------
//Called when delay loading a DLL failes
FARPROC WINAPI DllLoadFailure(dliNotification dliNotify, DelayLoadInfo *pdli)
{
  if(dliNotify == dliFailGetProcAddress)
    throw EDllLoadError("Failed to get address of: " + pdli->dlp.fImportByName ? String(pdli->dlp.szProcName) : IntToStr((int)pdli->dlp.dwOrdinal));
  throw EDllLoadError("Failed to load " + String(pdli->szDll));
}
//---------------------------------------------------------------------------
static struct TInitErrorHandling
{
  TInitErrorHandling()
  {
  	__pfnDliFailureHook = DllLoadFailure;
  }
} InitErrorHandling;
//---------------------------------------------------------------------------
/** Called when BOOST_ASSERT fails.
 */
#ifdef BOOST_ENABLE_ASSERT_HANDLER
void boost::assertion_failed(char const * expr, char const * function, char const * file, long line)
{
  String Message = "Assertion failed: " + String(expr) + ", File: " + file + ", Line " + line;
  throw EAssertionFailed(Message);
}
#endif
//---------------------------------------------------------------------------
void _RTLENTRY _EXPFUNC std::_assert(char * cond, char * file, int line)
{
#ifdef BOOST_ENABLE_ASSERT_HANDLER
	boost::assertion_failed(cond, NULL, file, line);
#endif
}
//---------------------------------------------------------------------------

