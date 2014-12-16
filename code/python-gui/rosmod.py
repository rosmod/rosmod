#!/usr/bin/python

from Tkinter import *

from collections import OrderedDict
import tkMessageBox

class Toolbar:
    def __init__(self, master,nameToCallbackMap):
        self.buttons=[]
        self.master=master
        self.nameToCallbackMap = nameToCallbackMap
        self.frame = Frame(self.master)
        for name,callback in self.nameToCallbackMap.iteritems():
            newButton = Button(self.frame, text=name, width=6, command=callback)
            self.buttons.append(newButton)
            newButton.grid(row=0,column=(len(self.buttons)-1))
        self.frame.pack(side=TOP,fill=X)

class Menubar:
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

class App:

    def __init__(self, master):

        self.master = master

        ''' Set up the program's menu '''
        menuDictDict = OrderedDict()
        fileDict = {}
        fileDict['New']=self.menubar_New_Callback
        fileDict['Open']=self.menubar_Open_Callback
        fileDict['Save']=self.menubar_Save_Callback
        fileDict['Quit']=self.menubar_Quit_Callback
        optionsDict = {}
        helpDict = {}
        helpDict['Version']=self.menubar_Version_Callback
        helpDict['Author']=self.menubar_Author_Callback
        helpDict['Email']=self.menubar_Email_Callback

        menuDictDict['File'] = fileDict
        menuDictDict['Options'] = optionsDict
        menuDictDict['Help'] = helpDict

        self.menubar = Menubar(self.master,menuDictDict)


        ''' Set up the toolbar '''
        toolbarMap = {}
        toolbarMap['interpret'] = self.toolbar_Interpret_Callback
        toolbarMap['analyze'] = self.toolbar_Analyze_Callback
        self.toolbar = Toolbar(self.master,toolbarMap)

    def menubar_Open_Callback(self):
        print "you pressed Open"

    def menubar_New_Callback(self):
        print "you pressed New"

    def menubar_Save_Callback(self):
        print "you pressed Save"

    def menubar_Version_Callback(self):
        print "ROSMOD GUI Version 0.0.1"

    def menubar_Author_Callback(self):
        print "William Emfinger; Pranav Srinivas Kumar"

    def menubar_Email_Callback(self):
        print "emfinger@isis.vanderbilt.edu; pkumar@isis.vanderbilt.edu"

    def menubar_Quit_Callback(self):
        print "you pressed Quit"
        self.master.quit()

    def toolbar_Interpret_Callback(self):
        print "you pressed interpret!"
        
    def toolbar_Analyze_Callback(self):
        print "you pressed analyze!"

root = Tk()

app = App(root)

root.mainloop()
#root.destroy()
