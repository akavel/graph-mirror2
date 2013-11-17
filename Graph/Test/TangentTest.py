import Graph

def Run(Level):
  print("Running tangent test...")
  Function = Graph.TStdFunc("x^2")
  Tan = Graph.TTan()
  Tan.t = 2
  assert not Tan.Valid
  Function.ChildList.append(Tan)
  assert not Tan.Valid # Not valid until drawn
