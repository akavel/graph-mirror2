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
#include "Unit1.h"
#include <delayimp.h>
#include <assert.h>
#ifdef MADEXCEPT
#include <madexcept.hpp>
#include "ConfigRegistry.h"
#include "Common.h"
#include <Registry.hpp>
#endif
//---------------------------------------------------------------------------
class EDllLoadError : public Exception
{
public:
  EDllLoadError(const String &Str) : Exception(Str) {}
};
//---------------------------------------------------------------------------
//Called when delay loading a DLL failes
FARPROC WINAPI DllLoadFailure(unsigned dliNotify, DelayLoadInfo *pdli)
{
  if(dliNotify == 4/*dliFailGetProcAddress*/)
    throw EDllLoadError("Failed to get address of: " + pdli->dlp.fImportByName ? String(pdli->dlp.szProcName) : IntToStr((int)pdli->dlp.dwOrdinal));
  throw EDllLoadError("Failed to load " + String(pdli->szDll));
}
//---------------------------------------------------------------------------
static struct TInitErrorHandling
{
  TInitErrorHandling()
  {
	__pfnDliFailureHook2 = DllLoadFailure;
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
void _RTLENTRY _EXPFUNC std::_assert(const char * cond, const char * file, int line)
{
#ifdef BOOST_ENABLE_ASSERT_HANDLER
	boost::assertion_failed(cond, NULL, file, line);
#endif
}
//---------------------------------------------------------------------------
#ifdef MADEXCEPT
class TMadExceptHandler
{
public:
  TMadExceptHandler();
  ~TMadExceptHandler();
  void __fastcall ExceptHandler(_di_IMEException exceptIntf, bool &handled);
  void __fastcall OnException(TObject *Sender, Exception *E);
};
TMadExceptHandler MadExceptHandler;
//---------------------------------------------------------------------------
TMadExceptHandler::TMadExceptHandler()
{
  RegisterExceptionHandler(&MadExceptHandler.ExceptHandler, stDontSync);
  Application->OnException = OnException;
}
//---------------------------------------------------------------------------
TMadExceptHandler::~TMadExceptHandler()
{
  UnregisterExceptionHandler(MadExceptHandler.ExceptHandler);
}
//---------------------------------------------------------------------------
void __fastcall TMadExceptHandler::ExceptHandler(_di_IMEException exceptIntf, bool &handled)
{
  if(!Form1->Data.GetFileName().empty())
    exceptIntf->AdditionalAttachments->Add(ToUString(Form1->Data.GetFileName()));

  String ExportFileName = GetTempFileName("Graph", "reg");
  ExportRegistryKey(REGISTRY_KEY, HKEY_CURRENT_USER, ToWString(ExportFileName));
//  TRegistry *Registry=new TRegistry;
//  Registry->SaveKey(REGISTRY_KEY, ExportFileName);
  exceptIntf->AdditionalAttachments->Add(ExportFileName, "Graph.reg");
}
//---------------------------------------------------------------------------
void __fastcall TMadExceptHandler::OnException(TObject *Sender, Exception *E)
{
  //Workaround for bug in C++ Builder XE6, see http://forum.madshi.net/viewtopic.php?f=4&t=27888
  HandleException();
}
#endif //MADEXCEPT
//---------------------------------------------------------------------------

