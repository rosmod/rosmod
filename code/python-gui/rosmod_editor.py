#!/usr/bin/python
'''
'''

from Tkinter import *
import tkFileDialog
import tkMessageBox

from collections import OrderedDict

class EditorFrame(Frame):
    def __init__(self, master, height, width, maxHeight, maxWidth):
        Frame.__init__(self,master)
        Frame.config(self,bd=2, relief=SUNKEN)

        self.height=height
        self.width=width

        self.VScrollBar = Scrollbar(self, orient=VERTICAL)
        self.VScrollBar.pack(fill='y', side=RIGHT)
        self.HScrollBar = Scrollbar(self, orient=HORIZONTAL)
        self.HScrollBar.pack(side=BOTTOM, fill=BOTH,expand=1)

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
        self.selectedObject = { "object": None }

        ypos = self.width * 3 / 4
        for name,attr in objectDict.iteritems():
            self._create_object((self.width/2, ypos), self.width * 3/4, attr[0], attr[1])
            ypos += self.width

        self.canvas.tag_bind("object", "<Button-1>", self.OnObjectButtonPress)

    def OnObjectButtonPress(self, event):
        self.selectedObject["object"] = self.canvas.find_closest(
            self.canvas.canvasx(event.x), 
            self.canvas.canvasy(event.y)
        )[0]
        print "Selected object {0} with tags {1}".format(
            self.selectedObject["object"],
            self.canvas.gettags(self.selectedObject["object"])
        )
        
class ModelViewer(EditorFrame):
    def __init__(self, master, height, width, maxHeight, maxWidth):
        EditorFrame.__init__(self,master,height,width,maxHeight,maxWidth)
