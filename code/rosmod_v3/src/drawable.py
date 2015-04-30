#!/usr/bin/python

import wx
import math
from collections import OrderedDict
import copy

# need float canvas for new style of rendering
from wx.lib.floatcanvas import NavCanvas, FloatCanvas, Resources

from metaModel import model_dict

class Text_Placement:
    TOP, BOTTOM, LEFT, RIGHT, CENTER, NONE = range(6)

class Child_Layout:
    ROWS, COLUMNS, STACK, LINE, SQUARE = range(5)

class Draw_Method:
    RECT, ROUND_RECT, ICON, HIDDEN = range(4)

def drawText(text,pos,style,canvas):
    if style.textPlacement != Text_Placement.NONE:
        posStr = "cc"
        if style.textPlacement == Text_Placement.TOP:
            posStr = "bc"
        elif style.textPlacement == Text_Placement.BOTTOM:
            posStr = "tc"
        elif style.textPlacement == Text_Placement.CENTER:
            posStr = "cc"
        elif style.textPlacement == Text_Placement.LEFT:
            posStr = "cr"
        elif style.textPlacement == Text_Placement.RIGHT:
            posStr = "cl"
        return canvas.AddScaledText(
            text,
            pos,
            Size=style.font['pointSize'],
            Position=posStr,
            Family=wx.MODERN
        )
    else:
        return None

def getWidthWithText(objSize,style,objName,canvas):
    txtPlacement = style.textPlacement
    txt = drawText(objName,(0,0),style,canvas)
    txtWidth = 0
    txtHeight = 0
    if txt != None:
        txt.CalcBoundingBox()
        BB = txt.BoundingBox
        txtHeight = abs(BB[0][1] - BB[1][1]) * 1.2
        txtWidth = abs(BB[0][0] - BB[1][0]) * 1.2
        canvas.RemoveObject(txt)
    width,height = objSize
    if txtPlacement == Text_Placement.TOP or txtPlacement == Text_Placement.BOTTOM:
        width = max(width,txtWidth)
        height += txtHeight
    elif txtPlacement == Text_Placement.LEFT or txtPlacement == Text_Placement.RIGHT:
        width += txtWidth
    elif txtPlacement == Text_Placement.CENTER:
        width = max(width,txtWidth)
    elif txtPlacement == Text_Placement.NONE:
        pass
    return width,height

def getTextPos(option,txtString,objPos,objSize,font):
    x = -1
    y = -1
    if option == Text_Placement.TOP:
        y = objPos[1]
        x = objPos[0] + objSize[0] / 2 
    elif option == Text_Placement.BOTTOM:
        y = objPos[1] - objSize[1]
        x = objPos[0] + objSize[0] / 2 
    elif option == Text_Placement.LEFT:
        y = objPos[1] - objSize[1] / 2 
        x = objPos[0] 
    elif option == Text_Placement.RIGHT:
        y = objPos[1] - objSize[1] / 2 
        x = objPos[0] + objSize[0]
    elif option == Text_Placement.CENTER:
        y = objPos[1] - objSize[1] / 2
        x = objPos[0] + objSize[0] / 2 
    elif option == Text_Placement.NONE:
        pass
    txtPos = wx.Point(x,y)
    return txtPos

class Draw_Style:
    def __init__(self, 
                 icon=None, 
                 font=OrderedDict([('pointSize',20)]), 
                 method=Draw_Method.ICON, 
                 placement=Text_Placement.TOP,
                 overlay = OrderedDict(),
                 padding = (10,10),
                 offset = (10,10),
                 minSize = (30,30)
             ):
        self.icon = icon
        self.font = font
        self.method = method
        self.textPlacement = placement
        self.overlay = overlay
        self.padding = padding
        self.offset = offset
        self.minSize = minSize

    def Copy(self,other):
        self.icon = other.icon
        self.font = copy.deepcopy(other.font)
        self.method = copy.deepcopy(other.method)
        self.textPlacement = copy.deepcopy(other.textPlacement)
        self.overlay = copy.deepcopy(other.overlay)
        self.padding = copy.deepcopy(other.padding)
        self.offset = copy.deepcopy(other.offset)
        self.minSize = copy.deepcopy(other.minSize)


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
        self.kind = ""
        self.properties = OrderedDict()
        self.children = []

        # these are configured (default or by user)
        self.style = Draw_Style()

        # these are calculated by the Layout() function
        self.topLeft = wx.Point()
        self.height = 0
        self.width = 0
        self.textPosition = wx.Point()
        
    def add(self, child):
        child.parent = self
        self.children.append(child)
        
    def delete(self):
        self.parent.children = [x for x in self.parent.children if x != self]

    def deleteAllRefs(self,project):
        referringObjectTypes = model_dict[self.kind].in_refs
        for refObjType,refName in referringObjectTypes:
            refObjs = project.getChildrenByKind(refObjType)
            for refObj in refObjs:
                if refObj.properties[refName] == self:
                    refObj.deleteAllRefs(project)
                    refObj.delete()
                    del refObj

    def getChildrenByKind(self,kind):
        if self.kind == kind:
            return [self]
        else:
            retKids = []
            for child in self.children:
                retKids.extend(child.getChildrenByKind(kind))
            return retKids

    '''
    Draw() is called after layout has been calculated
    Should receive the device context
    '''
    def Draw(self, canvas, leftClickFunc, rightClickFunc, leftDClickFunc):
        if self.style.method == Draw_Method.HIDDEN:
            return
        x,y = self.topLeft.Get()
        dObj = None
        if self.style.method == Draw_Method.ICON:
            if self.style.icon != None:
                dObj = canvas.AddScaledBitmap(
                    self.style.icon,
                    XY = (x,y),
                    Height = self.height,
                    Position = "tl")
                dObj.HitFill = True
                dObj.Name = self
        elif self.style.method == Draw_Method.RECT:
            pass
        elif self.style.method == Draw_Method.ROUND_RECT:
            dObj = canvas.AddRectangle(
                (x,y-self.height),
                (self.width,self.height),
                FillColor = self.style.overlay["fillColor"],
                InForeground = False
            )
            dObj.HitFill = True
            dObj.Name = self
        else:
            pass
        if dObj != None:
            dObj.Bind(FloatCanvas.EVT_FC_RIGHT_UP, rightClickFunc)
            dObj.Bind(FloatCanvas.EVT_FC_LEFT_UP, leftClickFunc)
            dObj.Bind(FloatCanvas.EVT_FC_LEFT_DCLICK, leftDClickFunc)
        if 'overlayColor' in self.style.overlay.keys():
            rect = canvas.AddRectangle(
                (x,y-self.height),
                (self.width,self.height),
                LineColor = self.style.overlay['overlayColor'],
                LineWidth = 5,
                FillColor = self.style.overlay['overlayColor'],
                InForeground = False,
                FillStyle = 'Transparent'
            )
        if self.textPosition != None and "name" in self.properties.keys():
            drawText(self.properties["name"],self.textPosition.Get(),self.style,canvas)
        canvas.AddPoint(self.topLeft.Get())
        canvas.AddPoint(self.textPosition.Get())
        for child in self.children:
            child.Draw(canvas,leftClickFunc,rightClickFunc,leftDClickFunc)

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
def Layout(dObj, topLeftPos, canvas):
    if dObj.style.method != Draw_Method.HIDDEN:
        # configure things that will be returned
        padding = dObj.style.padding
        minSize = dObj.style.minSize
        offset = dObj.style.offset
        maxObjWidth = minSize[0]
        maxObjHeight = minSize[1]
        dObj.topLeft = wx.Point(topLeftPos[0],topLeftPos[1])
        childPos = [topLeftPos[0] + offset[0], topLeftPos[1] - offset[1]]
        childTypes = model_dict[dObj.kind].children
        for childType in childTypes:
            children = dObj.getChildrenByKind(childType)
            maxWidth = 0
            for child in children:
                w,h = Layout(child,childPos,canvas)
                childPos[1] -= (padding[1] + h)
                maxWidth = max(w,maxWidth)
            maxObjHeight = max(maxObjHeight,abs(childPos[1] - topLeftPos[1]))
            maxObjWidth += maxWidth
            childPos = [childPos[0] + padding[0] + maxWidth,topLeftPos[1] - offset[1]]
        dObj.width = maxObjWidth
        dObj.height = maxObjHeight
        dObj.textPosition = getTextPos(
            option = dObj.style.textPlacement,
            txtString = dObj.properties["name"],
            objPos = dObj.topLeft.Get(),
            objSize = (dObj.width,dObj.height),
            font = dObj.style.font
        )
        maxObjWidth, maxObjHeight = getWidthWithText(
            objSize = (dObj.width,dObj.height),
            style = dObj.style,
            objName = dObj.properties["name"],
            canvas = canvas
        )
        return maxObjWidth,maxObjHeight
    else:
        return 0,0

def Configure(dObj,styleDict):
    dObj.style.Copy(styleDict[dObj.kind])
    if dObj.children != []:
        for child in dObj.children:
            Configure(child,styleDict)
    else:
        pass    
