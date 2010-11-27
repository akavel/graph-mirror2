import Graph
import vcl
import Gui

def Execute(Sender):
    import math
    yList = sorted(p[1] for p in Graph.Selected.Points)
    N = len(yList)
    Sum = math.fsum(yList)
    Mean = Sum / N

    Form = Gui.SimpleDialog(Caption="Statistics", Height=220, ShowCancel=False)
    Lines = "Statistics for " + Graph.Selected.LegendText + ":"
    Lines += "\n  Mean:\t\t" + str(Mean)
    Lines += "\n  Sum:\t\t" + str(Sum)
    Lines += "\n  Median:\t\t" + str(yList[N//2] if N % 2 else (yList[N//2-1] + yList[N//2]) / 2)
    Lines += "\n  Standard deviation: " + str( math.sqrt(math.fsum([(y-Mean)**2 for y in yList]) / N))
    Lines += "\n  1st quartile:\t" + str(yList[N//4] if (N//2) % 2 else (yList[N//4-1] + yList[N//4]) / 2)
    Lines += "\n  3rd quartile:\t" + str(yList[math.ceil(N/2)+N//4] if (N//2) % 2 else (yList[math.ceil(N/2)+N//4-1] + yList[math.ceil(N/2)+N//4]) / 2)
    Lines += "\n  Min:\t\t" + str(min(yList))
    Lines += "\n  Max:\t\t" + str(max(yList))
    Memo = vcl.TMemo(Parent = Form.panel, ReadOnly=True, Align="alClient", Color=-16777201, WantReturns=False)
    Memo.Font.Size = 10
    Memo.Lines.Text = Lines
    Form.ShowModal()

def OnSelect(Item):
    Action.Enabled = Item.__class__ is Graph.TPointSeries

Action = vcl.TAction(Name="Statistics", Caption="Statistics", OnExecute=Execute, Hint="Statistics on a point series")
Graph.AddActionToMainMenu(Action)
Graph.OnSelect.append(OnSelect)
