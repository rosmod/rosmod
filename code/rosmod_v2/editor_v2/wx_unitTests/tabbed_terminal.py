#!/usr/bin/env python
import wx
from terminal import *

class TabPanel(wx.Panel):
    def __init__(self, parent):
 
        wx.Panel.__init__(self, parent=parent, id=wx.ID_ANY)
 
        sizer = wx.BoxSizer(wx.VERTICAL)
        txtOne = wx.TextCtrl(self, wx.ID_ANY, "")
        txtTwo = wx.TextCtrl(self, wx.ID_ANY, "")
 
        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(txtOne, 0, wx.ALL, 5)
        sizer.Add(txtTwo, 0, wx.ALL, 5)
 
        self.SetSizer(sizer)

class Tabbed_Terminal(wx.Notebook):

    def __init__(self, parent):
        wx.Notebook.__init__(self, parent, id=wx.ID_ANY, style=
                             wx.BK_DEFAULT
                             #wx.BK_TOP 
                             #wx.BK_BOTTOM
                             #wx.BK_LEFT
                             #wx.BK_RIGHT
                             )
 
        # Create the first tab and add it to the notebook
        tabOne = TermEmulatorDemo(self)
        # tabOne.SetBackgroundColour("Black")
        self.AddPage(tabOne, "Terminal")
 
        # Create and add the second tab
        tabTwo = TabPanel(self)
        self.AddPage(tabTwo, "TabTwo")
 
        # Create and add the third tab
        self.AddPage(TabPanel(self), "TabThree")
 
        self.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGED, self.OnPageChanged)
        self.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGING, self.OnPageChanging)
 
    def OnPageChanged(self, event):
        old = event.GetOldSelection()
        new = event.GetSelection()
        sel = self.GetSelection()
        # print 'OnPageChanged,  old:%d, new:%d, sel:%d\n' % (old, new, sel)
        event.Skip()
 
    def OnPageChanging(self, event):
        old = event.GetOldSelection()
        new = event.GetSelection()
        sel = self.GetSelection()
        # print 'OnPageChanging, old:%d, new:%d, sel:%d\n' % (old, new, sel)
        event.Skip()
    
class ROSMOD_Console(wx.Frame):

    def __init__(self):

        wx.Frame.__init__(self, None, wx.ID_ANY,
                          "ROSMOD Console",
                          size=(600,400)
                          )
        panel = wx.Panel(self)
 
        notebook = Tabbed_Terminal(panel)
        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(notebook, 1, wx.ALL|wx.EXPAND, 5)
        panel.SetSizer(sizer)
        self.Layout()
 
        self.Show()
 
#----------------------------------------------------------------------
if __name__ == "__main__":

    app = wx.PySimpleApp()
    frame = ROSMOD_Console()
    app.MainLoop()
