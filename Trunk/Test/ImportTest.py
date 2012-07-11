import Graph

def Run():
    print("Running import test...")
    Graph.LoadDefault()
    Graph.ImportPointSeries("Test/Files/Test.csv")
    Graph.Update()
