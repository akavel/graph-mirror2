import Graph
from cmath import exp, pi, phase
from math import log10

# Number of samples; Must be a power of 2, i.e. 512, 1024, 2048, etc.
Samples = 8192
 
def fft(x):
    N = len(x)
    if N <= 1: return x
    even = fft(x[0::2])
    odd =  fft(x[1::2])
    T= [exp(-2j*pi*k/N)*odd[k] for k in range(N//2)]
    return [even[k] + T[k] for k in range(N//2)] + [even[k] - T[k] for k in range(N//2)]
    
def Execute(Sender):
  Start = Graph.Axes.xAxis.Min
  Stop = Graph.Axes.xAxis.Max
  Step = (Stop - Start) / Samples
  y = [Graph.Selected.Eval(Start + n * Step)[1] for n in range(Samples)]
  Y = fft(y)
  
  P = Graph.TPointSeries()
  P.LegendText = "FFT"
  P.LineSize = 1
  P.Size = 0
  Graph.FunctionList.append(P)
  for i in range(len(Y)): 
    L=("%.6F" % (i * 1 / (Stop - Start)), "%.6F%+.6Fi" % (Y[i].real, Y[i].imag), "", "")
    P.PointData.append(L)
  
#  P = Graph.TPointSeries()
#  P.LegendText = "FFT real"
#  P.LineSize = 1
#  P.Size = 0
#  P.Points = [(i * 1 / (Stop - Start), Y[i].real) for i in range(len(Y))]
#  Graph.FunctionList.append(P)
#
#  P = Graph.TPointSeries()
#  P.LegendText = "FFT imag"
#  P.LineSize = 1
#  P.Size = 0
#  P.Points = [(i * 1 / (Stop - Start), Y[i].imag) for i in range(len(Y))]
#  Graph.FunctionList.append(P)
#
  P = Graph.TPointSeries()
  P.LegendText = "FFT abs"
  P.LineSize = 1
  P.Size = 0
  P.Points = [(i * 1 / (Stop - Start), abs(Y[i])) for i in range(len(Y))]
  Graph.FunctionList.append(P)
  
  P = Graph.TPointSeries()
  P.LegendText = "FFT dB"
  P.LineSize = 1
  P.Size = 0
  P.Points = [(i * 1 / (Stop - Start), 10 * log10(abs(Y[i]))) for i in range(len(Y))]
  Graph.FunctionList.append(P)
  
  P = Graph.TPointSeries()
  P.LegendText = "FFT phase"
  P.LineSize = 1
  P.Size = 0
  P.Points = [(i * 1 / (Stop - Start), 180 * phase(Y[i]) / pi) for i in range(len(Y))]
  Graph.FunctionList.append(P)
  
  Graph.Redraw()

def OnSelect(Item):
    Action.Enabled = isinstance(Item, Graph.TStdFunc)
    ContextMenuItem.Visible = Action.Enabled

Action = Graph.CreateAction(Caption="FFTl", OnExecute=Execute, Hint="Calculate FFT for selected function.")
Graph.AddActionToMainMenu(Action)
ContextMenuItem = Graph.AddActionToContextMenu(Action)
Graph.OnSelect.append(OnSelect)
