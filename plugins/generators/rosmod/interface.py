#!/usr/bin/env python

"""This file defines an interface to the ROSMOD back-end"""

__author__ = "Pranav Srinivas Kumar"
__copyright__ = "Copyright 2015, ROSMOD"
__credits__ = ["Pranav Srinivas Kumar", "William Emfinger"]
__license__ = "GPL"
__version__ = "0.4"
__maintainer__ = "Pranav Srinivas Kumar"
__email__ = "pkumar@isis.vanderbilt.edu"
__status__ = "Production"

import os
import metamodel.project as ROSMOD

class ROSMOD_Interface():
    """Interface to the ROSMOD back-end library"""
    def __init__(self, gui=None):
        self._gui = gui
        
        self.project = ROSMOD.Project()
        print "Project Object Created"

    def new_project(self, 
                    name=ROSMOD.Name("NewProject"), \
                    path=ROSMOD.Path(""),
                    authors=ROSMOD.Authors([]),
                    software=ROSMOD.Software(ROSMOD.Name("NewSoftware")),
                    hardware=ROSMOD.Hardware(ROSMOD.Name("NewHardware")),
                    deployment=ROSMOD.Deployment(ROSMOD.Name("NewDeployment"))):
        """Create a new ROSMOD Project"""

        assert name != None, "Project Name is None!"
        assert path != None, "Project Path is None!"  
        assert authors != None, "Project Authors is None!"
        assert software != None, "Project Software Model is None!"
        assert hardware != None, "Project Hardware Model is None!"
        assert deployment != None, "Project Deployment Model is None!"

        assert name.value != "", "Project name is empty!"
        assert path.value != "", "Project path is empty!"
        for author in authors.value:
            assert author != "", "Project author is empty!"

        self.project['name'] = name
        self.project['path'] = path
        self.project['authors'] = authors

        self.project.add_child(software)
        self.project.add_child(hardware)
        self.project.add_child(deployment)

        project_dir = os.path.join(path.value, name.value)
        if not os.path.exists(project_dir):
            os.makedirs(project_dir)

        return self.project
        
    def save_project(self):
        """Create the current ROSMOD Project"""
        jsonpickle.set_encoder_options('simplejson',indent=4)
        encoder_output = jsonpickle.encode(self.project)
        project_dir = os.path.join(self.project['path'].value, 
                                   self.project['name'].value) 
        model = os.path.join(project_dir, self.project['name'].value + '.rml')
        with open(model, 'w') as model_file:
            model_file.write(encoder_output)

    def generate_artifacts(self):
        """Recursively generate all ROSMOD artifacts"""
        self.project.generate(project_name=self.project['name'],
                              project_path=self.project['path'])

    def save_project_as(self, name=None, path=None):
        """Create the current ROSMOD Project as..."""
        assert name != None and name.value != "", "Project Name is None!"
        assert path != None and path.value != "", "Project Path is None!"

        self.project['name'] = name
        self.project['path'] = path

        project_dir = os.path.join(path.value, name.value)
        if not os.path.exists(project_dir):
            os.makedirs(project_dir)
        self.save_project()

    def open_project(self, path=""):        
        """Open an existing ROSMOD Project"""
        assert path != "", "Project Path is empty!"
        with open(path, 'r') as input_model:
            self.project = jsonpickle.decode(input_model.read())
                            
        
