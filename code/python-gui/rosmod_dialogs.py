#!/usr/bin/python
'''
'''

from Tkinter import *
import tkFileDialog
import tkMessageBox

from collections import OrderedDict

from CanvasOptions import *

activeMenus = []

canvasOptionsDict = OrderedDict()
canvasPaddingOptions = PaddingOptions((5,10),(0,15))
canvasFontOptions = FontOptions()
canvasOptionsDict['service'] = CanvasOptions(
    canvasPaddingOptions,
    canvasFontOptions,
    DrawOptions(
        color = "green",
        minSize = (10,10),
        textPosition = "LEFT"
    ),
    tags = ("object","service")
)
canvasOptionsDict['message'] = CanvasOptions(
    canvasPaddingOptions,
    canvasFontOptions,
    DrawOptions(
        color = "blue",
        minSize = (10,10),
        textPosition = "LEFT"
    ),
    tags = ("object","message")
)
canvasOptionsDict['server'] = CanvasOptions(
    canvasPaddingOptions,
    canvasFontOptions,
    DrawOptions(
        color = "green",
        minSize = (10,10),
    ),
    tags = ("object","service")
)
canvasOptionsDict['client'] = CanvasOptions(
    canvasPaddingOptions,
    canvasFontOptions,
    DrawOptions(
        color = "green",
        minSize = (10,10),
    ),
    tags = ("object","service")
)
canvasOptionsDict['publisher'] = CanvasOptions(
    canvasPaddingOptions,
    canvasFontOptions,
    DrawOptions(
        color = "blue",
        minSize = (10,10),
    ),
    tags = ("object","message")
)
canvasOptionsDict['subscriber'] = CanvasOptions(
    canvasPaddingOptions,
    canvasFontOptions,
    DrawOptions(
        color = "blue",
        minSize = (10,10),
    ),
    tags = ("object","message")
)
canvasOptionsDict['timer'] = CanvasOptions(
    canvasPaddingOptions,
    canvasFontOptions,
    DrawOptions(
        color = "gray",
        minSize = (10,10),
    ),
    tags = ("object","timer")
)
canvasOptionsDict['component'] = CanvasOptions(
    canvasPaddingOptions,
    canvasFontOptions,
    DrawOptions(
        color = "red",
        minSize = (10,10),
    ),
    tags = ("object","component")
)
canvasOptionsDict['compInst'] = CanvasOptions(
    canvasPaddingOptions,
    canvasFontOptions,
    DrawOptions(
        color = "red",
        minSize = (10,10),
    ),
    tags = ("object","compInst")
)
canvasOptionsDict['node'] = CanvasOptions(
    canvasPaddingOptions,
    canvasFontOptions,
    DrawOptions(
        color = "cyan",
        minSize = (10,10),
    ),
    tags = ("object","node")
)


def closeAllContextMenus():
    global activeMenus
    for menu in activeMenus:
        menu.unpost()    

def registerContextMenu(menu):
    global activeMenus
    activeMenus.append(menu)

# USED FOR RIGHT CLICK MENU FOR OBJECTS (deletion, addition, etc)
class MenuPopup():
    def __init__(self,master,functionsDict=None, setterFunc=None):
        self.master = master
        self.contextMenu = Menu(self.master, tearoff=0)
        self.functionsDict = functionsDict
        self.setterFunc = setterFunc
        self.numCommands = 0
        
    def destroy(self):
        self.contextMenu.destroy()

    def popupCallback(self,event):
        x = self.master.canvasx(event.x)
        y = self.master.canvasy(event.y)
        objID = self.master.find_overlapping(x,y,x+1,y+1)#.find_closest(x,y)[0]
        if objID == ():
            objTags = ['object','canvas']
        else:
            objTags = self.master.gettags(objID[-1])
            if objTags[0] == "text":
                objTags = self.master.gettags(objID[1]) # 0 is canvas, 2 is the text we just had
        closeAllContextMenus()
        registerContextMenu(self.contextMenu)
        self.contextMenu.delete(0,self.numCommands)
        self.functions = self.functionsDict[objTags[1]]
        if self.functions != None:
            for name,callback in self.functions.iteritems():
                self.contextMenu.add_command(label=name, command=callback)
            self.numCommands = len(self.functions)
        if objTags[1] != "canvas" and self.setterFunc != None:
            self.setterFunc(canvasObjectDict[objTags[2]])

        self.contextMenu.post(event.x_root,event.y_root)



