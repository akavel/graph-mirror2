# This plugin adds a menu item to the Plugins menu and context menu.
# This will show a dialog that can calculate the srurface area of revolution of the selected function around the x- and y-axis
import Graph
import vcl
import Gui

class RevolutionDialog(Gui.SimpleDialog):
    def __init__(self):
        Gui.SimpleDialog.__init__(self, Caption="Area of revolution", Width=275, Height=180, ShowCancel=False)
        self.button1.Caption = "Close"
        self.Label1 = vcl.TLabel(None, Parent = self.panel, Caption = "From:", Top = 12, Left = 8)
        self.Label2 = vcl.TLabel(None, Parent = self.panel, Caption = "To:", Top = 44, Left = 8)
        self.Label5 = vcl.TLabel(None, Parent = self.panel, Caption = "Area:", Top = 76, Left = 8)
        self.Edit1 = vcl.TEdit(None, Parent = self.panel, Top = 8, Left = 50, Width = 100, OnChange = self.EditChange)
        self.Edit2 = vcl.TEdit(None, Parent = self.panel, Top = 40, Left = 50, Width = 100, OnChange = self.EditChange)
        self.Edit3 = vcl.TEdit(None, Parent = self.panel, Top = 72, Left = 50, Width = 100, ReadOnly = True, Color = 0xFF00000F)
        self.RadioGroup1 = vcl.TRadioGroup(None, Parent = self.panel, Top = 8, Left = 160, Width = 85, Height = 60, Caption="Rotate around", OnClick = self.EditChange)
        self.RadioGroup1.Items.Text = "X-axis\nY-Axis"
        self.RadioGroup1.ItemIndex = 0

    def EditChange(self, Sender):
        try:
            import math
            From = Graph.Eval(self.Edit1.Text)
            To = Graph.Eval(self.Edit2.Text)
            Dif = Graph.Selected.MakeDifFunc()
            if isinstance(Graph.Selected, Graph.TStdFunc):
                RevolutionFunc = Graph.TStdFunc("(%s) * sqrt(1+sqr(%s))" % ("x" if self.RadioGroup1.ItemIndex else Graph.Selected.Text, Dif.Text))
            elif isinstance(Graph.Selected, Graph.TParFunc):
                RevolutionFunc = Graph.TParFunc("t", "(%s) * sqrt(sqr(%s)+sqr(%s))" % (Graph.Selected.xText if self.RadioGroup1.ItemIndex else Graph.Selected.yText, Dif.xText, Dif.yText))
            elif isinstance(Graph.Selected, Graph.TPolFunc):
                RevolutionFunc = Graph.TParFunc("t", "(%s) * %s t * sqrt(sqr(%s)+sqr(%s))" % (Graph.Selected.Text, "cos" if self.RadioGroup1.ItemIndex else "sin", Graph.Selected.Text, Dif.Text))
            else:
                return # Error: Unknown function
            self.Edit3.Text = str(2*math.pi*RevolutionFunc.CalcArea(From, To))
        except Graph.EFuncError:
            self.Edit3.Text = "" # What to do on error?

def Execute(Sender):
    import math
    Form = RevolutionDialog()
    Form.ShowModal()

def OnSelect(Item):
    Action.Enabled = isinstance(Item, Graph.TBaseFuncType)

Action = Graph.CreateAction(Caption="Revolution", OnExecute=Execute, Hint="Area for surface of revolution.")
Graph.AddActionToMainMenu(Action)
Graph.AddActionToContextMenu(Action)
Graph.OnSelect.append(OnSelect)
