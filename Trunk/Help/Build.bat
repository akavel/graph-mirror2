@echo off
echo Copying image files...
XCopy Images\Common\*.* Temp\Images /I /Q /Y
XCopy Images\English\*.* Temp\Images /I /Q /Y
XCopy Source\*.css Temp /Q /Y

cd Source
echo.
echo Create PDF file...
..\..\Tools\xsltproc.exe --nonet --xinclude --output ..\Temp\Graph.fo ..\xsl\pdfdoc.xsl Graph.xml
call ..\..\Tools\fop\fop.bat -q -fo ..\Temp\Graph.fo -pdf ..\Graph-English.pdf

echo.
echo Create CHM file...
..\..\Tools\xsltproc.exe --nonet --xinclude --output ..\Temp\Graph ..\xsl\htmlhelp.xsl Graph.xml
..\..\Tools\hhc.exe ..\Temp\htmlhelp.hhp

echo.
pause
