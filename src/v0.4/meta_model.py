
"""
Can we generate meta-models using just:
* models and atoms
* grammar

All models will be XML, meta-models will also be xml,
These models will be monolithic (single-file) and will
allow for parsers based on type (which is a lot easier
to use than antlr).  

Using this basic model (below) we can generate xml files
describing meta-models, which can then generate classes
to check model files and build models.  
"""

"""
meta-meta-model: all meta-models are composed of models and atoms,
where models may contain one or more models and one or more atoms,
but atoms do not contain anything

Atoms provide the ability to add properties to objects and references
between objects

The Virtual_Base provides members common to all meta-meta-model objects
"""
class Virtual_Base:
    def __init__(self):
        self.name = ""
        self.kind = ""
        self.view = None    # the viewer for the object, defines display name, etc.
        self.control = None # the control for the object, defines validators, editing, etc.

class Model(Virtual_Base):
    def __init__(self):
        self.models = []  # children of the object
        self.atoms = []   # properties of the object

class Atom(Virtual_Base):
    def __init__(self):
        self.value = ""
