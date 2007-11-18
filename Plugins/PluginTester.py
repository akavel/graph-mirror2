import Graph

def OnNew():
    print "OnNew"

def OnLoad():
    print "OnLoad"

def OnSelect():
    print "OnSelect"

Graph.OnNew.append(OnNew)
Graph.OnLoad.append(OnLoad)
Graph.OnSelect.append(OnSelect)
