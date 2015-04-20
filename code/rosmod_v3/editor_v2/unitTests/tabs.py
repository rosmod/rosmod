#!/usr/bin/python
from Tkinter import *
from ttk import *

root = Tk()

note = Notebook(root)

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

