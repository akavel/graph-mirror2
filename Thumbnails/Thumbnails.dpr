library Thumbnails;

uses
  ComServ,
  Thumbnails_TLB in 'Thumbnails_TLB.pas',
  ThumbnailsImpl in 'ThumbnailsImpl.pas';

exports
  DllGetClassObject,
  DllCanUnloadNow,
  DllRegisterServer,
  DllUnregisterServer,
  DllInstall;

{$R *.TLB}
{$R *.RES}

begin
end.
