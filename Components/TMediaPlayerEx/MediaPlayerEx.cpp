//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "MediaPlayerEx.h"
#include <digitalv.h>
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TMediaPlayerEx *)
{
  new TMediaPlayerEx(NULL);
}
//---------------------------------------------------------------------------
namespace Mediaplayerex
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TMediaPlayerEx)};
     RegisterComponents("IComp", classes, 0);
  }
}
//---------------------------------------------------------------------------
__fastcall TMediaPlayerEx::TMediaPlayerEx(TComponent* Owner)
  : TMediaPlayer(Owner), FOnSignal(NULL), FRepeat(false), FReverse(false), FOnHint(NULL),
    LastButton(static_cast<TMPBtnType>(0xFF))
{
}
//---------------------------------------------------------------------------
void __fastcall TMediaPlayerEx::MMSignal(TMessage &Message)
{
  if(OnSignal)
    OnSignal(this, Message.LParam);
}
//---------------------------------------------------------------------------
void TMediaPlayerEx::SetSignal(unsigned At, unsigned Interval)
{
  MCI_DGV_SIGNAL_PARMS SignalParm;
  SignalParm.dwCallback = reinterpret_cast<DWORD>(Handle);
  SignalParm.dwPosition = At;
  SignalParm.dwPeriod = Interval;
  SignalParm.dwUserParm = 0;

  DWORD Flags = MCI_WAIT | MCI_DGV_SIGNAL_AT | MCI_DGV_SIGNAL_POSITION;
  if(Interval != 0)
    Flags |= MCI_DGV_SIGNAL_EVERY;
  mciSendCommand(DeviceID, MCI_SIGNAL, Flags, reinterpret_cast<DWORD>(&SignalParm));
}
//---------------------------------------------------------------------------
void TMediaPlayerEx::RemoveSignal(unsigned At)
{
  MCI_DGV_SIGNAL_PARMS SignalParm;
  SignalParm.dwCallback = reinterpret_cast<DWORD>(Handle);
  SignalParm.dwUserParm = At;

  mciSendCommand(DeviceID, MCI_SIGNAL, MCI_WAIT | MCI_DGV_SIGNAL_CANCEL | MCI_DGV_SIGNAL_USERVAL, reinterpret_cast<DWORD>(&SignalParm));
}
//---------------------------------------------------------------------------
void TMediaPlayerEx::RemoveAllSignals()
{
  MCI_DGV_SIGNAL_PARMS SignalParm;
  SignalParm.dwCallback = reinterpret_cast<DWORD>(Handle);

  mciSendCommand(DeviceID, MCI_SIGNAL, MCI_WAIT | MCI_DGV_SIGNAL_CANCEL, reinterpret_cast<DWORD>(&SignalParm));
}
//---------------------------------------------------------------------------
void TMediaPlayerEx::Play()
{
  if(!Repeat && !Reverse)
  {
    TMediaPlayer::Play();
    return;
  }

  TMCI_Play_Parms PlayParm;

  //raise exception if device is not open

  DWORD Flags = 0;

  if(Notify)
    Flags |= MCI_NOTIFY;
  if(Wait)
    Flags != MCI_WAIT;
  if(Repeat)
    Flags |= MCI_DGV_PLAY_REPEAT;
  if(Reverse)
    Flags |= MCI_DGV_PLAY_REVERSE;

  PlayParm.dwCallback = reinterpret_cast<DWORD>(Handle);
  mciSendCommand(DeviceID, MCI_PLAY, Flags, reinterpret_cast<DWORD>(&PlayParm));
}
//---------------------------------------------------------------------------
void __fastcall TMediaPlayerEx::Click(TMPBtnType Button, bool &DoDefault)
{
  TMediaPlayer::Click(Button, DoDefault);
  if(DoDefault && Button == btPlay)
  {
    Play();
    DoDefault = false;
  }
}
//---------------------------------------------------------------------------
TMPBtnType TMediaPlayerEx::FindButton(unsigned X)
{
  unsigned VisibleButtonCount = 0;
  for(TMPBtnType Button = btPlay; Button <= btEject; Button = static_cast<TMPBtnType>(Button+1))
    if(VisibleButtons.Contains(Button))
      VisibleButtonCount++;

  unsigned ButtonWidth = (Width - 1) / VisibleButtonCount + 1;
  unsigned VisibleBtn = X / ButtonWidth;
  for(TMPBtnType Button = btPlay; Button <= btEject; Button = static_cast<TMPBtnType>(Button+1))
    if(VisibleButtons.Contains(Button))
      if(VisibleBtn-- == 0)
        return Button;
  return static_cast<TMPBtnType>(0xFF);      
}
//---------------------------------------------------------------------------
void __fastcall TMediaPlayerEx::WMMouseMove(TWMMouseMove &Message)
{
  if(OnHint)
  {
    TMPBtnType NewButton = FindButton(Message.XPos);
    if(NewButton != LastButton)
      OnHint(this, NewButton);
    LastButton = NewButton;
  }
  TMediaPlayer::Dispatch(&Message);
}
//---------------------------------------------------------------------------

