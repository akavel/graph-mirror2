//---------------------------------------------------------------------------
#ifndef ICompCommonH
#define ICompCommonH
//---------------------------------------------------------------------------
typedef WideString __fastcall (*TTranslateFunc)(WideString);

WideString TranslateCompText(const WideString &Str);
void SetCompTranslateFunc(TTranslateFunc Func);
#endif
