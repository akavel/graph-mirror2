#ifndef DebugH

void __stdcall OutputDebugStringA(const char* lpOutputString);

class TDebugStreamBuf : public std::stringbuf
{
protected:
  int sync()
  {
    OutputDebugStringA(str().c_str());
    str("");
    return std::stringbuf::sync();
  }
};

#endif
 