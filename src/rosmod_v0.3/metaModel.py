
from collections import OrderedDict

# 
class Meta_Entity:
    def __init__(self, parent = "", children = [], properties = [], in_refs = [], out_refs = [], display_refs = [], context_funcs = OrderedDict()):
        self.parent = parent
        self.children = children
        self.properties = properties
        self.in_refs = in_refs
        self.out_refs = out_refs
        self.display_refs = display_refs
        self.context_funcs = context_funcs

# Set up a dictionary which desribes the meta-model
# for each entity in the metamodel:
#  the type of its parent
#  the types of its children
#  the properties it has
#  the types of objects which can refer to it
#  the types of objects to which it can refer
model_dict = OrderedDict()

model_dict["rml"] = Meta_Entity(
    parent = "Project",
    children = ["Package"],
    properties = ["name"],
    in_refs = [],
    out_refs = [])

model_dict["Package"] = Meta_Entity(
    parent = "rml", 
    children = ["Message","Service","Component"],
    properties = ["name"],
    in_refs = [], 
    out_refs = [])

model_dict["Message"] = Meta_Entity(
    parent = "Package", 
    children = [], 
    properties = ["name","definition"],
    in_refs = [["Publisher","message_reference"],["Subscriber","message_reference"]], 
    out_refs = [])
model_dict["Service"] = Meta_Entity(
    parent = "Package", 
    children = [],
    properties = ["name","definition"],
    in_refs = [["Server","service_reference"],["Client","service_reference"]], 
    out_refs = [])

model_dict["Component"] = Meta_Entity(
    parent = "Package", 
    children = ["Timer","Publisher","Subscriber","Client","Server"], 
    properties = ["name","datatype"],
    in_refs = [["Component_Instance","component_reference"]], 
    out_refs = [])
model_dict["Client"] = Meta_Entity(
    parent = "Component", 
    children = [], 
    properties = ["name","service_reference","reference", "port_network_profile"],
    in_refs = [["Port_Instance","port_reference"]], 
    out_refs = [["Service","project"]])
model_dict["Server"] = Meta_Entity(
    parent = "Component", 
    children = [],
    properties = ["name","service_reference","reference","priority","deadline","abstract_business_logic"],
    in_refs = [["Port_Instance","port_reference"]], 
    out_refs = [["Service","project"]])
model_dict["Publisher"] = Meta_Entity(
    parent = "Component", 
    children = [], 
    properties = ["name","message_reference","reference", "port_network_profile"],
    in_refs = [["Port_Instance","port_reference"]], 
    out_refs = [["Message","project"]])
model_dict["Subscriber"] = Meta_Entity(
    parent = "Component", 
    children = [],
    properties = ["name","message_reference","reference","priority","deadline", "port_network_profile", "abstract_business_logic"],
    in_refs = [["Port_Instance","port_reference"]], 
    out_refs = [["Message","project"]])
model_dict["Timer"] = Meta_Entity(
    parent = "Component", 
    children = [],
    properties = ["name","period","priority","deadline","abstract_business_logic"],
    in_refs = [], 
    out_refs = [])

model_dict["rhw"] = Meta_Entity(
    parent = "Project", 
    children = ["Hardware"], 
    properties = ["name"],
    in_refs = [["rdp","rhw_reference"]], 
    out_refs = [])
model_dict["Hardware"] = Meta_Entity(
    parent = "rhw", 
    children = [], 
    properties = ["name","ip_address","username","sshkey","deployment_path","install_path","init","arch","system_network_profile"],
    in_refs = [["Node","hardware_reference"]], 
    out_refs = [])
model_dict["rdp"] = Meta_Entity(
    parent = "Project", 
    children = ["Node"], 
    properties = ["name","rhw_reference"],
    in_refs = [], 
    out_refs = [["rhw","project"]])
model_dict["Node"] = Meta_Entity(
    parent = "rdp", 
    children = ["Component_Instance"], 
    properties = ["name","hardware_reference","priority","cmd_args","deployment_path"],
    in_refs = [], 
    out_refs = [["Hardware", "parent", "rhw_reference"]],
    display_refs = [["Node",["parent"],"hardware_reference","hardware_reference"]],
    context_funcs = [])
model_dict["Component_Instance"] = Meta_Entity(
    parent = "Node", 
    children = ["Port_Instance"], 
    properties = ["name","component_reference","reference","scheduling_scheme", "logging_critical","logging_error","logging_warning","logging_info", "logging_debug"],
    in_refs = [], 
    out_refs = [["Component","project"]],
    display_refs = [["Component_Instance",["parent","parent"],"component_reference","component_reference"]])
model_dict["Port_Instance"] = Meta_Entity(
    parent = "Component_Instance", 
    children = [], 
    properties = ["name","port_reference","reference","group"],
    in_refs = [], 
    out_refs = [["Client","parent","component_reference"],["Server","parent","component_reference"],["Publisher","parent","component_reference"],["Subscriber","parent","component_reference"]],
    display_refs = [["Port_Instance",["parent","parent","parent"],"group","group"]])
