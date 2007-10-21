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

    import vcl
    global Form1
    Form1 = vcl.TForm(GraphImpl.form1, owned=False)

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

Eval = GraphImpl.Eval
EvalComplex = GraphImpl.EvalComplex
