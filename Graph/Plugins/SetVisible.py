import Graph

def Execute(Sender):
	for Item in Graph.FunctionList:
		if Item != Graph.Selected and type(Item) != Graph.Data.TAxesView:
			Item.Visible = False
	Graph.Update()
			
Action = Graph.CreateAction(Caption="Disable all but this", OnExecute=Execute, Hint="Disable all but this item.")
ContextMenuItem = Graph.AddActionToContextMenu(Action)
