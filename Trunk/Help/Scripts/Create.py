import glob
import os
import gettext
import codecs

Languages = ["Danish", "Croatian", "Spanish", "German", "Swedish", "French", "Polish", "Czech", "Dutch", "Turkish"]
LocalePath = "d:\\Projects\\Graph\\Locale\\"

def Encode(Str):
    return Str.replace('"', '\\"').replace('\n', ' ')

os.chdir("../Source")
Files = glob.glob("*.xml")
os.system("..\\Scripts\\xml2po.py -k -o ..\\po\\GraphHelp.pot " + " ".join(Files))
os.chdir("../po")

for Language in Languages:
    print
    print Language + ":"
    FileName = "GraphHelp_%s.po" % (Language,)

    if os.path.exists(FileName):
        # Merge with old translation
        os.system("..\\..\\Tools\\msgmerge -U -v -q --backup=off %s GraphHelp.pot" % (FileName,))
        InFile = FileName
    else:
        InFile = "GraphHelp.pot"

    # This is a hack to trick gettext into laoding the .mo file
    Lang = gettext.translation(LocalePath + Language, languages=["test"])
    Lang.install()

    Lines = codecs.open(InFile, "r", "utf-8").readlines()
    OutFile = codecs.open(FileName, "w", "utf-8")
    IgnoreNext = False
    for Line in Lines:
        if not IgnoreNext:
            OutFile.write(Line)
        IgnoreNext = False
        if Line[:5] == "msgid":
            Str = Line[7 : Line.find('"', 7)]
            if Str:
                Str2 = Lang.ugettext(Str)
                if Str2 == Str:
                    Str2 = Lang.ugettext(Str + ":")[:-1]
                if Str2 != Str:
                    OutFile.write('msgstr "%s"\n' % (Encode(Str2),))
                    IgnoreNext = True
