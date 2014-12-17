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
        self.displayLayout['service'] = 50
        self.displayLayout['message'] = 50
        self.displayLayout['component'] = 150
        self.displayLayout['node'] = 250

        self.drawModel()

    def drawModel(self):
        ypos = 50
        for name,service in self.model.services.iteritems():
            self._create_object(
                (self.displayLayout['service'], ypos), 
                80, 
                self.displayMapping['service'][0], 
                self.displayMapping['service'][1]
            )
            ypos += 100

        for name,message in self.model.messages.iteritems():
            self._create_object(
                (self.displayLayout['message'], ypos), 
                80, 
                self.displayMapping['message'][0], 
                self.displayMapping['message'][1]
            )
            ypos += 100

        ypos = 50
        for name,component in self.model.components.iteritems():
            self._create_object(
                (self.displayLayout['component'], ypos), 
                80, 
                self.displayMapping['component'][0], 
                self.displayMapping['component'][1]
            )
            ypos += 100

        ypos = 50
        for name,node in self.model.nodes.iteritems():
            self._create_object(
                (self.displayLayout['node'], ypos), 
                80, 
                self.displayMapping['node'][0], 
                self.displayMapping['node'][1]
            )
            ypos += 100

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
