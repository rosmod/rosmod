#!/usr/bin/python

# this code: http://zetcode.com/wxpython/menustoolbars/

import wx

import tabbed_terminal

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
        toolMenu.Append(wx.ID_ANY, "Generate ROS Code", "Generate ROS application code and workspace.")
        toolMenu.Append(wx.ID_ANY, "Analyze Network", "Analyze application and system network resource utilization.")
        toolMenu.Append(wx.ID_ANY, "Analyze Timing", "Generate CPN Tokens and Analyze Business Logic Model.")
        
        # useful for nested menus
        #fileMenu.AppendMenu(wx.ID_ANY, '&Tools', toolMenu)

        viewMenu = wx.Menu()
        self.shst = viewMenu.Append(wx.ID_ANY, 'Show Statusbar', 'Show Statusbar', kind=wx.ITEM_CHECK)
        self.shtl = viewMenu.Append(wx.ID_ANY, 'Show Toolbar', 'Show Toolbar', kind=wx.ITEM_CHECK)
        self.shvw = viewMenu.Append(wx.ID_ANY, 'Show Viewer', 'Show Viewer', kind=wx.ITEM_CHECK)
        self.shop = viewMenu.Append(wx.ID_ANY, 'Show Output', 'Show Output', kind=wx.ITEM_CHECK)

        viewMenu.Check(self.shst.GetId(), True)
        viewMenu.Check(self.shtl.GetId(), True)
        viewMenu.Check(self.shvw.GetId(), True)
        viewMenu.Check(self.shop.GetId(), True)

        self.Bind(wx.EVT_MENU, self.ToggleStatusBar, self.shst)
        self.Bind(wx.EVT_MENU, self.ToggleToolBar, self.shtl)

        menubar.Append(fileMenu, '&File')
        menubar.Append(viewMenu, '&View')
        menubar.Append(toolMenu, '&Tools')
        self.SetMenuBar(menubar)

        self.toolbar = self.CreateToolBar()
        self.toolbar.AddLabelTool(1,'',wx.Bitmap('texit.png'))
        self.toolbar.Realize()

        self.statusbar = self.CreateStatusBar()
        self.statusbar.SetStatusText('Ready')

        self.panel = wx.Panel(self)

        notebook = tabbed_terminal.Tabbed_Terminal(self.panel)
        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(notebook, 1, wx.ALL|wx.EXPAND, 5)
        self.panel.SetSizer(sizer)
        self.Layout()
        
        self.SetSize((800, 600))
        self.SetTitle('Submenu')
        self.Centre()
        self.Show(True)
        
    def OnQuit(self, e):
        self.Close()
        
    def ToggleStatusBar(self, e):
        self.GetStatusBar().Show(e.IsChecked())

    def ToggleToolBar(self, e):
        self.GetToolBar().Show(e.IsChecked())
        self.SendSizeEvent()

def main():
    
    ex = wx.App()
    Example(None)
    ex.MainLoop()    


if __name__ == '__main__':
    main()
