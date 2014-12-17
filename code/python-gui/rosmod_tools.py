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

class MenuBar:
    def __init__(self, master, menuDictDict):
        self.master = master
        self.menu = Menu(self.master)
        self.menus = []
        for name,menuDict in menuDictDict.iteritems():
            self.menus.append(Menu(self.menu,tearoff=0))
            for option,callback in menuDict.iteritems():
                self.menus[-1].add_command(label=option,command=callback)
            self.menu.add_cascade(label=name,menu=self.menus[-1])
        self.master.config(menu=self.menu)

        self.menu.bind("<Button-1>",self._button1_callback)

    def _button1_callback(self,event):
        self.menu.focus_set()

class StatusBar(Frame):

    def __init__(self, master):
        Frame.__init__(self, master)
        self.label = Label(self, bd=1, relief=SUNKEN, anchor=S)
        self.label.pack(side="left",fill=X)
        self.bind("<Button-1>",self._button1_callback)

    def _button1_callback(self,event):
        self.focus_set()

    def set(self, format, *args):
        self.label.config(text=format % args)
        self.label.update_idletasks()

    def clear(self):
        self.label.config(text="")
        self.label.update_idletasks()
