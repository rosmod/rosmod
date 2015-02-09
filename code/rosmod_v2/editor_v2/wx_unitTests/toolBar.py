#!/usr/bin/python

# this code: http://zetcode.com/wxpython/menustoolbars/

import wx

class Example(wx.Frame):
    
    def __init__(self, *args, **kwargs):
        super(Example, self).__init__(*args, **kwargs) 
            
        self.InitUI()
        
    def InitUI(self):    

        vbox = wx.BoxSizer(wx.VERTICAL)

        self.count = 5

        self.toolbar1 = wx.ToolBar(self)
        self.toolbar1.AddLabelTool(wx.ID_ANY, '', wx.Bitmap('tnew.gif'))
        self.toolbar1.AddLabelTool(wx.ID_ANY, '', wx.Bitmap('topen.png'))
        self.toolbar1.AddLabelTool(wx.ID_ANY, '', wx.Bitmap('tsave.png'))
        self.toolbar1.AddSeparator()

        tundo = self.toolbar1.AddLabelTool(wx.ID_UNDO, '', wx.Bitmap('tundo.png'))
        tredo = self.toolbar1.AddLabelTool(wx.ID_REDO, '', wx.Bitmap('tredo.png'))
        self.toolbar1.EnableTool(wx.ID_REDO, False)

        self.toolbar1.AddSeparator()
        texit = self.toolbar1.AddLabelTool(wx.ID_EXIT, '', wx.Bitmap('texit.png'))
        self.toolbar1.Realize()

        self.toolbar2 = wx.ToolBar(self)
        qtool = self.toolbar2.AddLabelTool(wx.ID_EXIT, '', wx.Bitmap('texit.png'))
        self.toolbar2.Realize()

        self.Bind(wx.EVT_TOOL, self.OnQuit, texit)
        self.Bind(wx.EVT_TOOL, self.OnUndo, tundo)
        self.Bind(wx.EVT_TOOL, self.OnRedo, tredo)

        vbox.Add(self.toolbar1, 0, wx.EXPAND)
        vbox.Add(self.toolbar2, 0, wx.EXPAND)

        self.Bind(wx.EVT_TOOL, self.OnQuit, qtool)
        
        self.SetSizer(vbox)

        self.SetSize((250, 200))
        self.SetTitle('Simple toolbar')
        self.Centre()
        self.Show(True)
        
    def OnQuit(self, e):
        self.Close()

    def OnUndo(self, e):
        if self.count > 1 and self.count <= 5:
            self.count = self.count - 1

        if self.count == 1:
            self.toolbar1.EnableTool(wx.ID_UNDO, False)

        if self.count == 4:
            self.toolbar1.EnableTool(wx.ID_REDO, True)

    def OnRedo(self, e):
        if self.count < 5 and self.count >= 1:
            self.count = self.count + 1

        if self.count == 5:
            self.toolbar1.EnableTool(wx.ID_REDO, False)

        if self.count == 2:
            self.toolbar1.EnableTool(wx.ID_UNDO, True)
        
def main():
    
    ex = wx.App()
    Example(None)
    ex.MainLoop()    


if __name__ == '__main__':
    main()
