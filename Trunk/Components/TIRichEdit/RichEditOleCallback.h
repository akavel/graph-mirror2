//---------------------------------------------------------------------------
// class TRichEditOle - adds basic OLE functionality to TCustomRichEdit.
// Based on code found at borland.public.cppbuilder.* posted by Robert Dunn
// A lot of information about RichEdit can be found at his web site:
// http://home.att.net/~robertdunn/Yacs.html
//---------------------------------------------------------------------------
#ifndef RichEditOleCallbackH
#define RichEditOleCallbackH

#include <comctrls.hpp>
#include <comobj.hpp>
#include <\ole2.hpp>
#include <oledlg.hpp>
#include <richole.h>
//---------------------------------------------------------------------------
class TRichEditOleCallback : public IRichEditOleCallback
{
protected:
	TCustomRichEdit* RichEdit;
	ULONG FRefCnt;
	bool FAcceptDrop;

public:
	// *** IUnknown methods ***
	STDMETHOD(QueryInterface) (THIS_ REFIID riid, LPVOID FAR * lplpObj);
	STDMETHOD_(ULONG, AddRef) (THIS);
	STDMETHOD_(ULONG, Release) (THIS);

	// *** IRichEditOleCallback methods ***
	STDMETHOD(GetNewStorage) (THIS_ LPSTORAGE FAR * lplpstg);
	STDMETHOD(GetInPlaceContext) (THIS_ LPOLEINPLACEFRAME FAR * lplpFrame,
		LPOLEINPLACEUIWINDOW FAR * lplpDoc, LPOLEINPLACEFRAMEINFO lpFrameInfo);
	STDMETHOD(ShowContainerUI) (THIS_ BOOL fShow);
	STDMETHOD(QueryInsertObject) (THIS_ LPCLSID lpclsid, LPSTORAGE lpstg,
		LONG cp);
	STDMETHOD(DeleteObject) (THIS_ LPOLEOBJECT lpoleobj);
	STDMETHOD(QueryAcceptData) (THIS_ LPDATAOBJECT lpdataobj,
		CLIPFORMAT FAR * lpcfFormat, DWORD reco, BOOL fReally, HGLOBAL hMetaPict);
	STDMETHOD(ContextSensitiveHelp) (THIS_ BOOL fEnterMode);
	STDMETHOD(GetClipboardData) (THIS_ CHARRANGE FAR * lpchrg, DWORD reco,
		LPDATAOBJECT FAR * lplpdataobj);
	STDMETHOD(GetDragDropEffect) (THIS_ BOOL fDrag, DWORD grfKeyState,
		LPDWORD pdwEffect);
	STDMETHOD(GetContextMenu) (THIS_ WORD seltype, LPOLEOBJECT lpoleobj,
		CHARRANGE FAR * lpchrg, HMENU FAR * lphmenu);

	TRichEditOleCallback(TCustomRichEdit* ARichEdit);

	__property bool AcceptDrop = {read = FAcceptDrop, write = FAcceptDrop};
};
//---------------------------------------------------------------------------
#endif

