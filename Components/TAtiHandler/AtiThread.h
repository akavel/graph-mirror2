//---------------------------------------------------------------------------
#ifndef AtiThreadH
#define AtiThreadH
//---------------------------------------------------------------------------
#include "IThread.h"
#include <utility>
//---------------------------------------------------------------------------
class TAtiHandler;
enum TAtiThreadMessage {atmDataReceived=WM_USER, atmTerminate};

class TAtiThread : public TIThread
{
  TAtiHandler *AtiHandler;

  void __fastcall Execute();
  void DataReceived(const std::pair<BYTE*, unsigned> &Pair);

  public:
  __fastcall TAtiThread(TAtiHandler *AtiHandler);
};

#endif
