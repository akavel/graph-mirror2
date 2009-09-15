import os.path
import os
import sys
import imp
import traceback
import GraphImpl
import Settings
import Data
import vcl
Axes = Settings.GetAxes()
Property = Settings.Property
Redraw = Data.Redraw

Settings.TAxes.NumberFont = property(lambda self: vcl.TObject(handle=Settings._Settings.TAxes_NumberFont_get(self), owned=False))
Settings.TAxes.LabelFont = property(lambda self: vcl.TObject(handle=Settings._Settings.TAxes_LabelFont_get(self), owned=False))
Settings.TAxes.LegendFont = property(lambda self: vcl.TObject(handle=Settings._Settings.TAxes_LegenFont_get(self), owned=False))
Settings.TAxes.TitleFont = property(lambda self: vcl.TObject(handle=Settings._Settings.TAxes_TitleFont_get(self), owned=False))

Settings.TProperty.DefaultLabelFont = property(lambda self: vcl.TObject(handle=Settings._Settings.TProperty_DefaultLabelFont_get(self), owned=False))
Settings.TProperty.DefaultPointLabelFont = property(lambda self: vcl.TObject(handle=Settings._Settings.TProperty_DefaultPointLabelFont_get(self), owned=False))

Selected = Data.Selected
TGraphElem = Data.TGraphElem
TStdFunc = Data.TStdFunc

def InitPlugins():
    print("Loading plugins...")

    PluginsDir = os.path.dirname(sys.argv[0]) + '\\Plugins'
    sys.path.append(PluginsDir)

    global Form1
    Form1 = vcl.TForm(GraphImpl.form1, owned=False)

    Modules = []
    AllowedExt = [ i[0] for i in imp.get_suffixes() ]
    for ModuleName, Ext in [ os.path.splitext(f) for f in os.listdir(PluginsDir) ]:
        if Ext in AllowedExt and not ModuleName in Modules:
            try:
                Modules.append(ModuleName)
                __import__(ModuleName)

            except Exception:
                traceback.print_exc()


import collections
class ConstantsType(collections.MutableMapping):
    def keys(self):
        return GraphImpl.GetConstantNames()
    def __getitem__(self, name):
        return GraphImpl.GetConstant(name)
    def __setitem__(self, name, value):
        if value.__class__ == tuple:
            GraphImpl.SetConstant(name, value[1:], str(value[0]))
        else:
            GraphImpl.SetConstant(name, None, str(value))
    def __delitem__(self, name):
        GraphImpl.DelConstant(name)
    def __iter__(self):
        for k in self.keys(): yield k
    def __len__(self): return len(keys)

def ExecuteEvent(eventlist):
    for action in eventlist:
        try:
            action()
        except:
            traceback.print_exc()

def FindAction(name):
    for o in Form1.Components:
        if o.Name == "ActionManager":
            for a in o.Actions:
                if a.Name == name:
                    return a

Constants = ConstantsType()

Eval = GraphImpl.Eval
EvalComplex = GraphImpl.EvalComplex
SaveAsImage = GraphImpl.SaveAsImage
Update = GraphImpl.Update

OnNew = []
OnLoad = []
OnSelect = []
OnClose = []