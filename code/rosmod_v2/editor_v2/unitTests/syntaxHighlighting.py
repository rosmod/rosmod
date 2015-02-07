#!/usr/bin/python
from Tkinter import *
from ttk import *
from pygments.lexers import HtmlLexer, PythonLexer
from tkformatter  import TkFormatter
from pygtext import PygmentsText

root = Tk()

root.attributes('-topmost', 1)

format = "python"

py = open('syntaxHighlighting.py').read()
ptext = PygmentsText(root,PythonLexer(), TkFormatter())
ptext.insertFormatted("end",py)
ptext.pack()
root.mainloop()
