#!/usr/bin/python

import wx
from collections import OrderedDict

def scale_bitmap(bitmap, width, height):
    image = wx.ImageFromBitmap(bitmap)
    image = image.Scale(width, height, wx.IMAGE_QUALITY_HIGH)
    result = wx.BitmapFromImage(image)
    return result

def getConnectionPoint(objPos,objSize,objType):
    x = -1
    y = -1
    if objType == "message" or objType == "service" or objType == "component":
        x = objPos[0] + objSize[0]
        y = objPos[1] + objSize[1] / 2
        pass
    elif objType == "timer" or objType == "publisher" or objType == "subscriber" or objType == "client" or objType == "server" or objType == "component_instance":
        x = objPos[0]
        y = objPos[1] + objSize[1] / 2
    elif objType == "node":
        pass
    elif objType == "package":
        pass
    elif objType == "workspace":
        pass
    conPos = None
    if x > 0 and y > 0:
        conPos = wx.Point(x,y)
    return conPos

class Text_Placement:
    TOP, BOTTOM, LEFT, RIGHT, CENTER, NONE = range(6)

def getWidthWithText(objSize,txtPlacement,objName,fontSize):
    width,height = objSize
    if txtPlacement == Text_Placement.TOP or txtPlacement == Text_Placement.BOTTOM:
        height += fontSize[1]
        width = max(width,len(objName)*fontSize[0])
    elif txtPlacement == Text_Placement.LEFT or txtPlacement == Text_Placement.RIGHT:
        width += len(objName)*fontSize[0]
    elif txtPlacement == Text_Placement.CENTER:
        width = max(width,len(objName)*fontSize[0])
    elif txtPlacement == Text_Placement.NONE:
        pass
    return width,height

def getTextPos(option,txtString,objPos,objSize,fontSize):
    x = -1
    y = -1
    if option == Text_Placement.TOP:
        y = objPos[1] - fontSize[1]
        x = objPos[0] + objSize[0] / 2 - (len(txtString) * fontSize[0]) /2
    elif option == Text_Placement.BOTTOM:
        y = objPos[1] + objSize[1] + fontSize[1]
        x = objPos[0] + objSize[0] / 2 - (len(txtString) * fontSize[0]) /2
    elif option == Text_Placement.LEFT:
        y = objPos[1] + objSize[1] / 2 - fontSize[1] / 2
        x = objPos[0] - (fontSize[0] * len(txtString)) /2
    elif option == Text_Placement.RIGHT:
        y = objPos[1] + objSize[1] / 2 - fontSize[1] /2
        x = objPos[0] + objSize[0] 
    elif option == Text_Placement.CENTER:
        y = objPos[1] + objSize[1] / 2
        x = objPos[0] + objSize[0] / 2 - (len(txtString) * fontSize[0])/2
    elif option == Text_Placement.NONE:
        pass
    txtPos = None
    if x > 0 and y > 0:
        txtPos = wx.Point(x,y)
    return txtPos

class Draw_Method:
    RECT, ROUND_RECT, ICON = range(3)

class Draw_Style:
    def __init__(self, 
                 icon=None, 
                 font=(10,20), 
                 method=Draw_Method.ICON, 
                 placement=Text_Placement.TOP,
                 overlay = OrderedDict(),
                 padding = (10,10),
                 offset = (10,10),
                 minSize = (30,30)
             ):
        self.icon = icon
        self.fontSize = font
        self.method = method
        self.textPlacement = placement
        self.overlay = overlay
        self.padding = padding
        self.offset = offset
        self.minSize = minSize


'''
Drawable Objects are anything that should
be drawn into the viewer of ROSMOD.
This includes Packages, Workspaces,
Hardware, Deployments, and all of their
sub-objects.
'''
class Drawable_Object:
    def __init__(self):
        # these are used by all generator classes:
        self.parent = None
        self.children = []
        self.kind = ""
        self.properties = OrderedDict()

        # these are configured (default or by user)
        self.style = Draw_Style()

        # these are calculated by the Layout() function
        self.topLeft = wx.Point()
        self.height = 0
        self.width = 0
        self.textCenter = wx.Point()
        self.connectionPoint = wx.Point()
        
    def add(self, child):
        self.children.append(child)
        
    def delete(self, reference):
        self.children = [child for child in self.children if child != reference]

    '''
    Draw() is called after layout has been calculated
    Should receive the device context
    '''
    def Draw(self, dc):
        x,y = self.topLeft.Get()
        if self.style.method == Draw_Method.ICON:
            if self.style.icon != None:
                bmp = scale_bitmap(self.style.icon, self.width, self.height)
                dc.DrawBitmap(bmp,x,y)
        elif self.style.method == Draw_Method.RECT:
            pass
        elif self.style.method == Draw_Method.ROUND_RECT:
            pass
        else:
            pass
        if self.textCenter != None:
            dc.DrawText(self.properties["name"],self.textCenter.Get()[0],self.textCenter.Get()[1])
        for child in self.children:
            child.Draw(dc)

'''
The Layout function takes a top-level
drawable object and it's Position as input, and configures
it recursively by going through it's children
to determine the parent's final size.

The Children are laid out inside the parent according 
to some layout options

The layout function returns the maximum size (h,w) of the 
laid-out object; NOTE: this is not the h,w of the object
necessarily, as the text may extend beyond the object
and this would capture the extent of the text
'''
def Layout(dObj, topLeftPos):
    # configure things that will be returned
    padding = dObj.style.padding
    minSize = dObj.style.minSize
    offset = dObj.style.offset
    maxObjWidth = minSize[0]
    maxObjHeight = minSize[1]
    dObj.topLeft = wx.Point(topLeftPos[0],topLeftPos[1])
    childPos = [topLeftPos[0] + offset[0], topLeftPos[1] + offset[1]]
    if dObj.kind == "workspace":
        pass
    elif dObj.kind == "package":
        messages = []
        services = []
        components = []
        nodes = []
        for obj in dObj.children:
            if obj.kind == "message":
                messages.append(obj)
            elif obj.kind == "service":
                services.append(obj)
            elif obj.kind == "component":
                components.append(obj)
            elif obj.kind == "node":
                nodes.append(obj)
        maxWidth = 0
        maxHeight = 0
        # now Layout the Objects
        # draw label here for messages
        for msg in messages:
            w,h = Layout(msg,childPos)
            childPos[1] += padding[1] + h
            maxWidth = max(w,maxWidth)
        # draw label here for services
        for srv in services:
            w,h = Layout(srv,childPos)
            childPos[1] += padding[1] + h
            maxWidth = max(w,maxWidth)
        maxObjHeight = max(maxObjHeight,childPos[1])
        maxObjWidth = max(maxObjWidth, maxWidth)
        childPos = [childPos[0] + padding[0] + maxWidth,topLeftPos[1] + padding[1]]
        maxWidth = 0
        maxHeight = 0
        # draw label here for components
        for comp in components:
            w,h = Layout(comp,childPos)
            childPos[1] += padding[1] + h
            maxWidth = max(w,maxWidth)
        maxObjHeight = max(maxObjHeight,childPos[1])
        maxObjWidth = max(maxObjWidth, maxWidth)
        childPos = [childPos[0] + padding[0] + maxWidth,topLeftPos[1] + padding[1]]
        maxWidth = 0
        maxHeight = 0
        # draw label here for nodes
        for node in nodes:
            w,h = Layout(node,childPos)
            childPos[1] += padding[1] + h
            maxWidth = max(w,maxWidth)
        maxObjHeight = max(maxObjHeight,childPos[1])
        maxObjWidth = max(maxObjWidth, maxWidth)
    elif dObj.kind == "component" or dObj.kind == "node":
        maxWidth = 0
        maxHeight = 0
        for obj in dObj.children:
            w,h = Layout(obj,childPos)
            childPos[1] += padding[1] + h
            maxWidth = max(w,maxWidth)
        maxObjHeight = max(maxObjHeight,childPos[1])
        maxObjWidth = max(maxObjWidth, maxWidth)    
    elif dObj.kind == "message" or dObj.kind == "service" or dObj.kind == "timer" or dObj.kind == "client" or dObj.kind == "server" or dObj.kind == "publisher" or dObj.kind == "subscriber" or dObj.kind == "component_instance":
        pass
    elif dObj.kind == "hardware":
        pass
    elif dObj.kind == "deployment":
        pass
    else:
        pass
    dObj.width = maxObjWidth
    dObj.height = maxObjHeight
    dObj.connectionPoint = getConnectionPoint(
        objPos = dObj.topLeft.Get(),
        objSize = (dObj.width,dObj.height),
        objType = dObj.kind
    )
    dObj.textCenter = getTextPos(
        option = dObj.style.textPlacement,
        txtString = dObj.properties["name"],
        objPos = dObj.topLeft.Get(),
        objSize = (dObj.width,dObj.height),
        fontSize = dObj.style.fontSize
    )
    maxObjWidth, maxObjHeight = getWidthWithText(
        objSize = (dObj.width,dObj.height),
        txtPlacement = dObj.style.textPlacement,
        objName = dObj.properties["name"],
        fontSize = dObj.style.fontSize
    )
    return maxObjWidth,maxObjHeight

def Configure(dObj,styleDict):
    dObj.style = styleDict[dObj.kind]
    if dObj.kind == "package" or dObj.kind == "component" or dObj.kind == "node":
        for child in dObj.children:
            Configure(child,styleDict)
    else:
        pass    
