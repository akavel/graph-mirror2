import Graph

class EventLogger:
    def __init__(self, s):
        self.s = s
    def __call__(self, *args):
        print(self.s, args if len(args) != 1 else "(%s)" % args[0], sep='')

Graph.OnNew.append(EventLogger("OnNew"))
Graph.OnLoad.append(EventLogger("OnLoad"))
Graph.OnSelect.append(EventLogger("OnSelect"))
Graph.OnClose.append(EventLogger("OnClose"))
Graph.OnEdit.append(EventLogger("OnEdit"))
Graph.OnAnimate.append(EventLogger("OnAnimate"))

def test():
    import time
    I = 0
    while(1):
        print("Hello world", I)
        time.sleep(1)
        I += 1

