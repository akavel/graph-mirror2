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
bool ImportCsv(std::istream &Stream, TCsvGrid &CsvGrid);
bool ImportCsv(const std::string &Str, TCsvGrid &CsvGrid);
bool ImportCsvFromFile(const std::wstring &FileName, TCsvGrid &CsvGrid);
#endif
