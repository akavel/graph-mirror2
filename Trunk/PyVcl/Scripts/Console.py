from PyVcl import vcl

class ConsoleForm:
  def __init__(self):
    self.Form = vcl.CreateForm()
    self.Form.Caption = "Python interpreter"
    self.RichEdit = vcl.TRichEdit(None, Parent=self.Form, Align="alClient")
    self.RichEdit.Font.Name = "Courier New"
    self.RichEdit.Font.Size = 10
    self.WritePrompt()
    
  def WritePrompt(self, Str=">>> "):
    self.RichEdit.SelText = Str
    
    
if __name__ == "__main__":
  Form = ConsoleForm()
  vcl.Application.Run()