//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Config.h"
#include "SerialHandler.h"
#include "SerialThread.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
TSerialThread::TSerialThread(TSerialHandler *ASerialHandler)
  : Thread::TIThread(false), SerialHandler(ASerialHandler)
{
  memset(&Overlapped, 0, sizeof(Overlapped));
}
//---------------------------------------------------------------------------
void __fastcall TSerialThread::Execute()
{
  Overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  SetName("Serial thread");
  SetCommMask(SerialHandler->Handle.Get(), EV_BREAK | EV_RXCHAR | EV_TXEMPTY );
  std::vector<BYTE> Data;

  while(!Terminated)
  {
    try
    {
      DWORD EvtMask;
      DWORD Dummy;
      if(!WaitCommEvent(SerialHandler->Handle.Get(), &EvtMask, &Overlapped))
        if(GetLastError() == ERROR_IO_PENDING)
          Win32Check(GetOverlappedResult(SerialHandler->Handle.Get(), &Overlapped, &Dummy, TRUE));
        else
          RaiseLastOSError();

      if(Terminated)
        break;

      if(EvtMask & EV_BREAK)
      {
        if(SerialHandler->Synchronized)
          Queue(&SerialHandler->DoBreak);
        else
          SerialHandler->DoBreak();
      }

      if(EvtMask & EV_RXCHAR)
      {
        //Retrieve number of bytes waiting
        COMSTAT ComStat;
        Win32Check(ClearCommError(SerialHandler->Handle.Get(), &Dummy, &ComStat));

        if(ComStat.cbInQue > 0)
        {
          //Resize vector and read data
          Data.resize(ComStat.cbInQue);

          DWORD BytesRead;
          if(!ReadFile(SerialHandler->Handle.Get(), &Data[0], Data.size(), &BytesRead, &Overlapped))
            Win32Check(GetOverlappedResult(SerialHandler->Handle.Get(), &Overlapped, &Dummy, TRUE));

          //Only send data if we are not breaking
          if((EvtMask & EV_BREAK) == 0)
          {
            //Send data to main thread
            if(SerialHandler->Synchronized)
              Synchronize(&SerialHandler->DoDataReceived, Data);
            else
              SerialHandler->DoDataReceived(Data);
          }
        }
      }

      if(EvtMask & EV_TXEMPTY)
      {
        if(SerialHandler->Synchronized)
          Queue(&SerialHandler->DoTransmissionFinished);
        else
          SerialHandler->DoTransmissionFinished();
      }
    }
    catch(EOSError &E)
    {
      if(SerialHandler->Synchronized)
        Synchronize(&SerialHandler->DoSerialError, std::make_pair(E.ErrorCode, E.Message));
      else
        SerialHandler->DoSerialError(std::make_pair(E.ErrorCode, E.Message));
      if(E.ErrorCode == ERROR_OPERATION_ABORTED || E.ErrorCode == ERROR_ACCESS_DENIED)
        break; //Terminate thread. Nothing to do anyway.
    }
    catch(Exception &E)
    {
      Application->ShowException(&E);
    }
    catch(...)
    {
      Application->MessageBox(L"Unknown exception in thread", L"Exception", MB_ICONSTOP);
    }
  }
  CloseHandle(Overlapped.hEvent);
}
//---------------------------------------------------------------------------
void TSerialThread::Terminate()
{
  TThread::Terminate();
  SetEvent(Overlapped.hEvent); //To terminate GetOverlappedResult()
}
//---------------------------------------------------------------------------

