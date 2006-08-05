//---------------------------------------------------------------------------
#ifndef MediaPlayerExH
#define MediaPlayerExH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <MPlayer.hpp>
//---------------------------------------------------------------------------
class TMediaPlayerEx;
typedef void __fastcall (__closure *TSignalEvent)(TMediaPlayerEx* Sender, unsigned Position);
typedef void __fastcall (__closure *TMPHintEvent)(TMediaPlayerEx* Sender, TMPBtnType Button);

class PACKAGE TMediaPlayerEx : public TMediaPlayer
{
private:
  TSignalEvent FOnSignal;
  TMPHintEvent FOnHint;
  TMPBtnType LastButton;
  bool FRepeat;
  bool FReverse;

  TMPBtnType FindButton(unsigned X);

  void __fastcall MMSignal(TMessage &Message);
  void __fastcall WMMouseMove(TWMMouseMove &Message);
  DYNAMIC void __fastcall Click(TMPBtnType Button, bool &DoDefault);

BEGIN_MESSAGE_MAP
  VCL_MESSAGE_HANDLER(MM_MCISIGNAL, TMessage, MMSignal)
  VCL_MESSAGE_HANDLER(WM_MOUSEMOVE, TWMMouseMove, WMMouseMove)
END_MESSAGE_MAP(TMediaPlayer)

protected:
public:
  __fastcall TMediaPlayerEx(TComponent* Owner);
  void SetSignal(unsigned At, unsigned Interval = 0);
  void RemoveSignal(unsigned At);
  void RemoveAllSignals();
  void Play();

__published:
  __property bool Repeat = {read=FRepeat, write=FRepeat, default=false};
  __property bool Reverse = {read=FReverse, write=FReverse, default=false};
  __property TSignalEvent OnSignal = {read=FOnSignal, write=FOnSignal, default=NULL};
  __property TMPHintEvent OnHint = {read=FOnHint, write=FOnHint, default=NULL};
};
//---------------------------------------------------------------------------
#endif
