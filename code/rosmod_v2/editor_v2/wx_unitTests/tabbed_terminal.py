#!/usr/bin/env python
import wx
import wx.lib.agw.flatnotebook as fnb
from terminal import *

class Tabbed_Terminal(fnb.FlatNotebook):

    def __init__(self, parent):
        fnb.FlatNotebook.__init__(self, parent, id=wx.ID_ANY)
 
        # Create the first tab and add it to the notebook
        tabOne = TermEmulatorDemo(self)
        self.AddPage(tabOne, "Terminal")
 
        self.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGED, self.OnPageChanged)
        self.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGING, self.OnPageChanging)
 
    def OnPageChanged(self, event):
        old = event.GetOldSelection()
        new = event.GetSelection()
        sel = self.GetSelection()
        event.Skip()
 
    def OnPageChanging(self, event):
        old = event.GetOldSelection()
        new = event.GetSelection()
        sel = self.GetSelection()
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
