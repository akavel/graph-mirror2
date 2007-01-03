/** \mainpage
 *  \section License
 *  \verbatim
 * Copyright 2006 Ivan Johansen
 *
 * Func32 is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * Func32 is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Func32; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 * \endverbatim
 * \ref license "Full license"\n
 * The author can be contacted via email at func32@padowan.dk
 *
 * \section Description
 * The C++ library %Func32 is used for calculations on mathematical functions. The
 * library conatins a number of classes in a polymorphic structure, which makes it
 * easy to do evaluations on different types of functions.

 * \section Use
 * Applications that use %Func32 must include %Func32.h and link to the source files
 * in the library or a library file created from the source files. The Spirit
 * parser library is necessary to compile %Func32. Spirit can be downloaded from
 * http://spirit.sourceforge.net or as part of Boost library collection from
 * http://boost.org. %Func32 has been tested with Spirit 1.6.1. Note that the whole
 * %Func32 library has been placed in namespace Func32.
 */
/** \page license License
 *  \verbinclude COPYING
 */
//---------------------------------------------------------------------------
#ifndef Func32H
#define Func32H
//---------------------------------------------------------------------------
#ifdef _DEBUG
  #define BOOST_ENABLE_ASSERT_HANDLER //Assertion handler defined in application

  #define _STLP_DEBUG  //Debugging version of STLport
  #define _STLP_NO_FORCE_INSTANTIATE
  #define _STLP_USE_EXCEPTIONS
  #undef _STLP_NO_DEBUG_EXCEPTIONS
  #define _STLP_DEBUG_TERMINATE 1  //Call __stl_debug_terminate() to terminate (eg. on assertion failed and similar)
  #define _STLP_DEBUG_MESSAGE 1    //Call __stl_debug_message() to write debug text
  #include <cstdio>
  using std::vsnprintf;
#endif
#include <vector>
#include <complex>
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

//Save options; set data alignment to 64 bit; use smallest data for enum
#pragma option push -a8 -b-

namespace Func32
{

/** This enumeration descibes the different kind of errors that may occur in the
 *  library. Sometime the function TBaseFunc::ErrorCode() is used to report
 *  errors, but most function will throw an EFuncError exception or a descendent
 *  from EFuncError. EFuncError contains an ErrorCode variable indicating the
 *  type of error.
 */
enum TErrorCode
{
  ecNoError        =  0, //!< No error

  //Calculation errors
  ecPowCalcError    =  1, //!< Error while power raising
  ecTanError        =  2, //!< Tangent to 90°+p180°
  ecFactError       =  3, //!< fact must be a positive integer
  ecLogError        =  4, //!< Logarihm to number equal or less than zero
  ecSqrtError       =  5, //!< Sqrt on a negative number'
  ecComplexError    =  6, //!< Some error ocoured because the value had an imaginary part
  ecDivByZero       =  7, //!< Division by zero
  ecArcError        =  8, //!< Inverse trogonometric function out of range [-1;1]
  ecNotDefError     =  9, //!< Position not defined; returned by Sign(x) or U(x) if x=0, or by Undef
  ecATanhError      = 10, //!< atanh(x) is only defined for x=]-1;1] for real numbers
  ecACoshError      = 11, //!< acosh(x) is undefined for z < 1 for real numbers
  ecArgError        = 12, //!< arg(0) is undefined
  ecEvalFailed      = 13, //!< A complex function (W(z) for example) failed to evaluate an accurate result
  ecTotalLoss       = 14, //!< Argument would produce function result with total loss of significant digits, such as sin(10e70)
  ecSymbolNotFound  = 15, //!< Symbol does not exist or has wrong number of arguemnts; A custom function/constant is not updated
  ecRecusionLimit   = 16, //!< Recursive function has been called too many times

  //Parse errors
  ecOperatorError   = 50, //!< Operator cannot be placed here. Example "*5"
  ecNoEndPar        = 55, //!< End of parenthese missing
  ecArgCountError   = 56, //!< Invalid number of arguments when parsing function call
  ecInvalidCompare  = 57, //!< Compare operator misplaced. Only two compare operators allowed. "5<x<y<10" will give the error.
  ecInvalidNumber   = 58, //!< Number expected
  ecEmptyString     = 59, //!< String is empty
  ecCommaError      = 60, //!< Comma not allowed here; Use dot as decimal separator
  ecInvalidEndPar   = 61, //!< Unexpected end of parenthesis; Example: sin();  x^)
  ecFactorExpected  = 63, //!< A factor (Number, constant or function) was expected
  ecParAfterConst   = 64, //!< Parameter after constant or variable not allowed
  ecExpressionExp   = 65, //!< Expression expected
  ecUnknownVar      = 66, //!< Unknown variable found
  ecUnknownChar     = 67, //!< Unknown character found
  ecUnexpectedEnd   = 68, //!< The end of the expression was unexpected; Something else was expected here
  ecInvalidRelation = 69, //!< The expression is not a valid relation; Compare operator (=, <, <=, >, >=) missing.
  ecParseError      = 70, //!< Error parsing function text; Exact cause unknown

  //Other errors
  ecOverflow        = 71, //!< A calculation resulted in an overflow
  ecInvalidArgCount = 72, //!< Too few or too many arguments passed to a TCustomFunc instance
  ecInvalidValue    = 73, //!< An invalid value was used in the calculation
  ecTooFewPoints    = 74, //!< Not enough points for calculation
  ecIllegalName     = 75, //!< Built-in function names cannot be used for custom function names
  ecRecursiveDif    = 76, //!< Cannot differentiate recursve functions
  ecNoFunc          = 78, //!< No function defined
  ecNotDifAble      = 79, //!< Function cannot be differentiated
  ecErrorType       = 85, //!< The function passed as parameter is not correct type
  ecCalcError       = 86, //!< Not specified error under calculation; Overflow, division by zero, etc.
  ecBadGuess        = 87, //!< Too many iterations before solution found; bad guess
  ecInternalError   = 99  //!< Unknown error
};

//---------------------------------------------------------------------------
/*! \brief This class and its decendents are only used for throwing exceptions. This
 *  specific class is used when one of the decendents doesn't fit.
 */
struct EFuncError : public std::exception
{
  TErrorCode ErrorCode; //!<The error code indicating the precise cause of the error.
  std::string Str;      //!< A text supplement to the error code; Can be an unknown variable name
  EFuncError(TErrorCode AErrorCode) : ErrorCode(AErrorCode) {}
  EFuncError(TErrorCode AErrorode, const std::string &AStr) : ErrorCode(AErrorode), Str(AStr) {}
  ~EFuncError() throw() {} //Required by g++ 3.3.3
  const char* what() const throw() {return "Func32::EFuncError";}
};

//!Type used to indicate where an error occured.
enum TErrorFunc
{
  efFunc,   //!< Error in standard function (TFunc)
  efParamX, //!< Error in X-part of parameter function (TParamFunc)
  efParamY, //!< Error in Y-part of parameter function (TParamFunc)
  efPolar   //!< Error in polar function (TPolarFunc)
};

//! Thrown when an error occured while parsing function.
struct EParseError : public EFuncError
{
  TErrorFunc ErrorFunc; //!< Indicates what type of function error occured
  int ErrorPos;         //!< Indicates position in text where error occured
  EParseError(TErrorCode AErrorCode, int AErrorPos, const std::string &AStr)
    : EFuncError(AErrorCode, AStr), ErrorPos(AErrorPos), ErrorFunc(efFunc) {}
  EParseError(TErrorCode AErrorCode=ecNoError, int AErrorPos=-1)
    : EFuncError(AErrorCode), ErrorPos(AErrorPos), ErrorFunc(efFunc) {}
  const char* what() const throw() {return "Func32::EParseError";}
};

//!Thrown when an error occured while calculating.
struct ECalcError : public EFuncError
{
  ECalcError(TErrorCode AErrorCode = ecNoError) : EFuncError(AErrorCode) {}
  ECalcError(TErrorCode AErrorCode, const char *Str) : EFuncError(AErrorCode, Str) {}
  const char* what() const throw() {return "Func32::ECalcError";}
};

//!Struct with an (x,y) coordinate
template<typename T>
struct TCoord
{
  T x;
  T y;
  TCoord() : x(0), y(0) {}
  TCoord(const T &X, const T &Y) : x(X), y(Y) {}
};

//!Function used to create an (x,y) coordinate
template<typename T> inline TCoord<T> MakeCoord(const T &x, const T &y)
{
  return TCoord<T>(x, y);
}

template<typename T>
inline std::ostream& operator<<(std::ostream &Stream, const TCoord<T> &Coord)
{
  return Stream << Coord.x << ',' << Coord.y;
}

template<typename T>
inline std::istream& operator>>(std::istream &Stream, TCoord<T> &Coord)
{
  Stream >> Coord.x;
  Stream.ignore(); //Ignore ','
  return Stream >> Coord.y;
}

//---------------------------------------------------------------------------
//!The type Complex is used for all calculations with complex numbers.
typedef std::complex<long double> Complex;

//!Type that indicates if calculations are done with radians or degrees
enum TTrigonometry {Radian, Degree};

class TFunc;
class TSymbolList;
class TFuncData;

/** \brief This is an abstract class used as base class for TFunc, TParamFunc and
 *  TPolarFunc. The class defines the general function interface as vitual
 *  functions, but it also has some variables and non-virtual functions.
 */
class TBaseFunc
{
protected:
  //!This variable indicates if calculations should use radians or degrees in trigonometric functions.
  TTrigonometry Trigonometry;

  TBaseFunc(TTrigonometry ATrigonometry = Radian)
    : Trigonometry(ATrigonometry) {}
  TBaseFunc(const TBaseFunc &Func)
    : Trigonometry(Func.Trigonometry) {}

  void SwapBase(TBaseFunc &Func); //throw()
  const TBaseFunc& operator=(const TBaseFunc &F);

  //! Ensures that data are unique and no differentiation is buffered
  virtual void Unique() =0;

public:
  virtual ~TBaseFunc() {}

  //!Returns a new copy of the actual class instance
  virtual TBaseFunc* Clone() const =0;

  //NOTE: Trigonometry may change the differentiation
  void SetTrigonometry(TTrigonometry ATrigonometry) {Unique(); Trigonometry = ATrigonometry;}
  TTrigonometry GetTrigonometry() const {return Trigonometry;}

  virtual TCoord<long double> Calc(long double, ECalcError &E) const =0; //throw()
  virtual TCoord<Complex> Calc(Complex, ECalcError &E) const =0; //throw()

  virtual long double CalcX(long double, ECalcError &E) const =0; //throw()
  virtual Complex CalcX(Complex, ECalcError &E) const =0; //throw()

  virtual long double CalcY(long double, ECalcError &E) const =0; //throw()
  virtual Complex CalcY(Complex, ECalcError &E) const =0; //throw()

  TCoord<long double> Calc(long double) const;
  TCoord<Complex> Calc(Complex) const;

  long double CalcX(long double) const;
  Complex CalcX(Complex) const;

  long double CalcY(long double) const;
  Complex CalcY(Complex) const;

  /** Calculates the slope of the function at s. The calculation differentiating the function.
   *  \param s: Position to calculate slope at.
   *  \return The slope at s; +INF or -INF if slope is vertical depending of direction
   *  \throw ECalcError: Thrown when calculation fails
   *  \throw EFuncError: Thrown with ecNotDifAble if differentiation fails
   */
  virtual long double CalcSlope(long double s) const =0;

  /** This creates the first derivative of the function and returns a pointer to
   *  created function. Ownership is passed to the caller. The functions is of
   *  the same type as the original.
   *  \throw EFuncError: Thrown if function could not be differentiated
   */
  virtual TBaseFunc* MakeDifPtr() const =0;

  //! \overload
  virtual TBaseFunc* MakeDifPtr(const std::string &Var) const =0;

  //!This will clear the function. Any calculations on the function before it is set will result in an error.
  virtual void Clear()=0;

  //!Returns true if the function has not been set. Any calculations on an empty function will result in an error.
  virtual bool IsEmpty() const =0; //throw()

  //!Calculates the area of the function from sMin to sMax. The exact area depends on the type of function
  virtual long double CalcArea(long double sMin, long double sMax, unsigned n) const =0;

  //!Calculates the length of the function from sMin to sMax along the curve
  //!using Simpson's formula. n is the number of steps, and n must be even.
  virtual long double CalcArc(long double sMin, long double sMax, unsigned n) const =0;

  //!Returns a standard function that define the x-part of the function.
  virtual TFunc ConvXToFunc() const =0;

  //!Returns a standard function that defines the y-part of the function.
  virtual TFunc ConvYToFunc() const =0;

  //!Reduce function expressions for faster calculations. This is called
  //!implicitly for creation of the first derivative.
  virtual void Simplify() =0;

  /** This returns the slope of the function as an angle represented in radians.
   *  \param s: The position on function to calculate slope
   *  \return The angular slope in radians at s. 0 is horizontal right, and
   *  counter clockwize is positive. The value is in the range -PI to PI.
   *  \throw ECalcError on calculation error
   */
  virtual long double CalcAngleSlope(long double s) const =0;

  //!Assigns F to the function.
  //!\throw EFuncError: Thrown with ErrorCode=ecErrorType if the two functions are not of the same type.
  virtual void Assign(const TBaseFunc &F) = 0;

  virtual bool Update(const TSymbolList &SymbolList) =0;
};
//---------------------------------------------------------------------------
//!The TFunc class is derived from TBaseFunc. The class models a standard function y=f(x).
class TFunc : public TBaseFunc
{
  friend TFunc pow(const TFunc&, const TFunc&);
  friend TFunc log(const TFunc&);
  friend TFunc sqrt(const TFunc&);
  friend class TBaseFunc;
  friend class TParamFunc;
  friend class TPolarFunc;
  friend class TFuncData;

  boost::shared_ptr<TFuncData> FuncData;
  mutable boost::shared_ptr<TFuncData> DifData;
  void Unique();

public:
  using TBaseFunc::CalcX;
  using TBaseFunc::CalcY;

  TFunc();
  //!Copy constructor creates a TFunc object from another function
  TFunc(const TFunc &Func) : TBaseFunc(Func), FuncData(Func.FuncData) {}
  TFunc(long double Value);
  TFunc(const std::string &Text, const std::string &Variable = "x", TTrigonometry ATrigonometry = Radian);
  TFunc(const std::string &Text, const std::string &Variable, const TSymbolList &SymbolList, TTrigonometry ATrigonometry = Radian);
  TFunc(const boost::shared_ptr<TFuncData> &AFuncData, TTrigonometry ATrigonometry) : TBaseFunc(ATrigonometry), FuncData(AFuncData) {}

  TFunc* Clone() const {return new TFunc(*this);}

  void SetFunc(const std::string &Text, const std::string &Variable = "x");
  void SetFunc(const std::string &Text, const std::string &Variable, const TSymbolList &SymbolList);

  long double CalcX(long double x, ECalcError &E) const; //throw()
  Complex CalcX(Complex, ECalcError &E) const; //throw()

  long double CalcY(long double x, ECalcError &E) const; //throw()
  Complex CalcY(Complex, ECalcError &E) const; //throw()

  TCoord<long double> Calc(long double, ECalcError &E) const; //throw()
  TCoord<Complex> Calc(Complex, ECalcError &E) const; //throw()

  long double CalcSlope(long double x) const;
  long double CalcArea(long double xMin, long double xMax, unsigned n) const;
  long double CalcArc(long double xMin, long double xMax, unsigned n) const;
  long double CalcAngleSlope(long double s) const;

  TFunc* MakeDifPtr() const;
  TFunc* MakeDifPtr(const std::string &Var) const;
  TFunc MakeDif() const;
  TFunc MakeDif(const std::string &Var) const;

  void Clear();
  bool IsEmpty() const; //throw()
  std::string MakeText(const std::string &Variable = "x") const;

  void Simplify();
  void Swap(TFunc &Func); //throw()

  TFunc ConvXToFunc() const {return TFunc("x", "x", Trigonometry);}
  TFunc ConvYToFunc() const {return *this;}

  bool Update(const TSymbolList &SymbolList);

  //Overloaded operators
  TFunc& operator=(const TFunc &Func);
  void Assign(const TBaseFunc &F);
  friend const TFunc operator+(TFunc Func1,const TFunc &Func2);
  friend const TFunc operator-(TFunc Func1,const TFunc &Func2);
  friend const TFunc operator*(TFunc Func1,const TFunc &Func2);
  friend const TFunc operator/(TFunc Func1,const TFunc &Func2);
  TFunc& operator+=(const TFunc &Func);
  TFunc& operator-=(const TFunc &Func);
  TFunc& operator*=(const TFunc &Func);
  TFunc& operator/=(const TFunc &Func);
  bool operator==(const TFunc &Func) const;
  bool operator!=(const TFunc &Func) const;
  long double operator()(long double x) const;
  Complex operator()(Complex x) const;
};
//---------------------------------------------------------------------------
/*! \brief The TParamFunc class is derived from TBaseFunc. It models a parameter
 *  function f(t)=x(t),y(t).
 */
class TParamFunc:public TBaseFunc
{
  boost::shared_ptr<TFuncData> xFuncData;
  boost::shared_ptr<TFuncData> yFuncData;
  mutable boost::shared_ptr<TFuncData> xDifData;
  mutable boost::shared_ptr<TFuncData> yDifData;

  void Unique();
  void EnsureDif() const;

public:
  using TBaseFunc::CalcX;
  using TBaseFunc::CalcY;

  TParamFunc();
  TParamFunc(const std::string &xText, const std::string &yText, const std::string &Variable = "t", TTrigonometry ATrigonometry = Radian);
  TParamFunc(const std::string &xText, const std::string &yText, const std::string &Variable, const TSymbolList &SymbolList, TTrigonometry ATrigonometry = Radian);
  TParamFunc(const TParamFunc &Func) : TBaseFunc(Func), xFuncData(Func.xFuncData), yFuncData(Func.yFuncData) {}
  TParamFunc(long double xValue, long double yValue);
  explicit TParamFunc(const TFunc &Func);
  TParamFunc(const boost::shared_ptr<TFuncData> &AxFuncData, const boost::shared_ptr<TFuncData> &AyFuncData, TTrigonometry ATrigonometry)
    : TBaseFunc(ATrigonometry), xFuncData(AxFuncData), yFuncData(AyFuncData) {}

  TParamFunc* Clone() const {return new TParamFunc(*this);}
  void SetFunc(const std::string &xText, const std::string &yText, const std::string &Variable = "t");
  void SetFunc(const std::string &xText, const std::string &yText, const std::string &Variable, const TSymbolList &SymbolList);

  long double CalcX(long double t, ECalcError &E) const; //throw()
  Complex CalcX(Complex, ECalcError &E) const; //throw()

  long double CalcY(long double t, ECalcError &E) const; //throw()
  Complex CalcY(Complex, ECalcError &E) const; //throw()

  TCoord<long double> Calc(long double, ECalcError &E) const; //throw()
  TCoord<Complex> Calc(Complex, ECalcError &E) const; //throw()

  long double CalcSlope(long double t) const;
  long double CalcArea(long double sMin, long double sMax, unsigned n) const;
  long double CalcArc(long double tMin, long double tMax, unsigned n) const;
  long double CalcAngleSlope(long double s) const;

  TParamFunc* MakeDifPtr() const;
  TParamFunc* MakeDifPtr(const std::string &Var) const;
  TParamFunc MakeDif() const;
  TParamFunc MakeDif(const std::string &Var) const;

  void Clear();
  bool IsEmpty() const; //throw()
  std::string MakeXText(const std::string &Variable = "t") const;
  std::string MakeYText(const std::string &Variable = "t") const;

  TFunc ConvXToFunc() const;
  TFunc ConvYToFunc() const;

  void Swap(TParamFunc &Func); //throw()
  void Simplify();

  bool Update(const TSymbolList &SymbolList);

//Overloaded operators
  TParamFunc& operator=(const TParamFunc &Func);
  void Assign(const TBaseFunc &F);
  friend const TParamFunc operator+(TParamFunc Func1,const TParamFunc &Func2);
  friend const TParamFunc operator-(TParamFunc Func1,const TParamFunc &Func2);
  friend const TParamFunc operator*(long double Value, const TParamFunc &Func);
  friend const TParamFunc operator/(long double Value, const TParamFunc &Func);
  const TParamFunc operator*(long double Value) const;
  const TParamFunc operator/(long double Value) const;
  TParamFunc& operator+=(const TParamFunc &Func);
  TParamFunc& operator-=(const TParamFunc &Func);
  TParamFunc& operator*=(long double Value);
  TParamFunc& operator/=(long double Value);
  bool operator==(const TParamFunc &Func) const;
  bool operator!=(const TParamFunc &Func) const;
};
//---------------------------------------------------------------------------
//! The TPolarFunc class is derived from TBaseFunc. It models a polar function r(t).
class TPolarFunc : public TBaseFunc
{
  boost::shared_ptr<TFuncData> FuncData;
  mutable boost::shared_ptr<TFuncData> DifData;

  void Unique();

public:
  using TBaseFunc::Calc;
  using TBaseFunc::CalcX;
  using TBaseFunc::CalcY;

  TPolarFunc();
  TPolarFunc(const std::string &Text, const std::string &Variable = "t", TTrigonometry ATrigonometry = Radian);
  TPolarFunc(const std::string &Text, const std::string &Variable, const TSymbolList &SymbolList, TTrigonometry ATrigonometry = Radian);
  TPolarFunc(const TPolarFunc &Func) : TBaseFunc(Func), FuncData(Func.FuncData) {}
  TPolarFunc(const boost::shared_ptr<TFuncData> &AFuncData, TTrigonometry ATrigonometry) : TBaseFunc(ATrigonometry), FuncData(AFuncData) {}

  TPolarFunc* Clone() const {return new TPolarFunc(*this);}
  void SetFunc(const std::string &Text, const std::string &Variable = "t");
  void SetFunc(const std::string &Text, const std::string &Variable, const TSymbolList &SymbolList);

  long double CalcX(long double t, ECalcError &E) const; //throw()
  Complex CalcX(Complex, ECalcError &E) const; //throw()

  long double CalcY(long double t, ECalcError &E) const; //throw()
  Complex CalcY(Complex, ECalcError &E) const; //throw()

  TCoord<long double> Calc(long double, ECalcError &E) const; //throw()
  TCoord<Complex> Calc(Complex, ECalcError &E) const; //throw()

  long double CalcSlope(long double t) const;
  long double CalcR(long double t) const;
  long double CalcArc(long double tMin, long double tMax, unsigned n) const;
  long double CalcArea(long double tMin, long double tMax, unsigned n) const;
  long double CalcAngleSlope(long double s) const;

  TPolarFunc* MakeDifPtr() const;
  TPolarFunc* MakeDifPtr(const std::string &Var) const;
  TPolarFunc MakeDif() const;
  TPolarFunc MakeDif(const std::string &Var) const;

  void Clear();
  bool IsEmpty() const; //throw()
  std::string MakeText(const std::string &Variable = "t") const;

  TFunc ConvXToFunc() const;
  TFunc ConvYToFunc() const;
  TFunc ConvToFunc() const;

  void Swap(TPolarFunc &Func); //throw()
  void Simplify();
  void Assign(const TBaseFunc &F);
  bool Update(const TSymbolList &SymbolList);

//Overloaded operators
  TPolarFunc& operator=(const TPolarFunc &Func);
  bool operator==(const TPolarFunc &Func) const;
  bool operator!=(const TPolarFunc &Func) const;
  long double operator()(long double t) const;
};
//---------------------------------------------------------------------------
typedef std::vector<std::string> TArgType;
enum TFunctionType {ftEmpty, ftFunction, ftEquation, ftInequality};
class TCustomFunc
{
  friend class TFuncData;
  friend class TSymbolList;

  boost::shared_ptr<TFuncData> FuncData;
  TArgType Args;
  TTrigonometry Trigonometry;

  TCustomFunc(const boost::shared_ptr<TFuncData> &AFuncData, const TArgType &AArgs, TTrigonometry ATrigonometry)
    : FuncData(AFuncData), Args(AArgs), Trigonometry(ATrigonometry) {}
  void Unique();

public:
  TCustomFunc();
  TCustomFunc(const std::string &Text, const TArgType &AArgs, TTrigonometry Trig = Radian);
  TCustomFunc(const std::string &Text, const TArgType &AArgs, const TSymbolList &SymbolList, TTrigonometry Trig = Radian);
  TCustomFunc(long double Value);
  void SetFunc(const std::string &Text, const TArgType &AArgs);
  void SetFunc(const std::string &Text, const TArgType &AArgs, const TSymbolList &SymbolList);
  long double Calc(const std::vector<long double> &Values) const;
  long double Calc(const std::vector<long double> &Values, ECalcError &E) const;
  TCustomFunc MakeDif(unsigned Arg) const;
  const TArgType& GetArguments() const {return Args;}
  void SetTrigonometry(TTrigonometry Trig) {Trigonometry = Trig;}
  TTrigonometry GetTrigonometry() const {return Trigonometry;}
  TFunc ConvToFunc(const std::vector<long double> &Values, unsigned Variable) const;
  std::string MakeText() const;
  bool Update(const TSymbolList &SymbolList);
  TFunctionType GetFunctionType() const;
  void RemoveRelation();
  bool IsEmpty() const;

  long double operator()(const std::vector<long double> &Values) const {return Calc(Values);}
};

class TSymbolList
{
  std::map<std::string, TCustomFunc> List;

public:
  typedef std::map<std::string, TCustomFunc>::iterator TIterator;
  typedef std::map<std::string, TCustomFunc>::const_iterator TConstIterator;

  void Add(const std::string &Key);
  void Add(const std::string &Key, const std::string &Value, const std::vector<std::string> &Args = std::vector<std::string>());
  void Add(const std::string &Key, const TCustomFunc &CustomFunc);
  TConstIterator Begin() const {return List.begin();}
  TIterator Begin() {return List.begin();}
  TConstIterator End() const {return List.end();}
  TIterator End() {return List.end();}
  const TCustomFunc& Get(const std::string &Key) const;
  void Clear() {List.clear();}
  bool Empty() const {return List.empty();}
  bool Exists(const std::string &Key) const;
  void Erase(const std::string &Key);
  unsigned Size() const {return List.size();}
  void Swap(TSymbolList &SymbolList) {List.swap(SymbolList.List);}
  void Update() {for(TIterator Iter = List.begin(); Iter != List.end(); ++Iter) Iter->second.Update(*this);}
  TSymbolList& operator+=(const TSymbolList &SymbolList) {List.insert(SymbolList.List.begin(), SymbolList.List.end()); return *this;}
  TSymbolList operator+(const TSymbolList &SymbolList) const {return TSymbolList(*this) += SymbolList;}
  TSymbolList& operator-=(const TSymbolList &SymbolList)
  {
    for(TConstIterator Iter = SymbolList.List.begin(); Iter != SymbolList.List.end(); ++Iter)
      List.erase(Iter->first);
    return *this;
  }
  TSymbolList operator-(const TSymbolList &SymbolList) const {return TSymbolList(*this) -= SymbolList;}
};
//---------------------------------------------------------------------------
template<typename T>
inline TCoord<long double> GetReal(const TCoord<T> &Coord)
{
  return TCoord<long double>(GetReal(Coord.x), GetReal(Coord.y));
}
//---------------------------------------------------------------------------
inline long double GetReal(Complex Number)
{
  if(imag(Number))
    throw ECalcError(ecComplexError);
  return std::real(Number);
}
//---------------------------------------------------------------------------
inline long double GetReal(long double Number)
{
  return Number;
}
//---------------------------------------------------------------------------

typedef TCoord<double> TDblPoint;

//! Type use with the TrendLine() function to find a trend in a point series.
enum TTrendType
{
  ttLinear,       //!< Linear trend (y = a*x + b)
  ttLogarithmic,  //!< Logarithmic trend (y = a*ln(x) + b)
  ttPolynomial,   //!< Polynomial trend (y = an*x^n + ... + a3*x^3 + a2*x^2 + a1*x + a0). The order is given as argument to TrendLine().
  ttPower,        //!< Power trend (y = a*x^b)
  ttExponential,  //!< Exponential trend (y = a*b^x)
};

TFunc TrendLine(TTrendType Type, const std::vector<TDblPoint> &Points, unsigned N);
TFunc TrendLine(TTrendType Type, const std::vector<TDblPoint> &Points, unsigned N, double Intercept);
TParamFunc MovingAverage(const std::vector<TDblPoint> &Points, unsigned N);
void Regression(const std::vector<TDblPoint> &Points, const TCustomFunc &Func, std::vector<long double> &Values, unsigned MaxIter = 100, double Tol = 1E-6);
double CalcSSQ(const std::vector<TDblPoint> &Points, const TFunc &Func);
double LinearCorrelation(const std::vector<TDblPoint> &Points);
double Correlation(const std::vector<TDblPoint> &Points, const TFunc &Func);

std::ostream& operator<<(std::ostream &os, const TFunc &Func);
std::istream& operator>>(std::istream &is, TFunc &Func);

long double Eval(const std::string &Expr, TTrigonometry Trig = Radian);
long double Eval(const std::string &Expr, const TSymbolList &SymbolList, TTrigonometry Trig = Radian);
long double Eval(const std::string &Expr, long double x, const std::string &Var = "x", TTrigonometry Trig = Radian);
Complex EvalComplex(const std::string &Expr, TTrigonometry Trig = Radian);
Complex EvalComplex(const std::string &Expr, const TSymbolList &SymbolList, TTrigonometry Trig = Radian);
Complex EvalComplex(const std::string &Expr, Complex x, const std::string &Var = "x", TTrigonometry Trig = Radian);

long double FindCrossing(const TBaseFunc &Func1, long double Min1, long double Max1, const TBaseFunc &Func2, long double Min2, long double Max2, long double Tol);
std::vector<std::string> FindUnknowns(const std::string &Str);
bool IsValidName(const std::string &Name);

struct TCoordSet
{
  long double t;
  long double x;
  long double y;
  TCoordSet(long double at, long double ax, long double ay) : t(at), x(ax), y(ay) {}
  TCoordSet(long double at, const TCoord<long double> &Coord) : t(at), x(Coord.x), y(Coord.y) {}
};

class TEvalCoordSet
{
  const TBaseFunc &Func;
public:
  TEvalCoordSet(const TBaseFunc &AFunc) : Func(AFunc) {}
  TCoordSet operator()(const TCoordSet &CoordSet) {return TCoordSet(CoordSet.t, Func.Calc(CoordSet.t));}
};

enum TAnalyseType {atXAxisCross, atYAxisCross};
std::vector<TCoordSet> AnalyseFunction(const TBaseFunc &Func, long double Min, long double Max, unsigned Steps, long double Tol, TAnalyseType AnalyseType);
} //namespace Func32
//---------------------------------------------------------------------------

#pragma option pop
#endif



