#!/usr/bin/python

from Tkinter import *
from ttk import *

from collections import OrderedDict

activeMenus = []

def closeAllContextMenus():
    global activeMenus
    for menu in activeMenus:
        menu.unpost()    

def registerContextMenu(menu):
    global activeMenus
    activeMenus.append(menu)

class ContextMenu():
    def __init__(self,canvas):
        self.canvas = canvas
        self.menu = Menu(self.canvas, tearoff=0)
        self.numCommands = 0
        
    def destroy(self):
        self.menu.destroy()

    def callback(self,event):
        closeAllContextMenus()
        registerContextMenu(self.menu)
        self.menu.delete(0,self.numCommands)
        self.menu.add_command(label="test", command=testCommand)
        
        # useful if the event is on a canvas
        #x = self.canvas.canvasx(event.x)
        #y = self.canvas.canvasy(event.y)
        #objID = self.canvas.find_overlapping(x,y,x+1,y+1)#.find_closest(x,y)[0]

        self.menu.post(event.x_root,event.y_root) 

def testCommand():
    print "Context Menu Func invoked."

def button1_callback(event):
    closeAllContextMenus()

if __name__ == "__main__":
    root = Tk()
    root.title("ContextMenu Test")
    f = Frame(root)
    cv = Canvas(
        f,
        width = 100,
        height = 100
    )
    cm = ContextMenu(cv)
    cv.bind("<Button-1>", button1_callback)
    cv.bind("<Button-3>", cm.callback)

    # useful for objects
    #cv.tag_bind("","<Button-3>", cm.callback)

    cv.pack(fill=BOTH)
    f.pack(fill=BOTH)
    
    root.mainloop()
