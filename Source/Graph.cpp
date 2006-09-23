/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * Func32 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Func32; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
#include <atl\atlmod.h>
#include <atl\atlmod.h>
#include <atl\atlmod.h>
#include "OleServerImpl.h"
#include "GraphServerImpl.h"
#include "StackTrace.h"

USEFORM("Unit1.cpp", Form1);
USEFORM("Unit2.cpp", Form2);
USEFORM("Unit3.cpp", Form3);
USEFORM("Unit4.cpp", Form4);
USEFORM("Unit5.cpp", Form5);
USEFORM("Unit6.cpp", Form6);
USEFORM("Unit7.cpp", Form7);
USEFORM("Unit8.cpp", Form8);
USEFORM("Unit9.cpp", Form9);
USEFORM("Unit10.cpp", Form10);
USEFORM("Unit11.cpp", Form11);
USEFORM("Unit12.cpp", Form12);
USEFORM("Unit13.cpp", Form13);
USEFORM("Unit14.cpp", Form14);
USEFORM("Unit15.cpp", Form15);
USEFORM("Unit16.cpp", Form16);
USEFORM("Unit17.cpp", Form17);
USEFORM("Unit18.cpp", Form18);
USEFORM("Unit19.cpp", Form19);
USEFORM("Unit20.cpp", Form20);
USEFORM("TStdFuncFrame.cpp", StdFuncFrame); /* TFrame: File Type */
USEFORM("TParFuncFrame.cpp", ParFuncFrame); /* TFrame: File Type */
USEFORM("TPolFuncFrame.cpp", PolFuncFrame); /* TFrame: File Type */
USEFORM("TTanFrame.cpp", TanFrame); /* TFrame: File Type */
USEFORM("TAreaFrame.cpp", AreaFrame); /* TFrame: File Type */
USEFORM("..\Components\TSymbolDialog\SymbolForm.cpp", SymbolFrm);
//---------------------------------------------------------------------------
TComModule _ProjectModule(0 /*InitATLServer*/);
TComModule &_Module = _ProjectModule;

// The ATL Object map holds an array of _ATL_OBJMAP_ENTRY structures that
// described the objects of your OLE server. The MAP is handed to your
// project's CComModule-derived _Module object via the Init method.
//
BEGIN_OBJECT_MAP(ObjectMap)
  OBJECT_ENTRY(CLSID_OleServer, TOleServerImpl)
//  OBJECT_ENTRY(CLSID_GraphServer, TGraphServerImpl)
END_OBJECT_MAP()
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
  try
  {
    Application->Initialize();
    Application->Title = "Graph";

    if(FindCmdLineSwitch("EMBEDDING"))
      Application->ShowMainForm = false;

    Application->CreateForm(__classid(TForm1), &Form1);
     Application->Run();
  }
  catch (Exception &exception)
  {
    LogUncaughtException(NULL, &exception);
  }
  catch(std::exception &E)
  {
    Application->MessageBox(("Uncaught exception with message:\n" + AnsiString(E.what())).c_str(), "Application error", MB_ICONSTOP);
  }
  catch (...)
  {
    try
    {
      throw Exception("Unknown exception escaped from WinMain");
    }
    catch (Exception &exception)
    {
      Application->ShowException(&exception);
    }
  }
  return 0;
}
//---------------------------------------------------------------------------
