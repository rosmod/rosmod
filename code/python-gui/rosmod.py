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
from rosmod_dialogs import *
from CanvasOptions import *

import sys
import os

exeName = sys.argv[0]
dirName = os.path.abspath(exeName)
head,tail = os.path.split(dirName)
head,tail = os.path.split(head)
sys.path.append(head+'/ros_generator/python/')
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
        fileDict['Rename Model Workspace']=self.menubar_Rename_Workspace_Callback
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

        self.optionsDict = rosgen.canvasOptionsDict

        self.master.protocol("WM_DELETE_WINDOW", self.close_Callback)

        self.editor = None
        self.fileName = None
        self.Model = None

    def init_Editor(self):
        if self.editor == None:
            self.editor = Editor(
                master = self.master,
                height = self.editorHeight,
                width = self.editorWidth,
                splitWidth = self.objectWidth,
                editorDict = self.optionsDict,
                model = self.Model.workspace
            )

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
            self.Model = rosgen.parse_model(self.fileName)
            self.model_path = os.path.abspath(os.path.dirname(self.fileName))
            print "model path: ", self.model_path
            self.init_Editor()
            self.editor.reset(self.Model.workspace)

    def menubar_New_Callback(self):
        self.statusBar.set("Creating new model.")
        self.Model = rosgen.Listener()
        self.Model.workspace = rosgen.ROS_Workspace()
        d = EditorDialogPopup(parent=self.master,title="New Workspace")
        if d.result != None:
            self.Model.workspace.name = d.result[0]
            self.model_path = os.path.abspath(os.getcwd())
            self.init_Editor()
            self.editor.reset(self.Model.workspace)

    def menubar_Save_Callback(self):
        if self.Model == None or self.Model.workspace == None:
            tkMessageBox.showerror("Save File","Need to create new model first!")
            return
        if self.fileName == None:
            filePath = tkFileDialog.asksaveasfilename(
                parent=self.master,
                filetypes=self.fileFormats,
                title="Save the model as..."
            )
            head, self.fileName = os.path.split(filePath)
            self.model_path = head
        fileName = self.fileName
        # Overwrite the model file
        self.statusBar.set("Saved model!")
        rosgen.generate_model(self.Model.workspace, fileName, self.model_path)

    def menubar_Save_As_Callback(self):
        if self.Model == None or self.Model.workspace == None:
            tkMessageBox.showerror("Save File","Need to create new model first!")
            return
        filePath = tkFileDialog.asksaveasfilename(
            parent=self.master,
            filetypes=self.fileFormats,
            title="Save the model as...",
            initialdir=self.model_path
        )
        if filePath != None and filePath != '':
            head, self.fileName = os.path.split(filePath)
            self.model_path = head
            self.statusBar.set("Saved model as " + self.fileName)
            if len(self.fileName) > 0:
                self.statusBar.set("Saving model as {0}".format(self.fileName))
            # Save fileName.rosml model at filePath
            rosgen.generate_model(self.Model.workspace, self.fileName, head)

    def menubar_Rename_Workspace_Callback(self):
        if self.Model == None or self.Model.workspace == None:
            tkMessageBox.showerror("Rename Workspace","Need to have a model first!")
            return
        if self.Model.workspace.Edit():
            self.init_Editor()
            self.editor.reset(self.Model.workspace)

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
        if self.Model == None or self.Model.workspace == None:
            tkMessageBox.showerror("Interpret Model","No active model!")
            return
        filePath = tkFileDialog.askdirectory(
            parent=self.master,
            title="Choose Directory",
            initialdir=self.model_path
        )
        if filePath != None and filePath != '':
            head, self.workspaceDir = os.path.split(filePath)
            self.model_path = filePath
            rosgen.generate_workspace(self.Model, self.model_path)
            self.statusBar.set("Generated ROS Workspace into " + self.model_path)
        
    def toolbar_NetworkAnalysis_Callback(self):
        self.statusBar.set("Analyzing the network characteristics!")

    def toolbar_TimingAnalysis_Callback(self):
        self.statusBar.set("Analyzing the business logic timing characteristics!")

if __name__ == "__main__":

    root = Tk()

    root.title("ROSMOD")

    app = App(root)

    root.mainloop()
