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

  void SetFormat(::CHARFORMAT2 Format, unsigned Mask);
  void SetFormat(unsigned Format, unsigned Effects);
  ::CHARFORMAT2 GetFormat() const;

  TTextFormat(const TTextFormat&);                  //Not implemented
  const TCharFormat& operator=(const TTextFormat&); //Not implemented

public:
  TTextFormat(TIRichEdit *ARichEdit);
  void SetBold(bool Value);
  void SetItalic(bool Value);
  void SetUnderline(bool Value);
  void SetStrikeOut(bool Value);
  void SetSubscript(bool Value);
  void SetSuperscript(bool Value);
  void SetName(const AnsiString &Str);
  void SetSize(unsigned Value);
  void SetColor(TColor Color);
  void SetBackgroundColor(TColor Color);
  void SetLink(bool Value);
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

  void __fastcall SetTransparent(bool Value);
  void __fastcall SetBackgroundColor(TColor Color);

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
  void SetSelText(wchar_t Ch, const AnsiString &FontName, unsigned Size);
  void SetSelText(char Ch, const AnsiString &FontName, unsigned Size);
  void __fastcall SetAutoUrlDetect(bool Value);
  bool __fastcall GetAutoUrlDetect();
  void __fastcall SetOnLink(TLinkEvent Value);
  void SetEventMask(DWORD Mask, DWORD Value);
  __property ::TParaFormat *Paragraph = {read=FParagraph};
  __property SelText;
//  __property Lines; //Don't use this

__published:
  __property bool Transparent = {read=FTransparent, write=SetTransparent, default=false};
  __property TColor BackgroundColor = {read=FBackgroundColor, write=SetBackgroundColor, default=clDefault};
  __property bool AutoUrlDetect = {read=GetAutoUrlDetect, write=SetAutoUrlDetect};
  __property TOleErrorEvent OnOleError = {read=FOnOleError, write=FOnOleError, default=NULL};
  __property TLinkEvent OnLink = {read=FOnLink, write=SetOnLink, default=NULL};

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
};
//---------------------------------------------------------------------------
#endif
