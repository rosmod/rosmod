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

btn0 = Button(root,text="Ok")
btn1 = Button(root,text="Cancel")

ptext.grid(row=0,column=0, columnspan=2, sticky="nsew")
yscrollbar.grid(row=0,column=2, sticky="nsew")
btn0.grid(row=1,column=0,sticky="nsew")
btn1.grid(row=1,column=1,sticky="nsew")

root.rowconfigure(0,weight=1)
root.columnconfigure(0,weight=1)


#yscrollbar.pack(side=RIGHT,fill=Y)
#btn0.pack(side=LEFT)
#btn1.pack(side=LEFT)
#ptext.pack(side=TOP, fill=BOTH, expand = YES)

root.mainloop()
