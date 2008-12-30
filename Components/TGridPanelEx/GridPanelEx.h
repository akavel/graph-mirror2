//---------------------------------------------------------------------------
#ifndef GridPanelExH
#define GridPanelExH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class PACKAGE TGridPanelEx : public TGridPanel
{
private:
  void ArrangeControlInCell(TControl *AControl, TRect CellRect, TAlignInfo AlignInfo);
  void AdjustCellRect(TRect &Rect);
  void ArrangeControls(TRect &Rect);

protected:
  void RecalcCellDimensions(const TRect &Rect);
  void __fastcall UpdateControlOriginalParentSize(TControl *AControl, TPoint &AOriginalParentSize);
  void __fastcall AlignControls(TControl *AControl, TRect &Rect);

public:
  __fastcall TGridPanelEx(TComponent* Owner);
__published:
};
//---------------------------------------------------------------------------
#endif
