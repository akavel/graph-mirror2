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
namespace Func32
{

struct TSimplify
{
  TElem Function;
  TElem Par1;
  TElem Par2;
  TElem Result[4];
  TSimplify(const TElem &AFunction, const TElem &APar1, const TElem &APar2, const TElem &AResult1, const TElem &AResult2 = CodeNull, const TElem &AResult3 = CodeNull, const TElem &AResult4 = CodeNull)
    : Function(AFunction), Par1(APar1), Par2(APar2) {Result[0] = AResult1; Result[1] = AResult2; Result[2] = AResult3; Result[3] = AResult4;}
  TSimplify() {}
};

const TSimplify& SimplifyTable(unsigned Index)
{

static const TSimplify Table[] = {
TSimplify(CodeAdd, CodeIgnore, 0, CodePar1),
TSimplify(CodeAdd, 0, CodeIgnore, CodePar2),
TSimplify(CodeAdd, CodeIgnore, CodeNeg, CodeSub, CodePar1, CodePar2Par),
TSimplify(CodeAdd, CodeIgnore, CodePar1, CodeMul, 2, CodePar1),

TSimplify(CodeSub, CodeIgnore, 0, CodePar1),
TSimplify(CodeSub, 0, CodeNeg, CodePar2Par),
TSimplify(CodeSub, 0, CodeIgnore, CodeNeg, CodePar2),
TSimplify(CodeSub, CodeIgnore, CodePar1, 0),

TSimplify(CodeMul, CodeIgnore, 0, 0),
TSimplify(CodeMul, 0, CodeIgnore, 0),
TSimplify(CodeMul, CodeIgnore, 1, CodePar1),
TSimplify(CodeMul, 1, CodeIgnore, CodePar2),
TSimplify(CodeNeg, 0, CodeIgnore, 0),
TSimplify(CodeMul, CodeNeg, -1, CodePar1Par),
TSimplify(CodeMul, -1, CodeNeg, CodePar2Par),
TSimplify(CodeMul, CodeIgnore, -1, CodeNeg, CodePar1),
TSimplify(CodeMul, -1, CodeIgnore, CodeNeg, CodePar2),
TSimplify(CodeMul, CodeIgnore, CodeNeg, CodeNeg, CodeMul, CodePar1, CodePar2Par),
TSimplify(CodeMul, CodeNeg, CodeIgnore, CodeNeg, CodeMul, CodePar1Par, CodePar2),

TSimplify(CodeDiv, 0, CodeIgnore, 0),
TSimplify(CodeDiv, CodeIgnore, 1, CodePar1),

TSimplify(CodePow, 1, CodeIgnore, 1),
TSimplify(CodePow, CodeIgnore, 0, 1),
TSimplify(CodePow, CodeIgnore, 1, CodePar1),

TSimplify(CodeLn, Codee, CodeIgnore, 1),
TSimplify(CodeSqr, CodeSqrt, CodeIgnore, CodePar1Par)
};

  BOOST_ASSERT(Index < sizeof(Table)/sizeof(Table[0]));
  return Table[Index];
}

//---------------------------------------------------------------------------
//Returns true if the two ranges are equal; the iterators are updated
bool LoopIsEqual(std::list<TElem>::iterator &Iter1, std::list<TElem>::iterator &Iter2)
{
  if(*Iter1 != *Iter2)
    return false;

  unsigned Arguments = FunctionArguments(*Iter1);
  ++Iter1;
  ++Iter2;

  //Check that all arguments are equal
  for(unsigned I = 0; I < Arguments; I++)
    if(!LoopIsEqual(Iter1, Iter2))
      return false;
  return true;
}
//---------------------------------------------------------------------------
//Returns true if the two iterators points to equal ranges
bool IsEqual(std::list<TElem>::iterator Iter1, std::list<TElem>::iterator Iter2)
{
  return LoopIsEqual(Iter1, Iter2);
}
//---------------------------------------------------------------------------
/** Optimizes the data elements in Data
 */
void TFuncData::Simplify()
{
  if(Data.empty())
    return;
  std::list<TElem> List(Data.begin(), Data.end());
  std::list<TElem>::iterator Iter = List.begin();
  SimplifyData(List, Iter);
  std::vector<TElem> NewData(List.begin(), List.end());
  Data.swap(NewData);
}
//---------------------------------------------------------------------------
/** Optimizes the data elements starting from Iter, which points into list.
 *  \param List: Elements to optimize.
 *  \param Iter: Element to start optimization at. If Iter is erased, Iter is updated to point to the element that is replacing it.
 *  \return An iterator to the element following Iter, jumping over parameters.
 */
std::list<TElem>::iterator SimplifyData(std::list<TElem> &List, std::list<TElem>::iterator &Iter)
{
  std::list<TElem>::iterator Begin1 = Iter;
  ++Begin1;

  if(IsConstant(*Iter))
    return Begin1;
  std::list<TElem>::iterator End;
  std::list<TElem>::iterator Begin2 = SimplifyData(List, Begin1);
  if(IsFunction1P(*Iter))
    End = Begin2;
  else if(IsFunction2P(*Iter))
    End = SimplifyData(List, Begin2);
  else if(IsFunction3P(*Iter))
  {
    std::list<TElem>::iterator Begin3 = SimplifyData(List, Begin2);
    End = SimplifyData(List, Begin3);
  }
  else if(IsFunctionVariableP(*Iter))
  {
    End = Begin1;
    for(unsigned I = 0; I < Iter->Arguments; I++)
      End = SimplifyData(List, End);   //Simplify each argument
    return End;
  }
  else
    throw EFuncError(ecInternalError);

  for(unsigned I = 0; I < 25/*sizeof(SimplifyTable) / sizeof(SimplifyTable[0])*/; I++)
  {
    const TSimplify &Entry = SimplifyTable(I);
    if(*Iter == Entry.Function && (Entry.Par1 == CodeIgnore || *Begin1 == Entry.Par1) &&
      (Entry.Par2 == CodeIgnore || *Begin2 == Entry.Par2 ||
      (Entry.Par2 == CodePar1 && IsEqual(Begin1, Begin2))))
    {
      bool RemovePar1 = true;
      bool RemovePar2 = true;
      std::list<TElem>::iterator Pos = Begin1;

      for(int N = 0; N < 4; N++)
        switch(Entry.Result[N].Ident)
        {
          case CodePar1:
            RemovePar1 = false;
            Pos = Begin2;
            break;

          case CodePar2:
            RemovePar2 = false;
            break;

          case CodePar1Par:
            List.erase(Begin1++);
            RemovePar1 = false;
            Pos = Begin2;
            break;

          case CodePar2Par:
            List.erase(Begin2++);
            RemovePar2 = false;
            break;

          case CodeNull:
            break;

          default:
            List.insert(Pos, Entry.Result[N]);
        }

      if(RemovePar1)
        List.erase(Begin1, Begin2);
      if(RemovePar2)
        List.erase(Begin2, End);
      List.erase(Iter++);
      return End;
    }
 }

  if(Iter->Ident == CodeSub && Begin1->Ident == CodeNumber && Begin2->Ident == CodeNumber)
  {
    *Iter = TElem(CodeNumber, Begin1->Number - Begin2->Number);
    List.erase(Begin1, End);
  }
  else if(Iter->Ident == CodeAdd && Begin1->Ident == CodeNumber && Begin2->Ident == CodeNumber)
  {
    *Iter = TElem(CodeNumber, Begin1->Number + Begin2->Number);
    List.erase(Begin1, End);
  }
  else if(Iter->Ident == CodeMul)
  {
    //5*2 = 10
    if(Begin1->Ident == CodeNumber && Begin2->Ident == CodeNumber)
    {
      Begin1->Number *= Begin2->Number;
      List.erase(Iter);
      List.erase(Begin2);
      Iter = Begin1;
    }
    else
    {
      std::list<TElem>::iterator Par1 = Begin1;
      std::list<TElem>::iterator Par2 = Begin2;
      ++Par1;
      ++Par2;

      // (5*A)*(2*B) = 10*(A*B)
      if(Begin1->Ident == CodeMul && Par1->Ident == CodeNumber && Begin2->Ident == CodeMul && Par2->Ident == CodeNumber)
      {
        List.insert(Begin1, TElem(CodeNumber, Par1->Number * Par2->Number));
        List.erase(Par1);
        List.erase(Begin2);
        List.erase(Par2);
      }
      // 5*(2*A) = 10*A
      else if(Begin1->Ident == CodeNumber && Begin2->Ident == CodeMul && Par2->Ident == CodeNumber)
      {
        Begin1->Number *= Par2->Number;
        List.erase(Begin2);
        List.erase(Par2);
      }
      //(5*A)*2 = 10*A
      else if(Begin1->Ident == CodeMul && Par1->Ident == CodeNumber && Begin2->Ident == CodeNumber)
      {
        Par1->Number *= Begin2->Number;
        List.erase(Begin1);
        List.erase(Begin2);
      }
      //A*(5*B) = 5*(A*B)
      else if(Begin2->Ident == CodeMul && Par2->Ident == CodeNumber)
      {
        List.insert(Begin1, *Par2);
        List.insert(Begin1, CodeMul);
        List.erase(Par2);
        List.erase(Begin2);
      }
      //A*5 = 5*A
      else if(Begin2->Ident == CodeNumber)
      {
        List.insert(Begin1, *Begin2);
        List.erase(Begin2);
      }
      //(5*A)*B = 5*(A*B)
      else if(Begin1->Ident == CodeMul && Par1->Ident == CodeNumber)
        std::swap(*Begin1, *Par1);
    }
  }
  return End;
}
//---------------------------------------------------------------------------
} //namespace Func32











