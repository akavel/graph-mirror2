//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Config.h"
#include "AtiHandler.h"
#include "SerialHandler.h"
#include "AtiThread.h"
#include <fstream>
#pragma package(smart_init)
//---------------------------------------------------------------------------
static const WORD CrcTable[256] =
{
  0x0000,  0x1021,  0x2042,  0x3063,  0x4084,  0x50a5,  0x60c6,  0x70e7,
  0x8108,  0x9129,  0xa14a,  0xb16b,  0xc18c,  0xd1ad,  0xe1ce,  0xf1ef,
  0x1231,  0x0210,  0x3273,  0x2252,  0x52b5,  0x4294,  0x72f7,  0x62d6,
  0x9339,  0x8318,  0xb37b,  0xa35a,  0xd3bd,  0xc39c,  0xf3ff,  0xe3de,
  0x2462,  0x3443,  0x0420,  0x1401,  0x64e6,  0x74c7,  0x44a4,  0x5485,
  0xa56a,  0xb54b,  0x8528,  0x9509,  0xe5ee,  0xf5cf,  0xc5ac,  0xd58d,
  0x3653,  0x2672,  0x1611,  0x0630,  0x76d7,  0x66f6,  0x5695,  0x46b4,
  0xb75b,  0xa77a,  0x9719,  0x8738,  0xf7df,  0xe7fe,  0xd79d,  0xc7bc,
  0x48c4,  0x58e5,  0x6886,  0x78a7,  0x0840,  0x1861,  0x2802,  0x3823,
  0xc9cc,  0xd9ed,  0xe98e,  0xf9af,  0x8948,  0x9969,  0xa90a,  0xb92b,
  0x5af5,  0x4ad4,  0x7ab7,  0x6a96,  0x1a71,  0x0a50,  0x3a33,  0x2a12,
  0xdbfd,  0xcbdc,  0xfbbf,  0xeb9e,  0x9b79,  0x8b58,  0xbb3b,  0xab1a,
  0x6ca6,  0x7c87,  0x4ce4,  0x5cc5,  0x2c22,  0x3c03,  0x0c60,  0x1c41,
  0xedae,  0xfd8f,  0xcdec,  0xddcd,  0xad2a,  0xbd0b,  0x8d68,  0x9d49,
  0x7e97,  0x6eb6,  0x5ed5,  0x4ef4,  0x3e13,  0x2e32,  0x1e51,  0x0e70,
  0xff9f,  0xefbe,  0xdfdd,  0xcffc,  0xbf1b,  0xaf3a,  0x9f59,  0x8f78,
  0x9188,  0x81a9,  0xb1ca,  0xa1eb,  0xd10c,  0xc12d,  0xf14e,  0xe16f,
  0x1080,  0x00a1,  0x30c2,  0x20e3,  0x5004,  0x4025,  0x7046,  0x6067,
  0x83b9,  0x9398,  0xa3fb,  0xb3da,  0xc33d,  0xd31c,  0xe37f,  0xf35e,
  0x02b1,  0x1290,  0x22f3,  0x32d2,  0x4235,  0x5214,  0x6277,  0x7256,
  0xb5ea,  0xa5cb,  0x95a8,  0x8589,  0xf56e,  0xe54f,  0xd52c,  0xc50d,
  0x34e2,  0x24c3,  0x14a0,  0x0481,  0x7466,  0x6447,  0x5424,  0x4405,
  0xa7db,  0xb7fa,  0x8799,  0x97b8,  0xe75f,  0xf77e,  0xc71d,  0xd73c,
  0x26d3,  0x36f2,  0x0691,  0x16b0,  0x6657,  0x7676,  0x4615,  0x5634,
  0xd94c,  0xc96d,  0xf90e,  0xe92f,  0x99c8,  0x89e9,  0xb98a,  0xa9ab,
  0x5844,  0x4865,  0x7806,  0x6827,  0x18c0,  0x08e1,  0x3882,  0x28a3,
  0xcb7d,  0xdb5c,  0xeb3f,  0xfb1e,  0x8bf9,  0x9bd8,  0xabbb,  0xbb9a,
  0x4a75,  0x5a54,  0x6a37,  0x7a16,  0x0af1,  0x1ad0,  0x2ab3,  0x3a92,
  0xfd2e,  0xed0f,  0xdd6c,  0xcd4d,  0xbdaa,  0xad8b,  0x9de8,  0x8dc9,
  0x7c26,  0x6c07,  0x5c64,  0x4c45,  0x3ca2,  0x2c83,  0x1ce0,  0x0cc1,
  0xef1f,  0xff3e,  0xcf5d,  0xdf7c,  0xaf9b,  0xbfba,  0x8fd9,  0x9ff8,
  0x6e17,  0x7e36,  0x4e55,  0x5e74,  0x2e93,  0x3eb2,  0x0ed1,  0x1ef0
};

const unsigned PT_ATI_CONF1 = 0x01;
const unsigned NEW_SPEED = 0x00;

enum
{
  SPEED_REQUEST     = 0x00,
  SPEED_CONFIRM     = 0x01,
  SPEED_NO_CONFIRM  = 0x02
};
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TAtiHandler *)
{
  new TAtiHandler(NULL);
}
//---------------------------------------------------------------------------
namespace Atihandler
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TAtiHandler)};
     RegisterComponents("IComp", classes, 0);
  }
}
//---------------------------------------------------------------------------
template<typename TIter>
unsigned TAtiHandler::CalcCrc(TIter Begin, TIter End)
{
  WORD Crc = 0;
  for(TIter Iter = Begin; Iter != End; ++Iter)
    Crc = (Crc << 8) ^ CrcTable[(Crc >> 8) ^ *Iter];
  return Crc;
}
//---------------------------------------------------------------------------
__fastcall TAtiHandler::TAtiHandler(TComponent* Owner)
  : TComponent(Owner), FPort("COM1"), SerialHandler(new TSerialHandler(this)), Timer(new TTimer(this)), Breaking(false),
    FOnTelegramReceived(NULL), FOnSpeedChanged(NULL), FOnError(NULL)
{
  SerialHandler->OnBreak = &SerialHandlerBreak;
  SerialHandler->OnDataReceived = &SerialHandlerDataReceived;
  SerialHandler->Synchronized = false;
  Timer->OnTimer = &Timeout;
}
//---------------------------------------------------------------------------
void TAtiHandler::Connect()
{
  try
  {
    if(SerialHandler->Connected)
      return;

    SerialHandler->Port = Port;
    SerialHandler->Speed = 19200;
    SerialHandler->Parity = pbNoParity;
    SerialHandler->StopBits = sbOneStopBit;
    SerialHandler->ByteSize = 8;

    Thread = new TAtiThread(this);
    SerialHandler->Connect();
    SerialHandler->SetDTR();
    SerialHandler->SetRTS();
    BreakSync();
  }
  catch(...)
  {
    if(Thread)
      Thread->PostMessage(atmTerminate);  //The thread will delete itself
    Thread = NULL;
    throw EAtiError(aeConnectionError, "Connection failed");
  }
}
//---------------------------------------------------------------------------
void TAtiHandler::Disconnect()
{
  if(SerialHandler->Connected)
  {
    SerialHandler->Disconnect();
    Thread->PostMessage(atmTerminate);  //The thread will delete itself
    Thread = NULL;
  }
}
//---------------------------------------------------------------------------
void TAtiHandler::SendTelegram(const BYTE *Telegram, unsigned Size, bool UsesAck)
{
  if(!SerialHandler->Connected)
    throw EAtiError(aeNotConnected, "Not connected");
  if(Size > MaxTelegramSize || Size == 0)
    throw EAtiError(aeInvalidSize, "Invalid size in telegram");

  BYTE Data[MaxTelegramSize + 6];

  Data[0] = 0x81;
  Data[1] = (TxSequence << 4) | (UsesAck ? 0x0F : 0x07);
  Data[2] = Size;
  memcpy(Data+3, Telegram, Size);
  unsigned Crc = CalcCrc(Data+1, Data + Size + 3);
  Data[Size + 3] = Crc >> 8;
  Data[Size + 4] = Crc;
  Data[Size + 5] = 0x00;

  if(++TxSequence == 8)
    TxSequence = 0;
  SerialHandler->WriteBuffer(Data, Size + 6);

  std::vector<char> Text((Size+6)*2+1);
  BinToHex((char*)Data, &Text[0], Size + 6);
  AnsiString Str = "Send: ";
  Str += &Text[0];
  DebugLog(Str);
}
//---------------------------------------------------------------------------
void __fastcall TAtiHandler::SerialHandlerBreak(TObject *Sender)
{
  DebugLog("Break detected");
  Timer->Enabled = false;
  if(Breaking)
    DebugLog("Break off!");
  SerialHandler->ClearBreak();
  Breaking = false;
  Sleep(200);
  SerialHandler->Purge();
  TxSequence = 0;
  RxSequence = 0;
}
//---------------------------------------------------------------------------
void __fastcall TAtiHandler::SerialHandlerDataReceived(TSerialHandler *Sender, const BYTE *Data, unsigned Size)
{
  if(Breaking)
    return;

  std::vector<char> Text(Size*2+1);
  BinToHex((char*)Data, &Text[0], Size);
  AnsiString Str = "Data received: ";
  Str += &Text[0];
  DebugLog(Str.c_str());

  RxBuffer.insert(RxBuffer.end(), Data, Data + Size);

  while(RxBuffer.size() >= 3)
  {
    if(RxBuffer[0] != 0x81)
    {
      DebugLog("Start byte missing. Scanning for start byte...");

      //Start byte not found. Search for start byte and erase everything before
      std::vector<BYTE>::iterator Iter = std::find(RxBuffer.begin(), RxBuffer.end() - 2, 0x81);
      RxBuffer.erase(RxBuffer.begin(), Iter);
      continue;
    }

    if(RxBuffer[1] & 0x80)
    {
      if(RxBuffer[2] != 0x00)
      {
        //Stop byte missing. Discard frame.
        DebugLog("Stop byte missing!");
        RxBuffer.erase(RxBuffer.begin(), RxBuffer.begin() + 3);
        return;
      }

      //Control frame
      HandleControlFrame();
    }
    else
    {
      //Data frame
      unsigned Count = RxBuffer[2];
      //Size not received yet
      if(RxBuffer.size() < Count + 6)
        return;

      if(RxBuffer[Count + 5] != 0)
      {
        //Stop byte missing. Discard frame
        DebugLog("Stop byte missing!");
        RxBuffer.erase(RxBuffer.begin(), RxBuffer.begin() + Count + 6);
        return;
      }

      //Telegram received
      unsigned Crc = (RxBuffer[Count + 3] << 8) | (RxBuffer[Count + 4]);
      if(Crc != CalcCrc(RxBuffer.begin() + 1, RxBuffer.begin() + Count + 3))
      {
        DebugLog("CRC error!");
        BYTE TxData[3] = {0x81, 0xF1, 0x00}; //Send NACK (CRC error)
        Sender->WriteBuffer(TxData, sizeof(TxData));
        RxBuffer.erase(RxBuffer.begin(), RxBuffer.begin() + Count + 6);
        return;
      }

      //Send Ack if requested
      if(RxBuffer[1] & 0x08)
      {
        BYTE TxData[3] = {0x81, 0xF0, 0x00};
        Sender->WriteBuffer(TxData, sizeof(TxData));
        DebugLog("Send: ACK!");
      }

      BYTE *Telegram = new BYTE[Count];
      std::copy(RxBuffer.begin() + 3, RxBuffer.begin() + Count + 3, Telegram);
      Thread->PostMessage(atmDataReceived, reinterpret_cast<int>(Telegram), Count);
      RxBuffer.erase(RxBuffer.begin(), RxBuffer.begin() + Count + 6);
    }
  }
}
//---------------------------------------------------------------------------
void __fastcall TAtiHandler::Timeout(TObject *Sender)
{
  Timer->Enabled = false;
  if(Breaking)
  {
    DoError(aeBreakSyncFailed, "Break sync failed!");
    DebugLog("Break off!");
    SerialHandler->ClearBreak();
  }
}
//---------------------------------------------------------------------------
void TAtiHandler::HandleControlFrame()
{
  switch(RxBuffer[1] & 0x0F)
  {
    case 0:
      DebugLog("ACK");
      break;
    case 1:
      DebugLog("NACK");
      break;
    case 2:
      DoError(aeNackNoBuf, "NACK_NO_BUF received!");
      break;
    case 3:
      DoError(aeNackBadSeq, "NACK_BAD_SEQ received!");
      break;
    case 4:
      DoError(aeNackOverrun, "NACK_OVERRUN received!");
      break;

    default:
      //Handle invalid control frame
      DoError(aeInvalidFrame, "Invalid control frame received!");
      break;
  }

  RxBuffer.erase(RxBuffer.begin(), RxBuffer.begin() + 3);
}
//---------------------------------------------------------------------------
void TAtiHandler::BreakSync()
{
  DebugLog("Break on!");
  SerialHandler->SetBreak();
  Breaking = true;
  TxSequence = 0;
  RxSequence = 0;
  Timer->Interval = 200;
  Timer->Enabled = true;
}
//---------------------------------------------------------------------------
void TAtiHandler::ChangeSpeed(unsigned Speed)
{
  BYTE Data[9];
  Data[0] = PT_ATI_CONF1;
  Data[1] = NEW_SPEED;
  Data[2] = SPEED_REQUEST;
  Data[3] = Speed >> 24;
  Data[4] = (Speed >> 16) & 0xFF;
  Data[5] = (Speed >> 8) & 0xFF;
  Data[6] = Speed & 0xFF;
  Data[7] = 0;
  Data[8] = 0;
  SendTelegram(Data, sizeof(Data), false);
}
//---------------------------------------------------------------------------
void TAtiHandler::DoTelegramReceived(const BYTE *Telegram, unsigned Size)
{
  if(Telegram[0] == PT_ATI_CONF1)
  {
    if(Telegram[1] == NEW_SPEED && Telegram[2] == SPEED_CONFIRM)
    {
      unsigned Speed = Telegram[3] << 24;
      Speed |= Telegram[4] << 16;
      Speed |= Telegram[5] << 8;
      Speed |= Telegram[6];
      SerialHandler->Disconnect();
      SerialHandler->Speed = Speed;
      SerialHandler->Connect();
      Sleep(100);

      DebugLog(AnsiString("Speed changed: ") + Speed);
      if(OnSpeedChanged)
        OnSpeedChanged(this, Speed);
    }
  }
  else if(OnTelegramReceived)
    OnTelegramReceived(this, Telegram, Size);
}
//---------------------------------------------------------------------------
void TAtiHandler::DoError(TAtiError ErrorCode, const AnsiString &ErrorStr)
{
  DebugLog(ErrorStr);
  if(OnError)
    OnError(this, ErrorCode, ErrorStr);
}
//---------------------------------------------------------------------------
void TAtiHandler::DebugLog(const AnsiString &Str)
{
  static std::ofstream out(ChangeFileExt(Application->ExeName, ".log").c_str());
  out << Str.c_str() << std::endl;
  OutputDebugString(Str.c_str());
}
//---------------------------------------------------------------------------


