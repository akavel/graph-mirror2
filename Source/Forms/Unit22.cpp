//---------------------------------------------------------------------------
#include "Graph.h"
#pragma hdrstop
#include "Unit1.h"
#include "Unit22.h"
#include "PyGraph.h"
#include <clipbrd.hpp>
//---------------------------------------------------------------------------
#pragma link "IRichEdit"
#pragma resource "*.dfm"
TForm22 *Form22 = NULL;
//---------------------------------------------------------------------------
__fastcall TForm22::TForm22(TComponent* Owner)
  : TForm(Owner), LastIndex(0), FAllowChange(false), CacheIndex(0), TextCache(1), PromptIndex(0),
    IndentLevel(0)
{
  WritePrompt();
  Canvas->Font->Assign(IRichEdit1->Font);
  int CharWidth = (Canvas->TextWidth("A") * 15)/20;
  IRichEdit1->Paragraph->TabCount = 22;
  int x = CharWidth * 4;
  for(int I = 0; I < IRichEdit1->Paragraph->TabCount; I++, x+=CharWidth*2)
    IRichEdit1->Paragraph->Tab[I] = x;
}
//---------------------------------------------------------------------------
void __fastcall TForm22::FormHide(TObject *Sender)
{
  Form1->Panel6->Height = 0;
  Form1->Splitter2->Visible = false;
}
//---------------------------------------------------------------------------
void TForm22::WriteText(const String &Str, TColor Color)
{
  FAllowChange = true;
  int OldSelStart = IRichEdit1->SelStart;
  IRichEdit1->SelStart = PromptIndex;
  IRichEdit1->TextFormat.SetColor(Color);
  IRichEdit1->SelText = Str;
  int IndexChange = IRichEdit1->SelStart - PromptIndex;
  LastIndex += IndexChange;
  PromptIndex = IRichEdit1->SelStart;
  IRichEdit1->SelStart = OldSelStart + IndexChange;
  IRichEdit1->SelLength = 0;
  FAllowChange = false;
  SendMessage(IRichEdit1->Handle, WM_VSCROLL, SB_BOTTOM, 0);
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
  //Catch Paste (Ctrl+V or Shift+Insert)
  if((Shift == (TShiftState() << ssCtrl) && Key == 'V') ||
     (Shift == (TShiftState() << ssShift) && Key == VK_INSERT))
  {
    IRichEdit1->SelText = Clipboard()->AsText;
    Key = 0;
    return;
  }

  if(!Shift.Empty())
    return;
    
  switch(Key)
  {
    case VK_HOME:
    {
      int Index = IRichEdit1->LineIndex(-1);
      String Str = IRichEdit1->GetText(Index, Index + 4);
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
      String Str = IRichEdit1->GetText(LastIndex, MAXINT);
      Str = Str.TrimRight();
      IRichEdit1->SelStart = MAXINT;
      IRichEdit1->SelText = "\r";
      PromptIndex = IRichEdit1->SelStart;
      Command += Str;

      if(Python::ExecutePythonCommand(Command))
      {
        WritePrompt();
        Command = "";
        IndentLevel = 0;
      }
      else
      {
        WritePrompt("... ");
        Command += "\n";
        if(!Str.IsEmpty() && Str[Str.Length()] == ':')
          IndentLevel++;
      }

      if(!Str.IsEmpty())
      {
        TextCache.back() = Str;
        TextCache.push_back(String());
      }
      CacheIndex = TextCache.size() - 1;
      IRichEdit1->SelText = String::StringOfChar('\t', IndentLevel);
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
void TForm22::WritePrompt(const String &Str)
{
  FAllowChange = true;
  IRichEdit1->TextFormat.SetColor(clBlack);
  IRichEdit1->SelStart = MAXINT;
  PromptIndex = IRichEdit1->SelStart;
  IRichEdit1->SelText = Str;
  LastIndex = IRichEdit1->SelStart;
  IRichEdit1->GlobalTextFormat.SetProtected(true);
  IRichEdit1->SelLength = 1;
  IRichEdit1->TextFormat.SetProtected(false);
  IRichEdit1->SelLength = 0;
  FAllowChange = false;
  SendMessage(IRichEdit1->Handle, WM_VSCROLL, SB_BOTTOM, 0);
}
//---------------------------------------------------------------------------
void __fastcall TForm22::Clear1Click(TObject *Sender)
{
  FAllowChange = true;
  IRichEdit1->Clear();
  FAllowChange = false;
  LastIndex = 0;
  PromptIndex = 0;
  WritePrompt();
}
//---------------------------------------------------------------------------



