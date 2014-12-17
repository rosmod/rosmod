#!/usr/bin/python
'''
'''

from Tkinter import *
import tkFileDialog
import tkMessageBox

from collections import OrderedDict

from rosmod_classes import *

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

    def _create_object(self, name, coord, size, color, tagTuple):
        (x,y) = coord
        objectID = self.canvas.create_rectangle(
            x-size/2, y-size/2, x+size/2, y+size/2, 
            outline=color, fill=color, tags=tagTuple,
            activeoutline="black",
            activewidth=3.0
        )
        textID = self.canvas.create_text(
            (x,y),
            text=name,
            state=DISABLED,
            tags=tagTuple
        )
        if len(tagTuple) > 3:
            #print "{0}:{1}\n{2}".format(tagTuple[1],tagTuple[2],tagTuple[3])
            if tagTuple[1] == 'node':
                print "NEED TO CONNECT COMPONENTS"
                self.objects[tagTuple[2]] = [tagTuple[3],objectID,textID,x,y]
                # need to make small boxes for components
                # need to draw text for component names
                # need to connect small boxes to their actual components
                for name,comp in tagTuple[3].components.iteritems():
                    self.canvas.create_line(
                        x,y,
                        self.objects[comp][2],self.objects[comp][3],
                        arrow=FIRST
                    )
            elif tagTuple[1] == 'component':
                self.objects[tagTuple[3]] = [objectID,textID,x,y]
                # need to make small boxes for pubs, subs, clients, servers, & timers
                # need to draw text for names
                # need to connect small boxes to their respective objects
                print "NEED TO CONNECT MESSAGES & SERVICES"
                for name,srv in tagTuple[3].clients.iteritems():
                    self.canvas.create_line(
                        x,y,
                        self.objects[srv.name][3],self.objects[srv.name][4],
                        arrow=FIRST
                    )
                for name,srv in tagTuple[3].servers.iteritems():
                    self.canvas.create_line(
                        x,y,
                        self.objects[srv.name][3],self.objects[srv.name][4],
                        arrow=FIRST
                    )
                for name,msg in tagTuple[3].subscribers.iteritems():
                    self.canvas.create_line(
                        x,y,
                        self.objects[msg.name][3],self.objects[msg.name][4],
                        arrow=FIRST
                    )
                for name,msg in tagTuple[3].publishers.iteritems():
                    self.canvas.create_line(
                        x,y,
                        self.objects[msg.name][3],self.objects[msg.name][4],
                        arrow=FIRST
                    )
            else:
                self.objects[tagTuple[2]] = [tagTuple[3],objectID,textID,x,y] 

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
        EditorFrame.__init__(self,master,"Object Browser",height,width,maxHeight,maxWidth)

        self.selectedObjectType = None

        ypos = self.width * 3 / 4
        for name,attr in objectDict.iteritems():
            self._create_object(name,(self.width/2, ypos), self.width * 3/4, attr[0], attr[1])
            ypos += self.width

        self.canvas.tag_bind("object", "<Button-1>", self.OnObjectLeftClick)


    def OnObjectLeftClick(self, event):
        self.focus_set()
        selectedObject = self.canvas.find_closest(
            self.canvas.canvasx(event.x), 
            self.canvas.canvasy(event.y)
        )[0]
        self.canvas.focus_set()
        self.selectedObjectType = self.canvas.gettags(selectedObject)[1]
        

    def GetActiveObjectType(self):
        return self.selectedObjectType
        
class ModelViewer(EditorFrame):
    def __init__(self, master, height, width, maxHeight, maxWidth, displayMapping=None, model=None):
        EditorFrame.__init__(self,master,"Model Viewer",height,width,maxHeight,maxWidth)
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

        self.canvas.tag_bind("object", "<Button-1>", self.OnObjectLeftClick)
        # make tag so that right click on object can be used to inspect code
        self.canvas.tag_bind("object", "<Button-3>", self.OnObjectRightClick)

        self.var = StringVar()
        self.var.trace("w", self.OnTextUpdate)
        self.activeObject=None
        self.entry=None

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

    def drawObjectsFromDict(self, dictKey, drawDict, initY, padY):
        ypos = initY
        for name,object in drawDict.iteritems():
            self._create_object(
                name,
                (self.displayLayout[dictKey][0],ypos),
                self.displayLayout[dictKey][1],
                self.displayMapping[dictKey][0],
                self.displayMapping[dictKey][1] + (name,object)
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
        self.objectList = ObjectList(
            master = self.master,
            objectDict = self.editorDict,
            height = self.height,
            width = splitWidth,
            maxHeight = maxHeight,
            maxWidth = splitWidth
        )
        self.modelViewer = ModelViewer(
            master = self.master,
            height = self.height,
            width = self.width - splitWidth,
            maxHeight = maxHeight,
            maxWidth = maxWidth - splitWidth,
            displayMapping = self.editorDict,
            model=self.model
        )
        self.editorPane.add(self.objectList)
        self.editorPane.add(self.modelViewer)
        self.editorPane.pack(fill='both',expand=1)
