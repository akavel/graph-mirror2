@echo off
echo Copying image files...
XCopy Images\*.* Temp\Images /I /Q /Y

echo.
echo Create PDF file...
D:\Projects\Diverse\TortoiseSvn\Tools\xsltproc.exe --nonet --xinclude --output Temp\Graph.fo ..\xsl\pdfdoc.xsl Graph.xml
call D:\Projects\Diverse\TortoiseSvn\Tools\fop\fop.bat -q -fo Temp\Graph.fo -pdf Graph.pdf

echo.
echo Create CHM file...
D:\Projects\Diverse\TortoiseSvn\Tools\xsltproc.exe --nonet --xinclude --stringparam html.stylesheet styles_chm.css --stringparam use.id.as.filename 1 --output Temp\Graph ..\xsl\htmlhelp.xsl Graph.xml
D:\Projects\Diverse\TortoiseSvn\Tools\hhc.exe Temp\htmlhelp.hhp > NUL

echo.
pause
