def name_validator(project, obj, name):
    objType = obj.kind
    sameScopeObjects = obj.parent.getChildrenByKind(objType)
    invalidNames = [x.properties['name'] for x in sameScopeObjects if x != obj]
    if name in invalidNames:
        name = None
    return name

def datatype_validator(project, obj, datatype):
    valid_datatypes = ["Base", "I/O"]
    if datatype == None: # pass None to get possible datatypes
        datatype = valid_datatypes
    else: # pass a datatype in to check it 
        if datatype not in valid_datatypes:
            datatype = None
    return datatype

def port_reference_validator(project, obj, port_ref):
    return port_ref

