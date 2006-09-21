//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "IRichEdit.h"
#include <memory>
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TIRichEdit *)
{
  new TIRichEdit(NULL);
}
//---------------------------------------------------------------------------
namespace Irichedit
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TIRichEdit)};
     RegisterComponents("IComp", classes, 0);
  }
}
//---------------------------------------------------------------------------
__fastcall TIRichEdit::TIRichEdit(TComponent* Owner)
  : TCustomRichEdit(Owner), FTransparent(false), TextFormat(this), FOnOleError(NULL), FBackgroundColor(clDefault),
    FParagraph(new ::TParaFormat(this))
{
  ControlStyle = ControlStyle >> csSetCaption;
}
//---------------------------------------------------------------------------
__fastcall TIRichEdit::~TIRichEdit()
{
  delete FParagraph;
}
//---------------------------------------------------------------------------
//The following ensures that the new version of richedit (v2.0) is loaded;
// Note: Do not call TRichEdit::CreateParams -- it will attempt to
// load the older version of the rich edit DLL -- not a Good Thing -- too bad
// if anything important goes on there
void __fastcall TIRichEdit::CreateParams(Controls::TCreateParams &Params)
{
	// modified from TCustomRichEdit
	FLibHandle = LoadLibrary("RICHED20.DLL");
	if(!FLibHandle)
    FLibHandle = 0;

	TCustomMemo::CreateParams(Params); //Don't call TCustomRichEdit::CreateParams

	CreateSubClass(Params, "RichEdit20W" /*RICHEDIT_CLASS*/);
	Params.Style = Params.Style |
		(HideScrollBars ? 0 : ES_DISABLENOSCROLL) |
		(HideSelection ? 0 : ES_NOHIDESEL);
//	Params.WindowClass.style &= ~(CS_HREDRAW | CS_VREDRAW);

  Params.ExStyle |= (FTransparent ? WS_EX_TRANSPARENT : 0);
}
//---------------------------------------------------------------------------
//Free the library on destroy (actually, it will not unload until last
// TRIichEdit instance is destroyed)
void __fastcall TIRichEdit::DestroyWnd(void)
{
	TCustomRichEdit::DestroyWnd();

	if(FLibHandle)
    FreeLibrary(FLibHandle);
}
//---------------------------------------------------------------------------
void __fastcall TIRichEdit::SetTransparent(bool Value)
{
	FTransparent = Value;

  //We cannot change style if there is no handle
  if(HandleAllocated())
  {
    LONG exStyle = GetWindowLong(Handle, GWL_EXSTYLE);
    if(FTransparent)
    {
      exStyle |= WS_EX_TRANSPARENT;
      Brush->Style = bsClear;
    }
    else
    {
      exStyle &= ~(WS_EX_TRANSPARENT);
      Brush->Style = bsSolid;
    }
    SetWindowLong(Handle, GWL_EXSTYLE, exStyle);
  }
}
//---------------------------------------------------------------------------
void TIRichEdit::SetRichText(const AnsiString &Str)
{
  std::auto_ptr<TStringStream> Stream(new TStringStream(Str));
  Lines->LoadFromStream(Stream.get());
}
//---------------------------------------------------------------------------
AnsiString TIRichEdit::GetRichText()
{
  std::auto_ptr<TStringStream> Stream(new TStringStream(""));
  Lines->SaveToStream(Stream.get());

  //The string will have a '\0' embedded as the last character
  //I don't know where this comes from, but it gives problems when saved to file
  //It looks like there may be more than one
  AnsiString Str = Stream->DataString;
  while(Str[Str.Length()] == 0)
    Str.Delete(Str.Length(), 1);
  return Str;
}
//---------------------------------------------------------------------------
void TIRichEdit::Render(TCanvas *Canvas, const TPoint &Pos, int Width)
{
  FORMATRANGE Format;
  Format.hdc = Canvas->Handle;

  Format.hdcTarget = Canvas->Handle;

  //Convert coordinates to twips
  int PixelsX = GetDeviceCaps(Canvas->Handle, LOGPIXELSX);
  int PixelsY = GetDeviceCaps(Canvas->Handle, LOGPIXELSY);
  if(PixelsX == 0 || PixelsY == 0)
    return;

  double xPixels = 1440.0 / PixelsX;
  double yPixels = 1440.0 / PixelsY;
                                                  
  Format.rc.left = Pos.x * xPixels;
  Format.rc.top = Pos.y * yPixels;
  Format.rc.right = (Pos.x + Width) * xPixels;
  Format.rc.bottom = MAXSHORT;

  Format.rcPage = Format.rc;
  Format.chrg.cpMin = 0;
  Format.chrg.cpMax = -1;

  SendMessage(Handle, EM_FORMATRANGE, 1, reinterpret_cast<long>(&Format));

  TRect Rect(Pos, TPoint(Pos.x + Width, MaxInt));
  if(!SendMessage(Handle, EM_DISPLAYBAND , 0, reinterpret_cast<long>(&Rect)))
    RaiseLastOSError();

  //Free cached memory
  SendMessage(Handle, EM_FORMATRANGE, 1, NULL);
}                             
//---------------------------------------------------------------------------
//Returns the size of the text
TPoint TIRichEdit::GetTextSize()
{
  TPoint Point;
  int Lines = LineCount();
  TPoint Size(0, 0);
  for(int I = 0; I < Lines; I++)
  {
    int Index = I ? LineIndex(I) : MaxInt;
    SendMessage(Handle, EM_POSFROMCHAR, reinterpret_cast<WPARAM>(&Point), Index - 1);
    if(Point.x > Size.x)
      Size.x = Point.x;
    if(Point.y > Size.y)
      Size.y = Point.y;
  }

  return Size;
}
//---------------------------------------------------------------------------
bool TIRichEdit::IsEmpty() const
{
  return !Lines->Count;
}
//---------------------------------------------------------------------------
TTextFormat::TTextFormat(TIRichEdit *ARichEdit) : RichEdit(ARichEdit)
{
}
//---------------------------------------------------------------------------
void TTextFormat::SetFormat(::CHARFORMAT2 Format, unsigned Mask)
{
  Format.cbSize = sizeof(Format);
  Format.dwMask = Mask;
  Format.dwReserved = 0;
  if(!SendMessage(RichEdit->Handle, EM_SETCHARFORMAT, SCF_SELECTION, reinterpret_cast<long>(&Format)))
    RaiseLastOSError();
}
//---------------------------------------------------------------------------
void TTextFormat::SetFormat(unsigned Mask, unsigned Effects)
{
  ::CHARFORMAT2 Format;
  Format.dwEffects = Effects;
  SetFormat(Format, Mask);
}
//---------------------------------------------------------------------------
::CHARFORMAT2 TTextFormat::GetFormat() const
{
  ::CHARFORMAT2 Format;
  Format.cbSize = sizeof(Format);

  SendMessage(RichEdit->Handle, EM_GETCHARFORMAT, SCF_SELECTION, reinterpret_cast<long>(&Format));
  return Format;
}
//---------------------------------------------------------------------------
void TTextFormat::SetSuperscript(bool Value)
{
  SetFormat(CFM_SUPERSCRIPT, Value ? CFE_SUPERSCRIPT : 0);
}
//---------------------------------------------------------------------------
void TTextFormat::SetSubscript(bool Value)
{
  SetFormat(CFM_SUBSCRIPT, Value ? CFE_SUBSCRIPT : 0);
}
//---------------------------------------------------------------------------
void TTextFormat::SetBold(bool Value)
{
  SetFormat(CFM_BOLD, Value ? CFE_BOLD : 0);
}
//---------------------------------------------------------------------------
void TTextFormat::SetItalic(bool Value)
{
  SetFormat(CFM_ITALIC, Value ? CFE_ITALIC : 0);
}
//---------------------------------------------------------------------------
void TTextFormat::SetUnderline(bool Value)
{
  SetFormat(CFM_UNDERLINE, Value ? CFE_UNDERLINE : 0);
}
//---------------------------------------------------------------------------
void TTextFormat::SetStrikeOut(bool Value)
{
  SetFormat(CFM_STRIKEOUT, Value ? CFE_STRIKEOUT : 0);
}
//---------------------------------------------------------------------------
void TTextFormat::SetSize(unsigned Value)
{
  ::CHARFORMAT2 Format;
  Format.yHeight = Value * 20;
  SetFormat(Format, CFM_SIZE);
}
//---------------------------------------------------------------------------
unsigned GetCharset(const AnsiString &FontName)
{
  //We need the charset to make sure that the correct font is used. Especially "script"
  //Doesn't work else. This way is a little overkill, but it works.
  //It I first set the font to "ABSALOM" and when to "Script", the font will be
  //shown as wingdings instead. This seems to be a rich edit bug, as it also happens in Wordpad
  std::auto_ptr<TCanvas> Canvas(new TCanvas);
  Canvas->Handle = GetDC(NULL);
  Canvas->Font->Name = FontName;
  unsigned Charset = GetTextCharset(Canvas->Handle);
  ReleaseDC(NULL, Canvas->Handle);
  return Charset;
}
//---------------------------------------------------------------------------
void TTextFormat::SetName(const AnsiString &Str)
{
  ::CHARFORMAT2 Format;
  strncpy(Format.szFaceName, Str.c_str(), sizeof(Format.szFaceName));
  Format.szFaceName[sizeof(Format.szFaceName) - 1] = 0;
  Format.bCharSet = GetCharset(Str);
  SetFormat(Format, CFM_FACE);    //We must set the font name first and the charset afterwards
  SetFormat(Format, CFM_CHARSET);
}
//---------------------------------------------------------------------------
void TTextFormat::SetColor(TColor Value)
{
  ::CHARFORMAT2 Format;
  Format.crTextColor = ColorToRGB(Value);
  Format.dwEffects = 0;
  SetFormat(Format, CFM_COLOR);
}
//---------------------------------------------------------------------------
//Set background color for text; Use clNone for transparent
void TTextFormat::SetBackgroundColor(TColor Value)
{
  ::CHARFORMAT2 Format;
  Format.crBackColor = ColorToRGB(Value);
  Format.dwEffects = Value == clNone ? CFE_AUTOBACKCOLOR : 0;
  SetFormat(Format, CFM_BACKCOLOR);
}
//---------------------------------------------------------------------------
bool TTextFormat::GetSuperscript() const
{
  return GetFormat().dwEffects & CFE_SUPERSCRIPT;
}
//---------------------------------------------------------------------------
bool TTextFormat::GetSubscript() const
{
  return GetFormat().dwEffects & CFE_SUBSCRIPT;
}
//---------------------------------------------------------------------------
bool TTextFormat::GetBold() const
{
  return GetFormat().dwEffects & CFE_BOLD;
}
//---------------------------------------------------------------------------
bool TTextFormat::GetItalic() const
{
  return GetFormat().dwEffects & CFE_ITALIC;
}
//---------------------------------------------------------------------------
bool TTextFormat::GetUnderline() const
{
  return GetFormat().dwEffects & CFE_UNDERLINE;
}
//---------------------------------------------------------------------------
bool TTextFormat::GetStrikeOut() const
{
  return GetFormat().dwEffects & CFE_STRIKEOUT;
}
//---------------------------------------------------------------------------
unsigned TTextFormat::GetSize() const
{
  return GetFormat().yHeight / 20;
}
//---------------------------------------------------------------------------
AnsiString TTextFormat::GetName() const
{
  return GetFormat().szFaceName;
}
//---------------------------------------------------------------------------
TColor TTextFormat::GetColor() const
{
  return static_cast<TColor>(GetFormat().crTextColor);
}
//---------------------------------------------------------------------------
TColor TTextFormat::GetBackgroundColor() const
{
  ::CHARFORMAT2 Format = GetFormat();
  return Format.dwEffects & CFE_AUTOBACKCOLOR ? clNone : static_cast<TColor>(Format.crBackColor);
}
//---------------------------------------------------------------------------
void TIRichEdit::Redo()
{
  SendMessage(Handle, EM_REDO, 0, 0); 
}
//---------------------------------------------------------------------------
bool TIRichEdit::CanRedo()
{
  return SendMessage(Handle, EM_CANREDO, 0, 0);
}
//---------------------------------------------------------------------------
bool TIRichEdit::CanPaste() 
{
  return SendMessage(Handle, EM_CANPASTE, 0, 0);
}
//---------------------------------------------------------------------------
void TIRichEdit::SetUndoLimit(unsigned Limit)
{
  SendMessage(Handle, EM_SETUNDOLIMIT, Limit, 0);
}
//---------------------------------------------------------------------------
//Returns the character index for the given line number.
//Line=-1 return index for the current line
//Returns -1 if the line does not exist
int TIRichEdit::LineIndex(int Line)
{
  return SendMessage(Handle, EM_LINEINDEX, Line, 0);
}
//---------------------------------------------------------------------------
//Returns the number of lines in the rich edit
int TIRichEdit::LineCount()
{
  return SendMessage(Handle, EM_GETLINECOUNT, 0, 0);
}
//---------------------------------------------------------------------------
//Returns the length in characters of line index by character (and not indexed by line)
int TIRichEdit::LineLength(int Index)
{
  return SendMessage(Handle, EM_LINELENGTH, Index, 0);
}
//---------------------------------------------------------------------------
void __fastcall TIRichEdit::WMNotify(TMessage &Message)
{
  ENOLEOPFAILED *OleOpFailed = reinterpret_cast<ENOLEOPFAILED*>(Message.LParam);
  if(OleOpFailed->nmhdr.code == EN_OLEOPFAILED)
    if(FOnOleError)
      FOnOleError(this, OleOpFailed->lOper, OleOpFailed->hr);
  TCustomRichEdit::Dispatch(&Message);
}
//---------------------------------------------------------------------------
void __fastcall TIRichEdit::SetBackgroundColor(TColor Color)
{
  FBackgroundColor = Color;
  SendMessage(Handle, EM_SETBKGNDCOLOR, Color == clDefault, ColorToRGB(Color));
}
//---------------------------------------------------------------------------
void TIRichEdit::SetSelText(wchar_t Ch, const AnsiString &FontName, unsigned Size)
{
  SETTEXTEX SetTextEx = {ST_KEEPUNDO | ST_SELECTION, CP_ACP};
  AnsiString Temp;
  unsigned Char = Ch;
  Temp.sprintf("{\\rtf1{\\fonttbl{\\f0\\fcharset%u %s;}}\\f0 \\fs%u \\u%u?}", GetCharset(FontName), FontName.c_str(), Size*2, Char);

  SendMessage(Handle, EM_SETTEXTEX, reinterpret_cast<long>(&SetTextEx), reinterpret_cast<long>(Temp.c_str()));
}
//---------------------------------------------------------------------------
TParaFormat::TParaFormat(TIRichEdit *ARichEdit)
  : RichEdit(ARichEdit), TParaAttributes(ARichEdit)
{
}
//---------------------------------------------------------------------------
::PARAFORMAT2 TParaFormat::GetFormat() const
{
  ::PARAFORMAT2 Format;
  Format.cbSize = sizeof(Format);

  SendMessage(RichEdit->Handle, EM_GETPARAFORMAT, 0, reinterpret_cast<long>(&Format));
  return Format;
}
//---------------------------------------------------------------------------
TParaFormatAlignment TParaFormat::GetAlignment()
{
  return static_cast<TParaFormatAlignment>(GetFormat().wAlignment);
}
//---------------------------------------------------------------------------
void TParaFormat::SetAlignment(TParaFormatAlignment Value)
{
  ::PARAFORMAT2 Format;
  Format.cbSize = sizeof(Format);
  Format.wAlignment = Value;
  Format.dwMask = PFM_ALIGNMENT;
  SendMessage(RichEdit->Handle, EM_SETPARAFORMAT, 0, reinterpret_cast<long>(&Format));
}
//---------------------------------------------------------------------------

