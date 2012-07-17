unit Thumbnails_TLB;

// ************************************************************************ //
// WARNING
// -------
// The types declared in this file were generated from data read from a
// Type Library. If this type library is explicitly or indirectly (via
// another type library referring to this type library) re-imported, or the
// 'Refresh' command of the Type Library Editor activated while editing the
// Type Library, the contents of this file will be regenerated and all
// manual modifications will be lost.
// ************************************************************************ //

// $Rev: 45604 $
// File generated on 16-07-2012 11:12:06 from Type Library described below.

// ************************************************************************  //
// Type Lib: B:\Graph\Thumbnails\Thumbnails (1)
// LIBID: {C89F00EF-E696-4922-8198-4BD20820627D}
// LCID: 0
// Helpfile:
// HelpString:
// DepndLst:
//   (1) v2.0 stdole, (C:\Windows\SysWOW64\stdole2.tlb)
// SYS_KIND: SYS_WIN32
// ************************************************************************ //
{$TYPEDADDRESS OFF} // Unit must be compiled without type-checked pointers.
{$WARN SYMBOL_PLATFORM OFF}
{$WRITEABLECONST ON}
{$VARPROPSETTER ON}
{$ALIGN 4}

interface

uses Winapi.Windows, System.Classes, System.Variants, System.Win.StdVCL, Vcl.Graphics, Vcl.OleServer, Winapi.ActiveX;


// *********************************************************************//
// GUIDS declared in the TypeLibrary. Following prefixes are used:
//   Type Libraries     : LIBID_xxxx
//   CoClasses          : CLASS_xxxx
//   DISPInterfaces     : DIID_xxxx
//   Non-DISP interfaces: IID_xxxx
// *********************************************************************//
const
  // TypeLibrary Major and minor versions
  ThumbnailsMajorVersion = 1;
  ThumbnailsMinorVersion = 0;

  LIBID_Thumbnails: TGUID = '{C89F00EF-E696-4922-8198-4BD20820627D}';

  IID_IGraphThumbnails: TGUID = '{0637F3F6-5779-480E-81C0-91C8E8BBBC89}';
  CLASS_GraphThumbnails: TGUID = '{6F4F06C8-2E0C-4342-94CC-DC772BC53155}';
type

// *********************************************************************//
// Forward declaration of types defined in TypeLibrary
// *********************************************************************//
  IGraphThumbnails = interface;
  IGraphThumbnailsDisp = dispinterface;

// *********************************************************************//
// Declaration of CoClasses defined in Type Library
// (NOTE: Here we map each CoClass to its Default Interface)
// *********************************************************************//


// *********************************************************************//
// Interface: IGraphThumbnails
// Flags:     (4416) Dual OleAutomation Dispatchable
// GUID:      {0637F3F6-5779-480E-81C0-91C8E8BBBC89}
// *********************************************************************//
  IGraphThumbnails = interface(IDispatch)
    ['{0637F3F6-5779-480E-81C0-91C8E8BBBC89}']
  end;

// *********************************************************************//
// DispIntf:  IGraphThumbnailsDisp
// Flags:     (4416) Dual OleAutomation Dispatchable
// GUID:      {0637F3F6-5779-480E-81C0-91C8E8BBBC89}
// *********************************************************************//
  IGraphThumbnailsDisp = dispinterface
    ['{0637F3F6-5779-480E-81C0-91C8E8BBBC89}']
  end;

// *********************************************************************//
// The Class CoGraphThumbnails provides a Create and CreateRemote method to
// create instances of the default interface IGraphThumbnails exposed by
// the CoClass GraphThumbnails. The functions are intended to be used by
// clients wishing to automate the CoClass objects exposed by the
// server of this typelibrary.
// *********************************************************************//
  CoGraphThumbnails = class
    class function Create: IGraphThumbnails;
    class function CreateRemote(const MachineName: string): IGraphThumbnails;
  end;

implementation

uses System.Win.ComObj;

class function CoGraphThumbnails.Create: IGraphThumbnails;
begin
  Result := CreateComObject(CLASS_GraphThumbnails) as IGraphThumbnails;
end;

class function CoGraphThumbnails.CreateRemote(const MachineName: string): IGraphThumbnails;
begin
  Result := CreateRemoteComObject(MachineName, CLASS_GraphThumbnails) as IGraphThumbnails;
end;


end.

