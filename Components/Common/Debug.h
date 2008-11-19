//---------------------------------------------------------------------------
#ifndef DebugH
#define DebugH

struct TNameValue
{
  const char *Name;
  unsigned Value;
};

extern const TNameValue OleUIResultList[];
extern const TNameValue HResultList[];

//Defines and functions used for logging OLE calls
#define LOG_FUNCTION_CALL(x) DebugLogFunctionCall(#x, HResultList, (x))
#define LOG_OLEUI_CALL(x) DebugLogFunctionCall(#x, OleUIResultList, (x))
#define NAME_VALUE_ENTRY(x) {#x, x}
#define NAME_VALUE_END {NULL, 0}

HRESULT DebugLogFunctionCall(const char *Name, const TNameValue List[], HRESULT Result);
String ValueToStr(const TNameValue List[], unsigned Value);
String FlagsToStr(const TNameValue List[], unsigned Value);

void InitDebug();

//---------------------------------------------------------------------------
#endif
 