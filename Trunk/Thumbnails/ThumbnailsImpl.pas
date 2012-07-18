unit ThumbnailsImpl;

interface
uses
  System.Win.ComObj, Winapi.ActiveX, Thumbnails_TLB,
  Winapi.ShlObj, Winapi.Windows, Vcl.Graphics;

type
  IInitializeWithFile = interface(IUnknown)
    ['{b7d14566-0509-4cce-a71f-0a554233bd9b}']
    function Initialize(pszFilePath: LPWSTR; grfMode: DWORD):HRESULT;stdcall;
  end;

  TGraphThumbnails = class(TAutoObject, IGraphThumbnails, IInitializeWithFile, IExtractImage)
  private
    FileName : string;
    Size : TSize;
    PixelFormat : TPixelFormat;
  protected
    { IInitializeWithFile Methods }
    function IInitializeWithFile.Initialize = ShellInitialize;
    function ShellInitialize(pszFilePath: LPWSTR; grfMode: DWORD):HRESULT;stdcall;

    { IExtractImage Methods }
    function GetLocation(pszPathBuffer: LPWSTR; cch: DWORD; var pdwPriority: DWORD;
      var prgSize: TSize; dwRecClrDepth: DWORD;
      var pdwFlags: DWORD): HRESULT; stdcall;
    function Extract(var phBmpThumbnail: HBITMAP): HRESULT; stdcall;

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
  end;

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

function TGraphThumbnails.ShellInitialize(pszFilePath: LPWSTR; grfMode: DWORD):HRESULT;
begin
  WriteToLog(Format('ShellInitialize(%.8X, pszFilePath="%s", grfMode=%d)', [DWORD(self), string(pszFilePath), grfMode]));
  FileName := pszFilePath;
  Result := S_OK;
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
  R: TRect;
  Format : FORMATETC;
  Bitmap : TBitmap;
begin
  try
    WriteToLog(System.SysUtils.Format('Extract(%.8X, phBmpThumbnail=%.8X)', [DWORD(self), phBmpThumbnail]));
    OleCheck(StgCreateDocfile(nil, STGM_READWRITE or STGM_SHARE_EXCLUSIVE or STGM_CREATE or STGM_DELETEONRELEASE, 0, Storage));
    Format.cfFormat := CF_BITMAP;
    Format.ptd := nil;
    Format.dwAspect := DVASPECT_CONTENT{DVASPECT_THUMBNAIL};
    Format.lindex := -1;
    Format.tymed := TYMED_GDI;
    OleCheck(OleCreateLinkToFile(PWideChar(FileName), IOleObject,
      OLERENDER_FORMAT, @Format, nil, Storage, OleObject));

    R := Rect(0, 0, Size.cx, Size.cy);
    Bitmap := TBitmap.Create;
    Bitmap.Canvas.Lock;
    try
      Bitmap.Width := Size.cx;
      Bitmap.Height := Size.cy;
      Bitmap.PixelFormat := PixelFormat;
      OleCheck(OleDraw(OleObject, DVASPECT_CONTENT{DVASPECT_THUMBNAIL}, Bitmap.Canvas.Handle, R));
      phBmpThumbnail := Bitmap.Handle;
      Bitmap.ReleaseHandle;
    finally
      Bitmap.Canvas.Unlock;
      FreeAndNil(Bitmap);
    end;
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
begin
  WriteToLog(Format('UpdateRegistry(Register=%u)', [Register]));
  { perform normal registration }
  inherited UpdateRegistry(Register);
  if Register then
  begin
    CreateRegKey('.grf', '', 'GraphFile', HKEY_CLASSES_ROOT);
    CreateRegKey(RegKey, '', GUIDToString(ClassID), HKEY_CLASSES_ROOT);
  end
  else
    DeleteRegKey(RegKey);
end;

initialization
  WriteToLog('initialization');
  TGraphThumbnailsObjectFactory.Create(ComServer, TGraphThumbnails,
    CLASS_GraphThumbnails, ciMultiInstance, tmApartment);
finalization
  WriteToLog('finalization');
end.
