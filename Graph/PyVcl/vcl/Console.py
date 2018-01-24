import vcl
import sys
import __main__
   
class ConsoleForm:
  def __init__(self):
    self.LastIndex = 0 # Indicates position after prompt, i.e. start of command
    self.Command = ""
    self.TextCache = [""]
    self.CacheIndex = 0
    self.IndentLevel = 0
    
    self.Form = vcl.TForm(None, OnClose=self.Close, Width=vcl.Screen.Width//2, Height=vcl.Screen.Height//3) 
    self.Form.Caption = "Python interpreter"
    self.RichEdit = vcl.TRichEdit(None, Parent=self.Form, Align="alClient", ScrollBars="ssVertical", WantTabs=True,
      OnKeyDown=self.KeyDown, OnKeyPress=self.KeyPress)      
    self.RichEdit.Font.Name = "Courier New"
    self.RichEdit.Font.Size = 10
    
    self.PopupMenu = vcl.TPopupMenu(None)
    self.PopupMenu.Items.Add(vcl.TMenuItem(self.PopupMenu, Caption="Clear", _owned=False, OnClick=self.Clear))
    self.Form.PopupMenu = self.PopupMenu

    self.Form.Canvas.Font.Assign(self.RichEdit.Font)
    CharWidth = (self.Form.Canvas.TextWidth("A") * 15) // 22
    self.RichEdit.Paragraph.TabCount = 22
    x = CharWidth * 4
    for I in range(0, 22): 
      x += CharWidth * 2
#      self.RichEdit.Paragraph.Tab[I] = x
    
    self.WritePrompt()   
    self.stdout = sys.stdout
    sys.stdout = self
  
  def write(self, Str):
    self.WriteText(Str, 0)
    
  def flush(self):
    pass
  
  def Close(self, Sender, Action):
    sys.stdout = self.stdout
    
  def Clear(self, Sender):
    self.RichEdit.Clear()
    self.LastIndex = 0
    self.PromptIndex = 0
    self.WritePrompt()
    
  def WriteText(self, Str, Color):
    OldSelStart = self.RichEdit.SelStart
    self.RichEdit.SelStart = self.PromptIndex
    self.RichEdit.SelAttributes.Color = Color
    self.RichEdit.SelText = Str
    IndexChange = self.RichEdit.SelStart - self.PromptIndex
    self.LastIndex += IndexChange
    self.PromptIndex = self.RichEdit.SelStart
    self.RichEdit.SelStart = OldSelStart + IndexChange
    self.RichEdit.SelLength = 0
  
  def HandlePaste(self):
    if self.RichEdit.SelStart >= self.LastIndex:
      Lines = vcl.Clipboard.AsText.split("\r\n")
      for Str in Lines:
        self.RichEdit.SelText = Str
        if len(Lines) > 1:
          self.HandleNewLine()
    
  def KeyDown(self, Sender, Key, Shift):
    if (Shift == {"ssCtrl"} and Key.Value == ord("V")) or (Shift == {"ssShift"} and Key.Value == 0x2D): #0x2D=VK_INSERT
      self.HandlePaste()
      Key.Value = 0
    elif Shift == {"ssCtrl"} and Key.Value == ord("C"):
      if self.RichEdit.SelLength == 0:
        self.KeyboardInterrupt()
    if (Shift == {"ssCtrl"} and Key.Value == ord("X")) or (Shift == {"ssShift"} and Key.Value == 0x2E): #0x2E=VK_DELETE
      if self.RichEdit.SelStart < self.LastIndex:
        Key.Value = 0        
    elif Key.Value == 0x24: #VK_HOME
      Pos2 = 0
      while Pos2 != -1:
        Pos = Pos2 + 4
        Pos2 = self.RichEdit.FindText(">>> ", Pos, self.RichEdit.SelStart - Pos + 4, "") 
      PosEnd = self.RichEdit.SelStart
      self.RichEdit.SelStart = Pos
      if Shift == {"ssShift"}:
        self.RichEdit.SelLength = PosEnd - Pos
      Key.Value = 0  
    elif Key.Value == 0x08: #Backspace  
      if self.RichEdit.SelStart <= self.LastIndex:
        Key.Value = 0
    elif Key.Value == 0x2E: #Delete
      if self.RichEdit.SelStart < self.LastIndex:
        Key.Value = 0      
    elif Key.Value == 0x0D: #VK_RETURN
      self.HandleNewLine()
      Key.Value = 0
    elif not Shift and Key.Value == 0x1B: #VK_ESCAPE
      self.SetUserString("");
    elif not Shift and Key.Value == 0x26: #VK_UP
      if self.RichEdit.ActiveLineNo == self.RichEdit.Lines.Count - 1:
        if self.CacheIndex > 0:
          if self.CacheIndex == len(self.TextCache) - 1:
            self.TextCache[-1] = self.GetUserString()
          self.CacheIndex -= 1 
          self.SetUserString(self.TextCache[self.CacheIndex])
        Key.Value = 0
    elif not Shift and Key.Value == 0x28: #VK_DOWN
      if self.RichEdit.ActiveLineNo == self.RichEdit.Lines.Count - 1:
        if self.CacheIndex < len(self.TextCache) - 1:
          self.CacheIndex += 1
          self.SetUserString(self.TextCache[self.CacheIndex])
        Key.Value = 0    
        
  def KeyPress(self, Sender, Key):
    if self.RichEdit.SelStart < self.LastIndex:
      Key.Value = "\0"
  
  def KeyboardInterrupt(self):
    self.RichEdit.SelStart = 0x7FFFFFFF
    self.RichEdit.SelText = "\r"
    self.PromptIndex = self.RichEdit.SelStart
    self.WriteText("KeyboardInterrupt\r\n", 0xFF)
    self.WritePrompt()
    self.Command = ""
    self.IndentLevel = 0
    self.CacheIndex = len(self.TextCache) - 1
  
  def ShowException(self, StartLine):
    import traceback
    TraceBack = traceback.format_exception(*sys.exc_info())
    self.WriteText(TraceBack[0], 0xFF)
    for Str in TraceBack[StartLine:]: self.WriteText(Str, 0xFF)
    self.WritePrompt()
    self.Command = ""
    self.IndentLevel = 0

  def HandleNewLine(self):  
    Str = self.GetUserString()
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
      Code = code.compile_command(self.Command, "<console>")
      if Code:
        exec(Code, __main__.__dict__, __main__.__dict__)
        self.WritePrompt()
        self.Command = ""
        self.IndentLevel = 0
      else:  
        self.WritePrompt("... ")
        self.Command += "\n"
        if Str != "" and Str[-1] == ":":
          self.IndentLevel += 1
    except SystemExit:
      vcl.Application.Terminate()
    except SyntaxError:
      self.ShowException(6)
    except Exception:
      self.ShowException(2)
    self.RichEdit.SelText = "\t" * self.IndentLevel  
    
  def WritePrompt(self, Str=">>> "):
    self.RichEdit.SelStart = 0x7FFFFFFF
    self.PromptIndex = self.RichEdit.SelStart
    self.RichEdit.SelAttributes.Color = 0
    self.RichEdit.SelText = Str
    self.LastIndex = self.RichEdit.SelStart
  
  def SetUserString(self, Str):
    self.RichEdit.SelStart = self.LastIndex
    self.RichEdit.SelLength = 0x7FFFFFFF
    self.RichEdit.SelText = Str
  
  def GetUserString(self):
    return self.RichEdit.Text.rstrip().replace('\r', "")[self.LastIndex:]
  
  def ShowModal(self):
    return self.Form.ShowModal()
        
if __name__ == "__main__":
  Form = ConsoleForm()
  print(sys.version, "on", sys.platform)
  print('Type "help", "copyright", "credits" or "license" for more information.')
  Form.ShowModal()
