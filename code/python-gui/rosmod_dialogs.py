#!/usr/bin/python
'''
'''

from Tkinter import *
import tkFileDialog
import tkMessageBox
import tkSimpleDialog

from collections import OrderedDict

from CanvasOptions import *

activeMenus = []

editorDialogsDict = OrderedDict()
nameDialogDict = OrderedDict()
nodeDialogDict = OrderedDict()
compDialogDict = OrderedDict()
msgDialogDict = OrderedDict()
srvDialogDict = OrderedDict()
tmrDialogDict = OrderedDict()
editorDialogsDict['name'] = nameDialogDict
editorDialogsDict['node'] = nodeDialogDict
editorDialogsDict['component'] = compDialogDict
editorDialogsDict['message'] = msgDialogDict
editorDialogsDict['service'] = srvDialogDict
editorDialogsDict['timer'] = tmrDialogDict

canvasOptionsDict = OrderedDict()
canvasPaddingOptions = PaddingOptions((5,10),(0,15))
canvasFontOptions = FontOptions()
canvasOptionsDict['service'] = CanvasOptions(
    canvasPaddingOptions,
    canvasFontOptions,
    DrawOptions(
        color = "green",
        minSize = (10,10),
    ),
    tags = ("object","service")
)
canvasOptionsDict['message'] = CanvasOptions(
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
canvasOptionsDict['node'] = CanvasOptions(
    canvasPaddingOptions,
    canvasFontOptions,
    DrawOptions(
        color = "cyan",
        minSize = (10,10),
    ),
    tags = ("object","node")
)

badChars = ";,. `~[]{}:'\"/?\\<>*&^%$#@!()"

def badCharsInString(string):
    global badChars
    for char in badChars:
        if char in string:
            return True
    return False

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
        objID = self.master.find_closest(event.x, event.y)[0]
        objTags = self.master.gettags(objID)
        if objTags[0] == "text":
            objID = self.master.find_overlapping(event.x,event.y,event.x+1,event.y+1)
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

class EditorDialogPopup(tkSimpleDialog.Dialog):
    def body(self,master):
        self.entries = []

        Label(master, text="Name:").grid(row=0)

        entry1 = Entry(master)
        self.entries.append(entry1)
        self.entries[-1].grid(row=0,column=1)

        return self.entries[0] # initial focus

    def validate(self):
        for entry in self.entries:
            val = entry.get()
            if badCharsInString(val):
                tkMessageBox.showwarning(
                    "Bad Input",
                    "Input cannot contain\n{0}\n, try again.".format(badChars)
                )
                return 0
        return 1

    def apply(self):
        for entry in self.entries:
            val = entry.get()
            self.result.append(val)

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

