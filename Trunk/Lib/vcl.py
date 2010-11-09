import PyVcl
import sys

class Method:
    def __init__(self, handle, name):
        self._handle = handle
        self._name = name

    def __call__(self, *args):
        return PyVcl.CallMethod(self._handle, self._name)

class TObject(object):
    def __init__(self, handle=0, owned=True, Parent=None, **keywords):
        object.__setattr__(self, "_handle", handle)
        object.__setattr__(self, "_owned", owned)
        object.__setattr__(self, "Components", VclListWrapperType(handle, "Components", "ComponentCount"))
        object.__setattr__(self, "Actions", VclListWrapperType(handle, "Actions", "ActionCount"))
        if Parent != None:  self.__setattr__("Parent", Parent) #It is best to set Parent first
        for name, value in keywords.items():
            self.__setattr__(name, value)

    @property
    def PropertyList(self):
        return PyVcl.GetPropertyList(self._handle)

    def __del__(self):
        if "_owned" in self.__dict__ and self._owned:
            PyVcl.DeleteObject(self._handle)
    def __getattr__(self, name):
        result = PyVcl.GetProperty(self._handle, name)
        if result[1] == 10: #Method
            return Method(self._handle, name)
        if result[1] == 7:  # An object
            return TObject(result[0], owned=False) if result[0] != 0 else None
        return result[0]
    def __setattr__(self, name, value):
        PyVcl.SetProperty(self._handle, name, value._handle if isinstance(value, TObject) else value) # Handle objects as values

    def __repr__(self):
        try:
            return "<object '%s' of type '%s' instance of %s.%s>" % (self.Name, self.ClassName(), self.__class__.__module__, self.__class__.__name__)
        except PyVcl.PropertyError:
            return "<object of type '%s' instance of %s.%s>" % (self.ClassName, self.__class__.__module__, self.__class__.__name__)

class TAction(TObject):
    def __init__(self, handle=0, **keywords):
        import GraphImpl
        TObject.__init__(self, GraphImpl.CreateAction() if handle==0 else handle, owned=False, **keywords)
    def __setattr__(self, name, value):
        if name == "ShortCut":
            TObject.__setattr__(self, "ShortCut", PyVcl.CallFunction("TextToShortCut", int, value) & 0xFFFF)
        else:
            TObject.__setattr__(self, name, value)
    @property
    def ShortCut(self):
        return PyVcl.CallFunction("ShortCutToText", str, TObject.__getattr__(self, "ShortCut"))
    def Execute(self):
        PyVcl.CallMethod(self._handle, "Execute")

import collections
class VclListWrapperType(collections.UserList):
    def __init__(self, handle, listname, countname):
        self._handle = handle
        self._listname = listname
        self._countname = countname
    def __getitem__(self, key):
        try:
            return TObject(PyVcl.GetProperty(self._handle, self._listname, key)[0], owned=False)
        except VclError:
            raise IndexError
    def __len__(self):
        return PyVcl.GetProperty(self._handle, self._countname)[0]
    def __repr__(self):
        return repr(list(self))

def FindComponent(Parent, Name):
  for Component in Parent.Components:
    if Component.Name == Name:
        return Component

def ObjectInit(self, *args, **keywords):
    TObject.__init__(self, PyVcl.CallMethod(self._type, "Create", *args), **keywords)

class VclTypes:
    def __getattr__(self, name):
        NewType = type(name, (TObject,), dict(__init__=ObjectInit, _type=PyVcl.FindClass(name)))
        object.__setattr__(self, name, NewType)
        return NewType
    PropertyError = PyVcl.PropertyError
    VclError = PyVcl.VclError
    TObject = TObject

PropertyError = PyVcl.PropertyError
VclError = PyVcl.VclError
vcl = VclTypes()
sys.modules["vcl"] = vcl
