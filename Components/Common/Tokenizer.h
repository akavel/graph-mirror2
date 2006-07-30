//---------------------------------------------------------------------------
#ifndef TokenizerH
#define TokenizerH
//---------------------------------------------------------------------------
#include <string>

struct Delimiter
{
  char FDelimiter;
  Delimiter(char ADelimiter) : FDelimiter(ADelimiter) {}
};

class TTokenizer
{
  std::string Str;
  char FDelimiter;
  char NextDelimiter;
  std::string::const_iterator Iter;
  bool FFailed;
  char FQuote;

  void Extract(std::string &S);

public:
  TTokenizer() : FDelimiter(','), Iter(Str.begin()), FFailed(true), NextDelimiter(','), FQuote(0) {}
  TTokenizer(const std::string &S, char Delimiter = ',', char Quote = 0)
    : Str(S), FDelimiter(Delimiter), Iter(Str.begin()), FFailed(false), NextDelimiter(Delimiter), FQuote(Quote) {}
  TTokenizer(const char *S, char Delimiter = ',', char Quote = 0)
    : Str(S), FDelimiter(Delimiter), Iter(Str.begin()), FFailed(false), NextDelimiter(Delimiter), FQuote(Quote) {}
  TTokenizer(const TTokenizer &A)
    : Str(A.Str), FDelimiter(A.FDelimiter), NextDelimiter(A.NextDelimiter), Iter(Str.begin()), FFailed(false), FQuote(A.FQuote) {}

  std::string Next() {std::string Temp; Extract(Temp); return Temp;}
  std::string Next(char Delimiter) {std::string Temp; NextDelimiter = Delimiter; Extract(Temp); return Temp;};
  void SetText(const std::string &S) {Str = S; Iter = Str.begin(); FFailed = false; NextDelimiter = FDelimiter;}
  void SetDelimiter(char Delimiter) {FDelimiter = Delimiter;}
  void Ignore(unsigned Length);
  bool Failed() const {return FFailed;}

  operator void*() const {return reinterpret_cast<void*>(!FFailed);}
  TTokenizer& operator>>(std::string &S) {Extract(S); return *this;}
  TTokenizer& operator>>(const Delimiter& ADelimiter) {NextDelimiter = ADelimiter.FDelimiter; return *this;}
  template<typename T> TTokenizer& operator>>(T &Var)
  {
    if(!FFailed)
    {
      std::istringstream Stream(Next());
      Stream >> Var;
      FFailed = !Stream;
    }
    return *this;
  }
  const TTokenizer& operator=(const TTokenizer &A)
  {
    Str = A.Str;
    FDelimiter = A.FDelimiter;
    NextDelimiter = A.NextDelimiter;
    Iter = Str.begin();
    FFailed = false;
    FQuote = A.FQuote;
    return *this;
  }

  const TTokenizer& operator=(const std::string &Str)
  {
    SetText(Str);
    return *this;
  }
};
#endif
