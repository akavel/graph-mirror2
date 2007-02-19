/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2007 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#ifndef PngHelperH
#define PngHelperH
#include <ostream>
//---------------------------------------------------------------------------
bool SaveBitmapToPngStream(HBITMAP hBitmap, std::ostream &Stream);
HBITMAP CreateBitmapFromPNGMemory(const void *Data, unsigned Size);
#endif
