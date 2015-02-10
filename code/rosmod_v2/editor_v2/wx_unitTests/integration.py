#!/usr/bin/python

# this code: http://zetcode.com/wxpython/menustoolbars/

import wx
import wx.aui
from proportionalSplitter import ProportionalSplitter
import os

import tabbed_terminal

import aspect

class Example(wx.Frame):
    def __init__(self, *args, **kwargs):
        super(Example, self).__init__(*args,**kwargs)
        self.InitUI()
    
    def InitUI(self):

        self.fileTypes = "*.rml"

        self.count = 5 # for undo/redo calcs
        
        # build the MenuBar,Toolbar, and Statusbar
        self.BuildMenu()
        self.BuildStatusbar()

        # build the main frame (holds viewer in the top and the output in the bottom)
        #self.split1 = ProportionalSplitter(self,wx.ID_NEW,proportion=0.66)
        self.split1 = wx.SplitterWindow(self,wx.ID_NEW,style=wx.SP_PERMIT_UNSPLIT)

        self.BuildAspects()
        self.BuildOutput()

        self.split1.SplitHorizontally(self.activeAspect,self.output)
        
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
                                                  "View/Edit the packages in the model.",
                                                  kind=wx.ITEM_RADIO
                                              )
        self.hardwareAMI = self.aspectsMenu.Append(wx.ID_ANY,
                                                   "Hardware",
                                                   "View/Edit the hardware in the model.",
                                                   kind=wx.ITEM_RADIO
                                               )
        self.deploymentAMI = self.aspectsMenu.Append(wx.ID_ANY,
                                                     "Deployment",
                                                     "View/Manage the deployment of the model.",
                                                     kind=wx.ITEM_RADIO
                                                 )

        # tools menu for ROSMOD: generate code, analyze network and timing
        self.toolMenu = wx.Menu()
        self.generateMI = self.toolMenu.Append(wx.ID_ANY, 
                                               "Generate ROS Code\tCtrl+G", 
                                               "Generate ROS application code and workspace.")
        self.networkQoSMI = self.toolMenu.Append(wx.ID_ANY, 
                                                 "Analyze Network", 
                                                 "Analyze application and system network resource utilization.",)
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
        self.Bind(wx.EVT_MENU, self.ToggleAspectView, self.shvw)
        self.Bind(wx.EVT_MENU, self.ToggleOutputView, self.shop)

    def BuildStatusbar(self):
        self.statusbar = self.CreateStatusBar()
        self.statusbar.SetStatusText('Ready')

    def BuildOutput(self):
        self.output = tabbed_terminal.Tabbed_Terminal(self.split1)
        
    def BuildAspects(self):
        # package aspect
        self.PackageAspect = aspect.Aspect(self.split1)
        # hardware aspect
        self.HardwareAspect = aspect.Aspect(self.split1)
        self.HardwareAspect.Hide()
        # deployment aspect
        self.DeploymentAspect = aspect.Aspect(self.split1)
        self.DeploymentAspect.Hide()

        self.activeAspect = self.PackageAspect
        
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

    def HideAllAspects(self):
        self.PackageAspect.Hide()
        self.HardwareAspect.Hide()
        self.DeploymentAspect.Hide()

    def ShowAspect(self,aspect):
        aspect.Show()
        self.split1.ReplaceWindow(self.activeAspect,aspect)
        self.activeAspect = aspect
        self.viewMenu.Check(self.shtl.GetId(), aspect.toolbar.IsShown())

    def OnPackageAspect(self, e):
        self.HideAllAspects()
        self.ShowAspect(self.PackageAspect)

    def OnHardwareAspect(self, e):
        self.HideAllAspects()
        self.ShowAspect(self.HardwareAspect)

    def OnDeploymentAspect(self, e):
        self.HideAllAspects()
        self.ShowAspect(self.DeploymentAspect)
        
    def ToggleStatusBar(self, e):
        self.GetStatusBar().Show(e.IsChecked())

    def ToggleToolBar(self, e):
        self.activeAspect.toolbar.Show(e.IsChecked())
        self.SendSizeEvent()

    def UpdateMainWindow(self, e):
        self.split1.Show()
        self.split1.SplitHorizontally(self.activeAspect,self.output)
        if self.shvw.IsChecked() and self.shop.IsChecked():
            pass
        elif self.shvw.IsChecked() and not self.shop.IsChecked():
            self.split1.Unsplit(self.output)
        elif not self.shvw.IsChecked() and self.shop.IsChecked():
            self.split1.Unsplit(self.activeAspect)
        else:
            self.split1.Hide()
        self.split1.UpdateSize()

    def ToggleAspectView(self, e):
        self.UpdateMainWindow(e)
    
    def ToggleOutputView(self, e):
        self.UpdateMainWindow(e)

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
