#!/usr/bin/python
'''
'''

from Tkinter import *
import tkFileDialog
import tkMessageBox

from collections import OrderedDict

from rosmod_classes import *

# USED FOR RIGHT CLICK MENU FOR OBJECTS (deletion, addition, etc)
class MenuPopup():
    def __init__(self,master):
        self.master = master
        self.functions = OrderedDict()

# anything drawn in the editor is of this type
class EditorObject():
    def __init__(self,canvas, name, objRef, objPadding, childPadding, position, color, tags = (), drawChildren = True, connectFromObject=True):
        self.canvas = canvas

        self.fontWidth = 11
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

        # these will be generated
        self.maxChildNameLen = 0
        self.connectionPoint = [0,0]
        self.size = [0,0]
        self.children = OrderedDict()

        self.canvas.tag_bind(self.name,"<Double-Button-1>", self.OnDoubleClick)
        self.canvas.tag_bind(self.name,"<Button-1>", self.OnLeftClick)
        self.canvas.tag_bind(self.name,"<Button-3>", self.OnRightClick)

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
        print self.name, self.objRef

    def OnLeftClick(self,event):
        print self.name, self.objRef

    def OnDoubleClick(self,event):
        print self.name, self.objRef

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

class TextPopup():
    def __init__(self, master,objName,objTextVar,width,height):
        self.master = master
        self.frame = Frame(master=self.master,bg="gray",height=height,width=width)
        self.label = Label(self.frame, text=objName, anchor=N, bg="dark gray", fg="white", relief=RAISED)
        self.label.pack()
        self.window = self.master.create_window(
            0,0, 
            anchor=NW, 
            window=self.frame, 
            width=width,
            height=height
        )
        self.textVar = objTextVar
        self.text = Text(self.frame)
        self.scrollbar = Scrollbar(self.frame)
        self.scrollbar.pack(side=RIGHT, fill=Y)
        self.scrollbar.config(command=self.text.yview)
        self.text.config(yscrollcommand=self.scrollbar.set)
        self.button = Button(self.frame,text="Save",command=self._close_Callback)
        self.button.pack(side=BOTTOM)
        self.text.pack()
        self.text.insert(END,objTextVar.get())
        self.master.config(state=DISABLED)

    def _close_Callback(self):
        self.scrollbar.destroy()
        self.button.destroy()
        self.textVar.set(self.text.get(1.0,END))
        self.text.destroy()
        self.frame.destroy()
        self.label.destroy()
        self.master.delete(self.window)
        self.master.config(state=NORMAL)

class EditorFrame(Frame):
    def __init__(self, master, label, height, width, maxHeight, maxWidth):
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

        self.canvas.bind("<Button-1>", self._button1_callback)
        self.canvas.bind("<Delete>",self._delete_callback)

        self.canvas.pack(fill=BOTH)
        self.pack(fill=BOTH)

        self.objects = OrderedDict()

    def _button1_callback(self,event):
        self.focus_set()
        self.canvas.focus_set()

    def _delete_callback(self,event):
        print "delete has been pressed"

    def _mouse_wheel(self, event):
        direction = 0
        # respond to Linux or Windows wheel event
        if event.num == 5 or event.delta == -120:
            direction = 1
        if event.num == 4 or event.delta == 120:
            direction = -1
        self.canvas.yview_scroll(direction, "units")
        
class ModelViewer(EditorFrame):
    def __init__(self, master, height, width, maxHeight, maxWidth, displayMapping=None, model=None):
        EditorFrame.__init__(self,master,"Model Viewer",height,width,maxHeight,maxWidth)
        if model != None:
            self.model=model
        else:
            self.model=Model()

        self.displayMapping = displayMapping

        self.drawModel(initX=50, initY=50, padX=100, padY=20)

        self.canvas.tag_bind("object", "<Button-1>", self.OnObjectLeftClick)
        # make tag so that right click on object can be used to inspect code
        self.canvas.tag_bind("object", "<Button-3>", self.OnObjectRightClick)

        self.var = StringVar()
        self.var.trace("w", self.OnTextUpdate)
        self.activeObject=None
        self.entry=None
        self.objects = OrderedDict()

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

    def OnObjectRightClick(self, event):
        self.focus_set()
        self.canvas.focus_set()
        selectedObject = self.canvas.find_closest(
            self.canvas.canvasx(event.x), 
            self.canvas.canvasy(event.y)
        )[0]
        tags = self.canvas.gettags(selectedObject)
        if tags[1] == 'message':
            self.activeObject = (self.model.messages,tags[2])
            self.var.set(self.model.messages[tags[2]])
            self.entry = TextPopup(self.canvas,tags[2],self.var,200,200)
        elif tags[1] == 'service':
            self.activeObject = (self.model.services,tags[2])
            self.var.set(self.model.services[tags[2]])
            self.entry = TextPopup(self.canvas,tags[2],self.var,200,200)
        else:
            self.activeObject = None

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
                connectFromObject=isOnCanvas
            )
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
                self.displayMapping[dictKey][0]
            )
            if dictKey == 'component':
                self.createAndAddChildren(name,object.clients,'service',initX,ypos)
                self.createAndAddChildren(name,object.servers,'service',initX,ypos)
                self.createAndAddChildren(name,object.publishers,'message',initX,ypos)
                self.createAndAddChildren(name,object.subscribers,'message',initX,ypos)
                self.createAndAddChildren(name,object.timers,'timer',initX,ypos,False)
            elif dictKey == 'node':
                self.createAndAddChildren(name,object.components,'component',initX,ypos)
            width,height = self.objects[name].Draw()
            if (width + initX) > maxX:
                maxX = width + initX
            ypos += height + padY
        maxY = ypos
        return (maxX,maxY)

    def drawModel(self, initX,initY, padX, padY):
        #column 1
        xpos,ypos = self.drawObjectsFromDict('service',self.model.services,initX,initY,padY)
        xpos2,ypos = self.drawObjectsFromDict('message',self.model.messages,initX,ypos,padY)
        #column 2
        xpos,ypos = self.drawObjectsFromDict('component',self.model.components,xpos + padX,initY,padY)
        #column 3
        xpos,ypos = self.drawObjectsFromDict('node',self.model.nodes,xpos+padX,initY,padY)

class Editor():
    def __init__(self,master,height,width,splitWidth,maxWidth,maxHeight,editorDict=None,model=None):
        self.master = master
        self.height = height
        self.width = width
        self.editorDict = editorDict
        self.model = model
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
            width = self.width - splitWidth,
            maxHeight = maxHeight,
            maxWidth = maxWidth,
            displayMapping = self.editorDict,
            model=self.model
        )
        self.editorPane.add(self.modelViewer)
        self.editorPane.pack(fill='both',expand=1)
