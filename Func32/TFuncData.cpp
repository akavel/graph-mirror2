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
#include <boost/spirit/phoenix/operators.hpp>
#include <boost/spirit/phoenix/primitives.hpp>
#include <boost/function.hpp>
#include <limits>
//---------------------------------------------------------------------------
namespace Func32
{
struct TFuncTable
{
  const char *FuncName;
  boost::function1<bool, unsigned> ArgCountValid;
  const char *Definition;
  const char *Dif;
  TFuncTable(const char *AFuncName, const boost::function1<bool, unsigned> &AArgCountValid, const char *ADefinition = NULL, const char *ADif = NULL)
    : FuncName(AFuncName), ArgCountValid(AArgCountValid), Definition(ADefinition), Dif(ADif) {}
};

using phoenix::arg1;

inline const TFuncTable& FuncTable(TIdent Ident)
{
static const TFuncTable Table[] = {
/*CodeSin*/         TFuncTable("sin",   arg1 == 1, NULL, "cos(x)*dx"),
/*CodeCos*/         TFuncTable("cos",   arg1 == 1, NULL, "-sin(x)*dx"),
/*CodeTan*/         TFuncTable("tan",   arg1 == 1, NULL, "(1+tan(x)^2)*dx"),
/*CodeASin*/        TFuncTable("asin",  arg1 == 1, NULL, "1/sqrt(1-x^2)*dx"),
/*CodeACos*/        TFuncTable("acos",  arg1 == 1, NULL, "-1/sqrt(1-x^2)*dx"),
/*CodeATan*/        TFuncTable("atan",  arg1 == 1, NULL, "1/(1+x^2)*dx"),

/*CodeLog*/         TFuncTable("log",   arg1 == 1, NULL, "1/(x*ln(10))*dx"),
/*CodeLn*/          TFuncTable("ln",    arg1 == 1, NULL, "1/x*dx"),
/*CodeExp*/         TFuncTable("exp",   arg1 == 1, NULL, "exp(x)*dx"),
/*CodeSqr*/         TFuncTable("sqr",   arg1 == 1, NULL, "2x*dx"),
/*CodeSqrt*/        TFuncTable("sqrt",  arg1 == 1, NULL, "0.5/sqrt(x)*dx"),
/*CodeFact*/        TFuncTable("fact",  arg1 == 1),
/*CodeSign*/        TFuncTable("sign",  arg1 == 1),
/*CodeU*/           TFuncTable("u",     arg1 == 1),
/*CodeNeg*/         TFuncTable("",      0,         NULL, "-dx"), //Argument and name is not used
/*CodeSinh*/        TFuncTable("sinh",  arg1 == 1, NULL, "cosh(x)*dx"),
/*CodeCosh*/        TFuncTable("cosh",  arg1 == 1, NULL, "sinh(x)*dx"),
/*CodeTanh*/        TFuncTable("tanh",  arg1 == 1, NULL, "1/cosh(x)^2*dx"),
/*CodeASinh*/       TFuncTable("asinh", arg1 == 1, NULL, "1/sqrt(x^2+1)*dx"),
/*CodeACosh*/       TFuncTable("acosh", arg1 == 1, NULL, "1/sqrt(x^2-1)*dx"),
/*CodeATanh*/       TFuncTable("atanh", arg1 == 1, NULL, "1/(1-x^2)*dx"),
/*CodeAbs*/         TFuncTable("abs",   arg1 == 1, NULL, "sign(x)*dx"),
/*CodeArg*/         TFuncTable("arg",   arg1 == 1),
/*CodeConj*/        TFuncTable("conj",  arg1 == 1),
/*CodeRe*/          TFuncTable("re",    arg1 == 1),
/*CodeIm*/          TFuncTable("im",    arg1 == 1),
/*CodeTrunc*/       TFuncTable("trunc", arg1 == 1),
/*CodeFract*/       TFuncTable("fract", arg1 == 1),
/*CodeCeil*/        TFuncTable("ceil",  arg1 == 1),
/*CodeFloor*/       TFuncTable("floor", arg1 == 1),
/*CodeNot*/         TFuncTable("not",   arg1 == 1),
/*CodeGamma*/       TFuncTable("gamma", arg1 == 1),
/*CodeW*/           TFuncTable("w",     arg1 == 1, NULL, "W(x)/(x*(1+W(x)))*dx"),
/*CodeZeta*/        TFuncTable("zeta",  arg1 == 1),

/*CodeCsc*/         TFuncTable("csc",   arg1 == 1, "1/sin(x)", "-csc(x)*cot(x)*dx"),
/*CodeSec*/         TFuncTable("sec",   arg1 == 1, "1/cos(x)", "sec(x)*tan(x)*dx"),
/*CodeCot*/         TFuncTable("cot",   arg1 == 1, "1/tan(x)", "-csc(x)^2*dx"),
/*CodeACsc*/        TFuncTable("acsc",  arg1 == 1, "asin(1/x)", "-1/(abs(x)*sqrt(x^2-1))*dx"),
/*CodeASec*/        TFuncTable("asec",  arg1 == 1, "acos(1/x)", "1/(abs(x)*sqrt(x^2-1))*dx"),
/*CodeACot*/        TFuncTable("acot",  arg1 == 1, "if(x, atan(1/x), pi/2)", "-1/(1+x^2)*dx"),
/*CodeCsch*/        TFuncTable("csch",  arg1 == 1, "1/sinh(x)", "-csch(x)*coth(x)*dx"),
/*CodeSech*/        TFuncTable("sech",  arg1 == 1, "1/cosh(x)", "-sech(x)*tanh(x)*dx"),
/*CodeCoth*/        TFuncTable("coth",  arg1 == 1, "1/tanh(x)", "-csch(x)^2*dx"),
/*CodeACsch*/       TFuncTable("acsch", arg1 == 1, "asinh(1/x)", "-1/(abs(x)*sqrt(1+x^2))*dx"),
/*CodeASech*/       TFuncTable("asech", arg1 == 1, "acosh(1/x)", "-1/(abs(x)*sqrt(1-x^2))*dx"),
/*CodeACoth*/       TFuncTable("acoth", arg1 == 1, "atanh(1/x)", "1/(1-x^2)*dx"),

/*CodeAdd*/         TFuncTable("",      0, NULL, "dx+dx2"),
/*CodeSub*/         TFuncTable("",      0, NULL, "dx-dx2"),
/*CodeMul*/         TFuncTable("",      0, NULL, "dx*x2+x*dx2"),
/*CodeDiv*/         TFuncTable("",      0, NULL, "(dx*x2-x*dx2)/x2^2"),
/*CodePow*/         TFuncTable("",      0, NULL, "x2*x^(x2-1)*dx+x^x2*ln(x)*dx2"),
/*CodeCompare1*/    TFuncTable("",      0),
/*CodeAnd*/         TFuncTable("",      0),
/*CodeOr*/          TFuncTable("",      0),
/*CodeXor*/         TFuncTable("",      0),
/*CodeMod*/         TFuncTable("mod",   arg1 == 2),
/*CodeRound*/       TFuncTable("round", arg1 == 2),
/*CodeLogB*/        TFuncTable("logb",  arg1 == 2, NULL, "dx/(ln(x2)*x) - (ln(x)*dx2)/(x2*ln(x2)^2)"),
/*CodeRoot*/        TFuncTable("root",  arg1 == 2, NULL, "root(x, x2) * (dx2/(x2*x) - ln(x2)/x^2*dx)"),
/*CodeBeta*/        TFuncTable("beta",  arg1 == 2),

/*CodeIf*/          TFuncTable("if",    arg1 == 3),
/*CodeRange*/       TFuncTable("range", arg1 == 3),
/*CodeIntegrate*/   TFuncTable("integrate", arg1 == 3),
/*CodeSum*/         TFuncTable("sum",   arg1 == 3),
/*CodeCompare2*/    TFuncTable("",      arg1 == 3),

/*CodeMin*/         TFuncTable("min",   arg1 >= 2),
/*CodeMax*/         TFuncTable("max",   arg1 >= 2),
/*CodeIfSeq*/       TFuncTable("ifseq", arg1 >= 2),
/*CodeCustom*/      TFuncTable("",      0),
/*CodeDNorm*/       TFuncTable("dnorm", arg1 == 1 || arg1 == 3, "exp(-sqr(x-x2)/(2sqr(x3))) / (x3*sqrt(2pi))"),
};

  assert(Ident >= FirstFunction1P && Ident <= LastFunction);
  return Table[Ident - FirstFunction1P];
}

/** Return name of function corresponding to the given ident.
 *  Ident: Id to lookup.
 */
const char* FunctionName(TIdent Ident)
{
  return FuncTable(Ident).FuncName;
}

/** Return name of function/constant in the element.
 *  \param Elem: Element to find name for.
 */
const char* FunctionName(const TElem &Elem)
{
  if(Elem.Ident == CodeCustom)
    return Elem.Text;
  return FunctionName(Elem.Ident);
}

/** Return definition of function given by Ident. Might return NULL.
 *  \param Ident: Function to return definition for.
 */
const char* FunctionDefinition(TIdent Ident)
{
  return FuncTable(Ident).Definition;
}

/** Return differentiation of function given by Ident. The differentiation is done at the first call.
 *  \param Ident: Identifies the function to return the first derivative for.
 */
const TFuncData& GetDif(TIdent Ident)
{
  static std::vector<TFuncData> Table;
  assert(Ident >= FirstFunction1P && Ident <= LastFunction);
  if(Table.empty())
  {
    TSymbolList SymbolList;
    SymbolList.Add("dx");
    SymbolList.Add("x2");
    SymbolList.Add("dx2");
    SymbolList.Add("x3");
    SymbolList.Add("dx3");

    Table.reserve(LastFunction-FirstFunction1P+1);
    std::vector<std::string> Variable(1, "x");

    for(unsigned Ident = FirstFunction1P; Ident <= LastFunction; Ident++)
      if(FuncTable(static_cast<TIdent>(Ident)).Dif)
        Table.push_back(TFuncData(FuncTable(static_cast<TIdent>(Ident)).Dif, Variable, SymbolList));
//      else if(FuncTable(static_cast<TIdent>(Ident)).Definition)
//        Table.push_back(TFuncData(FuncTable(static_cast<TIdent>(Ident)).Dif, Variable, SymbolList)).MakeDif;
      else
        Table.push_back(TFuncData());
  }
  return Table[Ident - FirstFunction1P];
}

//---------------------------------------------------------------------------
/** Verifies if the number of arguments is valid for the given function.
 *  \param Ident: Function to validate argument count for.
 *  \param Args: Argument count to verify.
 */
bool ArgCountValid(TIdent Ident, unsigned Args)
{
  if(Ident < FirstFunction1P)
    return Args == 0;
  return FuncTable(Ident).ArgCountValid(Args);
}
//---------------------------------------------------------------------------
/** Copy another function into this object and replace all arguments with copies of custom functions.
 *  This is used as part of differentiation.
 *  \param Iter: Iterator pointing to first element in function to copy
 *  \param Args: Vector of iterators pointing to functions to replace arguments with.
 *  \todo Throw exception on recursive functions
 */
void TFuncData::CopyReplace(TConstIterator Iter, const std::vector<TConstIterator> &Args)
{
  DEBUG_LOG(std::clog << "CopyReplace: " << MakeText(Iter));
  DEBUG_LOG(for(unsigned I = 0; I < Args.size(); I++) std::clog << ";   Arg" << I << "=" << MakeText(Args[I]));
  DEBUG_LOG(std::clog << std::endl);

  TConstIterator End = FindEnd(Iter);
  for(; Iter != End; ++Iter)
    if(Iter->Ident == CodeCustom && Iter->Arguments > 0)
    {
      std::vector<TConstIterator> NewArgs;
      NewArgs.push_back(Iter + 1);
      for(unsigned I = 1; I < Iter->Arguments; I++)
        NewArgs.push_back(FindEnd(NewArgs.back()));
      CopyReplace(Iter->FuncData->Data.begin(), NewArgs);
      Iter = FindEnd(NewArgs.back()) - 1;
    }
    else if(Iter->Ident == CodeVariable && !Args.empty())
      CopyReplace(Args[Iter->Arguments], std::vector<TConstIterator>());
    else
      Data.push_back(*Iter);
}
//---------------------------------------------------------------------------
/** Replace all occurences of OldElem with NewElem.
 *  \param OldElem: Elements to replace.
 *  \param NewElem: Element to replace with.
 */
void TFuncData::Replace(const TElem &OldElem, const TElem &NewElem)
{
  std::replace(Data.begin(), Data.end(), OldElem, NewElem);
}
//---------------------------------------------------------------------------
/** Update data to use functions/constants in a new symbol list. This will loop through all the elements, and replace all
 *  custom functions/constants with ones with the same name in the symbol list.
 *  \param SymbolList: List of symbols to reaplce exiting ones with.
 */
bool TFuncData::Update(const TSymbolList &SymbolList)
{
  bool Result = true;
  for(std::vector<TElem>::iterator Iter = Data.begin(); Iter != Data.end(); ++Iter)
    if(Iter->Ident == CodeCustom)
    {
      Iter->FuncData = SymbolList.Get(Iter->Text).FuncData;
      if(!Iter->FuncData)
        Result = false;
    }
  return Result;
}
//---------------------------------------------------------------------------
/** Add a copy of a TFuncData object to the end of this object.
 *  \param FuncData: Data to add.
 *  \throw EFuncError: Thrown if FuncData is empty.
 */
void TFuncData::Add(const TFuncData &FuncData)
{
  if(FuncData.IsEmpty())
    throw EFuncError(ecNoFunc);
  Data.insert(Data.end(), FuncData.Data.begin(), FuncData.Data.end());
}
//---------------------------------------------------------------------------
} //namespace Func32

