#!/usr/bin/env python

"""This file defines ROSMOD back-end library interface with the ROSMOD GUI editor"""

__author__ = "Pranav Srinivas Kumar"
__copyright__ = "Copyright 2015, ROSMOD"
__credits__ = ["Pranav Srinivas Kumar", "William Emfinger"]
__license__ = "GPL"
__version__ = "0.4"
__maintainer__ = "Pranav Srinivas Kumar"
__email__ = "pkumar@isis.vanderbilt.edu"
__status__ = "Production"

class ROSMOD():
    def __init__(self, editor = None):
        assert editor != None, "ERROR::Editor Object is None!"
        self._editor = editor

        self.project = None
 
    def init_ui(self):

    def callback_new(self):
        
        self.project = Project(Name(name), Path(path))
        
    def create_action(self, icon="", name="", shortcut="", status_tip="", 
                      function=None):
        assert name != "", "ERROR::Action name is empty!"
        assert function != None, "ERROR::Action function is None!"
        self._editor.action_create(icon, name, self._editor, shortcut, status_tip, 
                                   function)
        
    def create_toolbar(self, name="", ):
        assert name != "", "ERROR::Toolbar name is empty!"
        self._editor.toolbar_create(name)

    def add_toolbar_action(self, toolbar="", action=None):
        assert toolbar != "", "ERROR::Toolbar name is empty!"
        assert action != None, "ERROR::Toolbar action is None!"
        self._editor.toolbar_add_action(toolbar, action)
        
    def create_menubar(self, name=""):
        assert name != "", "ERROR::Menubar name is empty!"
        self._editor.menubar_add_menu(name)

    def add_menu_action(self, menu="", action=None):
        assert menu != "", "ERROR::Menu name is empty!"
        assert action != None, "ERROR::Menu action is None!"
        self._editor.menu_add_action(menu, action)
        
