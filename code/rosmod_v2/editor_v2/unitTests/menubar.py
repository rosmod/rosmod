#!/usr/bin/python
'''
'''

from Tkinter import *
import tkFileDialog
import tkMessageBox

from collections import OrderedDict

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
