#!/usr/bin/python
from Tkinter import *
from ttk import *

from contextMenu import *

root = Tk()

note1 = Notebook(root)

tab1 = Frame(note1)
tab2 = Frame(note1)
tab3 = Frame(note1)

cm = ContextMenu(tab1)

tab1.bind("<Button-1>", button1_callback)
tab1.bind("<Button-3>", cm.callback)

Button(tab1, text='Exit', command=root.destroy).pack(padx=100, pady=100)

note1.add(tab1, text = "Tab One", compound=TOP)
note1.add(tab2, text = "Tab Two")
note1.add(tab3, text = "Tab Three")
note1.pack()

root.mainloop()
exit()
