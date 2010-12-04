import vcl

class SimpleDialog(vcl.TForm):
    def __init__(self, ShowCancel=True, **keywords):
        vcl.TForm.__init__(self, None, **keywords)
        self.Position = "poMainFormCenter"
        self.BorderStyle = "bsDialog"
        self.BorderIcons = "biSystemMenu"
        self.panel = vcl.TPanel(None, Parent=self, BevelOuter="bvNone", Width=self.ClientWidth, Height=self.ClientHeight - 40, Caption="", Anchors="akLeft,akTop,akRight,akBottom")
        self.button1 = vcl.TButton(None, Parent=self, Caption = "OK", Anchors = "akRight,akBottom", Default=True, OnClick=self.OnOk, Top=self.ClientHeight-32, Left=self.ClientWidth-176)
        if ShowCancel:
            self.button2 = vcl.TButton(None, Parent=self, Caption="Cancel", Anchors="akRight,akBottom", ModalResult=1, Cancel=True, Top=self.ClientHeight-32, Left=self.ClientWidth-88)
        else:
            self.button2 = None
            self.button1.Cancel = True
            self.button1.Anchors = "akLeft,akRight,akBottom"
            self.button1.Left = (self.ClientWidth - self.button1.Width) / 2
        self.OnShow = self.FormOnShow

    def FormOnShow(self, sender):
        self.button1.TabOrder = 100
        if self.button2:
            self.button2.TabOrder = 101
        pass

    def OnOk(self, sender):
        self.Close()

    def __setattr__(self, name, value):
        try:
            vcl.TForm.__setattr__(self, name, value)
        except: #PropertyError:
            object.__setattr__(self, name, value)

