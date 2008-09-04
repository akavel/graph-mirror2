import glob
import os
import gettext
import codecs

SourcePath = "../Source/"
OutFile = "..\\po\\GraphHelp.pot"
Language = "Danish"
LocalePath = "d:\\Projects\\Graph\\Locale\\"
#LocalePath = "d:\\"

os.chdir(SourcePath)
Files = glob.glob("*.xml")
os.system("..\\..\\Tools\\xml2po\\xml2po.py -k -o " + OutFile + " " + " ".join(Files))
os.chdir("..\\po")

# This is a hack to trick gettext into laoding the .mo file
Lang = gettext.translation(LocalePath + Language, languages=["test"])
Lang.install()

InFile = codecs.open("GraphHelp.pot", "r", "utf-8")
OutFile = codecs.open("GraphHelp_Danish.po", "w", "utf-8")

IgnoreNext = False
for Line in InFile:
    if not IgnoreNext:
        OutFile.write(Line)
    IgnoreNext = False
    if Line[:5] == "msgid":
        Str = Line[7:-3]
        if Str:
            Str2 = Lang.ugettext(Str)
            if Str2 == Str:
                Str2 = Lang.ugettext(Str + ":")[:-1]
            if Str2 != Str:
                OutFile.write("msgstr \"%s\"\n" % (Str2,))
                IgnoreNext = True
