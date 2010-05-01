//---------------------------------------------------------------------------
#include "Config.h"
#include <vcl.h>
#pragma hdrstop
#include "HandleCSV.h"
#include <algorithm>
#include <sstream>
#include <fstream>
#include "ICompCommon.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//Returns separator used for line ('\t', ';', ' ', ',')
//Check for separator in priority order. A space cannot be a separator if comes
//after a comma.
char GetSeparator(const std::string &Str)
{
  const char *Separators = "\t; ,";
  for(const char* Ch = Separators; *Ch; Ch++)
  {
    unsigned n = Str.find(*Ch);
    if(n != std::string::npos && n != 0 && Str[n-1] != ',')
      return *Ch;
  }
  return 0;
}
//---------------------------------------------------------------------------
unsigned CountCols(const std::string &Str, char Separator)
{
  unsigned Count = 1;
  unsigned Pos = 0;
  unsigned NextPos;
  while((NextPos = Str.find(Separator, Pos)) != std::string::npos)
  {
    if(NextPos != Pos)
      Count++;
    Pos = NextPos + 1;
  }
  return Count;
}
//---------------------------------------------------------------------------
bool ImportCsv(std::istream &Stream, TCsvGrid &CsvGrid)
{
  std::string Line;
  std::getline(Stream, Line);
  char Separator = GetSeparator(Line);
  unsigned ColCount = CountCols(Line, Separator);
  unsigned LineNo = 1;

  do
  {
    if(!Line.empty() && Line[Line.size()-1] == '\r')
      Line.erase(Line.size()-1);
    //Ignore empty lines
    if(Line.empty())
      continue;

    //Several separators after each other (eg. spaces) are ignored
    unsigned FirstPos = Line.find_first_not_of(Separator);
    unsigned Pos = Line.find(Separator, FirstPos);
    std::string xText = Trim(Line.substr(FirstPos, Pos - FirstPos));
    unsigned Col = 1;
    TCsvRow CsvRow(ColCount);
    CsvRow[0] = xText;
    for(unsigned LastPos = Line.find_first_not_of(Separator, Pos);
        Pos != std::string::npos; LastPos = Pos + 1)
    {
      Pos = Line.find(Separator, LastPos);

      //Ignore empty entries
      if(Pos == LastPos || LastPos == Line.size())
        continue;

      std::string yText = Trim(Line.substr(LastPos, Pos - LastPos));
      //Check if there are too many numbers on the line
      if(Col < ColCount)
        CsvRow[Col] = yText;
      Col++;
    }

    LineNo++;
    CsvGrid.push_back(CsvRow);
  }
  while(std::getline(Stream, Line));
  return true;
}
//---------------------------------------------------------------------------
bool ImportCsv(const std::string &Str, TCsvGrid &CsvGrid)
{
  std::istringstream Stream(Str);
  return ImportCsv(Stream, CsvGrid);
}
//---------------------------------------------------------------------------
bool ImportCsvFromFile(const std::wstring &FileName, TCsvGrid &CsvGrid)
{
  std::ifstream Stream(FileName.c_str());
  if(!Stream)
    return false;
  return ImportCsv(Stream, CsvGrid);
}
//---------------------------------------------------------------------------

