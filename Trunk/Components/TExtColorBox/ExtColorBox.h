//---------------------------------------------------------------------------
#ifndef ExtColorBoxH
#define ExtColorBoxH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TExtColorBox;
typedef void __fastcall (__closure *TPickColorEvent)(TExtColorBox* Sender, TColor &Color);
enum TColorDialogType {cdtColorDialog, cdtColorSelect};

class PACKAGE TExtColorBox : public TColorBox
{
private:
  TColorDialogType FColorDialogType;
  TPickColorEvent FOnPickColor;
  int FDroppedWidth;

  void __fastcall SetDroppedWidth(int AWidth);
  void __fastcall SetExtendedUI(bool Value);
  bool __fastcall GetExtendedUI();

protected:
  bool __fastcall PickCustomColor(void);
  void __fastcall Loaded(void);

public:
  __fastcall TExtColorBox(TComponent* Owner);

__published:
  __property TColorDialogType ColorDialogType = {read=FColorDialogType, write=FColorDialogType, default=cdtColorSelect};
  __property TPickColorEvent OnPickColor = {read=FOnPickColor, write=FOnPickColor, default=NULL};
  __property int DroppedWidth = {read=FDroppedWidth, write=SetDroppedWidth, default=0};
  __property bool ExtendedUI = {read=GetExtendedUI, write=SetExtendedUI};
};
//---------------------------------------------------------------------------
#endif
