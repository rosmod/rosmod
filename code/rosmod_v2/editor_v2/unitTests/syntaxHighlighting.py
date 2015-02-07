#!/usr/bin/python
from Tkinter import *
from ttk import *
from pygments.lexers import *
from tkformatter  import TkFormatter
from pygtext import PygmentsText
from msg_srv_lexer import MsgSrvLexer

fileName = 'test.msg'

root = Tk()

root.attributes('-topmost', 1)

py = open(fileName).read()

ptext = PygmentsText(root, MsgSrvLexer(), TkFormatter())

ptext.insertFormatted("end",py)

ptext.grid(row=0,column=0,sticky="nsw")
yscrollbar=Scrollbar(root,orient=VERTICAL,command=ptext.yview)
yscrollbar.grid(row=0,column=1,sticky="nsew")
ptext["yscrollcommand"] = yscrollbar.set
ptext.update()

h=int(round(ptext.winfo_height()/ptext["height"])), int(round(ptext.winfo_width()/ptext["width"]))

def resize(event):
    pixelX = root.winfo_width()-yscrollbar.winfo_width()
    pixelY = root.winfo_height()
    ptext["width"] = int(round(pixelX/h[1]))
    ptext["height"] = int(round(pixelY/h[0]))

root.bind("<Configure>", resize)

root.mainloop()
