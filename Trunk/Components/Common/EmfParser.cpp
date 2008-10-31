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
      Pos = MoveTo->ptl;
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
      EMRCREATEPEN *LogPen = (EMRCREATEPEN*)lpEMFR;
      TPenInfo Pen(LogPen->lopn.lopnStyle, LogPen->lopn.lopnWidth.x, SwapColor(LogPen->lopn.lopnColor));
      PenList[LogPen->ihPen] = Pen;
      Writer->SetPen(Pen);
      break;
    }

    case EMR_EXTCREATEPEN:
    {
      EMREXTCREATEPEN *ExtPen = (EMREXTCREATEPEN*)lpEMFR;
      TPenInfo Pen(ExtPen->elp.elpPenStyle, Pen.Width = ExtPen->elp.elpWidth, SwapColor(ExtPen->elp.elpColor));
      PenList[ExtPen->ihPen] = Pen;
      Writer->SetPen(Pen);
      break;
    }

    case EMR_CREATEBRUSHINDIRECT:
    {
      EMRCREATEBRUSHINDIRECT  *LogBrush = (EMRCREATEBRUSHINDIRECT*)lpEMFR;
      TBrushInfo Brush(LogBrush->lb.lbStyle, SwapColor(LogBrush->lb.lbColor), LogBrush->lb.lbHatch);
      BrushList[LogBrush->ihBrush] = Brush;
      Writer->SetBrush(Brush);
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
      EMREXTCREATEFONTINDIRECTW *ExtFont = (EMREXTCREATEFONTINDIRECTW*)lpEMFR;
      std::wstring Temp = ExtFont->elfw.elfLogFont.lfFaceName;
      Font.Name = std::string(Temp.begin(), Temp.end());
      int Scale = 25; //Magic number
      Font.Size = -ExtFont->elfw.elfLogFont.lfHeight / Scale;
      FontList[ExtFont->ihFont] = Font;
      break;
    }

    case EMR_SETTEXTCOLOR:
      Font.Color = reinterpret_cast<const EMRSETTEXTCOLOR*>(lpEMFR)->crColor;
      SwapColor(Font.Color);
      break;

    case EMR_EXTTEXTOUTW:
    {
      EMREXTTEXTOUTW *Text = (EMREXTTEXTOUTW*)lpEMFR;
      wchar_t *Str = (wchar_t*)(((char*)Text)+Text->emrtext.offString);
      int Size = Text->emrtext.nChars;
//      Font.Size = Text->rclBounds.bottom - Text->rclBounds.top;
      Writer->Text(Text->rclBounds.left, Text->rclBounds.bottom, std::wstring(Str, Size), Font);
      break;
    }

    case EMR_RECTANGLE:
      Writer->Rectangle(reinterpret_cast<const EMRRECTANGLE*>(lpEMFR)->rclBox);
      break;

    case EMR_ELLIPSE:
    {
      Writer->Ellipse(reinterpret_cast<const EMRELLIPSE*>(lpEMFR)->rclBox);
      break;
    }

    case EMR_SELECTOBJECT:
    {
      unsigned Index = reinterpret_cast<const EMRSELECTOBJECT*>(lpEMFR)->ihObject;
      if(Index & 0x80000000)
        //Object is a stock object. See MSN article Q142319
        switch(Index & 0x7FFFFFFF)
        {
          case BLACK_BRUSH:  Writer->SetBrush(TBrushInfo(BS_SOLID, 0x00FFFFFF, 0)); break;
          case DKGRAY_BRUSH: Writer->SetBrush(TBrushInfo(BS_SOLID, 0x00808080, 0)); break;
          case GRAY_BRUSH:   Writer->SetBrush(TBrushInfo(BS_SOLID, 0x00808080, 0)); break;
          case LTGRAY_BRUSH: Writer->SetBrush(TBrushInfo(BS_SOLID, 0x00C0C0C0, 0)); break;
          case NULL_BRUSH:   Writer->SetBrush(TBrushInfo(BS_NULL, 0, 0));           break;
          case WHITE_BRUSH:  Writer->SetBrush(TBrushInfo(BS_SOLID, 0x00000000, 0)); break;
          case BLACK_PEN:    Writer->SetPen(TPenInfo(PS_SOLID, 1, 0x00FFFFFF));     break;
          case NULL_PEN:     Writer->SetPen(TPenInfo(PS_NULL, 0, 0));               break;
          case WHITE_PEN:    Writer->SetPen(TPenInfo(PS_SOLID, 1, 0x00000000));     break;
        }
      else if(PenList.count(Index))
        Writer->SetPen(PenList[Index]);
      else if(BrushList.count(Index))
        Writer->SetBrush(BrushList[Index]);
      else if(FontList.count(Index))
        Font = FontList[Index];
      break;
    }

    case EMR_DELETEOBJECT:
    {
      unsigned Index = reinterpret_cast<const EMRDELETEOBJECT*>(lpEMFR)->ihObject;
      PenList.erase(Index);
      BrushList.erase(Index);
      FontList.erase(Index);
      break;
    }

    case EMR_EXCLUDECLIPRECT:
      Writer->ExcludeClipRect(reinterpret_cast<const EMREXCLUDECLIPRECT*>(lpEMFR)->rclClip);
      break;

    case EMR_FILLRGN:
    {
      const EMRFILLRGN *Region = reinterpret_cast<const EMRFILLRGN*>(lpEMFR);
      const RGNDATA *RegionData = reinterpret_cast<const RGNDATA*>(Region->RgnData);
      const RECT *Data = reinterpret_cast<const RECT*>(RegionData->Buffer);
      if(BrushList.count(Region->ihBrush))
      {
        Writer->SetBrush(BrushList[Region->ihBrush]);
        for(unsigned I = 0; I < RegionData->rdh.nCount; I++)
          Writer->Rectangle(reinterpret_cast<const RECTL&>(Data[I]));
      }
      break;
    }

    case EMR_PAINTRGN:
    {
      const EMRPAINTRGN *Region = reinterpret_cast<const EMRPAINTRGN*>(lpEMFR);
      const RGNDATA *RegionData = reinterpret_cast<const RGNDATA*>(Region->RgnData);
      const RECT *Data = reinterpret_cast<const RECT*>(RegionData->Buffer);
      for(unsigned I = 0; I < RegionData->rdh.nCount; I++)
        Writer->Rectangle(reinterpret_cast<const RECTL&>(Data[I]));
      break;
    }

    case EMR_GDICOMMENT: //Application specific data
      break;

    case EMR_SETMAPMODE:
    {
      const EMRSETMAPMODE *MapMode = reinterpret_cast<const EMRSETMAPMODE*>(lpEMFR);
//      assert(MapMode->iMode == MM_TEXT);
      break;
    }
    case EMR_SETVIEWPORTEXTEX:
    {
      const EMRSETVIEWPORTEXTEX *ViewPort = reinterpret_cast<const EMRSETVIEWPORTEXTEX*>(lpEMFR);
      ViewportSize = ViewPort->szlExtent;
      break;
    }
    case EMR_SETWINDOWEXTEX:
    {
      const EMRSETWINDOWEXTEX *Window = reinterpret_cast<const EMRSETWINDOWEXTEX*>(lpEMFR);
      WindowSize = Window->szlExtent;
      Writer->SetWindowMapping(WindowSize, ViewportSize, WindowOrg);
      break;
    }
    case EMR_SETTEXTALIGN:
    {
      const EMRSETTEXTALIGN *TextAlign = reinterpret_cast<const EMRSETTEXTALIGN*>(lpEMFR);
      break;
    }
    case EMR_SETWINDOWORGEX:
    {
      const EMRSETWINDOWORGEX *EmrWindowOrg = reinterpret_cast<const EMRSETWINDOWORGEX*>(lpEMFR);
      WindowOrg = EmrWindowOrg->ptlOrigin;
      break;
    }

    case EMR_SETBKMODE:
    case EMR_SETBKCOLOR:
    case EMR_SELECTPALETTE:
    case EMR_EXTSELECTCLIPRGN:
    case EMR_EOF:
      break;

    default:
    {
      int Command = lpEMFR->iType;
      break;
    }
  }
}
//---------------------------------------------------------------------------
void TEmfParser::Parse(HENHMETAFILE Handle, TGraphicWriter &AWriter)
{
  ENHMETAHEADER Header;
  assert(GetEnhMetaFileHeader(Handle, sizeof(Header), &Header));

  Writer = &AWriter;
  Writer->BeginFile(Header.rclBounds, Header.rclFrame.right - Header.rclFrame.left, Header.rclFrame.bottom - Header.rclFrame.top);

  EnumEnhMetaFile(NULL, Handle, &EnhMetaFileProc, this, NULL);
  Writer->EndOfFile();
}
//---------------------------------------------------------------------------
void TEmfParser::Parse(const char *FileName, TGraphicWriter &AWriter)
{
  HENHMETAFILE Handle = GetEnhMetaFileA(FileName);
  if(Handle == NULL)
    return;
  Parse(Handle, AWriter);
  DeleteEnhMetaFile(Handle);
}
//---------------------------------------------------------------------------
unsigned TEmfParser::SwapColor(unsigned Color)
{
  return ((Color & 0xFF) << 16) | (Color & 0xFF00) | ((Color & 0xFF0000) >> 16); // Swap Red and Blue
}
//---------------------------------------------------------------------------

