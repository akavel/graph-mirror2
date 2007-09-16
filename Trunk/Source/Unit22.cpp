//---------------------------------------------------------------------------
#include "Graph.h"
#pragma hdrstop
#include "Unit22.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TForm22 *Form22 = NULL;
//---------------------------------------------------------------------------
__fastcall TForm22::TForm22(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm22::FormHide(TObject *Sender)
{
  Form1->Panel5->Height = Form1->StatusBar1->Height;
  Form1->Splitter2->Visible = false;  
}
//---------------------------------------------------------------------------
