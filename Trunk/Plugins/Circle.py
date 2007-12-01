import Graph
import GraphImpl
import vcl

class CircleDialog(vcl.SimpleDialog):
    def __init__(self):
        vcl.SimpleDialog.__init__(self)
        self.Caption = "Insert circle"
        self.Height = 130

        self.label1 = vcl.TLabel(self, Caption = "Radius:", Top = 12, Left = 8)
        self.label2 = vcl.TLabel(self, Caption = "Center:", Top = 44, Left = 8)
        self.label3 = vcl.TLabel(self, Caption = "X:", Top = 44, Left = 50)
        self.label4 = vcl.TLabel(self, Caption = "Y:", Top = 44, Left = 180)
        self.edit1 = vcl.TEdit(self, Top = 8, Left = 70, Width = 100)
        self.edit2 = vcl.TEdit(self, Top = 40, Left = 70, Width = 100)
        self.edit3 = vcl.TEdit(self, Top = 40, Left = 200, Width = 100)

    def OnOk(self, sender):
        GraphImpl.CreateParametricFunction(self.edit2.Text + "+" + self.edit1.Text + "*cos t", self.edit3.Text + "+" + self.edit1.Text + "*sin t")
        self.Close()

def execute_action(action):
    d = CircleDialog()
    d.ShowModal()

action = vcl.TAction(Name="CircleAction", Caption="Create circle...", OnExecute=execute_action, Hint="Create circle from center and radius", ShortCut="Ctrl+Shift+C")
