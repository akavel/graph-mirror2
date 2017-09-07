# Add a residual plot as a point series when a trendline is created.
import Graph
       
def NewElem(Item):
    if isinstance(Item, Graph.TStdFunc) and isinstance(Graph.Selected, Graph.TPointSeries):
        if "R²" in Item.LegendText: # Silly way to detect that a function is a newly created trendline
            ResidualSeries = Graph.TPointSeries()
            ResidualSeries.LegendText = "Residual plot"
            ResidualSeries.LineSize = 0
            ResidualSeries.Size = 4
            ResidualSeries.Style = 2
            for (x,y) in Graph.Selected.Points:
                ResidualSeries.Points.append((x, y - Item.Eval(x)[1]))
            Graph.FunctionList.append(ResidualSeries)
            Graph.Redraw()

Graph.OnNewElem.append(NewElem)
