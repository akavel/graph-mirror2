//---------------------------------------------------------------------------
#ifndef HandleCSVH
#define HandleCSVH
#include <string>
#include <vector>
#include <iostream>
#include <exception>
//---------------------------------------------------------------------------
typedef std::vector<std::string> TCsvRow;
typedef std::vector<TCsvRow> TCsvGrid;
char GetSeparator(const std::string &Str2);
bool ImportCsv(std::istream &Stream, TCsvGrid &CsvGrid, char Separator=0);
bool ImportCsv(const std::string &Str, TCsvGrid &CsvGrid, char Separator=0);
bool ImportCsvFromFile(const std::wstring &FileName, TCsvGrid &CsvGrid, char Separator=0);
#endif
