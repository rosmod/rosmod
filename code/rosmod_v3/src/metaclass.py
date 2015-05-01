# ROSMOD Grammar MetaClass
# Author: Pranav Srinivas Kumar
# Date: 2015.04.27

from collections import OrderedDict
from metaModel import model_dict
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
        #print "enterModel:: Model Type = " + kind
        new_object = type( "ROS_" + kind, (object, Drawable_Object,), { '__init__' : Drawable_Object.__init__ })()
        new_object.kind = kind
        for prop in model_dict[kind].properties:
            new_object.properties[prop] = ""
        self.active_objects.append(new_object)
        #print "Created New Object: " + str(type(new_object))
    return enterModel

# Exit a Model class type while parsing
def create_exitModel():
    def exitModel(self, ctx):
        child_object = self.active_objects.pop()
        child_object.parent = self.active_objects[-1]
        if model_dict[child_object.kind].in_refs != []:
            scopeStr = ""
            if "name" in self.active_objects[-1].properties.keys():
                scopeStr += self.active_objects[-1].properties["name"] + "/"
            reference_dict[scopeStr + str(child_object.properties["name"])] = child_object
        self.active_objects[-1].add(child_object)
    return exitModel
    
# Enter a Atom type while parsing
# E.g. Name, Value, Period, Unit etc.
def create_enterAtom(kind):
    def enterAtom(self, ctx):
        #print "enterAtom:: Property Type = " + kind
        #print "enterAtom:: Property Value = " + ctx.getText()
        self.active_objects[-1].properties[kind] = ctx.getText()
    return enterAtom

# Exit a Atom type while parsing
def create_exitAtom():
    def exitAtom(self, ctx):
        pass
    return exitAtom


reference_dict = OrderedDict()

# Setup a Dictionary of recognized & parsed grammar fields
# Use this dictionary to generate the listener functions per field inside the builder classes
meta_class_dict = OrderedDict()

# Containers/Models
meta_class_dict["Package"] = Grammar_Field("object", "Package", create_enterModel, create_exitModel)

meta_class_dict["Message"] = Grammar_Field("object", "Message", create_enterModel, create_exitModel)
meta_class_dict["Service"] = Grammar_Field("object", "Service", create_enterModel, create_exitModel)

meta_class_dict["Component"] = Grammar_Field("object", "Component", create_enterModel, create_exitModel)
meta_class_dict["Client"] = Grammar_Field("object", "Client", create_enterModel, create_exitModel)
meta_class_dict["Server"] = Grammar_Field("object", "Server", create_enterModel, create_exitModel)
meta_class_dict["Publisher"] = Grammar_Field("object", "Publisher", create_enterModel, create_exitModel)
meta_class_dict["Subscriber"] = Grammar_Field("object", "Subscriber", create_enterModel, create_exitModel)

meta_class_dict["Node"] = Grammar_Field("object", "Node", create_enterModel, create_exitModel)
meta_class_dict["Component_Instance"] = Grammar_Field("object", "Component_instance", create_enterModel, create_exitModel)
meta_class_dict["Timer"] = Grammar_Field("object", "Timer", create_enterModel, create_exitModel)
meta_class_dict["Hardware"] = Grammar_Field("object", "Hardware", create_enterModel, create_exitModel)
meta_class_dict["Hardware_Instance"] = Grammar_Field("object", "Hardware_instance", create_enterModel, create_exitModel)
meta_class_dict["Node_Instance"] = Grammar_Field("object", "Node_instance", create_enterModel, create_exitModel)
meta_class_dict["Port_Instance"] = Grammar_Field("object", "Port_instance", create_enterModel, create_exitModel)
meta_class_dict["Step"] = Grammar_Field("object", "Step", create_enterModel, create_exitModel)
meta_class_dict["Local_Code_Block"] = Grammar_Field("object", "Local_code_block", create_enterModel, create_exitModel)
meta_class_dict["Client_Using_Srv"] = Grammar_Field("object", "Client_using_srv", create_enterModel, create_exitModel)
meta_class_dict["Publish_Using_Msg"] = Grammar_Field("object", "Publish_using_msg", create_enterModel, create_exitModel)
meta_class_dict["Loop"] = Grammar_Field("object", "Loop", create_enterModel, create_exitModel)

# Properties/Atoms
meta_class_dict["name"] = Grammar_Field("string", "Name", create_enterAtom, create_exitAtom)
meta_class_dict["arch"] = Grammar_Field("string", "Arch", create_enterAtom, create_exitAtom)
meta_class_dict["value"] = Grammar_Field("string", "Value", create_enterAtom, create_exitAtom)
meta_class_dict["group"] = Grammar_Field("string", "Group", create_enterAtom, create_exitAtom)
meta_class_dict["unit"] = Grammar_Field("string", "Unit", create_enterAtom, create_exitAtom)
meta_class_dict["datatype"] = Grammar_Field("string", "Datatype", create_enterAtom, create_exitAtom)
meta_class_dict["abstract_business_logic"] = Grammar_Field("code", "Abstract_business_logic", create_enterAtom, create_exitAtom)
meta_class_dict["reference"] = Grammar_Field("hidden", "Reference", create_enterAtom, create_exitAtom)
meta_class_dict["period"] = Grammar_Field("string", "Period", create_enterAtom, create_exitAtom)
meta_class_dict["priority"] = Grammar_Field("string", "Priority", create_enterAtom, create_exitAtom)
meta_class_dict["deadline"] = Grammar_Field("string", "Deadline", create_enterAtom, create_exitAtom)
meta_class_dict["ip_address"] = Grammar_Field("string", "Ip_address", create_enterAtom, create_exitAtom)
meta_class_dict["username"] = Grammar_Field("string", "Username", create_enterAtom, create_exitAtom)
meta_class_dict["sshkey"] = Grammar_Field("string", "Sshkey", create_enterAtom, create_exitAtom)
meta_class_dict["init"] = Grammar_Field("string", "Init", create_enterAtom, create_exitAtom)
meta_class_dict["ref"] = Grammar_Field("reference", "Ref", create_enterAtom, create_exitAtom)
meta_class_dict["cmd_args"] = Grammar_Field("string", "Cmd_args", create_enterAtom, create_exitAtom)

meta_class_dict["definition"] = Grammar_Field("code")

# Properties that are references
meta_class_dict["component_instance_reference"] = Grammar_Field("reference")
meta_class_dict["message_reference"] = Grammar_Field("reference")
meta_class_dict["service_reference"] = Grammar_Field("reference")
meta_class_dict["component_reference"] = Grammar_Field("reference")
meta_class_dict["node_reference"] = Grammar_Field("reference")
meta_class_dict["port_reference"] = Grammar_Field("reference")
meta_class_dict["rhw_reference"] = Grammar_Field("reference")
meta_class_dict["hardware_reference"] = Grammar_Field("reference")

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


