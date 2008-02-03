@echo off
echo Copying image files...
XCopy ..\Images\Common\*.* Temp\Images /I /Q /Y
XCopy ..\Images\English\*.* Temp\Images /I /Q /Y
XCopy ..\Source\*.css Temp /Q /Y

cd ..\Source
echo.
echo Create PDF file...
..\..\Tools\xsltproc.exe --nonet --xinclude --output ..\Output\Temp\Graph.fo ..\xsl\pdfdoc.xsl Graph.xml
call ..\..\Tools\fop\fop.bat -q -fo ..\Output\Temp\Graph.fo -pdf ..\Output\Graph.pdf

echo.
echo Create CHM file...
..\..\Tools\xsltproc.exe --nonet --xinclude --stringparam html.stylesheet styles_chm.css --stringparam use.id.as.filename 1 --output ..\Output\Temp\Graph ..\xsl\htmlhelp.xsl Graph.xml
..\..\Tools\hhc.exe ..\Output\Temp\htmlhelp.hhp

echo.
pause
