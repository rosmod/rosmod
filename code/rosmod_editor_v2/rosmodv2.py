#!/usr/bin/python
'''
This Program is designed to be the user interface
for the ROSMOD modeling language.  It has the ability
to create, load, display, edit, and export ROSMOD models.

Additionally, it supports the interaction with analysis
tools through the use of toolbar buttons and subprocesses.
'''

from Tkinter import *
from ttk import *

import tkFileDialog
import tkMessageBox

from collections import OrderedDict

import sys
import os

# need to have class containing methods and members for file operations
#self.fileFormats = [
#    ('ROSMOD Model','*.rosml')
#]

# need to have a class of options for the program:
# these options cover:
# * how objects are rendered in the editor:
# ** e.g. 2d, isometric, color, image, straight line, autorouted line, etc.
# ** font options, layout options, etc.
# * ideally we could use this to specify the behavior of the program, by adding 
#   the menu options and their functions to the options to make it easily extensible

# the main ROSMOD editor will have:
# * a menu bar (for file, aspect, tools, window, and help)
# * A notebook (to support tabs easily) for the editor's
#   3 main aspects: (packages, hardware, deployment)
# * A notebook for the editor's console (debug output),
#   bash terminal (optional), node/comp logs, etc.

# these notebooks need to all be scollable in both the horizontal and vertical directions
# and they need to fill the frame of the window and resize properly when the main editor
# is resized

class rosmod_editor:

    def __init__(self, master, options = None):

        self.master = master

        ''' Set up the program's menu '''
        ''' Set up the Editor notebook for the three aspects ''' 
        ''' Set up the output window for the different outputs (debug, bash, logs, etc.) '''

        # Ensure the the main window won't close without a popup (to be annoying :) )
        self.master.protocol("WM_DELETE_WINDOW", self.close_Callback)

    '''
    -------------
    Exit Handler:
    -------------
    '''
    def close_Callback(self):
        if tkMessageBox.askokcancel("Quit", "Do you really wish to quit?"):
            self.master.quit()

if __name__ == "__main__":

    root = Tk()

    root.title("ROSMOD")

    app = rosmod_editor(root)

    root.mainloop()
