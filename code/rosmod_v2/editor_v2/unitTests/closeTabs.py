#!/usr/bin/python
from Tkinter import *
from ttk import *
import os


root = Tk()

imgdir = os.path.join(os.path.dirname(__file__), 'img')

i1 = PhotoImage("img_close", file=os.path.join("./", 'windows_close_program.png'))
i2 = PhotoImage("img_closepressed", file=os.path.join("./", 'windows_close_program.png'))
i3 = PhotoImage("img_closeactive", file=os.path.join("./", 'windows_close_program.png'))
i4 = PhotoImage("img_close2", file=os.path.join("./", 'windows_close_program.png'))

style = Style()
style.element_create("close", "image", "img_close",
                     ("active", "pressed", "!disabled", "img_closepressed"),
                     ("active", "!disabled", "img_closeactive"), border=8, sticky='')
style.layout("ButtonNotebook", [("ButtonNotebook.client", {"sticky": "nswe"})])
style.layout("ButtonNotebook.Tab", [
    ("ButtonNotebook.tab", {"sticky": "nswe", "children":
                            [("ButtonNotebook.padding", {"side": "top", "sticky": "nswe",
                                                         "children":
                                                         [("ButtonNotebook.focus",{"side":"top", "sticky": "nswe",
                                                                                   "children":
                                                                                   [("ButtonNotebook.label", {"side":"left", "sticky": ''}),
                                                                                    ("ButtonNotebook.close", {"side":"left", "sticky": ''})]
                                                                               })]
                                                         })]
                            })]
             )

def btn_press(event):
    x,y,widget = event.x,event.y,event.widget
    elem = widget.identify(x,y)
    index = widget.index("@%d,%d" % (x,y))
    if "close" in elem:
        widget.state(['pressed'])
        widget.pressed_index = index

def btn_release(event):
    x,y,widget = event.x,event.y,event.widget
    if not widget.instate(['pressed']):
        return
    elem = widget.identify(x,y)
    index = widget.index("@%d,%d" % (x,y))
    if "close" in elem and widget.pressed_index == index:
        widget.forget(index)
        widget.event_generate("<<NotebookClosedTab>>")
    widget.state(["!pressed"])
    widget.pressed_index = None

root.bind_class("TNotebook", "<ButtonPress-1>", btn_press, True)
root.bind_class("TNotebook", "<ButtonRelease-1>", btn_release)

note = Notebook(root, style="ButtonNotebook")

tabnum = 0

def closeTab():
    activeTab = note.select()
    note.forget(activeTab)
    # select the first tab, since the last tab creates a new tab
    note.select(0)

tab1 = Frame(note)
tab2 = Frame(note)
tab3 = Frame(note)
newTab = Frame(note)
Button(tab1, text='Exit', command=root.destroy).pack(padx=100, pady=100)

def tabChanged(event):
    global tabnum
    numTabs = note.index("end")
    selectedTabID = note.index(note.select())
    # if the user presses the last tab, create a new tab just before it
    # and switch to it
    if selectedTabID == numTabs-1:
        t = Frame(note)
        note.insert(numTabs-1,t, text = "tab {}".format(tabnum))
        tabnum += 1
        note.select(t)

note.bind('<<NotebookTabChanged>>',tabChanged)

note.add(tab1, text = "Tab One", compound=TOP)
note.add(tab2, text = "Tab Two")
note.add(tab3, text = "Tab Three")
note.add(newTab, text = "New")
btn1 = Button(root, text="Close Active Tab", command = closeTab)
btn1.pack()
note.pack()
root.mainloop()
exit()

