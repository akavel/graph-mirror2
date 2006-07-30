//---------------------------------------------------------------------------
#include <basepch.h>
#pragma hdrstop
//---------------------------------------------------------------------------
//   Package source.
//---------------------------------------------------------------------------

USEFORM("TIPrintDialog\PrintForm.cpp", PrintFrm);
USEFORM("TSymbolDialog\SymbolForm.cpp", SymbolFrm);
//---------------------------------------------------------------------------
#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void*)
{
        return 1;
}
//---------------------------------------------------------------------------
 