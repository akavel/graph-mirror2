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
#include <cmath>
#include <cerrno>
#include <limits>

//Disable Warnings: "Condition is always true/false", "Unreachable code"
#pragma warn -8008
#pragma warn -8066
//---------------------------------------------------------------------------
#define EULER 2.718281828459045235360287L
#define PI    3.141592653589793238462643L
namespace Func32
{
//The using derectives are necessary because names in std:: will not be used if they are found in Func32::
using namespace std;
using std::pow;
using std::fmod;
using std::asin;
using std::acos;
using std::atan;
using std::floor;
using std::ceil;
using std::min;
using std::max;
using std::log;
using std::sqrt;

const unsigned MaxRecursion = 100; //Maximum number of recursive calls allowed
unsigned IntegrateSteps = 100; //Number of steps used by the integrate function

/** Delay creation of table until first use. The rest of the library must be initialized first
 *  \param Ident: Id to get data for.
 */
inline const TFuncData& FunctionTable(TIdent Ident)
{
//Table of functions defined by equations
  static std::vector<TFuncData> Table;
  if(Table.empty())
    for(TIdent Ident = CodeCsc; Ident <= CodeACoth; Ident = static_cast<TIdent>(Ident+1))
      Table.push_back(TFuncData(FunctionDefinition(Ident)));

  BOOST_ASSERT(Ident >= CodeCsc && Ident <= CodeACoth);
  return Table[Ident - CodeCsc];
}
//---------------------------------------------------------------------------
/** Generic operator! which returns true if the complex number is 0
 *  \param C: Complex number to check
 */
template<typename T>
inline bool operator!(const std::complex<T> &C)
{
  return !real(C) && !imag(C);
}
//---------------------------------------------------------------------------
// Returns the number itself; Exists for generic use with real(std::complex<T>)
/*
inline long double real(long double x)
{
  return x;
}
//---------------------------------------------------------------------------
// Returns 0. Exists for generic use with imag(std::complex<T>)
inline long double imag(long double x)
{
  return 0;
}
*/
//---------------------------------------------------------------------------
//Returns the number itself; Exists for generic use with arg(std::complex<T>)
/*
inline long double arg(long double x)
{
  return x >= 0 ? 0 : PI;
}
*/
//---------------------------------------------------------------------------
//Returns the number itself; Exists for generic use with conj(std::complex<T>)
inline long double Conj(long double x)
{
  return x;
}
//---------------------------------------------------------------------------
inline TComplex Conj(TComplex x)
{
  return conj(x);
}
//---------------------------------------------------------------------------
//Returns the inverse hyperbolic sine to x
template<typename T>
inline T asinh(T x)
{
  return log(x + sqrt(x*x + 1.0L));
}
//---------------------------------------------------------------------------
//Returns the inverse hyperbolic cosine to x
template<typename T>
inline T acosh(T x)
{
  T Temp = sqrt(x*x - 1.0L);
  if(errno)
    return std::numeric_limits<T>::quiet_NaN();
  return log(x + Temp);
}
//---------------------------------------------------------------------------
//Returns the inverse hyperbolic tangent to x
template<typename T>
inline T atanh(T x)
{
  if(x != 1.0L)
  {
    T Result = log((1.0L + x) / (1.0L - x));
    if(errno == 0)
      return Result * 0.5L;
  }
  errno = SING;
  return std::numeric_limits<T>::quiet_NaN();
}
//---------------------------------------------------------------------------
//Returns the inverse sine to x
template<typename T>
inline std::complex<T> asin(const std::complex<T> &C)
{
  if(!imag(C))
    return std::asin(real(C));
  errno = DOMAIN;
  return 0;
}
//---------------------------------------------------------------------------
//Returns the inverse cosine to x
template<typename T>
inline std::complex<T> acos(const std::complex<T> &C)
{
  if(!imag(C))
    return std::acos(real(C));
  errno = DOMAIN;
  return 0;
}
//---------------------------------------------------------------------------
//Returns the inverse tangent to x
template<typename T>
inline std::complex<T> atan(const std::complex<T> &C)
{
  if(!imag(C))
    return std::atan(real(C));
  errno = DOMAIN;
  return 0;
}
//---------------------------------------------------------------------------
//Returns x without the decimals
template<typename T>
inline T Trunc(const T &C)
{
  return static_cast<long long>(C);
}

template<typename T>
inline std::complex<T> Trunc(const std::complex<T> &C)
{
  return std::complex<T>(Trunc(C.real()), Trunc(C.imag()));
}
//---------------------------------------------------------------------------
template<typename T>
inline std::complex<T> ceil(const std::complex<T> &C)
{
  return std::complex<T>(std::ceil(C.real()), std::ceil(C.imag()));
}
//---------------------------------------------------------------------------
template<typename T>
inline std::complex<T> floor(const std::complex<T> &C)
{
  return std::complex<T>(std::floor(C.real()), std::floor(C.imag()));
}
//---------------------------------------------------------------------------
template<typename T>
inline std::complex<T> fmod(const std::complex<T> &C1, const std::complex<T> &C2)
{
  if(C1.imag() || C2.imag())
    errno = DOMAIN;
  return std::fmod(C1.real(), C2.real());
}
//---------------------------------------------------------------------------
//Return a number with the min real part and min imag part
template<typename T>
inline std::complex<T> min(const std::complex<T> &C1, const std::complex<T> &C2)
{
  return std::complex<T>(std::min(C1.real(), C2.real()), std::min(C1.imag(), C2.imag()));
}
//---------------------------------------------------------------------------
//Return a number with the max real part and max imag part
template<typename T>
inline std::complex<T> max(const std::complex<T> &C1, const std::complex<T> &C2)
{
  return std::complex<T>(std::max(C1.real(), C2.real()), std::max(C1.imag(), C2.imag()));
}
//---------------------------------------------------------------------------
//Rounds Number to the given number of decimals
template<typename T>
inline T Round(const T &Number, int Decimals)
{
  T DecMul = pow(10.0, Decimals);
  return Trunc(Number * DecMul + (Number < 0 ? -0.5L : 0.5L)) / DecMul;
}
//---------------------------------------------------------------------------
//Rounds Number to the given number of decimals
template<typename T>
inline std::complex<T> Round(const std::complex<T> &Number, int Decimals)
{
  return std::complex<T>(Round(real(Number), Decimals), Round(imag(Number), Decimals));
}
//---------------------------------------------------------------------------
//Calculate Greatest Common Divisor
int CalcGcd(int a, int b)
{
  if(b > a)
    return CalcGcd(b, a);

  if(b == 0)
    return a;

  return CalcGcd(b, a%b);
}
//---------------------------------------------------------------------------
long double Fact(unsigned x)
{
  static std::vector<long double> List; //Cache for Fact(0)..Fact(100)
  if(List.empty())
  {
    List.reserve(101);
    List.push_back(1); //Fact(0) = 1
    for(unsigned I = 1; I <= 100; I++)
      List.push_back(List.back() * I);
  }

  if(x <= 100)
    return List[x];

  long double Result = List[100];
  do
    Result *= x;
  while(--x > 100);
  return Result;
}
//---------------------------------------------------------------------------
/** Calculates the gamma function ( n! = Gamma(n+1) ), Gamma(z) is not defined for z=0
 *  Implemented as Lanczos approximation from description on http://www.rskey.org/gamma.htm
 *  The alogirthm is a modification of the one found in Numerical Recipes in C
 *  \param z: The argument to the Gamma function
 */
template<typename T>
T Gamma(const T &z)
{
  if(real(z) < 0)
    return T(PI)/(z * Gamma(-z) * sin(-T(PI)*z));

  const int N = 7;
  long double q[N] = {75122.6331530, 80916.6278952, 36308.2951477, 8687.24529705, 1168.92649479, 83.8676043424,	2.50662827511};
  T Sum = 0;
  for(int n = 0; n < N; n++)
    Sum += q[n]*pow(z, n);

  T Product = 1;
  for(unsigned n = 0; n < N; n++)
    Product *= z + T(n);

  T Result = Sum / Product * pow(z + T(5.5), z + T(0.5)) * exp(-z - T(5.5));
  return Result;
}
//---------------------------------------------------------------------------
/** An implementation of the Omega function (Lambert's W function) where the
 *  inverse is given by f(W)=W*e^W
 *  W(z) is defined for all complex numbers except for real values z < -1/e
 *  \param z: A real or complex number to evaluate W(z) for
 *  \param ErrorCode: Set to an error code if evaluation fails. Unchanged otherwise
 *  \return The omega function W evaluated for z.
 */
template<typename T>
T Omega(T z, TErrorCode &ErrorCode)
{
/*
  const long double Limit = -1/EULER - LDBL_EPSILON;
  if(!TComplexTrait<T>::HasImagUnit && real(z) < Limit)
  {
    ErrorCode = ecComplexError; //Not defined for z<-1/e when we are not using complex numbers
    return -1;
  }
*/
  const double eps = 0.00001; //Accuracy
  T p, w;
  if(!z)
    return 0.0;
  errno = 0;
  if(abs(z) < 1)
  {
    p = sqrt(2.0L * (EULER * z + 1.0L));
    w = -1.0L + p-p*p/3.0L + 11.0L/72.0*p*p*p;
  }
  else
    w = log(z);

  if(errno)
  {
    ErrorCode = ecComplexError; //Not defined for z<-1/e when we are not using complex numbers
    return -1;
  }

  if(abs(z) > 3)
    w = w-log(w);
  else if(w == -1.0L) //Definition from MathWorld
    return -1;

  for(int i = 0; i < 20; i++)
  {
    T e = exp(w);
    T t = w*e-z;
    t = t/(e*(w+1.0L) - 0.5L*(w+2.0L)*t/(w+1.0L));
    w = w-t;
    if(abs(t) < eps*(1.0+abs(w)))
      return w;
  }

  ErrorCode = ecEvalFailed; //No accurate solution found
  return -1;
}
//---------------------------------------------------------------------------
long double Binomial(unsigned n, unsigned k)
{
  return Fact(n) / (Fact(n-k) * Fact(k));
}
//---------------------------------------------------------------------------
//Calculate Zeta(s)
//See http://mathworld.wolfram.com/RiemannZetaFunction.html
//and http://numbers.computation.free.fr/Constants/Miscellaneous/zetaevaluations.pdf
const int ZetaN = 50; //Increase to increase precision of Zerta() function
long double ZetaHelper(unsigned k)
{
  long double Result = 0;
  for(int j = k ; j <= ZetaN; j++)
    Result += Fact(ZetaN + j - 1) * pow(4.0, j) / (Fact(ZetaN - j) * Fact(2*j));
  return ZetaN * Result;
}

template<typename T>
T Zeta(T s, TErrorCode &ErrorCode)
{
  if(real(s) == 1)
  {
    ErrorCode = ecNotDefError;
    return 0;
  }

  if(s == 0.0L) //Special case that we cannot handle else
    return -0.5L;

  if(real(s) < 0.5)
    return pow(T(2), s) * pow(T(PI), s-1.0L) * sin(PI/2 * s) * Gamma(1.0L - s) * Zeta(1.0L - s, ErrorCode);

  T Result = 0;
  for(unsigned k = 1; k <= ZetaN; k++)
    Result += ((k % 2) ? ZetaHelper(k) : -ZetaHelper(k)) / pow(T(k), s);
  return 1.0L / (ZetaHelper(0) * (1.0L - pow(T(2.0L), 1.0L-s))) * Result;
}
//---------------------------------------------------------------------------
//Evaluate the normal distribution with given mean and standard deviation for the value x
//Deviation must not be 0
long double NormalDist(long double x, long double Mean, long double Deviation)
{
  const long double Sqrt2_SqrtPi = 0.564189583547756286948L * 0.707106781186547524401L; //1/sqrt(pi)*1/sqrt(2)
  return std::exp(-(x-Mean)*(x-Mean) / (2*Deviation*Deviation)) * Sqrt2_SqrtPi / Deviation;
}
//---------------------------------------------------------------------------
template<typename T>
bool Compare(const T &t1, const T &t2, TCompareMethod Compare, TErrorCode &ErrorCode)
{
  if(Compare != cmEqual && Compare != cmNotEqual)
    if(imag(t1) || imag(t2))
      ErrorCode = ecComplexError;
  switch(Compare)
  {
    case cmEqual:         return t1 == t2;
    case cmNotEqual:      return t1 != t2;
    case cmLess:          return real(t1) < real(t2);
    case cmGreater:       return real(t1) > real(t2);
    case cmLessEqual:     return real(t1) <= real(t2);
    case cmGreaterEqual:  return real(t1) >= real(t2);
    default:              return 0; //Error
  }
}
//---------------------------------------------------------------------------
//Returns a^(b/c) for real numbers
//c may not be 0
long double PowDiv(const long double &a, const long double &b, const long double &c)
{
  if(!b) //Define: 0^0 = 1
    return 1;

  int bInt = b;
  int cInt = c;

  //If a is negative and both b and c are integers
  if(a < 0 && bInt == b && cInt == c)
  {
    //Reduce the fraction till either b or c is odd
    while((bInt & 0x01) == 0 && (cInt & 0x01) == 0)
      bInt >>= 1, cInt >>= 1;

    if(cInt & 0x01)
    {
      //If both b and c are odd integers, a is negative (and we are using real numbers)
      if(bInt & 0x01)
        return -pow(-a, static_cast<long double>(bInt)/cInt);
      //If b is even, c is odd, and a is negative
      else
        return pow(-a, static_cast<long double>(bInt)/cInt);
    }
  }
  return pow(a, b/c);
}
//---------------------------------------------------------------------------
// Returns a^b for complex numbers
template<typename T>
std::complex<T> Pow(const std::complex<T> &a, const std::complex<T> &b)
{
  if(!b) //Define: 0^0 = 1
    return 1;

  //pow(0, n) is undefined if n has an imaginary part or is real and less than 0
  if(!a && !imag(b) && real(b) > 0)
    return 0; //pow(0,b) seems to give problems with complex numbers

  //Calculations with complex numbers may return a complex number if Temp<0,
  //e.g. (-2)^2 = 4+i4.3368E-19; Because of this
  //evaluating f(x)=x/(x^2-4) results in f(-2)=4.6117E+18i
  if(!imag(a) && !imag(b) && Trunc(b) == b)
    return pow(real(a), real(b));

  //Calculations with complex numbers may return a complex number if a<0,
  //and b is not an integer
  return pow(a, b);
}
//---------------------------------------------------------------------------
template<typename T>
inline T Pow(const T &a, const T &b)
{
  if(!b)
    return 1;
  return pow(a, b);
}
//---------------------------------------------------------------------------
template<typename T>
inline std::complex<T> PowDiv(const std::complex<T> &a, const std::complex<T> &b, const std::complex<T> &c)
{
  return Pow(a, b/c);
}
//---------------------------------------------------------------------------

template<typename T> struct TComplexTrait
{
  typedef TExtFunc TExtFuncCall;
  static const bool HasImagUnit = false;
  static const T ImagUnit() {return 0;}
  static TExtFuncCall GetFunction(TExtFunc f1, TExtFuncComplex f2) {return f1;}
};
template<typename T> struct TComplexTrait<std::complex<T> >
{
  typedef TExtFuncComplex TExtFuncCall;
  static const bool HasImagUnit = true;
  static const std::complex<T> ImagUnit() {return std::complex<T>(0, 1);}
  static TExtFuncCall GetFunction(TExtFunc f1, TExtFuncComplex f2) {return f2;}
};
//---------------------------------------------------------------------------
template<typename T>
inline T TFuncData::CalcFunc(TConstIterator Iter, TDynData<T> &DynData)
{
  return CalcF(Iter, DynData);
}
//---------------------------------------------------------------------------
/** Evaluate Data with x as variable and returns the result
 *  \param DynData: Contains information used for the evaluation
 *  \return The result of the evaluation
 */
template<typename T>
T TFuncData::CalcF(TDynData<T> &DynData) const
{
  try
  {
    if(Data.empty())
    {
      DynData.ErrorCode = ecNoFunc;
      return 0;
    }
    DynData.ErrorCode = ecNoError;
    return CalcFunc(Data.begin(), DynData);
  }
  catch(...)
  {
    DynData.ErrorCode = ecCalcError;
    return std::numeric_limits<T>::quiet_NaN();
  }
}

//---------------------------------------------------------------------------
/** Evaluate from the place pointed to by Iter with x as variable and returns the result
 *  \param Iter: Iterator pointing at the element to start the evaluation. Iter is updated to point to one after the last element.
 *  \param DynData: Contains information used for the evaluation.
 *  \return The result of the evaluation.
 */
template<typename T>
T TFuncData::CalcF(TConstIterator &Iter, TDynData<T> &DynData)
{
  TErrorCode &ErrorCode = DynData.ErrorCode;
  const TElem &Elem = *Iter++;
  switch(Elem.Ident)
  {
    case CodeNumber:
      return Elem.Number;

    case CodeVariable:
      BOOST_ASSERT(DynData.Args);
      return DynData.Args[Elem.Arguments];

    case CodePi:
      return PI;

    case Codee:
      return EULER;

    case Codei:
      if(TComplexTrait<T>::HasImagUnit)
        return TComplexTrait<T>::ImagUnit();
      ErrorCode = ecComplexError;
      return 0;

    case CodeUndef:
      ErrorCode = ecNotDefError;
      return 0;

    case CodeRand:
      //This will generate the same sequence if it is called from a new thread each time
      //unless randomize() is called each time a new thread starts
      return rand()/ static_cast<long double>(RAND_MAX);

/*      case CodeConst:
      if(DynData.SymbolList)
        if(boost::shared_ptr<const TFuncData> FuncData = DynData.SymbolList->Get(Elem.Text))
          return FuncData->CalcF(DynData);
      ErrorCode = ecSymbolNotFound;
      return 0;
*/
//      case CodeConst2:
//        return *((long double*)Elem.Arguments);

    case CodeIntegrate:
    {
      TConstIterator F = Iter;
      Iter = FindEnd(Iter);
      long double Min = real(CalcF(Iter, DynData));
      long double Max = real(CalcF(Iter, DynData));
      if(ErrorCode)
        return 0;
      return IntegrateT<T>(F, Min, Max, IntegrateSteps, DynData.Trigonometry, ErrorCode);
    }

    case CodeSum:
    case CodeProduct:
    {
      TConstIterator F = Iter;
      Iter = FindEnd(Iter);
      T Min = CalcF(Iter, DynData);
      T Max = CalcF(Iter, DynData);
      if(imag(Min) || imag(Max))
      {
        ErrorCode = ecComplexError;
        return 0;
      }
      T Sum = Elem.Ident == CodeSum ? 0 : 1;
      TDynData<T> TempDynData(DynData);
      T Value;
      TempDynData.Args = &Value;
      for(long double i = real(Min); i <= real(Max); i++)
      {
        Value = i;
        if(Elem.Ident == CodeSum)
          Sum += CalcFunc(F, TempDynData);
        else
          Sum *= CalcFunc(F, TempDynData);
      }
      return Sum;
    }

    case CodeCustom:
    {
      if(++DynData.Recursion > MaxRecursion)
      {
        ErrorCode = ecRecusionLimit;
        return 0;
      }

      std::vector<T> Values;
      for(unsigned I = 0; I < Elem.Arguments; I++)
      {
        Values.push_back(CalcF(Iter, DynData));
        if(ErrorCode != ecNoError)
          return 0;
      }

      if(Elem.FuncData)
      {
        const T *OldArgs = DynData.Args;
        DynData.Args = Values.empty() ? NULL : &Values[0];
        T Result = Elem.FuncData->CalcF(DynData);
        DynData.Args = OldArgs;
        DynData.Recursion--;
        return Result;
      }

      ErrorCode = ecSymbolNotFound;
      DynData.ErrorStr = Elem.Text.c_str();
      return 0;
    }

    case CodeExtFunc:
    {
      //Weird construct: This is just forwarding to another function
      TComplexTrait<T>::TExtFuncCall FuncCall = TComplexTrait<T>::GetFunction(Elem.ExtFunc, Elem.ExtFuncComplex);
      if(FuncCall)
        return FuncCall(Elem.Custom, DynData.Args, Elem.Arguments, DynData.Trigonometry);

      ErrorCode = ecSymbolNotFound;
//      DynData.ErrorStr = Elem.Text;
      return 0;
    }

    case CodeOr:
    {
      T Temp = CalcF(Iter, DynData);
      //An error is handles as evaluation to 0
      if(!!Temp && ErrorCode == ecNoError)
      {
        Iter = FindEnd(Iter);
        return 1;
      }
      ErrorCode = ecNoError;
      Temp = CalcF(Iter, DynData);
      if(!!Temp && ErrorCode == ecNoError)
        return 1;
      ErrorCode = ecNoError;
      return 0;
    }
  }

  T Temp = CalcF(Iter, DynData);
  if(ErrorCode != ecNoError)
    return 0;                    //! \bug I think we may need to adjust Iter here
  errno = 0;
  switch(Elem.Ident)
  {
    case CodeAdd:
      return Temp + CalcF(Iter, DynData);

    case CodeSub:
      return Temp - CalcF(Iter, DynData);

    case CodeMul:
    {
      T Temp2 = CalcF(Iter, DynData);
      return Temp * Temp2;
    }

    case CodeDiv:
    {
      T Temp2 = CalcF(Iter, DynData);
      if(!!Temp2)
        return Temp / Temp2;
      if(!ErrorCode)
        ErrorCode = ecDivByZero;
      return 0;
    }

    case CodePow:
    {
      T Temp2 = CalcF(Iter, DynData);
      Temp = Pow(Temp, Temp2);
      if(errno)
        ErrorCode = ecPowCalcError;
      return Temp;
    }

    case CodeRoot: //y=root(n,x)
    {
      T Temp2 = CalcF(Iter, DynData);
      if(!Temp)  //Cannot raise to root 0
      {
        ErrorCode = ecDivByZero;
        return 0;
      }

      Temp = PowDiv(Temp2, T(1), Temp);
      if(errno)
        ErrorCode = ecPowCalcError;
      return Temp;
    }

    case CodePowDiv:
    {  //Temp^(Temp2/Temp3)
      T Temp2 = CalcF(Iter, DynData);
      T Temp3 = CalcF(Iter, DynData);
      if(!Temp3)  //Cannot raise to root 0
      {
        ErrorCode = ecDivByZero;
        return 0;
      }

      Temp = PowDiv(Temp, Temp2, Temp3);
      if(errno)
        ErrorCode = ecPowCalcError;

      return Temp;
    }

    case CodeMod:
    {
      T Temp2 = CalcF(Iter, DynData);
      Temp = fmod(Temp, Temp2);
      if(errno)
        ErrorCode = ecComplexError;
      //The C++ implementation doesn't follow the documentation, which also seems to be wrong.
      //We want the sign of mod(m,n) to be the same as the sign of n.
      if(!!Temp && (real(Temp) >= 0) != (real(Temp2) >= 0))
        return Temp + Temp2;
      return Temp;
    }
    case CodeRound:
    {
      T Temp2 = CalcF(Iter, DynData);
      if(imag(Temp2))
      {
        ErrorCode = ecComplexError;
        return std::numeric_limits<T>::quiet_NaN();
      }
      return Round(Temp, static_cast<int>(real(Temp2)));
    }

    case CodeNeg:
      return -Temp;

    case CodeSin:
    case CodeCos:
    case CodeTan:
      if(DynData.Trigonometry == Degree)
        Temp *= PI / 180;
      switch(Elem.Ident)
      {
        case CodeSin: Temp = sin(Temp); break;
        case CodeCos: Temp = cos(Temp); break;
        case CodeTan:
          Temp = tan(Temp);  //Notice that this will throw an exception for PI/2 when using comlex numbers
          if(errno)
            ErrorCode = ecTanError;
      }

      if(errno && !ErrorCode)
        ErrorCode = ecTotalLoss; //We should also check the value of errno
      return Temp;

    case CodeASin:
    case CodeACos:
    case CodeATan:
    case CodeArg:
      switch(Elem.Ident)
      {
        case CodeASin:
          Temp = asin(Temp); break;
        case CodeACos:
          Temp = acos(Temp); break;
        case CodeATan:
          Temp = atan(Temp); break;
        case CodeArg:
          Temp = arg(Temp);
          if(errno)
          {
            ErrorCode = ecArgError;
            return 0;
          }
      }
      if(errno)
        ErrorCode = ecArcError;
      if(DynData.Trigonometry == Degree)
        Temp *= (180 / PI);
      return Temp;

    case CodeLog:
      Temp = log10(Temp);
      if(errno)
        ErrorCode = ecLogError;
      return Temp;

    case CodeLn:
      Temp = log(Temp);
      if(errno)
        ErrorCode = ecLogError;
      return Temp;

    case CodeLogB:
    {
      T Temp2 = CalcF(Iter, DynData);
      if(ErrorCode)
        return 0;
      if(imag(Temp2))
      {
        ErrorCode = ecComplexError;
        return 0;
      }
      Temp = log(Temp) / log(Temp2);
      if(errno || real(Temp2) <= 0)
        ErrorCode = ecLogError;
      return Temp;
    }

    case CodeSqr:
      return Temp * Temp;

    case CodeExp:
      Temp = exp(Temp);
      if(errno)
        ErrorCode = ecPowCalcError;
      return Temp;

    case CodeSqrt:
      Temp = sqrt(Temp);
      if(errno)
        ErrorCode = ecSqrtError;
      return Temp;

    case CodeFact:
      if(!imag(Temp) && !fmod(real(Temp), 1) && real(Temp) >= 0)
        return Fact(static_cast<unsigned>(real(Temp)));
      ErrorCode = ecFactError;
      return 0;

    case CodeSign:
      if(!Temp) //if(Temp == 0)
        return 0;
      if(TComplexTrait<T>::HasImagUnit)
        return Temp / abs(Temp); //As defined by MathWorld
      return real(Temp) < 0 ? -1 : 1; //Optimized version for real numbers

    case CodeU:
      if(imag(Temp))
        ErrorCode = ecNotDefError;
      return real(Temp) < 0 ? 0 : 1;

    case CodeSinh:
      if(real(Temp) < -8191) //BCC 5.6.4 has an error. It returns a positive number for sinh(x) when x < -8191
        ErrorCode = ecHugeValReturned;
      Temp = sinh(Temp);
      if(errno)
        ErrorCode = ecHugeValReturned;
      return Temp;

    case CodeCosh:
      Temp = cosh(Temp);
      if(errno)
        ErrorCode = ecHugeValReturned;
      return Temp;

    case CodeTanh:  return tanh(Temp);
    case CodeASinh: return asinh(Temp);
    case CodeACosh:
      Temp = acosh(Temp);
      if(errno)
        ErrorCode = ecACoshError;
      return Temp;

    case CodeATanh:
      Temp = atanh(Temp);
      if(errno)
        ErrorCode = ecATanhError;
      return Temp;

    case CodeAbs:   return abs(Temp);
    case CodeConj:  return Conj(Temp);
    case CodeRe:    return real(Temp);
    case CodeIm:    return imag(Temp);
    case CodeTrunc: return Trunc(Temp);
    case CodeFract: return Temp - Trunc(Temp);
    case CodeCeil:  return ceil(Temp);
    case CodeFloor: return floor(Temp);
    case CodeMin:
      for(unsigned I = 1; I < Elem.Arguments; I++)
        Temp = min(Temp, CalcF(Iter, DynData));
      return Temp;

    case CodeMax:
      for(unsigned I = 1; I < Elem.Arguments; I++)
        Temp = max(Temp, CalcF(Iter, DynData));
      return Temp;

    case CodeIf:
      if(!!Temp)
      {
        Temp = CalcF(Iter, DynData);
        Iter = FindEnd(Iter);
        return Temp;
      }
      else
      {
        Iter = FindEnd(Iter);
        return CalcF(Iter, DynData);
      }
    case CodeRange:
      return max(Temp, min(CalcF(Iter, DynData), CalcF(Iter, DynData)));

    case CodeCompare1:
    {
      T Temp2 = CalcF(Iter, DynData);
      return Compare(Temp, Temp2, Elem.Compare[0], ErrorCode);
    }

    case CodeCompare2:
    {
      T Temp2 = CalcF(Iter, DynData);
      T Temp3 = CalcF(Iter, DynData);
      return Compare(Temp, Temp2, Elem.Compare[0], ErrorCode) && Compare(Temp2, Temp3, Elem.Compare[1], ErrorCode);
    }

    case CodeNot:
      return !Temp;

    case CodeAnd:
      return !!Temp & !!CalcF(Iter, DynData); //Do not use logical and because of hort-circuit rule

    case CodeXor:
      return !!Temp ^ !!CalcF(Iter, DynData);

    case CodeCsc:
    case CodeSec:
    case CodeCot:
    case CodeASec:
    case CodeACsc:
    case CodeACot:
    case CodeCsch:
    case CodeSech:
    case CodeCoth:
    case CodeASech:
    case CodeACsch:
    case CodeACoth:
    {
      const T *OldArgs = DynData.Args;
      DynData.Args = &Temp;
      Temp = FunctionTable(Elem.Ident).CalcF(DynData);
      DynData.Args = OldArgs;
      return Temp;
    }

    case CodeGamma:
      if(!Temp)
      {
        ErrorCode = ecNotDefError;
        return 0;
      }
      return Gamma(Temp);

    case CodeBeta:
    {
      T Temp2 = CalcF(Iter, DynData);
      return Gamma(Temp) * Gamma(Temp2) / Gamma(Temp + Temp2); //Gamma will never return 0
    }

    case CodeW:
      return Omega(Temp, ErrorCode);

    case CodeZeta:
      return Zeta(Temp, ErrorCode);

    case CodeIfSeq:
      for(unsigned I = 1; I < Elem.Arguments; I += 2)
      {
        if(!!Temp)
        {
          Temp = CalcF(Iter, DynData);
          while(++I < Elem.Arguments)
            Iter = FindEnd(Iter);
          return Temp;
        }

        Iter = FindEnd(Iter);
        if(I+1 < Elem.Arguments)
          Temp = CalcF(Iter, DynData);
      }
      if(Elem.Arguments % 2 == 0)
        ErrorCode = ecNotDefError;
      return Temp;

    case CodeDNorm:
    {
      T Temp2;
      T Temp3;
      if(Elem.Arguments == 1)
        Temp2 = 0, Temp3 = 1;
      else
      {
        Temp2 = CalcF(Iter, DynData);
        if(ErrorCode)
          return 0;
        Temp3 = CalcF(Iter, DynData);
      }
      if(!ErrorCode && (imag(Temp) || imag(Temp2) || imag(Temp3)))
        ErrorCode = ecComplexError;
      if(!ErrorCode && real(Temp3) <= 0)
        ErrorCode = ecNotDefError;
      if(ErrorCode)
        return 0;
      return NormalDist(real(Temp), real(Temp2), real(Temp3));
    }

    default:
      ErrorCode = ecInternalError;
      return std::numeric_limits<T>::quiet_NaN();
  }
}
//---------------------------------------------------------------------------
/** Returns the numeric integrale of the functions pointed to by Func from Min to Max using the Simpson's Rule.
 *  \param Func: Iterator pointing to the place to start the evaluation.
 *  \param Min:  Start value
 *  \param Max:  End value
 *  \param n:    Number of steps; must be even.
 *  \param Trigonometry: Used to tell if we should use radians or degrees for trigonometric functions.
 *  \param ErrorCode: Filled with error information on return.
 */
template<typename T>
long double TFuncData::IntegrateT(TConstIterator Func, long double Min, long double Max, unsigned n, TTrigonometry Trigonometry, TErrorCode &ErrorCode)
{
  const long double h = (Max - Min) / n;  //Step size
  T Value = Max;
  TDynData<T> DynData(&Value, Trigonometry);
  long double Sum = GetReal(CalcFunc(Func, DynData), DynData);
  Value = Min;
  Sum += GetReal(CalcFunc(Func, DynData), DynData);
  for(unsigned i = 1; i < n; i += 2)
  {
    Value = Min + i * h;
    Sum += 4 * GetReal(CalcFunc(Func, DynData), DynData);
  }

  for(unsigned i = 2; i < n; i += 2)
  {
    Value = Min + i * h;
    Sum += 2 * GetReal(CalcFunc(Func, DynData), DynData);
  }

  ErrorCode = DynData.ErrorCode;
  return Sum * h / 3;
}
//---------------------------------------------------------------------------
/** Returns the numeric integrale of the function from Min to Max using the Simpson's Rule.
 *  \param Min:  Start value
 *  \param Max:  End value
 *  \param n:    Number of steps; must be even.
 *  \param Trigonometry: Used to tell if we should use radians or degrees for trigonometric functions.
 *  \throw ECalcError: If evaluation fails.
 */
long double TFuncData::Integrate(long double Min, long double Max, unsigned n, TTrigonometry Trigonometry) const
{
  TErrorCode ErrorCode;
  long double Result = IntegrateT<TComplex>(Data.begin(), Min, Max, n, Trigonometry, ErrorCode);
  if(ErrorCode)
    throw ECalcError(ErrorCode);
  return Result;
}
//---------------------------------------------------------------------------
} //namespace Func32

//Math error handler
//Called on any math errors; Just set errno
#ifdef __BORLANDC__
int _RTLENTRY _matherrl(_exceptionl *a)
{
  using namespace std;
  DEBUG_LOG(std::wclog << "Math error: " << a->name << "(" << a->arg1 << ", " << a->arg2 << ")" << std::endl);
  a->retval = 0;//NAN gives problems with log(-0)
//  a->retval = std::numeric_limits<long double>::quiet_NaN();
  errno = a->type;
  return 1;
}
#endif
//---------------------------------------------------------------------------
//Instantiate versions of the template calculation functions for
//long double and std::complex<long double>
//Ignore by Doxygen
#ifndef DOXYGEN
template long double Func32::TFuncData::CalcF<long double>(TDynData<long double>&) const;
#endif

