//---------------------------------------------------------------------------
#ifndef DebugH
#define DebugH

struct TNameValue
{
  const char *Name;
  unsigned Value;
  TNameValue(const char *AName, unsigned AValue) : Name(AName), Value(AValue) {}
};

extern const TNameValue OleUIResultList[];
extern const TNameValue HResultList[];

//Defines and functions used for logging OLE calls
#define LOG_FUNCTION_CALL(x) DebugLogFunctionCall(#x, HResultList, (x))
#define LOG_OLEUI_CALL(x) DebugLogFunctionCall(#x, OleUIResultList, (x))
#define NAME_VALUE_ENTRY(x) TNameValue(#x, x)
#define NAME_VALUE_END TNameValue(NULL, 0)

HRESULT DebugLogFunctionCall(const char *Name, const TNameValue List[], HRESULT Result);
AnsiString ValueToStr(const TNameValue List[], unsigned Value);
AnsiString FlagsToStr(const TNameValue List[], unsigned Value);

void InitDebug();

//---------------------------------------------------------------------------
#endif
 