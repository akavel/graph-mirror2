unit ThumbnailsImpl;

interface
uses
  System.Win.ComObj, Winapi.ActiveX, Thumbnails_TLB,
  Winapi.ShlObj, Winapi.Windows, Vcl.Graphics, StdVcl;

type
  IThumbnailProvider = interface(IUnknown)
    ['{e357fccd-a995-4576-b01f-234630154e96}']
    function GetThumbnail(cx : uint; out hBitmap : HBITMAP; out bitmapType : dword):HRESULT;stdcall;
  end;


  IInitializeWithFile = interface(IUnknown)
    ['{b7d14566-0509-4cce-a71f-0a554233bd9b}']
    function Initialize(pszFilePath: LPWSTR; grfMode: DWORD):HRESULT;stdcall;
  end;

  TGraphThumbnails = class(TAutoObject, IGraphThumbnails, IPersistFile, IExtractImage{, IThumbnailProvider, IInitializeWithFile})
  private
    FileName : string;
    Size : TSize;
    PixelFormat : TPixelFormat;
  protected
    function QueryInterface(const IID: TGUID; out Obj): HResult; stdcall;
    function _AddRef: Integer; stdcall;
    function _Release: Integer; stdcall;

    { IPersistFile Methods }
    function GetClassID(out classID: TCLSID): HResult; stdcall;
    function GetCurFile(out pszFileName: POleStr): HResult; stdcall;
    function IsDirty: HResult; stdcall;
    function Load(pszFileName: POleStr; dwMode: Integer): HResult; stdcall;
    function Save(pszFileName: POleStr; fRemember: BOOL): HResult; stdcall;
    function SaveCompleted(pszFileName: POleStr): HResult; stdcall;

    { IExtractImage Methods }
    function GetLocation(pszPathBuffer: LPWSTR; cch: DWORD; var pdwPriority: DWORD;
      var prgSize: TSize; dwRecClrDepth: DWORD;
      var pdwFlags: DWORD): HRESULT; stdcall;
    function Extract(var phBmpThumbnail: HBITMAP): HRESULT; stdcall;

    { IInitializeWithFile Methods }
//    function IInitializeWithFile.Initialize = FileInitialize;
    function FileInitialize(pszFilePath: LPWSTR; grfMode: DWORD):HRESULT;stdcall;

    { IThumbnailProvider MMethods}
    function GetThumbnail(cx : uint; out hBitmap : HBITMAP; out bitmapType : dword):HRESULT;stdcall;

  public
{$IF Defined(DEBUG)}
    procedure AfterConstruction; override; {For logging only}
    destructor Destroy; override;          {For logging only}
{$IFEND}
  end;

  { the new class factory }
  TGraphThumbnailsObjectFactory = class(TAutoObjectFactory)
  public
    procedure UpdateRegistry(Register: Boolean); override;
    destructor Destroy; override;          {For logging only}
  protected
    function QueryInterface(const IID: TGUID; out Obj): HResult; stdcall;
    function _AddRef: Integer; stdcall;
    function _Release: Integer; stdcall;
  end;

//function DllCanUnloadNow: HResult; stdcall;
//{$NODEFINE DllCanUnloadNow}

implementation
uses
  System.SysUtils, System.Win.ComServ, System.Classes;

function ExtractLongPathName(const FileName: string): string;
var
  Buffer: array[0..MAX_PATH - 1] of Char;
begin
  SetString(Result, Buffer, GetLongPathName(PChar(FileName), Buffer, SizeOf(Buffer)));
end;

procedure WriteToLog(Str : string);
{$IF Defined(DEBUG)}
var
  f: TextFile;
  FileName : TFileName;
  I : Integer;
begin
  for I := 1 to 10 do
    try
      FileName := ChangeFileExt(ExtractLongPathName(GetModuleName(HInstance)), '.log');
      AssignFile(f, FileName);
      if FileExists(FileName) then
        Append(f)
      else
        Rewrite(f);
      Writeln(f, Str);
      Flush(f);  { ensures that the text was actually written to file }
      CloseFile(f);
      break;
    except
      Sleep(100);
    end;
end;
{$ELSE}
begin
end;
{$IFEND}

procedure LogException(E : Exception);
var
  Buffer: array[0..1023] of Char;
begin
  ExceptionErrorMessage(ExceptObject, ExceptAddr, Buffer, SizeOf(Buffer));
  WriteToLog(Buffer);
end;

  { TGraphThumbnails }

{$IF Defined(DEBUG)}
procedure TGraphThumbnails.AfterConstruction;
begin
  WriteToLog(Format('AfterConstruction(%.8X)', [DWORD(self)]));
  inherited AfterConstruction;
end;

destructor TGraphThumbnails.Destroy;
begin
  inherited Destroy;
  WriteToLog(Format('Destroy(%.8X)', [DWORD(self)]));
end;
{$IFEND}

function TGraphThumbnails.QueryInterface(const IID: TGUID; out Obj): HResult;
begin
  Result := inherited QueryInterface(IID, Obj);
//  WriteToLog(Format('QueryInterface(%.8X, IID=%s, Result=%.8X)', [DWORD(self), GUIDToString(IID), Result]));
end;

function TGraphThumbnails._AddRef: Integer;
begin
  Result := inherited _AddRef;
//  WriteToLog(Format('AddRef(%.8X):%d', [DWORD(self), Result]));
end;

function TGraphThumbnails._Release: Integer;
begin
  Result := inherited _Release;
//  WriteToLog(Format('Release(%.8X):%d', [DWORD(self), Result]));
end;

function TGraphThumbnails.GetClassID(out classID: TCLSID): HResult;
begin
  WriteToLog(Format('GetClassID(%.8X)', [DWORD(self)]));
  classID := CLASS_GraphThumbnails;
  Result := S_OK
end;

function TGraphThumbnails.GetCurFile(out pszFileName: POleStr): HResult;
begin
  WriteToLog(Format('GetCurFile(%.8X)', [DWORD(self)]));
  Result := E_NOTIMPL
end;

function TGraphThumbnails.IsDirty: HResult;
begin
  WriteToLog(Format('IsDirty(%.8X)', [DWORD(self)]));
  Result := E_NOTIMPL
end;

function TGraphThumbnails.Load(pszFileName: POleStr; dwMode: Integer): HResult;
begin
  WriteToLog(Format('Load(%.8X, fileName="%s", Mode=%.8X)', [DWORD(self), pszFileName, dwMode]));
  FileName := pszFileName;
  Result := S_OK
end;

function TGraphThumbnails.Save(pszFileName: POleStr; fRemember: BOOL): HResult;
begin
  WriteToLog(Format('Save(%.8X)', [DWORD(self)]));
  Result := E_NOTIMPL
end;

function TGraphThumbnails.SaveCompleted(pszFileName: POleStr): HResult;
begin
  WriteToLog(Format('SaveCompleted(%.8X)', [DWORD(self)]));
  Result := E_NOTIMPL
end;


function TGraphThumbnails.GetLocation(pszPathBuffer: LPWSTR; cch: DWORD; var pdwPriority: DWORD;
      var prgSize: TSize; dwRecClrDepth: DWORD;
      var pdwFlags: DWORD): HRESULT;
begin
  WriteToLog(Format('GetLocation(%.8X, prgSize=%dx%d, dwRecClrDepth=%d, pdwFlags=0x%.8X)', [DWORD(self), prgSize.cx, prgSize.cy, dwRecClrDepth, pdwFlags]));
  StrLCopy(pszPathBuffer, PWideChar(FileName), cch-1);
  pdwPriority := IEIT_PRIORITY_NORMAL;
  Size := prgSize;
  pdwFlags := pdwFlags or IEIFLAG_CACHE;
  case dwRecClrDepth of
    0..3: PixelFormat := pf1bit;
    4..7:  PixelFormat := pf4bit;
    8..14: PixelFormat := pf8bit;
    15:    PixelFormat := pf15bit;
    16..23: PixelFormat := pf16bit;
    24..31: PixelFormat := pf24bit;
  else
    PixelFormat := pf32bit;
  end;

  Result := S_OK;
end;

function TGraphThumbnails.Extract(var phBmpThumbnail: HBITMAP): HRESULT;
var
  OleObject: IOleObject;
  Storage : IStorage;
  DataObject : IDataObject;
  Format : FORMATETC;
  Medium : STGMEDIUM;
begin
  try
    WriteToLog(System.SysUtils.Format('Extract(%.8X, phBmpThumbnail=%.8X)', [DWORD(self), phBmpThumbnail]));
    OleCheck(StgCreateDocfile(nil, STGM_READWRITE or STGM_SHARE_EXCLUSIVE or STGM_CREATE or STGM_DELETEONRELEASE, 0, Storage));
    Format.cfFormat := CF_BITMAP;
    Format.ptd := nil;
    Format.dwAspect := {DVASPECT_CONTENT}DVASPECT_THUMBNAIL;
    Format.lindex := -1;
    Format.tymed := TYMED_GDI;
    OleCheck(OleCreateLinkToFile(PWideChar(FileName), IOleObject,
      OLERENDER_FORMAT, @Format, nil, Storage, OleObject));

    Dataobject := OleObject as IDataObject;
    OleCheck(DataObject.GetData(Format, Medium));
    phBmpThumbnail := Medium.hBitmap;
    Result := S_OK;
    //Warning: Do not attemp to free Storage and OleObject. Delphi will call _Release on interfaces when they go out of scope.
  except
    // Quiet failure
    on E: Exception do
    begin
      LogException(E);
      Result := E_FAIL;
    end;
  end;
end;


function TGraphThumbnails.FileInitialize(pszFilePath: LPWSTR; grfMode: DWORD):HRESULT;
begin
  WriteToLog(Format('FileInitialize(%.8X, pszFilePath=%s, Mode=%.8X)', [DWORD(self), pszFilePath, grfMode]));
  FileName := pszFilePath;
  Result := S_OK;
end;

function TGraphThumbnails.GetThumbnail(cx : uint; out hBitmap : HBITMAP; out bitmapType : dword):HRESULT;
var
  OleObject: IOleObject;
  Storage : IStorage;
  DataObject : IDataObject;
  Format : FORMATETC;
  Medium : STGMEDIUM;
begin
  try
    WriteToLog(System.SysUtils.Format('GetThumbnail(%.8X, cx=%d)', [DWORD(self), cx]));
    OleCheck(StgCreateDocfile(nil, STGM_READWRITE or STGM_SHARE_EXCLUSIVE or STGM_CREATE or STGM_DELETEONRELEASE, 0, Storage));
    Format.cfFormat := CF_BITMAP;
    Format.ptd := nil;
    Format.dwAspect := {DVASPECT_CONTENT}DVASPECT_THUMBNAIL;
    Format.lindex := -1;
    Format.tymed := TYMED_GDI;
    OleCheck(OleCreateLinkToFile(PWideChar(FileName), IOleObject,
      OLERENDER_FORMAT, @Format, nil, Storage, OleObject));

    Dataobject := OleObject as IDataObject;
    OleCheck(DataObject.GetData(Format, Medium));
    hBitmap := Medium.hBitmap;
    Result := S_OK;
    //Warning: Do not attemp to free Storage and OleObject. Delphi will call _Release on interfaces when they go out of scope.
  except
    // Quiet failure
    on E: Exception do
    begin
      LogException(E);
      Result := E_FAIL;
    end;
  end;
end;

{ TGraphThumbnailsObjectFactory }

procedure TGraphThumbnailsObjectFactory.UpdateRegistry(Register: Boolean);
Const
  RegKey = 'GraphFile\ShellEx\{BB2E617C-0920-11D1-9A0B-00C04FC2D6C1}';
//  RegKey = 'Graphfile\ShellEx\{E357FCCD-A995-4576-B01F-234630154E96}';
begin
  WriteToLog(Format('UpdateRegistry(Register=%u)', [Integer(Register)]));
  { perform normal registration }
  inherited UpdateRegistry(Register);
  if Register then
  begin
    CreateRegKey('.grf', '', 'GraphFile', HKEY_CLASSES_ROOT);
    CreateRegKey(RegKey, '', GUIDToString(ClassID), HKEY_CLASSES_ROOT);
    SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, nil, nil);
  end
  else
    DeleteRegKey(RegKey);
end;

destructor TGraphThumbnailsObjectFactory.Destroy;
begin
  WriteToLog('TGraphThumbnailsObjectFactory.Destroy');
end;

function TGraphThumbnailsObjectFactory.QueryInterface(const IID: TGUID; out Obj): HResult;
begin
  Result := inherited QueryInterface(IID, Obj);
  WriteToLog(Format('TGraphThumbnailsObjectFactory.QueryInterface(%.8X, IID=%s, Result=%.8X)', [DWORD(self), GUIDToString(IID), Result]));
end;

function TGraphThumbnailsObjectFactory._AddRef: Integer;
begin
  Result := inherited _AddRef;
  WriteToLog(Format('TGraphThumbnailsObjectFactory.AddRef(%.8X):%d', [DWORD(self), Result]));
end;

function TGraphThumbnailsObjectFactory._Release: Integer;
begin
  Result := inherited _Release;
  WriteToLog(Format('TGraphThumbnailsObjectFactory.Release(%.8X):%d', [DWORD(self), Result]));
end;

var
  Count : Integer = 1;
{
function DllCanUnloadNow: HResult;
begin
  Result := S_FALSE;
end;
}
initialization
  WriteToLog('initialization');
  TGraphThumbnailsObjectFactory.Create(ComServer, TGraphThumbnails,
    CLASS_GraphThumbnails, ciMultiInstance, tmApartment);
finalization
  WriteToLog('finalization');
end.

