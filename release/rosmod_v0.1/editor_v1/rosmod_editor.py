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
exeName = sys.argv[0]
dirName = os.path.abspath(exeName)
head,tail = os.path.split(dirName)
head,tail = os.path.split(head)
sys.path.append(head+'/generator_v1/python/')
import rosgen as rosgen

class EditorFrame(Frame):
    def __init__(self, master, label, height, width, contextDict):
        Frame.__init__(self,master)
        Frame.config(self,bd=2, relief=SUNKEN)

        self.height=height
        self.width=width

        self.label = Label(self, text=label, anchor=N, bg="dark gray", fg="white", relief=RAISED)
        self.label.pack()

        self.VScrollBar = Scrollbar(self, orient=VERTICAL)
        self.VScrollBar.pack(fill='y', side=RIGHT)
        self.HScrollBar = Scrollbar(self, orient=HORIZONTAL)
        self.HScrollBar.pack(side=BOTTOM, fill='x')

        self.canvas = Canvas(
            self,
            width=self.width, 
            height=self.height, 
            scrollregion=(0,0,self.width,self.height),
            xscrollcommand=self.HScrollBar.set,
            yscrollcommand=self.VScrollBar.set
        )

        self.contextMenu = MenuPopup(
            master=self.canvas,
            functionsDict=contextDict
        )
        self.canvas.tag_bind("object","<Button-3>", self.contextMenu.popupCallback)
        
        self.VScrollBar.config(command=self.canvas.yview)
        self.HScrollBar.config(command=self.canvas.xview)

        self.canvas.bind("<MouseWheel>", self._mouse_wheel)
        self.canvas.bind("<Button-4>", self._mouse_wheel)
        self.canvas.bind("<Button-5>", self._mouse_wheel)

        self.canvas.bind("<Button-1>", self._button1_callback)
        self.canvas.bind("<Button-3>", self.contextMenu.popupCallback)
        self.canvas.bind("<Delete>",self._delete_callback)

        self.canvas.pack(fill=BOTH)
        self.pack(fill=BOTH)

    def Update(self):
        pass

    def _button1_callback(self,event):
        closeAllContextMenus()
        self.focus_set()
        self.canvas.focus_set()

    def _button2_callback(self,event):
        self.canvas.tag_bind("object","<Button-3>", self.contextMenu.popupCallback)

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
    def __init__(self, master, height, width, optionsDict=None, model=None,contextDict=None):

        self.editorContextDict = OrderedDict()

        self.canvasContextDict=OrderedDict()
        self.canvasContextDict['Add Service'] = self.ContextAddService
        self.canvasContextDict['Add Message'] = self.ContextAddMessage
        self.canvasContextDict['Add Component Definition'] = self.ContextAddComponent
        self.canvasContextDict['Add Node'] = self.ContextAddNode

        self.srvContextDict = OrderedDict()
        self.msgContextDict = OrderedDict()

        self.compContextDict = OrderedDict()
        self.tmrContextDict = OrderedDict()
        self.pubContextDict = OrderedDict()
        self.subContextDict = OrderedDict()
        self.serverContextDict = OrderedDict()
        self.clientContextDict = OrderedDict()

        self.nodeContextDict = OrderedDict()
        self.compInstanceContextDict = OrderedDict()

        self.editorContextDict['canvas'] = self.canvasContextDict
        
        self.editorContextDict['service'] = self.srvContextDict
        self.editorContextDict['message'] = self.msgContextDict
        
        self.editorContextDict['component'] = self.compContextDict
        self.editorContextDict['timer'] = self.tmrContextDict
        self.editorContextDict['publisher'] = self.pubContextDict
        self.editorContextDict['subscriber'] = self.subContextDict
        self.editorContextDict['server'] = self.serverContextDict
        self.editorContextDict['client'] = self.clientContextDict
        
        self.editorContextDict['node'] = self.nodeContextDict
        self.editorContextDict['compInst'] = self.compInstanceContextDict

        self.srvContextDict['Edit Service'] = self.ContextEdit
        self.srvContextDict['Delete Service'] = self.ContextDelete

        self.msgContextDict['Edit Message'] = self.ContextEdit
        self.msgContextDict['Delete Message'] = self.ContextDelete

        self.tmrContextDict['Edit Timer'] = self.ContextEdit
        self.tmrContextDict['Delete Timer'] = self.ContextDelete

        self.pubContextDict['Edit Publisher'] = self.ContextEdit
        self.pubContextDict['Delete Publisher'] = self.ContextDelete

        self.subContextDict['Edit Subscriber'] = self.ContextEdit
        self.subContextDict['Delete Subscriber'] = self.ContextDelete

        self.serverContextDict['Edit Server'] = self.ContextEdit
        self.serverContextDict['Delete Server'] = self.ContextDelete

        self.clientContextDict['Edit Client'] = self.ContextEdit
        self.clientContextDict['Delete Client'] = self.ContextDelete

        self.compContextDict['Edit Component Definition'] = self.ContextEdit
        self.compContextDict['Delete Component Definition'] = self.ContextDelete
        self.compContextDict['Add Timer'] = self.CompAddTimer
        self.compContextDict['Add Publisher'] = self.CompAddPub
        self.compContextDict['Add Subscriber'] = self.CompAddSub
        self.compContextDict['Add Client'] = self.CompAddClient
        self.compContextDict['Add Server'] = self.CompAddServer
        
        self.compInstanceContextDict['Edit Component Instance'] = self.ContextEdit
        self.compInstanceContextDict['Delete Component Instance'] = self.ContextDelete

        self.nodeContextDict['Edit Node'] = self.ContextEdit
        self.nodeContextDict['Delete Node'] = self.ContextDelete
        self.nodeContextDict['Add Component Instance'] = self.NodeAddComp

        EditorFrame.__init__(self,master,"Model Viewer",height,width,self.editorContextDict)

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

    def Update(self, model, initPos, padding):
        self.model = model
        self.initPos = initPos
        self.padding = padding
        self.clear()
        if self.model != None:
            self.drawModel(self.initPos,self.padding)
        EditorFrame.Update(self)

    def SetActiveObject(self, object):
        self.activeObject = object

    def ConfigureNewObject(self, object, dictKey, pos=[0,0]):
        object.setCanvasParams(
            canvas = self.canvas,
            position = pos,
            canvasOptions = self.optionsDict[dictKey]
        )
        object.package = self.model

    def ContextAddService(self):
        newObj = rosgen.ROS_Service()
        self.ConfigureNewObject(newObj,'service')
        if newObj.Edit():
            self.model.addService(newObj)
            self.Update(self.model,self.initPos,self.padding)

    def ContextAddMessage(self):
        newObj = rosgen.ROS_Message()
        self.ConfigureNewObject(newObj,'message')
        if newObj.Edit():
            self.model.addMessage(newObj)
            self.Update(self.model,self.initPos,self.padding)

    def ContextAddComponent(self):
        newObj = rosgen.ROS_Component()
        self.ConfigureNewObject(newObj,'component')
        if newObj.Edit():
            self.model.addComponent(newObj)
            self.Update(self.model,self.initPos,self.padding)

    def ContextAddNode(self):
        newObj = rosgen.ROS_Node()
        self.ConfigureNewObject(newObj,'node')
        if newObj.Edit():
            self.model.addNode(newObj)
            self.Update(self.model,self.initPos,self.padding)

    def ContextDelete(self):
        self.activeObject.Delete()
        self.Update(self.model,self.initPos,self.padding)

    def ContextEdit(self):
        self.activeObject.Edit()
        self.Update(self.model,self.initPos,self.padding)

    def CompAddServer(self):
        newObj = rosgen.ROS_Server()
        self.ConfigureNewObject(newObj,'server')
        newObj.parentComp = self.activeObject
        if newObj.Edit():
            self.activeObject.addServer(newObj)
            self.Update(self.model,self.initPos,self.padding)

    def CompAddClient(self):
        newObj = rosgen.ROS_Client()
        self.ConfigureNewObject(newObj,'client')
        newObj.parentComp = self.activeObject
        if newObj.Edit():
            self.activeObject.addClient(newObj)
            self.Update(self.model,self.initPos,self.padding)

    def CompAddSub(self):
        newObj = rosgen.ROS_Subscriber()
        self.ConfigureNewObject(newObj,'subscriber')
        newObj.parentComp = self.activeObject
        if newObj.Edit():
            self.activeObject.addSubscriber(newObj)
            self.Update(self.model,self.initPos,self.padding)

    def CompAddPub(self):
        newObj = rosgen.ROS_Publisher()
        self.ConfigureNewObject(newObj,'publisher')
        newObj.parentComp = self.activeObject
        if newObj.Edit():
            self.activeObject.addPublisher(newObj)
            self.Update(self.model,self.initPos,self.padding)

    def CompAddTimer(self):
        newObj = rosgen.ROS_Timer()
        self.ConfigureNewObject(newObj,'timer')
        newObj.parentComp = self.activeObject
        if newObj.Edit():
            self.activeObject.addTimer(newObj)
            self.Update(self.model,self.initPos,self.padding)

    def NodeAddComp(self):
        newObj = rosgen.ROS_Component(isObjRef=True)
        self.ConfigureNewObject(newObj,'component')
        newObj.parentNode = self.activeObject
        if newObj.Edit():
            self.activeObject.addComponent(newObj)
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
            self.ConfigureNewObject(object,dictKey,[initX,ypos])
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
        initX = 0#self.initPos[0]
        initY = self.initPos[1]
        padX = self.padding[0]
        padY = self.padding[1]
        maxBaseNameLen = 0
        for key,obj in self.model.srv_dict.iteritems():
            if len(obj.name) > maxBaseNameLen:
                maxBaseNameLen = len(obj.name)
        for key,obj in self.model.msg_dict.iteritems():
            if len(obj.name) > maxBaseNameLen:
                maxBaseNameLen = len(obj.name)
        initX += maxBaseNameLen * DefaultFontOptions.width + DefaultDrawOptions.minSize[0]
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
        xpos = initX
        maxCompNameLen = 0
        for key,obj in self.model.component_definition_dict.iteritems():
            if len(obj.name) > maxCompNameLen:
                maxCompNameLen = len(obj.name)
        xpos += maxCompNameLen * DefaultFontOptions.width / 6
        xpos,ypos = self.drawObjectsFromDict(
            'component',
            self.model.component_definition_dict,
            xpos + padX,initY,
            padY
        )
        #column 3
        xpos += maxCompNameLen * DefaultFontOptions.width / 6
        maxNodeNameLen = 0
        for key,obj in self.model.nodes_dict.iteritems():
            if len(obj.name) > maxNodeNameLen:
                maxNodeNameLen = len(obj.name)
        xpos += maxNodeNameLen * DefaultFontOptions.width / 6
        xpos,ypos = self.drawObjectsFromDict(
            'node',
            self.model.nodes_dict,
            xpos+padX,initY,
            padY
        )
        self.canvas.configure(scrollregion=self.canvas.bbox('all'))

class PackageViewer(EditorFrame):
    def __init__(self, master, height, width, model, buttonVar, buttonCommand,contextDict=None):
    
        self.editorContextDict = OrderedDict()
        self.canvasContextDict=OrderedDict()
        self.canvasContextDict['Add New Package'] = self.ContextAddPackage
        self.canvasContextDict['Edit Selected Package'] = self.ContextEditPackage
        self.canvasContextDict['Delete Selected Package'] = self.ContextDeletePackage
        self.editorContextDict['canvas'] = self.canvasContextDict

        EditorFrame.__init__(self,master,"Package Viewer",height,width,self.editorContextDict)
        self.padding = [5,25]
        self.buttons = []
        self.model = model
        self.buttonVar = buttonVar
        self.buttonCommand = buttonCommand
        self.Update(self.model,self.buttonVar,self.buttonCommand)

    def ConfigureNewObject(self, object):
        object.canvas = self.canvas
        object.workspace = self.model

    def clear(self):
        for button in self.buttons:
            self.canvas.delete(button[0])
            button[1].destroy()
        self.buttons = []

    def ContextEditPackage(self):
        if self.buttonVar.get() != None and self.buttonVar.get() != "":
            package=self.model.packages_dict[self.buttonVar.get()]
            package.Edit()
            self.Update(self.model,self.buttonVar,self.buttonCommand)
            self.buttonVar.set(package.name)
            self.buttonCommand()
        else:
            tkMessageBox.showerror("Edit Package","Need to select a package first!")

    def ContextAddPackage(self):
        newObj = rosgen.ROS_Package()
        self.ConfigureNewObject(newObj)
        d = EditorDialogPopup(parent=self.master,title="New Package")
        if d.result != None:
            newObj.name = d.result[0]
            self.model.addPackage(newObj)
            self.Update(self.model,self.buttonVar,self.buttonCommand)
            self.buttonVar.set(newObj.name)
            self.buttonCommand()

    def ContextDeletePackage(self):
        if self.buttonVar.get() != None and self.buttonVar.get() != "":
            self.model.deletePackage(self.buttonVar.get())
            self.Update(self.model,self.buttonVar,self.buttonCommand)
            if self.model.packages_dict != OrderedDict():
                package = self.model.packages_dict.itervalues().next()
                self.buttonVar.set(package.name)
            else:
                self.buttonVar.set('')
            self.buttonCommand()
        else:
            tkMessageBox.showerror("Delete Package","Need to select a package first!")

    def Update(self,model,buttonVar,buttonCommand):
        self.model = model
        self.buttonVar = buttonVar
        self.buttonCommand = buttonCommand
        self.clear()
        ypos = 5
        for name,package in self.model.packages_dict.iteritems():
            self.ConfigureNewObject(package)
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
        self.canvas.configure(scrollregion=self.canvas.bbox('all'))

class Editor():
    def __init__(self,master,height,width,splitWidth,editorDict=None,model=None):
        self.master = master
        self.height = height
        self.width = width
        self.editorDict = editorDict
        self.model = model
        self.splitWidth = splitWidth
        self.selectedPackageVar = StringVar()

        self.editorPane = PanedWindow(
            self.master,
            orient = HORIZONTAL,
            opaqueresize = True,
            height = self.height,
            width = self.width
        )

        self.modelViewer = None
        self.packageViewer = None
        
        self.clear()

    def clear(self):
        if self.modelViewer != None:
            self.editorPane.forget(self.modelViewer)
            self.modelViewer = None
        if self.packageViewer == None:
            self.packageViewer = PackageViewer (
                master = self.master,
                height = self.height,
                width = self.splitWidth,
                model=self.model,
                buttonVar = self.selectedPackageVar,
                buttonCommand = self.OnPackageSelected
            )
            self.editorPane.add(self.packageViewer)
        self.editorPane.pack(fill='both',expand=1)

    def initModelViewer(self):
        if self.modelViewer == None:
            self.modelViewer = ModelViewer(
                master = self.master,
                height = self.height,
                width = self.width - self.splitWidth,
                optionsDict = self.editorDict,
                model=self.model
            )
            self.editorPane.add(self.modelViewer)
            self.editorPane.pack(fill='both',expand=1)

    def reset(self,model):
        self.model=model
        self.model.canvas = self.master
        self.clear()
        self.packageViewer.Update(
            model=self.model,
            buttonVar = self.selectedPackageVar,
            buttonCommand = self.OnPackageSelected
        )

    def OnPackageSelected(self):
        newModel = None
        self.initModelViewer()
        self.modelViewer.Update(
            None,
            initPos=[50,50],
            padding=[100,20]
        )
        if self.selectedPackageVar.get() != None and self.selectedPackageVar.get() != '':
            newModel = self.model.packages_dict[self.selectedPackageVar.get()]
            self.modelViewer.Update(
                newModel,
                initPos=[50,50],
                padding=[100,20]
            )
        
        
        
        

        
