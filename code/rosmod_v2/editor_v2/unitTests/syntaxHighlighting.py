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

yscrollbar=Scrollbar(root,orient=VERTICAL,command=ptext.yview)
ptext["yscrollcommand"] = yscrollbar.set

yscrollbar.pack(side=RIGHT,fill=Y)
ptext.pack(side=LEFT, fill=BOTH, expand = YES)

root.mainloop()
