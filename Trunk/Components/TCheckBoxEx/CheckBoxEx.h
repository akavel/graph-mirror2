//---------------------------------------------------------------------------
#ifndef CheckBoxExH
#define CheckBoxExH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TCheckBoxEx : public TCheckBox
{
private:
  void __fastcall CMTextChanged(TMessage &Message);
  void __fastcall CMFontChanged(TMessage &Message);

BEGIN_MESSAGE_MAP
  VCL_MESSAGE_HANDLER(CM_TEXTCHANGED, TMessage, CMTextChanged)
  VCL_MESSAGE_HANDLER(CM_FONTCHANGED, TMessage, CMFontChanged)
END_MESSAGE_MAP(TCheckBox)

protected:
  bool __fastcall CanAutoSize(int &NewWidth, int &NewHeight);

public:
  __fastcall TCheckBoxEx(TComponent* Owner);
__published:
  __property AutoSize;
};
//---------------------------------------------------------------------------
#endif
