import fmx
import sys
   
class ConsoleForm:
  def __init__(self):
    self.LastIndex = 0 # Indicates position after prompt, i.e. start of command
    self.Command = ""
    self.TextCache = [""]
    self.CacheIndex = 0
    self.IndentLevel = 0
    
    self.Form = fmx.CreateForm()
    self.Form.OnClose = self.Close
    Size = fmx.Platform.GetScreenSize()[0]
    Scale = fmx.Platform.GetScreenScale()
    self.Form.Width = Size[0] // (2 * Scale)
    self.Form.Height = Size[1] // (3 * Scale)
    self.Form.Caption = "Python interpreter"
    
    self.Memo = fmx.TMemo(None, Parent=self.Form, Align="alClient", OnKeyDown=self.KeyDown, OnKeyUp=self.KeyUp)      
    self.Memo.Font.Family = "Courier New"
    self.Memo.Font.Size = 28
    self.Memo.StyledSettings = {'FontColor', 'Style', 'Size'}
    self.Memo.TextSettings.WordWrap = True;

    self.PopupMenu = fmx.TPopupMenu(None)
    self.MenuItem = fmx.TMenuItem(self.PopupMenu, Text="Clear", _owned=False, OnClick=self.Clear)
    self.PopupMenu.AddObject(self.MenuItem)
    self.Memo.PopupMenu = self.PopupMenu
    self.Memo.Lines.LineBreak = "\n"
    
    self.stdout = sys.stdout
    sys.stdout = self
    self.WritePrompt()   
  
  def write(self, Str):
    self.WriteText(Str)
    
  def flush(self):
    pass
  
  def Close(self, Sender, Action):
    sys.stdout = self.stdout
    
  def Clear(self, Sender):
    self.Memo.Lines.Clear()
    self.LastIndex = 0
    self.PromptIndex = 0
    self.WritePrompt()
    
  def WriteText(self, Str):
    Lines = Str.split("\n")
    self.Memo.Lines[self.Memo.Lines.Count - 1] = self.Memo.Lines[self.Memo.Lines.Count - 1] + Lines[0]
    for Line in Lines[1:]:
      self.Memo.Lines.Add(Line)  
  
  def HandlePaste(self):
    if self.Memo.SelStart >= self.LastIndex:
      Lines = fmx.Platform.GetClipboard().split("\n")
      for Str in Lines:
        self.SetSelText(Str)
        if len(Lines) > 1:
          self.HandleNewLine(False)
    
  def KeyDown(self, Sender, Key, KeyChar, Shift):
    if (Shift == {"ssCtrl"} and Key.Value == ord("V")) or (Shift == {"ssShift"} and Key.Value == 0x2D): #0x2D=VK_INSERT
      self.HandlePaste()
      Key.Value = 0
    elif Shift == {"ssCtrl"} and Key.Value == ord("C"):
      if self.Memo.SelLength == 0:
        self.KeyboardInterrupt()
    if (Shift == {"ssCtrl"} and Key.Value == ord("X")) or (Shift == {"ssShift"} and Key.Value == 0x2E): #0x2E=VK_DELETE
      if self.Memo.SelStart < self.LastIndex:
        Key.Value = 0        
    elif Key.Value == 0x24: #VK_HOME
      Pos = self.Memo.Text.rfind(">>> ", 0, self.Memo.SelStart) 
      if Pos != -1:
        PosEnd = self.Memo.SelStart
        self.Memo.SelStart = Pos + 4
        if Shift == {"ssShift"}:
          self.Memo.SelLength = PosEnd - Pos - 4
      Key.Value = 0  
    elif Key.Value == 0x08: #Backspace  
      if self.Memo.SelStart <= self.LastIndex:
        Key.Value = 0
    elif Key.Value == 0x2E: #Delete
      if self.Memo.SelStart < self.LastIndex:
        Key.Value = 0      
    elif Key.Value == 0x0D: #VK_RETURN
      self.HandleNewLine()
      Key.Value = 0
    elif not Shift and Key.Value == 0x1B: #VK_ESCAPE
      self.SetUserString("");
    elif not Shift and Key.Value == 0x26: #VK_UP
      if self.Memo.CaretPosition[0] == self.Memo.Lines.Count - 1:
        if self.CacheIndex > 0:
          if self.CacheIndex == len(self.TextCache) - 1:
            self.TextCache[-1] = self.GetUserString()
          self.CacheIndex -= 1 
          self.SetUserString(self.TextCache[self.CacheIndex])
        Key.Value = 0
    elif not Shift and Key.Value == 0x28: #VK_DOWN
      if self.Memo.CaretPosition[0] == self.Memo.Lines.Count - 1:
        if self.CacheIndex < len(self.TextCache) - 1:
          self.CacheIndex += 1
          self.SetUserString(self.TextCache[self.CacheIndex])
        Key.Value = 0    
    elif KeyChar.Value != "\0" and self.Memo.SelStart < self.LastIndex:
      KeyChar.Value = "\0"
 
  # Tab key triggers OnKeyUp but not KeyDown 
  def KeyUp(self, Sender, Key, KeyChar, Shift):
    if Key.Value == 9: #Tab
      self.SetSelText("\t")  
  
  def KeyboardInterrupt(self):
    self.Memo.SelStart = 0x7FFFFFFF
    self.Memo.Lines.Add("")
    self.PromptIndex = self.Memo.SelStart
    self.WriteText("KeyboardInterrupt\n")
    self.Command = ""
    self.IndentLevel = 0
    self.CacheIndex = len(self.TextCache) - 1
    self.WritePrompt()
  
  def ShowException(self, StartLine):
    import traceback
    TraceBack = traceback.format_exception(*sys.exc_info())
    self.WriteText(TraceBack[0])
    for Str in TraceBack[StartLine:]: self.WriteText(Str)
    self.Command = ""
    self.IndentLevel = 0
    self.WritePrompt()

  def HandleNewLine(self, AutoIndent=True):  
    Str = self.GetUserString()
    self.Memo.SelStart = 0x7FFFFFFF
    self.Memo.Lines.Add("")
    self.PromptIndex = self.Memo.SelStart
    self.Command += Str
    
    if Str != "":
      self.TextCache[-1] = Str
      self.TextCache.append("")
    self.CacheIndex = len(self.TextCache) - 1 
    
    try:
      import code
      Code = code.compile_command(self.Command, "<console>")
      if Code:
        exec(Code, globals(), globals())
        self.Command = ""
        self.IndentLevel = 0
        self.WritePrompt()
      else:  
        self.Command += "\n"
        if Str != "" and Str[-1] == ":":
          self.IndentLevel += 1
        self.WritePrompt("... ", AutoIndent)
    except SystemExit:
      fmx.Application.Terminate()
    except SyntaxError:
      self.ShowException(6)
    except Exception:
      self.ShowException(2)
    
  def WritePrompt(self, Str=">>> ", AutoIndent=True):
    Lines = self.Memo.Lines
    if Lines.Count == 0 or len(Lines[Lines.Count-1]) > 0:
      Lines.Add("")
    self.PromptIndex = len(Lines.Text) # Indicates position before prompt
    Lines[Lines.Count - 1] = Str 
    self.LastIndex = len(Lines.Text)
    if AutoIndent:
      Lines[Lines.Count - 1] = Str + "\t" * self.IndentLevel  
    self.Memo.SelStart = 0x7FFFFFFF
  
  def SetUserString(self, Str):
    self.Memo.Text = self.Memo.Text[:self.LastIndex] + Str
    self.Memo.SelStart = 0x7FFFFFFF
  
  def GetUserString(self):
    return self.Memo.Text.rstrip()[self.LastIndex:]
  
  def SetSelText(self, Str):
    if self.Memo.SelStart >= self.LastIndex:
      S = self.Memo.Text
      SelStart = self.Memo.SelStart
      self.Memo.Text = S[:self.Memo.SelStart] + Str + S[self.Memo.SelStart + self.Memo.SelLength:]
      self.Memo.SelLength = 0
      self.Memo.SelStart = SelStart + len(Str)
  
  def ShowModal(self):
    return self.Form.ShowModal()
        
if __name__ == "__main__":
  Form = ConsoleForm()
  Form.ShowModal()
