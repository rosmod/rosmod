#!/usr/bin/python
from Tkinter import *
from ttk import *
import os

root = Tk()
note1 = Notebook(root, height=200, width=200)
tab1 = Frame(note1)
tab2 = Frame(note1)

wid_tab1 = tab1.winfo_id()
# -sb = scrolling
# -aw = auto-wrap
os.system('xterm -into %d -geometry -sb -maximized -aw -C &' % wid_tab1)

wid_tab2 = tab2.winfo_id()
os.system('xterm -into %d -geometry -sb -maximized -aw -C &' % wid_tab2)


tab1.pack(fill=BOTH, expand=YES)
tab2.pack(fill=BOTH, expand=YES)
note1.add(tab1, text="Tab One", compound=TOP)
note1.add(tab2, text="Tab Two")
note1.pack(fill=BOTH, expand=YES)

root.mainloop()

