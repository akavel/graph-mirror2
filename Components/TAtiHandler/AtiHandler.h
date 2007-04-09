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

enum TAtiError
{
  aeNoError = 0,
  aeConnectionError,
  aeNotConnected,
  aeInvalidSize,
  aeNoStopByte,
  aeAckMissing,
  aeNackNoBuf,
  aeNackBadSeq,
  aeNackOverrun,
  aeBreakSyncFailed,
  aeInvalidFrame
};

class EAtiError : Exception
{
  TAtiError ErrorCode;
public:
  EAtiError(TAtiError AErrorCode, const AnsiString &Str) : Exception(Str), ErrorCode(AErrorCode) {}  
};

class TAtiHandler;
class TAtiThread;
typedef void __fastcall (__closure *TTelegramReceivedEvent)(TAtiHandler *Sender, const BYTE *Telegram, unsigned Size);
typedef void __fastcall (__closure *TSpeedChangedEvent)(TAtiHandler *Sender, unsigned Speed);
typedef void __fastcall (__closure *TErrorEvent)(TAtiHandler *Sender, TAtiError ErrorCode, const AnsiString &ErrorStr);

class PACKAGE TAtiHandler : public TComponent
{
  friend TAtiThread;

  AnsiString FPort;
  TTelegramReceivedEvent FOnTelegramReceived;
  TSpeedChangedEvent FOnSpeedChanged;
  TErrorEvent FOnError;

  class TSerialHandler *SerialHandler;
  TAtiThread *Thread;
  TTimer *Timer;
  BYTE TxSequence;
  BYTE RxSequence;
  std::vector<BYTE> RxBuffer;
  bool Breaking;

  void __fastcall SerialHandlerBreak(TObject *Sender);
  void __fastcall SerialHandlerDataReceived(TSerialHandler *Sender, const BYTE *Data, unsigned Size);
  void __fastcall Timeout(TObject *Sender);
  void HandleControlFrame();
  void BreakSync();
  void DoTelegramReceived(const BYTE *Telegram, unsigned Size);
  void DoError(TAtiError ErrorCode, const AnsiString &ErrorStr);
  template<typename TIter>
  static unsigned TAtiHandler::CalcCrc(TIter Begin, TIter End);
  static void DebugLog(const AnsiString &Str);
  TStrings* GetSerialPorts();
  bool GetConnected();

public:
  __fastcall TAtiHandler(TComponent* Owner);
  void Connect();
  void Disconnect();
  void ChangeSpeed(unsigned Speed);
  void SendTelegram(const BYTE *Telegram, unsigned Size, bool UsesAck = false);

  __property TStrings* SerialPorts = {read=GetSerialPorts};
  __property bool Connected = {read=GetConnected};
  
__published:
  __property AnsiString Port = {read=FPort, write=FPort};

  __property TTelegramReceivedEvent OnTelegramReceived = {read=FOnTelegramReceived, write=FOnTelegramReceived, default=NULL};
  __property TSpeedChangedEvent OnSpeedChanged = {read=FOnSpeedChanged, write=FOnSpeedChanged, default=NULL};
  __property TErrorEvent OnError = {read=FOnError, write=FOnError, default=NULL};
};
//---------------------------------------------------------------------------
#endif
