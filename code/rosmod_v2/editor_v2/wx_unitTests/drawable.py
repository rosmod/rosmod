#!/usr/bin/python

import wx
from collections import OrderedDict

class Text_Placement:
    TOP, BOTTOM, LEFT, RIGHT, CENTER = range(5)

class Draw_Method:
    RECT, ROUND_RECT, ICON = range(3)

class Draw_Style:
    def __init__(self):
        self.icon = None
        self.fontSize = 10
        self.method = Draw_Method.ICON
        self.textPlacement = Text_Placement.TOP

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
        self.center = wx.Point()
        self.height = 0
        self.width = 0
        self.textCenter = wx.Point()
        
    def add(self, child):
        self.children.append(child)
        
    def delete(self, reference):
        self.children = [child for child in self.children if child != reference]

    '''
    Draw() is called after layout has been calculated
    '''
    def Draw(self):
        pass

'''
The Layout function takes a top-level
drawable object as input, and configures
it recursively by going through it's children
to determine the parent's final size.

The Children are laid out inside the parent according 
to some layout options
'''
def Layout(dObj):
    pass
