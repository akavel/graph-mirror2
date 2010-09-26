import PyVcl

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
        if result[1] == 7:  # An object
            return TObject(result[0], owned=False) if result[0] != 0 else None
        return result[0]
    def __setattr__(self, name, value):
        PyVcl.SetProperty(self._handle, name, value._handle if isinstance(value, TObject) else value, self) # Handle objects as values

    def __repr__(self):
        try:
            return "<object '%s' of type '%s' instance of %s.%s>" % (self.Name, self.ClassName, self.__class__.__module__, self.__class__.__name__)
        except PyVcl.PropertyError:
            return "<object of type '%s' instance of %s.%s>" % (self.ClassName, self.__class__.__module__, self.__class__.__name__)

class TForm(TObject):
    def __init__(self, handle = 0, **keywords):
        TObject.__init__(self, PyVcl.CreateObject("TForm") if handle == 0 else handle, **keywords)
    def __setattr__(self, name, value):
        try:
            if  "_handle" in self.__dict__:
                PyVcl.SetProperty(self._handle, name, value, self)
            else:
                object.__setattr__(self, name, value)
        except PropertyError:
            object.__setattr__(self, name, value)
    def ShowModal(self):
        PyVcl.CallMethod(self._handle, "ShowModal")
    def Close(self):
        PyVcl.CallMethod(self._handle, "Close")

def CreateObject(type, **keywords):
    return TObject(PyVcl.CreateObject(type), **keywords)

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

def ObjectInit(self, Parent=None, **keywords):
    TObject.__init__(self, PyVcl.CreateObject(self.__class__.__name__), Parent = Parent._handle if Parent else 0, **keywords)

class VclTypes:
    def __getattr__(self, name):
        return type(name, (TObject,), dict(__init__=ObjectInit))
    TForm = TForm
    TAction = TAction
    PropertyError = PyVcl.PropertyError
    VclError = PyVcl.VclError

PropertyError = PyVcl.PropertyError
VclError = PyVcl.VclError
vcl = VclTypes()
