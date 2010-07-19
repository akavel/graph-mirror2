//---------------------------------------------------------------------------
#pragma hdrstop
#include "../Func32.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdarg>
#include <limits>
#include <cmath>
#include <boost\math\special_functions\fpclassify.hpp>
//---------------------------------------------------------------------------
using namespace Func32;
using namespace std;

#define EULER 2.718281828459045235360287
#define PI    3.141592653589793238462643

#define SQRT2 1.41421356237
#define DEFAULT_FPU_CONTROL EM_INVALID | EM_DENORMAL | EM_OVERFLOW | EM_UNDERFLOW | EM_INEXACT | IC_AFFINE | RC_NEAR | PC_64
#define FPU_MASK MCW_EM | MCW_IC | MCW_RC | MCW_PC

const long double NaN = numeric_limits<long double>::quiet_NaN(); //0.0/0.0;
const long double INF = numeric_limits<long double>::infinity();
inline long double real(long double x) {return x;}
inline long double imag(long double x) {return 0;}

long double StrToDouble(const char *Str)
{
  istringstream Stream(Str);
  long double Number;
//  if(Stream >> Number)
//    return Number;
  sscanf(Str, "%Lf", &Number);
  return Number;
}

inline bool IsZero(long double a)
{
  return a == 0 || abs(a) <= 0.0000001L * (abs(a) + 1);
}

inline bool IsZero(TComplex c)
{
  return IsZero(real(c)) && IsZero(imag(c));
}

inline bool IsEqual(long double a, long double b)
{
  if(boost::math::isnan(a) && boost::math::isnan(b))
    return true;

  if(!boost::math::isfinite(a) || !boost::math::isfinite(b))
    return a == b;

  int a_exp, b_exp, exp;
  frexp(a, &a_exp);
  frexp(b, &b_exp);
  frexp(a - b, &exp);
  return IsZero(a-b) || (a_exp == b_exp && std::abs(exp - a_exp) > 40);
}

inline bool IsEqual(TComplex a, TComplex b)
{
  return IsEqual(real(a), real(b)) && IsEqual(imag(a), imag(b));
}

std::wstring ToWString(const std::string &Str)
{
  return std::wstring(Str.begin(), Str.end());
}

bool CompareFunc(const TFunc &f1, const TFunc &f2, double x)
{
  ECalcError E1;
  ECalcError E2;
  long double y1 = f1.CalcY(x, E1);
  long double y2 = f2.CalcY(x, E2);
  if(E1.ErrorCode != E2.ErrorCode)
    return false;
  if(E1.ErrorCode == ecNoError)
    return IsEqual(y1, y2);
  return true;
}

bool CompareFunc(const TFunc &f1, const TFunc &f2)
{
  double x[] = {-1000, -100, -10, -1, 0, 1, 10, 100, 1000};
  for(unsigned I = 0; I < sizeof(x)/sizeof(x[0]); I++)
    if(!CompareFunc(f1, f2, x[I]))
      return false;
  return true;
}

void TestText(const std::wstring &Str, const TSymbolList &SymbolList = TSymbolList())
{
  std::wstring Str2;
  try
  {
    TFunc Func(Str, L"x", SymbolList);
    Str2 = Func.MakeText();
    TFunc Func2(Str2, L"x", SymbolList);
    if(Func != Func2)
    {
      std::wcerr << "Failed to convert function back to text!" << std::endl;
      std::wcerr << "Function: " << Str << std::endl;
      std::wcerr << "Result:   " << Str2 << std::endl << std::endl << std::endl;
    }
  }
  catch(EFuncError &E)
  {
    std::wcerr << "Failed to convert function back to test!" << std::endl;
    std::wcerr << "Function:  " << Str << std::endl;
    std::wcerr << "Result:    " << Str2 << std::endl;
    std::wcerr << "ErrorCode: " << E.ErrorCode << std::endl << std::endl;
  }
}

template<typename T>
void TestEval(const std::wstring &Str, T x, T y, TTrigonometry Trig = Radian, const TSymbolList &SymbolList=TSymbolList())
{
  try
  {
    TFunc Func(Str, L"x", SymbolList);
    Func.SetTrigonometry(Trig);
    T f = Func.CalcY(x);
    if(!IsEqual(f, y))
    {
      wcerr << "Function:     " << Str << std::endl;
      wcerr << "x:            " << setprecision(10) << x << std::endl;
      wcerr << "Evaluated to: " << setprecision(10) << f << std::endl;
      wcerr << "Expected:     " << setprecision(10) << y << std::endl << std::endl;
    }
  }
  catch(EFuncError &E)
  {
    wcerr << "Function:  " << Str << std::endl;
    wcerr << "x:         " << setprecision(10) << x << std::endl;
    wcerr << "ErrorCode: " << E.ErrorCode << std::endl ;
    wcerr << "Expected:  " << setprecision(10) << y << std::endl << std::endl;
  }
}

void Test(const std::wstring &Str, long double x, long double y, TTrigonometry Trig = Radian, const std::wstring &ConvToText = L"", const TSymbolList &SymbolList=TSymbolList())
{
  TestEval<long double>(Str, x, y, Trig, SymbolList);
  TestEval<TComplex>(Str, x, y, Trig, SymbolList);
  TestText(ConvToText.empty() ? Str : ConvToText, SymbolList);
}

void Test(const std::string &Str, long double x, long double y, TTrigonometry Trig = Radian, const std::string &ConvToText = "", const TSymbolList &SymbolList=TSymbolList())
{
  Test(ToWString(Str), x, y, Trig, ToWString(ConvToText), SymbolList);
}

template<typename T>
void TestErrorEval(const std::wstring &Str, T x, Func32::TErrorCode Error, TTrigonometry Trig = Radian)
{
  try
  {
    TFunc Func(Str);
    Func.SetTrigonometry(Trig);
    T f = Func.CalcY(x);

    wcerr << "Function:       " << Str << std::endl;
    wcerr << "x:              " << setprecision(10) << x << std::endl;
    wcerr << "Evaluated to:   " << setprecision(10) << f << std::endl;
    wcerr << "Expected error: " << Error << std::endl << std::endl;
  }
  catch(EFuncError &E)
  {
    if(E.ErrorCode != Error)
    {
      wcerr << "Function:       " << Str << std::endl;
      wcerr << "x:              " << setprecision(10) << x << std::endl;
      wcerr << "Error code:     " << E.ErrorCode << std::endl;
      wcerr << "Expected error: " << Error << std::endl << std::endl;
    }
  }
}

void TestError(const std::wstring &Str, long double x, TErrorCode Error, TTrigonometry Trig = Radian)
{
  TestErrorEval<long double>(Str, x, Error, Trig);
  TestErrorEval<TComplex>(Str, x, Error, Trig);
}

void TestError(const std::string &Str, long double x, TErrorCode Error, TTrigonometry Trig = Radian)
{
  TestError(ToWString(Str), x, Error, Trig);
}

void TestTrendLine(Func32::TTrendType Type, const std::vector<TDblPoint> &P, const std::vector<double> &W, unsigned N, double Intercept, const std::wstring &Str)
{
  try
  {
    TFunc Func(Str);
    TFunc Result = TrendLine(Type, P, W, N, Intercept);
    if(!CompareFunc(Result, Func))
    {
      wcerr << "-- Trendline --" << endl;
      wcerr << "Expected trendline: f(x)=" << Str << std::endl;
      wcerr << "Evaluated to:       f(x)=" << std::setprecision(15) << Result << std::endl << std::endl;

    }
  }
  catch(EFuncError &E)
  {
    cerr << "-- Trendline --" << endl;
    cerr << "Error code:     " << E.ErrorCode << std::endl << std::endl;
  }
}

void TestCustomTrendLine(const std::wstring &Model, const std::vector<TDblPoint> &P, const std::vector<double> &W, const std::wstring &Str)
{
  try
  {
    std::vector<std::wstring> Unknowns = FindUnknowns(Model);
    std::vector<long double> Values(Unknowns.size(), 1);
    Unknowns.insert(Unknowns.begin(), L"x");
    TCustomFunc Func(Model, Unknowns);
    Regression(P, Func, Values, W);
    std::wstring Str2 = Func.ConvToFunc(Values, 0).MakeText(L"x", 5);
    if(Str != Str2)
    {
      wcerr << "-- Custom trendline --" << endl;
      wcerr << "Model:              f(x)=" << Model << std::endl;
      wcerr << "Expected trendline: f(x)=" << Str << std::endl;
      wcerr << "Evaluated to:       f(x)=" << Str2 << std::endl << std::endl;
    }
  }
  catch(EFuncError &E)
  {
    wcerr << "-- Custom trendline --" << endl;
    wcerr << "Model:      f(x)=" << Model << std::endl;
    wcerr << "Error code: " << E.ErrorCode << std::endl << std::endl;
  }
}

void TestTrendLineError(Func32::TTrendType Type, const TDblPoint *Points, unsigned Size, unsigned N, Func32::TErrorCode ErrorCode)
{
  try
  {
    std::vector<double> W;
    TrendLine(Type, std::vector<TDblPoint>(Points, Points + Size), W, N);

    cerr << "-- Trendline --" << endl;
    cerr << "Expected error code:     " << ErrorCode << endl << endl;
  }
  catch(EFuncError &E)
  {
    if(E.ErrorCode != ErrorCode)
    {
      cerr << "-- Trendline --" << endl;
      cerr << "Error code:     " << E.ErrorCode << endl << endl;
    }
  }
}


void TestDif(const std::wstring &f, const std::wstring &df, TTrigonometry Trig = Radian)
{
  try
  {
    TFunc Func(f, L"x", Trig);
    TFunc Dif = Func.MakeDif();
    TFunc Dif2(df, L"x", Trig);

    if(!CompareFunc(Dif, Dif2))
    {
      wcerr << "f(x)=" << f << std::endl;
      wcerr << "f'(x)=" << Dif.MakeText() << std::endl;
      wcerr << "Expected f'(x)=" << df << std::endl << std::endl;
    }
  }
  catch(EFuncError &E)
  {
    wcerr << "f(x)=" << f << std::endl;
    wcerr << "Expected f'(x)=" << df << std::endl;
    wcerr << "Error code: " << E.ErrorCode << std::endl << std::endl;
  }
}

void TestDif(const std::string &f, const std::string &df, TTrigonometry Trig = Radian)
{
  TestDif(ToWString(f), ToWString(df), Trig);
}

void TestDif(const std::wstring &Str, long double x, long double y, TTrigonometry Trig = Radian)
{
  try
  {
    TFunc Func(Str, L"x", Trig);
    TFunc Dif = Func.MakeDif();
    long double f = Dif(x);

    if(!IsEqual(f, y))
    {
      wcerr << "f(x)=" << Str << std::endl;
      wcerr << "f'(x)=" << Dif.MakeText() << std::endl;
      wcerr << "f'(" << x << ")=" << f << std::endl;
      wcerr << "Expected f'(" << x << ")=" << y << std::endl << std::endl;
    }
  }
  catch(EFuncError &E)
  {
    wcerr << "f(x)=" << Str << std::endl;
    wcerr << "Expected f'(" << x << ")=" << y << std::endl;
    wcerr << "Error code: " << E.ErrorCode << std::endl << std::endl;
  }
}

void TestDif(const std::string &Str, long double x, long double y, TTrigonometry Trig = Radian)
{
  TestDif(ToWString(Str), x, y, Trig);
}

void TestSimplify(const std::wstring &Str, const std::wstring &Str2)
{
  TFunc Func(Str);
  Func.Simplify();
  if(Func != TFunc(Str2))
  {
    wcerr << "f(x)=" << Str << std::endl;
    wcerr << "Simplified to: f(x)=" << Func.MakeText() << std::endl;;
    wcerr << "Expected: f(x)=" << Str2 << std::endl << std::endl;
  }
}

void TestSimplify(const std::string &Str, const std::string &Str2)
{
  TestSimplify(ToWString(Str), ToWString(Str2));
}

void TestCustom(const std::wstring &Str, const TArgType &Args, const std::vector<TComplex> &Values, const TComplex &Result)
{
  try
  {
    Func32::TCustomFunc Func(Str, Args);
    TComplex FuncResult = Func.Calc(Values);
    if(!IsEqual(Result, FuncResult))
    {
      wcerr << "Function:     " << Str << std::endl;
      for(unsigned I = 0; I < Values.size(); I++)
        wcerr << Args[I] << ":            " << setprecision(10) << Values[I] << std::endl;
      wcerr << "Evaluated to: " << setprecision(10) << FuncResult << std::endl;
      wcerr << "Expected:     " << setprecision(10) << Result << std::endl << std::endl;
    }
  }
  catch(EFuncError &E)
  {
    wcerr << "Function:     " << Str << std::endl;
    for(unsigned I = 0; I < Values.size(); I++)
      wcerr << Args[I] << ":            " << setprecision(10) << Values[I] << std::endl;
    wcerr << "Error code   : " << E.ErrorCode << std::endl;
    wcerr << "Expected:     " << setprecision(10) << Result << std::endl << std::endl;
  }
}

void TestMakeText(const TFunc &Func, const std::wstring &Result)
{
  try
  {
    std::wstring TextResult = Func.MakeText();
    if(TextResult != Result)
    {
      wcerr << "Function:     " << TextResult << std::endl;
      wcerr << "Expected:     " << Result << std::endl << std::endl;
    }
  }
  catch(EFuncError &E)
  {
    wcerr << "Error code   : " << E.ErrorCode << std::endl;
    wcerr << "Expected:     " << Result << std::endl << std::endl;
  }
}

/** Called when BOOST_ASSERT fails.
 */
namespace boost
{
  void assertion_failed(char const * expr, char const * function, char const * file, long line)
  {
    cerr << "Assertion failed: " << expr << ", File: " << file << ", Line " << line;
    abort();
  }
}

/** Called from STLport to log debug messages.
 */
void __stl_debug_message(const char * format_str, ...)
{
	va_list Args;
	va_start(Args, format_str);

  char Buffer[4096];
  vsnprintf(Buffer, sizeof(Buffer), format_str, Args);

  std::cerr << "DEBUG MESSAGE\n";
  std::cerr << Buffer << std::endl;

  va_end(Args);
}


/** Called from STLport to terminate the program, for example when an assertion fails.
 */
void __stl_debug_terminate(void)
{
  cerr << "DEBUG TERMINATE" << std::endl;
  abort();
}

double ErrorToWeight(double x) {return 1/(x*x);}

class TTestSqr : public TBaseCustomFunc
{
public:
  unsigned ArgumentCount() const {return 1;}
  long double Call(const long double *Args, TTrigonometry Trig, TErrorCode &ErrorCode, std::wstring &ErrorStr) const
  {
    return Args[0] * Args[0];
  }
  TComplex Call(const TComplex *Args, TTrigonometry Trig, TErrorCode &ErrorCode, std::wstring &ErrorStr) const
  {
    return Args[0] * Args[0];
  }
};

class TTestCube : public TBaseCustomFunc
{
public:
  unsigned ArgumentCount() const {return 3;}
  long double Call(const long double *Args, TTrigonometry Trig, TErrorCode &ErrorCode, std::wstring &ErrorStr) const
  {
    return Args[0] * Args[0] * Args[0] + Args[1] * Args[1] + Args[2];
  }
  TComplex Call(const TComplex *Args, TTrigonometry Trig, TErrorCode &ErrorCode, std::wstring &ErrorStr) const
  {
    return Args[0] * Args[0] * Args[0] + Args[1] * Args[1] + Args[2];
  }
};

void Test()
{
  //Test parsing errors
  TestError("sinx", 0, ecUnknownVar);
  TestError("x2", 0, ecUnknownVar);
  TestError("sin2 x", 0, ecUnknownVar);
  TestError("integrate(x, 5)", 0, ecArgCountError);
  TestError("integrate(x, x, 5, 7, 8)", 0, ecArgCountError);

  //Test redundant space
  Test("x*sin x", PI/2, PI/2);
  Test("  x*sin x", PI/2, PI/2);
  Test("x * sin x", PI/2, PI/2);
  Test("x*sin x   ", PI/2, PI/2);
  Test("x*sin    x", PI/2, PI/2);
  TestError("", 0, ecEmptyString);
  TestError("     ", 0, ecEmptyString);

  //Test case (in)sesitivity
  Test("Sin X+x/pI", 0, 0);
  TestError("4.5e2", 0, ecUnknownVar);
  TestError("4.5I", 0, ecUnknownVar);

  //Test numbers
  Test("45", 0, 45);
  Test("-0.5796", 0, -0.5796);
  Test("-.5796", 0, -0.5796);
  Test("-5.478E-23x", 10, -5.478E-22);
  Test("pi", 0, PI);
  Test("e", 0, EULER);
  TestError("E", 0, ecUnknownVar);
  TestEval<TComplex>(L"i*i", 0, -1);
  TestErrorEval<long double>(L"i*i", 0, ecComplexError);
  Test("1E400*x", 2, StrToDouble("2E400")); //2E400 doesn't work directly with BCC 5.6.4
  Test("1E4000", 1, StrToDouble("1E4000")); //2E400 doesn't work directly with BCC 5.6.4
  TestError("1E5000", 1, ecParseError); //Number too large
  TestError("1.2.3", 0, ecInvalidNumber);
  TestError("5.", 0, ecInvalidNumber);

  //Test constants
  Test("e", 0, EULER);
  Test("pi", 0, PI);
  TestErrorEval<long double>(L"i", 0, ecComplexError);
  TestEval<TComplex>(L"i", 0, TComplex(0, 1));
  TestErrorEval(L"undef", 0, ecNotDefError);

  //Test functions with arguments
  TestError("round x", 1.2345, ecArgCountError);
  TestError("round(x)", 1.2345, ecArgCountError);
  Test("round(x, 0)", 1.2345, 1);

  //Implied multiplication
  Test("10sin(pi/2)x", 10, 100);
  Test("sin(pi*x)4x", 1.5, -6);
  Test("-3x", 10, -30);

  //Test common operators
  Test("4.5+x", 3.1, 7.6);
  Test("x+4.5", 3.1, 7.6);
  Test("4.5-x", 3.1, 1.4);
  Test("x-4.5", 3.1, -1.4);
  Test("4.5*x", 3.1, 13.95);
  Test("x*4.5", 3.1, 13.95);
  Test("4.5/x", 3.1, 1.4516129);
  Test("x/4.5", 3.1, 0.688888889);
  Test("(x)-3", 1, -2);
  Test("-3*x", 10, -30);
  TestError("x(5.4)", 0, ecParAfterConst);
  TestError("rand(2)", 0, ecParAfterConst);
  Test("x 5.4", 3, 16.2);     //Should this be allowed?
  Test("--x", 5, 5);
  Test("-x", 5, -5);
  Test("-5", NaN, -5);
  Test("++x", 5, 5);
  Test("+x", 5, 5);
  Test("+5", NaN, 5);
  Test("+-x", 5, -5);
  Test("-+x", 5, -5);

  //Test power function
  Test("x^2", -2, 4);
  Test("x^2", 10, 100);
  Test("-x^2", 10, -100);
  Test("(-x)^2", 10, 100);
  Test("x^x", 0, 1);         //Important test
  Test("x^0", 458.789, 1);   //Important test
  Test("0^x", 456.789, 0);   //Important test
  Test("log(x)^2", 100, 4);
  TestError("x/(x^2-4)", -2, ecDivByZero);
  TestErrorEval<long double>(L"(-2)^x", 2.2, ecPowCalcError);
  TestEval<TComplex>(L"(-2)^x", 2.2, TComplex(3.717265962,2.70075181));
  TestErrorEval<long double>(L"(-2)^x", 2.3, ecPowCalcError);
  TestError("x^(-1)", 0, ecPowCalcError);
  TestErrorEval<TComplex>(L"0^x", TComplex(3,1), ecPowCalcError);
  TestEval<TComplex>(L"0^x", TComplex(3,0), 0);
  Test("e^(2x)", 2, M_E*M_E*M_E*M_E);
  Test("e^2x", 2, M_E*M_E*M_E*M_E); //Same as the above
  Test("x^2^3", 10, 1E8);
  Test("(x^2)^3", 10, 1E6);
  Test("3^x^2", 4, 43046721);
  TestErrorEval<TComplex>(L"0^x", TComplex(-2.8, 1), ecPowCalcError);
  TestError("x^-2.8", 0, ecPowCalcError);
  Test("e^x", -10000, 0);

  //Test power with fraction handling
  TestEval<long double>(L"x^(1/3)", -8, -2);
  TestEval<TComplex>(L"x^(1/3)", -8, TComplex(1,1.732050807568877));
  TestEval<long double>(L"x^(2/6)", -8, -2);
  TestEval<TComplex>(L"x^(2/6)", -8, TComplex(1,1.732050807568877));
  TestEval<long double>(L"x^(2/3)", -8, 4);
  TestEval<TComplex>(L"x^(2/3)", -8, TComplex(-2, 3.464101615137755));
  TestError("x^(2/0)", -8, ecDivByZero);
  Test("x^(2/3)", 0, 0);
  Test("x^(4/2)", -2, 4);

  //Test trigonometry functions
  Test("sin(x)", PI/2, 1);
  Test("sin(x)", 90, 1, Degree);
  Test("cos(x)", PI/2, 0);
  Test("cos(x)", 90, 0, Degree);
  Test("tan(x)", PI/4, 1);
  Test("tan(x)", 45, 1, Degree);
  Test("sec(x)", PI, -1);
  Test("sec(x)", 180, -1, Degree);
  Test("csc(x)", PI/2, 1);
  Test("csc(x)", 90, 1, Degree);
  Test("cot(x)", PI/4, 1);
  Test("cot(x)", 45, 1, Degree);

  //Test inverse trigonometry functions
  Test("asin(x)", 1, PI/2);
  Test("asin(x)", 1, 90, Degree);
  Test("acos(x)", 0, PI/2);
  Test("acos(x)", 0, 90, Degree);
  Test("atan(x)", 1, PI/4);
  Test("atan(x)", 1, 45, Degree);
  Test("asec(x)", -1, PI);
  Test("asec(x)", -1, 180, Degree);
  Test("acsc(x)", 1, PI/2);
  Test("acsc(x)", 1, 90, Degree);
  Test("acot(x)", 1, PI/4);
  Test("acot(x)", 1, 45, Degree);
  Test("acot(x)", 0, PI/2);
  Test("acot(x)", 0, 90, Degree);

  //Test logarihms
  Test("log(x)", 10000, 4);
  Test("logb(x,2)", 32, 5);
  Test("ln(x)", EULER*EULER, 2);
  Test("exp(x)", 2, EULER*EULER);
  TestError("log(-x)", 0, ecLogError);

  //Test square and roots
  Test("sqr(x)", 5, 25);
  Test("sqrt(x)", 100, 10);
  Test("root(3,x)", 125, 5);
  TestEval<long double>(L"root(3,x)", -27, -3);
  TestEval<TComplex>(L"root(3,x)", -27, TComplex(1.5, 2.598076211));
  TestErrorEval<long double>(L"root(3.5, x)", -27, ecPowCalcError);
  Test("root(4,x)", 625, 5);
  TestErrorEval<long double>(L"root(4,x)", -625, ecPowCalcError);
  TestEval<TComplex>(L"root(4,x)", -4, TComplex(1, 1));

  Test("fact(x)", 5, 120);

  Test("sign(x)", 7.98, 1);
  Test("sign(x)", -7.98, -1);
  Test("sign(x)", 0, 0);
  TestEval<TComplex>(L"sign(x)", TComplex(5, 5), TComplex(M_SQRT_2, M_SQRT_2));
  TestEval<TComplex>(L"sign(x)", TComplex(4, -3), TComplex(4.0/5, -3.0/5));

  Test("u(x)", 7.98, 1);
  Test("u(x)", -7.98, 0);
  Test("u(x)", 0, 1);

  //Test hyperbolic functions
  Test("sinh(x)", 5, 74.20321058);
  Test("cosh(x)", 5, 74.20994852);
  Test("tanh(x)", 5, 0.999909204);
  Test("asinh(x)", 5, 2.31243834);

  Test("acosh(x)", 5, 2.29243167);
  Test("acosh(x)", 1, 0);
  TestEval<TComplex>(L"acosh(x)", -1, TComplex(0, M_PI));
  TestErrorEval<long double>(L"acosh(x)", -1, ecACoshError);

  Test("atanh(x)", 0.5, 0.54930614);
  TestError("atanh(x)", 1, ecATanhError);
  TestError("atanh(x)", -1, ecATanhError);

  Test("abs(x)", -4.67, 4.67);
  TestEval<TComplex>(L"abs(x)", TComplex(3, 4), 5);
  TestEval<TComplex>(L"arg(x)", TComplex(3, 4), 0.927295218);
  TestEval<TComplex>(L"arg(x)", TComplex(3, 4), 53.13010235, Degree);
  TestEval<TComplex>(L"conj(x)", TComplex(3, 4), TComplex(3, -4));
  TestEval<TComplex>(L"re(x)", TComplex(3, 4), 3);
  TestEval<TComplex>(L"im(x)", TComplex(3, 4), 4);
  Test("re(x)", 3, 3);
  Test("im(x)", 3, 0);

  //Rounding
  Test("trunc(x)", -4.567, -4);
  Test("fract(x)", -4.567, -0.567);
  Test("ceil(x)", -4.567, -4);
  Test("floor(x)", -4.567, -5);
  Test("Round(x, 3)", 412.4572, 412.457);
  Test("Round(x, 2)", 412.4572, 412.46);
  Test("Round(x, 1)", 412.4572, 412.5);
  Test("Round(x, 0)", 412.4572, 412);
  Test("Round(x, -2)", 412.4572, 400);
  Test("Round(x, 2)", -412.4572, -412.46);

  //Boolean functions
  Test("not(x)", -4.567, 0);
  Test("not(x)", 0, 1);
  Test("x=5", 5, 1);
  Test("x=5", 3, 0);
  Test("x<5", 4, 1);
  Test("x<5", 5, 0);
  Test("x<5", 6, 0);
  Test("x>5", 4, 0);
  Test("x>5", 5, 0);
  Test("x>5", 6, 1);
  Test("x<=5", 4, 1);
  Test("x<=5", 5, 1);
  Test("x<=5", 6, 0);
  Test("x>=5", 4, 0);
  Test("x>=5", 5, 1);
  Test("x>=5", 6, 1);
  Test("3<x<6", 2, 0);
  Test("3<x<6", 3, 0);
  Test("3<x<6", 4, 1);
  Test("3<x<6", 6, 0);
  Test("3<x<6", 7, 0);

  Test("1 and 1", 0, 1);
  Test("0 and 1", 0, 0);
  Test("1 and 0", 0, 0);
  Test("0 and 0", 0, 0);
  Test("1 or 1", 0, 1);
  Test("0 or 1", 0, 1);
  Test("1 or 0", 0, 1);
  Test("0 or 0", 0, 0);
  Test("1 xor 1", 0, 0);
  Test("0 xor 1", 0, 1);
  Test("1 xor 0", 0, 1);
  Test("0 xor 0", 0, 0);

  //Test picewise functions
  Test("if(x<3, 5, 9)", 2, 5);
  Test("if(x<3, 5, 9)", 3, 9);
  Test("if(x<3, 5, 9)", 4, 9);
  Test("range(2,x,5)", 1, 2);
  Test("range(2,x,5)", 3, 3);
  Test("range(2,x,5)", 6, 5);
  Test("min(8, 2, 5)", 0, 2);
  Test("min(x,2)", 1, 1);
  Test("max(8, 2, 5)", 0, 8);

  Test("gamma(x)", 6, 120);
  Test("mod(x,5)", 12, 2);
  Test("mod(x,5)", 10, 0);
  Test("mod(x,-1)", -2, 0);
  Test("mod(x,-5)", 12, -3);
  Test("mod(x,-5)", -12, -2);
  Test("mod(x, 1.2)", 5.6, 0.8);
  Test("mod(x, 1.2)", -5.6, 0.4);
  Test("dnorm(x, 5, 7)", 3, 0.054712394277745);
  Test("dnorm(x)", 3, 0.00443184841193801);
  Test("dnorm(x, 0, 1)", 3, 0.00443184841193801);

  //Test backward compatibility
  Test("integrate(x^2,2,5)", NaN, 39);
  TestError("sum(x, 3, 7)", NaN, ecArgCountError);
  TestError("product(x, 3, 7)", NaN, ecArgCountError);

  //Test improved integrate
  Test("integrate(x^2,x,2,5)", NaN, 39);
  Test("integrate(dnorm(x,100,60),x,-inf,100)", 0, 0.5);
  Test("integrate(dnorm(x,100,20),x,-inf,100)", 0, 0.5);
  Test("integrate(dnorm(x,100,60),x,100,inf)", 0, 0.5);
  Test("integrate(dnorm(x,100,20),x,100,+inf)", 0, 0.5);
  Test("integrate(e^x,x,-inf,0)", 0, 1);
  Test("integrate(e^x,x,0,-inf)", 0, -1);
  Test("integrate(e^-x,x,inf,0)", 0, -1);
  Test("integrate(e^-x,x,0,inf)", 0, 1);
  Test("integrate(e^-abs(x),x,-inf,inf)", 0, 2);
  Test("integrate(e^-abs(x),x,inf,-inf)", 0, -2);
  Test("integrate(x*t^2, t, 0, 3)", 2, 18);
  Test("sum(x, x, 3, 7)", NaN, 3+4+5+6+7);
  Test("product(x, x, 3, 7)", NaN, 3*4*5*6*7);
  Test("sum(x*t, t, 3, 7)", 2, 2*3+2*4+2*5+2*6+2*7);
  Test("product(x*t, t, 3, 7)", 2, 2*3*2*4*2*5*2*6*2*7);

  //Test infinity
  Test("inf", 0, INF);
  Test("-inf", 0, -INF);
  TestEval<long double>(L"2*inf", 0, INF);
  TestEval<TComplex>(L"2*inf", 0, TComplex(INF, NaN));
  Test("2+inf", 0, INF);
  TestEval<long double>(L"inf*x", 0, NaN);
  TestEval<TComplex>(L"inf*x", 0, TComplex(NaN, NaN));
  TestEval<long double>(L"inf/inf", 0, NaN);
  TestEval<TComplex>(L"inf/inf", 0, TComplex(NaN, NaN));

  //Combined test cases
  Test("(1.01^x-1)/0.01", 1, 1);

  //Test undef
  TestError("undef", 0, ecNotDefError);
  Test("if(x<5, x, undef)", 0, 0);
  TestError("if(x<5, x, undef)", 6, ecNotDefError);

  //Test block function
  Test("if(x<-2, x, 2x)", -3, -3);
  Test("if(x<-2, x, 2x)", 1, 2);
  Test("if(x<-2, x, x<3, 2x, 3x)", 1, 2);
  Test("if(x<-2, x, x<3, 2x, 3x)", 4, 12);
  Test("2+if(x<-2, x, x<3, 2x, 3x)", 4, 14);
  Test("if(x<-2, x, x<3, 2x)", 1, 2);
  TestError("if(x<-2, x, x<3, 2x)", 4, ecNotDefError);

  //Test Zeta function
  Test("zeta(x)", 0, -0.5);
  Test("zeta(x)", 10, 1.000994575);
  TestError("zeta(x)", 1, ecNotDefError);
  Test("zeta(x)", -5, -1.0/252);

  //Test Omega function
  TestErrorEval<long double>(L"W(x)", -M_PI/2, ecComplexError);
  TestEval<TComplex>(L"W(x)", -M_PI/2, TComplex(0, M_PI/2));
  Test("W(-1/e)", 0, -1);
  Test("W(x)", 0, 0);
  Test("W(x)", -M_LN2/2, -M_LN2);
  Test("W(x)", 1, 0.5671432904097838729999686622);
  Test("W(x)", M_E, 1);

  //Test vertical trend lines
  TDblPoint Vertical[] = {TDblPoint(5,1), TDblPoint(5,7)};
  TestTrendLineError(ttPower, Vertical, 2, 0, ecNoResult);

  //Test difficult trend line
  TDblPoint Points1[] = {TDblPoint(1950,1571), TDblPoint(1970,524), TDblPoint(1980, 208), TDblPoint(2003, 29)};
  std::vector<double> Empty;
  std::vector<TDblPoint> Points(Points1, Points1 + 4);
  TestTrendLine(ttPower, Points, Empty, 0, NaN, L"7.23106321804096256E+498*x^(-150.630652337941856)");

  //Test sample trendline
  TDblPoint P1[] = {TDblPoint(0,0.1), TDblPoint(1,0.9), TDblPoint(2,1.9), TDblPoint(3,2.7), TDblPoint(4,4.7)};
  double W1[] = {0.1, 0.13, 0.16, 0.2, 0.25};
  std::vector<TDblPoint> P(P1, P1 + 5);
  std::vector<double> W(5);
  std::transform(W1, W1+5, W.begin(), ErrorToWeight); //Perform x=1/(x*x)

  TestTrendLine(ttLinear, P, Empty, 0, NaN, L"1.1*x-0.14");
  TestTrendLine(ttLinear, P, Empty, 0, 1, L"0.72*x+1"); //Force crossing with y-axis
  TestTrendLine(ttLinear, P, W, 0, NaN, L"1.01084802043554*x+0.00367555924507695");
  TestTrendLine(ttLinear, P, W, 0, 1, L"0.615568703919657*x+1"); //Force crossing with y-axis

  TestTrendLine(ttPolynomial, P, Empty, 2, NaN, L"0.157142857142857*x^2+0.471428571428571*x+0.174285714285714");
  TestTrendLine(ttPolynomial, P, Empty, 2, 1, L"0.290322580645161*x^2-0.247741935483871*x+1"); //Force crossing
  TestTrendLine(ttPolynomial, P, W, 2, NaN, L"0.123428984493036*x^2+0.596597125830378*x+0.12279529365147");
  TestTrendLine(ttPolynomial, P, W, 2, 1, L"0.292651922716118*x^2-0.254573787956674*x+1");

  TestTrendLine(ttPower, P, Empty, 0, NaN, L"0.872147571158689*x^1.14047602443827");
  TestTrendLine(ttPower, P, W, 0, NaN, L"0.890072586434138*x^1.10326113209501");

  TestTrendLine(ttExponential, P, Empty, 0, NaN, L"0.200922305275277*2.41063632810424^x");
  TestTrendLine(ttExponential, P, Empty, 0, 1, L"1.41191238522143^x");
  TestTrendLine(ttExponential, P, W, 0, NaN, L"0.14747627125184*2.92012743745353^x");
  TestTrendLine(ttExponential, P, W, 0, 1, L"1.36647807441143^x");

  TestCustomTrendLine(L"$a*x+$b", P, Empty, L"1.1*x-0.14");
  TestCustomTrendLine(L"$a*x+1", P, Empty, L"0.72*x+1");
  TestCustomTrendLine(L"$a*x+$b", P, W, L"1.01085*x+0.00368");
  TestCustomTrendLine(L"$a*x+1", P, W, L"0.61557*x+1");

  TestCustomTrendLine(L"$a*x^2+$b*x+$c", P, Empty, L"0.15714*x^2+0.47143*x+0.17429");
  TestCustomTrendLine(L"$a*x^2+$b*x+1", P, Empty, L"0.29032*x^2-0.24774*x+1");
  TestCustomTrendLine(L"$a*x^2+$b*x+$c", P, W, L"0.12343*x^2+0.5966*x+0.1228");
  TestCustomTrendLine(L"$a*x^2+$b*x+1", P, W, L"0.29265*x^2-0.25457*x+1");

  //Test differentiation of common operators
  TestDif("-0.5796", "0");
  TestDif("-3x", "-3");
  TestDif("-3x^1", "-3");
  TestDif("-3x^2", "-6x");
  TestDif("-3x^3", "-9x^2");
  TestDif("5x+sin(x)", "5+cos(x)");
  TestDif("5x-sin(x)", "5-cos(x)");
  TestDif("5/x", "-5/x^2");
  TestDif("sin(x)/x", "(cos(x)*x-sin(x))/x^2");
  TestDif("2*sin 30*x", "1", Degree);
  TestDif("x-cos x", "1+sin x");

  //Test differentiation of special functions
  TestDif("dnorm(x)", "-exp(-x^2/2)*x*sqrt(2pi)/2pi");
  TestDif("dnorm(x, 0, 1)", "-exp(-x^2/2)*x*sqrt(2pi)/2pi");

  //Test differentiation of inverse trigonometric functions
  TestDif("acot(x)", 1, -28.647889756541160438399077407053, Degree);

  //Differentiation of special power handling
  TestDif("x^(1/3)", "1/3*x^(-2/3)");
  TestDif("2^(x/3)", "1/3*ln(2)*2^(x/3)");
  TestDif("2^(1/x)", "-ln(2)*2^(1/x)/x^2");

  //Differentiation of piecewise functions
  TestDif("min(x,2)", "if(x<2,1,0)");

  //Test the simplify code
  TestSimplify("ln(e)", "1");
  TestSimplify("log(10)", "1");
  TestSimplify("1--sin x", "1+sin x");
  TestSimplify("--sin x", "sin x");

  //Test custom functions
  TArgType Args;
  Args.push_back(L"x");
  Args.push_back(L"y");
  TestCustom(L"x*y+1=0", Args, std::vector<TComplex>(2, 0), 0);

  //Test symbol lists
  TSymbolList SymbolList;
  SymbolList.Add(L"TestSqr", boost::shared_ptr<TBaseCustomFunc>(new TTestSqr));
  SymbolList.Add(L"TestCube", boost::shared_ptr<TBaseCustomFunc>(new TTestCube));
  SymbolList.Add(L"k", L"42");
  Args.clear();
  Args.push_back(L"a");
  Args.push_back(L"b");
  Args.push_back(L"c");
  SymbolList.Add(L"foo", L"a^3+b^2+c", Args);
  Test("k*x", 2, 84, Radian, "", SymbolList);
  Test("TestSqr(x)", 10, 100, Radian, "", SymbolList);
  Test("TestCube(5, 2, x)", 1, 130, Radian, "", SymbolList);
  Test("foo(5, 2, x)", 1, 130, Radian, "", SymbolList);

  //Test conversion to text
  TestMakeText(TFunc(L"0"), L"0");
}

std::wstringstream DebugStreamBuf;
int main()
{
  _control87(DEFAULT_FPU_CONTROL, FPU_MASK);
  std::wclog.rdbuf(DebugStreamBuf.rdbuf()); //Write debug messages to stringstream instead of console
  try
  {
    Test();
  }
  catch(...)
  {
    std::cerr << "Unexpected exception!" << std::endl;
    std::cin.ignore();
    return 1;
  }
  cout << "Test finished" << std::endl;
  std::cin.ignore();
  return 0;
}
//---------------------------------------------------------------------------

