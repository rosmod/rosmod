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

# these notebooks should be spawnable and killable by the window menubar calls

# the tabs of the notebooks should be generated automatically for each aspect
# * the packages aspect has a tab for each package and a tab containing all package info
# * the hardware aspect only has one tab (for now)
# * the deployment aspect has a tab for each package and a tab containing all package info

# each aspect determines its own context menu (right click menu)

# each object (drawn on an aspect canvas) determines its own context menu

# each object (and context menu) can have it's own pop-ups (e.g. msg editors)
# which must be managed and must take control to prevent the user from 
# interacting with the main editor window while the popup is active (this 
# was already present in v1 of the editor).  

# these popups must support object references, text input, name input, etc.
# and must be spawnable from context menus

# A state change is only triggered by the main menubar of the application
# i.e. modifying one of the aspects (insertion, modification, deletion) does
# not ever switch aspects

# Inside the deployment aspect, we can deploy nodes or applications to hosts
# using a context menu, which controls a process pool that governs lifecycle of the
# testbed.  This must be aware of what is currently happening in the system
# and keep track of its own state transitions (such that an app can only be stopped if 
# it is running, and an app cannot be spawned if it has already been spawned.) This can 
# be partially accomplisehd by keeping as little state as possible and querying the system:
# code has already been developed which can tell if a certain node is running on a certain
# host, and can kill the node if it is, and start the node if it isn't

# for deployment managing, we will probably need to have multiple subprocesses/threads 
# which handle lifecycle management of the processes, getting logs, setting up nodes,
# testing connectivity, and editor state update

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
