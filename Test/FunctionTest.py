import Graph

def Run(Level):
  print("Running function test...")
  Func = Graph.TStdFunc("x^2+3")
  assert Func.Eval(2) == (2,7)