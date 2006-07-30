//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "Config.h"
#include "AtiThread.h"
#include "AtiHandler.h"
#pragma package(smart_init)
//---------------------------------------------------------------------------
__fastcall TAtiThread::TAtiThread(TAtiHandler *AAtiHandler)
  : AtiHandler(AAtiHandler)
{
}
//---------------------------------------------------------------------------
void __fastcall TAtiThread::Execute()
{
  SetName("Ati thread");
  while(!Terminated)
  {
    TMessage Message;
    GetMessage(Message);
    switch(Message.Msg)
    {
      case atmDataReceived:
        Synchronize(&DataReceived, std::make_pair(reinterpret_cast<BYTE*>(Message.WParam), Message.LParam));
        break;

      case atmTerminate:
        FreeOnTerminate = true;
        return;
    }
  }
}
//---------------------------------------------------------------------------
void TAtiThread::DataReceived(const std::pair<BYTE*, unsigned> &Pair)
{
  AtiHandler->DoTelegramReceived(Pair.first, Pair.second);
  delete[] Pair.first;
}
//---------------------------------------------------------------------------

