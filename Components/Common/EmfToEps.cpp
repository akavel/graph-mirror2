/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2006 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Config.h"
#include <vcl.h>
#pragma hdrstop
#include "EmfToEps.h"
#include <cassert>
#include <vector>
#include "plot_ps.h"
//---------------------------------------------------------------------------
TEmfToEps::TEmfToEps(HENHMETAFILE AHandle)
  : Handle(AHandle)
{
}
//---------------------------------------------------------------------------
int CALLBACK TEmfToEps::EnhMetaFileProc(

    HDC hDC,	// handle to device context
    HANDLETABLE FAR *lpHTable,	// pointer to metafile handle table
    const ENHMETARECORD FAR *lpEMFR,	// pointer to metafile record
    int nObj,	// count of objects
    LPARAM lpData 	// pointer to optional data
   )
{
  assert(lpData);
  TEmfToEps *Object = reinterpret_cast<TEmfToEps*>(lpData);

  switch(lpEMFR->iType)
  {
    case EMR_HEADER: //Ignore
      break;

    case EMR_MOVETOEX:
    {
      EMRMOVETOEX* MoveTo = (EMRMOVETOEX*)lpEMFR;
      ps_movea(MoveTo->ptl.x, -MoveTo->ptl.y);
      break;
    }
    case EMR_LINETO:
    {
      EMRLINETO* LineTo = (EMRLINETO*)lpEMFR;
      ps_drawa(LineTo->ptl.x, -LineTo->ptl.y);
      break;
    }

    case EMR_CREATEPEN:
    {
      EMRCREATEPEN* LogPen = (EMRCREATEPEN*)lpEMFR;
      ps_set_rgb_color(GetRValue(LogPen->lopn.lopnColor)/255.0, GetGValue(LogPen->lopn.lopnColor)/255.0, GetBValue(LogPen->lopn.lopnColor)/255.0);
      ps_line_width(LogPen->lopn.lopnWidth.x);
      break;
    }

    case EMR_POLYGON16:
    {
      EMRPOLYGON16 *Polygon = (EMRPOLYGON16*)lpEMFR;
      std::vector<float> x;
      std::vector<float> y;
      for(unsigned I = 0; I < Polygon->cpts; I++)
      {
        x.push_back(Polygon->apts[I].x);
        y.push_back(-Polygon->apts[I].y);
      }
      ps_closed_curve(&x.front(), &y.front(), Polygon->cpts, 1, 1, 1);
      break;
    }

    case EMR_POLYLINE:
      break;

    case EMR_EXTCREATEFONTINDIRECTW:
    {
      EMREXTCREATEFONTINDIRECTW *Font = (EMREXTCREATEFONTINDIRECTW*)lpEMFR;
      ps_font("Times-Roman", -Font->elfw.elfLogFont.lfHeight * Object->CmPerPixel);
      break;
    }
    case EMR_EXTTEXTOUTW:
    {
      EMREXTTEXTOUTW *Text = (EMREXTTEXTOUTW*)lpEMFR;
      WideString Str((wchar_t*)(((char*)Text)+Text->emrtext.offString), Text->emrtext.nChars);
//      ps_font("Times-Roman", 1);
      ps_chars(Text->rclBounds.left, -Text->rclBounds.top, PL_AH, AnsiString(Str).c_str(), hl_crte, 0);
      break;
    }

    default:
      break;
  }
  return true;
}
//---------------------------------------------------------------------------
void TEmfToEps::SaveToFile(const char *FileName)
{
  ENHMETAHEADER Header;
  assert(GetEnhMetaFileHeader(Handle, sizeof(Header), &Header));
  ps_iniplot(const_cast<char*>(FileName), Header.rclFrame.right/1000.0);
  CmPerPixel = Header.rclFrame.right/(1000.*Header.rclBounds.right);
  ps_echel(CmPerPixel, Header.rclFrame.bottom/(1000.*Header.rclBounds.bottom), 0., Header.rclFrame.bottom/1000., 1., 0.);

  EnumEnhMetaFile(NULL, Handle, &EnhMetaFileProc, this, NULL);

  ps_endplot(const_cast<char*>(FileName),'f');  /* close, spoole and delete the file */
}
//---------------------------------------------------------------------------

