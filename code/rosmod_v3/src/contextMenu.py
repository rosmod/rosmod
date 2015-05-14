#!/usr/bin/python

# this code: http://zetcode.com/wxpython/menustoolbars/

import wx

from collections import OrderedDict

from rosmod import MakeAdd
from metaModel import model_dict

class ContextMenu(wx.Menu):
    
    def __init__(self, parent, functionsDict):
        super(ContextMenu, self).__init__()
        
        self.parent = parent

        for name,func in functionsDict.iteritems():
            mi = wx.MenuItem(self, wx.NewId(), name)
            self.AppendItem(mi)
            self.Bind(wx.EVT_MENU, func, mi)
        

def AspectContextMenu(self, obj):
    cm = OrderedDict()
    cm['Edit'] = self.AspectEdit        # edits the object's properties (name, fields, etc.)
    cm['Delete'] = self.AspectDelete    # deletes the object and all references from the model
    childTypes = model_dict[obj.kind].children
    for childType in childTypes:
        cm['Add ' + childType] = MakeAdd(self,childType)
    for contextFuncName, contextFunc in model_dict[obj.kind].context_funcs:
        cm[contextFuncName] = lambda e: contextFunc(self,obj,e)
    return cm

class ContextMenuExample(wx.Frame):
    
    def __init__(self, *args, **kwargs):
        super(ContextMenuExample, self).__init__(*args, **kwargs) 
            
        self.InitUI()
        
    def InitUI(self):

        self.Bind(wx.EVT_RIGHT_DOWN, self.OnRightDown)

        self.SetSize((250, 200))
        self.SetTitle('Context menu')
        self.Centre()
        self.Show(True)
        
    def OnRightDown(self, e):
        cm = OrderedDict()
        cm['Edit'] = self.OnEdit
        cm['Delete'] = self.OnDelete
        self.PopupMenu(ContextMenu(self,cm), e.GetPosition())

    def OnEdit(self, e):
        print "In OnEdit()!"

    def OnDelete(self, e):
        print "In OnDelete()!"
        
def context_menu_main():
    
    ex = wx.App()
    ContextMenuExample(None)
    ex.MainLoop()    


if __name__ == '__main__':
    context_menu_main()
