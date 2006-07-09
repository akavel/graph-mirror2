/* Copyright 2006 Ivan Johansen
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
//---------------------------------------------------------------------------
namespace Func32
{
const unsigned MaxDifLevel = 10; //The maximum number of call levels allowed to avoid recursive functions

/** Diferentiate data and return the result in a new object.
 *  \param Var: Variable to differenciate with respect to
 *  \param Trigonometry: Choose to differentiate trigonometric functions as radians or degrees.
 *  \throw EFuncError: Thrown if differentiation fails.
 */
boost::shared_ptr<TFuncData> TFuncData::MakeDif(const TElem &Var, TTrigonometry Trigonometry) const
{
  if(Data.empty())
    throw EFuncError(ecNoFunc);

  boost::shared_ptr<TFuncData> Temp(new TFuncData);
  Temp->CopyReplace(Data.begin(), std::vector<TConstIterator>());
  boost::shared_ptr<TFuncData> Dest(new TFuncData);
  Dest->AddDif(Temp->Data.begin(), Var, Trigonometry, 0);

  //It is sometimes necesarry to optimize. For example d(x^2) needs an ln(x) optimized away
  DEBUG_LOG(std::clog << "Before simplify: f'(x)=" << MakeText(Dest->Data.begin()) << std::endl);
  Dest->Simplify();
  DEBUG_LOG(std::clog << "After simplify: f'(x)=" << MakeText(Dest->Data.begin()) << std::endl);
  Dest->Simplify();
  DEBUG_LOG(std::clog << "After simplify: f'(x)=" << MakeText(Dest->Data.begin()) << std::endl);
  return Dest;
}
//---------------------------------------------------------------------------
/** Returns an iterator to the element following Iter. Jumps over parameters.
 *  \param Iter: Iterator to find next element for.
 */
std::vector<TElem>::const_iterator FindEnd(std::vector<TElem>::const_iterator Iter)
{
  unsigned Arguments = FunctionArguments(*Iter);
  ++Iter;
  for(unsigned I = 0; I < Arguments; I++)
    Iter = FindEnd(Iter);
  return Iter;
}
//---------------------------------------------------------------------------
/** Saves the first derivative of the sequence starting at Iter in the back *this
 *  \param Iter: Iterator to function to differentiate.
 *  \param Var: Variable to differentiate with respect to.
 *  \param Trigonemetry: Differentiate trigonometric functions as radians og degrees.
 *  \param Level: Indicates the number os times the function has been called recursive. To prevent infinite loops.
 *  \throw EFuncError: Thrown if differentiation fails.
 */
void TFuncData::AddDif(TConstIterator Iter, const TElem &Var, TTrigonometry Trigonometry, unsigned Level)
{
  if(*Iter == Var)
    Data.push_back(TElem(CodeNumber, 1));
  else if(Iter->Ident == CodeRand)
    throw EFuncError(ecNotDifAble);
  else if(IsConstant(*Iter))
    Data.push_back(TElem(CodeNumber, 0.0));
  else if(IsFunction1P(*Iter) || IsFunction2P(*Iter))
  {
    if(Trigonometry == Degree)
    {
      //Sin, Cos, Tan, Csc, Sec, Cot must be multiplied with PI/180 when differentiated using degrees
      if((Iter->Ident >= CodeSin && Iter->Ident <= CodeTan) || (Iter->Ident >= CodeCsc && Iter->Ident <= CodeCot))
      {
        Data.push_back(CodeMul);
        Data.push_back(CodeDiv);
        Data.push_back(CodePi);
        Data.push_back(180);
      }
      //ASin, ACos, ATan, ACsc, ASec, ACot must be multiplied with 180/PI when differentiated using degrees
      else if((Iter->Ident >= CodeASin && Iter->Ident <= CodeATan) || (Iter->Ident >= CodeACsc && Iter->Ident <= CodeACot))
      {
        Data.push_back(CodeMul);
        Data.push_back(CodeDiv);
        Data.push_back(180);
        Data.push_back(CodePi);
      }
    }

    const TFuncData &DifData = GetDif(Iter->Ident);
    if(DifData.IsEmpty())
      throw EFuncError(ecNotDifAble);

    TConstIterator End = DifData.Data.end();
    TConstIterator FirstPar = Iter;             //Start of first parenthesis
    ++FirstPar;
    TConstIterator SecondPar = FindEnd(FirstPar); //Start of second parenthesis

    for(TConstIterator Elem = DifData.Data.begin(); Elem != End; ++Elem)
      if(Elem->Ident == CodeVariable)
        Data.insert(Data.end(), FirstPar, SecondPar);
      else if(*Elem == TElem(CodeCustom, "dx"))
        AddDif(FirstPar, Var, Trigonometry, Level);
      else if(*Elem == TElem(CodeCustom, "x2"))
        Data.insert(Data.end(), SecondPar, FindEnd(SecondPar));
      else if(*Elem == TElem(CodeCustom, "dx2"))
        AddDif(SecondPar, Var, Trigonometry, Level);
      else
        Data.push_back(*Elem);
  }
  else switch(Iter->Ident)
  {
    case CodeIf:
    case CodeIfSeq:
    {
      //f(x)=ifseq(a1,b1,a2,b2, ... , an,bn [,c])
      //f'(x)=ifseq(a1,b1',a2,b2', ... , an, bn' [,c'])
      Data.push_back(*Iter); //IfSeq if same number of arguments
      unsigned Arguments = FunctionArguments(*Iter);
      ++Iter;
      for(unsigned I = 0; I < Arguments-1; I++)
      {
        TConstIterator End = FindEnd(Iter);
        if(I % 2)
          AddDif(Iter, Var, Trigonometry, Level);
        else
          Data.insert(Data.end(), Iter, End);
        Iter = End;
      }
      AddDif(Iter, Var, Trigonometry, Level);
      break;
    }

    case CodeMin:
    case CodeMax:
    {
      //f(x)=min(a1,a2,a3, ... , an) f'(x)=ifseq(a1<a2 and a1<a3 ...,a1', a2<a1 and a2<a3 ...,a2',
      unsigned Arguments = Iter->Arguments;
      Data.push_back(TElem(CodeIfSeq, 2*Arguments-1, 0));
      TConstIterator Param = Iter + 1;
      for(unsigned I = 0; I < Arguments-1; I++)
      {
        TConstIterator End = FindEnd(Param);
        for(unsigned J = 0; J < Arguments-2; J++)
          Data.push_back(CodeAnd);

        TConstIterator Param2 = Iter+1;
        for(unsigned J = 0; J < Arguments; J++)
        {
          if(J != I)
          {
            Data.push_back(TElem(CodeCompare1, Iter->Ident == CodeMin ? cmLess : cmGreater));
            Data.insert(Data.end(), Param, End);
            Data.insert(Data.end(), Param2, FindEnd(Param2));
          }
          Param2 = FindEnd(Param2);
        }

        AddDif(Param, Var, Trigonometry, Level);
        Param = End;
      }
      AddDif(Param, Var, Trigonometry, Level);
      break;
    }

    case CodeCustom:
      if(Level > MaxDifLevel)
        throw EFuncError(ecRecursiveDif);
      if(Iter->FuncData)
        AddDif(Iter->FuncData->Data.begin(), Var, Trigonometry, Level + 1);
      else
        throw EFuncError(ecSymbolNotFound, Iter->Text);
      break;

    case CodeDNorm:
    {
      std::vector<std::string> ArgNames;
      ArgNames.push_back("x");
      ArgNames.push_back("x2");
      ArgNames.push_back("x3");
      TFuncData Temp(FunctionDefinition(CodeDNorm), ArgNames);
      TFuncData Temp2;
      std::vector<TConstIterator> Args;
      Args.push_back(Iter + 1);
      std::vector<TElem> Elements;
      Elements.push_back(TElem(CodeNumber, 0.0));
      Elements.push_back(TElem(CodeNumber, 1.0));
      if(Iter->Arguments > 2)
        Args.push_back(FindEnd(Args.back())), Args.push_back(FindEnd(Args.back()));
      else
        Args.push_back(Elements.begin()), Args.push_back(Elements.begin()+1);
      Temp2.CopyReplace(Temp.Data.begin(), Args);
      AddDif(Temp2.Data.begin(), Var, Trigonometry, Level + 1);
      break;
    }
    default:
      throw EFuncError(ecNotDifAble);
  }
}
//---------------------------------------------------------------------------
} //namespace Func32











