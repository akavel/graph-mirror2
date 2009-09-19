import os.path
import os
import sys
import imp
import traceback
import GraphImpl
import Settings
import Data
import vcl
import xmlrpc.client

Axes = Settings.GetAxes()
Property = Settings.Property
Redraw = Data.Redraw

Settings.TProperty.DefaultLabelFont = property(lambda self: vcl.TObject(handle=Settings._Settings.TProperty_DefaultLabelFont_get(self), owned=False))
Settings.TProperty.DefaultPointLabelFont = property(lambda self: vcl.TObject(handle=Settings._Settings.TProperty_DefaultPointLabelFont_get(self), owned=False))

Selected = Data.Selected
TGraphElem = Data.TGraphElem
TStdFunc = Data.TStdFunc
CreateStdFunc = Data.CreateStdFunc
CreateParFunc = Data.CreateParFunc
CreatePolFunc = Data.CreatePolFunc

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

def ExecuteEvent(event, *args):
    for action in EventList[event]:
        try:
            if action(*args):
                return True
        except:
            traceback.print_exc()

def FindAction(name):
    for o in Form1.Components:
        if o.Name == "ActionManager":
            for a in o.Actions:
                if a.Name == name:
                    return a

class FunctionListType(collections.MutableSequence):
    def __getitem__(self, key):
        if key < Data.GetFunctionListSize():
            return Data.GetFunctionListItem(key)
        raise IndexError
    def __len__(self):
        return Data.GetFunctionListSize()
    def insert(self, key, value):
        Data.InsertFunctionListItem(key, value)
    def __setitem__(self, key, value):
        Data.ReplaceFunctionListItem(key, value)
    def append(self, value):
        Data.InsertFunctionListItem(Data.GetFunctionListSize(), value)
    def __delitem__(self, key):
        Data.DeleteFunctionListItem(key)
    def __repr__(self):
        return repr(list(self))

class PluginDataType(collections.MutableMapping):
    def __init__(self, dict): self.data = dict
    def __len__(self): return self.size()
    def __getitem__(self, key): return xmlrpc.client.loads(self.data[key])[0]
    def __setitem__(self, key, item): self.data[key] = xmlrpc.client.dumps(item).replace("\n", "")
    def __delitem__(self, key): self.data.erase(key)
    def __iter__(self): return iter(self.data)
    def __contains__(self, key): return key in self.data
    def __repr__(self): return repr(dict(self))

TGraphElem.PluginData = property(lambda self: PluginDataType(self._PluginData))


Constants = ConstantsType()
FunctionList = FunctionListType()

Eval = GraphImpl.Eval
EvalComplex = GraphImpl.EvalComplex
SaveAsImage = GraphImpl.SaveAsImage
Update = GraphImpl.Update

OnNew = []
OnLoad = []
OnSelect = []
OnClose = []
OnEdit = []
EventList = [OnNew, OnLoad, OnSelect, OnClose, OnEdit]
