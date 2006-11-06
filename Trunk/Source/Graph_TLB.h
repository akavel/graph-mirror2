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

// C++ TLBWRTR : $Revision:   1.151.1.0.1.27  $
// File generated on 06-11-2006 19:26:09 from Type Library described below.

// ************************************************************************  //
// Type Lib: D:\Projects\Graph\Source\Graph.tlb (1)
// LIBID: {6EA12C95-4B54-4F1B-BB3E-2D0266ECA69E}
// LCID: 0
// Helpfile: 
// HelpString: Graph 
// DepndLst: 
//   (1) v2.0 stdole, (C:\WINDOWS\system32\stdole2.tlb)
// ************************************************************************ //
#ifndef   Graph_TLBH
#define   Graph_TLBH

#pragma option push -b -w-inl

#include <utilcls.h>
#if !defined(__UTILCLS_H_VERSION) || (__UTILCLS_H_VERSION < 0x0600)
//
// The code generated by the TLIBIMP utility or the Import|TypeLibrary 
// and Import|ActiveX feature of C++Builder rely on specific versions of
// the header file UTILCLS.H found in the INCLUDE\VCL directory. If an 
// older version of the file is detected, you probably need an update/patch.
//
#error "This file requires a newer version of the header UTILCLS.H" \
       "You need to apply an update/patch to your copy of C++Builder"
#endif
#include <olectl.h>
#include <ocidl.h>
#if defined(USING_ATLVCL) || defined(USING_ATL)
#if !defined(__TLB_NO_EVENT_WRAPPERS)
#include <atl/atlmod.h>
#endif
#endif


// *********************************************************************//
// Forward reference of some VCL types (to avoid including STDVCL.HPP)    
// *********************************************************************//
namespace Stdvcl {class IStrings; class IStringsDisp;}
using namespace Stdvcl;
typedef TComInterface<IStrings> IStringsPtr;
typedef TComInterface<IStringsDisp> IStringsDispPtr;

namespace Graph_tlb
{

// *********************************************************************//
// HelpString: Graph 
// Version:    1.0
// *********************************************************************//


// *********************************************************************//
// GUIDS declared in the TypeLibrary. Following prefixes are used:        
//   Type Libraries     : LIBID_xxxx                                      
//   CoClasses          : CLSID_xxxx                                      
//   DISPInterfaces     : DIID_xxxx                                       
//   Non-DISP interfaces: IID_xxxx                                        
// *********************************************************************//
extern "C" const __declspec(selectany) GUID LIBID_Graph = {0x6EA12C95, 0x4B54, 0x4F1B,{ 0xBB, 0x3E, 0x2D,0x02, 0x66, 0xEC,0xA6, 0x9E} };
extern "C" const __declspec(selectany) GUID IID_IGraphServer = {0x4729DE98, 0xE9CB, 0x4DFC,{ 0x87, 0xCE, 0xC7,0xCC, 0xF9, 0xE3,0x02, 0xC3} };
extern "C" const __declspec(selectany) GUID CLSID_GraphServer = {0xB1C5A0CC, 0x9E80, 0x4C24,{ 0x85, 0x1D, 0xD8,0x22, 0x6E, 0x00,0x62, 0x09} };
extern "C" const __declspec(selectany) GUID GUID_TImageFileType = {0x8B9E62BD, 0xE066, 0x4A03,{ 0xAD, 0xFF, 0x3D,0xDB, 0x48, 0xBB,0x54, 0xB2} };
extern "C" const __declspec(selectany) GUID GUID_TAxisRecord = {0x55651771, 0x60FE, 0x4F58,{ 0x80, 0x26, 0x86,0x7E, 0x00, 0xCC,0xE3, 0x99} };
extern "C" const __declspec(selectany) GUID GUID_TAxesRecord = {0x4C8B053E, 0x1B17, 0x42DA,{ 0xBB, 0xB2, 0xB8,0xA0, 0x08, 0x7A,0x3E, 0x23} };
extern "C" const __declspec(selectany) GUID GUID_TFuncRecord = {0xACDCE646, 0x377F, 0x4ABB,{ 0xA8, 0xBC, 0xEF,0x13, 0x16, 0xDE,0xF1, 0x32} };

// *********************************************************************//
// Forward declaration of types defined in TypeLibrary                    
// *********************************************************************//
enum      TImageFileType;
struct    tagTAxisRecord;
typedef   tagTAxisRecord TAxisRecord;

struct    tagTAxesRecord;
typedef   tagTAxesRecord TAxesRecord;

struct    tagTFuncRecord;
typedef   tagTFuncRecord TFuncRecord;

interface DECLSPEC_UUID("{4729DE98-E9CB-4DFC-87CE-C7CCF9E302C3}") IGraphServer;
typedef TComInterface<IGraphServer, &IID_IGraphServer> IGraphServerPtr;


// *********************************************************************//
// Declaration of CoClasses defined in Type Library                       
// (NOTE: Here we map each CoClass to its Default Interface)              
//                                                                        
// The LIBID_OF_ macro(s) map a LIBID_OF_CoClassName to the GUID of this  
// TypeLibrary. It simplifies the updating of macros when CoClass name    
// change.                                                                
// *********************************************************************//
typedef IGraphServer GraphServer;
typedef IGraphServerPtr GraphServerPtr;

#define LIBID_OF_GraphServer (&LIBID_Graph)

// *********************************************************************//
// Declaration of Enumerations defined in Type Library                    
// *********************************************************************//
typedef enum TImageFileType
{
  iftEnhancedMeta = 1, 
  iftBitmap = 2, 
  iftPng = 3
} TImageFileType;


// *********************************************************************//
// Declaration of Structures and Unions defined in Type Library           
// *********************************************************************//
struct tagTAxisRecord
{
  double Min;
  double Max;
  double TickUnit;
  double GridUnit;
  VARIANT_BOOL LogScale;
  VARIANT_BOOL ShowNumbers;
  VARIANT_BOOL AutoTick;
  VARIANT_BOOL AutoGrid;
  VARIANT_BOOL ShowTicks;
  VARIANT_BOOL ShowGrid;
  VARIANT_BOOL ShowLabel;
  double Cross;
  BSTR Label;
};

struct tagTAxesRecord
{
  Graph_tlb::TAxisRecord xAxis;
  Graph_tlb::TAxisRecord yAxis;
  unsigned AxesStyle;
  unsigned Trigonometry;
  unsigned BackgroundColor;
  unsigned AxesColor;
  unsigned GridColor;
  BSTR Title;
};

struct tagTFuncRecord
{
  unsigned Color;
  unsigned Size;
  double From;
  double To;
  unsigned Steps;
  unsigned LineStyle;
  unsigned StartMarker;
  unsigned EndMarker;
  BSTR FuncText;
  BSTR LegendText;
};

// *********************************************************************//
// Interface: IGraphServer
// Flags:     (4416) Dual OleAutomation Dispatchable
// GUID:      {4729DE98-E9CB-4DFC-87CE-C7CCF9E302C3}
// *********************************************************************//
interface IGraphServer  : public IDispatch
{
public:
  // [1] Show application
  virtual HRESULT STDMETHODCALLTYPE Show(void) = 0;
  // [2] Hide the application
  virtual HRESULT STDMETHODCALLTYPE Hide(void) = 0;
  // [3] Load a grf file
  virtual HRESULT STDMETHODCALLTYPE Load(BSTR FileName/*[in]*/) = 0;
  // [4] Import point series from text file
  virtual HRESULT STDMETHODCALLTYPE ImportPointSeries(BSTR FileName/*[in]*/) = 0;
  // [5] Save to a grf file
  virtual HRESULT STDMETHODCALLTYPE Save(BSTR FileName/*[in]*/, VARIANT_BOOL Remember/*[in]*/) = 0;
  // [6] Save current coordinate system as an image
  virtual HRESULT STDMETHODCALLTYPE SaveAsImage(BSTR FileName/*[in]*/, 
                                                Graph_tlb::TImageFileType ImageFileType/*[in]*/) = 0;
  // [7] Zoom all points
  virtual HRESULT STDMETHODCALLTYPE ZoomAllPoints(void) = 0;
  virtual HRESULT STDMETHODCALLTYPE Refresh(void) = 0; // [13]
  virtual HRESULT STDMETHODCALLTYPE get_Axes(Graph_tlb::TAxesRecord* Value/*[out,retval]*/) = 0; // [15]
  virtual HRESULT STDMETHODCALLTYPE set_Axes(Graph_tlb::TAxesRecord Value/*[in]*/) = 0; // [15]
  virtual HRESULT STDMETHODCALLTYPE AddFunc(Graph_tlb::TFuncRecord Func/*[in]*/) = 0; // [12]

#if !defined(__TLB_NO_INTERFACE_WRAPPERS)

  Graph_tlb::TAxesRecord __fastcall get_Axes(void)
  {
    Graph_tlb::TAxesRecord Value;
    OLECHECK(this->get_Axes((Graph_tlb::TAxesRecord*)&Value));
    return Value;
  }


  __property   Graph_tlb::TAxesRecord Axes = {read = get_Axes, write = set_Axes};

#endif //   __TLB_NO_INTERFACE_WRAPPERS

};

#if !defined(__TLB_NO_INTERFACE_WRAPPERS)
// *********************************************************************//
// SmartIntf: TCOMIGraphServer
// Interface: IGraphServer
// *********************************************************************//
template <class T /* IGraphServer */ >
class TCOMIGraphServerT : public TComInterface<IGraphServer>, public TComInterfaceBase<IUnknown>
{
public:
  TCOMIGraphServerT() {}
  TCOMIGraphServerT(IGraphServer *intf, bool addRef = false) : TComInterface<IGraphServer>(intf, addRef) {}
  TCOMIGraphServerT(const TCOMIGraphServerT& src) : TComInterface<IGraphServer>(src) {}
  TCOMIGraphServerT& operator=(const TCOMIGraphServerT& src) { Bind(src, true); return *this;}

  HRESULT         __fastcall Show(void);
  HRESULT         __fastcall Hide(void);
  HRESULT         __fastcall Load(BSTR FileName/*[in]*/);
  HRESULT         __fastcall ImportPointSeries(BSTR FileName/*[in]*/);
  HRESULT         __fastcall Save(BSTR FileName/*[in]*/, VARIANT_BOOL Remember/*[in]*/);
  HRESULT         __fastcall SaveAsImage(BSTR FileName/*[in]*/, 
                                         Graph_tlb::TImageFileType ImageFileType/*[in]*/);
  HRESULT         __fastcall ZoomAllPoints(void);
  HRESULT         __fastcall Refresh(void);
  HRESULT         __fastcall get_Axes(Graph_tlb::TAxesRecord* Value/*[out,retval]*/);
  Graph_tlb::TAxesRecord __fastcall get_Axes(void);
  HRESULT         __fastcall set_Axes(Graph_tlb::TAxesRecord Value/*[in]*/);
  HRESULT         __fastcall AddFunc(Graph_tlb::TFuncRecord Func/*[in]*/);

  __property   Graph_tlb::TAxesRecord Axes = {read = get_Axes, write = set_Axes};
};
typedef TCOMIGraphServerT<IGraphServer> TCOMIGraphServer;

// *********************************************************************//
// DispIntf:  IGraphServer
// Flags:     (4416) Dual OleAutomation Dispatchable
// GUID:      {4729DE98-E9CB-4DFC-87CE-C7CCF9E302C3}
// *********************************************************************//
template<class T>
class IGraphServerDispT : public TAutoDriver<IGraphServer>
{
public:
  IGraphServerDispT(){}

  IGraphServerDispT(IGraphServer *pintf)
  {
    TAutoDriver<IGraphServer>::Bind(pintf, false);
  }

  IGraphServerDispT(IGraphServerPtr pintf)
  {
    TAutoDriver<IGraphServer>::Bind(pintf, true);
  }

  IGraphServerDispT& operator=(IGraphServer *pintf)
  {
    TAutoDriver<IGraphServer>::Bind(pintf, false);
    return *this;
  }

  IGraphServerDispT& operator=(IGraphServerPtr pintf)
  {
    TAutoDriver<IGraphServer>::Bind(pintf, true);
    return *this;
  }

  HRESULT BindDefault()
  {
    return OLECHECK(Bind(CLSID_GraphServer));
  }

  HRESULT BindRunning()
  {
    return BindToActive(CLSID_GraphServer);
  }

  HRESULT         __fastcall Show();
  HRESULT         __fastcall Hide();
  HRESULT         __fastcall Load(BSTR FileName/*[in]*/);
  HRESULT         __fastcall ImportPointSeries(BSTR FileName/*[in]*/);
  HRESULT         __fastcall Save(BSTR FileName/*[in]*/, VARIANT_BOOL Remember/*[in]*/);
  HRESULT         __fastcall SaveAsImage(BSTR FileName/*[in]*/, 
                                         Graph_tlb::TImageFileType ImageFileType/*[in]*/);
  HRESULT         __fastcall ZoomAllPoints();
  HRESULT         __fastcall Refresh();
  HRESULT         __fastcall get_Axes(/*AUTO_PARAM_ERROR(Graph_tlb::TAxesRecord* Value)*/ TVariant*  Value);
  HRESULT         __fastcall set_Axes(/*AUTO_PARAM_ERROR(Graph_tlb::TAxesRecord Value)*/ TVariant&  Value);
  HRESULT         __fastcall AddFunc(/*AUTO_PARAM_ERROR(Graph_tlb::TFuncRecord Func)*/ TVariant&  Func);

};
typedef IGraphServerDispT<IGraphServer> IGraphServerDisp;

// *********************************************************************//
// SmartIntf: TCOMIGraphServer
// Interface: IGraphServer
// *********************************************************************//
template <class T> HRESULT __fastcall
TCOMIGraphServerT<T>::Show(void)
{
  return (*this)->Show();
}

template <class T> HRESULT __fastcall
TCOMIGraphServerT<T>::Hide(void)
{
  return (*this)->Hide();
}

template <class T> HRESULT __fastcall
TCOMIGraphServerT<T>::Load(BSTR FileName/*[in]*/)
{
  return (*this)->Load(FileName);
}

template <class T> HRESULT __fastcall
TCOMIGraphServerT<T>::ImportPointSeries(BSTR FileName/*[in]*/)
{
  return (*this)->ImportPointSeries(FileName);
}

template <class T> HRESULT __fastcall
TCOMIGraphServerT<T>::Save(BSTR FileName/*[in]*/, VARIANT_BOOL Remember/*[in]*/)
{
  return (*this)->Save(FileName, Remember);
}

template <class T> HRESULT __fastcall
TCOMIGraphServerT<T>::SaveAsImage(BSTR FileName/*[in]*/, 
                                  Graph_tlb::TImageFileType ImageFileType/*[in]*/)
{
  return (*this)->SaveAsImage(FileName, ImageFileType);
}

template <class T> HRESULT __fastcall
TCOMIGraphServerT<T>::ZoomAllPoints(void)
{
  return (*this)->ZoomAllPoints();
}

template <class T> HRESULT __fastcall
TCOMIGraphServerT<T>::Refresh(void)
{
  return (*this)->Refresh();
}

template <class T> HRESULT __fastcall
TCOMIGraphServerT<T>::get_Axes(Graph_tlb::TAxesRecord* Value/*[out,retval]*/)
{
  return (*this)->get_Axes(Value);
}

template <class T> Graph_tlb::TAxesRecord __fastcall
TCOMIGraphServerT<T>::get_Axes(void)
{
  Graph_tlb::TAxesRecord Value;
  OLECHECK(this->get_Axes((Graph_tlb::TAxesRecord*)&Value));
  return Value;
}

template <class T> HRESULT __fastcall
TCOMIGraphServerT<T>::set_Axes(Graph_tlb::TAxesRecord Value/*[in]*/)
{
  return (*this)->set_Axes(Value);
}

template <class T> HRESULT __fastcall
TCOMIGraphServerT<T>::AddFunc(Graph_tlb::TFuncRecord Func/*[in]*/)
{
  return (*this)->AddFunc(Func);
}

// *********************************************************************//
// DispIntf:  IGraphServer
// Flags:     (4416) Dual OleAutomation Dispatchable
// GUID:      {4729DE98-E9CB-4DFC-87CE-C7CCF9E302C3}
// *********************************************************************//
template <class T> HRESULT __fastcall
IGraphServerDispT<T>::Show()
{
  _TDispID _dispid(*this, OLETEXT("Show"), DISPID(1));
  return OleFunction(_dispid);
}

template <class T> HRESULT __fastcall
IGraphServerDispT<T>::Hide()
{
  _TDispID _dispid(*this, OLETEXT("Hide"), DISPID(2));
  return OleFunction(_dispid);
}

template <class T> HRESULT __fastcall
IGraphServerDispT<T>::Load(BSTR FileName/*[in]*/)
{
  _TDispID _dispid(*this, OLETEXT("Load"), DISPID(3));
  TAutoArgs<1> _args;
  _args[1] = FileName /*[VT_BSTR:0]*/;
  return OleFunction(_dispid, _args);
}

template <class T> HRESULT __fastcall
IGraphServerDispT<T>::ImportPointSeries(BSTR FileName/*[in]*/)
{
  _TDispID _dispid(*this, OLETEXT("ImportPointSeries"), DISPID(4));
  TAutoArgs<1> _args;
  _args[1] = FileName /*[VT_BSTR:0]*/;
  return OleFunction(_dispid, _args);
}

template <class T> HRESULT __fastcall
IGraphServerDispT<T>::Save(BSTR FileName/*[in]*/, VARIANT_BOOL Remember/*[in]*/)
{
  _TDispID _dispid(*this, OLETEXT("Save"), DISPID(5));
  TAutoArgs<2> _args;
  _args[1] = FileName /*[VT_BSTR:0]*/;
  _args[2] = Remember /*[VT_BOOL:0]*/;
  return OleFunction(_dispid, _args);
}

template <class T> HRESULT __fastcall
IGraphServerDispT<T>::SaveAsImage(BSTR FileName/*[in]*/, 
                                  Graph_tlb::TImageFileType ImageFileType/*[in]*/)
{
  _TDispID _dispid(*this, OLETEXT("SaveAsImage"), DISPID(6));
  TAutoArgs<2> _args;
  _args[1] = FileName /*[VT_BSTR:0]*/;
  _args[2] = (int)ImageFileType /*[VT_USERDEFINED:0]*/;
  return OleFunction(_dispid, _args);
}

template <class T> HRESULT __fastcall
IGraphServerDispT<T>::ZoomAllPoints()
{
  _TDispID _dispid(*this, OLETEXT("ZoomAllPoints"), DISPID(7));
  return OleFunction(_dispid);
}

template <class T> HRESULT __fastcall
IGraphServerDispT<T>::Refresh()
{
  _TDispID _dispid(*this, OLETEXT("Refresh"), DISPID(13));
  return OleFunction(_dispid);
}

template <class T> HRESULT __fastcall
IGraphServerDispT<T>::get_Axes(/*AUTO_PARAM_ERROR(Graph_tlb::TAxesRecord* Value)*/ TVariant*  Value)
{
  _TDispID _dispid(*this, OLETEXT("Axes"), DISPID(15));
  TAutoArgs<0> _args;
  return OutRetValSetterPtr(Value /*[VT_USERDEFINED:1]*/, _args, OlePropertyGet(_dispid, _args));
}

template <class T> HRESULT __fastcall
IGraphServerDispT<T>::set_Axes(/*AUTO_PARAM_ERROR(Graph_tlb::TAxesRecord Value)*/ TVariant&  Value)
{
  _TDispID _dispid(*this, OLETEXT("Axes"), DISPID(15));
  TAutoArgs<1> _args;
  _args[1] = Value /*[VT_USERDEFINED:0]*/;
  return OlePropertyPut(_dispid, _args);
}

template <class T> HRESULT __fastcall
IGraphServerDispT<T>::AddFunc(/*AUTO_PARAM_ERROR(Graph_tlb::TFuncRecord Func)*/ TVariant&  Func)
{
  _TDispID _dispid(*this, OLETEXT("AddFunc"), DISPID(12));
  TAutoArgs<1> _args;
  _args[1] = Func /*[VT_USERDEFINED:0]*/;
  return OleFunction(_dispid, _args);
}

// *********************************************************************//
// The following typedefs expose classes (named CoCoClassName) that       
// provide static Create() and CreateRemote(LPWSTR machineName) methods   
// for creating an instance of an exposed object. These functions can     
// be used by client wishing to automate CoClasses exposed by this        
// typelibrary.                                                           
// *********************************************************************//

// *********************************************************************//
// COCLASS DEFAULT INTERFACE CREATOR
// CoClass  : GraphServer
// Interface: TCOMIGraphServer
// *********************************************************************//
typedef TCoClassCreatorT<TCOMIGraphServer, IGraphServer, &CLSID_GraphServer, &IID_IGraphServer> CoGraphServer;
#endif  //   __TLB_NO_INTERFACE_WRAPPERS


};     // namespace Graph_tlb

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace Graph_tlb;
#endif

#pragma option pop

#endif // Graph_TLBH
