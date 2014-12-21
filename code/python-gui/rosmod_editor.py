#!/usr/bin/python
'''
'''

from Tkinter import *
import tkFileDialog
import tkMessageBox

from collections import OrderedDict

from rosmod_dialogs import *

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

        self.compContextDict['Edit Component'] = self.ContextEdit

        self.srvContextDict['Edit Service'] = self.ContextEdit

        self.msgContextDict['Edit Message'] = self.ContextEdit

        self.tmrContextDict['Edit Timer'] = self.ContextEdit

        self.nodeContextDict['Edit Node'] = self.ContextEdit

        EditorFrame.__init__(self,master,"Model Viewer",height,width,maxHeight,maxWidth,self.editorContextDict)

        self.model=model
        self.optionsDict = optionsDict

        self.var = StringVar()
        self.activeObject=[]
        self.contextMenu.objVar = self.activeObject
        self.entry=None

    def clear(self):
        self.canvas.delete(ALL)
        self.activeObject = None
        self.entry = None
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
        print "Editing active object {0}".format(self.activeObject)

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

    def drawModel(self, initX,initY, padX, padY):
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
    def __init__(self, master, height, width, maxHeight, maxWidth, packageDict, buttonVar, buttonCommand,contextDict=None):
    
        self.editorContextDict = OrderedDict()
        self.canvasContextDict=OrderedDict()
        self.canvasContextDict['Add Package'] = self.ContextAddPackage
        self.editorContextDict['canvas'] = self.canvasContextDict

        EditorFrame.__init__(self,master,"Package Viewer",height,width,maxHeight,maxWidth,self.editorContextDict)
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
            optionsDict = self.editorDict,
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
        
        
        
        

        
