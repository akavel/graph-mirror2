import Graph
import GraphUtil
import wx


def execute_action():
    class CircleDialog(wx.Dialog):
        def __init__(self, parent, title):
            wx.Dialog.__init__(self, parent, wx.ID_ANY, title, size=(200,100))
            wx.Button(self, 10, "OK")
            wx.Button(self, 11, "Cancel")
            wx.EVT_BUTTON(self, 10, self.OnOk)
            wx.EVT_BUTTON(self, 11, self.OnCancel)

        def OnOk(self, event):
            print "OK"
            d = ScrolledMessageDialog(GraphUtil.root, "Hello world", "Test")
            d.ShowModal()
            pass

        def OnCancel(self, event):
            Close()

    from wx.lib.dialogs import ScrolledMessageDialog
#    d = ScrolledMessageDialog(GraphUtil.root, "Hello world", "Test")
#    d = wx.MessageDialog(GraphUtil.root, "Hello world")
    d = CircleDialog(GraphUtil.root, 'Small editor')
    d.ShowModal()
    print "Hello world"


action = GraphUtil.Action("Create circle...", execute_action, icon="Graph.bmp", hint="Create circle from center and radius", shortcut="Ctrl+Shift+C")
