unit Unit1;

//
//  This unit contains the Design Time DataModule for the EasyNSE.  It is created automaticlly
// when a new EasyNSE project is created.  Simply add your shell components from the EasyNSE
// tab in the toolbar and design as you would a normal form.  There are a few things to remember though.
//
//  1) If you change the names of the DataModule or the Factory Manager you must
//       reflect those changes in the function InitializeLibrary below manually.
//
//  2) Do not add more TEasyFactoryManager objects to the DataModule.
//
//  3) As Explorer (or other applications that use the shell interfaces) calls to obtain an
//     interface EasyNSE will create a new copy if the object that implements that particular
//     handler.
//
//  4) Be Careful with global resources.  There can be numerous objects for a handler created and active
//     at the same time that are in different threads.
//
//  5) Be Careful as the objects could also be in different processes so any global variables will
//     need to be reinitialized when the DLL is loaded in a new process.

interface

{$include ..\Source\Compilers.Inc}

uses
  Windows, SysUtils, Classes, Graphics, EasyNSETypes,
  EasyIDEComponents, EasyCommonObjects, EasyThumbnailHandler,
  Olectnrs, ActiveX, ComObj, Dialogs;

type
  TDataModule1= class(TDataModule)
    EasyFactoryManager1: TEasyFactoryManager;
    EasyThumbnailHandler1: TEasyThumbnailHandler;
    procedure EasyThumbnailHandler1Initialize(
      Sender: TEasyThumbnailHandler; var Initialized: Boolean);
    procedure EasyThumbnailHandler1ExtractImage(
      Sender: TEasyThumbnailHandler; var Bitmap: TBitmap;
      var Extracted: Boolean);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

procedure InitializeLibrary;
procedure FinalizeLibrary;
function RegisterLibrary: Boolean;
function UnRegisterLibrary: Boolean;

var
  DataModule1: TDataModule1;

implementation

{$R *.dfm}

function GetLongPathName(lpszLongPath: PChar; lpszShortPath: PChar; cchBuffer: DWORD): DWORD; stdcall; external kernel32 name 'GetLongPathNameA';
function ExtractLongPathName(const FileName: string): string;
var
  Buffer: array[0..MAX_PATH - 1] of Char;
begin
  SetString(Result, Buffer, GetLongPathName(PChar(FileName), Buffer, SizeOf(Buffer)));
end;

procedure WriteToLog(Str : string);
var
  f: TextFile;
  FileName : TFileName;
begin
  FileName := ChangeFileExt(ExtractLongPathName(GetModuleName(HInstance)), '.log');
  AssignFile(f, FileName);
  if FileExists(FileName) then
    Append(f)
  else
    Rewrite(f);
  Writeln(f, Str);
  Flush(f);  { ensures that the text was actually written to file }
  CloseFile(f);
end;

procedure LogException(E : Exception);
var
  Buffer: array[0..1023] of Char;
begin
  ExceptionErrorMessage(ExceptObject, ExceptAddr, Buffer, SizeOf(Buffer));
  WriteToLog(Buffer);
end;

function GetFileClass : string;
begin
  Result := GetRegStringValue('Software\Ivan\Thumbnails', 'Files', HKEY_CURRENT_USER);
  ShowMessage(Result);
end;

function RegisterLibrary: Boolean;
//
// Called when the DLL is registered as a COM Server with RegSvr32.exe
// Return:
//   Return false if the library should not be registered as a COM Server
//
begin
  Result := True;
end;

function UnRegisterLibrary: Boolean;
//
// Called when the DLL is begin un-registered as a COM Server with RegSvr32.exe
// Return:
//   Return false if the library should not be un-registered as a COM Server
//
begin
  Result := True;
end;


procedure InitializeLibrary;
//
// Called when the library is loaded into a processes memory space or when the
// library is loaded for any reason to create the objects to implement the shell
// handler or NSE.
//
// Notes:
//   Keep any code in this function to a minimum.  Do not do any thing that would
// cause other DLL's to be loaded within this function
//
    function FindFactoryManager(DataModule: TDataModule): TEasyFactoryManager;
    var
      i: Integer;
    begin
      Result := nil;
      if Assigned(DataModule) then
      begin
        for i := 0 to DataModule1.ComponentCount - 1 do
        begin
          if DataModule.Components[i] is TEasyFactoryManager then
            Result := DataModule.Components[i] as TEasyFactoryManager
        end
      end
    end;

var
  i: Integer;
begin
  // Create the DataModule if necessary
  if not Assigned(DataModule1) then
    DataModule1 := TDataModule1.Create(nil);

  // Assign the FactoryManager if necessary
  if not Assigned(FactoryManager) then
  begin
    FactoryManager := FindFactoryManager(DataModule1);
    for i := 0 to DataModule1.ComponentCount - 1 do
    begin
      if not( DataModule1.Components[i] is TEasyFactoryManager) then
        FactoryManager.ShellHandlerList.Add(DataModule1.Components[i])
    end
  end;

//  if DataModule1.EasyThumbnailHandler1.Registration is TEasyRegistration then
//    (DataModule1.EasyThumbnailHandler1.Registration as TEasyRegistration).FileClass := GetFileClass;
end;

procedure FinalizeLibrary;
//
// Called when the library is freed from a processes memory space or when the library
// is being released
//
// Notes:
//   Keep any code in this function to a minimum.  Do not do any thing that would
// cause other DLL's to be loaded within this function
//
begin
  FactoryManager := nil;
  FreeAndNil(DataModule1);
end;

procedure TDataModule1.EasyThumbnailHandler1Initialize(
  Sender: TEasyThumbnailHandler; var Initialized: Boolean);
begin
  Sender.Cache := True;  // Have Explorer cach the image
//  Sender.Async := False; // If the client wants to extract the image in a different thread don't allow it
  Sender.Async := True; //Allow etraction from another thread. Is this a good idea? Consequences?
  Sender.Refresh := True; //Show Refresh thumbnail in explorer context menu (Doesn't seem to work)
  Initialized := True;
end;


//Windows XP: Thumbnails are 96*96
//Windows 2000: Thumbnails are 120*120
//Windows Me: Thumbnails are 80*80; Webview seems to be 120*120 scaled down to 80*80 and up again to 120*120
procedure TDataModule1.EasyThumbnailHandler1ExtractImage(
  Sender: TEasyThumbnailHandler; var Bitmap: TBitmap;
  var Extracted: Boolean);

    function ExtractIt : Boolean;
    var
      OleObject: IOleObject;
      Storage : IStorage;
      R: TRect;
      Format : FORMATETC;
    begin
      try
{$IF Defined(_DEBUG)}
        WriteToLog('Extracting file ' + string(Sender.FileName) + '   Width=' + IntToStr(Sender.Width) + '   Height=' + IntToStr(Sender.Height));
{$IFEND}
        OleCheck(StgCreateDocfile(nil, STGM_READWRITE or STGM_SHARE_EXCLUSIVE or STGM_CREATE or STGM_DELETEONRELEASE, 0, Storage));
        Format.cfFormat := CF_BITMAP;
        Format.ptd := nil;
        Format.dwAspect := DVASPECT_CONTENT{DVASPECT_THUMBNAIL};
        Format.lindex := -1;
        Format.tymed := TYMED_GDI;
        OleCheck(OleCreateLinkToFile(PWideChar(WideString(Sender.FileName)), IOleObject,
          OLERENDER_FORMAT, @Format, nil, Storage, OleObject));
        R := Rect(0, 0, Sender.Width, Sender.Height);
        OleCheck(OleDraw(OleObject, DVASPECT_CONTENT{DVASPECT_THUMBNAIL}, Bitmap.Canvas.Handle, R));
        Result := True;
        //Warning: Do not attemp to free Storage and OleObject. Delphi will call _Release on interfaces when they go out of scope.
      except
        // Quiet failure
        on E: Exception do
          begin
            WriteToLog('Error handling file ' + string(Sender.FileName));
            LogException(E);
            Result := False;
          end;
      end;
    end;


begin
  if Sender.AspectRatio then
  begin
    // Extract the image and "squeeze" it into the Bitmap
    Extracted := ExtractIt;
  end else
  if Sender.OrgSize then
  begin
    // Extract the image with out changing its size but clip it to the size of
    // the Bitmap
    Extracted := ExtractIt;
  end else
  if Sender.Screen then
  begin
    // Extract the image so it will look best for the current screen size
    // (Best not to exceed the size of the Bitmap but can be smaller)
    Extracted := ExtractIt;
  end;
end;

end.



