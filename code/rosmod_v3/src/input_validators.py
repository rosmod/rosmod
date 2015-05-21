def name_validator(project, obj, name):
    error = None
    objType = obj.kind
    sameScopeObjects = obj.parent.getChildrenByKind(objType)
    invalidNames = [x.properties['name'] for x in sameScopeObjects if x != obj]
    if name in invalidNames:
        name = None
        error = "Name must be unique within scope"
    return name,error

def datatype_validator(project, obj, datatype):
    valid_datatypes = ["Base", "I/O"]
    error = None
    if datatype == None: # pass None to get possible datatypes
        datatype = valid_datatypes
    else: # pass a datatype in to check it 
        if datatype not in valid_datatypes:
            datatype = None
            error = "Datatype must be one of {}".format(valid_datatypes)
    return datatype, error

def port_reference_validator(project, obj, port_ref):
    error = None
    activePortRefs = [x.properties['port_reference'] for x in obj.parent.children if x != obj]
    if port_ref in activePortRefs:
        port_ref = None
        error = "Another port instance already references this port!"
    return port_ref, error

