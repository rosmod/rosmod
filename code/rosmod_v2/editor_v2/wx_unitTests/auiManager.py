#!/usr/bin/python
import wx
import wx.aui

class MyFrame(wx.Frame):
    def __init__(self, *args, **kwargs):
    	wx.Frame.__init__(self, *args, **kwargs)

    	self.mgr = wx.aui.AuiManager(self)

    	leftpanel = wx.Panel(self, -1, size = (200, 150))
    	rightpanel = wx.Panel(self, -1, size = (200, 150))
    	bottompanel = wx.Panel(self, -1, size = (200, 150))

    	self.mgr.AddPane(leftpanel, wx.aui.AuiPaneInfo().Bottom())
    	self.mgr.AddPane(rightpanel, wx.aui.AuiPaneInfo().Left().Layer(1))
    	self.mgr.AddPane(bottompanel, wx.aui.AuiPaneInfo().Center().Layer(2))

    	self.mgr.Update()


class MyApp(wx.App):
    def OnInit(self):
    	frame = MyFrame(None, -1, '07_wxaui.py')
    	frame.Show()
    	self.SetTopWindow(frame)
    	return 1

if __name__ == "__main__":
    app = MyApp(0)
    app.MainLoop()
