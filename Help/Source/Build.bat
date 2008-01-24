@echo off
D:\Projects\Diverse\TortoiseSvn\Tools\xsltproc.exe --nonet --xinclude --output Graph.fo ..\xsl\pdfdoc.xsl Graph.xml
call D:\Projects\Diverse\TortoiseSvn\Tools\fop\fop.bat -q -fo Graph.fo -pdf Graph.pdf
pause
