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
std::string TFuncData::MakeText(TConstIterator Iter)
{
  std::string Str;
  std::vector<std::string> Args;
  Args.push_back("Arg0");
  Args.push_back("Arg1");
  Args.push_back("Arg2");
  Args.push_back("Arg3");
  Args.push_back("Arg4");
  Args.push_back("Arg5");
  Args.push_back("Arg6");
  Args.push_back("Arg7");
  Args.push_back("Arg8");
  CreateText(Iter, Str, Args);
  return Str;
}
//---------------------------------------------------------------------------
/** Create text from internal data in the object.
 *  \param Args: Vector of strings with the argument names.
 *  \return String with definition of the function.
 *  \throw EFuncError: Thrown if the object is empty.
 */
std::string TFuncData::MakeText(const std::vector<std::string> &Args) const
{
  if(Data.empty())
    throw EFuncError(ecNoFunc);
  std::string Str;
  if(CreateText(Data.begin(), Str, Args) != Data.end())
    throw EFuncError(ecInternalError);
  return Str;
}
//---------------------------------------------------------------------------
std::string GetCompareString(TCompareMethod CompareMethod)
{
  switch(CompareMethod)
  {
    case cmEqual:        return "=";
    case cmNotEqual:     return "<>";
    case cmLess:         return "<";
    case cmGreater:      return ">";
    case cmLessEqual:    return "<=";
    case cmGreaterEqual: return ">=";
  }
  return "";
}
//---------------------------------------------------------------------------
std::vector<TElem>::const_iterator TFuncData::CreateTextInPar(TConstIterator Iter, std::string &Str, const std::vector<std::string> &Args)
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
std::vector<TElem>::const_iterator TFuncData::CreateText(TConstIterator Iter, std::string &Str, const std::vector<std::string> &Args)
{
  TElem Elem = *Iter++;
  switch(Elem.Ident)
  {
    case CodeNumber:
    {
      //Don't use std::ostringstream; It will fail for numbers >MAXDOUBLE under STLport. sprintf() seems to work fine
      char S[30];
      std::sprintf(S, "%.8LG", Elem.Number);
      if(Elem.Number < 0 && !Str.empty() && *Str.rbegin() == '+')
        Str.erase(Str.end() - 1), Str += S;
      if(Elem.Number < 0 && !Str.empty() && IsOperator(*Str.rbegin()))
        Str += std::string("(") + S + ")";
      else
        Str += S;
      return Iter;
    }

    case CodeVariable:
      BOOST_ASSERT(Elem.Arguments < Args.size());
      Str += Args[Elem.Arguments];
      return Iter;

    case Codee:
      Str += 'e';
      return Iter;

    case CodePi:
      Str += "pi";
      return Iter;

    case CodeUndef:
      Str += "undef";
      return Iter;

    case Codei:
      Str += 'i';
      return Iter;

    case CodeRand:
      Str += "rand";
      return Iter;

    case CodeAdd:
      Iter = CreateText(Iter, Str, Args);
      Str += '+';
      Iter = CreateText(Iter, Str, Args);
      return Iter;

    case CodeSub:
      Iter = CreateText(Iter, Str, Args);
      Str += '-';
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
      Str += '*';
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
      Str += '/';
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
      Str += '^';
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
      Str += "^(";
      if(Iter->Ident == CodeAdd || Iter->Ident == CodeSub)
        Iter = CreateTextInPar(Iter, Str, Args);
      else
        Iter = CreateText(Iter, Str, Args);
      Str += '/';
      if(Iter->Ident == CodeAdd || Iter->Ident == CodeSub || Iter->Ident == CodeMul || Iter->Ident == CodeDiv)
        Iter = CreateTextInPar(Iter, Str, Args);
      else
        Iter = CreateText(Iter, Str, Args);
      Str += ')';
      return Iter;

    case CodeNeg:
    {
      bool Parenthese = !Str.empty() && IsOperator(*Str.rbegin());
      if(Parenthese)
        Str += '(';
      Str += '-';
      if(Iter->Ident == CodeAdd || Iter->Ident == CodeSub)
        Iter = CreateTextInPar(Iter, Str, Args);
      else
        Iter = CreateText(Iter, Str, Args);
      if(Parenthese)
        Str += ')';
      return Iter;
    }
    case CodeSqr:
      if(IsOperator(*Iter) || Iter->Ident == CodeNeg || (Iter->Ident == CodeNumber &&Iter->Number < 0))
        Iter = CreateTextInPar(Iter, Str, Args);
      else
        Iter = CreateText(Iter, Str, Args);
      Str += "^2";
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
      Str += Elem.Ident == CodeAnd ? " and " : Elem.Ident == CodeOr ? " or " : " xor ";
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
      Str += '(';
      Iter = CreateText(Iter, Str, Args);
      for(unsigned I = 1; I < FunctionArguments(Elem); I++)
      {
        Str += ',';
        Iter = CreateText(Iter, Str, Args);
      }
      Str += ')';
      return Iter;
  }
}
//---------------------------------------------------------------------------
} //namespace Func32


