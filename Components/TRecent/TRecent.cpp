//===========================================================================
// Copyright © 2003 Ivan Johansen
// TRecent.cpp
//===========================================================================
#include <vcl.h>
#pragma hdrstop
#include "Config.h"
#include <Registry.hpp>
#include "TRecent.h"
#include <deque>
#include <vector>
#include <memory>
#include <StrUtils.hpp>
#include <Shlwapi.h>
#pragma package(smart_init)
#pragma link "Shlwapi.lib"

//Header that isn't included correct
#define SHARD_PATH     0x00000002L
SHSTDAPI_(void) SHAddToRecentDocs(UINT uFlags, LPCVOID pv);

struct TImpl
{
  std::deque<std::pair<String,TMenuItem*> > FileList; //We dont want deque in the interface; Size problem with debug version of STLport
};
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TRecent *)
{
	new TRecent(NULL);
}
//---------------------------------------------------------------------------
namespace Trecent
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TRecent)};
		RegisterComponents("IComp", classes, 0);
	}
}
//---------------------------------------------------------------------------
//Constructor initialzes values and allocates memory
__fastcall TRecent::TRecent(TComponent* Owner)
	: TComponent(Owner), FEnabled(true), Impl(new TImpl), FMaxFiles(4), FOnLoadFile(NULL),
    FFileMenu(NULL), FAddToRecentDocs(true), Seperator1(NULL), Seperator2(NULL), FMaxPathLen(50)
{
  ReadFromRegistry();//Read file names from registry
}
//---------------------------------------------------------------------------
//Destructor deallocates memory
__fastcall TRecent::~TRecent()
{
  RemoveMenuItems();//Remove shown menu items
  delete Impl;
}
//---------------------------------------------------------------------------
void __fastcall TRecent::SetMaxFiles(unsigned Value)
{
  RemoveMenuItems();//Remove shown menu items
  if(Value > MaxRecentFiles)
    FMaxFiles = MaxRecentFiles;
  else
    FMaxFiles = Value;
  while(Impl->FileList.size() > FMaxFiles)
  {
    delete Impl->FileList.back().second;
    Impl->FileList.pop_back(); //Make sure there are only MaxFiles file names in list
  }
  SaveToRegistry();
  ShowMenuItems();//Show menu items again
}
//---------------------------------------------------------------------------
void __fastcall TRecent::SetRegistryKey(const String &Value)
{
  RemoveMenuItems();//Remove shown menu items
  FRegistryKey = Value;//Change used registry key
  ReadFromRegistry();//Read data from new key in registry
  ShowMenuItems();//Show menu items
}
//---------------------------------------------------------------------------
void __fastcall TRecent::SetFileMenu(TMenuItem *Value)
{
  RemoveMenuItems();//Remove shown menu items
  FFileMenu = Value;
  ShowMenuItems();//Show menu items in a new place
}
//---------------------------------------------------------------------------
int TRecent::FileIndex(const String &FileName)
{
  for(unsigned I = 0; I < Impl->FileList.size(); I++)
    if(Impl->FileList[I].first.CompareIC(FileName) == 0)
      return I;
  return -1;
}
//---------------------------------------------------------------------------
int TRecent::ObjectIndex(TObject *Object)
{
  for(unsigned I = 0; I < Impl->FileList.size(); I++)
    if(Impl->FileList[I].second == Object)
      return I;
  return -1;
}
//---------------------------------------------------------------------------
//Called by user to indicate that file have been used
void __fastcall TRecent::FileUsed(const String &FileName)
{
  if(FMaxFiles)
  {
    RemoveMenuItems();//Remove shown menu items
    ReadFromRegistry();//Read data to get changes made by other version of program
    int Index = FileIndex(FileName);//Get index of FileName in list
    if(Index == -1)//If FileName not in list
    {
      if(Impl->FileList.size() == static_cast<unsigned>(FMaxFiles))//If list is full
        Impl->FileList.pop_back();//Erase last file name in list
    }
    else
      Impl->FileList.erase(Impl->FileList.begin() + Index);//Remove FileName from list

    Impl->FileList.push_front(std::pair<String, TMenuItem*>(FileName, NULL));//Add FileName to start of list
    SaveToRegistry();//Write new file list to registry
    ShowMenuItems();//Show new list of menu items
  }
  
  //Add Filename to recent documents list in the start menu
  if(FAddToRecentDocs)
    SHAddToRecentDocs(SHARD_PATH, FileName.c_str());
}
//---------------------------------------------------------------------------
//This function is called then one of the menu items is selected
void __fastcall TRecent::MenuClick(TObject *Sender)
{
  int Index = ObjectIndex(Sender);    //Get index of selected menu item in list
  String FileName = Impl->FileList[Index].first; //Get file name coresponding to menu item
  if(FOnLoadFile)                               //If event handler sat
    if(FOnLoadFile(this, FileName))             //Call user selected event handler with FileName as parameter
      FileUsed(FileName);                       //Move FileName to first position in file list
}
//---------------------------------------------------------------------------
static String CompactPath(const String &Path, unsigned MaxLen)
{
  std::vector<wchar_t> Str(MaxLen + 1);
  PathCompactPathEx(&Str[0], Path.c_str(), MaxLen+1, 0);
  return &Str[0];
}
//---------------------------------------------------------------------------
//This function shows file names in the menu
void __fastcall TRecent::ShowMenuItems(void)
{
  if(!FFileMenu || Impl->FileList.empty() || !Enabled)//If nowhere to show or nothing to show
    return;

  //If item, to place after, is not a seperator
  if(FFileMenu->Caption != '-')
  {
    Seperator1 = new TMenuItem(NULL);
    Seperator1->Caption = '-';//Make seperator
    //Add seperator after FFileMenu
    FFileMenu->Parent->Insert(FFileMenu->MenuIndex+1, Seperator1);
  }

  //If FFileMenu is not the last menu item
  if(FFileMenu->MenuIndex+1 + (Seperator1 != NULL) < FFileMenu->Parent->Count)
    //If the next item is not a seperator
    if(FFileMenu->Parent->Items[FFileMenu->MenuIndex+1+ (Seperator1!=NULL)]->Caption != '-')
    {
      //Add seperator after recent file list
      Seperator2 = new TMenuItem(NULL);
      Seperator2->Caption = '-';//Add seperator after the other one
      FFileMenu->Parent->Insert(FFileMenu->MenuIndex+1, Seperator2);
    }

  for(unsigned I = 0; I < Impl->FileList.size(); I++)
  {
    //Loop through all file names
    TMenuItem *MenuItem = new TMenuItem(NULL);//Make new menu item
    MenuItem->Caption = "&" + String(I+1) + " " + CompactPath(Impl->FileList[I].first, FMaxPathLen);
    //Call MenuClick() then menu item is pressed
    MenuItem->OnClick = &MenuClick;
    //Set hint and substitute %s with the filename
    MenuItem->Hint = AnsiReplaceStr(FHint, "%s", Impl->FileList[I].first);
    Impl->FileList[I].second = MenuItem;//Add pointer to menu item to FileList
    //Add menu item to menu
    FFileMenu->Parent->Insert(FileMenu->MenuIndex + I+1 + (Seperator1 != NULL), MenuItem);
  }
}
//---------------------------------------------------------------------------
//This function removes all menu items added by this component
void __fastcall TRecent::RemoveMenuItems()
{
  if(!FFileMenu || Impl->FileList.empty())//If no FileMenu or no file names to show
    return;

  if(!FFileMenu->Parent)//If parent to FFileMenu has been deleted
    return;

  if(Seperator1)
  {
    delete Seperator1;//Remove seperator before menu items
    Seperator1 = NULL;
  }

  for(unsigned I = 0; I < Impl->FileList.size(); I++)//Loop through each made menu item
  {
    delete Impl->FileList[I].second;//Remove menu item
    Impl->FileList[I].second = NULL;//File name no more have a menu item attached
  }

  if(Seperator2)
  {
    delete Seperator2;//Remove seperator after menu items
    Seperator2 = NULL;
  }
}
//---------------------------------------------------------------------------
//This function reads file names of recent used files from the registry
void __fastcall TRecent::ReadFromRegistry(void)
{
  if(ComponentState.Contains(csDesigning))//If design time
    return;//don't load registry

  Impl->FileList.clear();//Make sure there are no files in list
  if(FRegistryKey.IsEmpty())//If no registry key set
    return;//Don't read data from registry

  std::auto_ptr<TRegistry> Registry(new TRegistry());
  if(Registry->OpenKeyReadOnly(FRegistryKey))
    for(unsigned I = 1; I <= FMaxFiles; I++)//Loop through all values in registry
      if(Registry->ValueExists("Recent" + String(I)))
        Impl->FileList.push_back(std::pair<String,TMenuItem*>(Registry->ReadString("Recent" + String(I)), NULL));//Add file name to list
}
//---------------------------------------------------------------------------
//This function saves file names to the registry
void __fastcall TRecent::SaveToRegistry(void)
{
  if(ComponentState.Contains(csDesigning))//If design time
    return;//don't write to registry

  if(FRegistryKey.IsEmpty())//If no registry key choosen
    return;//Don't save data

  std::auto_ptr<TRegistry> Registry(new TRegistry);
  if(Registry->OpenKey(FRegistryKey, true))
  {
    //Open key;Create key if not found
    for(unsigned I = 0; I < Impl->FileList.size(); I++)//Loop through recent files in list
      Registry->WriteString("Recent" + String(I+1), Impl->FileList[I].first);//Write file name to registry

    for(unsigned I = Impl->FileList.size(); Registry->ValueExists("Recent" + String(I+1)); I++)//Loop through until no more fake values
      Registry->DeleteValue("Recent"+String(I+1));//Delete file name
  }
}
//---------------------------------------------------------------------------
void __fastcall TRecent::SetEnabled(bool AEnabled)
{
  if(FEnabled != AEnabled)
  {
    FEnabled = AEnabled;
    if(FEnabled)
      ShowMenuItems();
    else
      RemoveMenuItems();
  }
}
//---------------------------------------------------------------------------
//WARNING: Do not change the argument to a reference. It will crash the IDE
void __fastcall TRecent::SetHint(String Str)
{
  FHint = Str;
  for(unsigned I = 0; I < Impl->FileList.size(); I++)
    if(Impl->FileList[I].second)
      Impl->FileList[I].second->Hint = AnsiReplaceStr(FHint, "%s", Impl->FileList[I].first);
}
//---------------------------------------------------------------------------

