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
/** Finds out if the next non-operator is a negative number or CodeNeg.
 *  e.g. CodeMul(-2, x) will be true.
 *  \param Iter: Iterator to start the search from.
 */
bool IsNextNeg(TConstIterator Iter)
{
  if(IsOperator(*Iter))
    return IsNextNeg(++Iter);
  if(Iter->Ident == CodeNeg)
    return true;
  if(Iter->Ident == CodeNumber && Iter->Number < 0)
    return true;
  return false;
}
//---------------------------------------------------------------------------
/** Create text defining function pointed to by the iterator. For debug use only.
 *  \param Iter: Iterator pointing to the first element in the function.
 *  \return Converted function text
 */
std::wstring TFuncData::MakeText(TConstIterator Iter)
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
  std::wstringstream Stream;
  TMakeTextData TextData = {Iter, Args, Stream};
  CreateText(TextData);
  return Stream.str();
}
//---------------------------------------------------------------------------
/** Create text from internal data in the object.
 *  \param Args: Vector of strings with the argument names.
 *  \param Stream: Stream to write converted function text to.
 *  \throw EFuncError: Thrown if the object is empty.
 */
void TFuncData::MakeText(const std::vector<std::wstring> &Args, std::wostream &Stream) const
{
  if(Data.empty())
    throw EFuncError(ecNoFunc);
  TMakeTextData TextData = {Data.begin(), Args, Stream};
  CreateText(TextData);
  if(TextData.Iter != Data.end())
    throw EFuncError(ecInternalError);
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
  std::wostream &Stream = TextData.Stream;
  const TElem &Elem = *TextData.Iter++;
  const TConstIterator &Iter = TextData.Iter;
  if(AddPar)
    Stream << L'(';
  switch(Elem.Ident)
  {
    case CodeNumber:
      //std::uppercase is used to show E in 5E3 in uppercase
      if(std::abs(Elem.Number) >= 10000 || std::abs(Elem.Number) < 0.0001)
        Stream << std::scientific;
      else
        Stream << std::fixed;
      Stream << std::uppercase << Elem.Number;
      break;

    case CodeVariable:
      BOOST_ASSERT(Elem.Arguments < TextData.Args.size());
      Stream << TextData.Args[Elem.Arguments];
      break;

    case Codee:
      Stream << L'e';
      break;

    case CodePi:
      Stream << L"pi";
      break;

    case CodeUndef:
      Stream << L"undef";
      break;

    case Codei:
      Stream << L'i';
      break;

    case CodeRand:
      Stream << L"rand";
      break;

    case CodeInf:
      Stream << "INF";
      break;

    case CodeAdd:
      CreateText(TextData);
      if(!IsNextNeg(Iter))
        Stream << L'+';
      CreateText(TextData);
      break;

    case CodeSub:
      CreateText(TextData);
      Stream << L'-';
      CreateText(TextData, *Iter == CodeAdd || *Iter == CodeSub || IsNextNeg(Iter));
      break;

    case CodeMul:
      CreateText(TextData, *Iter == CodeAdd || *Iter == CodeSub);
      Stream << L'*';
      CreateText(TextData, *Iter == CodeAdd || *Iter == CodeSub);
      break;

    case CodeDiv:
      CreateText(TextData, *Iter == CodeAdd || *Iter == CodeSub);
      Stream << L'/';
      CreateText(TextData, *Iter == CodeAdd || *Iter == CodeSub || *Iter == CodeMul || *Iter == CodeDiv);
      break;

    case CodePow:
      CreateText(TextData, IsOperator(*Iter) || *Iter == CodeSqr || IsNextNeg(Iter));
      Stream << L'^';
      CreateText(TextData, IsOperator(*Iter));
      break;

    case CodePowDiv:
      CreateText(TextData, IsOperator(*Iter) || *Iter == CodeSqr || IsNextNeg(Iter));
      Stream << L"^(";
      CreateText(TextData, *Iter == CodeAdd || *Iter == CodeSub);
      Stream << L'/';
      CreateText(TextData, *Iter == CodeAdd || *Iter == CodeSub || *Iter == CodeMul || *Iter == CodeDiv);
      Stream << L')';
      break;

    case CodeNeg:
    {
      Stream << L'-';
      CreateText(TextData, *Iter == CodeAdd || *Iter == CodeSub);
      break;
    }
    case CodeSqr:
      CreateText(TextData, IsOperator(*Iter) || IsNextNeg(Iter));
      Stream << L"^2";
      break;

    case CodeCompare1:
      CreateText(TextData);
      Stream << GetCompareString(Elem.Compare[0]);
      CreateText(TextData);
      break;

    case CodeCompare2:
      CreateText(TextData);
      Stream << GetCompareString(Elem.Compare[0]);
      CreateText(TextData);
      Stream << GetCompareString(Elem.Compare[1]);
      CreateText(TextData);
      break;

    case CodeAnd:
    case CodeOr:
    case CodeXor:
      CreateText(TextData);
      Stream << (Elem == CodeAnd ? L" and " : Elem == CodeOr ? L" or " : L" xor ");
      CreateText(TextData, *Iter == CodeAnd || *Iter == CodeOr || *Iter == CodeXor);
      break;

    default:
      if(Elem.Ident > LastFunction)
        throw EFuncError(ecInternalError);
      Stream << FunctionName(Elem);
      if(FunctionArguments(Elem) == 0)
        break;
      Stream << L'(';
      CreateText(TextData);
      for(unsigned I = 1; I < FunctionArguments(Elem); I++)
      {
        Stream << L',';
        CreateText(TextData);
      }
      Stream << L')';
      break;
  }
  if(AddPar)
    Stream << L')';
}
//---------------------------------------------------------------------------
} //namespace Func32


