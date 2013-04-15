import Graph
import Test

def Run(Level):
  print("Running plugin data test...")
  Graph.GlobalPluginData["Test"] = (5, "Hello world", [1,2,3,4])
  assert Graph.GlobalPluginData["Test"] == (5, "Hello world", [1,2,3,4])
  Graph.GlobalPluginData["Test2"] = ({"1":5, "2":7, "6":9},)
  Graph.GlobalPluginData["Test3"] = (3, 5.5, True)  
  assert Graph.GlobalPluginData["Test2"] == ({"1":5, "2":7, "6":9},)
  assert Graph.GlobalPluginData["Test3"] == (3, 5.5, True)
  assert [x for x in Graph.GlobalPluginData] == ["Test", "Test2", "Test3"]
  del Graph.GlobalPluginData["Test"]
  del Graph.GlobalPluginData["Test2"]
  del Graph.GlobalPluginData["Test3"]

  with Test.assertRaises(KeyError):
    del Graph.GlobalPluginData["Test"]
  with Test.assertRaises(KeyError):
    del Graph.GlobalPluginData[5]
  with Test.assertRaises(KeyError):
    Graph.GlobalPluginData[5] = (7,)
  