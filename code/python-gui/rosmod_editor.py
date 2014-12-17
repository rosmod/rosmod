#!/usr/bin/python
'''
'''

from Tkinter import *
import tkFileDialog
import tkMessageBox

from collections import OrderedDict

from rosmod_classes import *

class EditorFrame(Frame):
    def __init__(self, master, height, width, maxHeight, maxWidth):
        Frame.__init__(self,master)
        Frame.config(self,bd=2, relief=SUNKEN)

        self.height=height
        self.width=width

        self.VScrollBar = Scrollbar(self, orient=VERTICAL)
        self.VScrollBar.pack(fill='y', side=RIGHT)
        self.HScrollBar = Scrollbar(self, orient=HORIZONTAL)
        self.HScrollBar.pack(side=BOTTOM, fill='x')

        self.canvas = Canvas(
            self,
            width=width, 
            height=height, 
            scrollregion=(0,0,maxWidth,maxHeight),
            xscrollcommand=self.HScrollBar.set,
            yscrollcommand=self.VScrollBar.set
        )
        
        self.VScrollBar.config(command=self.canvas.yview)
        self.HScrollBar.config(command=self.canvas.xview)

        self.canvas.bind("<MouseWheel>", self._mouse_wheel)
        self.canvas.bind("<Button-4>", self._mouse_wheel)
        self.canvas.bind("<Button-5>", self._mouse_wheel)

        self.canvas.pack()
        self.pack()

    def _create_object(self, coord, size, color, tagTuple):
        (x,y) = coord
        self.canvas.create_rectangle(
            x-size/2, y-size/2, x+size/2, y+size/2, 
            outline=color, fill=color, tags=tagTuple
        )

    def _mouse_wheel(self, event):
        direction = 0
        # respond to Linux or Windows wheel event
        if event.num == 5 or event.delta == -120:
            direction = 1
        if event.num == 4 or event.delta == 120:
            direction = -1
        self.canvas.yview_scroll(direction, "units")

class ObjectList(EditorFrame):
    def __init__(self, master, objectDict, height, width, maxHeight, maxWidth):
        EditorFrame.__init__(self,master,height,width,maxHeight,maxWidth)

        self.objects = []
        self.numObjects = 100
        self.selectedObjectType = None

        ypos = self.width * 3 / 4
        for name,attr in objectDict.iteritems():
            self._create_object((self.width/2, ypos), self.width * 3/4, attr[0], attr[1])
            ypos += self.width

        self.canvas.tag_bind("object", "<Button-1>", self.OnObjectButtonPress)

    def OnObjectButtonPress(self, event):
        selectedObject = self.canvas.find_closest(
            self.canvas.canvasx(event.x), 
            self.canvas.canvasy(event.y)
        )[0]
        self.selectedObjectType = self.canvas.gettags(selectedObject)[1]

    def GetActiveObjectType(self):
        return self.selectedObjectType
        
class ModelViewer(EditorFrame):
    def __init__(self, master, height, width, maxHeight, maxWidth, displayMapping=None, model=None):
        EditorFrame.__init__(self,master,height,width,maxHeight,maxWidth)
        if model != None:
            self.model=model
        else:
            self.model=Model()

        self.displayMapping = displayMapping

        self.displayLayout = {}
        # value is [xpos, size]
        self.displayLayout['service'] = [50,80]
        self.displayLayout['message'] = [50,80]
        self.displayLayout['component'] = [150,80]
        self.displayLayout['node'] = [250,80]

        self.drawModel()

    def drawObjectsFromDict(self, dictKey, drawDict, initY, padY):
        ypos = initY
        for name,object in drawDict.iteritems():
            self._create_object(
                (self.displayLayout[dictKey][0],ypos),
                self.displayLayout[dictKey][1],
                self.displayMapping[dictKey][0],
                self.displayMapping[dictKey][1]
            )
            ypos += self.displayLayout[dictKey][1] + padY
        return ypos

    def drawModel(self):
        ypos = self.drawObjectsFromDict('service',self.model.services,50,20)
        self.drawObjectsFromDict('message',self.model.messages,ypos,20)
        self.drawObjectsFromDict('component',self.model.components,50,20)
        self.drawObjectsFromDict('node',self.model.nodes,50,20)

    def addService(self, name, definition = ''):
        self.model.addService(name,definition)
    
    def addMessage(self, name, definition = ''):
        self.model.addMessage(name,definition)

    def addComponent(self, name, component):
        self.model.addComponent(name, component)

    def addNode(self, name, node):
        self.model.addNode(name,node)

    def addPackage(self, name, package):
        self.model.addPackage(name,package)
