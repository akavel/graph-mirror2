//---------------------------------------------------------------------------
#include "Graph.h"
#pragma hdrstop
#include "Unit22.h"
#include "PythonBind.h"
//---------------------------------------------------------------------------
#pragma link "IRichEdit"
#pragma resource "*.dfm"
TForm22 *Form22 = NULL;
//---------------------------------------------------------------------------
__fastcall TForm22::TForm22(TComponent* Owner)
  : TForm(Owner), LastIndex(0), FAllowChange(false), CacheIndex(0), TextCache(1)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm22::FormHide(TObject *Sender)
{
  Form1->Panel5->Height = Form1->StatusBar1->Height;
  Form1->Splitter2->Visible = false;  
}
//---------------------------------------------------------------------------
void TForm22::WriteText(const AnsiString &Str, TColor Color)
{
  FAllowChange = true;
  IRichEdit1->TextFormat.SetColor(Color);
  IRichEdit1->SelStart = LastIndex;
  IRichEdit1->SelText = Str;
  LastIndex = IRichEdit1->SelStart;
  IRichEdit1->GlobalTextFormat.SetProtected(true);
  IRichEdit1->SelLength = 1;
  IRichEdit1->TextFormat.SetProtected(false);
  IRichEdit1->SelLength = 0;
  FAllowChange = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm22::IRichEdit1ProtectChange(TObject *Sender,
      int StartPos, int EndPos, bool &AllowChange)
{
  AllowChange = FAllowChange;
}
//---------------------------------------------------------------------------
void __fastcall TForm22::IRichEdit1KeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if(!Shift.Empty())
    return;
    
  switch(Key)
  {
    case VK_HOME:
    {
      int Index = IRichEdit1->LineIndex(-1);
      AnsiString Str = IRichEdit1->GetText(Index, Index + 4);
      if(Str == ">>> ")
      {
        IRichEdit1->SelStart = Index + 4;
        Key = 0;
      }
      break;
    }

    case VK_UP:
      if(IRichEdit1->GetLine(-1) == IRichEdit1->LineCount() - 1)
      {
        if(CacheIndex > 0)
        {
          IRichEdit1->SelStart = LastIndex;
          IRichEdit1->SelLength = MAXINT;
          if(CacheIndex == (int)TextCache.size() -1)
            TextCache.back() = IRichEdit1->SelText;
          IRichEdit1->SelText = TextCache[--CacheIndex];
        }
        Key = 0;
      }
      break;

    case VK_DOWN:
      if(IRichEdit1->GetLine(-1) == IRichEdit1->LineCount() - 1)
      {
        if(CacheIndex < (int)TextCache.size() - 1)
        {
          IRichEdit1->SelStart = LastIndex;
          IRichEdit1->SelLength = MAXINT;
          IRichEdit1->SelText = TextCache[++CacheIndex];
        }
        Key = 0;  
      }
      break;

    case VK_RETURN:
    {
      AnsiString Str = IRichEdit1->GetText(LastIndex, MAXINT);
      IRichEdit1->SelStart = MAXINT;
      IRichEdit1->SelText = "\r";
      LastIndex = IRichEdit1->SelStart;
      Command += Str;
      if(ExecutePythonCommand(Command))
      {
        WriteText(">>> ");
        Command = "";
      }
      else
      {
        WriteText("... ");
        Command += "\n";
      }
      TextCache.back() = Str;
      CacheIndex = TextCache.size();
      TextCache.push_back(AnsiString());
      Key = 0;
      break;
    }

    case VK_ESCAPE:
      IRichEdit1->SelStart = LastIndex;
      IRichEdit1->SelLength = MaxInt;
      IRichEdit1->SelText = L"";
      break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TForm22::FormShow(TObject *Sender)
{
  IRichEdit1->SelStart = MAXINT;
}
//---------------------------------------------------------------------------

