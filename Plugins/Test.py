import Graph

def Run(Level = 0):
    print("Running automatic tests...")
    Modules = Graph.LoadPlugins(Graph.GraphDir + "\\Test")
    for Module in Modules:
        Module.Run(Level)
