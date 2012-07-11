import Graph

def Run():
    print("Running automatic tests...")
    Modules = Graph.LoadPlugins(Graph.GraphDir + "\\Test")
    for Module in Modules:
        Module.Run()
