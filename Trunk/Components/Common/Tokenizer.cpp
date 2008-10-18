/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Config.h"
#pragma hdrstop
#include <sstream>
#include "Tokenizer.h"
//---------------------------------------------------------------------------
void TTokenizer::Extract(std::string &S)
{
  std::string::const_iterator Start = Iter;
  std::string::const_iterator End = Str.end();

  if(Iter == End)
  {
    FFailed = true;
    return;
  }

  if(FQuote && *Iter == FQuote)
  {
    ++Start;
    while(++Iter != End && *Iter != FQuote);
    if(Iter == End)
    {
      FFailed = true;
      return;
    }
    S = std::string(Start, Iter++);
    NextDelimiter = FDelimiter;
    if(Iter != End && *(++Iter) != FQuote)
      FFailed = true;
    return;
  }

  while(++Iter != End)
  {
    if(*Iter == NextDelimiter)
    {
      S = std::string(Start, Iter++);
      NextDelimiter = FDelimiter;
      return;
    }
  }
  S = std::string(Start, End);
  NextDelimiter = FDelimiter;
}
//---------------------------------------------------------------------------
//Ignore the next Count tokens
void TTokenizer::Ignore(unsigned Count)
{
  std::string Temp;
  while(Count--)
    Extract(Temp);
}
//---------------------------------------------------------------------------

