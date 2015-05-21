#!/usr/bin/python
from Tkinter import *
from ttk import *

root = Tk()

pw = PanedWindow(
    root,
    orient = VERTICAL,
)

note1 = Notebook(root)
note2 = Notebook(root)

tab1 = Frame(note1)
tab2 = Frame(note1)
tab3 = Frame(note1)
Button(tab1, text='Exit', command=root.destroy).pack(padx=100, pady=100)

tab4 = Frame(note2)
tab5 = Frame(note2)
tab6 = Frame(note2)
Button(tab5, text='Exit', command=root.destroy).pack(padx=100, pady=100)

note1.add(tab1, text = "Tab One", compound=TOP)
note1.add(tab2, text = "Tab Two")
note1.add(tab3, text = "Tab Three")
note1.pack()

note2.add(tab4, text = "Tab One", compound=TOP)
note2.add(tab5, text = "Tab Two")
note2.add(tab6, text = "Tab Three")
note2.pack()

pw.add(note1)
pw.add(note2)
pw.pack(fill='both')

pw.forget(note1)
pw.pack(fill='both')

root.mainloop()
exit()
