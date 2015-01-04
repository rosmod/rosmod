from Tkinter import * 
import tkFileDialog
import tkMessageBox
import tkSimpleDialog
from collections import OrderedDict

canvasObjectDict = OrderedDict()

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

badCharString = ";,. `~[]{}:'\"/?\\<>*&^%$#@!()"

def badCharsInString(string,badChars=badCharString):
    for char in badChars:
        if char in string:
            return True
    return False

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
                    "Input cannot contain\n{0}\n, try again.".format(badCharString)
                )
                return 0
        return 1

    def apply(self):
        self.result= []
        for entry in self.entries:
            val = entry.get()
            self.result.append(val)

class PaddingOptions():
    def __init__(self, childOffset = (5,10), childPadding=(0,15)):
        self.childOffset = childOffset # (x,y) tuple
        self.childPadding = childPadding # (x,y) tuple

class FontOptions():
    def __init__(self, height=10, width=8, fg="black",bg=""):
        self.height = height
        self.width = width
        self.fg = fg
        self.bg = bg

class DrawOptions():
    def __init__(self, color="black", minSize=(10,10), textPosition="TOP", image=None, drawChildren=True, connectFrom=False):
        self.color = color
        self.minSize = minSize
        self.image = image
        self.textPosition = textPosition
        self.drawChildren = drawChildren
        self.connectFrom = connectFrom        

class CanvasOptions():
    def __init__(self, paddingOptions, fontOptions, drawOptions, tags=()):
        self.paddingOptions = paddingOptions
        self.fontOptions = fontOptions
        self.drawOptions = drawOptions
        self.tags = tags

    def copy(self,canvasOptions):
        self.paddingOptions = canvasOptions.paddingOptions
        self.fontOptions = canvasOptions.fontOptions
        self.drawOptions = canvasOptions.drawOptions
        self.tags = canvasOptions.tags

# Anything that can be drawn on a Tkinter canvas should inherit from this class 
class CanvasObject(CanvasOptions):
    def __init__(self, name = "", isObjRef = False, objRef = None):
        self.maxChildNameLen = 0
        self.connectionPoint = [-1,-1]
        self.size = [0,0]
        self.children = OrderedDict()
        self.isObjRef = isObjRef
        self.objRef = objRef
        self.name = name

    def setCanvasParams(self, canvas, position, canvasOptions):
        global canvasObjectDict
        CanvasOptions.copy(self,canvasOptions)
        # these need to be provided
        self.position = position
        self.canvas = canvas
        self.tags += (self.name,None)
        canvasObjectDict[self.name] = self
        
    def buildChildList(self):
        pass

    def Edit(self):
        result = self.name
        d = EditorDialogPopup(parent=self.canvas,title=result)
        if d.result != None:
            result = d.result[0]
        self.name = result
            

    def cleanChildren(self):
        self.children.clear()

    def addChild(self, name, child):
        if len(name) > self.maxChildNameLen:
            self.maxChildNameLen = len(name)
        numChildren = len(self.children)
        offsetX = self.paddingOptions.childOffset[0] + self.paddingOptions.childOffset[0]
        offsetY = numChildren * self.paddingOptions.childPadding[1] + self.paddingOptions.childOffset[1]
        child.position = (self.position[0] + offsetX, self.position[1] + offsetY)
        self.children[name] = child

    def Draw(self):
        self.size[0] = self.drawOptions.minSize[0]
        self.size[1] = self.drawOptions.minSize[1]
        if self.drawOptions.drawChildren == True and len(self.children) > 0:
            self.size[0] += self.maxChildNameLen * self.fontOptions.width + self.paddingOptions.childOffset[0] * 2
            self.size[1] += len(self.children) * self.paddingOptions.childPadding[1] + self.paddingOptions.childOffset[1]
        self.connectionPoint = [
            self.position[0] + self.size[0],
            self.position[1] + self.size[1]/2
        ]
        # need to take into account image here!
        objectID = self.canvas.create_rectangle(
            self.position[0], self.position[1], 
            self.position[0] + self.size[0], self.position[1] + self.size[1], 
            outline=self.drawOptions.color, fill=self.drawOptions.color, tags=self.tags,
            activeoutline="black",
            activewidth=3.0
        )
        # need to take into account text position here!
        if self.drawOptions.textPosition == "TOP":
            anchor = CENTER
            textPos = (self.position[0] + self.size[0]/2, self.position[1] - self.fontOptions.height)
        elif self.drawOptions.textPosition == "BOTTOM":
            anchor = CENTER
            textPos = (self.position[0] + self.size[0]/2, self.position[1] + self.size[1] + self.fontOptions.height)
        elif self.drawOptions.textPosition == "RIGHT":
            anchor = W
            textPos = (self.connectionPoint[0] + 3, self.connectionPoint[1])
        elif self.drawOptions.textPosition == "LEFT":
            anchor = E
            textPos = (self.position[0] - 3, self.connectionPoint[1])

        textID = self.canvas.create_text(
            textPos,
            text=self.name,
            state=DISABLED,
            tags = "text",
            anchor = anchor
        )
        if self.drawOptions.connectFrom == True and  self.objRef != None and self.objRef.connectionPoint[0] > 0:
            self.canvas.create_line(
                self.position[0],self.position[1]+self.size[1]/2,
                self.objRef.connectionPoint[0],self.objRef.connectionPoint[1],
                arrow=FIRST
            )
        if self.drawOptions.drawChildren == True:
            for name,child in self.children.iteritems():
                child.Draw()
        return self.size
