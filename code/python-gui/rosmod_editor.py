#!/usr/bin/python
'''
'''

from Tkinter import *
import tkFileDialog
import tkMessageBox

from collections import OrderedDict

from rosmod_dialogs import *

import sys
import os
sys.path.append('../ros_generator/python/')
import rosgen as rosgen

class EditorFrame(Frame):
    def __init__(self, master, label, height, width, maxHeight, maxWidth,contextDict):
        Frame.__init__(self,master)
        Frame.config(self,bd=2, relief=SUNKEN)

        self.height=height
        self.width=width
        self.maxWidth = maxWidth
        self.maxHeight = maxHeight

        self.label = Label(self, text=label, anchor=N, bg="dark gray", fg="white", relief=RAISED)
        self.label.pack()

        self.VScrollBar = Scrollbar(self, orient=VERTICAL)
        self.VScrollBar.pack(fill='y', side=RIGHT)
        self.HScrollBar = Scrollbar(self, orient=HORIZONTAL)
        self.HScrollBar.pack(side=BOTTOM, fill='x')

        self.canvas = Canvas(
            self,
            width=width, 
            height=height, 
            scrollregion=(0,0,self.maxWidth,self.maxHeight),
            xscrollcommand=self.HScrollBar.set,
            yscrollcommand=self.VScrollBar.set
        )

        self.canvasID = self.canvas.create_rectangle(
            0, 0, 
            self.maxWidth, self.maxHeight, 
            outline="light gray", fill="light gray", 
            tags=("object","canvas")
        )

        self.contextMenu = MenuPopup(self.canvas,contextDict)
        self.canvas.tag_bind("object","<Button-3>", self.contextMenu.popupCallback)
        
        self.VScrollBar.config(command=self.canvas.yview)
        self.HScrollBar.config(command=self.canvas.xview)

        self.canvas.bind("<MouseWheel>", self._mouse_wheel)
        self.canvas.bind("<Button-4>", self._mouse_wheel)
        self.canvas.bind("<Button-5>", self._mouse_wheel)

        self.canvas.bind("<Button-1>", self._button1_callback)
        self.canvas.bind("<Delete>",self._delete_callback)

        self.canvas.pack(fill=BOTH)
        self.pack(fill=BOTH)

    def Update(self):
        self.canvasID = self.canvas.create_rectangle(
            0, 0, 
            self.maxWidth, self.maxHeight, 
            outline="light gray", fill="light gray", 
            tags=("object","canvas")
        )
        self.canvas.tag_bind("object","<Button-3>", self.contextMenu.popupCallback)

    def _button1_callback(self,event):
        closeAllContextMenus()
        self.focus_set()
        self.canvas.focus_set()

    def _delete_callback(self,event):
        print "delete has been pressed"

    def _mouse_wheel(self, event):
        closeAllContextMenus()
        direction = 0
        # respond to Linux or Windows wheel event
        if event.num == 5 or event.delta == -120:
            direction = 1
        if event.num == 4 or event.delta == 120:
            direction = -1
        self.canvas.yview_scroll(direction, "units")

    def destroy(self):
        self.HScrollBar.destroy()
        self.VScrollBar.destroy()
        self.canvas.destroy()
        self.contextMenu.destroy()
        Frame.destroy(self)
        
class ModelViewer(EditorFrame):
    def __init__(self, master, height, width, maxHeight, maxWidth, optionsDict=None, model=None,contextDict=None):

        self.editorContextDict = OrderedDict()
        self.canvasContextDict=OrderedDict()
        self.canvasContextDict['Add Service'] = self.ContextAddService
        self.canvasContextDict['Add Message'] = self.ContextAddMessage
        self.canvasContextDict['Add Component'] = self.ContextAddComponent
        self.canvasContextDict['Add Node'] = self.ContextAddNode

        self.srvContextDict = OrderedDict()
        self.msgContextDict = OrderedDict()
        self.tmrContextDict = OrderedDict()
        self.compContextDict = OrderedDict()
        self.nodeContextDict = OrderedDict()
        self.editorContextDict['canvas'] = self.canvasContextDict
        self.editorContextDict['service'] = self.srvContextDict
        self.editorContextDict['message'] = self.msgContextDict
        self.editorContextDict['timer'] = self.tmrContextDict
        self.editorContextDict['component'] = self.compContextDict
        self.editorContextDict['node'] = self.nodeContextDict

        self.srvContextDict['Edit Service'] = self.ContextEdit
        self.srvContextDict['Delete'] = self.ContextDelete

        self.msgContextDict['Edit Message'] = self.ContextEdit
        self.msgContextDict['Delete'] = self.ContextDelete

        self.tmrContextDict['Edit Timer'] = self.ContextEdit
        self.tmrContextDict['Delete'] = self.ContextDelete

        self.compContextDict['Edit Component'] = self.ContextEdit
        self.compContextDict['Delete'] = self.ContextDelete
        self.compContextDict['Add Timer'] = self.CompAddTimer
        self.compContextDict['Add Publisher'] = self.CompAddPub
        self.compContextDict['Add Subscriber'] = self.CompAddSub
        self.compContextDict['Add Client'] = self.CompAddClient
        self.compContextDict['Add Server'] = self.CompAddServer

        self.nodeContextDict['Edit Node'] = self.ContextEdit
        self.nodeContextDict['Delete'] = self.ContextDelete
        self.nodeContextDict['Add Component'] = self.NodeAddComp

        EditorFrame.__init__(self,master,"Model Viewer",height,width,maxHeight,maxWidth,self.editorContextDict)

        self.model=model
        self.optionsDict = optionsDict

        self.initPos = [10,10]
        self.padding = [10,10]

        self.var = StringVar()
        self.activeObject=None
        self.contextMenu.setterFunc = self.SetActiveObject
        self.entry=None

    def clear(self):
        self.canvas.delete(ALL)
        self.activeObject = None
        self.entry = None
        EditorFrame.Update(self)

    def Update(self, model, initPos, padding):
        self.model = model
        self.initPos = initPos
        self.padding = padding
        self.clear()
        if self.model != None:
            self.drawModel(self.initPos,self.padding)

    def SetActiveObject(self, object):
        self.activeObject = object

    def ContextAddService(self):
        print "Popup dialog for adding service"
        newObj = rosgen.ROS_Service()
        newObj.name = "dummyService"
        self.model.AddService(newObj)
        self.Update(self.model,self.initPos,self.padding)

    def ContextAddMessage(self):
        print "Popup dialog for adding message"
        newObj = rosgen.ROS_Message()
        newObj.name = "dummyMessage"
        self.model.AddMessage(newObj)
        self.Update(self.model,self.initPos,self.padding)

    def ContextAddComponent(self):
        print "Popup dialog for adding component"
        newObj = rosgen.ROS_Component()
        newObj.name = "dummyComponent"
        self.model.AddComponent(newObj)
        self.Update(self.model,self.initPos,self.padding)

    def ContextAddNode(self):
        newObj = rosgen.ROS_Node()
        d = EditorDialogPopup(parent=self.master,title="Enter Node Name")
        if d.result != None:
            newObj.name = d.result[0]
            self.model.AddNode(newObj)
            self.Update(self.model,self.initPos,self.padding)

    def ContextDelete(self):
        print "Deleting active object {0}".format(self.activeObject)
        if self.activeObject.isObjRef == True:
            print "Active object refers to object {0}".format(self.activeObject.objRef)
        self.Update(self.model,self.initPos,self.padding)

    def ContextEdit(self):
        print "Editing active object {0}".format(self.activeObject)
        if self.activeObject.isObjRef == True:
            print "Active object refers to object {0}".format(self.activeObject.objRef)
            self.activeObject.objRef.Edit()
        else:
            self.activeObject.Edit()
        self.Update(self.model,self.initPos,self.padding)

    def CompAddServer(self):
        print "popup for adding server to component {0}".format(self.activeObject)
        self.Update(self.model,self.initPos,self.padding)

    def CompAddClient(self):
        print "popup for adding client to component {0}".format(self.activeObject)
        self.Update(self.model,self.initPos,self.padding)

    def CompAddSub(self):
        print "popup for adding subscriber to component {0}".format(self.activeObject) 
        self.Update(self.model,self.initPos,self.padding)   

    def CompAddPub(self):
        print "popup for adding publisher to component {0}".format(self.activeObject)
        self.Update(self.model,self.initPos,self.padding)

    def CompAddTimer(self):
        print "popup for adding timer to component {0}".format(self.activeObject)
        self.Update(self.model,self.initPos,self.padding)

    def NodeAddComp(self):
        print "popup for adding component to node {0}".format(self.activeObject)
        self.Update(self.model,self.initPos,self.padding)

    def OnObjectLeftClick(self, event):
        self.focus_set()
        self.canvas.focus_set()
        selectedObject = self.canvas.find_closest(
            self.canvas.canvasx(event.x), 
            self.canvas.canvasy(event.y)
        )[0]
        tags = self.canvas.gettags(selectedObject)

    def drawObjectsFromDict(self, dictKey, drawDict, initX, initY, padY):
        ypos = initY
        maxX = 0
        for name,object in drawDict.iteritems():
            object.setCanvasParams(
                canvas = self.canvas,
                position = (initX,ypos),
                canvasOptions = self.optionsDict[dictKey]
            )
            object.buildChildList()
            width,height = object.Draw()
            if (width + initX) > maxX:
                maxX = width + initX
            ypos += height + padY
        maxY = ypos
        return (maxX,maxY)

    def drawModel(self, initPos, padding):
        self.initPos = initPos
        self.padding = padding
        initX = self.initPos[0]
        initY = self.initPos[1]
        padX = self.padding[0]
        padY = self.padding[1]
        #column 1
        xpos,ypos = self.drawObjectsFromDict(
            'service',
            self.model.srv_dict,
            initX,initY,
            padY
        )
        xpos2,ypos = self.drawObjectsFromDict(
            'message',
            self.model.msg_dict,
            initX,ypos,
            padY
        )
        #column 2
        xpos,ypos = self.drawObjectsFromDict(
            'component',
            self.model.component_definition_dict,
            xpos + padX,initY,
            padY
        )
        #column 3
        xpos,ypos = self.drawObjectsFromDict(
            'node',
            self.model.nodes_dict,
            xpos+padX,initY,
            padY
        )

class PackageViewer(EditorFrame):
    def __init__(self, master, height, width, maxHeight, maxWidth, model, buttonVar, buttonCommand,contextDict=None):
    
        self.editorContextDict = OrderedDict()
        self.canvasContextDict=OrderedDict()
        self.canvasContextDict['Add New Package'] = self.ContextAddPackage
        self.canvasContextDict['Delete Selected Package'] = self.ContextDeletePackage
        self.editorContextDict['canvas'] = self.canvasContextDict

        EditorFrame.__init__(self,master,"Package Viewer",height,width,maxHeight,maxWidth,self.editorContextDict)
        self.padding = [5,25]
        self.buttons = []
        self.model = model
        self.buttonVar = buttonVar
        self.buttonCommand = buttonCommand
        self.Update(self.model,self.buttonVar,self.buttonCommand)

    def ContextAddPackage(self):
        newObj = rosgen.ROS_Package()
        d = EditorDialogPopup(parent=self.master,title="New Package")
        if d.result != None:
            newObj.name = d.result[0]
            self.model.AddPackage(newObj)
            self.Update(self.model,self.buttonVar,self.buttonCommand)
            self.buttonCommand()

    def ContextDeletePackage(self):
        if self.buttonVar.get() != None and self.buttonVar.get() != "":
            print "removing {0}".format(self.buttonVar.get())
            self.model.DeletePackage(self.buttonVar.get())
        self.buttonVar.set("")
        self.Update(self.model,self.buttonVar,self.buttonCommand)
        self.buttonCommand()

    def Update(self,model,buttonVar,buttonCommand):
        self.model = model
        self.buttonVar = buttonVar
        self.buttonVar.set("")
        self.buttonCommand = buttonCommand
        for button in self.buttons:
            self.canvas.delete(button[0])
            button[1].destroy()
        self.buttons=[]
        ypos = 5
        for name,package in self.model.packages_dict.iteritems():
            newButton = Radiobutton(
                self.canvas, 
                text = name, 
                variable=self.buttonVar, 
                value=name, 
                indicatoron=0, 
                command = self.buttonCommand
            )
            self.buttons.append(
                [self.canvas.create_window((self.padding[0],ypos),window=newButton,anchor=NW),
                 newButton]
            )
            ypos += self.padding[1]

class Editor():
    def __init__(self,master,height,width,splitWidth,maxWidth,maxHeight,editorDict=None,model=None):
        self.master = master
        self.height = height
        self.width = width
        self.editorDict = editorDict
        self.model = model
        self.maxWidth = maxWidth
        self.maxHeight = maxHeight
        self.splitWidth = splitWidth
        self.selectedPackageVar = StringVar()
        
        self.editorPane = PanedWindow(
            self.master,
            orient = HORIZONTAL,
            opaqueresize = True,
            height = self.height,
            width = self.width
        )
        self.modelViewer = ModelViewer(
            master = self.master,
            height = self.height,
            width = self.width - self.splitWidth,
            maxHeight = self.maxHeight,
            maxWidth = self.maxWidth,
            optionsDict = self.editorDict,
            model=self.model
        )
        self.packageViewer = PackageViewer (
            master = self.master,
            height = self.height,
            width = self.splitWidth,
            maxHeight = self.maxHeight,
            maxWidth = self.maxWidth,
            model=self.model,
            buttonVar = self.selectedPackageVar,
            buttonCommand = self.OnPackageSelected
        )
        self.editorPane.add(self.packageViewer)
        self.editorPane.add(self.modelViewer)
        self.editorPane.pack(fill='both',expand=1)

    def reset(self,model):
        self.model=model
        self.modelViewer.clear()
        self.packageViewer.Update(
            model=self.model,
            buttonVar = self.selectedPackageVar,
            buttonCommand = self.OnPackageSelected
        )

    def OnPackageSelected(self):
        print "You've selected {0}".format(self.selectedPackageVar.get())
        newModel = None
        if self.selectedPackageVar.get() != None and self.selectedPackageVar.get() != '':
            newModel = self.model.packages_dict[self.selectedPackageVar.get()]
        self.modelViewer.Update(
            newModel,
            initPos=[50,50],
            padding=[100,20]
        )
        
        
        
        

        
