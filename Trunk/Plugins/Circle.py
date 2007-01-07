# File: Circle.py
import tkSimpleDialog
import Graph
import GraphUtil
from Tkinter import *

class MyDialog(tkSimpleDialog.Dialog):
	def body(self, master):
		Label(master, text="Radius:").grid(row=0)
		Label(master, text="Center:").grid(row=1)
		Label(master, text="X:").grid(row=1, column=1)
		Label(master, text="Y:").grid(row=1, column=3)

		self.e1 = Entry(master, width=10)
		self.e2 = Entry(master, width=10)
		self.e3 = Entry(master, width=10)

		self.e1.grid(row=0, column=2)
		self.e2.grid(row=1, column=2)
		self.e3.grid(row=1, column=4)

		self.resizable(height=FALSE, width=FALSE)
		self.geometry("+%d+%d" % (500, 300))

		return self.e1 # initial focus

	def validate(self):
		#self.result = float(self.e1.get()), float(self.e2.get()), float(self.e3.get())
		return 1

	def apply(self):
		Graph.CreateParametricFunction(self.e2.get() + "+" + self.e1.get() + "*cos t", self.e3.get() + "+" + self.e1.get() + "*sin t")

def execute_action():
	d = MyDialog(None, "Create circle")

action = GraphUtil.Action("Create circle...", execute_action, icon="Graph.bmp", hint="Create circle from center and radius", shortcut="Ctrl+Alt+C")
