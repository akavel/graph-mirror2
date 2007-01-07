import os.path
import os
import sys
import imp
import traceback
from Tkinter import *
import Graph

def InitPlugins():
	print "Loading plugins..."
	sys.stdin = None

	global root
	root = Tk()
	root.withdraw()

	PluginsDir = os.path.dirname(sys.argv[0]) + '\\Plugins'
	sys.path.append(PluginsDir)

	Modules = []
	AllowedExt = [ i[0] for i in imp.get_suffixes() ]
	for ModuleName, Ext in [ os.path.splitext(f) for f in os.listdir(PluginsDir) ]: 
		if Ext in AllowedExt and not ModuleName in Modules:
			try:
				Modules.append(ModuleName)
				__import__(ModuleName)

			except ImportError, e:
				traceback.print_exc()


class Action(object):
		def __init__(self, caption, event, **keywords): 
			object.__setattr__(self, "id", Graph.CreateAction())
			Graph.SetActionAttr(self.id, caption=caption, event=event, **keywords) 

		def __getattr__(self, name): 
			return Graph.GetActionAttr(self.id)[name]
		def __setattr__(self, name, value): 
			Graph.SetActionAttr(self.id, **{name:value})
