#!/usr/bin/python

import wx
import os,sys

sys.path.append("../../generator_v2")
import rosgen

# for ordered dictionaries
from collections import OrderedDict

# proportional splitter should work for resizing window
from proportionalSplitter import ProportionalSplitter

# flat notebook allows us to have scroll buttons and a close button
import wx.lib.agw.flatnotebook as fnb

# useful for drawing the model and having good scrolling of it
import wx.lib.scrolledpanel as scrolled

# need float canvas for new style of rendering
from wx.lib.floatcanvas import NavCanvas, FloatCanvas, Resources

try:
    import numpy as N
    import numpy.random as RandomArray
    haveNumpy = True
    #print "Using numpy, version:", N.__version__
except ImportError:
            # numpy isn't there
            haveNumpy = False
            errorText = (
            "The FloatCanvas requires the numpy module, version 1.* \n\n"
            "You can get info about it at:\n"
            "http://numpy.scipy.org/\n\n"
            )

# terminal allows us to have a terminal panel
from terminal import *

# the dialogs that we use (popups)
import dialogs

# to draw the objects of the model
import drawable

class Example(wx.Frame):
    def __init__(self, *args, **kwargs):
        super(Example, self).__init__(*args,**kwargs)
        self.InitUI()
    
    def InitUI(self):

        self.fileTypes = "ROSMOD Model Files (*.rml *.rosml)|*.rosml"
        self.model_path = ''
        self.model = rosgen.ROS_Workspace()
        self.BuildStyleDict()

        # build the MenuBar,Toolbar, and Statusbar
        self.BuildMenu()
        self.BuildToolbar()
        self.BuildStatusbar()

        # build the main frame (holds viewer in the top and the output in the bottom)
        #self.viewSplitter = ProportionalSplitter(self,wx.ID_NEW,proportion=0.66)
        self.viewSplitter = wx.SplitterWindow(self,wx.ID_NEW,style=wx.SP_PERMIT_UNSPLIT)
        self.BuildAspects()
        self.BuildOutput()
        self.viewSplitter.SplitHorizontally(self.activeAspect,self.output)
        
        self.toolbar.Realize()
        self.Layout()
        
        self.SetSize((800, 600))
        self.SetTitle('ROSMOD Editor V2')
        self.Centre()
        self.Show(True)

    '''
    Build the output notebook for ROSMOD which holds:
    * the program output
    * a terminal
    * any logs requested from deployment
    '''
    def BuildOutput(self):
        self.output = fnb.FlatNotebook(self.viewSplitter, wx.ID_ANY)
        self.output.AddPage(wx.Panel(self.output), "Console Output")
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
        self.BuildPackageAspectNotebook()
        self.BuildPackageAspectToolbar()
    def BuildPackageAspectNotebook(self):
        self.PackageAspect = wx.Notebook(self.viewSplitter, wx.ID_ANY)
        self.PackageAspect.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGED, self.OnPageChanged)
        self.PackageAspect.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGING, self.OnPageChanging)
    def BuildPackageAspectToolbar(self):
        # create / delete packages
        self.packageTools = OrderedDict()
        self.packageTB_create_ID = wx.NewId()
        self.packageTB_delete_ID = wx.NewId()
        self.toolbar.AddSeparator()
        self.packageTB_create = self.toolbar.AddLabelTool(self.packageTB_create_ID, '', wx.Bitmap('tnew.png'), shortHelp="New Package")
        self.packageTB_delete = self.toolbar.AddLabelTool(self.packageTB_delete_ID, '', wx.Bitmap('texit.png'), shortHelp="Remove Package")
        self.packageTools['create package'] = [self.packageTB_create_ID,self.packageTB_create]
        self.packageTools['delete package'] = [self.packageTB_delete_ID,self.packageTB_delete]
        self.Bind(wx.EVT_TOOL, self.OnPackageCreate, self.packageTB_create)
        self.Bind(wx.EVT_TOOL, self.OnPackageDelete, self.packageTB_delete)
    def BuildPackageAspectPagesFromModel(self):
        self.PackageAspect.DeleteAllPages()
        self.pkgPanels = OrderedDict()
        for pkg in self.model.workspace.children:
            pkg.style.icon = None
            self.BuildPackagePage(self.PackageAspect,pkg)
        self.BuildPackagePage(self.PackageAspect,self.model.workspace)

    def OnPackagePaint(self,event):
        panel = event.GetEventObject()
        dc = wx.PaintDC(panel)
        dc.Clear()
        selectedPage = self.PackageAspect.GetSelection()
        packageName = self.PackageAspect.GetPageText(selectedPage)
        pkg = self.pkgPanels[packageName][0]
        drawable.Configure(pkg,self.styleDict)
        width,height = drawable.Layout(pkg,(0,0))
        panel.SetVirtualSize((width,height))
        panel.DoPrepareDC(dc)
        pkg.Draw(dc)
        panel.SetVirtualSize((width,height))

    def BuildPackagePage(self,parent,pkg):
        newPage = wx.ScrolledWindow(self.PackageAspect)
        newPage.SetScrollbars(wx.VERTICAL,10,1,10)
        newPage.SetScrollbar(wx.HORIZONTAL,10,1,10)
        newPage.Bind(wx.EVT_PAINT, self.OnPackagePaint)
        self.pkgPanels[pkg.properties["name"]] = [pkg,newPage]
        self.PackageAspect.AddPage( newPage, pkg.properties["name"])
        

    '''
    Hardware Aspect: panel with toolbar for configuring system hardware (hosts)
    '''
    def BuildHardwareAspect(self):
        self.HardwareAspect = wx.Notebook(self.viewSplitter)
        self.BuildHardwareAspectToolbar()
        self.HardwareAspect.Hide()
    def BuildHardwareAspectToolbar(self):
        self.hardwareTools = OrderedDict()
    '''
    Deployment Aspect: panel with toolbar and notebook for configuring and managing
    node deployment onto hosts (and roscore deployment)
    '''        
    def BuildDeploymentAspect(self):
        self.DeploymentAspect = wx.Notebook(self.viewSplitter)
        self.BuildDeploymentAspectToolbar()
        self.DeploymentAspect.Hide()
    def BuildDeploymentAspectToolbar(self):
        self.deploymentTools = OrderedDict()

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
        self.toolbar.Show(e.IsChecked())
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
        filename,model_path = dialogs.RMLFileDialog(
            frame = self,
            prompt ="Save Model As...", 
            path = self.model_path,
            fileTypes = self.fileTypes, 
            fd_flags = wx.FD_SAVE | wx.FD_OVERWRITE_PROMPT
        )
        if filename != None and model_path != None:
            self.filename = filename
            self.model_path = model_path
            self.statusbar.SetStatusText('Created model {} in {}'.format(self.filename,self.model_path))

    def OnOpen(self, e):
        filename, model_path = dialogs.RMLFileDialog(
            frame = self,
            fileTypes = self.fileTypes,
            path = self.model_path,
            prompt = "Choose a model",
            fd_flags = wx.FD_OPEN | wx.FD_FILE_MUST_EXIST
        )
        if filename != None and model_path != None:
            self.filename = filename
            self.model_path = model_path
            self.model = rosgen.parse_model(self.model_path+'/'+self.filename)
            self.BuildPackageAspectPagesFromModel()
            self.statusbar.SetStatusText('Loaded {} from {}'.format(self.filename,self.model_path))

    def OnSave(self, e):
        filename,model_path = dialogs.RMLFileDialog(
            frame = self,
            prompt = "Save Model As...",
            path = self.model_path,
            fileTypes = self.fileTypes, 
            fd_flags = wx.FD_SAVE | wx.FD_OVERWRITE_PROMPT
        )
        if filename != None and model_path != None:
            self.filename = filename
            self.model_path = model_path
            self.statusbar.SetStatusText('Saved model {} into {}'.format(self.filename,self.model_path))

    def OnUndo(self, e):
        pass
    def OnRedo(self, e):
        pass

    def OnTerminal(self, e):
        self.output.AddPage(TermEmulatorDemo(self.output), "Terminal")

    '''
    Package Aspect Functions
    '''
    def OnPackageCreate(self, e):
        newTab = wx.Panel(self.PackageAspect)
        newTabName = "New Package!"
        numPages = self.PackageAspect.GetPageCount()
        self.PackageAspect.InsertPage(numPages-1,newTab, newTabName)
        self.PackageAspect.SetSelection(numPages-1)
    
    def OnPackageDelete(self, e):
        selectedPage = self.PackageAspect.GetSelection()
        numPages = self.PackageAspect.GetPageCount()
        if selectedPage != numPages - 1:
            if wx.MessageBox("Really delete package {}?".format(self.PackageAspect.GetPageText(selectedPage)), "Confirm",
                             wx.ICON_QUESTION | wx.YES_NO, self) == wx.NO:
                return
            self.PackageAspect.DeletePage(selectedPage)
        if self.PackageAspect.GetSelection() == numPages - 2: # deleted into last page
            self.toolbar.EnableTool(self.packageTB_delete_ID, False)

    def pageChange(self, event):
        try:
            old = event.GetOldSelection()
            new = event.GetSelection()
            sel = self.PackageAspect.GetSelection()
            numPages = self.PackageAspect.GetPageCount()
            if new == numPages - 1:
                self.toolbar.EnableTool(self.packageTB_delete_ID, False)
            else:
                self.toolbar.EnableTool(self.packageTB_delete_ID, True)
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
        generate_path = dialogs.RMLGenerateDirDialog(frame=self,path=self.model_path)
        if generate_path != None:
            print generate_path
            rosgen.check_workspace(self.model, generate_path)
            rosgen.generate_workspace(self.model, generate_path)
            self.statusbar.SetStatusText('Generated workspace into {}'.format(generate_path))
        
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
    '''
    def BuildToolbar(self):
        self.toolbar = self.CreateToolBar()
        # file operations
        self.tb_new = self.toolbar.AddLabelTool(wx.ID_NEW, '', wx.Bitmap('tnew.png'), shortHelp="New")
        self.tb_open = self.toolbar.AddLabelTool(wx.ID_OPEN, '', wx.Bitmap('topen.png'), shortHelp="Open")
        self.tb_save = self.toolbar.AddLabelTool(wx.ID_SAVE, '', wx.Bitmap('tsave.png'), shortHelp="Save")
        self.toolbar.AddSeparator()
        # undo/redo
        self.tb_undo = self.toolbar.AddLabelTool(wx.ID_UNDO, '', wx.Bitmap('tundo.png'), shortHelp="Undo")
        self.tb_redo = self.toolbar.AddLabelTool(wx.ID_REDO, '', wx.Bitmap('tredo.png'), shortHelp="Redo")
        self.toolbar.EnableTool(wx.ID_UNDO, False)
        self.toolbar.EnableTool(wx.ID_REDO, False)
        self.toolbar.AddSeparator()
        self.tb_term = self.toolbar.AddLabelTool(wx.ID_ANY, '', wx.Bitmap('tterm.png'), shortHelp="Terminal")
        self.Bind(wx.EVT_TOOL, self.OnNew, self.tb_new)
        self.Bind(wx.EVT_TOOL, self.OnOpen, self.tb_open)
        self.Bind(wx.EVT_TOOL, self.OnSave, self.tb_save)
        self.Bind(wx.EVT_TOOL, self.OnUndo, self.tb_undo)
        self.Bind(wx.EVT_TOOL, self.OnRedo, self.tb_redo)
        self.Bind(wx.EVT_TOOL, self.OnTerminal, self.tb_term)

    '''
    Build the Statusbar which provides extra information about
    all the objects and menus in ROSMOD.  It also displays short
    info from the output about results of operations.
    '''
    def BuildStatusbar(self):
        self.statusbar = self.CreateStatusBar()
        self.statusbar.SetStatusText('Ready')

    def BuildStyleDict(self):
        self.styleDict = OrderedDict()
        font = OrderedDict()
        minSize = (30,30)
        padding = (10,10)
        pkgOffset = (100,50)
        msgIcon = wx.Bitmap('msgIcon_small.png')
        msgIcon = drawable.scale_bitmap(msgIcon, minSize[0], minSize[1])
        srvIcon = wx.Bitmap('srvIcon_small.png')
        srvIcon= drawable.scale_bitmap(srvIcon, minSize[0], minSize[1])
        tmrIcon = wx.Bitmap('tmrIcon_small.png')
        tmrIcon= drawable.scale_bitmap(tmrIcon, minSize[0], minSize[1])
        pubIcon = wx.Bitmap('pubIcon_small.png')
        pubIcon= drawable.scale_bitmap(pubIcon, minSize[0], minSize[1])
        subIcon = wx.Bitmap('subIcon_small.png')
        subIcon= drawable.scale_bitmap(subIcon, minSize[0], minSize[1])
        clientIcon = wx.Bitmap('clientIcon_small.png')
        clientIcon = drawable.scale_bitmap(clientIcon, minSize[0], minSize[1])
        serverIcon = wx.Bitmap('serverIcon_small.png')
        serverIcon = drawable.scale_bitmap(serverIcon, minSize[0], minSize[1])
        compInstIcon = wx.Bitmap('compInstIcon_small.png')
        compInstIcon = drawable.scale_bitmap(compInstIcon, minSize[0], minSize[1])

        e=wx.FontEnumerator()
        e.EnumerateFacenames(fixedWidthOnly = True)
        elist=e.GetFacenames()
        elist.sort()
        print elist[2]
        font['facename'] = elist[2]
        font['pointSize'] = 10

        testFont = wx.Font(
            pointSize=font['pointSize'],
            family=wx.FONTFAMILY_TELETYPE,
            style=wx.NORMAL,
            weight=wx.NORMAL,
            underline=False,
            face=font['facename']
        )
        font['size'] = testFont.GetPixelSize()

        minSize = (50,50)
        WrkStyle = drawable.Draw_Style(icon=None, 
                              font=font, 
                              method=drawable.Draw_Method.ICON, 
                              placement=drawable.Text_Placement.TOP,
                                       overlay = OrderedDict() )
        PkgStyle = drawable.Draw_Style(icon=None, 
                                       font=font, 
                                       method=drawable.Draw_Method.ICON, 
                                       placement=drawable.Text_Placement.TOP,
                                       overlay = OrderedDict(),
                                       padding = (50,10),
                                       offset = pkgOffset )
        MsgStyle = drawable.Draw_Style(icon=msgIcon, 
                              font=font, 
                              method=drawable.Draw_Method.ICON, 
                              placement=drawable.Text_Placement.TOP,
                              overlay = OrderedDict() )
        SrvStyle = drawable.Draw_Style(icon=srvIcon,
                              font=font, 
                              method=drawable.Draw_Method.ICON, 
                              placement=drawable.Text_Placement.TOP,
                              overlay = OrderedDict() )
        CompStyle = drawable.Draw_Style(icon=None,
                               font=font, 
                               method=drawable.Draw_Method.ROUND_RECT, 
                               placement=drawable.Text_Placement.TOP,
                                        overlay = OrderedDict([('fillColor','STEEL BLUE')]) )
        TmrStyle = drawable.Draw_Style(icon=tmrIcon,
                              font=font, 
                              method=drawable.Draw_Method.ICON, 
                              placement=drawable.Text_Placement.RIGHT,
                              overlay = OrderedDict() )
        PubStyle = drawable.Draw_Style(icon=pubIcon,
                              font=font, 
                              method=drawable.Draw_Method.ICON, 
                              placement=drawable.Text_Placement.RIGHT,
                              overlay = OrderedDict() )
        SubStyle = drawable.Draw_Style(icon=subIcon,
                              font=font, 
                              method=drawable.Draw_Method.ICON, 
                              placement=drawable.Text_Placement.RIGHT,
                              overlay = OrderedDict() )
        CliStyle = drawable.Draw_Style(icon=clientIcon,
                              font=font, 
                              method=drawable.Draw_Method.ICON, 
                              placement=drawable.Text_Placement.RIGHT,
                              overlay = OrderedDict() )
        SerStyle = drawable.Draw_Style(icon=serverIcon,
                              font=font, 
                              method=drawable.Draw_Method.ICON, 
                              placement=drawable.Text_Placement.RIGHT,
                              overlay = OrderedDict() )
        NodeStyle = drawable.Draw_Style(icon=None,
                               font=font, 
                               method=drawable.Draw_Method.ROUND_RECT, 
                               placement=drawable.Text_Placement.TOP,
                                        overlay = OrderedDict([('fillColor','TURQUOISE')]) )
        CompInstStyle = drawable.Draw_Style(icon=compInstIcon,
                                   font=font, 
                                   method=drawable.Draw_Method.ICON, 
                                   placement=drawable.Text_Placement.RIGHT,
                                   overlay = OrderedDict() )
        HardwareStyle = drawable.Draw_Style(icon=None, 
                                   font=font, 
                                   method=drawable.Draw_Method.ICON, 
                                   placement=drawable.Text_Placement.TOP,
                                   overlay = OrderedDict() )
        DeploymentStyle = drawable.Draw_Style(icon=None, 
                                     font=font, 
                                     method=drawable.Draw_Method.ICON, 
                                     placement=drawable.Text_Placement.TOP,
                                     overlay = OrderedDict() )

        self.styleDict["workspace"] = WrkStyle
        self.styleDict["package"] = PkgStyle
        self.styleDict["message"] = MsgStyle
        self.styleDict["service"] = SrvStyle
        self.styleDict["component"] = CompStyle
        self.styleDict["timer"] = TmrStyle
        self.styleDict["publisher"] = PubStyle
        self.styleDict["subscriber"] = SubStyle
        self.styleDict["client"] = CliStyle
        self.styleDict["server"] = SerStyle
        self.styleDict["node"] = NodeStyle
        self.styleDict["component_instance"] = CompInstStyle
        self.styleDict["hardware"] = HardwareStyle
        self.styleDict["deployment"] = DeploymentStyle

def main():
    
    ex = wx.App()
    Example(None)
    ex.MainLoop()    


if __name__ == '__main__':
    main()
