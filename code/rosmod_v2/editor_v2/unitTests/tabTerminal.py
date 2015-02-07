#!/usr/bin/python
from Tkinter import *
from ttk import *

import os

root = Tk()

note1 = Notebook(root)

tab1 = Frame(note1)
tab2 = Frame(note1)
tab3 = Frame(note1)

wid = tab1.winfo_id()
os.system('xterm -into %d &' % wid)

Button(tab2, text='Exit', command=root.destroy).pack(padx=100, pady=100)

note1.add(tab1, text = "Tab One", compound=TOP)
note1.add(tab2, text = "Tab Two")
note1.add(tab3, text = "Tab Three")
note1.pack()

root.mainloop()
exit()
