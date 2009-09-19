import Graph
import vcl

class CircleDialog(vcl.SimpleDialog):
    def __init__(self, CircleItem=None):
        self.CircleItem = CircleItem
        vcl.SimpleDialog.__init__(self)
        self.Caption = "Insert circle"
        self.Height = 130
        CircleData = CircleItem.PluginData["CircleExample"] if CircleItem else ("","","")

        self.label1 = vcl.TLabel(self, Caption = "Radius:", Top = 12, Left = 8)
        self.label2 = vcl.TLabel(self, Caption = "Center:", Top = 44, Left = 8)
        self.label3 = vcl.TLabel(self, Caption = "X:", Top = 44, Left = 50)
        self.label4 = vcl.TLabel(self, Caption = "Y:", Top = 44, Left = 180)
        self.edit1 = vcl.TEdit(self, Top = 8, Left = 70, Width = 100, Text = CircleData[0])
        self.edit2 = vcl.TEdit(self, Top = 40, Left = 70, Width = 100, Text = CircleData[1])
        self.edit3 = vcl.TEdit(self, Top = 40, Left = 200, Width = 100, Text = CircleData[2])

    def OnOk(self, sender):
        Func = Graph.CreateParFunc(self.edit2.Text + "+" + self.edit1.Text + "*cos t", self.edit3.Text + "+" + self.edit1.Text + "*sin t")
        Func.PluginData["CircleExample"] = (self.edit1.Text, self.edit2.Text, self.edit3.Text)
        if self.CircleItem:
            Graph.FunctionList[Graph.FunctionList.index(self.CircleItem)] = Func
        else:
            Graph.FunctionList.append(Func)
        Graph.Redraw()
        self.Close()

def execute_action(action):
    d = CircleDialog()
    d.ShowModal()

def OnEdit(Item):
    if "CircleExample" in Item.PluginData:
        d = CircleDialog(Item)
        d.ShowModal()
        return True

action = vcl.TAction(Name="CircleAction", Caption="Insert circle...", OnExecute=execute_action, Hint="Create circle from center and radius", ShortCut="Ctrl+Shift+C")
Graph.OnEdit.append(OnEdit)
