#!/usr/bin/python

from Tkinter import *

class App:
    
    def __init__(self, master):
        
        frame = Frame(master)
        frame.pack()
        
        self.button = Button(
            frame, text="QUIT", fg="red", command=frame.quit
        )
        self.button.pack(side=LEFT)

        self.hi_there = Button(frame, text="Hello", command=self.say_hi)
        self.hi_there.pack(side=LEFT)

    def say_hi(self):
        print "Hi there, everyone!"

root = Tk()

app = App(root)

root.mainloop()
root.destroy()

'''
Gotten as example for setting up callbacks
'''

import tkMessageBox

root = Tk()

class StatusBar(Frame):

    def __init__(self, master):
        Frame.__init__(self, master)
        self.label = Label(self, bd=1, relief=SUNKEN, anchor=W)
        self.label.pack(fill=X)

    def set(self, format, *args):
        self.label.config(text=format % args)
        self.label.update_idletasks()

    def clear(self):
        self.label.config(text="")
        self.label.update_idletasks()

def key(event):
    print "pressed", repr(event.char)

def right_click_callback(event):
    frame.focus_set()
    print "right clicked at", event.x, event.y

def middle_click_callback(event):
    frame.focus_set()
    print "middle clicked at", event.x, event.y

def left_click_callback(event):
    frame.focus_set()
    print "left clicked at", event.x, event.y

def configure_callback(event):
    frame.focus_set()
    print "New width is", event.width, "and height is", event.height

def close_callback():
    if tkMessageBox.askokcancel("Quit", "Do you really wish to quit?"):
        root.destroy()

def toolbar_callback():
    status.set("%s","you pressed a toolbar button")
    print "pressed a toolbar button"

toolbar = Frame(root)

b = Button(toolbar, text="new", width=6, command=toolbar_callback)
b.pack(side=LEFT, padx=2, pady=2)
b = Button(toolbar, text="open", width=6, command=toolbar_callback)
b.pack(side=LEFT, padx=2, pady=2)

toolbar.pack(side=TOP, fill=X)

frame = Frame(root, width=100,height=100)
frame.bind("<Key>", key)
frame.bind("<Button-1>", left_click_callback)
frame.bind("<Button-2>", middle_click_callback)
frame.bind("<Button-3>", right_click_callback)
frame.bind("<Configure>", configure_callback)
frame.pack()

status = StatusBar(root)
status.pack(side=BOTTOM, fill=X)

root.protocol("WM_DELETE_WINDOW", close_callback)

root.mainloop()

'''
Gotten as example for moving widgets around w/ mouse
'''

import Tkinter as tk

class SampleApp(tk.Tk):
    '''Illustrate how to drag items on a Tkinter canvas'''

    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)

        # create a canvas
        self.canvas = tk.Canvas(width=400, height=400)
        self.canvas.pack(fill="both", expand=True)

        # this data is used to keep track of an 
        # item being dragged
        self._drag_data = {"x": 0, "y": 0, "item": None}

        # create a couple movable objects
        self._create_token((100, 100), "white")
        self._create_token((200, 100), "black")

        # add bindings for clicking, dragging and releasing over
        # any object with the "token" tag
        self.canvas.tag_bind("token", "<ButtonPress-1>", self.OnTokenButtonPress)
        self.canvas.tag_bind("token", "<ButtonRelease-1>", self.OnTokenButtonRelease)
        self.canvas.tag_bind("token", "<B1-Motion>", self.OnTokenMotion)

    def _create_token(self, coord, color):
        '''Create a token at the given coordinate in the given color'''
        (x,y) = coord
        self.canvas.create_oval(x-25, y-25, x+25, y+25, 
                                outline=color, fill=color, tags="token")

    def OnTokenButtonPress(self, event):
        '''Being drag of an object'''
        # record the item and its location
        self._drag_data["item"] = self.canvas.find_closest(event.x, event.y)[0]
        self._drag_data["x"] = event.x
        self._drag_data["y"] = event.y

    def OnTokenButtonRelease(self, event):
        '''End drag of an object'''
        # reset the drag information
        self._drag_data["item"] = None
        self._drag_data["x"] = 0
        self._drag_data["y"] = 0

    def OnTokenMotion(self, event):
        '''Handle dragging of an object'''
        # compute how much this object has moved
        delta_x = event.x - self._drag_data["x"]
        delta_y = event.y - self._drag_data["y"]
        # move the object the appropriate amount
        self.canvas.move(self._drag_data["item"], delta_x, delta_y)
        # record the new position
        self._drag_data["x"] = event.x
        self._drag_data["y"] = event.y

if __name__ == "__main__":
    app = SampleApp()
    app.mainloop()
