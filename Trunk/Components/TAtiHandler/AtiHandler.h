//---------------------------------------------------------------------------
#ifndef AtiHandlerH
#define AtiHandlerH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <ExtCtrls.hpp>
#include <vector>
//---------------------------------------------------------------------------

const unsigned MaxTelegramSize = 240;

class TAtiHandler;
class TAtiThread;
typedef void __fastcall (__closure *TTelegramReceivedEvent)(TAtiHandler *Sender, const BYTE *Telegram, unsigned Size);
typedef void __fastcall (__closure *TSpeedChangedEvent)(TAtiHandler *Sender, unsigned Speed);

class PACKAGE TAtiHandler : public TComponent
{
  friend TAtiThread;
  
  AnsiString FPort;
  TTelegramReceivedEvent FOnTelegramReceived;
  TSpeedChangedEvent FOnSpeedChanged;

  class TSerialHandler *SerialHandler;
  TAtiThread *Thread;
  TTimer *Timer;
  unsigned Sequence;
  std::vector<BYTE> Buffer;
  bool Breaking;

  void __fastcall SerialHandlerBreak(TObject *Sender);
  void __fastcall SerialHandlerDataReceived(TSerialHandler *Sender, const BYTE *Data, unsigned Size);
  void __fastcall Timeout(TObject *Sender);
  void HandleControlFrame();
  void BreakSync();
  void DoTelegramReceived(const BYTE *Telegram, unsigned Size);

public:
  __fastcall TAtiHandler(TComponent* Owner);
  void Connect();
  void Disconnect();
  void ChangeSpeed(unsigned Speed);
  void SendTelegram(const BYTE *Telegram, unsigned Size, bool UsesAck = false);

__published:
  __property AnsiString Port = {read=FPort, write=FPort};

  __property TTelegramReceivedEvent OnTelegramReceived = {read=FOnTelegramReceived, write=FOnTelegramReceived, default=NULL};
  __property TSpeedChangedEvent OnSpeedChanged = {read=FOnSpeedChanged, write=FOnSpeedChanged, default=NULL};
};
//---------------------------------------------------------------------------
#endif
 