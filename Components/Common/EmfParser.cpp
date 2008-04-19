/* Graph (http://sourceforge.net/projects/graph)
 * Copyright 2008 Ivan Johansen
 *
 * Graph is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 */
//---------------------------------------------------------------------------
#include "Config.h"
#pragma hdrstop
#include "EmfParser.h"
#include <cassert>
#include <vector>
//---------------------------------------------------------------------------
TEmfParser::TEmfParser()
{
}
//---------------------------------------------------------------------------
int CALLBACK TEmfParser::EnhMetaFileProc(

    HDC hDC,	// handle to device context
    HANDLETABLE FAR *lpHTable,	// pointer to metafile handle table
    const ENHMETARECORD FAR *lpEMFR,	// pointer to metafile record
    int nObj,	// count of objects
    LPARAM lpData 	// pointer to optional data
   )
{
  assert(lpData);
  TEmfParser *Object = reinterpret_cast<TEmfParser*>(lpData);
  Object->HandleRecord(lpEMFR);
  return true;
}
//---------------------------------------------------------------------------
void TEmfParser::HandleRecord(const ENHMETARECORD *lpEMFR)
{
  switch(lpEMFR->iType)
  {
    case EMR_HEADER: //Ignore
      break;

    case EMR_MOVETOEX:
    {
      EMRMOVETOEX* MoveTo = (EMRMOVETOEX*)lpEMFR;
      Pos = MoveTo->ptl;;
      break;
    }
    case EMR_LINETO:
    {
      EMRLINETO* LineTo = (EMRLINETO*)lpEMFR;
      Writer->Line(Pos.x, Pos.y, LineTo->ptl.x, LineTo->ptl.y);
      break;
    }

    case EMR_CREATEPEN:
    {
      EMRCREATEPEN* LogPen = (EMRCREATEPEN*)lpEMFR;
      Writer->SetPen(LogPen->lopn.lopnStyle, LogPen->lopn.lopnWidth.x, LogPen->lopn.lopnColor);
      break;
    }

    case EMR_POLYGON16:
    {
      EMRPOLYGON16 *Polygon = (EMRPOLYGON16*)lpEMFR;
      Writer->Polygon(Polygon->apts, Polygon->cpts);
      break;
    }

    case EMR_POLYLINE16:
    {
      EMRPOLYGON16 *Polyline = (EMRPOLYLINE16*)lpEMFR;
      Writer->Polyline(Polyline->apts, Polyline->cpts);
      break;
    }

    case EMR_EXTCREATEFONTINDIRECTW:
    {
      EMREXTCREATEFONTINDIRECTW *Font = (EMREXTCREATEFONTINDIRECTW*)lpEMFR;
      std::wstring Temp = Font->elfw.elfLogFont.lfFaceName;
      std::string FontName(Temp.begin(), Temp.end());
      Writer->SetFont(FontName.c_str(), -Font->elfw.elfLogFont.lfHeight, 0);
      break;
    }
    
    case EMR_EXTTEXTOUTW:
    {
      EMREXTTEXTOUTW *Text = (EMREXTTEXTOUTW*)lpEMFR;
      wchar_t *Str = (wchar_t*)(((char*)Text)+Text->emrtext.offString);
      int Size = Text->emrtext.nChars;
      int NewSize = WideCharToMultiByte(CP_ACP, 0, Str, Size, NULL, 0, NULL, NULL);
      std::vector<char> NewStr(NewSize + 1);
      WideCharToMultiByte(CP_ACP, 0, Str, Size, &NewStr[0], NewSize, NULL, NULL);
      NewStr.back() = 0; //Zero terminate
      Writer->Text(Text->rclBounds.left, Text->rclBounds.bottom, &NewStr[0], Text->rclBounds.bottom - Text->rclBounds.top);
      break;
    }

    case EMR_RECTANGLE:
    {
      EMRRECTANGLE *Rectangle = (EMRRECTANGLE*)lpEMFR;
      Writer->Rectangle(Rectangle->rclBox);
      break;
    }

    default:
      break;
  }
}
//---------------------------------------------------------------------------
void TEmfParser::Parse(HENHMETAFILE Handle, TGraphicWriter &AWriter)
{
  ENHMETAHEADER Header;
  assert(GetEnhMetaFileHeader(Handle, sizeof(Header), &Header));
//  ps_iniplot(const_cast<char*>(FileName), Header.rclFrame.right/1000.0);
//  CmPerPixel = Header.rclFrame.right/(1000.*Header.rclBounds.right);
//  ps_echel(CmPerPixel, Header.rclFrame.bottom/(1000.*Header.rclBounds.bottom), 0., Header.rclFrame.bottom/1000., 1., 0.);

  Writer = &AWriter;
  Writer->BeginFile(Header.rclBounds.right - Header.rclBounds.left - 1, Header.rclBounds.bottom - Header.rclBounds.top - 2);
  EnumEnhMetaFile(NULL, Handle, &EnhMetaFileProc, this, NULL);
  Writer->EndOfFile();
}
//---------------------------------------------------------------------------
void TEmfParser::Parse(const char *FileName, TGraphicWriter &AWriter)
{
  HENHMETAFILE Handle = GetEnhMetaFile(FileName);
  Parse(Handle, AWriter);
  DeleteEnhMetaFile(Handle);
}
//---------------------------------------------------------------------------

