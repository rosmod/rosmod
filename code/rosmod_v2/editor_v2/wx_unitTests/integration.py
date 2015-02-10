#!/usr/bin/python

# this code: http://zetcode.com/wxpython/menustoolbars/

import wx
import os
# proportional splitter should work for resizing window
from proportionalSplitter import ProportionalSplitter
# flat notebook allows us to have scroll buttons and a close button
import wx.lib.agw.flatnotebook as fnb
# terminal allows us to have a terminal panel
from terminal import *
# the dialogs that we use (popups)
import dialogs

class Example(wx.Frame):
    def __init__(self, *args, **kwargs):
        super(Example, self).__init__(*args,**kwargs)
        self.InitUI()
    
    def InitUI(self):

        self.fileTypes = "*.rml"

        # build the MenuBar,Toolbar, and Statusbar
        self.BuildMenu()
        self.BuildStatusbar()

        # build the main frame (holds viewer in the top and the output in the bottom)
        #self.split1 = ProportionalSplitter(self,wx.ID_NEW,proportion=0.66)
        self.viewSplitter = wx.SplitterWindow(self,wx.ID_NEW,style=wx.SP_PERMIT_UNSPLIT)

        self.BuildAspects()
        self.BuildOutput()

        self.viewSplitter.SplitHorizontally(self.activeAspect,self.output)
        
        self.Layout()
        
        self.SetSize((800, 600))
        self.SetTitle('Submenu')
        self.Centre()
        self.Show(True)

    '''
    Build the output notebook for ROSMOD which holds:
    * a terminal
    * the program output
    * any logs requested from deployment
    '''
    def BuildOutput(self):
        self.output = fnb.FlatNotebook(self.viewSplitter, wx.ID_ANY)
        self.output.AddPage(TermEmulatorDemo(self.output), "Terminal")
        
    '''
    Build all the Aspects required for ROSMOD:
    * Packages aspect : used for setting up mgs,srv,comp,node,etc.
    * Hardware aspect : used for configure the system hardware (hosts)
    * Deployment aspect : used for assigning nodes to hosts
    '''
    def BuildAspects(self):
        self.BuildPackageAspect()
        self.BuildHardwareAspect()
        self.BuildDeploymentAspect()
        self.activeAspect = self.PackageAspect
    '''
    Package Aspect: panel with toolbar and notebook for managing packages
    '''
    def BuildPackageAspect(self):
        self.PackageAspect = wx.Panel(self.viewSplitter)
        self.BuildPackageAspectNotebook()
        self.BuildPackageAspectToolbar()
        self.packageSizer = wx.BoxSizer(wx.VERTICAL)
        self.packageSizer.Add(self.packageTB, 0, wx.ALL | wx.ALIGN_LEFT | wx.EXPAND, 4)
        self.packageSizer.Add(self.packageNB, 1, wx.ALL | wx.ALIGN_LEFT | wx.EXPAND, 4)
        self.PackageAspect.SetSizer(self.packageSizer)
    def BuildPackageAspectNotebook(self):
        self.packageNB = wx.Notebook(self.PackageAspect, wx.ID_ANY)
        # THIS PART CHANGES BASED ON MODEL, NEED TO REWRITE THIS WITH MODEL INTEGRATION
        self.packageNB.AddPage(wx.Panel(self.packageNB), "Package 1")
        self.packageNB.AddPage(wx.Panel(self.packageNB), "Package 2")
        self.packageNB.AddPage(wx.Panel(self.packageNB), "Package 3")
        self.packageNB.AddPage(wx.Panel(self.packageNB), "All Packages")
        self.packageNB.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGED, self.OnPageChanged)
        self.packageNB.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGING, self.OnPageChanging)
    def BuildPackageAspectToolbar(self):
        self.packageTB = wx.ToolBar(self.PackageAspect)
        # file operations
        self.packageTB_new_ID = wx.NewId()
        self.packageTB_open_ID = wx.NewId()
        self.packageTB_save_ID = wx.NewId()
        self.packageTB_new = self.packageTB.AddLabelTool(self.packageTB_new_ID, 'New', wx.Bitmap('tnew.png'), shortHelp="New")
        self.pacakgeTB_open = self.packageTB.AddLabelTool(self.packageTB_open_ID, '', wx.Bitmap('topen.png'), shortHelp="Open")
        self.packageTB_save = self.packageTB.AddLabelTool(self.packageTB_save_ID, '', wx.Bitmap('tsave.png'), shortHelp="Save")
        self.packageTB.AddSeparator()
        # undo/redo
        self.packageTB_undo_ID = wx.NewId()
        self.packageTB_redo_ID = wx.NewId()
        self.packageTB_undo = self.packageTB.AddLabelTool(self.packageTB_undo_ID, '', wx.Bitmap('tundo.png'), shortHelp="Undo")
        self.packageTB_redo = self.packageTB.AddLabelTool(self.packageTB_redo_ID, '', wx.Bitmap('tredo.png'), shortHelp="Redo")
        self.packageTB.EnableTool(self.packageTB_undo_ID, False)
        self.packageTB.EnableTool(self.packageTB_redo_ID, False)
        self.packageTB.AddSeparator()
        # create / delete packages
        self.packageTB_create_ID = wx.NewId()
        self.packageTB_delete_ID = wx.NewId()
        self.packageTB_create = self.packageTB.AddLabelTool(self.packageTB_create_ID, '', wx.Bitmap('tnew.png'), shortHelp="New Package")
        self.packageTB_delete = self.packageTB.AddLabelTool(self.packageTB_delete_ID, '', wx.Bitmap('texit.png'), shortHelp="Remove Package")

        wx.EVT_TOOL( self.PackageAspect, self.packageTB_new_ID, self.OnNew)
        wx.EVT_TOOL( self.PackageAspect, self.packageTB_open_ID, self.OnOpen)
        wx.EVT_TOOL( self.PackageAspect, self.packageTB_save_ID, self.OnSave)
        wx.EVT_TOOL( self.PackageAspect, self.packageTB_create_ID, self.OnPackageCreate)
        wx.EVT_TOOL( self.PackageAspect, self.packageTB_delete_ID, self.OnPackageDelete)
        wx.EVT_TOOL( self.PackageAspect, self.packageTB_undo_ID, self.OnUndo)
        wx.EVT_TOOL( self.PackageAspect, self.packageTB_redo_ID, self.OnRedo)
    '''
    Hardware Aspect: panel with toolbar for configuring system hardware (hosts)
    '''
    def BuildHardwareAspect(self):
        self.HardwareAspect = wx.Panel(self.viewSplitter)
        self.BuildHardwareAspectToolbar()
        self.HardwareAspect.Hide()
    def BuildHardwareAspectToolbar(self):
        pass
    '''
    Deployment Aspect: panel with toolbar and notebook for configuring and managing
    node deployment onto hosts (and roscore deployment)
    '''        
    def BuildDeploymentAspect(self):
        self.DeploymentAspect = wx.Panel(self.viewSplitter)
        self.BuildDeploymentAspectToolbar()
        self.DeploymentAspect.Hide()
    def BuildDeploymentAspectToolbar(self):
        pass

    '''
    Aspect Menubar Menu functions
    '''
    def HideAllAspects(self):
        self.PackageAspect.Hide()
        self.HardwareAspect.Hide()
        self.DeploymentAspect.Hide()

    def ShowAspect(self,aspect):
        if self.shvw.IsChecked():
            aspect.Show()
            self.viewSplitter.ReplaceWindow(self.activeAspect,aspect)
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
        
    '''
    View Menu Functions
    '''
    def UpdateMainWindow(self, e):
        self.viewSplitter.Show()
        self.viewSplitter.SplitHorizontally(self.activeAspect,self.output)
        if self.shvw.IsChecked() and self.shop.IsChecked():
            pass
        elif self.shvw.IsChecked() and not self.shop.IsChecked():
            self.viewSplitter.Unsplit(self.output)
        elif not self.shvw.IsChecked() and self.shop.IsChecked():
            self.viewSplitter.Unsplit(self.activeAspect)
        else:
            self.viewSplitter.Hide()
        self.viewSplitter.UpdateSize()
    def ToggleAspectView(self, e):
        self.UpdateMainWindow(e)    
    def ToggleOutputView(self, e):
        self.UpdateMainWindow(e)
    def ToggleStatusBar(self, e):
        self.GetStatusBar().Show(e.IsChecked())
    def ToggleToolBar(self, e):
        self.activeAspect.toolbar.Show(e.IsChecked())
        self.SendSizeEvent()

    '''
    Toolbar and File Menubar Menu Functions
    '''
    def OnQuit(self, e):
        if wx.MessageBox("Really quit ROSMOD?", "Confirm",
                         wx.ICON_QUESTION | wx.YES_NO, self) == wx.NO:
            return
        self.Close()

    def OnNew(self, e):
        self.dirname = ''
        self.filename, self.dirname = dialogs.RMLFileDialog(
            frame = self,
            prompt ="Save Model As...", 
            path = self.dirname,
            fileTypes = self.fileTypes, 
            fd_flags = wx.FD_SAVE | wx.FD_OVERWRITE_PROMPT
        )
        self.statusbar.SetStatusText('Created model {} in {}'.format(self.filename,self.dirname))

    def OnOpen(self, e):
        self.dirname = ''
        self.filename, self.dirname = dialogs.RMLFileDialog(
            frame = self,
            fileTypes = self.fileTypes,
            path = self.dirname,
            prompt = "Choose a model",
            fd_flags = wx.FD_OPEN | wx.FD_FILE_MUST_EXIST
        )
        self.statusbar.SetStatusText('Loaded {} from {}'.format(self.filename,self.dirname))

    def OnSave(self, e):
        self.dirname = ''
        self.filename, self.dirname = dialogs.RMLFileDialog(
            frame = self,
            prompt = "Save Model As...",
            path = self.dirname,
            fileTypes = self.fileTypes, 
            fd_flags = wx.FD_SAVE | wx.FD_OVERWRITE_PROMPT
        )
        self.statusbar.SetStatusText('Saved model {} into {}'.format(self.filename,self.dirname))

    '''
    Package Aspect Functions
    '''
    def OnPackageCreate(self, e):
        newTab = wx.Panel(self.packageNB)
        newTabName = "New Package!"
        numPages = self.packageNB.GetPageCount()
        self.packageNB.InsertPage(numPages-1,newTab, newTabName)
        self.packageNB.SetSelection(numPages-1)
    
    def OnPackageDelete(self, e):
        selectedPage = self.packageNB.GetSelection()
        numPages = self.packageNB.GetPageCount()
        if selectedPage != numPages - 1:
            if wx.MessageBox("Really delete package {}?".format(self.packageNB.GetPageText(selectedPage)), "Confirm",
                             wx.ICON_QUESTION | wx.YES_NO, self) == wx.NO:
                return
            self.packageNB.DeletePage(selectedPage)
        if self.packageNB.GetSelection() == numPages - 2: # deleted into last page
            self.packageTB.EnableTool(self.tdelete.GetId(), False)

    def OnUndo(self, e):
        pass
    def OnRedo(self, e):
        pass

    def pageChange(self, event):
        try:
            old = event.GetOldSelection()
            new = event.GetSelection()
            sel = self.packageNB.GetSelection()
            numPages = self.packageNB.GetPageCount()
            if new == numPages - 1:
                self.packageTB.EnableTool(self.packageTB_delete_ID, False)
            else:
                self.packageTB.EnableTool(self.packageTB_delete_ID, True)
            #print 'OnPageChanged,  old:%d, new:%d, sel:%d\n' % (old, new, sel)
        except wx.PyDeadObjectError:
            test = None #do nothing

    def OnPageChanged(self, event):
        self.pageChange(event)
        event.Skip()
 
    def OnPageChanging(self, event):
        self.pageChange(event)
        event.Skip()

    '''
    Tools Menubar Menu Functions
    '''
    def GenerateCode(self, e):
        self.dirname = ''
        self.dirname = dialogs.RMLGenerateDirDialog(frame=self,path=self.dirname)
        self.statusbar.SetStatusText('Generated workspace into {}'.format(self.dirname))
        
    def AnalyzeNetwork(self, e):
        pass

    def AnalyzeTiming(self, e):
        pass

    '''
    Build the menubar which allows for operations on
    * Files: New, Open, Save, Quit
    * Aspects: Packages, Hardware, Deployment
    * Views: Toolbar, Statusbar, Aspects, Output
    * Tools: Generator, Network QoS Analysis, Timing Analysis
    '''
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

    '''
    Bind menu items to functions for event processing
    '''
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

    '''
    Build the Statusbar which provides extra information about
    all the objects and menus in ROSMOD.  It also displays short
    info from the output about results of operations.
    '''
    def BuildStatusbar(self):
        self.statusbar = self.CreateStatusBar()
        self.statusbar.SetStatusText('Ready')

def main():
    
    ex = wx.App()
    Example(None)
    ex.MainLoop()    


if __name__ == '__main__':
    main()
