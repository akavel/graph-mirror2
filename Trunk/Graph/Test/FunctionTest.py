import Graph

def Run(Level):
  print("Running function test...")
  Func = Graph.TStdFunc("x^2+3")
  assert Func.Eval(2) == (2,7)
  
  Func = Graph.TParFunc("t^3", "t^2")
  assert Func.Eval(2) == (8,4)
  
  Func = Graph.TPolFunc("5")
  assert Func.Eval(0) == (5,0)