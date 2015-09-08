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

from classes import *

class ROSMOD():
    def __init__(self, editor = None):
        assert editor != None, "ERROR::Editor Object is None!"
        self._editor = editor

        self.project = Project()
 
    def init_ui(self):

    def new_project(self 
            name=Name("NewProject"), 
            path=Path(""),
            software=Software(Name("NewSoftware")),
            hardware=Hardware(Name("NewHardware")),
            deployment=Deployment(Name("NewDeployment"))):

        assert name != None, "Project name is None!"
        assert path != None, "Project path is None!"        
        assert software != None, "Project Software Model is None!"
        assert hardware != None, "Project Hardware Model is None!"
        assert deployment != None, "Project Deployment Model is None!"

        assert name.value != "", "Project name is empty!"
        assert path.value != "", "Project path is empty!"

        self.project['name'] = name
        self.project['path'] = path

        self.project.add_child(software)
        self.project.add_child(hardware)
        self.project.add_child(deployment)

        project_dir = os.path.join(path.value, name.value)
        if not os.path.exists(project_dir):
            os.makedirs(project_dir)
        
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
        
