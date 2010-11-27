unit FindClass;

interface
uses TypInfo;

function LookUpClass(Name : string) : PTypeInfo;

implementation
uses Forms, StdCtrls, Types, Graphics, SysUtils, ActnList, ExtCtrls;

type
  TClassList = record
    Name : string;
    TypeInfo : PTypeInfo;
    constructor Create(AName : string; ATypeInfo : PTypeInfo);
  end;

var
  ClassList : array[1..20] of TClassList;

constructor TClassList.Create(AName : string; ATypeInfo : PTypeInfo);
begin
  Name := AName;
  TypeInfo := ATypeInfo;
end;

function LookUpClass(Name : string) : PTypeInfo;
var
  I : Integer;
begin
  for I := 1 to length(ClassList) do
    if ClassList[I].Name = Name then
      Exit(ClassList[I].TypeInfo);
  raise Exception.Create('Class "' + Name + '" not found');
end;

procedure InitClassList;
begin
  ClassList[1] := TClassList.Create('TForm', TypeInfo(TForm));
  ClassList[2] := TClassList.Create('TButton', TypeInfo(TButton));
  ClassList[3] := TClassList.Create('TRect', TypeInfo(TRect));
  ClassList[4] := TClassList.Create('TPoint', TypeInfo(TPoint));
  ClassList[5] := TClassList.Create('Integer', TypeInfo(Integer));
  ClassList[6] := TClassList.Create('TAction', TypeInfo(TAction));
  ClassList[7] := TClassList.Create('TPanel', TypeInfo(TPanel));
  ClassList[8] := TClassList.Create('TLabel', TypeInfo(TLabel));
  ClassList[9] := TClassList.Create('TEdit', TypeInfo(TEdit));
  ClassList[10] := TClassList.Create('TColorBox', TypeInfo(TColorBox));
  ClassList[11] := TClassList.Create('TMemo', TypeInfo(TMemo));
end;

begin
  InitClassList;
end.
