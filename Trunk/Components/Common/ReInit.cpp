//===========================================================================
// Copyright © 2004 Ivan Johansen
// VclObject.h
//===========================================================================
#include <vcl.h>
#pragma hdrstop
#include <memory>
#include "ReInit.h"
//---------------------------------------------------------------------------
class TAsInheritedReader : public TReader
{
public:
  void __fastcall ReadPrefix(TFilerFlags &Flags,int &AChildPos);
  __fastcall TAsInheritedReader(TStream* Stream, int BufSize):TReader(Stream,BufSize){};
};
//---------------------------------------------------------------------------
void __fastcall TAsInheritedReader::ReadPrefix(TFilerFlags &Flags,int &AChildPos)
{
  TReader::ReadPrefix(Flags, AChildPos);
  Flags << ffInherited;
}
//---------------------------------------------------------------------------
HINSTANCE SetResourceHInstance(HINSTANCE NewInstance, bool FreeOldInstance)
{
  //The TLibModule in sysclass.h is incorrect
  struct PACKAGE TLibModule
  {
    TLibModule  *next;
    long        instance;
    long        codeinstance;
    long        datainstance;
    long        resinstance;
    long        reserved;
  };

  TLibModule *CurModule = reinterpret_cast<TLibModule*>(LibModuleList);
  while(CurModule != NULL)
  {
    if((HINSTANCE)CurModule->instance == HInstance)
    {
      if(CurModule->resinstance != CurModule->instance && FreeOldInstance)
        FreeLibrary((HINSTANCE)CurModule->resinstance);
      CurModule->resinstance = (long)NewInstance;
      return NewInstance;
    }
    CurModule = CurModule->next;
  }
  return NULL;
}
//---------------------------------------------------------------------------
HINSTANCE LoadNewResourceModule(const AnsiString &Ext)
{
  if(Ext.AnsiCompareIC("exe") == 0)
    SetResourceHInstance(MainInstance);

  AnsiString FileName = ChangeFileExt(Application->ExeName, AnsiString('.') + Ext);
  HINSTANCE NewInst = LoadLibraryEx(FileName.c_str(), 0, LOAD_LIBRARY_AS_DATAFILE);
  if(!NewInst)
    RaiseLastOSError();
  return SetResourceHInstance(NewInst);
}
//---------------------------------------------------------------------------
HINSTANCE LoadNewResourceModule(LCID Locale)
{
  char LocaleName[4];
  GetLocaleInfo(Locale, LOCALE_SABBREVLANGNAME, LocaleName, sizeof(LocaleName));
  HINSTANCE NewInst = NULL;
  if(LocaleName[0])
  {
    // Then look for a potential language/country translation
    NewInst = LoadNewResourceModule(LocaleName);
    if(NewInst == NULL)
    {
      // Finally look for a language only translation
      LocaleName[2] = 0;
      NewInst = LoadNewResourceModule(LocaleName);
    }
  }
  return NewInst;
}
//---------------------------------------------------------------------------
bool InternalReloadComponentRes(AnsiString ResName, HINSTANCE HInst, TComponent *&Instance)
{
  //avoid possible EResNotFound exception
  if(HInst == 0)
    HInst = HInstance;
  HINSTANCE HRsrc = FindResource(HInst, ResName.c_str(), RT_RCDATA);
  if(HRsrc == 0)
    return false;

  std::auto_ptr<TResourceStream> ResStream(new TResourceStream((int)HInst, ResName, RT_RCDATA));
  std::auto_ptr<TAsInheritedReader> AsInheritedReader(new TAsInheritedReader(ResStream.get(), 4096));
  Instance = AsInheritedReader->ReadRootComponent(Instance);
  return true;
}
//---------------------------------------------------------------------------
bool InitComponent(TComponent *Instance, TClass ClassType)
{
  if(ClassType == __classid(TComponent) || ClassType==__classid(TObject))
    return false;
  bool Result = InitComponent(Instance, TObject::ClassParent(ClassType));
  if(InternalReloadComponentRes(ClassType->ClassName(), (HINSTANCE)FindResourceHInstance(FindClassHInstance(ClassType)), Instance))
    return true;
  return Result;
}
//---------------------------------------------------------------------------
bool ReloadInheritedComponent(TComponent *Instance)
{
  return InitComponent(Instance, Instance->ClassType());
}
//---------------------------------------------------------------------------
void ReinitializeForms()
{
  for(int I = 0; I < Screen->FormCount; I++)
    ReloadInheritedComponent(Screen->Forms[I]);
}
//---------------------------------------------------------------------------
TComponent* ReloadComponent(TForm *Form, TComponent *Comp)
{
  std::auto_ptr<TResourceStream> ResStream(new TResourceStream(reinterpret_cast<int>(HInstance), Form->ClassName(), RT_RCDATA));
  std::auto_ptr<TMemoryStream> MemStream(new TMemoryStream);
  std::auto_ptr<TMemoryStream> BinStream(new TMemoryStream);
  std::auto_ptr<TStringList> TempList(new TStringList);

  ObjectBinaryToText(ResStream.get(), MemStream.get());

  MemStream->Position = 0;
  TempList->LoadFromStream(MemStream.get());
  AnsiString SearchString = "object " + Comp->Name + AnsiString(": ") + Comp->ClassName();
  while(TempList->Count && TempList->Strings[0].Pos(SearchString) == 0)
    TempList->Delete(0);

  if(!TempList->Count)
    throw EResNotFound("Component '" + Comp->Name + "' not found in resource '" + Form->ClassName() + "'");
  MemStream->Clear();
  TempList->SaveToStream(MemStream.get());
  MemStream->Position = 0;
  ObjectTextToBinary(MemStream.get(), BinStream.get());

  BinStream->Position = 0;
  std::auto_ptr<TReader> Reader(new TReader(BinStream.get(), 1024));
  Reader->Root = Form;
  Reader->Parent = Form;
  Reader->Owner = Form;

  Reader->ReadSignature();
  Reader->BeginReferences();

  //Old component must be deleted before the new one is created
  delete Comp;
  TComponent *NewComp = Reader->ReadComponent(NULL);

  Reader->FixupReferences();
  Reader->EndReferences();
  return NewComp;
}
//---------------------------------------------------------------------------

