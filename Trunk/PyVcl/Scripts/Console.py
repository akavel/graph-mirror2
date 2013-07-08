from PyVcl import vcl

class ConsoleForm:
  def __init__(self):
    self.LastIndex = 0
    self.Command = ""
    self.TextCache = [""]
    self.IndentLevel = 0
    
    self.Form = vcl.TForm(None) 
    self.Form.Caption = "Python interpreter"
    self.RichEdit = vcl.TRichEdit(None, Parent=self.Form, Align="alClient", OnKeyDown=self.RichEditKeyDown)
    self.RichEdit.Font.Name = "Courier New"
    self.RichEdit.Font.Size = 10
    
    self.PopupMenu = vcl.TPopupMenu(None)
    self.PopupMenu.Items.Add(vcl.TMenuItem(self.PopupMenu, Caption="Clear", _owned=False, OnClick=self.Clear))
    self.Form.PopupMenu = self.PopupMenu
    self.WritePrompt()
    
  def Clear(self, Sender):
    self.RichEdit.Clear()
    self.LastIndex = 0
    self.PromptIndex = 0
    self.WritePrompt()
    
  def RichEditKeyDown(self, Sender, Key, Shift):
    if Key.Value == 0x0D: #VK_RETURN
      if Shift == "":
        self.HandleNewLine()
        Key = 0
        
  def HandleNewLine(self):
    Str = self.RichEdit.Text[self.LastIndex:].rstrip()
    self.RichEdit.SelStart = 0x7FFFFFFF
    self.RichEdit.SelText = "\r";
    self.PromptIndex = self.RichEdit.SelStart
    self.Command += Str
    
    if Str != "":
      self.TextCache[-1] = Str
      self.TextCache.append("")
    self.CacheIndex = len(self.TextCache) - 1 
    
    try:
      import code
      Code = code.compile_command(Str, "<console>")
      if Code:
        exec(Code)
        self.WritePrompt()
        self.Command = ""
        self.IndentLevel = 0
      else:  
        self.WritePromtp("... ")
        self.Command += "\n"
        if Str != "" and Str[-1] == ":":
          self.IndentLevel += 1
    except:
      import traceback
      traceback.print_exc()
      self.WritePrompt()
      self.Command = ""
      self.IndentLevel = 0
    self.RichEdit.SelText = "\t" * self.IndentLevel  
    
  def WritePrompt(self, Str=">>> "):
    self.RichEdit.SelText = Str
    self.LastIndex = self.RichEdit.SelStart
  
  def ShowModal(self):
    return self.Form.ShowModal()
    
if __name__ == "__main__":
  Form = ConsoleForm()
  Form.ShowModal()
