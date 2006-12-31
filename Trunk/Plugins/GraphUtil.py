import os.path
from Tkinter import *

def InitPlugins():
	print "Loading plugins..."
	root = Tk()
	root.withdraw()
	for FileName in os.listdir("Plugins"):
		if os.path.splitext(FileName)[1] == ".py":
			__import__(os.path.splitext(FileName)[0])
