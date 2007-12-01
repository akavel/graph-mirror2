import Graph

def OnNew():
    print "OnNew"

def OnLoad():
    print "OnLoad"

def OnSelect():
    print "OnSelect"

#Graph.OnNew.append(OnNew)
#Graph.OnLoad.append(OnLoad)
#Graph.OnSelect.append(OnSelect)

def test():
    import time
    I = 0
    while(1):
        print "Hello world", I
        time.sleep(1)
        I += 1
        
