#!/usr/bin/python
from Tkinter import *
from ttk import *

import os

class CreateToolTip(object):
    '''
    create a tooltip for a given widget
    '''
    def __init__(self, widget, text='widget info'):
        self.widget = widget
        self.text = text
        self.widget.bind("<Enter>", self.enter)
        self.widget.bind("<Leave>", self.close)

    def enter(self, event=None):
        x = y = 0
        x, y, cx, cy = self.widget.bbox("insert")
        x += self.widget.winfo_rootx() + 25
        y += self.widget.winfo_rooty() + 20
        # creates a toplevel window
        self.tw = Toplevel(self.widget)
        # Leaves only the label and removes the app window
        self.tw.wm_overrideredirect(True)
        self.tw.wm_geometry("+%d+%d" % (x, y))
        label = Label(self.tw, text=self.text, justify='left',
                       background='white', relief='solid', borderwidth=1)
        label.pack(ipadx=1)

    def close(self, event=None):
        if self.tw:
            self.tw.destroy()


root = Tk()

note1 = Notebook(root)

tab1 = Frame(note1)
tab2 = Frame(note1)
tab3 = Frame(note1)

tab1.pack(fill=BOTH,expand=1)
tab2.pack(fill=BOTH,expand=1)
tab3.pack(fill=BOTH,expand=1)

wid = tab1.winfo_id()
os.system('xterm -into %d &' % wid)

btn1 = Button(tab2, text='Exit', command=root.destroy)
btn1.pack(padx=100, pady=100)
button_ttp = CreateToolTip(btn1, "exit the program.")

note1.add(tab1, text = "Tab One", compound=TOP)
note1.add(tab2, text = "Tab Two")
note1.add(tab3, text = "Tab Three")
note1.pack(fill=BOTH, expand=1)

root.mainloop()
exit()
