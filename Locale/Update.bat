@echo off
dxgettext ..\*.dfm ..\resource.rc \..\..\Components\TIPrintDialog\*.dfm \..\..\Components\Common\*.rc
msgremove default.po -i ignore.po -o temp.po
findstr /b /v "#" temp.po > temp2.po
findstr /b /v /C:"msgid \"\"" temp2.po > temp3.po
findstr /v "Project-Id-Version:" temp3.po > temp4.po

echo msgid "" > Template.po
echo msgstr "Project-Id-Version: Graph\n" >> Template.po
cat temp4.po >> Template.po

del temp.po
del temp2.po
del temp3.po
del temp4.po
del default.po
