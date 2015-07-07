
"""
Can we generate meta-models using just:
* models and atoms
* grammar
"""

"""
meta-meta-model: all meta-models are composed of models and atoms,
where models may contain one or more models and one or more atoms,
but atoms do not contain anything

Atoms provide the ability to add properties to objects and references
between objects
"""
class Model:
    def __init__(self):
        self.models = []  # children of the object
        self.atoms = []   # properties of the object
        self.name = ""
        self.kind = "" # e.g. component, message, node
        self.view = None # viewer for the model
        self.control = None # controller for the model

class Atom:
    def __init__(self):
        self.name = ""
        self.kind = ""  # e.g. artifact, string, float, reference
        self.value = ""
        self.view = None # viewer for the atom, coontains things like display namen
        self.control = None # controller for the atom, contains things like validator reference
