import Graph
import GraphImpl
import vcl

class CircleDialog():
    def __init__(self):
        self.Form = vcl.TForm(Caption = "Insert circle", Height = 140, Position = 6, BorderStyle = 1, BorderIcons = "biSystemMenu")

        self.label1 = vcl.TLabel(self.Form, Caption = "Radius:", Top = 12, Left = 8)
        self.label2 = vcl.TLabel(self.Form, Caption = "Center:", Top = 44, Left = 8)
        self.label3 = vcl.TLabel(self.Form, Caption = "X:", Top = 44, Left = 50)
        self.label4 = vcl.TLabel(self.Form, Caption = "Y:", Top = 44, Left = 180)
        self.edit1 = vcl.TEdit(self.Form, Top = 8, Left = 70, Width = 100)
        self.edit2 = vcl.TEdit(self.Form, Top = 40, Left = 70, Width = 100)
        self.edit3 = vcl.TEdit(self.Form, Top = 40, Left = 200, Width = 100)

        self.button1 = vcl.TButton(self.Form, Caption = "OK", Top = 80, Left = 20, Default = True)
        self.button2 = vcl.TButton(self.Form, Caption = "Cancel", Top = 80, Left = 120, ModalResult=1, Cancel = True)
        self.button1.OnClick = self.OnOk

    def OnOk(self):
        GraphImpl.CreateParametricFunction(self.edit2.Text + "+" + self.edit1.Text + "*cos t", self.edit3.Text + "+" + self.edit1.Text + "*sin t")
        self.Form.Close()

def execute_action():
    d = CircleDialog()
    d.Form.ShowModal()

action = Graph.Action("Create circle...", execute_action, icon="Graph.bmp", hint="Create circle from center and radius", shortcut="Ctrl+Shift+C")
