#!/usr/bin/python
'''
'''

from Tkinter import *
import tkFileDialog
import tkMessageBox

from collections import OrderedDict

from rosmod_dialogs import *

# anything drawn in the editor is of this type
class EditorObject():
    def __init__(self,canvas, name, objRef, objPadding, childPadding, position, color, contextFunctionDict=None, tags = (), drawChildren = True, connectFromObject=True):
        self.canvas = canvas

        self.fontWidth = 8
        self.fontHeight = 10

        # these need to be provided
        self.objRef = objRef
        self.name = name
        self.objectPadding = objPadding # (10,10)
        self.childPadding = childPadding # (0,15)
        self.drawChildren = drawChildren # True
        self.position = position # (0,0)
        self.tags = tags + (self.name,None)
        self.color = color
        self.connectFromObject = connectFromObject
        self.contextFunctions = contextFunctionDict

        # these will be generated
        self.maxChildNameLen = 0
        self.connectionPoint = [0,0]
        self.size = [0,0]
        self.children = OrderedDict()

        self.canvas.tag_bind(self.name,"<Button-1>", self.OnLeftClick)
        self.canvas.tag_bind(self.name,"<Double-Button-1>", self.OnDoubleClick)
        self.contextMenu = MenuPopup(self.canvas,contextFunctionDict)
        self.canvas.tag_bind(self.name,"<Button-3>", self.contextMenu.popupCallback)

    def __str__(self):
        retStr = ""
        retStr += "{0}".format(self.position)
        retStr += "{0}".format(self.connectionPoint)
        return retStr

    def addChild(self, name, obj):
        if len(name) > self.maxChildNameLen:
            self.maxChildNameLen = len(name)
        numChildren = len(self.children)
        offsetX = self.childPadding[0]
        offsetY = numChildren * self.childPadding[1] + self.objectPadding[1]/2
        obj.position = (self.position[0] + offsetX, self.position[1] + offsetY)
        self.children[name] = obj

    def OnRightClick(self, event):
        print "RIGHT CLICK",self.name

    def OnLeftClick(self,event):
        closeAllContextMenus()
        print "LEFT CLICK",self.name

    def OnDoubleClick(self,event):
        print "DOUBLE CLICK",self.name

    def Draw(self, textOnSide = False, drawArrowToObjRef=False):
        self.size[0] = self.objectPadding[0]
        self.size[1] = self.objectPadding[1]
        if self.drawChildren == True:
            self.size[0] += self.maxChildNameLen * self.fontWidth
            self.size[1] += len(self.children) * self.childPadding[1]
        self.connectionPoint = [
            self.position[0] + self.size[0],
            self.position[1] + self.size[1]/2
        ]
        objectID = self.canvas.create_rectangle(
            self.position[0], self.position[1], 
            self.position[0] + self.size[0], self.position[1] + self.size[1], 
            outline=self.color, fill=self.color, tags=self.tags,
            activeoutline="black",
            activewidth=3.0
        )
        textPos = (self.position[0] + self.size[0]/2, self.position[1] - self.fontHeight)
        anchor = CENTER
        if textOnSide == True:
            textPos = (self.connectionPoint[0] + 3, self.connectionPoint[1])
            anchor = W
        textID = self.canvas.create_text(
            textPos,
            text=self.name,
            state=DISABLED,
            tags = self.tags,
            anchor = anchor
        )
        if drawArrowToObjRef == True and self.connectFromObject == True:
            self.canvas.create_line(
                self.position[0],self.position[1]+self.size[1]/2,
                self.objRef.connectionPoint[0],self.objRef.connectionPoint[1],
                arrow=FIRST
            )
        if self.drawChildren == True:
            for name,child in self.children.iteritems():
                child.Draw(textOnSide=True,drawArrowToObjRef=True)
        return self.size

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
            tags="canvas"
        )

        self.contextMenu = MenuPopup(self.canvas,contextDict)
        self.canvas.tag_bind("canvas","<Button-3>", self.contextMenu.popupCallback)
        
        self.VScrollBar.config(command=self.canvas.yview)
        self.HScrollBar.config(command=self.canvas.xview)

        self.canvas.bind("<MouseWheel>", self._mouse_wheel)
        self.canvas.bind("<Button-4>", self._mouse_wheel)
        self.canvas.bind("<Button-5>", self._mouse_wheel)

        self.canvas.bind("<Button-1>", self._button1_callback)
        self.canvas.bind("<Delete>",self._delete_callback)

        self.canvas.pack(fill=BOTH)
        self.pack(fill=BOTH)

        self.objects = OrderedDict()

    def Update(self):
        self.canvasID = self.canvas.create_rectangle(
            0, 0, 
            self.maxWidth, self.maxHeight, 
            outline="light gray", fill="light gray", 
            tags="canvas"
        )
        self.canvas.tag_bind("canvas","<Button-3>", self.contextMenu.popupCallback)

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
    def __init__(self, master, height, width, maxHeight, maxWidth, displayMapping=None, model=None,contextDict=None):
        if contextDict==None:
            contextDict=OrderedDict()
            contextDict['Add Service'] = self.ContextAddService
            contextDict['Add Message'] = self.ContextAddMessage
            contextDict['Add Component'] = self.ContextAddComponent
            contextDict['Add Node'] = self.ContextAddNode

        EditorFrame.__init__(self,master,"Model Viewer",height,width,maxHeight,maxWidth,contextDict)

        self.editorObjectContextDict = OrderedDict()
        self.srvContextDict = OrderedDict()
        self.msgContextDict = OrderedDict()
        self.tmrContextDict = OrderedDict()
        self.compContextDict = OrderedDict()
        self.nodeContextDict = OrderedDict()
        self.editorObjectContextDict['service'] = self.srvContextDict
        self.editorObjectContextDict['message'] = self.msgContextDict
        self.editorObjectContextDict['timer'] = self.tmrContextDict
        self.editorObjectContextDict['component'] = self.compContextDict
        self.editorObjectContextDict['node'] = self.nodeContextDict
        self.compContextDict['Edit Component'] = self.ContextEdit
        self.srvContextDict['Edit Service'] = self.ContextEdit
        self.msgContextDict['Edit Message'] = self.ContextEdit
        self.tmrContextDict['Edit Timer'] = self.ContextEdit
        self.nodeContextDict['Edit Node'] = self.ContextEdit

        self.model=model
        self.displayMapping = displayMapping

        self.var = StringVar()
        self.var.trace("w", self.OnTextUpdate)
        self.activeObject=None
        self.entry=None
        self.objects = OrderedDict()

    def clear(self):
        self.canvas.delete(ALL)
        self.activeObject = None
        self.entry = None
        self.objects = OrderedDict()
        EditorFrame.Update(self)

    def Update(self, model,initX,initY, padX,padY):
        self.model = model
        self.clear()
        self.drawModel(initX,initY,padX,padY)

    def ContextAddService(self):
        print "Popup dialog for adding service"

    def ContextAddMessage(self):
        print "Popup dialog for adding message"

    def ContextAddComponent(self):
        print "Popup dialog for adding component"

    def ContextAddNode(self):
        print "Popup dialog for adding node"

    def ContextEdit(self):
        print "Editing active object {0}".format(self.var.get())

    def OnTextUpdate(self,*args):
        if self.activeObject != None:
            self.activeObject[0][self.activeObject[1]] = self.var.get()

    def OnObjectLeftClick(self, event):
        self.focus_set()
        self.canvas.focus_set()
        selectedObject = self.canvas.find_closest(
            self.canvas.canvasx(event.x), 
            self.canvas.canvasy(event.y)
        )[0]
        tags = self.canvas.gettags(selectedObject)

    def createAndAddChildren(self, objName, objDict, dispMapKey, initX, ypos, isOnCanvas=True):
        for childName, child in objDict.iteritems():
            objRef = child
            if isOnCanvas == True:
                objRef = self.objects[child.name]
            self.objects[childName] = EditorObject(
                self.canvas,
                childName,
                objRef,
                (10,10),
                (0,15),
                (initX,ypos),
                self.displayMapping[dispMapKey][0],
                contextFunctionDict=self.editorObjectContextDict[dispMapKey],
                connectFromObject=isOnCanvas
            )
            self.objects[childName].contextMenu.objVar = self.var
            self.objects[childName].contextMenu.objVarValue = childName
            self.objects[objName].addChild(childName,self.objects[childName])

    def drawObjectsFromDict(self, dictKey, drawDict, initX, initY, padY):
        ypos = initY
        maxX = 0
        for name,object in drawDict.iteritems():
            self.objects[name] = EditorObject(
                self.canvas, 
                name, 
                object, 
                (10,10), 
                (0,15), 
                (initX,ypos),
                self.displayMapping[dictKey][0],
                contextFunctionDict=self.editorObjectContextDict[dictKey]
            )
            self.objects[name].contextMenu.objVar = self.var
            self.objects[name].contextMenu.objVarValue = name
            if dictKey == 'component':
                self.createAndAddChildren(name,object.required_services_dict,'service',initX,ypos)
                self.createAndAddChildren(name,object.provided_services_dict,'service',initX,ypos)
                self.createAndAddChildren(name,object.publishers_dict,'message',initX,ypos)
                self.createAndAddChildren(name,object.subscribers_dict,'message',initX,ypos)
                self.createAndAddChildren(name,object.timers_dict,'timer',initX,ypos,False)
            elif dictKey == 'node':
                self.createAndAddChildren(name,object.component_instance_dict,'component',initX,ypos)
            width,height = self.objects[name].Draw()
            if (width + initX) > maxX:
                maxX = width + initX
            ypos += height + padY
        maxY = ypos
        return (maxX,maxY)

    def drawModel(self, initX,initY, padX, padY):
        #column 1
        xpos,ypos = self.drawObjectsFromDict('service',self.model.srv_dict,initX,initY,padY)
        xpos2,ypos = self.drawObjectsFromDict('message',self.model.msg_dict,initX,ypos,padY)
        #column 2
        xpos,ypos = self.drawObjectsFromDict('component',self.model.component_definition_dict,xpos + padX,initY,padY)
        #column 3
        xpos,ypos = self.drawObjectsFromDict('node',self.model.nodes_dict,xpos+padX,initY,padY)

class PackageViewer(EditorFrame):
    def __init__(self, master, height, width, maxHeight, maxWidth, packageDict, buttonVar, buttonCommand,contextDict=None):
    
        if contextDict==None:
            contextDict=OrderedDict()
            contextDict['Add Package'] = self.ContextAddPackage

        EditorFrame.__init__(self,master,"Package Viewer",height,width,maxHeight,maxWidth,contextDict)
        self.padX = 5
        self.padY = 25
        self.buttons = []
        self.Update(packageDict,buttonVar,buttonCommand)

    def ContextAddPackage(self):
        print "Popup dialog for adding package"

    def Update(self,packageDict,buttonVar,buttonCommand):
        for button in self.buttons:
            self.canvas.delete(button[0])
            button[1].destroy()
        self.buttons=[]
        ypos = 5
        for name,package in packageDict.iteritems():
            newButton = Radiobutton(
                self.canvas, 
                text = name, 
                variable=buttonVar, 
                value=name, 
                indicatoron=0, 
                command = buttonCommand
            )
            self.buttons.append(
                [self.canvas.create_window((self.padX,ypos),window=newButton,anchor=NW),
                 newButton]
            )
            ypos += self.padY

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
            displayMapping = self.editorDict,
            model=self.model
        )
        self.packageViewer = PackageViewer (
            master = self.master,
            height = self.height,
            width = self.splitWidth,
            maxHeight = self.maxHeight,
            maxWidth = self.maxWidth,
            packageDict=self.model.packages_dict,
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
            packageDict=self.model.packages_dict,
            buttonVar = self.selectedPackageVar,
            buttonCommand = self.OnPackageSelected
        )

    def OnPackageSelected(self):
        print "You've selected {0}".format(self.selectedPackageVar.get())
        self.modelViewer.Update(
            self.model.packages_dict[self.selectedPackageVar.get()],
            initX=50,initY=50,padX=100,padY=20
        )
        
        
        
        

        
