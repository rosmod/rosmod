#!/usr/bin/python

# this code: http://zetcode.com/wxpython/menustoolbars/

import wx

from collections import OrderedDict


class ContextMenu(wx.Menu):
    
    def __init__(self, parent, functionsDict):
        super(ContextMenu, self).__init__()
        
        self.parent = parent

        for name,func in functionsDict.iteritems():
            mi = wx.MenuItem(self, wx.NewId(), name)
            self.AppendItem(mi)
            self.Bind(wx.EVT_MENU, func, mi)
        

class Example(wx.Frame):
    
    def __init__(self, *args, **kwargs):
        super(Example, self).__init__(*args, **kwargs) 
            
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
        
def main():
    
    ex = wx.App()
    Example(None)
    ex.MainLoop()    


if __name__ == '__main__':
    main()
