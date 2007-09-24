import os.path
import os
import sys
import imp
import traceback
import GraphImpl

def InitPlugins():
    print "Loading plugins..."
    sys.stdin = None

    PluginsDir = os.path.dirname(sys.argv[0]) + '\\Plugins'
    sys.path.append(PluginsDir)

    Modules = []
    AllowedExt = [ i[0] for i in imp.get_suffixes() ]
    for ModuleName, Ext in [ os.path.splitext(f) for f in os.listdir(PluginsDir) ]:
        if Ext in AllowedExt and not ModuleName in Modules:
            try:
                Modules.append(ModuleName)
                __import__(ModuleName)

            except ImportError, e:
                traceback.print_exc()


class Action(object):
    def __init__(self, caption, event, **keywords):
        object.__setattr__(self, "id", GraphImpl.CreateAction())
        GraphImpl.SetActionAttr(self.id, caption=caption, event=event, **keywords)

    def __getattr__(self, name):
        return GraphImpl.GetActionAttr(self.id)[name]
    def __setattr__(self, name, value):
        GraphImpl.SetActionAttr(self.id, **{name:value})

def CompileCommand(source):
    """Try to compile a piece of source code, returning a status code
    and the compiled result.  If the status code is "okay" the code is
    complete and compiled successfully; if it is "more" then the code
    can be compiled, but an interactive session should wait for more
    input; if it is "bad" then there is a syntax error in the code and
    the second returned value is the error message."""
    err = err1 = err2 = None
    code = code1 = code2 = None

    try:
        code = compile(source, "<console>", "single")
    except SyntaxError, err:
        pass
    else:
        return "okay", code

    try:
        code1 = compile(source + "\n", "<console>", "single")
    except SyntaxError, err1:
        pass
    else:
        return "more", code1

    try:
        code2 = compile(source + "\n\n", "<console>", "single")
    except SyntaxError, err2:
        pass

    try:
        code3 = compile(source + "\n", "<console>", "exec")
    except SyntaxError, err3:
        pass
    else:
        return "okay", code3

    try:
        code4 = compile(source + "\n\n", "<console>", "exec")
    except SyntaxError, err4:
        pass

    if err3[1][2] != err4[1][2]:
        return "more", None

    if err1[1][2] != err2[1][2]:
        return "more", None

    return "bad", err1

def ExecuteCommand(source):
    status, code = CompileCommand(source)

    if status == "more":
        raise code

    elif status == "bad":
        traceback.print_exception(SyntaxError, code, None)

    elif status == "okay":
        exec code