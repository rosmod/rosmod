#!/usr/bin/python

# this code: http://zetcode.com/wxpython/menustoolbars/

import wx
import os

import tabbed_terminal

import aspect

class Example(wx.Frame):
    def __init__(self, *args, **kwargs):
        super(Example, self).__init__(*args,**kwargs)
        self.InitUI()
    
    def InitUI(self):

        self.fileTypes = "*.rosml"

        self.count = 5 # for undo/redo calcs
        
        # build the MenuBar,Toolbar, and Statusbar
        self.BuildMenu()
        self.BuildToolbar()
        self.BuildStatusbar()

        # build the main frame (holds viewer in the top and the output in the bottom)

        self.BuildAspects()
        
        # build the output
        #self.outputPanel = wx.Panel(self)
        #self.output = tabbed_terminal.Tabbed_Terminal(self.outputPanel)
        #self.opSizer = wx.BoxSizer(wx.VERTICAL)
        #self.opSizer.Add(self.output, 1, wx.ALL|wx.EXPAND, 5)
        #self.outputPanel.SetSizer(self.opSizer)
        
        self.Layout()
        
        self.SetSize((800, 600))
        self.SetTitle('Submenu')
        self.Centre()
        self.Show(True)

    def BuildMenu(self):
        self.menubar = wx.MenuBar()

        # normal file operations menu
        self.fileMenu = wx.Menu()
        self.newMI = self.fileMenu.Append(wx.ID_NEW, '&New', 'New ROSML Model')
        self.openMI = self.fileMenu.Append(wx.ID_OPEN, '&Open', 'Open existing ROSML Model')
        self.saveMI = self.fileMenu.Append(wx.ID_SAVE, '&Save', 'Save current ROSML Model')
        self.fileMenu.AppendSeparator()
        self.quitMI = wx.MenuItem(self.fileMenu, wx.ID_EXIT, '&Quit\tCtrl+W', 'Quit ROSMOD')
        self.fileMenu.AppendItem(self.quitMI)

        # aspects (of the viewer) menu for ROSMOD: packages, hardware, deployment
        self.aspectsMenu = wx.Menu()
        self.packageAMI = self.aspectsMenu.Append(wx.ID_ANY,
                                                  "Packages",
                                                  "View/Edit the packages in the model.")
        self.hardwareAMI = self.aspectsMenu.Append(wx.ID_ANY,
                                                   "Hardware",
                                                   "View/Edit the hardware in the model.")
        self.deploymentAMI = self.aspectsMenu.Append(wx.ID_ANY,
                                                     "Deployment",
                                                     "View/Manage the deployment of the model.")

        # tools menu for ROSMOD: generate code, analyze network and timing
        self.toolMenu = wx.Menu()
        self.generateMI = self.toolMenu.Append(wx.ID_ANY, 
                                          "Generate ROS Code\tCtrl+G", 
                                          "Generate ROS application code and workspace.")
        self.networkQoSMI = self.toolMenu.Append(wx.ID_ANY, 
                                            "Analyze Network", 
                                            "Analyze application and system network resource utilization.")
        self.blTimingMI = self.toolMenu.Append(wx.ID_ANY, 
                                          "Analyze Timing", 
                                          "Generate CPN Tokens and Analyze Business Logic Model.")

        # view menu: show/hide statusbar/toolbar/viewer/output
        self.viewMenu = wx.Menu()
        self.shst = self.viewMenu.Append(wx.ID_ANY, 'Show Statusbar', 'Show Statusbar', kind=wx.ITEM_CHECK)
        self.shtl = self.viewMenu.Append(wx.ID_ANY, 'Show Toolbar', 'Show Toolbar', kind=wx.ITEM_CHECK)
        self.shvw = self.viewMenu.Append(wx.ID_ANY, 'Show Viewer', 'Show Viewer', kind=wx.ITEM_CHECK)
        self.shop = self.viewMenu.Append(wx.ID_ANY, 'Show Output', 'Show Output', kind=wx.ITEM_CHECK)
        self.viewMenu.Check(self.shst.GetId(), True)
        self.viewMenu.Check(self.shtl.GetId(), True)
        self.viewMenu.Check(self.shvw.GetId(), True)
        self.viewMenu.Check(self.shop.GetId(), True)

        # add the menus to the menubar
        self.menubar.Append(self.fileMenu, '&File')
        self.menubar.Append(self.viewMenu, '&View')
        self.menubar.Append(self.aspectsMenu, '&Aspects')
        self.menubar.Append(self.toolMenu, '&Tools')
        self.SetMenuBar(self.menubar)
        
        # set up the events for the items in the menubar
        self.RegisterMenuEvents()

    def RegisterMenuEvents(self):
        # file menu
        self.Bind(wx.EVT_MENU, self.OnNew, self.newMI)
        self.Bind(wx.EVT_MENU, self.OnOpen, self.openMI)
        self.Bind(wx.EVT_MENU, self.OnSave, self.saveMI)
        self.Bind(wx.EVT_MENU, self.OnQuit, self.quitMI)
        # aspect menu
        self.Bind(wx.EVT_MENU, self.OnPackageAspect, self.packageAMI)
        self.Bind(wx.EVT_MENU, self.OnHardwareAspect, self.hardwareAMI)
        self.Bind(wx.EVT_MENU, self.OnDeploymentAspect, self.deploymentAMI)
        # tools menu
        self.Bind(wx.EVT_MENU, self.GenerateCode, self.generateMI)
        self.Bind(wx.EVT_MENU, self.AnalyzeNetwork, self.networkQoSMI)
        self.Bind(wx.EVT_MENU, self.AnalyzeTiming, self.blTimingMI)
        # view menu
        self.Bind(wx.EVT_MENU, self.ToggleStatusBar, self.shst)
        self.Bind(wx.EVT_MENU, self.ToggleToolBar, self.shtl)

    def BuildToolbar(self):
        self.toolbar = self.CreateToolBar()
        # file operations
        self.tnew = self.toolbar.AddLabelTool(wx.ID_ANY, 'New', wx.Bitmap('tnew.png'), shortHelp="New")
        self.topen = self.toolbar.AddLabelTool(wx.ID_ANY, '', wx.Bitmap('topen.png'), shortHelp="Open")
        self.tsave = self.toolbar.AddLabelTool(wx.ID_ANY, '', wx.Bitmap('tsave.png'), shortHelp="Save")
        self.toolbar.AddSeparator()
        # undo/redo
        self.tundo = self.toolbar.AddLabelTool(wx.ID_UNDO, '', wx.Bitmap('tundo.png'), shortHelp="Undo")
        self.tredo = self.toolbar.AddLabelTool(wx.ID_REDO, '', wx.Bitmap('tredo.png'), shortHelp="Redo")
        self.toolbar.EnableTool(wx.ID_REDO, False)
        self.toolbar.AddSeparator()
        # application exit
        self.tcreate = self.toolbar.AddLabelTool(wx.ID_ANY, '', wx.Bitmap('tnew.png'), shortHelp="New Package")
        self.tdelete = self.toolbar.AddLabelTool(wx.ID_ANY, '', wx.Bitmap('texit.png'), shortHelp="Remove Package")
        self.toolbar.AddSeparator()
        # application exit
        self.texit = self.toolbar.AddLabelTool(wx.ID_EXIT, '', wx.Bitmap('texit.png'), shortHelp="Exit")
        self.toolbar.Realize()
        # register the events
        self.RegisterToolbarEvents()

    def RegisterToolbarEvents(self):
        self.Bind(wx.EVT_TOOL, self.OnNew, self.tnew)
        self.Bind(wx.EVT_TOOL, self.OnOpen, self.topen)
        self.Bind(wx.EVT_TOOL, self.OnSave, self.tsave)

        self.Bind(wx.EVT_TOOL, self.OnUndo, self.tundo)
        self.Bind(wx.EVT_TOOL, self.OnRedo, self.tredo)

        self.Bind(wx.EVT_TOOL, self.OnCreate, self.tcreate)
        self.Bind(wx.EVT_TOOL, self.OnDelete, self.tdelete)

        self.Bind(wx.EVT_TOOL, self.OnQuit, self.texit)

    def BuildStatusbar(self):
        self.statusbar = self.CreateStatusBar()
        self.statusbar.SetStatusText('Ready')

    def BuildAspects(self):
        # build the aspect Panel
        self.aspectPanel = wx.Panel(self)
        # build the aspects
        # package aspect
        self.PackageAspect = aspect.Aspect(self.aspectPanel)
        self.PackageAspect.AddPage(aspect.AspectTab(self.PackageAspect), "Package 1")
        self.PackageAspect.AddPage(aspect.AspectTab(self.PackageAspect), "Package 2")
        self.PackageAspect.AddPage(aspect.AspectTab(self.PackageAspect), "Package 3")
        self.PackageAspect.AddPage(aspect.AspectTab(self.PackageAspect), "All Packages")
        # hardware aspect
        self.HardwareAspect = aspect.Aspect(self.aspectPanel)
        self.HardwareAspect.AddPage(aspect.AspectTab(self.HardwareAspect), "System Hardware")
        self.HardwareAspect.Hide()
        # deployment aspect
        self.DeploymentAspect = aspect.Aspect(self.aspectPanel)
        self.DeploymentAspect.AddPage(aspect.AspectTab(self.DeploymentAspect), "Package 1")
        self.DeploymentAspect.AddPage(aspect.AspectTab(self.DeploymentAspect), "Package 2")
        self.DeploymentAspect.AddPage(aspect.AspectTab(self.DeploymentAspect), "Package 3")
        self.DeploymentAspect.AddPage(aspect.AspectTab(self.DeploymentAspect), "All Packages")
        self.DeploymentAspect.Hide()

        self.apSizer = wx.BoxSizer(wx.VERTICAL)
        self.apSizer.Add(self.PackageAspect, 1, wx.ALL|wx.EXPAND, 5)
        self.aspectPanel.SetSizer(self.apSizer)
        
    def OnQuit(self, e):
        self.Close()

    def OnNew(self, e):
        pass

    def OnOpen(self, e):
        """ Open a file"""
        self.dirname = ''
        dlg = wx.FileDialog(self, "Choose a file", self.dirname, "", self.fileTypes, wx.OPEN)
        if dlg.ShowModal() == wx.ID_OK:
            self.filename = dlg.GetFilename()
            self.dirname = dlg.GetDirectory()
            f = open(os.path.join(self.dirname, self.filename), 'r')
            #self.control.SetValue(f.read())
            f.close()
            self.statusbar.SetStatusText('Loaded {} from {}'.format(self.filename,self.dirname))
        dlg.Destroy()

    def OnSave(self, e):
        pass

    def OnUndo(self, e):
        if self.count > 1 and self.count <= 5:
            self.count = self.count - 1
        if self.count == 1:
            self.toolbar.EnableTool(wx.ID_UNDO, False)
        if self.count == 4:
            self.toolbar.EnableTool(wx.ID_REDO, True)

    def OnRedo(self, e):
        if self.count < 5 and self.count >= 1:
            self.count = self.count + 1
        if self.count == 5:
            self.toolbar.EnableTool(wx.ID_REDO, False)
        if self.count == 2:
            self.toolbar.EnableTool(wx.ID_UNDO, True)

    def OnCreate(self, e):
        newTab = aspect.AspectTab(self.PackageAspect)
        newTabName = "New Package!"
        numPages = self.PackageAspect.GetPageCount()
        self.PackageAspect.InsertPage(numPages-1,newTab, newTabName)
        self.PackageAspect.SetSelection(numPages-1)
    
    def OnDelete(self, e):
        selectedPage = self.PackageAspect.GetSelection()
        numPages = self.PackageAspect.GetPageCount()
        if selectedPage != numPages - 1:
            self.PackageAspect.DeletePage(selectedPage)
        else:
            pass # ERROR DIALOG HERE; can we bake this into the aspect?
        pass

    def HideAllAspects(self):
        self.PackageAspect.Hide()
        self.HardwareAspect.Hide()
        self.DeploymentAspect.Hide()
        self.apSizer.Clear()
        self.apSizer.Layout()
        self.toolbar.EnableTool(self.tcreate.GetId(), False)
        self.toolbar.EnableTool(self.tdelete.GetId(), False)

    def ShowAspect(self,aspect):
        self.apSizer.Add(aspect, 1, wx.ALL|wx.EXPAND, 5)
        aspect.Show()
        self.apSizer.Layout()

    def OnPackageAspect(self, e):
        self.HideAllAspects()
        self.ShowAspect(self.PackageAspect)
        self.toolbar.EnableTool(self.tcreate.GetId(), True)
        self.toolbar.EnableTool(self.tdelete.GetId(), True)

    def OnHardwareAspect(self, e):
        self.HideAllAspects()
        self.ShowAspect(self.HardwareAspect)

    def OnDeploymentAspect(self, e):
        self.HideAllAspects()
        self.ShowAspect(self.DeploymentAspect)
        
    def ToggleStatusBar(self, e):
        self.GetStatusBar().Show(e.IsChecked())

    def ToggleToolBar(self, e):
        self.GetToolBar().Show(e.IsChecked())
        self.SendSizeEvent()

    def GenerateCode(self, e):
        dlg = wx.MessageDialog( self, "Generate ROS Code", "ROSMOD Generator", wx.OK)
        dlg.ShowModal() # Show it, modal means user can't do anything with base app until this is closed
        dlg.Destroy() # finally destroy it when finished.
        pass
        
    def AnalyzeNetwork(self, e):
        pass

    def AnalyzeTiming(self, e):
        pass

def main():
    
    ex = wx.App()
    Example(None)
    ex.MainLoop()    


if __name__ == '__main__':
    main()
