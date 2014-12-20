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
from rosmod_classes import *

import sys
import os
sys.path.append('../ros_generator/python/')
import rosgen as rosgen

class App:

    def __init__(self, master):

        self.master = master

        self.fileFormats = [
            ('ROSMOD Model','*.rosml')
        ]

        ''' Set up the program's menu '''
        menuDictDict = OrderedDict()
        fileDict = OrderedDict()
        fileDict['New Model']=self.menubar_New_Callback
        fileDict['Open Model']=self.menubar_Open_Callback
        fileDict['Save Model']=self.menubar_Save_Callback
        fileDict['Save Model As']=self.menubar_Save_As_Callback
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
        self.statusBar.pack(side=BOTTOM, fill=X)

        ''' Set up the Editor ''' 
        self.editorHeight = 800
        self.editorWidth = 800
        self.objectWidth = 100

        self.Model = rosgen.ROS_Workspace()

        self.optionsDict = rosgen.OrderedDict()
        self.paddingOptions = rosgen.PaddingOptions((5,10),(0,15))
        self.fontOptions = rosgen.FontOptions()
        self.optionsDict['service'] = rosgen.CanvasOptions(
            self.paddingOptions,
            self.fontOptions,
            rosgen.DrawOptions(
                color = "green",
                minSize = (10,10),
            ),
            tags = ("object","service")
        )
        self.optionsDict['message'] = rosgen.CanvasOptions(
            self.paddingOptions,
            self.fontOptions,
            rosgen.DrawOptions(
                color = "blue",
                minSize = (10,10),
            ),
            tags = ("object","message")
        )
        self.optionsDict['timer'] = rosgen.CanvasOptions(
            self.paddingOptions,
            self.fontOptions,
            rosgen.DrawOptions(
                color = "gray",
                minSize = (10,10),
            ),
            tags = ("object","timer")
        )
        self.optionsDict['component'] = rosgen.CanvasOptions(
            self.paddingOptions,
            self.fontOptions,
            rosgen.DrawOptions(
                color = "red",
                minSize = (10,10),
            ),
            tags = ("object","component")
        )
        self.optionsDict['node'] = rosgen.CanvasOptions(
            self.paddingOptions,
            self.fontOptions,
            rosgen.DrawOptions(
                color = "cyan",
                minSize = (10,10),
            ),
            tags = ("object","node")
        )

        self.editor = Editor(
            master = self.master,
            height = self.editorHeight,
            width = self.editorWidth,
            splitWidth = self.objectWidth,
            maxWidth = self.editorHeight * 2,
            maxHeight = self.editorHeight * 2,
            editorDict = self.optionsDict,
            model = self.Model
        )

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
            self.fileName = file.name
            self.statusBar.set("Opened {0}".format(self.fileName))
            file.close()
            #implement the open functionality here to load the model in
            #self.Model = Model()
            #self.editor.modelViewer.canvas.delete(ALL)
            #self.editor.modelViewer.drawModel()  
            self.Model = rosgen.parse_model(self.fileName)
            self.model_path = os.path.abspath(os.path.dirname(self.fileName))
            self.editor.reset(self.Model.workspace)

    def menubar_New_Callback(self):
        self.statusBar.set("Creating new model.")
        self.editor.modelViewer.canvas.delete(ALL)
        self.editor.modelViewer.drawModel()

    def menubar_Save_Callback(self):
        fileName = self.fileName
        # Overwrite the model file
        self.statusBar.set("Saved model!")
        rosgen.generate_model(self.Model.workspace, fileName, self.model_path)

    def menubar_Save_As_Callback(self):
        filePath = tkFileDialog.asksaveasfilename(
            parent=self.master,
            filetypes=self.fileFormats,
            title="Save the model as..."
        )
        head, fileName = os.path.split(filePath)
        self.statusBar.set("Saved model as " + fileName)
        if len(fileName) > 0:
            self.statusBar.set("Saving model as {0}".format(fileName))
        # Save fileName.rosml model at filePath
        rosgen.generate_model(self.Model.workspace, fileName, head)

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
        rosgen.generate_workspace(self.Model, self.model_path)
        
    def toolbar_NetworkAnalysis_Callback(self):
        self.statusBar.set("Analyzing the network characteristics!")

    def toolbar_TimingAnalysis_Callback(self):
        self.statusBar.set("Analyzing the business logic timing characteristics!")

if __name__ == "__main__":

    root = Tk()

    app = App(root)

    root.mainloop()
