// CodeGear C++Builder
// Copyright (c) 1995, 2013 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'FindClass.pas' rev: 25.00 (Windows)

#ifndef FindclassHPP
#define FindclassHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.TypInfo.hpp>	// Pascal unit
#include <System.Classes.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Findclass
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
extern DELPHI_PACKAGE System::Typinfo::PTypeInfo __fastcall LookUpClass(System::UnicodeString Name);
extern DELPHI_PACKAGE System::Classes::TStringList* __fastcall GetTypeList(void);
extern DELPHI_PACKAGE void __fastcall RegisterType(System::Typinfo::PTypeInfo TypeInfo);
}	/* namespace Findclass */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_FINDCLASS)
using namespace Findclass;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// FindclassHPP
