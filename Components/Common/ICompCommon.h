//---------------------------------------------------------------------------
#ifndef ICompCommonH
#define ICompCommonH
//---------------------------------------------------------------------------
typedef String __fastcall (*TTranslateFunc)(String);

String TranslateCompText(const String &Str);
void SetCompTranslateFunc(TTranslateFunc Func);
#endif
