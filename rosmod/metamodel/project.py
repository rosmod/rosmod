#!/usr/bin/env python

"""This file defines the ROSMOD Project Model"""

__author__ = "Pranav Srinivas Kumar"
__copyright__ = "Copyright 2015, ROSMOD"
__credits__ = ["Pranav Srinivas Kumar", "William Emfinger"]
__license__ = "GPL"
__version__ = "0.4"
__maintainer__ = "Pranav Srinivas Kumar"
__email__ = "pkumar@isis.vanderbilt.edu"
__status__ = "Production"

import os
import jsonpickle
from base import Model, Artifact, Children
from attributes import Name, Path, Authors
from software import Software
from hardware import Hardware
from deployment import Deployment

class Project(Model):
    """Project Class
    
    The Project class is the root of a ROSMOD model.
    This object contains the following types of children:
    Software -- Describes the software workspace
    Hardware -- Describes the hardware configuration
    Deployment -- Maps software instances and hardware computers
    """
    def __init__(self, name=Name("NewProject"), path=Path(""), 
                 authors=Authors([]), parent=None):
        super(Project, self).__init__()
        self.kind = "Project"

        assert name != None, "Project name is None!"
        assert path != None, "Project path is None!"
        assert authors != None, "Project authors is None!"

        self.parent = parent
        self['name'] = name
        self['path'] = path
        self['authors'] = authors

        self.children = Children(allowed=[Software(), 
                                          Hardware(), 
                                          Deployment()], 
                                 cardinality={str(type(Software())) : '1',
                                              str(type(Hardware()))\
                                              : '1..*',
                                              str(type(Deployment()))\
                                              : '1..*'})

    def update_artifacts(self, **kwargs):
        self.artifacts = [Artifact(kind="folder", name="workspace", 
                                   destination=self['path'].value +\
                                   self['name'].value)]  
              
    def new(self, 
            name=Name("NewProject"), 
            path=Path(""),
            authors=Authors([]),
            software=Software(Name("NewSoftware")),
            hardware=Hardware(Name("NewHardware")),
            deployment=Deployment(Name("NewDeployment")) ):

        assert name != None, "Project name is None!"
        assert path != None, "Project path is None!"        
        assert authors != None, "Project author is None!"
        assert software != None, "Project Software Model is None!"
        assert hardware != None, "Project Hardware Model is None!"
        assert deployment != None, "Project Deployment Model is None!"

        assert name.value != "", "Project name is empty!"
        assert path.value != "", "Project path is empty!"
        for author in authors.value:
            assert author != "", "Project author is empty!"

        self['name'] = name
        self['path'] = path
        self['authors'] = authors

        self.add_child(software)
        self.add_child(hardware)
        self.add_child(deployment)

        project_dir = os.path.join(path.value, name.value)
        if not os.path.exists(project_dir):
            os.makedirs(project_dir)

    def save(self):
        jsonpickle.set_encoder_options('simplejson',indent=4)
        encoder_output = jsonpickle.encode(self)
        project_dir = os.path.join(self['path'].value, self['name'].value) 
        model = os.path.join(project_dir, self['name'].value + '.rml')
        with open(model, 'w') as model_file:
            model_file.write(encoder_output)

    def generate_artifacts(self):
        self.generate(project_name=self['name'],
                      project_path=self['path'])

    def save_as(self, name=None, path=None):
        assert name != None and name.value != "", "Project name is None!"
        assert path != None and path.value != "", "Project path is None!"

        self['name'] = name
        self['path'] = path

        project_dir = os.path.join(path.value, name.value)
        if not os.path.exists(project_dir):
            os.makedirs(project_dir)
        self.save()

    def open(self, model=""):        
        assert model != "", "Project path is empty!"
        with open(model, 'r') as input_model:
            self = jsonpickle.decode(input_model.read())
