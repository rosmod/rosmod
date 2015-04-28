# ROSMOD Grammar MetaClass
# Author: Pranav Srinivas Kumar
# Date: 2015.04.27

from collections import OrderedDict
from drawable import Drawable_Object

# Grammar Field class used to generate grammar field-specific listener functions
class Grammar_Field:
    def __init__(self, kind = "", name = "", entry_point = None, exit_point = None, syntax_checker = None):
        self.kind = kind
        self.name = name
        self.entry_point = entry_point
        self.exit_point = exit_point
        self.syntax_checker = syntax_checker

# Enter a Model class type while parsing
# E.g. Package, Component, Timer etc.
def create_enterModel(kind):
    def enterModel(self, ctx):
        print "enterModel:: Model Type = " + kind
        new_object = type( "ROS_" + kind, (object, Drawable_Object,), { '__init__' : Drawable_Object.__init__ })()
        new_object.kind = kind
        self.active_objects.append(new_object)
        print "Created New Object: " + str(type(new_object))
    return enterModel

# Exit a Model class type while parsing
def create_exitModel():
    def exitModel(self, ctx):
        child_object = self.active_objects.pop()
        child_object.parent = self.active_objects[-1]
        self.active_objects[-1].add(child_object)
    return exitModel
    
# Enter a Atom type while parsing
# E.g. Name, Value, Period, Unit etc.
def create_enterAtom(kind):
    def enterAtom(self, ctx):
        print "enterAtom:: Property Type = " + kind
        print "enterAtom:: Property Value = " + ctx.getText()
        self.active_objects[-1].properties[kind] = ctx.getText()
    return enterAtom

# Exit a Atom type while parsing
def create_exitAtom():
    def exitAtom(self, ctx):
        pass
    return exitAtom

# Setup a Dictionary of recognized & parsed grammar fields
# Use this dictionary to generate the listener functions per field inside the builder classes
meta_class_dict = OrderedDict()

# Containers/Models
meta_class_dict["Package"] = Grammar_Field("string", "Package", create_enterModel, create_exitModel)

meta_class_dict["Message"] = Grammar_Field("string", "Message", create_enterModel, create_exitModel)
meta_class_dict["Field"] = Grammar_Field("string", "Field", create_enterModel, create_exitModel)

meta_class_dict["Service"] = Grammar_Field("string", "Service", create_enterModel, create_exitModel)
meta_class_dict["Request"] = Grammar_Field("string", "Request", create_enterModel, create_exitModel)
meta_class_dict["Response"] = Grammar_Field("string", "Response", create_enterModel, create_exitModel)

meta_class_dict["Component"] = Grammar_Field("string", "Component", create_enterModel, create_exitModel)
meta_class_dict["Client"] = Grammar_Field("string", "Client", create_enterModel, create_exitModel)
meta_class_dict["Server"] = Grammar_Field("string", "Server", create_enterModel, create_exitModel)
meta_class_dict["Publisher"] = Grammar_Field("string", "Publisher", create_enterModel, create_exitModel)
meta_class_dict["Subscriber"] = Grammar_Field("string", "Subscriber", create_enterModel, create_exitModel)

meta_class_dict["Node"] = Grammar_Field("string", "Node", create_enterModel, create_exitModel)
meta_class_dict["Component_Instance"] = Grammar_Field("string", "Component_instance", create_enterModel, create_exitModel)
meta_class_dict["Timer"] = Grammar_Field("string", "Timer", create_enterModel, create_exitModel)
meta_class_dict["Hardware"] = Grammar_Field("string", "Hardware", create_enterModel, create_exitModel)
meta_class_dict["Hardware_Instance"] = Grammar_Field("string", "Hardware_instance", create_enterModel, create_exitModel)
meta_class_dict["Node_Instance"] = Grammar_Field("string", "Node_instance", create_enterModel, create_exitModel)
meta_class_dict["Group"] = Grammar_Field("string", "Group", create_enterModel, create_exitModel)
meta_class_dict["Port"] = Grammar_Field("string", "Port", create_enterModel, create_exitModel)

# Properties/Atoms
meta_class_dict["name"] = Grammar_Field("string", "Name", create_enterAtom, create_exitAtom)
meta_class_dict["arch"] = Grammar_Field("string", "Arch", create_enterAtom, create_exitAtom)
meta_class_dict["value"] = Grammar_Field("string", "Value", create_enterAtom, create_exitAtom)
meta_class_dict["unit"] = Grammar_Field("string", "Unit", create_enterAtom, create_exitAtom)
meta_class_dict["datatype"] = Grammar_Field("string", "Datatype", create_enterAtom, create_exitAtom)
meta_class_dict["period"] = Grammar_Field("string", "Period", create_enterAtom, create_exitAtom)
meta_class_dict["priority"] = Grammar_Field("string", "Priority", create_enterAtom, create_exitAtom)
meta_class_dict["deadline"] = Grammar_Field("string", "Deadline", create_enterAtom, create_exitAtom)
meta_class_dict["rdp_hardware"] = Grammar_Field("string", "Rdp_hardware", create_enterAtom, create_exitAtom)
meta_class_dict["ip_address"] = Grammar_Field("string", "Ip_address", create_enterAtom, create_exitAtom)
meta_class_dict["username"] = Grammar_Field("string", "Username", create_enterAtom, create_exitAtom)
meta_class_dict["sshkey"] = Grammar_Field("string", "Sshkey", create_enterAtom, create_exitAtom)
meta_class_dict["init"] = Grammar_Field("string", "Init", create_enterAtom, create_exitAtom)
meta_class_dict["ref"] = Grammar_Field("string", "Ref", create_enterAtom, create_exitAtom)
meta_class_dict["cmd_args"] = Grammar_Field("string", "Cmd_args", create_enterAtom, create_exitAtom)

# Properties that are references
meta_class_dict["component_instance_reference"] = Grammar_Field("reference")

# Grammar Metaclass to generate listener functions as part of the builder classes
class Grammar_MetaClass(type):
    # Create new class
    def __new__(cls, name, bases, attrs):
        if name.startswith('None'):
            return None
        for key, value in meta_class_dict.iteritems():
            if value.entry_point:
                attrs['enter' + value.name] = value.entry_point(key)
            if value.exit_point:
                attrs['exit' + value.name] = value.exit_point()
        return super(Grammar_MetaClass, cls).__new__(cls, name, bases, attrs)

    # Initialize new class
    def __init__(self, name, bases, attrs):
        super(Grammar_MetaClass, self).__init__(name, bases, attrs)
