import Graph
import vcl
import Gui
class CircleDialog(Gui.SimpleDialog):
    def __init__(self, CircleItem=None):
        Gui.SimpleDialog.__init__(self)
        self.CircleItem = CircleItem
        self.Caption = "Insert circle"
        self.Height = 170
        self.Width = 330
        CircleData = CircleItem.PluginData["CircleExample"] if CircleItem else ("","","")

        self.label1 = vcl.TLabel(None, Parent = self.panel, Caption = "Radius:", Top = 12, Left = 8)
        self.label2 = vcl.TLabel(None, Parent = self.panel, Caption = "Center:", Top = 44, Left = 8)
        self.label3 = vcl.TLabel(None, Parent = self.panel, Caption = "X:", Top = 44, Left = 50)
        self.label4 = vcl.TLabel(None, Parent = self.panel, Caption = "Y:", Top = 44, Left = 180)
        self.label5 = vcl.TLabel(None, Parent = self.panel, Caption = "Width:", Top = 76, Left = 8)
        self.label6 = vcl.TLabel(None, Parent = self.panel, Caption = "Color:", Top = 76, Left = 180)
        self.edit1 = vcl.TEdit(None, Parent = self.panel, Top = 8, Left = 70, Width = 100, Text = CircleData[0])
        self.edit2 = vcl.TEdit(None, Parent = self.panel, Top = 40, Left = 70, Width = 100, Text = CircleData[1])
        self.edit3 = vcl.TEdit(None, Parent = self.panel, Top = 40, Left = 210, Width = 100, Text = CircleData[2])
        self.edit4 = vcl.TEdit(None, Parent = self.panel, Top = 72, Left = 70, Width = 100, Text = str(CircleItem.Size) if CircleItem else "1")
        self.colorbox = vcl.TExtColorBox(None, Parent = self.panel, Top = 72, Left = 210, Width = 100, Selected = CircleItem.Color if CircleItem else 0x0000FF)

    def OnOk(self, sender):
        CircleData = (self.edit1.Text, self.edit2.Text, self.edit3.Text)
        Func = Graph.TParFunc("{1}+{0}*cos t".format(*CircleData), "{2}+{0}*sin t".format(*CircleData))
        Func.PluginData["CircleExample"] = CircleData
        Func.LegendText = "Circle: radius={0}, center=({1},{2})".format(*CircleData)
        Func.Color = self.colorbox.Selected
        Func.Size = int(self.edit4.Text)

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

Action = Graph.CreateAction(Name="Circle", Caption="Insert circle...", OnExecute=execute_action, Hint="Create circle from center and radius", ShortCut="Ctrl+Shift+C", IconFile="Circle.bmp")
Graph.AddActionToMainMenu(Action)
Graph.OnEdit.append(OnEdit)
