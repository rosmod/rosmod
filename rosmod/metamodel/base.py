#!/usr/bin/env python

"""This file defines base clases for the ROSMOD metamodel."""

__author__ = "Pranav Srinivas Kumar"
__copyright__ = "Copyright 2015, ROSMOD"
__credits__ = ["Pranav Srinivas Kumar", "William Emfinger"]
__license__ = "GPL"
__version__ = "0.4"
__maintainer__ = "Pranav Srinivas Kumar"
__email__ = "pkumar@isis.vanderbilt.edu"
__status__ = "Production"

import os
from collections import OrderedDict, MutableSequence

class Attribute(object):
    """Generic Attributes class

    Each Attribute has the following:
    kind -- The datatype of the attribute e.g. string, float, bool etc.
    value -- The value of the attributes e.g. "my_component", 1.642 etc.
    """
    tooltip = ''
    display = ''
    options = []
    editable = True
    def __init__(self, kind, value):
        super(Attribute, self).__init__()
        self.kind = kind
        self.value = value

    def __setitem__(self, key, val):
        self.value[key] = val

    def __getitem__(self, key):
        return self.value[key]

class Model(object):
    """Generic Model/Container class

    Every Model has the following:
    kind -- The Domain-specific kind of Model e.g. Component, Port etc.
    parent -- A parent Model Object.
    children -- A list of children (Model) objects.
    attributes -- A dictionary of attributes.
    """
    def __init__(self):
        super(Model, self).__init__()
        self.kind = ""
        self.parent = None
        self.children = None
        self.attributes = OrderedDict() 
        self.artifacts = []
        self.kwargs = {}

    def __getitem__(self, key):
        return self.attributes[key]

    def __setitem__(self, key, value):
        self.attributes[key] = value

    def child_count(self):
        return len(self.children)

    def child(self, position):
        return self.children[position]

    def row(self):
        if self.parent:
            return self.parent.children.index(self)

    def remove_child(self, position):
        if position < 0 or position > len(self.children):
            return False
        child = self.children.pop(position)
        child.parent = None
        return True

    def insert_child(self, position, child_model):
        if position < 0 or position > len(self.children):
            return False
        child_model.parent = self
        self.children.insert(position, child_model)
        return True

    def add_child(self, child_model):
        child_model.parent = self
        self.children.append(child_model)

    def add_attribute(self, name, kind, value):
        self[name] = Attribute(kind, value)

    def get_children(self, kind):
        if self.kind == kind:
            return [self]
        else:
            kids = []
            for c in self.children:
                kids.extend(c.get_children(kind))
            return kids

    def get_artifact(self, name):
        for a in self.artifacts:
            if a.name == name:
                return a

    def update_artifacts(self, **kwargs):
        return

    def resolve_artifacts(self, artifact_list=None):
        if artifact_list == None:
            artifact_list = self.artifacts
        for artifact in artifact_list:
            if artifact.kind == "code":
                for o in artifact.order:
                    sub_artifact = self.get_artifact(o)
                    if sub_artifact != None:
                        artifact.value += sub_artifact.value
                    else:
                        for child in self.children:
                            child_artifact = child.get_artifact(o)
                            if child_artifact != None:
                                artifact.value += child_artifact.value

    def generate(self, **kwargs):
        self.update_artifacts(**kwargs)
        self.resolve_artifacts(self.artifacts)
        if self.artifacts != None:
            for artifact in self.artifacts:
                if artifact.kind == "folder":
                    folder = os.path.join(artifact.destination, artifact.name)
                    if not os.path.exists(folder):
                        os.makedirs(folder)

        for child in self.children:
            child.generate(**kwargs)

        if self.artifacts != None:
            for artifact in self.artifacts:
                if artifact.kind == "code":                    
                    file_artifact = os.path.join(artifact.destination, artifact.name)
                    with open(file_artifact, 'w') as art_file:
                        art_file.write(artifact.value)
        self.kwargs = kwargs

class Artifact(object):
    """Generic Artifact object
    
    Each Artifact has the following:
    kind -- the type of artifact e.g. code, build_system, analysis, file etc.
    value -- The value of the artifact e.g. "#include <...", "<?xml version" etc.
    name -- file/folder name of the artifact
    destination -- destination directory of the artifact
    source -- source directory of a "file/static" artifact if any e.g. node_main.cpp
    """
    tooltip = ''
    display = ''
    def __init__(self, kind=None, value="", name=None, destination=None, 
                 source=None, order=[]):
        assert kind != None, "Artifact kind is None!"
        assert isinstance(value, basestring) == True,\
            "Artifact value is not a string!"

        self.kind = kind
        self.value = value
        self.name = name
        self.destination = destination
        self.source = source
        self.order = order

class Children(MutableSequence):
    """Children List
    
    _inner -- Contents of the list
    _allowed -- The list will accept only object types contained in _allowed
    _cardinality -- Cardinality of each accepted type
    """
    def __init__(self, it=(), allowed=(), cardinality=()):
        self._inner = list(it)
        self._allowed = list(type(a) for a in allowed)
        self._cardinality = cardinality

    def __len__(self):
        return len(self._inner)
    def __iter__(self):
        return iter(self._inner)
    def __contains__(self, item):
        return item in self._inner
    def __getitem__(self, index):
        return self._inner[index]
    def __setitem__(self, index, value):
        self._inner[index] = value
    def __delitem__(self, index):
        del self._inner[index]
    def __repr__(self):
        return 'Children({})'.format(self._inner)
    def insert(self, index, item):
        if type(item) in self._allowed:
            if item not in self._inner:
                item_cardinality = self._cardinality[str(type(item))]
                children_types = [str(type(val)) for val in self._inner]
                if item_cardinality == '1':
                    if str(type(item)) not in children_types:
                        return self._inner.insert(index, item)
                    else:
                        print "ERROR::Cardinality Error!"
                else:
                    return self._inner.insert(index, item)
        else:
            print "ERROR::Cannot add child: " + str(item)
            return self._inner
