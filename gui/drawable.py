#!/usr/bin/python

import wx
import math
from collections import OrderedDict
import copy

# need float canvas for new style of rendering
from wx.lib.floatcanvas import NavCanvas, FloatCanvas, Resources

from metaModel import model_dict

def drawText(text,pos,style,canvas):
    if style['textPlacement'] != 'NONE':
        posStr = "cc"
        if style['textPlacement'] == 'TOP':
            posStr = "bc"
        elif style['textPlacement'] == 'BOTTOM':
            posStr = "tc"
        elif style['textPlacement'] == 'CENTER':
            posStr = "cc"
        elif style['textPlacement'] == 'LEFT':
            posStr = "cr"
        elif style['textPlacement'] == 'RIGHT':
            posStr = "cl"
        output_text = canvas.AddScaledText(text,
                                           pos,
                                           Size=style['font']['pointSize'],
                                           Position=posStr,
                                           Family=wx.MODERN)
        output_text.Color = style['font']['color']
        output_text.Weight = style['font']['weight']
        return output_text
    else:
        return None

def getWidthWithText(objSize,style,objName,canvas):
    txtPlacement = style['textPlacement']
    txt = drawText(objName,(0,0),style,canvas)
    txtWidth = 0
    txtHeight = 0
    if txt != None:
        txt.CalcBoundingBox()
        BB = txt.BoundingBox
        txtHeight = abs(BB[0][1] - BB[1][1]) * 1.0#2
        txtWidth = abs(BB[0][0] - BB[1][0]) * 1.0#2
        canvas.RemoveObject(txt)
    width,height = objSize
    if txtPlacement == 'TOP' or txtPlacement == 'BOTTOM':
        width = max(width,txtWidth)
        height += txtHeight
    elif txtPlacement == 'LEFT' or txtPlacement == 'RIGHT':
        width += txtWidth
    elif txtPlacement == 'CENTER':
        width = max(width,txtWidth)
    elif txtPlacement == 'NONE':
        pass
    return width,height

def getTextPos(option,txtString,objPos,objSize,font):
    x = -1
    y = -1
    if option == 'TOP':
        y = objPos[1]
        x = objPos[0] + objSize[0] / 2 
    elif option == 'BOTTOM':
        y = objPos[1] - objSize[1]
        x = objPos[0] + objSize[0] / 2 
    elif option == 'LEFT':
        y = objPos[1] - objSize[1] / 2 
        x = objPos[0] 
    elif option == 'RIGHT':
        y = objPos[1] - objSize[1] / 2 
        x = objPos[0] + objSize[0]
    elif option == 'CENTER':
        y = objPos[1] - objSize[1] / 2
        x = objPos[0] + objSize[0] / 2 
    elif option == 'NONE':
        pass
    txtPos = wx.Point(x,y)
    return txtPos

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
        self.style = OrderedDict()

        # these are calculated by the Layout() function
        self.topLeft = wx.Point()
        self.height = 0
        self.width = 0
        self.textPosition = wx.Point()

    def Copy(self,other):
        #self.parent = other.parent
        #self.kind = other.kind
        self.properties = copy.deepcopy(other.properties)
        self.children = copy.deepcopy(other.children)
        self.style = copy.copy(other.style)
        self.topLeft = copy.deepcopy(other.topLeft)
        self.height = copy.copy(other.height)
        self.width = copy.copy(other.width)
        self.textPosition = copy.deepcopy(other.textPosition)
        
    def add(self, child):
        child.parent = self
        self.children.append(child)
        
    def delete(self):
        self.parent.children = [x for x in self.parent.children if x != self]

    def deleteAllRefs(self,project):
        for child in self.children:
            child.deleteAllRefs(project)
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
    def Draw(self, iconDict, canvas, leftClickFunc, rightClickFunc, leftDClickFunc):
        if self.style['method'] == 'HIDDEN':
            return
        x,y = self.topLeft.Get()
        dObj = None
        if self.style['method'] == 'ICON':
            if iconDict[self.kind] != None:
                dObj = canvas.AddScaledBitmap(
                    iconDict[self.kind],
                    XY = (x,y),
                    Height = self.height,
                    Position = "tl")
        elif self.style['method'] == 'RECT' or\
             self.style['method'] == 'ROUND_RECT':
            dObj = canvas.AddRectangle(
                (x,y-self.height),
                (self.width,self.height),
                FillColor = self.style['overlay']["fillColor"],
                InForeground = False
            )
        else:
            pass
        if dObj != None:
            dObj.HitFill = True
            dObj.Name = self
            dObj.Bind(FloatCanvas.EVT_FC_RIGHT_UP, rightClickFunc)
            dObj.Bind(FloatCanvas.EVT_FC_LEFT_UP, leftClickFunc)
            dObj.Bind(FloatCanvas.EVT_FC_LEFT_DCLICK, leftDClickFunc)
        if 'outlineColor' in self.style['overlay'].keys():
            rect = canvas.AddRectangle(
                (x,y-self.height),
                (self.width,self.height),
                LineColor = self.style['overlay']['outlineColor'],
                LineWidth = 2,
                InForeground = False,
                FillStyle = 'Transparent'
            )
        if self.textPosition != None and "name" in self.properties.keys():
            input_text = self.style['font']['prefix'] + self.properties["name"]
            drawText(input_text, 
                     self.textPosition.Get(),
                     self.style,
                     canvas)
        canvas.AddPoint(self.topLeft.Get())
        canvas.AddPoint(self.textPosition.Get())
        for child in self.children:
            child.Draw(iconDict,canvas,leftClickFunc,rightClickFunc,leftDClickFunc)

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
    if dObj.style['method'] != 'HIDDEN':
        # configure things that will be returned
        padding = dObj.style['padding']
        minSize = dObj.style['minSize']
        offset = dObj.style['offset']
        maxObjWidth = minSize[0]
        maxObjHeight = minSize[1]
        dObj.topLeft = wx.Point(topLeftPos[0],topLeftPos[1])
        childPos = [topLeftPos[0] + offset[0], topLeftPos[1] - offset[1]]
        childTypes = model_dict[dObj.kind].children
        maxWidth = 0
        if dObj.style['childLayout'] == 'COLUMNS':
            for childType in childTypes:
                children = dObj.getChildrenByKind(childType)
                maxWidth = 0
                for child in children:
                    w,h = Layout(child,childPos,canvas)
                    childPos[1] -= (padding[1] + h)
                    maxWidth = max(w,maxWidth)
                for child in children:
                    child.width = maxWidth
                    child.textPosition = getTextPos(
                        option = child.style['textPlacement'],
                        txtString = child.style['font']['prefix'] + child.properties["name"],
                        objPos = child.topLeft.Get(),
                        objSize = (child.width,child.height),
                        font = child.style['font']
                    )
                maxObjHeight = max(maxObjHeight,abs(childPos[1] - topLeftPos[1]))
                maxObjWidth += maxWidth
                childPos = [childPos[0] + padding[0] + maxWidth,topLeftPos[1] - offset[1]]
        elif dObj.style['childLayout'] == 'STACK':
            maxWidth = 0
            for child in dObj.children:
                w,h = Layout(child,childPos,canvas)
                w -= padding[0]
                childPos[1] -= (padding[1] + h)
                maxWidth = max(w,maxWidth)
            #for child in dObj.children:
            #    child.width = maxWidth
            maxObjHeight = max(maxObjHeight,abs(childPos[1] - topLeftPos[1]))
            maxObjWidth += maxWidth
        elif dObj.style['childLayout'] == 'SQUARE':
            sideLen = int(math.sqrt(len(dObj.children)))
            maxWidth = 0
            numDone = 0
            startInd = 0
            while numDone < len(dObj.children):
                obj = dObj.children[numDone]
                w,h = Layout(obj,childPos,canvas)
                childPos[1] -= (padding[1] + h)
                maxWidth = max(w,maxWidth)
                numDone += 1
                if (numDone % sideLen) == 0:
                    for child in dObj.children[startInd:numDone]:
                        child.width = maxWidth
                        child.textPosition = getTextPos(
                            option = child.style['textPlacement'],
                            txtString = child.style['font']['prefix'] + child.properties["name"],
                            objPos = child.topLeft.Get(),
                            objSize = (child.width,child.height),
                            font = child.style['font']
                        )
                    startInd = numDone
                    maxObjHeight = max(maxObjHeight,abs(childPos[1] - topLeftPos[1]))
                    maxObjWidth += maxWidth
                    childPos = [childPos[0] + padding[0] + maxWidth,topLeftPos[1] - offset[1]]
                    maxWidth = 0
            for child in dObj.children[startInd:numDone]:
                child.width = maxWidth
                child.textPosition = getTextPos(
                    option = child.style['textPlacement'],
                    txtString = child.style['font']['prefix'] + child.properties["name"],
                    objPos = child.topLeft.Get(),
                    objSize = (child.width,child.height),
                    font = child.style['font']
                )
        elif dObj.style['childLayout'] == 'LINE':
            pass # unimplemented
        elif dObj.style['childLayout'] == 'ROWS':
            pass # unimplemented
        maxObjHeight = max(maxObjHeight,abs(childPos[1] - topLeftPos[1]))
        maxObjWidth = max(maxObjWidth, maxWidth)

        dObj.width = maxObjWidth
        dObj.height = maxObjHeight
        dObj.textPosition = getTextPos(
            option = dObj.style['textPlacement'],
            txtString = dObj.style['font']['prefix'] + dObj.properties["name"],
            objPos = dObj.topLeft.Get(),
            objSize = (dObj.width,dObj.height),
            font = dObj.style['font']
        )
        maxObjWidth, maxObjHeight = getWidthWithText(
            objSize = (dObj.width,dObj.height),
            style = dObj.style,
            objName = dObj.style['font']['prefix'] + dObj.properties["name"],
            canvas = canvas
        )
        return maxObjWidth,maxObjHeight
    else:
        return 0,0

def Configure(dObj,styleDict):
    dObj.style = copy.deepcopy(styleDict[dObj.kind])
    if dObj.children != []:
        for child in dObj.children:
            Configure(child,styleDict)
    else:
        pass    
