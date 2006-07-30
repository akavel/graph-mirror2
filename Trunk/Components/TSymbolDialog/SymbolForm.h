//---------------------------------------------------------------------------

#ifndef SymbolFormH
#define SymbolFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include "FocusPanel.h"
#include "TntStdCtrls.hpp"
//---------------------------------------------------------------------------
typedef __declspec(dllimport) int (FAR WINAPI *TGetUNameFunc)(DWORD,LPVOID);
typedef DWORD (WINAPI *TGetFontUnicodeRanges)(HDC, LPGLYPHSET);
typedef DWORD (WINAPI *TGetGlyphIndices)(HDC, wchar_t*, int, LPWORD, DWORD);

const int ColCount = 20;
const int RowCount = 12;

class TSymbolFrm : public TForm
{
__published:	// IDE-managed Components
  TTntButton *Button1;
  TTntButton *Button2;
  TComboBox *ComboBox1;
  TStatusBar *StatusBar1;
  TImage *Image2;
  TFocusPanel *FocusPanel1;
  TImage *Image1;
  TTntLabel *Label1;
  TScrollBar *ScrollBar1;
  TTntLabel *Label2;
  TComboBox *ComboBox2;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall Image1DblClick(TObject *Sender);
  void __fastcall FocusPanel1KeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
  void __fastcall ComboBox1Select(TObject *Sender);
  void __fastcall ComboBox1Exit(TObject *Sender);
  void __fastcall ScrollBar1Change(TObject *Sender);
private:	// User declarations
  HMODULE hUNameDll;
  HMODULE hGdi32Dll;
  TGetUNameFunc pGetUNameFunc;
  TGetFontUnicodeRanges pGetFontUnicodeRanges;
  TGetGlyphIndices pGetGlyphIndices;
  int OldItemIndex;
  bool ShowUnicode;
  wchar_t BeginSymbol;
  wchar_t EndSymbol;
  wchar_t Selected;
  wchar_t CharTable[ColCount][RowCount];

  void UpdateImage();
  bool SetSelected(unsigned Row, unsigned Col);

public:		// User declarations
  wchar_t GetSelected() {return Selected;}
  __fastcall TSymbolFrm(TComponent* Owner, bool AShowUnicode);
  __fastcall ~TSymbolFrm();
};
//---------------------------------------------------------------------------
#endif
