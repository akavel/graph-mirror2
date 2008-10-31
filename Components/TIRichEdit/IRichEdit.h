//---------------------------------------------------------------------------
#ifndef IRichEditH
#define IRichEditH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TIRichEdit;
typedef void __fastcall (__closure *TOleErrorEvent)(TIRichEdit* Sender, int Oper, int ErrorCode);
typedef void __fastcall (__closure *TLinkEvent)(TIRichEdit* Sender, unsigned Min, unsigned Max);

class TTextFormat
{
  class TIRichEdit *RichEdit;
  bool Global;

  void SetFormat(::CHARFORMAT2 Format, unsigned Mask);
  void SetFormat(unsigned Format, unsigned Effects);
  ::CHARFORMAT2 GetFormat() const;

  TTextFormat(const TTextFormat&);                  //Not implemented
  const TCharFormat& operator=(const TTextFormat&); //Not implemented

public:
  TTextFormat(TIRichEdit *ARichEdit, bool AGlobal);
  void SetBold(bool Value);
  void SetItalic(bool Value);
  void SetUnderline(bool Value);
  void SetStrikeOut(bool Value);
  void SetSubscript(bool Value);
  void SetSuperscript(bool Value);
  void SetName(const String &Str);
  void SetSize(unsigned Value);
  void SetColor(TColor Color);
  void SetBackgroundColor(TColor Color);
  void SetLink(bool Value);
  void SetProtected(bool Value);
  bool GetBold() const;
  bool GetItalic() const;
  bool GetUnderline() const;
  bool GetStrikeOut() const;
  bool GetSubscript() const;
  bool GetSuperscript() const;
  AnsiString GetName() const;
  unsigned GetSize() const;
  TColor GetColor() const;
  TColor GetBackgroundColor() const;
  bool GetLink() const;
  bool GetProtected() const;
};

enum TParaFormatAlignment
{
  pfaLeft = PFA_LEFT,
  pfaRight = PFA_RIGHT,
  pfaCenter = PFA_CENTER,
  pfaJustify = PFA_JUSTIFY,
  pfaFullInterword = PFA_FULL_INTERWORD
};

class TParaFormat : public TParaAttributes
{
  TIRichEdit *RichEdit;
  TParaFormatAlignment GetAlignment();
  void SetAlignment(TParaFormatAlignment Value);
  ::PARAFORMAT2 GetFormat() const;

public:
  TParaFormat(TIRichEdit *ARichEdit);
  __property TParaFormatAlignment Alignment = {read=GetAlignment, write=SetAlignment};
};

class TIRichEdit : public TCustomRichEdit
{
private:
  bool FTransparent;
  HINSTANCE FLibHandle;
  TOleErrorEvent FOnOleError;
  TColor FBackgroundColor;
  ::TParaFormat *FParagraph;
  TLinkEvent FOnLink;
  bool FProtectedChange;

  void __fastcall SetTransparent(bool Value);
  void __fastcall SetBackgroundColor(TColor Color);
  void __fastcall SetAutoUrlDetect(bool Value);
  bool __fastcall GetAutoUrlDetect();
  void __fastcall SetOnLink(TLinkEvent Value);
  void __fastcall SetWideSelText(const WideString &Str);
  WideString __fastcall GetWideSelText();

  void __fastcall WMNotify(TMessage &Message);

BEGIN_MESSAGE_MAP
  VCL_MESSAGE_HANDLER(CN_NOTIFY, TMessage, WMNotify)
END_MESSAGE_MAP(TCustomRichEdit)

protected:
  void __fastcall CreateParams(Controls::TCreateParams &Params);
  void __fastcall DestroyWnd();
  bool DoLink(UINT Msg, unsigned Min, unsigned Max);

public:
  ::TTextFormat TextFormat;
  ::TTextFormat GlobalTextFormat;

  __fastcall TIRichEdit(TComponent* Owner);
  __fastcall ~TIRichEdit();

  void SetRichText(const AnsiString &Str);
  AnsiString GetRichText();
  void Render(TCanvas *Canvas, const TPoint &Pos, int Width = MAXSHORT);
  TPoint GetTextSize();
  bool IsEmpty() const;
  void Redo();
  bool CanRedo();
  bool CanPaste();
  void SetUndoLimit(unsigned Limit);
  AnsiString GetPlainText() {return Lines->Text;}
  int LineIndex(int Line);
  int LineCount();
  int LineLength(int Index);
  int GetLine(int Index);
  void SetSelText(wchar_t Ch, const AnsiString &FontName, unsigned Size);
  void SetSelText(char Ch, const AnsiString &FontName, unsigned Size);
  void SetEventMask(DWORD Mask, DWORD Value);
  int TextSize();

  __property ::TParaFormat *Paragraph = {read=FParagraph};
  __property WideString SelText = {read=GetWideSelText, write=SetWideSelText};
//  __property Lines; //Don't use this
  WideString GetText(int Min, int Max);

__published:
  __property bool Transparent = {read=FTransparent, write=SetTransparent, default=false};
  __property TColor BackgroundColor = {read=FBackgroundColor, write=SetBackgroundColor, default=clDefault};
  __property bool AutoUrlDetect = {read=GetAutoUrlDetect, write=SetAutoUrlDetect};
  __property TOleErrorEvent OnOleError = {read=FOnOleError, write=FOnOleError, default=NULL};
  __property TLinkEvent OnLink = {read=FOnLink, write=SetOnLink, default=NULL};
  __property bool ProtectedChange = {read=FProtectedChange, write=FProtectedChange, default=false};

  __property BorderStyle;
  __property WordWrap;
  __property HideScrollBars;
  __property HideSelection;
  __property ScrollBars;
  __property WantReturns;
  __property WantTabs;
  __property ReadOnly;
  __property Visible;
  __property Align;
  __property Font;
  __property ShowHint;
  __property ParentShowHint;
  __property PopupMenu;
  __property ParentFont;
  __property TabOrder;
  __property Enabled;
  __property BiDiMode;
  __property ParentBiDiMode;

  __property OnChange;
  __property OnSelectionChange;
  __property OnKeyDown;
  __property OnKeyUp;
  __property OnKeyPress;
  __property OnEnter;
  __property OnExit;
  __property OnMouseMove;
  __property OnMouseDown;
  __property OnMouseUp;
  __property OnProtectChange;
};
//---------------------------------------------------------------------------
#endif
