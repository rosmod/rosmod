from metaModel import model_dict

def name_validator(project, obj, name, key):
    error = None
    if obj != None:
        objType = obj.kind
        sameScopeObjects = obj.parent.getChildrenByKind(objType)
        invalidNames = [x.properties['name'] for x in sameScopeObjects if x != obj]
        if name in invalidNames:
            name = None
            error = "Name must be unique within scope"
    return name,error

def list_validator(options,project, obj, selection, key):
    error = None
    if selection == None: # pass None to get possible selections
        selection = options
    else: # pass a selection in to check it 
        if selection not in options:
            selection = None
            error = "Selection must be one of {}".format(options)
    return selection, error

def ref_validator(project, obj, ref, key):
    error = None
    references = []
    if obj != None:
        refObjTypes = model_dict[obj.kind].out_refs
        for refObjType in refObjTypes:
            if refObjType[1] == 'project':
                references.extend(project.getChildrenByKind(refObjType[0]))
            elif refObjType[1] == 'parent':
                references.extend(obj.parent.properties[refObjType[2]].getChildrenByKind(refObjType[0]))
        if references == []:
            ref = None
            error = "No reference-able {} exists!".format(key.split('_')[0])
        else:
            if ref != None:
                if ref not in references:
                    ref = None
                    error = "Reference does not exist!"
            else:
                ref = references
    return ref, error

def singleton_ref_validator(project, obj, ref, key):
    error = None
    references = []
    activeRefs = []
    if obj != None:
        refObjTypes = model_dict[obj.kind].out_refs
        for refObjType in refObjTypes:
            if refObjType[1] == 'project':
                references.extend(project.getChildrenByKind(refObjType[0]))
            elif refObjType[1] == 'parent':
                references.extend(obj.parent.properties[refObjType[2]].getChildrenByKind(refObjType[0]))
        activeRefs = [x.properties[key] for x in obj.parent.children if x != obj]
        references = [x for x in references if x not in activeRefs]
        if references == []:
            ref = None
            error = "No reference-able {} exists!".format(key.split('_')[0])
        else:
            if ref != None:
                if ref in activeRefs:
                    ref = None
                    error = "Another object already references this {}!".format(refObjType[0])
                elif ref not in references:
                    ref = None
                    error = "No reference-able {} exists!".format(key.split('_')[0])
            else:
                ref = references
    return ref, error

