/* Copyright 2007 Ivan Johansen
 *
 * Func32 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Func32.h"
#include "Func32Impl.h"
#pragma hdrstop
#include <cstdio>
//---------------------------------------------------------------------------
namespace Func32
{
/** Returns true if Ch is an operator (+, -, *, /, or ^)
 *  \param Ch: Character to check.
 */
bool IsOperator(char Ch)
{
  return Ch == '+' || Ch == '-' || Ch == '*' || Ch == '/' || Ch == '^';
}
//---------------------------------------------------------------------------
/** Create text defining function pointed to by the iterator. For debug use only.
 *  \param Iter: Iterator pointing to the first element in the function.
 *  \param Decimals: Number of decimals in numbers.
 */
std::wstring TFuncData::MakeText(TConstIterator Iter, unsigned Decimals)
{
  std::vector<std::wstring> Args;
  Args.push_back(L"Arg0");
  Args.push_back(L"Arg1");
  Args.push_back(L"Arg2");
  Args.push_back(L"Arg3");
  Args.push_back(L"Arg4");
  Args.push_back(L"Arg5");
  Args.push_back(L"Arg6");
  Args.push_back(L"Arg7");
  Args.push_back(L"Arg8");
  TMakeTextData TextData = {Iter, Args, Decimals, std::wstring()};
  CreateText(TextData);
  return TextData.Str;
}
//---------------------------------------------------------------------------
/** Create text from internal data in the object.
 *  \param Args: Vector of strings with the argument names.
 *  \param Decimals: Number of decimals in numbers.
 *  \return String with definition of the function.
 *  \throw EFuncError: Thrown if the object is empty.
 */
std::wstring TFuncData::MakeText(const std::vector<std::wstring> &Args, unsigned Decimals) const
{
  if(Data.empty())
    throw EFuncError(ecNoFunc);
  TMakeTextData TextData = {Data.begin(), Args, Decimals, std::wstring()};
  CreateText(TextData);
  if(TextData.Iter != Data.end())
    throw EFuncError(ecInternalError);
  return TextData.Str;
}
//---------------------------------------------------------------------------
/** Returns a string indicating the type of comparasion.
 *  \param CompareMethod: Indicates the comparation string.
 *  \return A string eg. "<="
 */
std::wstring GetCompareString(TCompareMethod CompareMethod)
{
  switch(CompareMethod)
  {
    case cmEqual:        return L"=";
    case cmNotEqual:     return L"<>";
    case cmLess:         return L"<";
    case cmGreater:      return L">";
    case cmLessEqual:    return L"<=";
    case cmGreaterEqual: return L">=";
  }
  return L"";
}
//---------------------------------------------------------------------------
/** Converts data at Iter to text saved at the end of Str. Recursive call.
 *  \param TextData: Data used for generating text
 *  \param AddPar: Indicates if parentheses should be added around the text.
 */
void TFuncData::CreateText(TMakeTextData &TextData, bool AddPar)
{
  std::wstring &Str = TextData.Str;
  const TConstIterator &Iter = TextData.Iter;
  const TElem &Elem = *TextData.Iter++;
  if(AddPar)
    Str += L'(';
  switch(Elem.Ident)
  {
    case CodeNumber:
    {
      //Don't use std::ostringstream; It will fail for numbers >MAXDOUBLE under STLport. sprintf() seems to work fine
      wchar_t S[30];
      std::swprintf(S, L"%.*LG", TextData.Decimals, Elem.Number);
      if(Elem.Number < 0 && !Str.empty() && *Str.rbegin() == '+')
        Str.erase(Str.end() - 1), Str += S;
      else if(Elem.Number < 0 && !Str.empty() && IsOperator(*Str.rbegin()))
        Str += std::wstring(L"(") + S + L")";
      else
        Str += S;
      break;
    }

    case CodeVariable:
      BOOST_ASSERT(Elem.Arguments < TextData.Args.size());
      Str += TextData.Args[Elem.Arguments];
      break;

    case Codee:
      Str += L'e';
      break;

    case CodePi:
      Str += L"pi";
      break;

    case CodeUndef:
      Str += L"undef";
      break;

    case Codei:
      Str += L'i';
      break;

    case CodeRand:
      Str += L"rand";
      break;

    case CodeAdd:
      CreateText(TextData);
      Str += L'+';
      CreateText(TextData);
      break;

    case CodeSub:
      CreateText(TextData);
      Str += L'-';
      CreateText(TextData, Iter->Ident == CodeAdd || Iter->Ident == CodeSub);
      break;

    case CodeMul:
      CreateText(TextData, Iter->Ident == CodeAdd || Iter->Ident == CodeSub);
      Str += L'*';
      CreateText(TextData, Iter->Ident == CodeAdd || Iter->Ident == CodeSub);
      break;

    case CodeDiv:
      CreateText(TextData, Iter->Ident == CodeAdd || Iter->Ident == CodeSub);
      Str += L'/';
      CreateText(TextData, Iter->Ident == CodeAdd || Iter->Ident == CodeSub || Iter->Ident == CodeMul || Iter->Ident == CodeDiv);
      break;

    case CodePow:
      CreateText(TextData, IsOperator(*Iter) || Iter->Ident == CodeNeg || (Iter->Ident == CodeNumber && Iter->Number < 0));
      Str += L'^';
      CreateText(TextData, IsOperator(*Iter));
      break;

    case CodePowDiv:
      CreateText(TextData, IsOperator(*Iter) || Iter->Ident == CodeNeg || (Iter->Ident == CodeNumber && Iter->Number < 0));
      Str += L"^(";
      CreateText(TextData, Iter->Ident == CodeAdd || Iter->Ident == CodeSub);
      Str += L'/';
      CreateText(TextData, Iter->Ident == CodeAdd || Iter->Ident == CodeSub || Iter->Ident == CodeMul || Iter->Ident == CodeDiv);
      Str += L')';
      break;

    case CodeNeg:
    {
      bool Parenthese = !Str.empty() && IsOperator(*Str.rbegin());
      if(Parenthese)
        Str += L'(';
      Str += L'-';
      CreateText(TextData, Iter->Ident == CodeAdd || Iter->Ident == CodeSub);
      if(Parenthese)
        Str += L')';
      break;
    }
    case CodeSqr:
      CreateText(TextData, IsOperator(*Iter) || Iter->Ident == CodeNeg || (Iter->Ident == CodeNumber &&Iter->Number < 0));
      Str += L"^2";
      break;

    case CodeCompare1:
      CreateText(TextData);
      Str += GetCompareString(Elem.Compare[0]);
      CreateText(TextData);
      break;

    case CodeCompare2:
      CreateText(TextData);
      Str += GetCompareString(Elem.Compare[0]);
      CreateText(TextData);
      Str += GetCompareString(Elem.Compare[1]);
      CreateText(TextData);
      break;

    case CodeAnd:
    case CodeOr:
    case CodeXor:
      CreateText(TextData);
      Str += Elem.Ident == CodeAnd ? L" and " : Elem.Ident == CodeOr ? L" or " : L" xor ";
      CreateText(TextData, Iter->Ident == CodeAnd || Iter->Ident == CodeOr || Iter->Ident == CodeXor);
      break;

    default:
      if(Elem.Ident > LastFunction)
        throw EFuncError(ecInternalError);
      Str += FunctionName(Elem);
      if(FunctionArguments(Elem) == 0)
        break;
      Str += L'(';
      CreateText(TextData);
      for(unsigned I = 1; I < FunctionArguments(Elem); I++)
      {
        Str += L',';
        CreateText(TextData);
      }
      Str += L')';
      break;
  }
  if(AddPar)
    Str += L')';
}
//---------------------------------------------------------------------------
} //namespace Func32


