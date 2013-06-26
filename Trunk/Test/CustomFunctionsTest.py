import Graph
import Test

def Run(Level):
  print("Running custom functions test...")
  with Test.assertRaises(Graph.EFuncError):
    Graph.CustomFunctions["f("] = lambda x: 2

  Graph.CustomFunctions["const"] = 5  
    
  with Test.assertRaises(Graph.EFuncError):
    Graph.Eval("x+y")  
  assert Graph.Eval("x+y", Locals={"x":2, "y":3}) == 5  
  assert Graph.Eval("foo(x)", Locals={"x":5, "foo": lambda x:x*x}) == 25
  
  with Test.assertRaises(Graph.EFuncError):
    Graph.EvalComplex("x+y")  
  assert Graph.EvalComplex("x+y", Locals={"x":2, "y":3}) == 5  
  assert Graph.EvalComplex("foo(x)", Locals={"x":5, "foo": lambda x:x*x}) == 25