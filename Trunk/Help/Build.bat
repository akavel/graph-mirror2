@echo off
echo Copying image files...
XCopy Images\Common\*.* Temp\Images /I /Q /Y
XCopy Images\English\*.* Temp\Images /I /Q /Y
XCopy Source\*.css Temp /Q /Y

cd Source
echo.
echo Creating English PDF file...
..\..\Tools\xsltproc.exe --nonet --xinclude --output ..\Temp\Graph.fo ..\xsl\pdfdoc.xsl Graph.xml
call ..\..\Tools\fop\fop.bat -q -fo ..\Temp\Graph.fo -pdf ..\Graph-English.pdf

echo.
echo Creating English CHM file...
..\..\Tools\xsltproc.exe --nonet --xinclude --output ..\Temp\Graph ..\xsl\htmlhelp.xsl Graph.xml
..\..\Tools\hhc.exe ..\Temp\htmlhelp.hhp

rem Create Danish translation
..\..\Tools\xmllint --nonet --xinclude --output Graph.tmp Graph.xml
..\..\Tools\xml2po\xml2po.py -p ..\po\GraphHelp_Danish.po -l da -o Graph2.tmp Graph.tmp

echo Creating Danish PDF file...
..\..\Tools\xsltproc.exe --nonet --xinclude --output ..\Temp\Graph.fo ..\xsl\pdfdoc.xsl Graph2.tmp
call ..\..\Tools\fop\fop.bat -q -fo ..\Temp\Graph.fo -pdf ..\Graph-Danish.pdf

echo Creating Danish CHM file...
..\..\Tools\xsltproc.exe --nonet --xinclude --stringparam htmlhelp.chm ..\Graph-Danish.chm --output ..\Temp\Graph ..\xsl\htmlhelp.xsl Graph2.tmp
..\..\Tools\hhc.exe ..\Temp\htmlhelp.hhp

echo.
pause
