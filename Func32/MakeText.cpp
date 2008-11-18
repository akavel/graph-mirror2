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
 */
std::wstring TFuncData::MakeText(TConstIterator Iter)
{
  std::wstring Str;
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
  CreateText(Iter, Str, Args);
  return Str;
}
//---------------------------------------------------------------------------
/** Create text from internal data in the object.
 *  \param Args: Vector of strings with the argument names.
 *  \return String with definition of the function.
 *  \throw EFuncError: Thrown if the object is empty.
 */
std::wstring TFuncData::MakeText(const std::vector<std::wstring> &Args) const
{
  if(Data.empty())
    throw EFuncError(ecNoFunc);
  std::wstring Str;
  if(CreateText(Data.begin(), Str, Args) != Data.end())
    throw EFuncError(ecInternalError);
  return Str;
}
//---------------------------------------------------------------------------
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
std::vector<TElem>::const_iterator TFuncData::CreateTextInPar(TConstIterator Iter, std::wstring &Str, const std::vector<std::wstring> &Args)
{
  Str += '(';
  std::vector<TElem>::const_iterator Result = CreateText(Iter, Str, Args);
  Str += ')';
  return Result;
}
//---------------------------------------------------------------------------
/** Converts data at Iter to text saved at the end of Str. Recursive call.
 *  \param Iter: Iterator pointing to next element to convert.
 *  \param Str:  String to add result to.
 *  \param Args: Vector of argument names.
 *  \return Iterator to one after the last element in the function.
 */
std::vector<TElem>::const_iterator TFuncData::CreateText(TConstIterator Iter, std::wstring &Str, const std::vector<std::wstring> &Args)
{
  TElem Elem = *Iter++;
  switch(Elem.Ident)
  {
    case CodeNumber:
    {
      //Don't use std::ostringstream; It will fail for numbers >MAXDOUBLE under STLport. sprintf() seems to work fine
      wchar_t S[30];
      std::swprintf(S, L"%.8LG", Elem.Number);
      if(Elem.Number < 0 && !Str.empty() && *Str.rbegin() == '+')
        Str.erase(Str.end() - 1), Str += S;
      if(Elem.Number < 0 && !Str.empty() && IsOperator(*Str.rbegin()))
        Str += std::wstring(L"(") + S + L")";
      else
        Str += S;
      return Iter;
    }

    case CodeVariable:
      BOOST_ASSERT(Elem.Arguments < Args.size());
      Str += Args[Elem.Arguments];
      return Iter;

    case Codee:
      Str += L'e';
      return Iter;

    case CodePi:
      Str += L"pi";
      return Iter;

    case CodeUndef:
      Str += L"undef";
      return Iter;

    case Codei:
      Str += L'i';
      return Iter;

    case CodeRand:
      Str += L"rand";
      return Iter;

    case CodeAdd:
      Iter = CreateText(Iter, Str, Args);
      Str += L'+';
      Iter = CreateText(Iter, Str, Args);
      return Iter;

    case CodeSub:
      Iter = CreateText(Iter, Str, Args);
      Str += L'-';
      if(Iter->Ident == CodeAdd || Iter->Ident == CodeSub)
        Iter = CreateTextInPar(Iter, Str, Args);
      else
        Iter = CreateText(Iter, Str, Args);
      return Iter;

    case CodeMul:
      if(Iter->Ident == CodeAdd || Iter->Ident == CodeSub)
        Iter = CreateTextInPar(Iter, Str, Args);
      else
        Iter = CreateText(Iter, Str, Args);
      Str += L'*';
      if(Iter->Ident == CodeAdd || Iter->Ident == CodeSub)
        Iter = CreateTextInPar(Iter, Str, Args);
      else
        Iter = CreateText(Iter, Str, Args);
      return Iter;

    case CodeDiv:
      if(Iter->Ident == CodeAdd || Iter->Ident == CodeSub)
        Iter = CreateTextInPar(Iter, Str, Args);
      else
        Iter = CreateText(Iter, Str, Args);
      Str += L'/';
      if(Iter->Ident == CodeAdd || Iter->Ident == CodeSub || Iter->Ident == CodeMul || Iter->Ident == CodeDiv)
        Iter = CreateTextInPar(Iter, Str, Args);
      else
        Iter = CreateText(Iter, Str, Args);
      return Iter;

    case CodePow:
      if(IsOperator(*Iter) || Iter->Ident == CodeNeg || (Iter->Ident == CodeNumber && Iter->Number < 0))
        Iter = CreateTextInPar(Iter, Str, Args);
      else
        Iter = CreateText(Iter, Str, Args);
      Str += L'^';
      if(IsOperator(*Iter))
        Iter = CreateTextInPar(Iter, Str, Args);
      else
        Iter = CreateText(Iter, Str, Args);
      return Iter;

    case CodePowDiv:
      if(IsOperator(*Iter) || Iter->Ident == CodeNeg || (Iter->Ident == CodeNumber && Iter->Number < 0))
        Iter = CreateTextInPar(Iter, Str, Args);
      else
        Iter = CreateText(Iter, Str, Args);
      Str += L"^(";
      if(Iter->Ident == CodeAdd || Iter->Ident == CodeSub)
        Iter = CreateTextInPar(Iter, Str, Args);
      else
        Iter = CreateText(Iter, Str, Args);
      Str += L'/';
      if(Iter->Ident == CodeAdd || Iter->Ident == CodeSub || Iter->Ident == CodeMul || Iter->Ident == CodeDiv)
        Iter = CreateTextInPar(Iter, Str, Args);
      else
        Iter = CreateText(Iter, Str, Args);
      Str += L')';
      return Iter;

    case CodeNeg:
    {
      bool Parenthese = !Str.empty() && IsOperator(*Str.rbegin());
      if(Parenthese)
        Str += L'(';
      Str += L'-';
      if(Iter->Ident == CodeAdd || Iter->Ident == CodeSub)
        Iter = CreateTextInPar(Iter, Str, Args);
      else
        Iter = CreateText(Iter, Str, Args);
      if(Parenthese)
        Str += L')';
      return Iter;
    }
    case CodeSqr:
      if(IsOperator(*Iter) || Iter->Ident == CodeNeg || (Iter->Ident == CodeNumber &&Iter->Number < 0))
        Iter = CreateTextInPar(Iter, Str, Args);
      else
        Iter = CreateText(Iter, Str, Args);
      Str += L"^2";
      return Iter;

    case CodeCompare1:
      Iter = CreateText(Iter, Str, Args);
      Str += GetCompareString(Elem.Compare[0]);
      Iter = CreateText(Iter, Str, Args);
      return Iter;

    case CodeCompare2:
      Iter = CreateText(Iter, Str, Args);
      Str += GetCompareString(Elem.Compare[0]);
      Iter = CreateText(Iter, Str, Args);
      Str += GetCompareString(Elem.Compare[1]);
      Iter = CreateText(Iter, Str, Args);
      return Iter;

    case CodeAnd:
    case CodeOr:
    case CodeXor:
      Iter = CreateText(Iter, Str, Args);
      Str += Elem.Ident == CodeAnd ? L" and " : Elem.Ident == CodeOr ? L" or " : L" xor ";
      if(Iter->Ident == CodeAnd || Iter->Ident == CodeOr || Iter->Ident == CodeXor)
        Iter = CreateTextInPar(Iter, Str, Args);
      else
        Iter = CreateText(Iter, Str, Args);
      return Iter;

    default:
      if(Elem.Ident > LastFunction)
        throw EFuncError(ecInternalError);
      Str += FunctionName(Elem);
      if(FunctionArguments(Elem) == 0)
        return Iter;
      Str += L'(';
      Iter = CreateText(Iter, Str, Args);
      for(unsigned I = 1; I < FunctionArguments(Elem); I++)
      {
        Str += L',';
        Iter = CreateText(Iter, Str, Args);
      }
      Str += L')';
      return Iter;
  }
}
//---------------------------------------------------------------------------
} //namespace Func32


