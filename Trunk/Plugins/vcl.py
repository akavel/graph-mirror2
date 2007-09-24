import PyVcl

class TObject(object):
    def __init__(self, handle=0, owned=True, **keywords):
        object.__setattr__(self, "_handle", handle)
        object.__setattr__(self, "_owned", owned)
        for name, value in keywords.iteritems():
            PyVcl.SetProperty(handle, name, value)
        
    def __del__(self):
        if self._owned:
            PyVcl.DeleteObject(self._handle)
    def __getattr__(self, name):
        return PyVcl.GetProperty(self._handle, name)
    def __setattr__(self, name, value):
        PyVcl.SetProperty(self._handle, name, value)

class TForm(TObject):
    def __init__(self, handle = 0, **keywords):
        TObject.__init__(self, PyVcl.CreateObject("TForm") if handle == 0 else handle, handle == 0, **keywords)
    def __setattr__(self, name, value):
        try:
            PyVcl.SetProperty(self._handle, name, value)
        except PyVcl.PropertyError:
            object.__setattr__(self, name, value)
    def ShowModal(self):
        PyVcl.CallMethod(self._handle, "ShowModal")
    def Close(self):
        PyVcl.CallMethod(self._handle, "Close")

class TLabel(TObject):
    def __init__(self, Parent, **keywords):
        TObject.__init__(self, PyVcl.CreateObject("TLabel"), Parent = Parent._handle, **keywords)

class TEdit(TObject):
    def __init__(self, Parent, **keywords):
        TObject.__init__(self, PyVcl.CreateObject("TEdit"), Parent = Parent._handle, **keywords)

class TButton(TObject):
    def __init__(self, Parent, **keywords):
        TObject.__init__(self, PyVcl.CreateObject("TButton"), Parent = Parent._handle, **keywords)

import GraphImpl
Form1 = TForm(GraphImpl.form1)