#!/usr/bin/python
'''
'''

from Tkinter import *
import tkFileDialog
import tkMessageBox

from collections import OrderedDict

class ToolBar:
    def __init__(self, master,nameToCallbackMap):
        self.buttons=[]
        self.master=master
        self.nameToCallbackMap = nameToCallbackMap
        self.frame = Frame(self.master)
        for name,callback in self.nameToCallbackMap.iteritems():
            newButton = Button(self.frame, text=name, width=len(name), command=callback)
            self.buttons.append(newButton)
            newButton.grid(row=0,column=(len(self.buttons)-1))
        self.frame.pack(side=TOP,fill=X)
        self.frame.bind("<Button-1>",self._button1_callback)

    def _button1_callback(self,event):
        self.frame.focus_set()
