/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef ClipbardH
#define ClipbardH
//---------------------------------------------------------------------------
class TGraphClipboard
{
  unsigned ClipboardFormat;
  unsigned PngFormat;

  void SetClipboardData(unsigned Format, const void *Data, unsigned DataSize);
  bool GetClipboardData(unsigned Format, void *Data, unsigned DataSize);
  int GetClipboardDataSize(unsigned Format);

  public:
  TGraphClipboard();
  void Copy(const TBaseFuncType *Func);
  void Copy(const TPointSeries *Func);
  void Copy(const TTextLabel *Label);
  void Copy(const TRelation *Relation);
  void Paste(TData &Data);
  bool HasData();
  void CopyPngData(const std::string &Str);
};

extern TGraphClipboard GraphClipboard;

#endif
