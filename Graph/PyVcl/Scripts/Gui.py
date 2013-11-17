import vcl

class DfmForm:
  def __init__(self, Form):
    object.__setattr__(self, "Form", Form)
    
  def __getattr__(self, Key):
    try:
      return getattr(self.Form, Key)
    except:
      Component = self.Form.FindComponent(Key)
      if Component:
        object.__setattr__(self, Key, Component)
        return Component
      raise  
    
  def __setattr__(self, Key, Value):
    try:
      setattr(self.Form, Key, Value)
    except: #PropertyError:
      object.__setattr__(self, Key, Value)
    

def LoadDfmFile(FileName):
    Stream = vcl.TFileStream(FileName, 0x20)
    Stream2 = vcl.TMemoryStream()
    vcl.ObjectTextToBinary(Stream, Stream2)
    Stream2.Position = 0
    Form = vcl.TForm(None)
    Stream2.ReadComponent(Form)
    return DfmForm(Form)
