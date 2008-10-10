import os
import shutil
import sys

ToolsDir = "..\\..\\Tools\\"
Languages = [
    ("English",  "en", {"$LabelWidth": "2em"}),
    ("Danish",   "da", {"$LabelWidth": "5em"}),
    ("German",   "de", {"$LabelWidth": "2em"}),
    ("Spanish",  "es", {"$LabelWidth": "2em"}),
    ("Swedish",  "sv", {"$LabelWidth": "4em"}),
    ("French",   "fr", {"$LabelWidth": "2em"}),
    ("Croatian", "hr", {"$LabelWidth": "5em"}),
]

def ReplaceStrings(FileName, Dict):
    if Dict:
        File = open(FileName)
        Str = File.read()
        for Key in Dict:
            Str = Str.replace(Key, Dict[Key])
        File.close()
        File = open(FileName, "w")
        File.write(Str)

def HandleLanguage(Language, Lang, Dict):
    os.system(ToolsDir + "xmllint --nonet --xinclude --output ..\\Temp\Graph.tmp Graph.xml")

    if Language != "English":
        print
        print "Creating %s translation" % (Language,)
        os.system("..\\Scripts\\xml2po.py -p ..\\po\\GraphHelp_%s.po -l %s -o ../Temp/Graph2.tmp ../Temp/Graph.tmp" % (Language, Lang))
        os.system("XCopy ..\\Images\\%s\\*.* ..\\Temp\\Images /I /Q /Y > NUL" % (Language,))

    print "Creating %s PDF file..." % (Language,)
    InFile = "..\\Temp\\Graph2.tmp" if Language != "English" else "..\\Temp\\Graph.tmp"
    ReplaceStrings(InFile, Dict)
    os.system(ToolsDir + "xsltproc.exe --nonet --xinclude --output ..\\Temp\\Graph.fo ..\\xsl\\pdfdoc.xsl " + InFile)
    os.system(ToolsDir + "fop\\fop.bat -q -fo ..\\Temp\\Graph.fo -pdf ..\\Graph-%s.pdf" % (Language,))

    print "Creating %s CHM file..." % (Language,)
    os.system(ToolsDir + "xsltproc.exe --nonet --xinclude --stringparam htmlhelp.chm ..\\Graph-%s.chm --output ..\\Temp\\Graph ..\\xsl\\htmlhelp.xsl %s"  % (Language, InFile))
    os.system(ToolsDir + "hhc.exe ..\\Temp\\htmlhelp.hhp > NUL")


print "Copying image files..."
os.system("XCopy ..\\Images\\Common\\*.* ..\\Temp\\Images /I /Q /Y > NUL")
os.system("XCopy ..\\Images\\English\\*.* ..\\Temp\\Images /I /Q /Y > NUL")
os.system("XCopy ..\\Images\\dtd ..\\Temp\\Images\\dtd /I /Q /Y > NUL")
os.system("XCopy ..\\Source\\dtd ..\\Temp\\dtd /S /I /Q /Y > NUL")
os.system("XCopy ..\\Source\\*.css ..\\Temp /Q /Y > NUL")

os.chdir("..\\Source")
os.system(ToolsDir + "xsltproc.exe --output ..\\xsl\\titlepage.xsl " + ToolsDir + "xsl\\template\\titlepage.xsl ..\\xsl\\titlepage.xml")

for Language, Lang, Dict in Languages:
    if len(sys.argv) == 1 or Language in sys.argv:
        HandleLanguage(Language, Lang, Dict)

os.remove(".xml2po.mo")
