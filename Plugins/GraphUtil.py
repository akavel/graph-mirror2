import os.path
import os
import sys
import imp
import traceback
from Tkinter import *

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
