//---------------------------------------------------------------------------
#ifndef ScaledImageListH
#define ScaledImageListH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Classes.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class PACKAGE TScaledImageList : public TCustomImageList
{
private:
  ULONG_PTR Token;
  TCustomImageList *FImages;
  int FWidth;
  int FHeight;

  void RecreateImageList();
  void __fastcall SetWidth(int Value);
  void __fastcall SetHeight(int Value);
  void __fastcall SetImages(TCustomImageList *Value);

protected:

public:
  __fastcall TScaledImageList(TComponent* Owner);
  __fastcall ~TScaledImageList();
  void SetSize(int W, int H);

__published:
  __property TCustomImageList *Images = {read=FImages, write=SetImages};
  __property int Width = {read=FWidth, write=SetWidth};
  __property int Height = {read=FHeight, write=SetHeight};
};
//---------------------------------------------------------------------------
#endif
