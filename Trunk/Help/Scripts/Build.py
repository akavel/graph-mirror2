import os
import shutil

ToolsDir = "..\\..\\Tools\\"
Languages = [
    ("English", "en"),
    ("Danish", "da"),
    ("Spanish", "es"),
]

print "Copying image files..."
os.system("XCopy ..\\Images\\Common\\*.* ..\\Temp\\Images /I /Q /Y > NUL")
os.system("XCopy ..\\Images\\English\\*.* ..\\Temp\\Images /I /Q /Y > NUL")
os.system("XCopy ..\\Images\\dtd ..\\Temp\\Images\\dtd /I /Q /Y > NUL")
os.system("XCopy ..\\Source\\dtd ..\\Temp\\dtd /S /I /Q /Y > NUL")
os.system("XCopy ..\\Source\\*.css ..\\Temp /Q /Y > NUL")

os.chdir("..\\Source")
os.system(ToolsDir + "xsltproc.exe --output ..\\xsl\\titlepage.xsl " + ToolsDir + "xsl\\template\\titlepage.xsl ..\\xsl\\titlepage.xml")

for Lang in Languages:
    os.system(ToolsDir + "xmllint --nonet --xinclude --output ..\\Temp\Graph.tmp Graph.xml")

    if Lang[0] != "English":
        print
        print "Creating %s translation" % Lang[:1]
        os.system("..\\Scripts\\xml2po.py -p ..\\po\\GraphHelp_%s.po -l %s -o ../Temp/Graph2.tmp ../Temp/Graph.tmp" % Lang)
        os.system("XCopy ..\\Images\\%s\\*.* ..\\Temp\\Images /I /Q /Y > NUL" % Lang[:1])

    print "Creating %s PDF file..." % Lang[:1]
    InFile = "..\\Temp\\Graph2.tmp" if Lang[0] != "English" else "Graph.xml"
    os.system(ToolsDir + "xsltproc.exe --nonet --xinclude --output ..\\Temp\\Graph.fo ..\\xsl\\pdfdoc.xsl " + InFile)
    os.system(ToolsDir + "fop\\fop.bat -q -fo ..\\Temp\\Graph.fo -pdf ..\\Graph-%s.pdf" % Lang[:1])

    print "Creating %s CHM file..." % Lang[:1]
    os.system(ToolsDir + "xsltproc.exe --nonet --xinclude --stringparam htmlhelp.chm ..\\Graph-%s.chm --output ..\\Temp\\Graph ..\\xsl\\htmlhelp.xsl %s"  % (Lang[0], InFile))
    os.system(ToolsDir + "hhc.exe ..\\Temp\\htmlhelp.hhp > NUL")

os.remove(".xml2po.mo")
