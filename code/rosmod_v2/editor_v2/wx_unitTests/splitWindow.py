#!/usr/bin/python

import wx
from proportionalSplitter import ProportionalSplitter

class MainFrame(wx.Frame):

    def __init__(self,parent,id,title,position,size):
        wx.Frame.__init__(self, parent, id, title, position, size)

        ## example code that would be in your window's init handler:

        ## create splitters:
        self.split1 = ProportionalSplitter(self,-1, 0.66)
        self.split2 = ProportionalSplitter(self.split1,-1, 0.50)

        #self.split1 = wx.SplitterWindow(self)
        #self.split2 = wx.SplitterWindow(self.split1)
        ## create controls to go in the splitter windows...
        self.rightpanel = wx.Panel (self.split1)
        self.rightpanel.SetBackgroundColour('cyan')
        self.topleftpanel = wx.Panel (self.split2)
        self.topleftpanel.SetBackgroundColour('pink')
        self.bottomleftpanel = wx.Panel (self.split2)
        self.bottomleftpanel.SetBackgroundColour('sky Blue')

        ## add your controls to the splitters:
        self.split1.SplitVertically(self.split2, self.rightpanel)
        self.split2.SplitHorizontally(self.topleftpanel, self.bottomleftpanel)

app = wx.App(0)
win = MainFrame(None, -1, "Hello!", (50, 50), (600, 400))
win.Show()
app.MainLoop()
