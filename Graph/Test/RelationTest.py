import Graph

def Run(Level):
  print("Running relation test...")
  Relation = Graph.TRelation("x^2+y^2 = 25", "x>0")
  assert Relation.Eval(2, 1) == -20
  
  Relation = Graph.TRelation("x^2+y^2 < 25", "x>0")
  assert Relation.Eval(2, 1) == 1.0
