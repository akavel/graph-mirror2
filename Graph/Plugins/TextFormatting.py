import Graph

def FormatPolar(S, Decimals=None):
  return Graph.FormatNumber(Graph.EvalComplex(S), Decimals, Graph.cfPolar)
  
def FormatRectangular(S, Decimals=None):
  return Graph.FormatNumber(Graph.EvalComplex(S), Decimals, Graph.cfRectangular)
  
def FormatReal(S, Decimals=None):
  return Graph.FormatNumber(Graph.Eval(S), Decimals, Graph.cfReal)
        
Graph.AddTextFormatFunc("polar", FormatPolar)  
Graph.AddTextFormatFunc("rectangular", FormatRectangular)
Graph.AddTextFormatFunc("real", FormatReal)
