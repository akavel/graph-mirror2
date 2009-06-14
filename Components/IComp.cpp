//---------------------------------------------------------------------------

#include <basepch.h>
#pragma hdrstop
USEFORM("TIPrintDialog\PrintForm.cpp", PrintFrm);
USEFORM("TSymbolDialog\SymbolForm.cpp", SymbolFrm);
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Package source.
//---------------------------------------------------------------------------


#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
	return 1;
}
//---------------------------------------------------------------------------
