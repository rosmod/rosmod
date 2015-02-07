#!/usr/bin/python
from Tkinter import *
from ttk import *
from pygments.lexers import HtmlLexer, PythonLexer, CppLexer
from tkformatter  import TkFormatter
from pygtext import PygmentsText
from msg_srv_lexer import MsgSrvLexer

root = Tk()

root.attributes('-topmost', 1)

py = open('test.msg').read()
#ptext = PygmentsText(root,CppLexer(), TkFormatter())
ptext = PygmentsText(root,MsgSrvLexer(), TkFormatter())
ptext.insertFormatted("end",py)
ptext.pack()
root.mainloop()
