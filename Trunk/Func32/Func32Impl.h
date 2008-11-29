/* Copyright 2007 Ivan Johansen
 *
 * Func32 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef Func32ImplH
#define Func32ImplH
//---------------------------------------------------------------------------
#include <list>

#ifdef _DEBUG
  #include <iostream>
  #define DEBUG_LOG(x) x
#else
  #define DEBUG_LOG(x)
#endif

namespace Func32
{
enum TCompareMethod
{
  cmEqual,
  cmNotEqual,
  cmLess,
  cmGreater,
  cmLessEqual,
  cmGreaterEqual
};

enum TIdent
{
  CodeNull,       //!< Nothing
  CodeVariable,   //!< Element is an argument to the function
  CodeNumber,     //!< Element is a number
  Codee,          //!< Element is Eulers constant
  Codei,          //!< Element is the imaginary unit
  CodePi,         //!< Element indicating number PI
  CodeUndef,      //!< Element indicates undefined value
  CodeRand,       //!< Evaluates to a random number between 0 and 1
//  CodeConst,      //!< Element indicate a user defined constant
//  CodeConst2,

  FirstFunction1P,//!<Indicate first function with one parameter
  CodeSin = FirstFunction1P,//!<Element indicating sinus
  CodeCos,        //!< Element indicating cosinus
  CodeTan,        //!< Element indicating tangent
  CodeASin,       //!< Element indicating inverse sinus
  CodeACos,       //!< Element indicating inverse cosinus
  CodeATan,       //!< Element indicating inverse tangent
  CodeLog,        //!< Element indicating 10'th logarithm
  CodeLn,         //!< Element indicating natural logarithm
  CodeExp,        //!< Element indicating e^x
  CodeSqr,        //!< Element indicating square
  CodeSqrt,       //!< Element indicating square root
  CodeFact,       //!< Element indicating factorial
  CodeSign,       //!< Element indicating Sign
  CodeU,          //!< Element indicating u(t)
  CodeNeg,        //!< Element insdicates multiply by -1
  CodeSinh,       //!< Element indicating hyperbolic sine
  CodeCosh,       //!< Element indicating hyperbolic cosine
  CodeTanh,       //!< Element indicating hyperbolic tangent
  CodeASinh,      //!< Element indicating hyperbolic inverse sine
  CodeACosh,      //!< Element indicating hyperbolic inverse cosine
  CodeATanh,      //!< Element indicating hyperbolic inverse tangent
  CodeAbs,        //!< Element indicating absolute value
  CodeArg,        //!< Element indicating angle of complex number
  CodeConj,       //!< Element indicating complex conjugate; Swap sign of imaginary part
  CodeRe,         //!< Element indicating real part of complex number
  CodeIm,         //!< Element indicating imaginary part of complex number
  CodeTrunc,      //!< Remove decimals from parameter
  CodeFract,      //!< Return decimal part of number
  CodeCeil,       //!< Round up
  CodeFloor,      //!< Round down
  CodeNot,        //!< Return 1 if the argument is 0, else return 0
  CodeGamma,      //!< The gamma function (n! = gamma(n+1)
  CodeW,          //!< The Lambert W function (omega function)
  CodeZeta,       //!< The Riemann Zeta Function

  CodeCsc,        //!< Element indicating cosecant; csc(x)=1/sin(x)
  CodeSec,        //!< Element indicating secant; sec(x)=1/cos(x)
  CodeCot,        //!< Element indicating cotangent; cot(x)=1/tan(x)
  CodeACsc,       //!< Element indicating inverse cosecant
  CodeASec,       //!< Element indicating inverse secant
  CodeACot,       //!< Element indicating inverse cotangent
  CodeCsch,       //!< Element indicating hyperbolic cosecant; csch(x)=1/sinh(x)
  CodeSech,       //!< Element indicating hyperbolic secant; sech(x)=1/cosh(x)
  CodeCoth,       //!< Element indicating hyperbolic cotangent; coth(x)=1/tanh(x)
  CodeACsch,      //!< Element indicating inverse hyperbolic cosecant
  CodeASech,      //!< Element indicating inverse hyperbolic secant
  CodeACoth,      //!< Element indicating inverse hyperbolic cotangent

  FirstFunction2P,//!< Indicate first function with two parameters
  CodeAdd = FirstFunction2P, //!<Element is a +
  CodeSub,        //!< Element is a -
  CodeMul,        //!< Element is a *
  CodeDiv,        //!< Element is a /
  CodePow,        //!< Element is a ^ (power)

  CodeCompare1,   //!< Element is a compare between two arguments, for example "x < 5"

  CodeAnd,        //!< Return 1 if both arguments are different from 1; else return 0
  CodeOr,         //!< Return 1 if one of the two arguments are different from 0; else return 0
  CodeXor,        //!< Return 1 if one and only one of the two arguments are different from 0; else return 0
  CodeMod,        //!< Return modulus; the remainder of the first parameter divided by the second parameter
  CodeRound,      //!< Round the first argument to the number of decimals given by the second argument
  CodeLogB,       //!< Logarithm with base as second parameter
  CodeRoot,       //!< root(n,z): n'th root of z
  CodeBeta,       //!< The beta function (B(m, n) = gamma(m) * gamma(n) / gamma(m+n))

  FirstFunction3P,//!< Indicate first function with three parameters
  CodeIf = FirstFunction3P, //!< if(x,y,z): Returns y when x!=0; Returns z when x==0
  CodeRange,      //!< Range(x,y,z): returns max(x,min(y,z))
  CodeIntegrate,  //!< Integrate(f,min,max) returns the numeric integrale of f from min to max
  CodeSum,        //!< Sum(f,min,max) returns the sum of f(i) where i are integers in the range [min;max]
  CodeProduct,    //!< Prod(f,min,max) returns the product of f(i) where i are integers in the range [min;max]
  CodeCompare2,   //!< Element is a compare between three arguments, for example "3 < x < 27"
  CodePowDiv,     //!< Element is a^(b/c). This makes it possible to handle x^(1/3) correctly

  FirstFunctionVariableP, //!< Indicates first function with variable number or parameters
  CodeMin = FirstFunctionVariableP, //!< Return the lowest of the parameters
  CodeMax,        //!< Return the greatest of the parameters
  CodeIfSeq,      //!< ifseq(a,f1,b,f2,f3) returns if(a) f1 else if(b) f2 else f3
  CodeCustom,     //!< Indicates a custom function
  CodeExtFunc,    //!< Indicates a custom plugin function to call
  CodeDNorm,      //!< The normal distribution normaldist(x, mean, deviation)
  LastFunction = CodeDNorm, //!< Indicates last function

  CodePar1      = 100,  //!< Element indicating first parameter to function
  CodePar2,             //!< Element indicating second parameter to function
  CodeIgnore,           //!< Element indicating the element should not be compared when optimizing
  CodePar1Par,          //!< Element indicating the first argument of the first argument of the function
  CodePar2Par,          //!< Elemetn indicating the first argument of the second argument of the function
};
//---------------------------------------------------------------------------
inline unsigned FunctionArguments(TIdent Ident)
{
  if(Ident < FirstFunction1P)
    return 0;
  else if(Ident < FirstFunction2P)
    return 1;
  else if(Ident < FirstFunction3P)
    return 2;
  else if(Ident < FirstFunctionVariableP)
    return 3;
  return 0;
}
//---------------------------------------------------------------------------
struct TElem
{
  TIdent Ident;
  unsigned Arguments;
  boost::shared_ptr<TFuncData> FuncData;
  std::wstring Text;
  union
  {
    long double Number; //A value if Ident is CodeNumber
    TCompareMethod Compare[2];
    struct
    {
      TExtFunc ExtFunc;
      TExtFuncComplex ExtFuncComplex;
      void *Custom;
    };
  };

  TElem() : Ident(CodeNull), Arguments(0) {};
  TElem(TIdent AIdent) : Ident(AIdent), Arguments(FunctionArguments(AIdent)) {}
  TElem(TIdent AIdent, long double AVal) : Ident(AIdent), Number(AVal) {}
  TElem(TIdent AIdent, unsigned AArguments, int) : Ident(AIdent), Arguments(AArguments) {}
  TElem(long double AVal) : Ident(CodeNumber), Number(AVal) {}
  TElem(TIdent AIdent, const std::wstring &Str, unsigned Args=0, const boost::shared_ptr<TFuncData> &AFuncData = boost::shared_ptr<TFuncData>())
    : Ident(AIdent), Arguments(Args), FuncData(AFuncData), Text(Str) {}
  TElem(TCompareMethod Compare1) : Ident(CodeCompare1), Arguments(0) {Compare[0] = Compare1;}
  TElem(TCompareMethod Compare1, TCompareMethod Compare2) : Ident(CodeCompare2), Arguments(0) {Compare[0] = Compare1; Compare[1] = Compare2;}
  bool operator ==(const TElem &E) const {return Ident==E.Ident && (Ident==CodeNumber ? Number==E.Number : (Ident==CodeCustom ? Text == E.Text : Arguments==E.Arguments));}
  bool operator !=(const TElem &E) const {return !(*this == E);}
};
//---------------------------------------------------------------------------
template<typename T>
struct TDynData
{
  const T *Args;
  TTrigonometry Trigonometry;
  TErrorCode ErrorCode;
  const wchar_t *ErrorStr;
  unsigned Recursion;

  TDynData(const T *AArgs, TTrigonometry ATrigonometry)
    : Args(AArgs), Trigonometry(ATrigonometry), ErrorCode(ecNoError), ErrorStr(NULL), Recursion(0) {}
};

class TFuncData
{
  typedef std::vector<TElem>::iterator TIterator;
  typedef std::vector<TElem>::const_iterator TConstIterator;

  std::vector<TElem> Data;

  template<typename T>
  static T CalcF(TConstIterator &Iter, TDynData<T> &DynData);
  template<typename T>
  static T CalcFunc(TConstIterator Iter, TDynData<T> &DynData);
  template<typename T>
  T CalcF(TDynData<T> &DynData) const;

  template<typename T>
  static long double IntegrateT(TConstIterator Iter, long double Min, long double Max, unsigned n, TTrigonometry Trigonometry, TErrorCode &ErrorCode);
  void AddDif(TConstIterator Iter, const TElem &Var, TTrigonometry Trigonometry, unsigned Level);
  static std::wstring MakeText(TConstIterator Iter);
  static TConstIterator CreateText(TConstIterator Iter, std::wstring &Str, const std::vector<std::wstring> &Args);
  static TConstIterator CreateTextInPar(TConstIterator Iter, std::wstring &Str, const std::vector<std::wstring> &Args);
  bool CheckRecursive(std::vector<const TFuncData*> &FuncStack) const;
  bool CheckRecursive() const;
  static void HandleParseError(const EParseError &E, const wchar_t* Where, unsigned Pos);
  static void CopyReplace(std::vector<TElem> &List, TConstIterator Iter, const std::vector<std::vector<TElem> > &Args);

public:
  TFuncData() {}
  TFuncData(const TFuncData &FuncData) : Data(FuncData.Data) {}
  TFuncData(const std::wstring &Str){Parse(Str, std::vector<std::wstring>(1, L"x"));}
  TFuncData(const std::wstring &Str, const std::wstring &Variable) {Parse(Str, std::vector<std::wstring>(1, Variable));}
  TFuncData(const std::wstring &Str, const std::vector<std::wstring> &Args, const TSymbolList &SymbolList)
  {Parse(Str, Args, &SymbolList);}
  TFuncData(const std::wstring &Str, const std::wstring &Variable, const TSymbolList &SymbolList)
  {Parse(Str, std::vector<std::wstring>(1, Variable), &SymbolList);}
  TFuncData(const std::wstring &Str, const std::vector<std::wstring> &Args) {Parse(Str, Args);}

  void Parse(const std::wstring &Str, const std::vector<std::wstring> &Args, const TSymbolList *SymbolList = NULL);

  template<typename T>
  T Calc(const T *Args, TTrigonometry Trigonometry, ECalcError &CalcError) const
  {
    TDynData<T> DynData(Args, Trigonometry);
    T Result = CalcF(DynData);
    CalcError.ErrorCode = DynData.ErrorCode;
    if(DynData.ErrorStr)
      CalcError.Str = DynData.ErrorStr;
    return Result;
  }

  template<typename T>
  T Calc(const T *Args, TTrigonometry Trigonometry) const
  {
    TDynData<T> DynData(Args, Trigonometry);
    T Result = CalcF(DynData);
    if(DynData.ErrorCode)
      throw ECalcError(DynData.ErrorCode, DynData.ErrorStr ? DynData.ErrorStr : L"");
    return Result;
  }

  boost::shared_ptr<TFuncData> MakeDif(const TElem &Var, TTrigonometry Trigonometry) const;
  void Simplify();
  void ReplaceConst();
  void Replace(const TElem &OldElem, const TElem &NewElem);
  std::wstring MakeText(const std::vector<std::wstring> &Args) const;
  bool Update(const TSymbolList &SymbolList);

  bool IsEmpty() const {return Data.empty();}
  void Clear() {Data.clear();}
  void Add(const TElem &Elem) {Data.push_back(Elem);}
  void Add(const TFuncData &FuncData);
  void AddFront(const TElem &Elem) {Data.insert(Data.begin(), Elem);}
  bool operator==(const TFuncData &FuncData) const {return Data == FuncData.Data;}
  long double Integrate(long double Min, long double Max, unsigned n, TTrigonometry Trigonometry) const;
  TElem& Front() {BOOST_ASSERT(!Data.empty()); return Data.front();}
};
//---------------------------------------------------------------------------

std::list<TElem>::iterator SimplifyData(std::list<TElem> &List, std::list<TElem>::iterator &Iter);
std::list<TElem>::const_iterator ValidateData(std::list<TElem>::const_iterator Iter);
void ValidateData(const std::list<TElem> &Data);

const wchar_t* FunctionName(TIdent Ident);
const wchar_t* FunctionName(const TElem &Elem);
const wchar_t* FunctionDefinition(TIdent Ident);
bool ArgCountValid(TIdent Ident, unsigned Args);
const TFuncData& GetDif(TIdent Ident);
std::vector<TElem>::const_iterator FindEnd(std::vector<TElem>::const_iterator Iter);
std::wstring ToLower(const std::wstring &Str);

//---------------------------------------------------------------------------
inline bool IsConstant(const TElem &Elem)
{
  return Elem.Ident < FirstFunction1P || (Elem.Ident == CodeCustom && Elem.Arguments == 0);
}
//---------------------------------------------------------------------------
inline bool IsFunction1P(const TElem &Elem)
{
  return Elem.Ident >= FirstFunction1P && Elem.Ident < FirstFunction2P;
}
//---------------------------------------------------------------------------
inline bool IsFunction2P(const TElem &Elem)
{
  return Elem.Ident >= FirstFunction2P && Elem.Ident < FirstFunction3P;
}
//---------------------------------------------------------------------------
inline bool IsFunction3P(const TElem &Elem)
{
  return Elem.Ident >= FirstFunction3P && Elem.Ident < FirstFunctionVariableP;
}
//---------------------------------------------------------------------------
inline bool IsFunctionVariableP(const TElem &Elem)
{
  return Elem.Ident >= FirstFunctionVariableP && Elem.Ident <= LastFunction;
}
//---------------------------------------------------------------------------
inline bool IsOperator(const TElem &Elem)
{
  return Elem.Ident >= FirstFunction2P && Elem.Ident <= CodePow;
}
//---------------------------------------------------------------------------
inline unsigned FunctionArguments(const TElem &Elem)
{
  if(Elem.Ident < FirstFunction1P)
    return 0;
  else if(Elem.Ident < FirstFunction2P)
    return 1;
  else if(Elem.Ident < FirstFunction3P)
    return 2;
  else if(Elem.Ident < FirstFunctionVariableP)
    return 3;
  return Elem.Arguments;
}
//---------------------------------------------------------------------------
template<typename T2, typename T>
inline long double GetReal(T Number, TDynData<T2> &DynData)
{
  if(imag(Number))
    DynData.ErrorCode = ecComplexError;
  return real(Number);
}
//---------------------------------------------------------------------------
//Check if a is very close to zero
//From an article "The G3D Graphics Engine" by Morgan McGuire in CUJ Dec. 2004
inline bool IsZero(long double a)
{
  return a == 0 || std::abs(a) <= 0.0000001L * (std::abs(a) + 1);
}

template<typename T> inline T Sqr(T t)
{
  return t*t;
}

} //namespace Func32
//---------------------------------------------------------------------------
#endif
