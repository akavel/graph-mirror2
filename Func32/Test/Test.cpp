//---------------------------------------------------------------------------
#pragma hdrstop
#include "../Func32.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdarg>
#include <limits>
#include <cmath>
//---------------------------------------------------------------------------
using namespace Func32;
using namespace std;

#define EULER 2.718281828459045235360287
#define PI    3.141592653589793238462643

#define SQRT2 1.41421356237

const long double NaN = numeric_limits<long double>::quiet_NaN(); //0.0/0.0;
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
  int a_exp, b_exp, exp;
  frexp(a, &a_exp);
  frexp(b, &b_exp);
  frexp(a - b, &exp);
  return IsZero(a-b) || (a_exp == b_exp && std::abs(exp - a_exp) > 50);
}

inline bool IsEqual(TComplex a, TComplex b)
{
  return IsEqual(real(a), real(b)) && IsEqual(imag(a), imag(b));
}

template<typename T>
void TestEval(const char *Str, T x, T y, TTrigonometry Trig = Radian)
{
  try
  {
    TFunc Func(Str);
    Func.SetTrigonometry(Trig);
    T f = Func.CalcY(x);
    if(!IsEqual(f, y))
    {
      cerr << "Function:     " << Str << std::endl;
      cerr << "x:            " << setprecision(10) << x << std::endl;
      cerr << "Evaluated to: " << setprecision(10) << f << std::endl;
      cerr << "Expected:     " << setprecision(10) << y << std::endl << std::endl;
      cin.ignore();
    }
  }
  catch(EFuncError &E)
  {
    cerr << "Function:  " << Str << std::endl;
    cerr << "x:         " << setprecision(10) << x << std::endl;
    cerr << "ErrorCode: " << E.ErrorCode << std::endl ;
    cerr << "Expected:  " << setprecision(10) << y << std::endl << std::endl;
    cin.ignore();
  }
}

void Test(const char *Str, long double x, long double y, TTrigonometry Trig = Radian)
{
  TestEval<long double>(Str, x, y, Trig);
  TestEval<TComplex>(Str, x, y, Trig);
}

template<typename T>
void TestErrorEval(const char *Str, T x, Func32::TErrorCode Error, TTrigonometry Trig = Radian)
{
  try
  {
    TFunc Func(Str);
    Func.SetTrigonometry(Trig);
    T f = Func.CalcY(x);

    cerr << "Function:       " << Str << std::endl;
    cerr << "x:              " << setprecision(10) << x << std::endl;
    cerr << "Evaluated to:   " << setprecision(10) << f << std::endl;
    cerr << "Expected error: " << Error << std::endl << std::endl;
    cin.ignore();
  }
  catch(EFuncError &E)
  {
    if(E.ErrorCode != Error)
    {
      cerr << "Function:       " << Str << std::endl;
      cerr << "x:              " << setprecision(10) << x << std::endl;
      cerr << "Error code:     " << E.ErrorCode << std::endl;
      cerr << "Expected error: " << Error << std::endl << std::endl;
      cin.ignore();
    }
  }
}

void TestError(const char *Str, long double x, TErrorCode Error, TTrigonometry Trig = Radian)
{
  TestErrorEval<long double>(Str, x, Error, Trig);
  TestErrorEval<TComplex>(Str, x, Error, Trig);
}


void TestTrendLine(Func32::TTrendType Type, const TDblPoint *Points, unsigned Size, unsigned N)
{
  try
  {
    TrendLine(Type, std::vector<TDblPoint>(Points, Points + Size), N);
  }
  catch(EFuncError &E)
  {
    cerr << "-- Trendline --" << endl;
    cerr << "Error code:     " << E.ErrorCode << std::endl << std::endl;
    cin.ignore();
  }
}

void TestTrendLineError(Func32::TTrendType Type, const TDblPoint *Points, unsigned Size, unsigned N, Func32::TErrorCode ErrorCode)
{
  try
  {
    TrendLine(Type, std::vector<TDblPoint>(Points, Points + Size), N);

    cerr << "-- Trendline --" << endl;
    cerr << "Expected error code:     " << ErrorCode << endl << endl;
    cin.ignore();
  }
  catch(EFuncError &E)
  {
    if(E.ErrorCode != ErrorCode)
    {
      cerr << "-- Trendline --" << endl;
      cerr << "Error code:     " << E.ErrorCode << endl << endl;
      cin.ignore();
    }
  }
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

void TestDif(const char *f, const char *df, TTrigonometry Trigonometry = Radian)
{
  try
  {
    TFunc Func(f, "x", Trigonometry);
    TFunc Dif = Func.MakeDif();
    TFunc Dif2(df, "x", Trigonometry);

    if(!CompareFunc(Dif, Dif2))
    {
      cerr << "f(x)=" << f << std::endl;
      cerr << "f'(x)=" << Dif.MakeText() << std::endl;
      cerr << "Expected f'(x)=" << df << std::endl << std::endl;
      cin.ignore();
    }
  }
  catch(EFuncError &E)
  {
    cerr << "f(x)=" << f << std::endl;
    cerr << "Expected f'(x)=" << df << std::endl;
    cerr << "Error code: " << E.ErrorCode << std::endl << std::endl;
    cin.ignore();
  }
}


void TestDif(const char *Str, long double x, long double y, TTrigonometry Trigonometry = Radian)
{
  try
  {
    TFunc Func(Str, "x", Trigonometry);
    TFunc Dif = Func.MakeDif();
    long double f = Dif(x);

    if(!IsEqual(f, y))
    {
      cerr << "f(x)=" << Str << std::endl;
      cerr << "f'(x)=" << Dif.MakeText() << std::endl;
      cerr << "f'(" << x << ")=" << f << std::endl;
      cerr << "Expected f'(" << x << ")=" << y << std::endl << std::endl;
      cin.ignore();
    }
  }
  catch(EFuncError &E)
  {
    cerr << "f(x)=" << Str << std::endl;
    cerr << "Expected f'(" << x << ")=" << y << std::endl;
    cerr << "Error code: " << E.ErrorCode << std::endl << std::endl;
    cin.ignore();
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

void Test()
{
  Test("1E5000", 1, 1/*StrToDouble("1E5000")*/); //Number too large
  //Test redundant space
  Test("x*sin x", PI/2, PI/2);
  Test("  x*sin x", PI/2, PI/2);
  Test("x * sin x", PI/2, PI/2);
  Test("x*sin x   ", PI/2, PI/2);
  Test("x*sin    x", PI/2, PI/2);

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
  TestEval<TComplex>("i*i", 0, -1);
  TestErrorEval<long double>("i*i", 0, ecComplexError);
  Test("1E400*x", 2, StrToDouble("2E400")); //2E400 doesn't work directly with BCC 5.6.4
  Test("1E4000", 1, StrToDouble("1E4000")); //2E400 doesn't work directly with BCC 5.6.4
  Test("1E5000", 1, 1/*StrToDouble("1E5000")*/); //Number too large
  TestError("1.2.3", 0, ecInvalidNumber);
  TestError("5.", 0, ecInvalidNumber);

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
  TestErrorEval<long double>("(-2)^x", 2.2, ecPowCalcError);
  TestEval<TComplex>("(-2)^x", 2.2, TComplex(3.717265962,2.70075181));
  TestErrorEval<long double>("(-2)^x", 2.3, ecPowCalcError);
  TestError("x^(-1)", 0, ecPowCalcError);
  TestErrorEval<TComplex>("0^x", TComplex(3,1), ecPowCalcError);
  TestEval<TComplex>("0^x", TComplex(3,0), 0);
  Test("e^(2x)", 2, M_E*M_E*M_E*M_E);
  Test("e^2x", 2, M_E*M_E*M_E*M_E); //Same as the above
  Test("x^2^3", 10, 1E8);
  Test("(x^2)^3", 10, 1E6);

  //Test special power function handling
  TestEval<long double>("x^(1/3)", -8, -2);
  TestEval<TComplex>("x^(1/3)", -8, TComplex(1,1.732050807568877));
  TestEval<long double>("x^(2/6)", -8, -2);
  TestEval<TComplex>("x^(2/6)", -8, TComplex(1,1.732050807568877));
  TestEval<long double>("x^(2/3)", -8, 4);
  TestEval<TComplex>("x^(2/3)", -8, TComplex(-2, 3.464101615137755));
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

  //Test square and roots
  Test("sqr(x)", 5, 25);
  Test("sqrt(x)", 100, 10);
  Test("root(3,x)", 125, 5);
  TestEval<long double>("root(3,x)", -27, -3);
  TestEval<TComplex>("root(3,x)", -27, TComplex(1.5, 2.598076211));
  TestErrorEval<long double>("root(3.5, x)", -27, ecPowCalcError);
  Test("root(4,x)", 625, 5);
  TestErrorEval<long double>("root(4,x)", -625, ecPowCalcError);
  TestEval<TComplex>("root(4,x)", -4, TComplex(1, 1));

  Test("fact(x)", 5, 120);
  Test("sign(x)", 7.98, 1);
  Test("sign(x)", -7.98, -1);
  TestError("sign(x)", 0, ecNotDefError);
  Test("u(x)", 7.98, 1);
  Test("u(x)", -7.98, 0);
  Test("u(x)", 0, 1);

  //Test hyperbolic functions
  Test("sinh(x)", 5, 74.20321058);
  Test("cosh(x)", 5, 74.20994852);
  Test("tanh(x)", 5, 0.999909204);
  Test("asinh(x)", 5, 2.31243834);
  Test("acosh(x)", 5, 2.29243167);
  Test("atanh(x)", 0.5, 0.54930614);

  Test("abs(x)", -4.67, 4.67);
  TestEval<TComplex>("abs(x)", TComplex(3, 4), 5);
  TestEval<TComplex>("arg(x)", TComplex(3, 4), 0.927295218);
  TestEval<TComplex>("arg(x)", TComplex(3, 4), 53.13010235, Degree);
  TestEval<TComplex>("conj(x)", TComplex(3, 4), TComplex(3, -4));
  TestEval<TComplex>("re(x)", TComplex(3, 4), 3);
  TestEval<TComplex>("im(x)", TComplex(3, 4), 4);
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
  Test("max(8, 2, 5)", 0, 8);

  Test("gamma(x)", 6, 120);
  Test("mod(x,5)", 12, 2);
  Test("mod(x,5)", 10, 0);
  Test("mod(x,-1)", -2, 0);
  Test("mod(x,-5)", 12, -3);
  Test("mod(x,-5)", -12, -2);
  Test("mod(x, 1.2)", 5.6, 0.8);
  Test("mod(x, 1.2)", -5.6, 0.4);
  Test("integrate(x^2,2,5)", NaN, 39);
  Test("sum(x, 3, 7)", NaN, 3+4+5+6+7);
  Test("product(x, 3, 7)", NaN, 3*4*5*6*7);
  Test("dnorm(x, 5, 7)", 3, 0.054712394277745);
  Test("dnorm(x)", 3, 0.00443184841193801);
  Test("dnorm(x, 0, 1)", 3, 0.00443184841193801);

  //Combined test cases
  Test("(1.01^x-1)/0.01", 1, 1);

  //Test undef
  TestError("undef", 0, ecNotDefError);
  Test("if(x<5, x, undef)", 0, 0);
  TestError("if(x<5, x, undef)", 6, ecNotDefError);

  //Test block function
  Test("ifseq(x<-2, x, 2x)", -3, -3);
  Test("ifseq(x<-2, x, 2x)", 1, 2);
  Test("ifseq(x<-2, x, x<3, 2x, 3x)", 1, 2);
  Test("ifseq(x<-2, x, x<3, 2x, 3x)", 4, 12);
  Test("2+ifseq(x<-2, x, x<3, 2x, 3x)", 4, 14);
  Test("ifseq(x<-2, x, x<3, 2x)", 1, 2);
  TestError("ifseq(x<-2, x, x<3, 2x)", 4, ecNotDefError);

  //Test Zeta function
  Test("zeta(x)", 0, -0.5);
  Test("zeta(x)", 10, 1.000994575);
  TestError("zeta(x)", 1, ecNotDefError);
  Test("zeta(x)", -5, -1.0/252);

  //Test vertical trend lines
  TDblPoint Vertical[] = {TDblPoint(5,1), TDblPoint(5,7)};
  TestTrendLineError(ttPower, Vertical, 2, 0, ecOverflow);

  //Test difficult trend line
  TDblPoint Points[] = {TDblPoint(1950,1571), TDblPoint(1970,524), TDblPoint(1980, 208), TDblPoint(2003, 29)};
  TestTrendLine(ttPower, Points, 4, 0);

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

  //Test differentiation of special functions
  TestDif("dnorm(x)", "-exp(-x^2/2)*x*sqrt(2pi)/2pi");
  TestDif("dnorm(x, 0, 1)", "-exp(-x^2/2)*x*sqrt(2pi)/2pi");

  //Test differentiation of inverse trigonometric functions
  TestDif("acot(x)", 1, -28.647889756541160438399077407053, Degree);

  //Differentiation of special power handling
  TestDif("x^(1/3)", "1/3*x^(-2/3)");
  TestDif("2^(x/3)", "1/3*ln(2)*2^(x/3)");
  TestDif("2^(1/x)", "-ln(2)*2^(1/x)/x^2");
}

int main()
{
  std::stringstream DebugStreamBuf;
  std::clog.rdbuf(DebugStreamBuf.rdbuf()); //Write debug messages to stringstream instead of console

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
  cout << "Test finished";
  return 0;
}
//---------------------------------------------------------------------------

