
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
#  the types of its children
#  the type of its parent
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
    ["Message","Service","Component","Node"],
    ["name"],
    [], 
    [])

model_dict["Message"] = Meta_Entity(
    "Package", 
    ["Field"], 
    ["name"],
    ["Publisher","Subscriber"], 
    [])
model_dict["Field"] = Meta_Entity(
    "Message", 
    [], 
    ["datatype","name","value"],
    [], 
    [])

model_dict["Service"] = Meta_Entity(
    "Package", 
    ["Request","Response"],
    ["name"],
    ["Server","Client"], 
    [])
model_dict["Request"] = Meta_Entity(
    "Service",
    ["Field"],
    ["name"],
    [], 
    [])
model_dict["Response"] = Meta_Entity(
    "Service", 
    ["Field"],
    ["name"],
    [], 
    [])

model_dict["Component"] = Meta_Entity(
    "Package", 
    ["Timer","Publisher","Subscriber","Client","Server"], 
    ["name"],
    ["Component_Instance"], 
    [])
model_dict["Client"] = Meta_Entity(
    "Component", 
    [], 
    ["name","service_reference","reference"],
    ["Port_Instance"], 
    ["Service"])
model_dict["Server"] = Meta_Entity(
    "Component", 
    [],
    ["name","service_reference","reference","priority","deadline"],
    ["Port_Instance"], 
    ["Service"])
model_dict["Publisher"] = Meta_Entity(
    "Component", 
    [], 
    ["name","message_reference","reference"],
    ["Port_Instance"], 
    ["Message"])
model_dict["Subscriber"] = Meta_Entity(
    "Component", 
    [],
    ["name","message_reference","reference","priority","deadline"],
    ["Port_Instance"], 
    ["Message"])
model_dict["Timer"] = Meta_Entity(
    "Component", 
    [],
    ["name","period","priority","deadline"],
    [], 
    [])

model_dict["Node"] = Meta_Entity(
    "Package", 
    ["Component_Instance"], 
    ["name"],
    ["Node_Instance"], 
    [])
model_dict["Component_Instance"] = Meta_Entity(
    "Node", 
    [], 
    ["name","component_reference","reference"],
    [], 
    ["Component"])

model_dict["rhw"] = Meta_Entity(
    "Project", 
    ["Hardware"], 
    ["name"],
    ["rdp"], 
    [])
model_dict["Hardware"] = Meta_Entity(
    "rhw", 
    [], 
    ["name","arch"],
    ["Hardware_Instance"], 
    [])

model_dict["rdp"] = Meta_Entity(
    "Project", 
    ["Hardware_Instance"], 
    ["name","rhw_reference"],
    [], 
    ["rhw"])
model_dict["Hardware_Instance"] = Meta_Entity(
    "rdp", 
    ["Node_Instance"],
    ["name","ip_address","username","sshkey","init"], 
    [], 
    ["Hardware"])
model_dict["Node_Instance"] = Meta_Entity(
    "Hardware_Instance", 
    ["Port_Instance"], 
    ["name","node_reference","reference","cmd_args"],
    [], 
    ["Node"])
model_dict["Port_Instance"] = Meta_Entity(
    "Node_Instance", 
    [], 
    ["name","port_reference","reference","group"],
    [], 
    ["Client","Server","Publisher","Subscriber","Timer"])
