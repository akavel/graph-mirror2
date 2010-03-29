//---------------------------------------------------------------------------
#ifndef ICompCommonH
#define ICompCommonH
#include <string>
//---------------------------------------------------------------------------
typedef String __fastcall (*TTranslateFunc)(String);

String TranslateCompText(const String &Str);
void SetCompTranslateFunc(TTranslateFunc Func);
std::string Trim(const std::string &Str);
std::wstring Trim(const std::wstring &Str);
#endif
