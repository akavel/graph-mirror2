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
#include <limits>
#include "Matrix.h"
#include <boost\math\special_functions\fpclassify.hpp>
#include <limits>

namespace Func32
{
//---------------------------------------------------------------------------
/** Create a linear curve y=a*x+b of best fit using Least Squares method.
 *  If Variance is not empty, the Sum of Squares is weighted by the inverse of each variance value.
 *  \param Points: The point series to find trendline for
 *  \param Weights: The weight for each point. The size of Weights must be the same as the size of Points or 0.
 *         The wight is usually 1/(sigma*sigma) where sigma is the uncertainty.
 *  \param Intercept: Indicate the interception with the y-axis. If Intercept==NaN, Intercept is considered unknown.
 *  \return The trendline for the points
 */
TFunc CreateLinearTrendLine(const std::vector<TDblPoint> &Points, const std::vector<double> &Weights, double Intercept)
{
  if(Points.empty() || (boost::math::isfinite(Intercept) && Points.size() < 2))
    throw EFuncError(ecTooFewPoints);
  int n = Points.size();
  double SumX = 0;
  double SumY = 0;
  double SumXY = 0;
  double SumSqrX = 0;
  double SumWeight = 0;

  for(int I = 0; I < n; I++)
  {
    double Weight = Weights.empty() ? 1 : Weights[I];
    SumX += Points[I].x * Weight;
    SumY += Points[I].y * Weight;
    SumXY += Points[I].x * Points[I].y * Weight;
    SumSqrX += Points[I].x * Points[I].x * Weight;
    SumWeight += Weight;
  }

  double a;
  double b = Intercept;
  if(boost::math::isfinite(Intercept)) //If an interception with y-axis is given
    a = (SumXY - b * SumX) / (SumWeight*SumSqrX);
  else
  {
    double d = SumWeight*SumSqrX - SumX*SumX;

    //Take care of division by zero
    if(IsZero(d))
      throw EFuncError(ecOverflow);
    a = (SumWeight*SumXY - SumX*SumY) / d;
    b = (SumY - a*SumX) / SumWeight;
  }

  return TFunc(a) * TFunc(L"x") + TFunc(b);
}
//---------------------------------------------------------------------------
/** Returns the curve of best fit for the point series using the Least Squares fit.
 *  If Variance is not empty, the Sum of Squares is weighted by the inverse of each variance value.
 *  \param Type: Indicates the type of trend to find
 *  \param Points: The point series to find trendline for
 *  \param Weights: The weight for each point. The size of Weights must be the same as the size of Points or 0.
           The wight is usually 1/(sigma*sigma) where sigma is the uncertainty.
 *  \param N: Only used for polygons (order)
 *  \return The trendline for the points
 */
TFunc TrendLine(Func32::TTrendType Type, const std::vector<TDblPoint> &Points, const std::vector<double> &Weights, unsigned N)
{
  if(!Weights.empty() && Weights.size() != Points.size())
    throw EFuncError(ecInvalidWeight);
  try
  {
    switch(Type)
    {
      case ttLogarithmic:
      {
        //y = a*ln(x) + b
        if(Points.size() < 2)
          throw EFuncError(ecTooFewPoints);
        int n = Points.size();
        double SumX = 0;
        double SumY = 0;
        double SumXY = 0;
        double SumSqrX = 0;

        for(std::vector<TDblPoint>::const_iterator Iter = Points.begin(); Iter != Points.end(); ++Iter)
        {
          if(Iter->x <= 0)
            throw EFuncError(ecInvalidValue);
          double LogX = std::log(Iter->x);
          SumX += LogX;
          SumY += Iter->y;
          SumXY += LogX * Iter->y;
          SumSqrX += LogX * LogX;
        }

        double d = n*SumSqrX-SumX*SumX;

        //Take care of division by zero
        if(IsZero(d))
          throw EFuncError(ecOverflow);

        double a = ((n*SumXY-SumX*SumY) / d);
        double b = (SumY - a*SumX) / n;
        return TFunc(a) * log(TFunc(L"x")) + TFunc(b);
      }

      case ttPolynomial:
      {
        //y = an*x^n + ... + a3*x^3 + a2*x^2 + a1*x + a0
        if(Points.size() <= N)
          throw EFuncError(ecTooFewPoints);

        TMatrix<long double> M(N+1, N+2);
        std::vector<long double> SumX(2*(N+1), 0);
        for(std::vector<TDblPoint>::const_iterator Iter = Points.begin(); Iter != Points.end(); ++Iter)
        {
          long double PowX = 1;
          for(unsigned n = 0; n < 2*(N+1); n++)
          {
            SumX[n] += PowX;
            if(n <= N)
              M[n][N+1] += PowX * Iter->y;
            PowX *= Iter->x;
          }
        }

        for(unsigned j = 0; j <= N; j++)
          for(unsigned k = 0; k <= N; k++)
            M[j][k] = SumX[j+k];

        std::vector<long double> a = M.Gauss();
        TFunc Func;
        TFunc xFunc(L"x");
        for(unsigned n = a.size()-1; n >= 2; n--)
          if(a[n] < 0)
            Func -= -a[n] * pow(xFunc, n);
          else
            Func += a[n] * pow(xFunc, n);
        if(N>0)
          if(a[1] < 0)
            Func -= -a[1] * xFunc;
          else
            Func += a[1] * xFunc;
        if(a[0] < 0)
          Func -= -a[0];
        else
          Func += a[0];
        return Func;
      }

      case ttPower:
      {
        //y = a*x^b, ln y = ln a +b*ln x
        if(Points.size() < 2)
          throw EFuncError(ecTooFewPoints);
        int n = Points.size();
        double SumX = 0;
        double SumY = 0;
        double SumXY = 0;
        double SumSqrX = 0;

        for(std::vector<TDblPoint>::const_iterator Iter = Points.begin(); Iter != Points.end(); ++Iter)
        {
          //The power function is not defined for negative values. The algorithm cannot handle x=0 and y=0, and
          //as (0,0) is defined for the function, we ignore points with x=0 or y=0
          if(Iter->x < 0 || Iter->y < 0)
            throw EFuncError(ecInvalidValue);
          else if(Iter->x == 0 || Iter->y == 0)
            n--;
          else
          {
            double LogX = std::log(Iter->x);
            double LogY = std::log(Iter->y);
            SumX += LogX;
            SumY += LogY;
            SumXY += LogX * LogY;
            SumSqrX += LogX * LogX;
          }
        }

        double d = n*SumSqrX-SumX*SumX;

        //Take care of division by zero
        if(IsZero(d))
          throw EFuncError(ecOverflow);

        long double b = (n*SumXY - SumX*SumY) / d;
        long double a = std::exp((SumY - b*SumX) / n);

        return TFunc(a) * pow(TFunc(L"x"),TFunc(b));
      }

      case ttExponential:
      {
        //y = a*b^x, ln y = ln a + (ln b)*x
        if(Points.size() < 2)
          throw EFuncError(ecTooFewPoints);
        int n = Points.size();
        long double SumX = 0;
        long double SumY = 0;
        long double SumXY = 0;
        long double SumSqrX = 0;

        for(std::vector<TDblPoint>::const_iterator Iter = Points.begin(); Iter != Points.end(); ++Iter)
        {
          if(Iter->y <= 0)
            throw EFuncError(ecInvalidValue);
          long double LogY = std::log(Iter->y);
          SumX += Iter->x;
          SumY += LogY;
          SumXY += Iter->x * LogY;
          SumSqrX += Iter->x * Iter->x;
        }

        long double d = (n*SumSqrX-SumX*SumX);

        //Take care of division by zero
        if(IsZero(d))
          throw EFuncError(ecOverflow);

        long double LogB = (n*SumXY - SumX*SumY) / d;
        long double LogA = (SumY - LogB*SumX) / n;
        long double a = std::expl(LogA);
        long double b = std::expl(LogB);
        return TFunc(a) * pow(TFunc(b),TFunc(L"x"));
      }

      default:
        return TrendLine(Type, Points, Weights, N, std::numeric_limits<double>::quiet_NaN());
    }
  }
  catch(EMatrix& M)
  {
    throw EFuncError(ecCalcError);
  }
}
//---------------------------------------------------------------------------
/** Returns the curve of best fit for the point series using the Least Squares fit.
 *  If Variance is not empty, the Sum of Squares is weighted by the inverse of each variance value.
 *  \param Type: Indicates the type of trend to find
 *  \param Points: The point series to find trendline for
 *  \param Weights: The weight for each point. The size of Weights must be the same as the size of Points or 0.
           The wight is usually 1/(sigma*sigma) where sigma is the uncertainty.
 *  \param N: Only used for polygons (order)
 *  \param Intercept: If not NaN, indicates the crossing with the y-axis.
 *  \return The trendline for the points
 */
TFunc TrendLine(TTrendType Type, const std::vector<TDblPoint> &Points, const std::vector<double> &Weights, unsigned N, double Intercept)
{
  try
  {
    switch(Type)
    {
      case ttLinear:
        //y = a*x + b, b=Intercept
        return CreateLinearTrendLine(Points, Weights, Intercept);

      case ttPolynomial:
        {
          //y = an*x^n + ... + a3*x^3 + a2*x^2 + a1*x + a0, where a0=const
          if(Points.size() < N)
            throw EFuncError(ecTooFewPoints);

          double q = Intercept;
          TMatrix<long double> M(N, N+1);
          std::vector<long double> SumX(2*N, 0);
          for(std::vector<TDblPoint>::const_iterator Iter = Points.begin(); Iter != Points.end(); ++Iter)
          {
            long double PowX = 1;
            for(unsigned n = 0; n < 2*N; n++)
            {
              PowX *= Iter->x;
              SumX[n] += PowX;
              if(n < N)
                M[n][N] += PowX * (Iter->y - q);
            }
          }

          for(unsigned j = 0; j < N; j++)
            for(unsigned k = 0; k < N; k++)
              M[j][k] = SumX[j+k+1];

          std::vector<long double> a = M.Gauss();
          TFunc Func;
          TFunc xFunc(L"x");
          for(unsigned n = a.size()-1; n >= 1; n--)
            if(a[n] < 0)
              Func -= -a[n] * pow(xFunc, n+1);
            else
              Func += a[n] * pow(xFunc, n+1);
          if(N>0)
            if(a[0] < 0)
              Func -= -a[0] * xFunc;
            else
              Func += a[0] * xFunc;
          if(q < 0)
            Func -= -q;
          else
            Func += q;
          return Func;
        }

      case ttExponential:
        {
          //y = a*b^x, ln y = ln a + (ln b)*x, a=Intercept
          if(Points.size() < 1)
            throw EFuncError(ecTooFewPoints);
            if(Intercept <= 0)
              throw EFuncError(ecInvalidValue);
          double SumX = 0;
          double SumXY = 0;
          double SumSqrX = 0;

          for(std::vector<TDblPoint>::const_iterator Iter = Points.begin(); Iter != Points.end(); ++Iter)
          {
            if(Iter->y <= 0)
              throw EFuncError(ecInvalidValue);
            SumX += Iter->x;
            SumXY += Iter->x * std::log(Iter->y);
            SumSqrX += Iter->x * Iter->x;
          }

          if(IsZero(SumSqrX))
            throw EFuncError(ecOverflow);

          double a = Intercept;
          double b = std::exp((SumXY - std::log(a) * SumX) / SumSqrX);
          return TFunc(a) * pow(TFunc(b),TFunc(L"x"));
        }

      case ttLogarithmic:
      case ttPower:
      default:
        throw EFuncError(ecInternalError);
    }
  }
  catch(EMatrix& M)
  {
    throw EFuncError(ecCalcError);
  }
}
//---------------------------------------------------------------------------
/** Create a function with a moving average of the data points
 *  x(t)=ifseq(t<1, x1 + a1*t, t<2, x2 + a2*t, ...)
 *  y(t)=ifseq(t<1, y1 + b1*t, t<2, y2 + b2*t, ...)
 *  \param Points: Data points to create moving average from
 *  \param N:      The average period; Number of coordiantes used for average.
 *  \return A parameter function with the moving average
 */
TParamFunc MovingAverage(const std::vector<TDblPoint> &Points, unsigned N)
{
  if(Points.size() <= N || N == 0)
    throw EFuncError(ecTooFewPoints);

  double Average = 0;
  for(unsigned I = 0; I < N; I++)
    Average += Points[I].y;
  Average /= N;

  std::wostringstream xStr, yStr;
  xStr << "ifseq(";
  yStr << "ifseq(";

  for(unsigned I = N; I < Points.size(); I++)
  {
    double OldAverage = Average;
    Average -= Points[I - N].y / N;
    Average += Points[I].y / N;

    double b = Average - OldAverage;
    double a = Points[I].x - Points[I - 1].x;

    if(I != N)
    {
      xStr << ',';
      yStr << ',';
    }

    double x1 = Points[I-1].x - a*(I-N);
    double y1 = OldAverage - b*(I-N);

    xStr << std::uppercase << "t<" << (I-N+1) << ',' << x1 << '+' << a << 't';
    yStr << std::uppercase << "t<" << (I-N+1) << ',' << y1 << '+' << b << 't';
  }

  xStr << ')';
  yStr << ')';
  return TParamFunc(xStr.str(), yStr.str());
}
//---------------------------------------------------------------------------
/** Returns the sum of squares: F=sum((y(xk)-yk)^2). (xk, yk) is the coordinate number k.
 *  \param Points: Series of data points
 *  \param Func:   Function, usually a trendline, to find the square error for.
 *  \return The SSQ (Sum of Squares)
 */
double CalcSSQ(const std::vector<TDblPoint> &Points, const TFunc &Func)
{
  double Sum = 0;
  std::vector<TDblPoint>::const_iterator Begin = Points.begin();
  std::vector<TDblPoint>::const_iterator End = Points.end();
  for(std::vector<TDblPoint>::const_iterator Iter = Begin; Iter != End; ++Iter)
  {
    double d = Iter->y - Func.CalcY(Iter->x);
    Sum += d*d;
  }
  return Sum;
}
//---------------------------------------------------------------------------
/** Returns the sum of squared error: F=sum((y(xk)-yk)^2). (xk, yk) is the coordinate number k.
 *  \param Points: Series of data points
 *  \param Func:   Custom function, usually a trendline, to find the square error for.
 *  \param Values: Arguments passed as arguments to the functions. Values[0] is ignored and replaced with the x-coordinates.
 *  \return The SSQ (sum of squares)
 */
double CalcSSQ(const std::vector<TDblPoint> &Points, const TCustomFunc &Func, std::vector<long double> Values)
{
  BOOST_ASSERT(Values.size() == Func.GetArguments().size());
  long double Sum = 0;
  std::vector<TDblPoint>::const_iterator Begin = Points.begin();
  std::vector<TDblPoint>::const_iterator End = Points.end();
  for(std::vector<TDblPoint>::const_iterator Iter = Begin; Iter != End; ++Iter)
  {
    Values[0] = Iter->x;
    long double d = Iter->y - Func(Values);
    Sum += d*d;
  }

  //Values larger than MAXDOUBLE are giving problems, especially when written to a stream
  if(Sum > std::numeric_limits<double>::max())
    return std::numeric_limits<double>::infinity();
  return Sum;
}
//---------------------------------------------------------------------------
/** Returns the Scaled Root of the Mean Square: R(%) = sqrt(F*m)/sum(yk)*100
 *  F is the square error; m is the number of points; yk is the y-coordinate of point k
 *  \param Points: Series of data points
 *  \param Func:   Function, usually a trendline, to find the scaled error for.
 *  \return The scaled error
 */
double ScaledRMS(const std::vector<TDblPoint> &Points, const TFunc &Func)
{
  double F = 0;
  double SumY = 0;
  std::vector<TDblPoint>::const_iterator Begin = Points.begin();
  std::vector<TDblPoint>::const_iterator End = Points.end();
  for(std::vector<TDblPoint>::const_iterator Iter = Begin; Iter != End; ++Iter)
  {
    double d = Iter->y - Func.CalcY(Iter->x);
    F += d*d;
    SumY += Iter->y;
  }
  return std::sqrt(F * Points.size()) / SumY * 100;
}
//---------------------------------------------------------------------------
/** Finds the correlation coefficient (R^2) for the linear best fit.
 *  The closer the correlation is to 1 the nearer the points are to a straight line.
 *  \param Points: Series of data points
 */
double LinearCorrelation(const std::vector<TDblPoint> &Points)
{
  double SumX = 0, SumY = 0;

  std::vector<TDblPoint>::const_iterator Begin = Points.begin();
  std::vector<TDblPoint>::const_iterator End = Points.end();
  for(std::vector<TDblPoint>::const_iterator Iter = Begin; Iter != End; ++Iter)
  {
    SumX += Iter->x;
    SumY += Iter->y;
  }

  double MeanX = SumX / Points.size();
  double MeanY = SumY / Points.size();

  SumX = 0; SumY = 0;
  double SumXY = 0;
  for(std::vector<TDblPoint>::const_iterator Iter = Begin; Iter != End; ++Iter)
  {
    SumX += (Iter->x - MeanX) * (Iter->x - MeanX);
    SumY += (Iter->y - MeanY) * (Iter->y - MeanY);
    SumXY+= (Iter->x - MeanX) * (Iter->y - MeanY);
  }

  return Sqr(SumXY) / (SumX * SumY);
}
//---------------------------------------------------------------------------
/** Finds the correlation coefficient (R^2) as defined by CurveExpert
 *  For more information, see http://curvefit.com/goodness_of_fit.htm
 *  The closer the correlation is to 1 the nearer the points are to the line given by Func
 *  \param Points: Series of data points
 *  \param Func: Function to compare the points to
 */
double Correlation(const std::vector<TDblPoint> &Points, const TFunc &Func)
{
  long double yMean = 0;

  std::vector<TDblPoint>::const_iterator Begin = Points.begin();
  std::vector<TDblPoint>::const_iterator End = Points.end();
  for(std::vector<TDblPoint>::const_iterator Iter = Begin; Iter != End; ++Iter)
    yMean += Iter->y;
  yMean /= Points.size();

  long double St = 0;
  long double Sy = 0;
  for(std::vector<TDblPoint>::const_iterator Iter = Begin; Iter != End; ++Iter)
  {
    St += Sqr(yMean - Iter->y);
    Sy += Sqr(Iter->y - Func(Iter->x));
  }

  if(St == 0) //Prevent division by zero. St==0 means that the line is a perfect fit.
    return 1;
  return 1 - Sy/St;
}
//---------------------------------------------------------------------------
/** Find the missing constants in a function model to make the function a best fit of the points
 *  \param Points:  Points to find best fit for
 *  \param Func:    Function model. The first argument is the variable and the rest are the constants to find.
 *  \param Values:  Initial guess for the constants. On return Values contains the found constants.
 *                  Values.size() must be one less than the number of arguments for Func.
 *  \param MaxIter: Maximum number of iterations before giving up.
 *  \param Tol:     Max tolerance allowed for found constants
 */
void Regression(const std::vector<TDblPoint> &Points, const TCustomFunc &Func, std::vector<long double> &Values, unsigned MaxIter, double Tol)
{
  if(Values.size() != Func.GetArguments().size() - 1)
    throw EFuncError(ecInvalidArgCount);

  std::vector<long double> Args = Values;
  Args.insert(Args.begin(), 0); //Allocate space for the variable "x"
  try
  {
    double My = 0.01;

    const unsigned m = Points.size();
    const unsigned n = Values.size();

    if(m < n)
      throw EFuncError(ecTooFewPoints);

    DEBUG_LOG(std::wclog << "f(" << Func.GetArguments()[0] << L")=" << Func.MakeText() << std::endl);
    std::vector<TCustomFunc> PartialDif;
    for(unsigned J = 0; J < n; J++)
    {
      PartialDif.push_back(Func.MakeDif(J+1));
      DEBUG_LOG(std::wclog << "f'(" << Func.GetArguments()[J+1] << L")=" << PartialDif.back().MakeText() << std::endl);
    }

    DEBUG_LOG(std::wclog << "It " << 0 << L"   ");
    DEBUG_LOG(for(unsigned J = 1; J < Args.size(); J++) std::wclog << Func.GetArguments()[J] << " = " << Args[J] << "  ");
    DEBUG_LOG(std::wclog << std::endl);

    const TMatrix<double> I = IdentityMatrix<double>(n);
    double LastSum = CalcSSQ(Points, Func, Args);

    //We cannot continue with a guess that gives a huge SSQ
    if(LastSum == std::numeric_limits<double>::infinity())
      throw EFuncError(ecBadGuess);

    DEBUG_LOG(std::wclog << L"SquareError = " << LastSum << std::endl);

    std::vector<long double> LastArgs = Args;

    for(unsigned Step = 1; Step <= MaxIter; Step++)
    {
      std::vector<double> g(n);
      for(unsigned i = 0; i < n; i++)
        for(unsigned k = 0; k < m; k++)
        {
          Args[0] = Points[k].x;
          g[i] += PartialDif[i](Args) * (Func(Args) - Points[k].y);
        }

      TMatrix<double> G(n, n);
      for(unsigned i = 0; i < n; i++)
        for(unsigned j = 0; j < n; j++)
          for(unsigned k = 0; k < m; k++)
          {
            Args[0] = Points[k].x;
            G(i,j) += PartialDif[i](Args) * PartialDif[j](Args);
          }

      std::vector<double> d(n);
      for(unsigned i = 0; i < n; i++)
        d[i] = std::sqrt(G(i, i));

      TMatrix<double> D(n, n);
      for(unsigned i = 0; i < n; i++)
        D(i, i) = d[i];

      TMatrix<double> C2 = D.Inverse() * G * D.Inverse();
      TMatrix<double> C(n, n);
      for(unsigned i = 0; i < n; i++)
        for(unsigned j = 0; j < n; j++)
          C(i, j) = (G(i, j)/d[i])/d[j];

      TMatrix<double> s = ((-D.Inverse() * (C + I * My).Inverse()) * D.Inverse()) * g;
      Args.swap(LastArgs);

      DEBUG_LOG(std::clog << "It " << Step << "   ");
      for(unsigned J = 1; J < Args.size(); J++)
      {
        Args[J] = LastArgs[J] + s(J-1, 0);
        DEBUG_LOG(std::wclog << Func.GetArguments()[J] << " = " << Args[J] << "  ");
      }
      DEBUG_LOG(std::wclog << std::endl);

      double Sum = CalcSSQ(Points, Func, Args);

      DEBUG_LOG(std::wclog << L"SquareError = " << Sum << L"   LastError = " << LastSum << L"   My = " << My << std::endl);

      if(std::abs(Sum - LastSum) < Tol)
      {
        //Overwrite Values with the result
        std::copy(Args.begin() + 1, Args.end(), Values.begin());
        return;
      }

      if(Sum < LastSum)
      {
        My /= 10;
        LastSum = Sum;
      }
      else
      {
        My *= 10;
        Args.swap(LastArgs);
        if(My >= 1000000)
          throw EFuncError(ecBadGuess);
      }
    }
    throw EFuncError(ecBadGuess);
  }
  catch(EMatrix& E)
  {
    throw EFuncError(ecBadGuess);
  }
}
//---------------------------------------------------------------------------
} //namespace Func32






