import Graph

def Run(Level):
  print("Running shading test...")
  Function = Graph.TStdFunc("sin x")
  Shading = Graph.TShading()
  Function.ChildList.append(Shading)