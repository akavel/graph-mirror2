; *** Inno Setup version 5.1.0+ Spanish messages ***
;
; To download user-contributed translations of this file, go to:
;   http://www.jrsoftware.org/is3rdparty.php
;
; Note: When translating this text, do not add periods (.) to the end of
; messages that didn't have them already, because on those messages Inno
; Setup adds the periods automatically (appending a period would result in
; two periods being displayed).
;
; $jrsoftware: issrc/Files/Default.isl,v 1.66 2005/02/25 20:23:48 mlaan Exp $
; Translated by Emiliano Llano D�az
; emiliano_d@hotmail.com

[LangOptions]
LanguageName=Espa<00f1>ol
LanguageID=$0c0a
LanguageCodePage=0
; If the language you are translating to requires special font faces or
; sizes, uncomment any of the following entries and change them accordingly.
;DialogFontName=verdana
;DialogFontSize=8
;WelcomeFontName=Verdana
;WelcomeFontSize=12
;TitleFontName=Arial
;TitleFontSize=29
;CopyrightFontName=Arial
;CopyrightFontSize=8

[Messages]

; *** Application titles
SetupAppTitle=Instalar
SetupWindowTitle=Instalar - %1
UninstallAppTitle=Eliminar
UninstallAppFullTitle=Eliminar - %1

; *** Misc. common
InformationTitle=Informaci�n
ConfirmTitle=Confirmar
ErrorTitle=Error

; *** SetupLdr messages
SetupLdrStartupMessage=Este asistente instalar� %1 en su ordenador. �Desea continuar?
LdrCannotCreateTemp=Imposible crear un archivo temporal. Instalaci�n cancelada.
LdrCannotExecTemp=Imposible ejecutar archivo en la carpeta temporal. Instalaci�n cancelada.

; *** Startup error messages
LastErrorMessage=%1.%n%nError %2: %3
SetupFileMissing=El archivo %1 no se encuentra en la carpeta de instalaci�n. Por favor, corrija el problema u obtenga una nueva copia del programa.
SetupFileCorrupt=Los archivos de instalaci�n est�n da�ados. Por favor, obtenga una nueva copia del programa.
SetupFileCorruptOrWrongVer=Los archivos de instalaci�n est�n da�ados, o son incompatibles con esta versi�n del asistente. Por favor, corrija el problema u obtenga una nueva copia del programa.
NotOnThisPlatform=Este programa no se ejecutar� en %1.
OnlyOnThisPlatform=Este programa debe ser ejecutado en %1.
OnlyOnTheseArchitectures=Este programa s�lo puede ser instalado en versiones de Windows dise�adas para las siguientes arquitecturas del procesador:%n%n%1
MissingWOW64APIs=La versi�n de Windows que est� ejecutando no tiene la funcionalidad exigida por el asistente para realizar una instalaci�n de 64 bits. Para corregir este problema, por favor, instale Service Pack %1.
WinVersionTooLowError=Este programa necesita %1 versi�n %2 o posterior.
WinVersionTooHighError=Este programa no puede ser instalado en %1 versi�n %2 o posterior.
AdminPrivilegesRequired=Debe estar registrado como administrador para instalar este programa.
PowerUserPrivilegesRequired=Debe estar registrado como administrador o equivalente para poder instalar este programa.
SetupAppRunningError=El asistente ha detectado que %1 est� actualmente abierto.%n%nPor favor, cierre todas las ejecuciones del programa, y haga clic en Aceptar para continuar, o Cancelar para salir.
UninstallAppRunningError=El asistente de desinstalaci�n ha detectado que %1 est� actualmente abierto.%n%nPor favor, cierre todas las ejecuciones del programa, y haga clic en Aceptar para continuar, o Cancelar para salir.

; *** Misc. errors
ErrorCreatingDir=El asistente ha sido incapaz de crear la carpeta "%1"
ErrorTooManyFilesInDir=Imposible crear un archivo en la carpeta "%1" porque �sta contiene demasiados archivos

; *** Setup common messages
ExitSetupTitle=Salir de la instalaci�n
ExitSetupMessage=La instalaci�n no se complet�. Si abandona ahora, el programa no ser� instalado.%n%nPodr� ejecutar de nuevo la instalaci�n en otro momento, y completarla.%n%n�Desea salir de la instalaci�n?
AboutSetupMenuItem=&Acerca de la instalaci�n...
AboutSetupTitle=Acerca de la instalaci�n
AboutSetupMessage=%1 versi�n %2%n%3%n%n%1 p�gina Web:%n%4
AboutSetupNote=
TranslatorNote=


; *** Buttons
ButtonBack=< &Atr�s
ButtonNext=&Siguiente >
ButtonInstall=&Instalar
ButtonOK=Aceptar
ButtonCancel=Cancelar
ButtonYes=&S�
ButtonYesToAll=S� a &todo
ButtonNo=&No
ButtonNoToAll=N&o a todo
ButtonFinish=&Terminar
ButtonBrowse=&Examinar...
ButtonWizardBrowse=E&xaminar...
ButtonNewFolder=&Crear nueva carpeta


; *** "Select Language" dialog messages
SelectLanguageTitle=Seleccione el idioma de la instalaci�n
SelectLanguageLabel=Seleccione el idioma que desee emplear durante la instalaci�n:

; *** Common wizard text
ClickNext=Haga clic en Siguiente para continuar, o en Cancelar para salir de la instalaci�n.
BeveledLabel=
BrowseDialogTitle=Buscar carpeta
BrowseDialogLabel=Seleccione una carpeta de la lista inferior, y haga clic en Aceptar.
NewFolderName=Nueva carpeta

; *** "Welcome" wizard page
WelcomeLabel1=Bienvenido al asistente de instalaci�n de [name].
WelcomeLabel2=Este asistente instalar� [name/ver] en su ordenador.%n%nSe recomienda cerrar todas las aplicaciones en ejecuci�n antes de continuar.

; *** "Password" wizard page
WizardPassword=Contrase�a
PasswordLabel1=Esta instalaci�n est� protegida por contrase�a.
PasswordLabel3=Por favor, indique su contrase�a, y haga clic en Continuar.%n%nLas contrase�as distinguen may�sculas y min�sculas.
PasswordEditLabel=&Contrase�a:
IncorrectPassword=La contrase�a introducida no es correcta. Por favor, int�ntelo de nuevo.

; *** "License Agreement" wizard page
WizardLicense=Acuerdo de licencia
LicenseLabel=Por favor, lea la siguiente importante informaci�n antes de continuar.
LicenseLabel3=Por favor, lea el siguiente contrato de licencia. Debe aceptar los t�rminos de este contrato antes de continuar con la instalaci�n.
LicenseAccepted=&Acepto el contrato de licencia
LicenseNotAccepted=&No acepto el contrato de licencia

; *** "Information" wizard pages
WizardInfoBefore=Informaci�n
InfoBeforeLabel=Por favor, lea la siguiente importante informaci�n antes de continuar.
InfoBeforeClickLabel=Cuando est� listo para continuar con la instalaci�n, haga clic en Siguiente.
WizardInfoAfter=Informaci�n
InfoAfterLabel=Por favor, lea la siguiente importante informaci�n antes de continuar.
InfoAfterClickLabel=Cuando est� listo para continuar con la instalaci�n, haga clic en Siguiente.

; *** "User Information" wizard page
WizardUserInfo=Informaci�n de usuario
UserInfoDesc=Por favor, introduzca su informaci�n.
UserInfoName=Nombre de &usuario:
UserInfoOrg=&Organizaci�n:
UserInfoSerial=N�mero de &serie:
UserInfoNameRequired=Debe introducir un nombre.

; *** "Select Destination Directory" wizard page
WizardSelectDir=Seleccione la ubicaci�n de destino
SelectDirDesc=�D�nde se instalar� [name]?
SelectDirLabel3=El asistente instalar� [name] en la siguiente carpeta.
SelectDirBrowseLabel=Para continuar, haga clic en Siguiente. Si desea seleccionar una carpeta diferente, haga clic en Examinar.
DiskSpaceMBLabel=Se necesitan, al menos, [mb] MB de espacio libre en el disco.
ToUNCPathname=El asistente no puede realizar la instalaci�n en un directorio UNC. Si est� tratando de instalar el programa en una red, necesitar� asignarlo a una unidad de red.
InvalidPath=Debe introducir una ruta completa con la letra de la unidad; por ejemplo:%nC:\APP%n%ni un directorio UNC en la forma:%n%n\\server\share
InvalidDrive=La unidad o el directorio UNC que ha seleccionado no existe. Por favor, seleccione otra.
DiskSpaceWarningTitle=No hay espacio suficiente en el disco
DiskSpaceWarning=El asistente necesita, al menos, %1 KB de espacio libre para la instalaci�n, pero la unidad seleccionada s�lo tiene %2 KB disponibles.%n%n�Desea continuar de todos modos?
DirNameTooLong=El nombre de la carpeta o ruta es demasiado largo.
InvalidDirName=El nombre de la carpeta no es v�lido.
BadDirName32=El nombre de la carpeta no puede incluir los siguientes caracteres:%n%n%1
DirExistsTitle=La carpeta ya existe
DirExists=La carpeta:%n%n%1%n%nya existe. �Desea realizar la instalaci�n en esta carpeta de todos modos?
DirDoesntExistTitle=La carpeta no existe
DirDoesntExist=La carpeta:%n%n%1%n%nno existe. �Desea crear la carpeta?

; *** "Select Components" wizard page
WizardSelectComponents=Selecci�n de componentes
SelectComponentsDesc=�Qu� componentes desea instalar?
SelectComponentsLabel2=Seleccione los componentes que desea instalar, y deseleccione aquellos que no desea instalar. Cuando est� listo para continuar, haga clic en Siguiente.
FullInstallation=Instalaci�n completa
; if possible don't translate 'Compact' as 'Minimal' (I mean 'Minimal' in your language)
CompactInstallation=Instalaci�n compacta
CustomInstallation=Instalaci�n personalizada
NoUninstallWarningTitle=Componentes existentes
NoUninstallWarning=El asistente ha detectado que los siguientes componentes ya est�n instalados en su ordenador:%n%n%1%n%nDeseleccionando esos componentes, no los desinstalar�.%n%n�Desea continuar de todos modos?
ComponentSize1=%1 KB
ComponentSize2=%1 MB
ComponentsDiskSpaceMBLabel=La selecci�n actual necesita, al menos, [mb] MB de espacio libre en disco.

; *** "Select Additional Tasks" wizard page
WizardSelectTasks=Seleccione las tareas adicionales
SelectTasksDesc=�Qu� tareas adicionales deber ser ejecutadas?
SelectTasksLabel2=Seleccione las tareas adicionales que desear�a que el asistente ejecutase mientras instala [name], y haga clic en Siguiente.

; *** "Select Start Menu Folder" wizard page
WizardSelectProgramGroup=Seleccione la carpeta del men� de Inicio
SelectStartMenuFolderDesc=�D�nde deben colocarse los accesos directos del programa?
SelectStartMenuFolderLabel3=El asistente crear� los accesos directos del programa en la siguiente carpeta del men� Inicio.
SelectStartMenuFolderBrowseLabel=Para continuar, haga clic en Siguiente. Si desea seleccionar una carpeta diferente, haga clic en Examinar.
MustEnterGroupName=Debe introducir un nombre de carpeta.
GroupNameTooLong=El nombre de la carpeta o ruta es demasiado largo.
InvalidGroupName=El nombre de la carpeta no es v�lido.
BadGroupName=El nombre de la carpeta no puede incluir ninguno de los siguientes caracteres:%n%n%1
NoProgramGroupCheck2=&No crear carpeta en el men� Inicio

; *** "Ready to Install" wizard page
WizardReady=Listo para instalar
ReadyLabel1=El asistente est� listo para iniciar la instalaci�n de [name] en su ordenador.
ReadyLabel2a=Haga clic en Instalar para continuar con la instalaci�n, o en Atr�s si desea revisar o cambiar la configuraci�n.
ReadyLabel2b=Haga clic en Instalar para continuar con la instalaci�n.
ReadyMemoUserInfo=Informaci�n de usuario:
ReadyMemoDir=Ubicaci�n de destino:
ReadyMemoType=Tipo de instalaci�n:
ReadyMemoComponents=Componentes seleccionados:
ReadyMemoGroup=Carpeta del men� Inicio:
ReadyMemoTasks=Tareas adicionales:

; *** "Preparing to Install" wizard page
WizardPreparing=Prepar�ndose para instalar
PreparingDesc=El asistente est� prepar�ndose para instalar [name] en su ordenador.
PreviousInstallNotCompleted=La instalaci�n/eliminaci�n de una versi�n previa del programa no se complet�. Debe reiniciar el ordenador para completar la instalaci�n.%n%nTras reiniciar el ordenador, ejecute de nuevo el asistente para finalizar la instalaci�n de [name].
CannotContinue=La instalaci�n no puede continuar. Por favor, haga clic en Cancelar para salir.

; *** "Installing" wizard page
WizardInstalling=Instalando
InstallingLabel=Por favor, espere mientras el asistente instala [name] en su ordenador.

; *** "Setup Completed" wizard page
FinishedHeadingLabel=Finalizando la instalaci�n de [name]
FinishedLabelNoIcons=El asistente ha concluido la instalaci�n de [name] en su ordenador.
FinishedLabel=El asistente ha terminado de instalar [name] en su ordenador. El programa puede ser ejecutado mediante el icono creado.
ClickFinish=Haga clic en Finalizar para terminar la instalaci�n.
FinishedRestartLabel=Para completar la instalaci�n de [name] debe reiniciar su ordenador. �Desea reiniciar ahora?
FinishedRestartMessage=Para completar la instalaci�n de [name] debe reiniciar su ordenador.%n%n�Desea reiniciar ahora?
ShowReadmeCheck=S�, deseo ver el archivo L�AME
YesRadio=&S�, deseo reiniciar el ordenador ahora
NoRadio=&No, reiniciar� el ordenador m�s tarde
; used for example as 'Run MyProg.exe'
RunEntryExec=Ejecutar %1
; used for example as 'View Readme.txt'
RunEntryShellExec=Ver %1

; *** "Setup Needs the Next Disk" stuff    ---------------------------------------------------
ChangeDiskTitle=La instalaci�n necesita el siguiente disco
SelectDiskLabel2=Por favor, inserte el disco %1 y haga clic en Aceptar.%n%nSi los archivos se localizan en una carpeta diferente a la mostrada abajo, proporcione la ruta correcta o haga clic en Examinar.
PathLabel=&Ruta:
FileNotInDir2=El archivo "%1" no se encuentra en "%2". Por favor, inserte el disco correcto o seleccione otra carpeta.
SelectDirectoryLabel=Por favor, especifique la ubicaci�n del siguiente disco.

; *** Installation phase messages
SetupAborted=La instalaci�n no ha sido terminada.%n%nPor favor, corrija el problema y ejecute Instalar de nuevo.
EntryAbortRetryIgnore=Haga clic en Reintentar para intentar de nuevo, en Ignorar para proceder de cualquier forma, o en Cancelar para interrumpir la instalaci�n.

; *** Installation status messages
StatusCreateDirs=Creando carpetas...
StatusExtractFiles=Copiando archivos...
StatusCreateIcons=Creando iconos del programa...
StatusCreateIniEntries=Creando entradas INI...
StatusCreateRegistryEntries=Creando entradas en el registro...
StatusRegisterFiles=Registrando archivos...
StatusSavingUninstall=Guardando informaci�n para eliminar el programa...
StatusRunProgram=Terminando la instalaci�n...
StatusRollback=Deshaciendo los cambios...

; *** Misc. errors
ErrorInternal2=Error Interno %1
ErrorFunctionFailedNoCode=%1 fall�
ErrorFunctionFailed=%1 fall�; c�digo %2
ErrorFunctionFailedWithMessage=%1 fall�; c�digo %2.%n%3
ErrorExecutingProgram=Imposible ejecutar el archivo:%n%1

; *** Registry errors
ErrorRegOpenKey=Error abriendo la clave de registro:%n%1\%2
ErrorRegCreateKey=Error creando la clave de registro:%n%1\%2
ErrorRegWriteKey=Error escribiendo en la clave de registro:%n%1\%2

; *** INI errors
ErrorIniEntry=Error creando entrada en archivo INI "%1".

; *** File copying errors
FileAbortRetryIgnore=Haga clic en Reintentar para probar de nuevo, en Ignorar para omitir este archivo (no recomendado), o en Cancelar para interrumpir la instalaci�n.
FileAbortRetryIgnore2=Haga clic en Reintentar para probar de nuevo, en Ignorar para proceder de cualquier forma (no se recomienda), o en Cancelar para interrumpir la instalaci�n.
SourceIsCorrupted=El archivo de origen est� da�ado
SourceDoesntExist=El archivo de origen "%1" no existe
ExistingFileReadOnly=El archivo existente est� marcado como de s�lo lectura.%n%nHaga clic en Reintentar para quitar el atributo de s�lo lectura y probar de nuevo, en Ignorar para omotir este archivo, o en Cancelar para interrumpir la instalaci�n.
ErrorReadingExistingDest=Ocurri� un error al tratar de leer el archivo:
FileExists=El archivo ya existe.%n%n�Desea sobrescribirlo?
ExistingFileNewer=El archivo existente es m�s reciente que el que est� tratando de instalar. Se recomienda conservarlo.%n%n�Desea mantener el archivo existente?
ErrorChangingAttr=Ocurri� un error al tratar de cambiar los atributos del archivo:
ErrorCreatingTemp=Ocurri� un error al tratar de crear un archivo en la carpeta destino:
ErrorReadingSource=Ocurri� un error al tratar de leer el archivo origen:
ErrorCopying=Ocurri� un error al tratar de copiar el archivo:
ErrorReplacingExistingFile=Ocurri� un error al tratar de reemplazar el archivo existente:
ErrorRestartReplace=Fall� el reintento de reemplazar:
ErrorRenamingTemp=Ocurri� un error al tratar de renombrar un archivo en la carpeta de destino:
ErrorRegisterServer=Imposible registrar la librer�a DLL/OCX: %1
ErrorRegisterServerMissingExport=No se encontr� DllRegisterServer export
ErrorRegisterTypeLib=Imposible registrar la biblioteca de tipo: %1

; *** Post-installation errors
ErrorOpeningReadme=Ocurri� un error al tratar de abrir el archivo L�AME.
ErrorRestartingComputer=El programa de instalaci�n no puede reiniciar la computadora. Por favor, h�galo manualmente.

; *** Uninstaller messages
UninstallNotFound=El archivo "%1" no existe. No se puede eliminar.
UninstallOpenError=No se pudo abrir el archivo "%1". No es posible eliminar el programa
UninstallUnsupportedVer=El archivo de bit�cora para eliminar "%1" se halla en un formato no reconocido por esta versi�n del programa. No se puede continuar
UninstallUnknownEntry=Se encontr� un registro desconocido (%1 en la bit�cora de eliminaci�n
ConfirmUninstall=�Est� seguro que desea eliminar completamente %1 y todos sus componentes?
UninstallOnlyOnWin64=Esta instalaci�n s�lo puede eliminarse en un sistema Windows de 64 bits.
OnlyAdminCanUninstall=Este programa s�lo puede eliminarlo un usuario con privilegios de administrador.
UninstallStatusLabel=Por favor, espere mientras se elimina %1 de su ordenador.
UninstalledAll=%1 se elimin� con �xito de su ordenador.
UninstalledMost=Finaliz� la eliminaci�n de %1.%n%nAlgunos elementos no pudieron quitarse. �stos pueden borrarse manualmente.
UninstalledAndNeedsRestart=Para completar la eliminaci�n de %1 se requiere reiniciar el ordenador.%n%n�Desea reiniciarlo en este momento?
UninstallDataCorrupted=El archivo "%1" est� da�ado. No se puede eliminar

; *** Uninstallation phase messages
ConfirmDeleteSharedFileTitle=�Eliminar archivos compartidos?
ConfirmDeleteSharedFile2=El sistema informa que el siguiente archivo compartido no es utilizado por otros programas. �Desea eliminar este archivo?%n%nSi otros programas est�n us�ndolo y es eliminado, �stos podr�an no funcionar correctamente. Si no est� seguro, elija No. Dejar el archivo en su sistema no causa ning�n da�o.
SharedFileNameLabel=Nombre del archivo:
SharedFileLocationLabel=Ubicaci�n:
WizardUninstalling=Estado de la eliminaci�n
StatusUninstalling=Eliminando %1...

; The custom messages below aren't used by Setup itself, but if you make
; use of them in your scripts, you'll want to translate them.

[CustomMessages]

NameAndVersion=%1 versi�n %2
AdditionalIcons=Iconos adicionales:
CreateDesktopIcon=Crear un acceso directo en el &Escritorio
CreateQuickLaunchIcon=Crear un icono de acceso &r�pido
ProgramOnTheWeb=%1 en la Web
UninstallProgram=Eliminar %1
LaunchProgram=Ejecutar %1
AssocFileExtension=&Asociar %1 con la extensi�n de archivo %2
AssocingFileExtension=Asociar %1 con la extensi�n de archivo %2...
