import Graph

ShowEvents = False
LogFile = "Events.log"

File = open(LogFile, "w") if LogFile else None
class EventLogger:
    def __init__(self, s):
        self.s = s
    def __call__(self, *args):
        Str = self.s + (str(args) if len(args) != 1 else "(%s)" % args[0])
        if ShowEvents:
            print(Str)
        if File:
            File.write(Str + "\n")

Graph.OnNew.append(EventLogger("OnNew"))
Graph.OnLoad.append(EventLogger("OnLoad"))
Graph.OnSelect.append(EventLogger("OnSelect"))
Graph.OnClose.append(EventLogger("OnClose"))
Graph.OnEdit.append(EventLogger("OnEdit"))
Graph.OnAnimate.append(EventLogger("OnAnimate"))
Graph.OnDelete.append(EventLogger("OnDelete"))
Graph.OnAxesChanged.append(EventLogger("OnAxesChanged"))
Graph.OnZoom.append(EventLogger("OnZoom"))
Graph.OnOptionsChanged.append(EventLogger("OnOptionsChanged"))
Graph.OnCustomFunctions.append(EventLogger("OnCustomFunctions"))
Graph.OnNewElem.append(EventLogger("OnNewElem"))
Graph.OnChanged.append(EventLogger("OnChanged"))
Graph.OnMoved.append(EventLogger("OnMoved"))
File.write("PluginTester loaded!\n")

def TestUndo():
    Graph.LoadDefault
    Undo = Graph.FindAction("UndoAction").Execute
    Redo = Graph.FindAction("RedoAction").Execute
    F1 = Graph.TStdFunc("sin(x)")
    F2 = Graph.TRelation("x^2+y^2=25")
    F3 = Graph.TPointSeries()
    Graph.FunctionList.append(F1)
    Graph.FunctionList.append(F2)
    assert(Graph.FunctionList[1] == F1)
    assert(Graph.FunctionList[2] == F2)
    assert(len(Graph.FunctionList) == 3)
    Undo()
    assert(len(Graph.FunctionList) == 2)
    Redo()
    assert(len(Graph.FunctionList) == 3)
    assert(Graph.FunctionList[2] == F2)
    Graph.FunctionList[1] = F3
    assert(Graph.FunctionList[1] == F3)
    assert(Graph.FunctionList[2] == F2)
    Undo()
    assert(Graph.FunctionList[1] == F1)
    assert(Graph.FunctionList[2] == F2)
    Redo()
    assert(Graph.FunctionList[1] == F3)
    assert(Graph.FunctionList[2] == F2)
    del Graph.FunctionList[1]
    assert(Graph.FunctionList[1] == F2)
    assert(len(Graph.FunctionList) == 2)
    Undo()
    assert(Graph.FunctionList[1] == F3)
    assert(Graph.FunctionList[2] == F2)
    Redo()
    assert(Graph.FunctionList[1] == F2)
    assert(len(Graph.FunctionList) == 2)

def Run():
    TestUndo()
