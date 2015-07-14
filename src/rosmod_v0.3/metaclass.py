# ROSMOD Grammar MetaClass
# Author: Pranav Srinivas Kumar
# Date: 2015.04.27

from collections import OrderedDict
from metaModel import model_dict
from drawable import Drawable_Object
from input_validators import *

# Grammar Field class used to generate grammar field-specific listener functions
class Grammar_Field:
    def __init__(self, kind = "", 
                 name = "", 
                 entry_point = None, 
                 exit_point = None, 
                 input_validator = None, 
                 display_name = ""):
        self.kind = kind
        self.name = name
        self.entry_point = entry_point
        self.exit_point = exit_point
        self.input_validator = input_validator
        self.display_name = display_name

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
# RML RELATED OBJECTS
meta_class_dict["Package"] = Grammar_Field(
    "object", "Package", create_enterModel, create_exitModel, None, "Package")
meta_class_dict["Message"] = Grammar_Field(
    "object", "Message", create_enterModel, create_exitModel, None, "Message")
meta_class_dict["Service"] = Grammar_Field(
    "object", "Service", create_enterModel, create_exitModel, None, "Service")
meta_class_dict["Component"] = Grammar_Field(
    "object", "Component", create_enterModel, create_exitModel, None, "Component")
meta_class_dict["Client"] = Grammar_Field(
    "object", "Client", create_enterModel, create_exitModel, None, "Client")
meta_class_dict["Server"] = Grammar_Field(
    "object", "Server", create_enterModel, create_exitModel, None, "Server")
meta_class_dict["Publisher"] = Grammar_Field(
    "object", "Publisher", create_enterModel, create_exitModel, None, "Publisher")
meta_class_dict["Subscriber"] = Grammar_Field(
    "object", "Subscriber", create_enterModel, create_exitModel, None, "Subscriber")
meta_class_dict["Timer"] = Grammar_Field(
    "object", "Timer", create_enterModel, create_exitModel, None, "Timer")

# RHW RELATED OBJECTS
meta_class_dict["Hardware"] = Grammar_Field(
    "object", "Hardware", create_enterModel, create_exitModel, None, "Hardware")

# RDP RELATED OBJECTS
meta_class_dict["Node"] = Grammar_Field(
    "object", "Node", create_enterModel, create_exitModel, None, "Node")
meta_class_dict["Component_Instance"] = Grammar_Field(
    "object", "Component_instance", create_enterModel, create_exitModel, None, "Component Instance")
meta_class_dict["Port_Instance"] = Grammar_Field(
    "object", "Port_instance", create_enterModel, create_exitModel, None, "Port Instance")

# Properties/Atoms
meta_class_dict["name"] = Grammar_Field(
    "string", "Name", create_enterAtom, create_exitAtom, name_validator, "Name")
meta_class_dict["arch"] = Grammar_Field(
    "string", "Arch", create_enterAtom, create_exitAtom, None, "Architecture")
meta_class_dict["value"] = Grammar_Field(
    "string", "Value", create_enterAtom, create_exitAtom, None, "Value")
meta_class_dict["group"] = Grammar_Field(
    "string", "Group", create_enterAtom, create_exitAtom, None, "Group")

meta_class_dict["logging_debug"] = Grammar_Field(
    "boolean", "Logging_debug", create_enterAtom, create_exitAtom, None, "Log Level - DEBUG")
meta_class_dict["logging_info"] = Grammar_Field(
    "boolean", "Logging_info", create_enterAtom, create_exitAtom, None, "Log Level - INFO")
meta_class_dict["logging_warning"] = Grammar_Field(
    "boolean", "Logging_warning", create_enterAtom, create_exitAtom, None, "Log Level - WARNING")
meta_class_dict["logging_error"] = Grammar_Field(
    "boolean", "Logging_error", create_enterAtom, create_exitAtom, None, "Log Level - ERROR")
meta_class_dict["logging_critical"] = Grammar_Field(
    "boolean", "Logging_critical", create_enterAtom, create_exitAtom, None, "Log Level - CRITICAL")

valid_datatypes = ["Base", "KSP"]
valid_scheduling_schemes = ["FIFO", "PFIFO", "EDF"]
meta_class_dict["datatype"] = Grammar_Field(
    "list", 
    "Datatype", 
    create_enterAtom, 
    create_exitAtom, 
    lambda p,o,s,k : list_validator(valid_datatypes,p,o,s,k), 
    "Component Type")
meta_class_dict["scheduling_scheme"] = Grammar_Field(
    "list", 
    "Scheduling_scheme", 
    create_enterAtom, 
    create_exitAtom, 
    lambda p,o,s,k : list_validator(valid_scheduling_schemes,p,o,s,k), 
    "Scheduling Scheme")

valid_comm_layers = ["ROSMOD", "ROSCPP"]
meta_class_dict["communication_layer"] = Grammar_Field(
    "list", 
    "", 
    create_enterAtom, 
    create_exitAtom, 
    lambda p,o,s,k : list_validator(valid_comm_layers,p,o,s,k), 
    "Communication Layer")

meta_class_dict["reference"] = Grammar_Field(
    "hidden", "Reference", create_enterAtom, create_exitAtom, None, "Reference")
meta_class_dict["period"] = Grammar_Field(
    "double", "Period", create_enterAtom, create_exitAtom, None, "Period (s)")
meta_class_dict["priority"] = Grammar_Field(
    "integer", "Priority", create_enterAtom, create_exitAtom, None, "Priority")
meta_class_dict["deadline"] = Grammar_Field(
    "double", "Deadline", create_enterAtom, create_exitAtom, None, "Deadline (s)")
meta_class_dict["ip_address"] = Grammar_Field(
    "string", "Ip_address", create_enterAtom, create_exitAtom, None, "IP Address")
meta_class_dict["username"] = Grammar_Field(
    "string", "Username", create_enterAtom, create_exitAtom, None, "Username")
meta_class_dict["sshkey"] = Grammar_Field(
    "string", "Sshkey", create_enterAtom, create_exitAtom, None, "SSH Key")
meta_class_dict["deployment_path"] = Grammar_Field(
    "string", "Deployment_path", create_enterAtom, create_exitAtom, None, "Deployment Path")
meta_class_dict["install_path"] = Grammar_Field(
    "string", "Install_path", create_enterAtom, create_exitAtom, None, "ROS Installation Path")
meta_class_dict["init"] = Grammar_Field(
    "string", "Init", create_enterAtom, create_exitAtom, None, "Initialization Script")
meta_class_dict["ref"] = Grammar_Field(
    "reference", "Ref", create_enterAtom, create_exitAtom, None, "Reference")
meta_class_dict["cmd_args"] = Grammar_Field(
    "string", "Cmd_args", create_enterAtom, create_exitAtom, None, "Command-line Arguments")

meta_class_dict["abstract_business_logic"] = Grammar_Field( # used for ABL for callbacks
    kind="code", 
    name="Abstract_business_logic", 
    entry_point=create_enterAtom, 
    exit_point=create_exitAtom, 
    input_validator=None, 
    display_name="Abstract Business Logic")

meta_class_dict["definition"] = Grammar_Field( # used for msg/srv definitions
    kind="code",
    display_name="Definition")

meta_class_dict["port_network_profile"] = Grammar_Field(
    kind="code", 
    display_name = "Port Network Profile\n<time,bandwidth,latency,interface>")
meta_class_dict["system_network_profile"] = Grammar_Field(
    kind="code", 
    display_name = "System Network Profile\n<time,bandwidth,latency,interface>")

# related only to deployment options
meta_class_dict["numPeriods"] = Grammar_Field(
    kind="string", 
    display_name = "Number of Periods")
meta_class_dict["command"] = Grammar_Field(
    kind="string", 
    display_name = "Remote Command")

# Properties that are references
meta_class_dict["message_reference"] = Grammar_Field(
    kind="reference",
    display_name="Message",
    input_validator=ref_validator)
meta_class_dict["service_reference"] = Grammar_Field(
    kind="reference",
    display_name="Service",
    input_validator=ref_validator)
meta_class_dict["component_reference"] = Grammar_Field(
    kind="reference",
    display_name="Component",
    input_validator=ref_validator)
meta_class_dict["port_reference"] = Grammar_Field(
    kind="reference",
    display_name="Port",
    input_validator=singleton_ref_validator)
meta_class_dict["rhw_reference"] = Grammar_Field(
    kind="reference",
    display_name="RHW",
    input_validator=ref_validator)
meta_class_dict["hardware_reference"] = Grammar_Field(
    kind="reference",
    display_name="Hardware",
    input_validator=ref_validator)

# code-generation/preserveration related properties : should be hidden from user
meta_class_dict["init_business_logic"] = Grammar_Field("hidden")
meta_class_dict["user_includes"] = Grammar_Field("hidden")
meta_class_dict["user_globals"] = Grammar_Field("hidden")
meta_class_dict["hpp_globals"] = Grammar_Field("hidden")
meta_class_dict["user_private_variables"] = Grammar_Field("hidden")
meta_class_dict["destructor"] = Grammar_Field("hidden")
meta_class_dict["cmakelists_packages"] = Grammar_Field("hidden")
meta_class_dict["cmakelists_functions"] = Grammar_Field("hidden")
meta_class_dict["cmakelists_include_dirs"] = Grammar_Field("hidden")
meta_class_dict["cmakelists_cpp_marker"] = Grammar_Field("hidden")
meta_class_dict["cmakelists_targetlinklibs_marker"] = Grammar_Field("hidden")

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


