import Graph
import vcl

def SortValues(Sender):
  print(Sender)
  if Sender.Name == "IncX": pass

def OnShowForm(Form):
  if Form.Name == "Form14":
    SortItem = vcl.TMenuItem(Form.PopupMenu1, Caption="Sort", _owned=False)
    Form.PopupMenu1.Items.Insert(7, SortItem)
    SortItem.Add(vcl.TMenuItem(SortItem, Caption="Increasing x", Name="IncX", OnClick=SortValues, _owned=False))
    SortItem.Add(vcl.TMenuItem(SortItem, Caption="Decreasing x", Name="DecX", OnClick=SortValues, _owned=False))
    SortItem.Add(vcl.TMenuItem(SortItem, Caption="Increasing y", Name="IncY", OnClick=SortValues, _owned=False))
    SortItem.Add(vcl.TMenuItem(SortItem, Caption="Decreasing y", Name="DecY", OnClick=SortValues, _owned=False))

Graph.OnShowForm.append(OnShowForm)
