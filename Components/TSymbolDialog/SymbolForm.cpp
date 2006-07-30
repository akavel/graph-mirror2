//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Config.h"
#include "SymbolForm.h"
#include "CompRes.h"
#include <vector>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "FocusPanel"
#pragma resource "CompRes.res"
#pragma link "TntStdCtrls"
#pragma resource "*.dfm"

const wchar_t FirstSymbol = 32;
const int Delta = 20;
const int FontSize = 12;
const int PreviewFontSize = 30;

struct TSubset
{
  unsigned First;
  unsigned Last;
  const char *Name;
};

TSubset Subsets[] = {
{0x0020, 0x007A, "Basic Latin"},
{0x00A0, 0x00FF, "Latin-1"},
{0x0100, 0x017F, "Latin Extended-A"},
{0x0180, 0x024F, "Latin Extended-B"},
{0x0250, 0x02AF, "IPA Extensions"},
{0x02B0, 0x02FF, "Spacing Modifier Letters"},
{0x0300, 0x036F, "Combining Diacritical Marks"},
{0x0370, 0x03FF, "Greek and Coptic"},
{0x0000, 0x0000, ""},
{0x0400, 0x052F, "Cyrillic"},
{0x0530, 0x058F, "Armenian"},
{0x0590, 0x05FF, "Basic Hebrew"},
{0x0000, 0x0000, ""},
{0x0600, 0x06FF, "Basic Arabic"},
{0x0000, 0x0000, ""},
{0x0900, 0x097F, "Devanagari"},
{0x0980, 0x09FF, "Bengali"},
{0x0A00, 0x0A7F, "Gurmukhi"},
{0x0A80, 0x0AFF, "Gujarati"},
{0x0B00, 0x0B7F, "Oriya"},
{0x0B80, 0x0BFF, "Tamil"},
{0x0C00, 0x0C7F, "Telugu"},
{0x0C80, 0x0CFF, "Kannada"},
{0x0D00, 0x0D7F, "Malayalam"},
{0x0E00, 0x0E7F, "Thai"},
{0x0E80, 0x0EFF, "Lao"},
{0x10A0, 0x10FF, "Georgian"},
{0x0000, 0x0000, ""},
{0x1100, 0x11FF, "Hangul Jamo"},
{0x1E00, 0x1EFF, "Latin Extended Additional"},
{0x1F00, 0x1FFF, "Greek Extended"},
{0x2000, 0x206F, "General Punctuation"},
{0x2070, 0x209F, "Subscripts and Superscripts"},
{0x20A0, 0x20CF, "Currency Symbols"},
{0x20D0, 0x20FF, "Combining Diacritical Marks for Symbols"},
{0x2100, 0x214F, "Letter-like Symbols"},
{0x2150, 0x218F, "Number Forms"},
{0x2190, 0x297F, "Arrows"},
{0x2200, 0x29FF, "Mathematical Operators"},
{0x2300, 0x23FF, "Miscellaneous Technical"},
{0x2400, 0x243F, "Control Pictures"},
{0x2440, 0x245F, "Optical Character Recognition"},
{0x2460, 0x24FF, "Enclosed Alphanumerics"},
{0x2500, 0x257F, "Box Drawing"},
{0x2580, 0x259F, "Block Elements"},
{0x25A0, 0x25FF, "Geometric Shapes"},
{0x2600, 0x26FF, "Miscellaneous Symbols"},
{0x2700, 0x27BF, "Dingbats"},
{0x3000, 0x303F, "Chinese, Japanese, and Korean (CJK) Symbols and Punctuation"},
{0x3040, 0x309F, "Hiragana"},
{0x30A0, 0x31FF, "Katakana"},
{0x3100, 0x318F, "Bopomofo"},
{0x3130, 0x318F, "Hangul Compatibility Jamo"},
{0x0000, 0x0000, ""},
{0x3200, 0x32FF, "Enclosed CJK Letters and Months"},
{0x3300, 0x33FF, "CJK Compatibility"},
{0xAC00, 0xD7A3, "Hangul"},
{0xD800, 0xDFFF, "Surrogates"},
{0x0000, 0x0000, ""},
{0x0000, 0x0000, ""}, //Bit 59: This is actually several ranges
{0xE000, 0xF8FF, "Private Use Area"},
{0xF900, 0xFAFF, "CJK Compatibility Ideographs"},
{0xFB00, 0xFB4F, "Alphabetic Presentation Forms"},
{0xFB50, 0xFDFF, "Arabic Presentation Forms-A"},
{0xFE20, 0xFE2F, "Combining Half Marks"},
{0xFE30, 0xFE4F, "CJK Compatibility Forms"},
{0xFE50, 0xFE6F, "Small Form Variants"},
{0xFE70, 0xFEFE, "Arabic Presentation Forms-B"},
{0xFF00, 0xFFEF, "Halfwidth and Fullwidth Forms"},
{0xFFF0, 0xFFFF, "Specials"},
{0x0F00, 0x0FFF, "Tibetan"},
{0x0700, 0x074F, "Syriac"},
{0x0780, 0x07BF, "Thaana"},
{0x0D80, 0x0DFF, "Sinhala"},
{0x1000, 0x109F, "Myanmar"},
{0x1200, 0x12BF, "Ethiopic"},
{0x13A0, 0x13FF, "Cherokee"},
{0x1400, 0x167F, "Canadian Aboriginal Syllabics"},
{0x1680, 0x169F, "Ogham"},
{0x16A0, 0x16FF, "Runic"},
{0x1780, 0x17FF, "Khmer"}, //Khmer Symbols are missing
{0x1800, 0x18AF, "Mongolian"},
{0x2800, 0x28FF, "Braille"},
{0xA000, 0xA4CF, "Yi"},
{0x1700, 0x177F, "Tagalog, Hanunoo, Buhid, Tagbanwa"},
{0x10300, 0x1032F, "Old Italic"},
{0x10330, 0x1034F, "Gothic"},
{0x10440, 0x1044F, "Deseret"},
{0x1D000, 0x1D1FF, "Musical Symbols"},
{0x1D400, 0x1D7FF, "Mathematical Alphanumeric Symbols"},
};
//---------------------------------------------------------------------------
__fastcall TSymbolFrm::TSymbolFrm(TComponent* Owner, bool AShowUnicode)
  : TForm(Owner), Selected(FirstSymbol), hUNameDll(NULL), pGetUNameFunc(NULL), OldItemIndex(0), ShowUnicode(AShowUnicode),
    BeginSymbol(0), EndSymbol(0), hGdi32Dll(NULL), pGetFontUnicodeRanges(NULL), pGetGlyphIndices(NULL)
{
  Caption = LoadStr(RES_SYMBOL_BOX_CAPTION);
  Label1->Caption = "&" + LoadStr(RES_FONT_NAME);
  Label2->Caption = "&" + LoadStr(RES_FONT_SUBSET);
  Button1->Caption = LoadStr(RES_BUTTON_CLOSE);
  Button2->Caption = LoadStr(RES_BUTTON_INSERT);
  ComboBox1->Left = Label1->Left + Label1->Width + 5;

  hUNameDll = LoadLibrary("GETUNAME.DLL");
  if(hUNameDll)
    pGetUNameFunc = reinterpret_cast<TGetUNameFunc>(GetProcAddress(hUNameDll, "GetUName"));

  hGdi32Dll = LoadLibrary("GDI32.DLL");
  if(hGdi32Dll)
  {
    pGetFontUnicodeRanges = reinterpret_cast<TGetFontUnicodeRanges>(GetProcAddress(hGdi32Dll, "GetFontUnicodeRanges"));
    pGetGlyphIndices = reinterpret_cast<TGetGlyphIndices>(GetProcAddress(hGdi32Dll, "GetGlyphIndicesW"));
  }

  //Add list of fonts; Take care of duplicates, that for some strange reason has happened
  for(int I = 0; I < Screen->Fonts->Count; I++)
    if(ComboBox1->Items->IndexOf(Screen->Fonts->Strings[I]) == -1)
      ComboBox1->Items->Add(Screen->Fonts->Strings[I]);

  if(!ShowUnicode)
  {
    Label2->Visible = false;
    ComboBox2->Visible = false;
    ScrollBar1->Visible = false;
  }

  FocusPanel1->DoubleBuffered = true;

  for(unsigned I = 0; I < sizeof(Subsets)/sizeof(Subsets[0]); I++)
    ComboBox2->Items->Add(Subsets[I].Name);
}
//---------------------------------------------------------------------------
__fastcall TSymbolFrm::~TSymbolFrm()
{
  if(hUNameDll)
    FreeLibrary(hUNameDll);
  if(hGdi32Dll)
    FreeLibrary(hGdi32Dll);
}
//---------------------------------------------------------------------------
void TSymbolFrm::UpdateImage()
{
  BeginSymbol = ScrollBar1->Position * ColCount + FirstSymbol;
  EndSymbol = ShowUnicode ? ColCount * RowCount + BeginSymbol : 256;

  TCanvas *Canvas = Image1->Canvas;
  int Width = Image1->Width;
  int Height = Image1->Height;

  Canvas->Brush->Color = clWhite;
  Canvas->Pen->Color = clBlack;
  Canvas->Font->Name = ComboBox1->Text;
  Canvas->Font->Size = FontSize;
  Canvas->Font->Color = clBlack;

  //Clear image
  Canvas->FillRect(Image1->ClientRect);

  //Draw symbols
  wchar_t Symbol = BeginSymbol;
  WideString Str(L' ');
  for(int Row = 0; Row < RowCount; Row++)
    for(int Col = 0; Col < ColCount && Symbol < EndSymbol; Col++, Symbol++)
    {
      Str[1] = Symbol;
      TRect Rect(Col * Delta, Row * Delta, (Col+1) * Delta, (Row+1) * Delta);
      DrawTextW(Canvas->Handle, Str.c_bstr(), 1, &Rect, DT_CENTER | DT_VCENTER);
    }

  //Draw horizontal and vertical lines
  for(int X = 0; X < Width; X += Delta)
  {
    Canvas->MoveTo(X, 0);
    Canvas->LineTo(X, Height);
  }

  for(int Y = 0; Y < Height; Y += Delta)
  {
    Canvas->MoveTo(0, Y);
    Canvas->LineTo(Width, Y);
  }

  int Col = (Selected-FirstSymbol) % ColCount;
  int Row = (Selected-BeginSymbol) / ColCount;
  if(Selected < BeginSymbol)
    Row = 0, Selected = BeginSymbol + Col;
  else if(Row >= RowCount)
    Row = RowCount - 1, Selected = BeginSymbol + Row * ColCount + Col;

  //Draw selected symbol
  Canvas->Brush->Color = (TColor)0x006B2408; //A dark blue color
  TRect SelectedRect(Col * Delta + 1, Row * Delta + 1, (Col+1) * Delta, (Row+1) * Delta);
  Canvas->FillRect(SelectedRect);
  Canvas->Font->Color = clWhite;
  Str[1] = Selected;
  DrawTextW(Canvas->Handle, Str.c_bstr(), 1, &SelectedRect, DT_CENTER | DT_VCENTER);
  if(FocusPanel1->Focused())
    Canvas->DrawFocusRect(SelectedRect);

  //Show large preview
  Image2->Canvas->Brush->Color = clBtnFace;                    
  Image2->Canvas->FillRect(Image2->ClientRect);
  Image2->Canvas->Font->Name = ComboBox1->Text;
  Image2->Canvas->Font->Size = PreviewFontSize;
  TRect Rect = Image2->ClientRect;
  DrawTextW(Image2->Canvas->Handle, Str.c_bstr(), 1, &Rect, DT_CENTER | DT_VCENTER);

  //Show character information in status bar
  if(pGetUNameFunc)
  {
    int Charset = GetTextCharset(Canvas->Handle);
    if(Charset != SYMBOL_CHARSET)
    {
      wchar_t CharInfo[200];
      wchar_t Char = WideString(AnsiString((char)Selected))[1];
      pGetUNameFunc(Char, CharInfo);
      StatusBar1->Panels->Items[0]->Text = WideString(CharInfo);
    }
    else
      StatusBar1->Panels->Items[0]->Text = "";
  }

  //how character number in status bar
  StatusBar1->Panels->Items[1]->Text = "U+" + IntToHex(Selected, 4) + " (" + (int)Selected + ")";

  Image1->Repaint();
  Image2->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TSymbolFrm::FormShow(TObject *Sender)
{
  OldItemIndex = ComboBox1->ItemIndex;
  UpdateImage();
}
//---------------------------------------------------------------------------
void __fastcall TSymbolFrm::Image1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if(Button == mbLeft)
    if(SetSelected(X / Delta, Y / Delta))
    {
      UpdateImage();
      if(Shift.Contains(ssDouble))
        Button2->Click();
    }
}
//---------------------------------------------------------------------------
void __fastcall TSymbolFrm::Image1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  if(Shift.Contains(ssLeft))
    if(SetSelected(X / Delta, Y / Delta))
      UpdateImage();
}
//---------------------------------------------------------------------------
void __fastcall TSymbolFrm::Button1Click(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TSymbolFrm::Image1DblClick(TObject *Sender)
{
  Button2->Click();
}
//---------------------------------------------------------------------------
void __fastcall TSymbolFrm::FocusPanel1KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  int Row = (Selected - BeginSymbol) / ColCount;
  int Col = (Selected - BeginSymbol) % ColCount;
  switch(Key)
  {
    case VK_LEFT:
      if(Col > 0)
        Col--;
      else if(Row > 0)
        Row--, Col = ColCount-1;
      else
        ScrollBar1->Position = ScrollBar1->Position - ScrollBar1->SmallChange;
      break;

    case VK_RIGHT:
      if(Col < ColCount-1)
        Col++;
      else if(Row < RowCount-1)
        Row++, Col = 0;
      else
        ScrollBar1->Position = ScrollBar1->Position + ScrollBar1->SmallChange;
      break;

    case VK_UP:
      if(Row > 0)
        Row--;
      else
        ScrollBar1->Position = ScrollBar1->Position - ScrollBar1->SmallChange;
      break;

    case VK_DOWN:
      if(Row <  RowCount-1)
        Row++;
      else
        ScrollBar1->Position = ScrollBar1->Position + ScrollBar1->SmallChange;
      break;

    case VK_PRIOR:
      ScrollBar1->Position = ScrollBar1->Position - ScrollBar1->LargeChange;
      break;

    case VK_NEXT:
      ScrollBar1->Position = ScrollBar1->Position + ScrollBar1->LargeChange;
      break;

    case VK_HOME:
      ScrollBar1->Position = ScrollBar1->Min;
      Row = 0;
      Col = 0;
      break;

    case VK_END:
      ScrollBar1->Position = ScrollBar1->Max;
      Row = RowCount - 1;
      Col = ColCount - 1;
      break;
  }

  if(SetSelected(Col, Row))
    UpdateImage();
}
//---------------------------------------------------------------------------
void __fastcall TSymbolFrm::ComboBox1Select(TObject *Sender)
{
  OldItemIndex = ComboBox1->ItemIndex;
  UpdateImage();

  //Symbol charset only has characters up to '\xFF'
  int Charset = GetTextCharset(Image1->Canvas->Handle);
  ScrollBar1->Enabled = Charset != SYMBOL_CHARSET;
  ComboBox2->Enabled = Charset != SYMBOL_CHARSET;

  FONTSIGNATURE FontSignature;
  Charset = GetTextCharsetInfo(Image1->Canvas->Handle, &FontSignature, 0);

  if(pGetGlyphIndices)
  {
    WORD Array[10];
    wchar_t Input[] = {L'A', 0x1EA2, 0};
    unsigned Result = pGetGlyphIndices(Image1->Canvas->Handle, Input, 1, Array, GGI_MARK_NONEXISTING_GLYPHS);
  }

  if(pGetFontUnicodeRanges)
  {
    std::vector<BYTE> Data(pGetFontUnicodeRanges(Image1->Canvas->Handle, NULL));
    GLYPHSET *Glyphset = reinterpret_cast<GLYPHSET*>(&Data[0]);
    pGetFontUnicodeRanges(Image1->Canvas->Handle, Glyphset);
  }
}
//---------------------------------------------------------------------------
void __fastcall TSymbolFrm::ComboBox1Exit(TObject *Sender)
{
  ComboBox1->ItemIndex = OldItemIndex;
  ComboBox1->Text = ComboBox1->Items->Strings[OldItemIndex];
}
//---------------------------------------------------------------------------
void __fastcall TSymbolFrm::ScrollBar1Change(TObject *Sender)
{
  UpdateImage();
}
//---------------------------------------------------------------------------
bool TSymbolFrm::SetSelected(unsigned Col, unsigned Row)
{
  wchar_t NewSelected = Row * ColCount + Col + BeginSymbol;
  if(NewSelected != Selected && NewSelected >= BeginSymbol && NewSelected < EndSymbol)
  {
    Selected = NewSelected;
    return true;
  }
  return false;      
}
//---------------------------------------------------------------------------

