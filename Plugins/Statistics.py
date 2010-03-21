import Graph
import vcl

def Execute(Sender):
    import math
    yList = sorted(p[1] for p in Graph.Selected.Points)
    N = len(yList)
    Sum = math.fsum(yList)
    Mean = Sum / N
    print("Statistics for " + Graph.Selected.LegendText + ":")
    print("  Mean:", Mean)
    print("  Sum:", Sum)
    print("  Median:", yList[N//2] if N % 2 else (yList[N//2-1] + yList[N//2]) / 2)
    print("  Standard deviation:", math.sqrt(math.fsum([(y-Mean)**2 for y in yList]) / N))
    print("  1st quartile:", yList[N//4] if (N//2) % 2 else (yList[N//4-1] + yList[N//4]) / 2)
    print("  3rd quartile:", yList[math.ceil(N/2)+N//4] if (N//2) % 2 else (yList[math.ceil(N/2)+N//4-1] + yList[math.ceil(N/2)+N//4]) / 2)
    print("  Min:", min(yList))
    print("  Max:", max(yList))

def OnSelect(Item):
    Action.Enabled = Item.__class__ is Graph.TPointSeries


Action = vcl.TAction(Name="Statistics", Caption="Statistics", OnExecute=Execute, Hint="Statistics on a point series")
Graph.OnSelect.append(OnSelect)

