//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Config.h"
#include "SerialHandler.h"
#include "SerialThread.h"
#include <Registry.hpp>
#pragma package(smart_init)
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TSerialHandler *)
{
  new TSerialHandler(NULL);
}
//---------------------------------------------------------------------------
namespace Serialhandler
{
  void __fastcall PACKAGE Register()
  {
     TComponentClass classes[1] = {__classid(TSerialHandler)};
     RegisterComponents("IComp", classes, 0);   
  }
}
//---------------------------------------------------------------------------
__fastcall TSerialHandler::TSerialHandler(TComponent* Owner)
  : TComponent(Owner), FPort("COM1"), FSpeed(19200), FByteSize(8), FParity(pbNoParity), FStopBits(sbOneStopBit),
    FSynchronized(true),
    FOnBreak(NULL), FOnDataReceived(NULL), FOnTransmissionFinished(NULL), FOnSerialError(NULL)
{
}
//---------------------------------------------------------------------------
__fastcall TSerialHandler::~TSerialHandler()
{
  Disconnect();
}
//---------------------------------------------------------------------------
void TSerialHandler::Connect()
{
  if(Handle.Get())   //If already open, don't bother
      return;

  THandle TempHandle(CreateFileA(("\\\\.\\" + Port).c_str(),
                      GENERIC_READ | GENERIC_WRITE,
                      0,    /* comm devices must be opened w/exclusive-access */
                      NULL, /* no security attrs */
                      OPEN_EXISTING, /* comm devices must use OPEN_EXISTING */
                      FILE_FLAG_OVERLAPPED,    /* Overlapped I/O */
                      NULL  /* hTemplate must be NULL for comm devices */
                      ));

  //If CreateFile fails, throw an exception. CreateFile will fail if the
  //port is already open, or if the com port does not exist.
  if(TempHandle.Get() == INVALID_HANDLE_VALUE)
    RaiseLastOSError();

  UpdateState(TempHandle);

  //Set the intial size of the transmit and receive queues. These are
  //not exposed to outside clients of the class either. Perhaps they should be?
  //I set the receive buffer to 32k, and the transmit buffer to 9k (a default).
  Win32Check(SetupComm(TempHandle.Get(), 1024*32, 1024*9));

  // These values are just default values that I determined empirically.
  // Adjust as necessary. I don't expose these to the outside because
  // most people aren't sure how they work (uhhh, like me included).
  //WARNING: It looks like Windows may report that transmission has finished
  //nearly imediately when a USB dongle is used instead of a real serial port.
  COMMTIMEOUTS TimeOuts;
  TimeOuts.ReadIntervalTimeout         = 15;
  TimeOuts.ReadTotalTimeoutMultiplier  = 1;
  TimeOuts.ReadTotalTimeoutConstant    = 250;
  TimeOuts.WriteTotalTimeoutMultiplier = 0; //Set both write values to 0 to prevent
  TimeOuts.WriteTotalTimeoutConstant   = 0; //a write timeout
  Win32Check(SetCommTimeouts(TempHandle.Get(), &TimeOuts));

  Handle.Swap(TempHandle);
  Thread = new TSerialThread(this);
}
//---------------------------------------------------------------------------
void TSerialHandler::Disconnect()
{
  if(Thread)
  {
    Thread->Terminate();
    Thread->WaitFor();
    delete Thread;
    Thread = NULL;
  }
  Handle.Close();
}
//---------------------------------------------------------------------------
bool TSerialHandler::GetConnected()
{
	return Handle.Get();
}
//---------------------------------------------------------------------------
void TSerialHandler::UpdateState(const THandle &TempHandle)
{
  if(TempHandle.Get())
  {
    //Now get the DCB properties of the port we just opened
    DCB dcb;
    Win32Check(GetCommState(TempHandle.Get(), &dcb));

    //dcb contains the actual port properties.  Now copy our settings into this dcb
    dcb.BaudRate  = FSpeed;
    dcb.ByteSize  = FByteSize;
    dcb.Parity    = FParity;
    dcb.StopBits  = FStopBits;

    //Now we can set the properties of the port with our settings.
    Win32Check(SetCommState(TempHandle.Get(), &dcb));
  }
}
//---------------------------------------------------------------------------
void TSerialHandler::SetBreak()
{
  Win32Check(SetCommBreak(GetHandle()));
}
//---------------------------------------------------------------------------
void TSerialHandler::ClearBreak()
{
  Win32Check(ClearCommBreak(GetHandle()));
}
//---------------------------------------------------------------------------
void TSerialHandler::DoBreak()
{
  if(FOnBreak)
    FOnBreak(this);
}
//---------------------------------------------------------------------------
HANDLE TSerialHandler::GetHandle()
{
  if(!Handle)
    throw Exception("Serial port is not open");
  return Handle.Get();  
}
//---------------------------------------------------------------------------
//This callback function is called when an asynchronious write has finished
void CALLBACK WriteFinished(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, OVERLAPPED *Overlapped)
{
  delete Overlapped;
}
//---------------------------------------------------------------------------
void TSerialHandler::WriteBuffer(const void *Buffer, unsigned ByteCount, bool Wait)
{
  if(ByteCount == 0 || Buffer == NULL)
    return;
  //Never reuse an OVERLAPPED structure until the previous operation has using the
  //structure has completed
  if(Wait)
  {
    DWORD Dummy;
    OVERLAPPED Overlapped = {0};
    WriteFile(GetHandle(), Buffer, ByteCount, NULL, &Overlapped);
    Win32Check(GetOverlappedResult(GetHandle(), &Overlapped, &Dummy, TRUE));
  }
  else
  {
    OVERLAPPED *Overlapped = new OVERLAPPED;
    memset(Overlapped, 0, sizeof(OVERLAPPED));
    WriteFileEx(GetHandle(), Buffer, ByteCount, Overlapped, WriteFinished);
    SleepEx(0, true); //This ensures that WriteFinished will be called
  }
}
//---------------------------------------------------------------------------
void TSerialHandler::DoDataReceived(const std::vector<BYTE> &Data)
{
  try
  {
    if(FOnDataReceived)
      FOnDataReceived(this, &Data[0], Data.size());
  }
  catch(Exception &E)
  {
    Application->ShowException(&E);
  }
}
//---------------------------------------------------------------------------
void TSerialHandler::Purge()
{
  Win32Check(PurgeComm(GetHandle(), PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR));
}
//---------------------------------------------------------------------------
//WARNING: Windows may report that transmission has finsihed much too early if
//a USB dongle is used instead of a real serial port.
void TSerialHandler::DoTransmissionFinished()
{
  try
  {
    if(FOnTransmissionFinished)
      FOnTransmissionFinished(this);
  }
  catch(Exception &E)
  {
    Application->ShowException(&E);
  }
}
//---------------------------------------------------------------------------
void TSerialHandler::DoSerialError(const std::pair<DWORD, String> &Error)
{
  try
  {
  	if(FOnSerialError)
	    FOnSerialError(this, Error.first, Error.second);
	  else
	    Application->MessageBox(Error.second.c_str(), L"Exception", MB_ICONSTOP);
  }
  catch(Exception &E)
  {
    Application->ShowException(&E);
  }
}
//---------------------------------------------------------------------------
void TSerialHandler::ClearDTR()
{
  Win32Check(EscapeCommFunction(GetHandle(), CLRDTR));
}
//---------------------------------------------------------------------------
void TSerialHandler::ClearRTS()
{
  Win32Check(EscapeCommFunction(GetHandle(), CLRRTS));
}
//---------------------------------------------------------------------------
void TSerialHandler::SetDTR()
{
  Win32Check(EscapeCommFunction(GetHandle(), SETDTR));
}
//---------------------------------------------------------------------------
void TSerialHandler::SetRTS()
{
  Win32Check(EscapeCommFunction(GetHandle(), SETRTS));
}
//---------------------------------------------------------------------------
void TSerialHandler::GetSerialPorts(TStrings *SerialPorts)
{
  SerialPorts->Clear();
  std::auto_ptr<TRegistry> Registry(new TRegistry);
  Registry->RootKey = HKEY_LOCAL_MACHINE;
  if(Registry->OpenKeyReadOnly("Hardware\\DeviceMap\\SerialComm"))
  {
    std::auto_ptr<TStrings> Strings(new TStringList);
    Registry->GetValueNames(Strings.get());
    for(int I = 0; I < Strings->Count; I++)
      SerialPorts->Add(Registry->ReadString(Strings->Strings[I]));
  }
}
//---------------------------------------------------------------------------
void __fastcall TSerialHandler::SetSpeed(unsigned Value)
{
  FSpeed = Value;
  UpdateState(Handle);
}
//---------------------------------------------------------------------------
unsigned __fastcall TSerialHandler::GetSpeed()
{
  if(Handle.Get())
  {
    DCB dcb;
    Win32Check(GetCommState(Handle.Get(), &dcb));
    return dcb.BaudRate;
  }
  return FSpeed;
}
//---------------------------------------------------------------------------
void __fastcall TSerialHandler::SetByteSize(unsigned Value)
{
  if(Value != FByteSize)
  {
    FByteSize = Value;
    UpdateState(Handle);
  }
}
//---------------------------------------------------------------------------
void __fastcall TSerialHandler::SetParity(TParityBit Value)
{
  if(Value != FParity)
  {
    FParity = Value;
    UpdateState(Handle);
  }
}
//---------------------------------------------------------------------------
void __fastcall TSerialHandler::SetStopBits(TStopBits Value)
{
  if(Value != FStopBits)
  {
    FStopBits = Value;
    UpdateState(Handle);
  }
}
//---------------------------------------------------------------------------

