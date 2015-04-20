#!/usr/bin/python

# this code: http://zetcode.com/wxpython/menustoolbars/

import wx

class Example(wx.Frame):
    def __init__(self, *args, **kwargs):
        super(Example, self).__init__(*args,**kwargs)
        self.InitUI()
    
    def InitUI(self):
        
        menubar = wx.MenuBar()
        fileMenu = wx.Menu()
        fileMenu.Append(wx.ID_NEW, '&New')
        fileMenu.Append(wx.ID_OPEN, '&Open')
        fileMenu.Append(wx.ID_SAVE, '&Save')
        fileMenu.AppendSeparator()

        qmi = wx.MenuItem(fileMenu, wx.ID_EXIT, '&Quit\tCtrl+W')
        fileMenu.AppendItem(qmi)

        self.Bind(wx.EVT_MENU, self.OnQuit, qmi)

        toolMenu = wx.Menu()
        toolMenu.Append(wx.ID_ANY, "Generate ROS Code")
        toolMenu.Append(wx.ID_ANY, "Analyze Network")
        toolMenu.Append(wx.ID_ANY, "Analyze Timing")
        
        #fileMenu.AppendMenu(wx.ID_ANY, '&Tools', toolMenu)

        viewMenu = wx.Menu()
        self.shst = viewMenu.Append(wx.ID_ANY, 'Show Statusbar', 'Show Statusbar', kind=wx.ITEM_CHECK)
        self.shtl = viewMenu.Append(wx.ID_ANY, 'Show Toolbar', 'Show Toolbar', kind=wx.ITEM_CHECK)
        #self.shvw = viewMenu.Append(wx.ID_ANY, 'Show Viewer', 'Show Viewer', kind=wx.ITEM_CHECK)
        #self.shop = viewMenu.Append(wx.ID_ANY, 'Show Output', 'Show Output', kind=wx.ITEM_CHECK)

        viewMenu.Check(self.shst.GetId(), True)
        viewMenu.Check(self.shtl.GetId(), True)

        self.Bind(wx.EVT_MENU, self.ToggleStatusBar, self.shst)
        self.Bind(wx.EVT_MENU, self.ToggleToolBar, self.shtl)

        menubar.Append(fileMenu, '&File')
        menubar.Append(viewMenu, '&View')
        self.SetMenuBar(menubar)

        self.toolbar = self.CreateToolBar()
        self.toolbar.AddLabelTool(1,'',wx.Bitmap('texit.png'))
        self.toolbar.Realize()

        self.statusbar = self.CreateStatusBar()
        self.statusbar.SetStatusText('Ready')
        
        self.SetSize((300, 200))
        self.SetTitle('Submenu')
        self.Centre()
        self.Show(True)
        
    def OnQuit(self, e):
        self.Close()
        
    def ToggleStatusBar(self, e):        
        if self.shst.IsChecked():
            self.statusbar.Show()
        else:
            self.statusbar.Hide()

    def ToggleToolBar(self, e):        
        if self.shtl.IsChecked():
            self.toolbar.Show()
        else:
            self.toolbar.Hide()  

def main():
    
    ex = wx.App()
    Example(None)
    ex.MainLoop()    


if __name__ == '__main__':
    main()
