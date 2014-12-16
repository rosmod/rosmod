#!/usr/bin/python
'''
This Program is designed to be the user interface
for the ROSMOD modeling language.  It has the ability
to create, load, display, edit, and export ROSMOD models.

Additionally, it supports the interaction with analysis
tools through the use of toolbar buttons and subprocesses.
'''

from Tkinter import *
import tkFileDialog
import tkMessageBox

from collections import OrderedDict

from rosmod_tools import *
from rosmod_editor import *

class App:

    def __init__(self, master):

        self.master = master

        self.fileFormats = [
            ('ROSMOD Model','*.model'),
            ('ROSMOD Component','*.comp'),
            ('ROSMOD Node','*.node')
        ]

        ''' Set up the program's menu '''
        menuDictDict = OrderedDict()
        fileDict = OrderedDict()
        fileDict['New Model']=self.menubar_New_Callback
        fileDict['Open Model']=self.menubar_Open_Callback
        fileDict['Save Model']=self.menubar_Save_Callback
        fileDict['Quit']=self.menubar_Quit_Callback
        optionsDict = OrderedDict()
        helpDict = OrderedDict()
        helpDict['Version']=self.menubar_Version_Callback
        helpDict['Author']=self.menubar_Author_Callback
        helpDict['Email']=self.menubar_Email_Callback

        menuDictDict['File'] = fileDict
        menuDictDict['Options'] = optionsDict
        menuDictDict['Help'] = helpDict

        self.menubar = MenuBar(self.master,menuDictDict)


        ''' Set up the toolbar '''
        toolbarMap = OrderedDict()
        toolbarMap['interpret'] = self.toolbar_Interpret_Callback
        toolbarMap['Network QoS'] = self.toolbar_NetworkAnalysis_Callback
        toolbarMap['BL Timing'] = self.toolbar_TimingAnalysis_Callback
        self.toolbar = ToolBar(self.master,toolbarMap)

        ''' Set up the Status Bar for messages '''
        self.statusBar = StatusBar(self.master)
        self.statusBar.pack(side=BOTTOM, fill=BOTH,expand=1)

        ''' Set up the Object Browser ''' 
        self.editorHeight = 800
        self.editorWidth = 800
        self.objectWidth = 100

        ''' Set up the Editor Pane ''' 
        self.editorPane = PanedWindow(
            self.master, 
            orient = HORIZONTAL, 
            opaqueresize = True, 
            height = self.editorHeight, 
            width = self.editorWidth
        )
        ''' Set up the Object Browser ''' 
        objectDict = OrderedDict()
        objectDict['service'] = ["green",("object","service")]
        objectDict['message'] = ["blue",("object","message")]
        objectDict['timer'] = ["gray",("object","timer")]
        objectDict['component'] = ["red",("object","component")]
        objectDict['node'] = ["black",("object","node")]
        self.objectList = ObjectList(
            self.master, 
            objectDict=objectDict,
            height=self.editorHeight, 
            width=self.objectWidth,
            maxHeight=self.editorHeight*2,
            maxWidth=self.objectWidth
        )
        ''' Set up the Model View / Editor ''' 
        self.modelViewer = ModelViewer(
            self.master, 
            height=self.editorHeight, 
            width=self.editorWidth-self.objectWidth,
            maxHeight=self.editorHeight*2,
            maxWidth=self.editorWidth*4
        )
        self.editorPane.add(self.objectList)
        self.editorPane.add(self.modelViewer)

        self.editorPane.pack(fill='both', expand=1)

        self.master.protocol("WM_DELETE_WINDOW", self.close_Callback)

    '''
    -------------
    Exit Handler:
    -------------
    '''
    def close_Callback(self):
        if tkMessageBox.askokcancel("Quit", "Do you really wish to quit?"):
            self.master.quit()

    '''
    ------------------------
    Menu Callback Functions:
    ------------------------
    '''
    def menubar_Open_Callback(self):
        file = tkFileDialog.askopenfile(
            parent=self.master,
            mode='rb',
            filetypes=self.fileFormats,
            title="Select Model"
        )
        if file != None:
            data = file.read()
            print "File {0} has {1} bytes".format(file,len(data))
            file.close()

    def menubar_New_Callback(self):
        print "you pressed New"

    def menubar_Save_Callback(self):
        fileName = tkFileDialog.asksaveasfilename(
            parent=self.master,
            filetypes=self.fileFormats,
            title="Save the model as..."
        )
        if len(fileName) > 0:
            print "Now saving model as {0}".format(fileName)

    def menubar_Version_Callback(self):
        self.statusBar.set("ROSMOD GUI Version 0.0.1")

    def menubar_Author_Callback(self):
        self.statusBar.set("William Emfinger; Pranav Srinivas Kumar")

    def menubar_Email_Callback(self):
        self.statusBar.set("emfinger@isis.vanderbilt.edu; pkumar@isis.vanderbilt.edu")

    def menubar_Quit_Callback(self):
        self.close_Callback()

    '''
    ---------------------------
    Toolbar Callback Functions:
    ---------------------------
    '''
    def toolbar_Interpret_Callback(self):
        self.statusBar.set("Generating ROS package files!")
        
    def toolbar_NetworkAnalysis_Callback(self):
        self.statusBar.set("Analyzing the network characteristics!")

    def toolbar_TimingAnalysis_Callback(self):
        self.statusBar.set("Analyzing the business logic timing characteristics!")

if __name__ == "__main__":

    root = Tk()

    app = App(root)

    root.mainloop()
