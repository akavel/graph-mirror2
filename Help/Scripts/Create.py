import glob
import os
import gettext
import codecs

Languages = ["Danish", "Croatian", "Spanish", "German", "Swedish", "French"]
LocalePath = "d:\\Projects\\Graph\\Locale\\"

def Encode(Str):
    return Str.replace('"', '\\"')

os.chdir("../Source")
Files = glob.glob("*.xml")
os.system("..\\Scripts\\xml2po.py -k -o ..\\po\\GraphHelp.pot " + " ".join(Files))
os.chdir("../po")

for Language in Languages:
    # This is a hack to trick gettext into laoding the .mo file
    Lang = gettext.translation(LocalePath + Language, languages=["test"])
    Lang.install()

    InFile = codecs.open("GraphHelp.pot", "r", "utf-8")
    OutFile = codecs.open("GraphHelp_" + Language + ".po", "w", "utf-8")
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
                    OutFile.write('msgstr "%s"\n' % (Encode(Str2),))
                    IgnoreNext = True
