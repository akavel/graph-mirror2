# ---------------------------------------------------------------------------
!if !$d(BCB)
BCB = $(MAKEDIR)\..
!endif

# ---------------------------------------------------------------------------
# IDE SECTION
# ---------------------------------------------------------------------------
# The following section of the project makefile is managed by the BCB IDE.
# It is recommended to use the IDE to change any of the values in this
# section.
# ---------------------------------------------------------------------------

VERSION = BCB.06.00
# ---------------------------------------------------------------------------
PROJECT = IComp.bpl
OBJFILES = IComp.obj TCross\Cross.obj TFocusPanel\FocusPanel.obj \
    TIColorBox\IColorBox.obj TGrid\Grid.obj \
    TOpenPreviewDialog\OpenPreviewDialog.obj TLineSelect\LineSelect.obj \
    TMyEdit\MyEdit.obj TMyRadioButton\MyRadioButton.obj \
    TMySplitter\MySplitter.obj TPointSelect\PointSelect.obj \
    TProgress\Progress.obj TProgressForm\ProgressForm.obj TRecent\TRecent.obj \
    TShadeSelect\ShadeSelect.obj TIColorSelect\IColorSelect.obj \
    TIPageSetup\IPageSetup.obj TIPolygon\IPolygon.obj TIFontBox\IFontBox.obj \
    TIRichEdit\IRichEdit.obj TIRichEdit\RichEditOle.obj \
    TIRichEdit\RichEditOleCallback.obj TIPrintDialog\PrintForm.obj \
    TIPrintDialog\IPrintDialog.obj TSaveDialogEx\SaveDialogEx.obj \
    TSymbolDialog\SymbolDialog.obj TSymbolDialog\SymbolForm.obj \
    TExtColorBox\ExtColorBox.obj
RESFILES = IComp.res TCross\Cross.dcr TIColorBox\IColorBox.dcr \
    TLineSelect\LineSelect.dcr TPointSelect\PointSelect.dcr \
    TProgress\Progress.dcr TProgressForm\ProgressForm.dcr TRecent\TRecent.dcr \
    TShadeSelect\ShadeSelect.dcr TIColorSelect\IColorSelect.dcr \
    TIPageSetup\IPageSetup.dcr tipolygon\IPolygon.dcr \
    TIPrintDialog\IPrintDialog.dcr TSymbolDialog\SymbolDialog.dcr \
    Common\CompRes.res
MAINSOURCE = IComp.cpp
RESDEPEN = $(RESFILES) TIPrintDialog\PrintForm.dfm
LIBFILES = 
IDLFILES = 
IDLGENFILES = 
LIBRARIES = 
PACKAGES = rtl.bpi vcl.bpi
SPARELIBS = rtl.lib vcl.lib
DEFFILE = 
OTHERFILES = 
# ---------------------------------------------------------------------------
DEBUGLIBPATH = $(BCB)\lib\debug
RELEASELIBPATH = $(BCB)\lib\release
USERDEFINES = 
SYSDEFINES = NO_STRICT;USEPACKAGES
INCLUDEPATH = $(BCB)\include;$(BCB)\include\vcl;TSaveDialogEx;TIPrintDialog;TOpenPreviewDialog;TIRichEdit;TIFontBox;TIPolygon;TIPageSetup;TIColorSelect;TShadeSelect;TRecent;TProgressForm;TProgress;TPointSelect;TMySplitter;TMyRadioButton;TMyEdit;TLineSelect;TGrid;TIColorBox;TFocusPanel;TCross;Common
LIBPATH = Common;TSymbolDialog;TSaveDialogEx;TIPrintDialog;TOpenPreviewDialog;TIRichEdit;TIFontBox;TIPolygon;TIPageSetup;TIColorSelect;TShadeSelect;TRecent;TProgressForm;TProgress;TPointSelect;TMySplitter;TMyRadioButton;TMyEdit;TLineSelect;TGrid;TIColorBox;TFocusPanel;TCross;$(BCB)\lib\obj;$(BCB)\lib;$(BCB)\lib\Psdk
WARNINGS= -w-par -w-8027
PATHCPP = .;TCross;TFocusPanel;TIColorBox;TGrid;TOpenPreviewDialog;TLineSelect;TMyEdit;TMyRadioButton;TMySplitter;TPointSelect;TProgress;TProgressForm;TRecent;TShadeSelect;TIColorSelect;TIPageSetup;TIPolygon;TIFontBox;TIRichEdit;TIPrintDialog;TSaveDialogEx
PATHASM = .;
PATHPAS = .;
PATHRC = .;
PATHOBJ = .;$(LIBPATH)
# ---------------------------------------------------------------------------
CFLAG1 = -O2 -H=b:\vcl60.csm -Hc -Vx -Ve -X- -a8 -6 -b- -d -k- -vi -c -tWM
IDLCFLAGS = 
PFLAGS = -$Y- -$L- -$D- -$A8 -v -JPHNE -M
RFLAGS = 
AFLAGS = /mx /w2 /zn
LFLAGS = -D"Ivan's components (VCL)" -aa -Tpp -x -Gl -Gi
# ---------------------------------------------------------------------------
ALLOBJ = c0pkg32.obj $(PACKAGES) sysinit.obj $(OBJFILES)
ALLRES = $(RESFILES)
ALLLIB = $(LIBFILES) $(LIBRARIES) import32.lib cp32mt.lib
# ---------------------------------------------------------------------------
!ifdef IDEOPTIONS

[Version Info]
IncludeVerInfo=0
AutoIncBuild=0
MajorVer=1
MinorVer=0
Release=0
Build=0
Debug=0
PreRelease=0
Special=0
Private=0
DLL=0

[Version Info Keys]
CompanyName=
FileDescription=
FileVersion=1.0.0.0
InternalName=
LegalCopyright=
LegalTrademarks=
OriginalFilename=
ProductName=
ProductVersion=1.0.0.0
Comments=

[Debugging]
DebugSourceDirs=$(BCB)\source\vcl

!endif





# ---------------------------------------------------------------------------
# MAKE SECTION
# ---------------------------------------------------------------------------
# This section of the project file is not used by the BCB IDE.  It is for
# the benefit of building from the command-line using the MAKE utility.
# ---------------------------------------------------------------------------

.autodepend
# ---------------------------------------------------------------------------
!if "$(USERDEFINES)" != ""
AUSERDEFINES = -d$(USERDEFINES:;= -d)
!else
AUSERDEFINES =
!endif

!if !$d(BCC32)
BCC32 = bcc32
!endif

!if !$d(CPP32)
CPP32 = cpp32
!endif

!if !$d(DCC32)
DCC32 = dcc32
!endif

!if !$d(TASM32)
TASM32 = tasm32
!endif

!if !$d(LINKER)
LINKER = ilink32
!endif

!if !$d(BRCC32)
BRCC32 = brcc32
!endif


# ---------------------------------------------------------------------------
!if $d(PATHCPP)
.PATH.CPP = $(PATHCPP)
.PATH.C   = $(PATHCPP)
!endif

!if $d(PATHPAS)
.PATH.PAS = $(PATHPAS)
!endif

!if $d(PATHASM)
.PATH.ASM = $(PATHASM)
!endif

!if $d(PATHRC)
.PATH.RC  = $(PATHRC)
!endif

!if $d(PATHOBJ)
.PATH.OBJ  = $(PATHOBJ)
!endif
# ---------------------------------------------------------------------------
$(PROJECT): $(OTHERFILES) $(IDLGENFILES) $(OBJFILES) $(RESDEPEN) $(DEFFILE)
    $(BCB)\BIN\$(LINKER) @&&!
    $(LFLAGS) -L$(LIBPATH) +
    $(ALLOBJ), +
    $(PROJECT),, +
    $(ALLLIB), +
    $(DEFFILE), +
    $(ALLRES)
!
# ---------------------------------------------------------------------------
.pas.hpp:
    $(BCB)\BIN\$(DCC32) $(PFLAGS) -U$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -O$(INCLUDEPATH) --BCB {$< }

.pas.obj:
    $(BCB)\BIN\$(DCC32) $(PFLAGS) -U$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -O$(INCLUDEPATH) --BCB {$< }

.cpp.obj:
    $(BCB)\BIN\$(BCC32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n$(@D) {$< }

.c.obj:
    $(BCB)\BIN\$(BCC32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n$(@D) {$< }

.c.i:
    $(BCB)\BIN\$(CPP32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n. {$< }

.cpp.i:
    $(BCB)\BIN\$(CPP32) $(CFLAG1) $(WARNINGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -n. {$< }

.asm.obj:
    $(BCB)\BIN\$(TASM32) $(AFLAGS) -i$(INCLUDEPATH:;= -i) $(AUSERDEFINES) -d$(SYSDEFINES:;= -d) $<, $@

.rc.res:
    $(BCB)\BIN\$(BRCC32) $(RFLAGS) -I$(INCLUDEPATH) -D$(USERDEFINES);$(SYSDEFINES) -fo$@ $<



# ---------------------------------------------------------------------------




