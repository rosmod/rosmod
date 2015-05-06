
from collections import OrderedDict

# 
class Meta_Entity:
    def __init__(self, parent = "", children = [], properties = [], in_refs = [], out_refs = []):
        self.parent = parent
        self.children = children
        self.properties = properties
        self.in_refs = in_refs
        self.out_refs = out_refs

# Set up a dictionary which desribes the meta-model
# for each entity in the metamodel:
#  the type of its parent
#  the types of its children
#  the properties it has
#  the types of objects which can refer to it
#  the types of objects to which it can refer
model_dict = OrderedDict()

model_dict["rml"] = Meta_Entity(
    "Project",
    ["Package"],
    ["name"],
    [],
    [])

model_dict["Package"] = Meta_Entity(
    "rml", 
    ["Message","Service","Component"],
    ["name"],
    [], 
    [])

model_dict["Message"] = Meta_Entity(
    "Package", 
    [], 
    ["name","definition"],
    [["Publisher","message_reference"],["Subscriber","message_reference"]], 
    [])
model_dict["Service"] = Meta_Entity(
    "Package", 
    [],
    ["name","definition"],
    [["Server","service_reference"],["Client","service_reference"]], 
    [])

model_dict["Component"] = Meta_Entity(
    "Package", 
    ["Timer","Publisher","Subscriber","Client","Server"], 
    ["name","datatype","scheduling_scheme", "logging_critical","logging_error","logging_warning","logging_info", "logging_debug"],
    [["Component_Instance","component_reference"]], 
    [])
model_dict["Client"] = Meta_Entity(
    "Component", 
    [], 
    ["name","service_reference","reference", "port_network_profile"],
    [["Port_Instance","port_reference"]], 
    [["Service","project"]])
model_dict["Server"] = Meta_Entity(
    "Component", 
    [],
    ["name","service_reference","reference","priority","deadline","abstract_business_logic"],
    [["Port_Instance","port_reference"]], 
    [["Service","project"]])
model_dict["Publisher"] = Meta_Entity(
    "Component", 
    [], 
    ["name","message_reference","reference", "port_network_profile"],
    [["Port_Instance","port_reference"]], 
    [["Message","project"]])
model_dict["Subscriber"] = Meta_Entity(
    "Component", 
    [],
    ["name","message_reference","reference","priority","deadline","abstract_business_logic"],
    [["Port_Instance","port_reference"]], 
    [["Message","project"]])
model_dict["Timer"] = Meta_Entity(
    "Component", 
    [],
    ["name","period","priority","deadline","abstract_business_logic"],
    [], 
    [])

model_dict["rhw"] = Meta_Entity(
    "Project", 
    ["Hardware"], 
    ["name"],
    [["rdp","rhw_reference"]], 
    [])
model_dict["Hardware"] = Meta_Entity(
    "rhw", 
    [], 
    ["name","arch"],
    [["Hardware_Instance","hardware_reference"]], 
    [])
model_dict["rdp"] = Meta_Entity(
    "Project", 
    ["Hardware_Instance"], 
    ["name","rhw_reference"],
    [], 
    [["rhw","project"]])
model_dict["Hardware_Instance"] = Meta_Entity(
    "rdp", 
    ["Node"],
    ["name","hardware_reference","ip_address","username","sshkey","deployment_path","init","reference"], 
    [], 
    [["Hardware","parent","rhw_reference"]])

model_dict["Node"] = Meta_Entity(
    "Hardware_Instance", 
    ["Component_Instance"], 
    ["name","priority","cmd_args"],
    [], 
    [])
model_dict["Component_Instance"] = Meta_Entity(
    "Node", 
    ["Port_Instance"], 
    ["name","component_reference","reference"],
    [], 
    [["Component","project"]])
model_dict["Port_Instance"] = Meta_Entity(
    "Component_Instance", 
    [], 
    ["name","port_reference","reference","group"],
    [], 
    [["Client","parent","component_reference"],["Server","parent","component_reference"],["Publisher","parent","component_reference"],["Subscriber","parent","component_reference"]])
