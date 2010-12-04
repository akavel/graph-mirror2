import PyVcl
import sys

class Method:
    def __init__(self, handle, name):
        self._handle = handle
        self._name = name

    def __call__(self, *args):
        Args = tuple([x._handle if "_handle" in dir(x) else x for x in args])
        result = PyVcl.CallMethod(self._handle, self._name, Args)
        if result[1] == 7:  # An object
            return TObject(result[0], owned=False) if result[0] != 0 else None
        return result[0]

class IndexedProperty:
    def __init__(self, handle, name):
        self._handle = handle
        self._name = name

    def __getitem__(self, index):
        result = PyVcl.GetProperty(self._handle, self._name, index)
        if result[1] == 7:
            return TObject(result[0], owned=False)
        return result[0]

    def __iter__(self):
        i = 0
        try:
            while True:
                yield self[i]
                i = i + 1
        except IndexError:
            pass

    def __repr__(self):
        return repr(list(self))

class TObject(object):
    def __init__(self, handle, owned=True, Parent=None, **keywords):
        object.__setattr__(self, "_handle", handle)
        object.__setattr__(self, "_owned", owned)
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
        if result[1] == 20: #indexed property
            return IndexedProperty(self._handle, name)
        return result[0]
    def __setattr__(self, name, value):
        PyVcl.SetProperty(self._handle, name, value._handle if isinstance(value, TObject) else value) # Handle objects as values
    def __repr__(self):
        try:
            return "<object '%s' of type '%s' instance of %s.%s>" % (self.Name, self.ClassName(), self.__class__.__module__, self.__class__.__name__)
        except:#PyVcl.PropertyError:
            return "<object of type '%s' instance of %s.%s>" % (self.ClassName(), self.__class__.__module__, self.__class__.__name__)

    def __dir__(self):
        return list(self.__dict__.keys()) + PyVcl.GetPropertyList(self._handle)

def ObjectInit(self, *args, handle=0, **keywords):
    Args = tuple([x._handle if "_handle" in dir(x) else x for x in args])
    TObject.__init__(self, handle if handle != 0 else PyVcl.CallMethod(self._type, "Create", Args)[0], owned = (handle == 0), **keywords)

class VclTypes:
    VclError = PyVcl.VclError
    TObject = TObject
    Application = TObject(handle=PyVcl.Application, owned=False)
    Clipboard = TObject(handle=PyVcl.Clipboard, owned=False)
    Mouse = TObject(handle=PyVcl.Mouse, owned=False)
    Screen = TObject(handle=PyVcl.Screen, owned=False)

    def __getattr__(self, name):
        NewType = type(name, (TObject,), dict(__init__=ObjectInit, _type=PyVcl.FindClass(name)))
        object.__setattr__(self, name, NewType)
        return NewType

    def TextToShortCut(self, S):
        return PyVcl.CallFunction("TextToShortCut", (S,))

    def ShortCutToText(self, ShortCut):
        return PyVcl.CallFunction("ShortCutToText", (ShortCut,))

VclError = PyVcl.VclError
vcl = VclTypes()
sys.modules["vcl"] = vcl
