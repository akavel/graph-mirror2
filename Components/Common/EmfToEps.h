/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef EmfToEpsH
#define EmfToEpsH
//---------------------------------------------------------------------------
class TEmfToEps
{
  HENHMETAFILE Handle;
  double CmPerPixel;
  static int CALLBACK EnhMetaFileProc(HDC hDC, HANDLETABLE *lpHTable, const ENHMETARECORD *lpEMFR, int nObj, LPARAM lpData);

public:
  TEmfToEps(HENHMETAFILE AHandle);
  void SaveToFile(const char *FileName);
};
#endif
