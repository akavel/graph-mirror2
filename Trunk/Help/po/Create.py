import glob
import os

SourcePath = "../Source/"
OutFile = "..\\po\\GraphHelp.pot"
IgnoreFile = "..\\po\\Ignore.po"

os.chdir(SourcePath)
Files = glob.glob("*.xml")
os.system("..\\..\\Tools\\xml2po\\xml2po.py -k -o Temp.po " + " ".join(Files))
os.system("msgremove Temp.po -i " + IgnoreFile + " -o " + OutFile)
os.remove("Temp.po")
