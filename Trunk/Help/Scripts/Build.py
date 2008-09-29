import os
import shutil

ToolsDir = "..\\..\\Tools\\"
Languages = [
    ("Danish", "da"),
    ("Spanish", "es"),
]

print "Copying image files..."
os.system("XCopy ..\\Images\\Common\\*.* ..\\Temp\\Images /I /Q /Y")
os.system("XCopy ..\\Images\\English\\*.* ..\\Temp\\Images /I /Q /Y")
os.system("XCopy ..\\Images\\dtd ..\\Temp\\Images\\dtd /I /Q /Y")
os.system("XCopy ..\\Source\\*.css ..\\Temp /Q /Y")

os.chdir("..\\Source")
os.system(ToolsDir + "xsltproc.exe --output ..\\xsl\\titlepage.xsl " + ToolsDir + "xsl\\template\\titlepage.xsl ..\\xsl\\titlepage.xml")

print
print "Creating English PDF file..."
os.system(ToolsDir + "xsltproc.exe --nonet --xinclude --output ..\\Temp\\Graph.fo ..\\xsl\\pdfdoc.xsl Graph.xml")
os.system(ToolsDir + "fop\\fop.bat -q -fo ..\\Temp\\Graph.fo -pdf ..\\Graph-English.pdf")

print
print "Creating English CHM file..."
os.system(ToolsDir + "xsltproc.exe --nonet --xinclude --output ..\\Temp\\Graph ..\\xsl\\htmlhelp.xsl Graph.xml")
os.system(ToolsDir + "hhc.exe ..\\Temp\\htmlhelp.hhp")

os.system(ToolsDir + "xmllint --nonet --xinclude --output ..\\Temp\Graph.tmp Graph.xml")

for Lang in Languages:
    print
    print "Creating %s translation" % Lang[:1]
    os.system("..\\Scripts\\xml2po.py -p ..\\po\\GraphHelp_%s.po -l %s -o ../Temp/Graph2.tmp ../Temp/Graph.tmp" % Lang)
    os.system("XCopy ..\\Images\\%s\\*.* ..\\Temp\\Images /I /Q /Y" % Lang[:1])

    print "Creating %s PDF file..." % Lang[:1]
    os.system(ToolsDir + "xsltproc.exe --nonet --xinclude --output ..\\Temp\\Graph.fo ..\\xsl\\pdfdoc.xsl ..\\Temp\\Graph2.tmp")
    os.system(ToolsDir + "fop\\fop.bat -q -fo ..\\Temp\\Graph.fo -pdf ..\\Graph-%s.pdf" % Lang[:1])

    print "Creating %s CHM file..." % Lang[:1]
    os.system(ToolsDir + "xsltproc.exe --nonet --xinclude --stringparam htmlhelp.chm ..\\Graph-%s.chm --output ..\\Temp\\Graph ..\\xsl\\htmlhelp.xsl ..\\Temp\\Graph2.tmp" % Lang[:1])
    os.system(ToolsDir + "hhc.exe ..\\Temp\\htmlhelp.hhp")

os.remove(".xml2po.mo")
