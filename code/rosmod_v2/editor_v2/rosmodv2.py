#!/usr/bin/python
'''
This Program is designed to be the user interface
for the ROSMOD modeling language.  It has the ability
to create, load, display, edit, and export ROSMOD models.

Additionally, it supports the interaction with analysis
tools through the use of toolbar buttons and subprocesses.
'''

import wx
import wx.stc as stc
import os,sys

sys.path.append("../generator_v2")
import rosgen

# for ordered dictionaries
from collections import OrderedDict

# proportional splitter should work for resizing window
#from proportionalSplitter import ProportionalSplitter

# flat notebook allows us to have scroll buttons and a close button
import wx.lib.agw.flatnotebook as fnb

# useful for drawing the model and having good scrolling of it
import wx.lib.scrolledpanel as scrolled

# need float canvas for new style of rendering
from wx.lib.floatcanvas import NavCanvas, FloatCanvas, Resources, Utilities

from contextMenu import ContextMenu

from dialogs import *

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
        self.viewSplitter = wx.SplitterWindow(self,wx.ID_NEW,style=wx.SP_PERMIT_UNSPLIT|wx.SP_BORDER|wx.SP_3DBORDER)
        self.BuildAspects()
        self.BuildOutput()
        self.viewSplitter.SplitHorizontally(self.activeAspect,self.output)
        self.viewSplitter.Bind(wx.EVT_SPLITTER_DCLICK,self.OnSplitterDClick)
        
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
        self.packageTB_create = self.toolbar.AddLabelTool(self.packageTB_create_ID, '', wx.Bitmap('icons/toolbar/tnew.png'), shortHelp="New Package")
        self.packageTB_delete = self.toolbar.AddLabelTool(self.packageTB_delete_ID, '', wx.Bitmap('icons/toolbar/texit.png'), shortHelp="Remove Package")
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

    def BuildPackagePage(self,parent,pkg):
        newPage = wx.Panel(parent)
        navCanvas = NavCanvas.NavCanvas(newPage,BackgroundColor = "BEIGE")
        canvas = navCanvas.Canvas
        msgWindow = stc.StyledTextCtrl(newPage,wx.ID_ANY,
                                style= wx.SUNKEN_BORDER)
        msgWindow.SetReadOnly(True)
        panelSizer = wx.BoxSizer(wx.VERTICAL)
        panelSizer.Add(navCanvas, 5, wx.EXPAND | wx.ALIGN_TOP )
        panelSizer.Add(msgWindow, 1, wx.EXPAND | wx.ALL | wx.ALIGN_BOTTOM ) 
        newPage.SetSizer(panelSizer)

        self.pkgPanels[pkg.properties["name"]] = [pkg,newPage,canvas,msgWindow,panelSizer]
        self.PackageAspect.AddPage( newPage, pkg.properties["name"])
        
        canvas.InitAll()
        drawable.Configure(pkg,self.styleDict)
        self.DrawModel(pkg,canvas)
        canvas.Zoom(1,pkg.textPosition.Get(),pkg.textPosition.Get())

    def DrawModel(self, model, canvas):
        canvasSizePixels = canvas.GetSize()
        bbox = [canvas.PixelToWorld((0,0)),canvas.PixelToWorld(canvasSizePixels)]
        bbox = [bbox[0][0],bbox[0][1],bbox[1][0],bbox[1][1]]
        bbox = Utilities.BBox.asBBox([[bbox[0],bbox[1]],[bbox[2],bbox[3]]])
        canvas.UnBindAll()
        canvas.ClearAll()
        canvas.SetProjectionFun(None)
        self.BindCanvasMouseEvents(canvas)
        width,height = drawable.Layout(model,(0,0),canvas)
        model.Draw(canvas,self.OnPkgLeftClick,self.OnPkgRightClick)
        canvas.Draw()
        canvas.ZoomToBB(bbox,True)

    def OnPkgLeftClick(self, Object):
        info = self.GetPackagePanelInfo()
        pkg = info[0]
        canvas = info[2]
        #print bbox
        self.activeObject = Object.Name
        drawable.Configure(pkg,self.styleDict)
        self.activeObject.style.overlay['overlayColor']="RED"
        kind = self.activeObject.kind
        keys = []
        if kind == 'message':
            keys = [['publisher','message_reference'],['subscriber','message_reference']]
        elif kind == 'service':
            keys = [['server','service_reference'],['client','service_reference']]
        elif kind == 'component':
            keys = [['component_instance','component_reference']]
        for key in keys:
            children = pkg.getChildrenByKind(key[0])
            for child in children:
                if child.properties[key[1]] == self.activeObject:
                    child.style.overlay['overlayColor']='RED'
        self.DrawModel(pkg,canvas)

    def OnPkgRightClick(self, Object):
        info = self.GetPackagePanelInfo()
        canvas = info[2]
        self.activeObject = Object.Name
        cm = OrderedDict()
        # set up proper context menu here: should be different per type of object
        cm['Edit'] = self.PkgEdit        # edits the object's properties (name, fields, etc.)
        cm['Delete'] = self.PkgDelete    # deletes the object and all references from the model
        self.PopupMenu(ContextMenu(canvas,cm))

    def PkgEdit(self, e):
        info = self.GetPackagePanelInfo()
        pkg = info[0]
        canvas = info[2]
        msgWindow = info[3]
        self.PackageLog(
            "Editing {} of type {}".format(self.activeObject.properties['name'],self.activeObject.kind),
            msgWindow)
        references = []
        if self.activeObject.kind == 'publisher' or self.activeObject.kind == 'subscriber':
            references = pkg.getChildrenByKind('message')
        elif self.activeObject.kind == 'server' or self.activeObject.kind == 'client':
            references = pkg.getChildrenByKind('service')
        elif self.activeObject.kind == 'component_instance':
            references = pkg.getChildrenByKind('component')
        ed = EditDialog(canvas,
                        editDict=self.activeObject.properties,
                        title="Edit "+self.activeObject.kind,
                        references = references,
                        style=wx.RESIZE_BORDER)
        ed.ShowModal()
        inputs = ed.GetInput()
        if inputs != OrderedDict():
            for key,value in inputs.iteritems():
                self.activeObject.properties[key] = value
            drawable.Configure(pkg,self.styleDict)
            self.DrawModel(pkg,canvas)

    def PkgDelete(self, e):
        info = self.GetPackagePanelInfo()
        canvas = info[2]
        msgWindow = info[3]
        self.PackageLog("Deleting {}".format(self.activeObject.properties),msgWindow)
        ConfirmDialog(canvas,"Delete {}?".format(self.activeObject.properties['name']))

    def BindCanvasMouseEvents(self,canvas):
        canvas.Bind(FloatCanvas.EVT_MOTION, self.OnPackageMouseMove)
        canvas.Bind(FloatCanvas.EVT_MOUSEWHEEL, self.OnPackageMouseWheel)
        canvas.Bind(FloatCanvas.EVT_LEFT_DOWN, self.OnPackageLeftDown) 
        canvas.Bind(FloatCanvas.EVT_LEFT_UP, self.OnPackageLeftUp)
        canvas.Bind(FloatCanvas.EVT_LEFT_DCLICK, self.OnPackageLeftDouble) 

        canvas.Bind(FloatCanvas.EVT_MIDDLE_DOWN, self.OnPackageMiddleDown) 
        canvas.Bind(FloatCanvas.EVT_MIDDLE_UP, self.OnPackageMiddleUp) 
        canvas.Bind(FloatCanvas.EVT_MIDDLE_DCLICK, self.OnPackageMiddleDouble) 

        canvas.Bind(FloatCanvas.EVT_RIGHT_DOWN, self.OnPackageRightDown) 
        canvas.Bind(FloatCanvas.EVT_RIGHT_UP, self.OnPackageRightUp) 
        canvas.Bind(FloatCanvas.EVT_RIGHT_DCLICK, self.OnPackageRightDouble)

    def UnBindCanvasMouseEvents(self,canvas):
        canvas.Unbind(FloatCanvas.EVT_MOTION)
        canvas.Unbind(FloatCanvas.EVT_MOUSEWHEEL)
        canvas.Unbind(FloatCanvas.EVT_LEFT_DOWN)
        canvas.Unbind(FloatCanvas.EVT_LEFT_UP)
        canvas.Unbind(FloatCanvas.EVT_LEFT_DCLICK)

        canvas.Unbind(FloatCanvas.EVT_MIDDLE_DOWN)
        canvas.Unbind(FloatCanvas.EVT_MIDDLE_UP)
        canvas.Unbind(FloatCanvas.EVT_MIDDLE_DCLICK)

        canvas.Unbind(FloatCanvas.EVT_RIGHT_DOWN)
        canvas.Unbind(FloatCanvas.EVT_RIGHT_UP)
        canvas.Unbind(FloatCanvas.EVT_RIGHT_DCLICK)

    def GetPackagePanelInfo(self):
        selectedPage = self.PackageAspect.GetSelection()
        packageName = self.PackageAspect.GetPageText(selectedPage)
        return self.pkgPanels[packageName]
    def PackageLog(self, text, msgWindow):
        msgWindow.SetReadOnly(False)
        msgWindow.AppendText(text)
        if not text[-1] == "\n":
            msgWindow.AppendText("\n")
        msgWindow.SetReadOnly(True)
        msgWindow.ScrollToLine(msgWindow.GetLineCount())
    def PrintCoords(self,event,msgWindow):
        self.PackageLog("coords are: %s"%(event.Coords,),msgWindow)
        self.PackageLog("pixel coords are: %s\n"%(event.GetPosition(),),msgWindow)
    def OnPackageMouseMove(self,event):
        pass
    def OnPackageMouseWheel(self,event):
        info = self.GetPackagePanelInfo()
        canvas = info[2]
        msgWindow = info[3]
        self.PackageLog("Mouse Wheel",msgWindow)
        self.PrintCoords(event,msgWindow)
        Rot = event.GetWheelRotation()
        Rot = Rot / abs(Rot) * 0.1
        if event.ControlDown(): # move left-right
            canvas.MoveImage( (Rot, 0), "Panel" )
        else: # move up-down
            canvas.MoveImage( (0, -Rot), "Panel" )
    def OnPackageLeftDown(self,event):
        info = self.GetPackagePanelInfo()
        msgWindow = info[3]
        self.PackageLog("LeftDown",msgWindow)
        self.PrintCoords(event,msgWindow)
    def OnPackageLeftUp(self,event):
        pass
    def OnPackageLeftDouble(self,event):
        pass
    def OnPackageMiddleDown(self,event):
        pass
    def OnPackageMiddleUp(self,event):
        pass
    def OnPackageMiddleDouble(self,event):
        pass
    def OnPackageRightDown(self,event):
        pass
    def OnPackageRightUp(self,event):
        info = self.GetPackagePanelInfo()
        canvas = info[2]
        self.activeObject = info[0]

        cm = OrderedDict()
        # set up proper context menu here: should be different per type of object
        cm['Edit'] = self.PkgEdit        # edits the object's properties (name, fields, etc.)
        cm['Delete'] = self.PkgDelete    # deletes the object and all references from the model
        self.PopupMenu(ContextMenu(canvas,cm))

    def OnPackageRightDouble(self,event):
        pass

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
    def OnSplitterDClick(self, e):
        self.shop.Check(False)
        self.UpdateMainWindow(e)
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
        self.tb_new = self.toolbar.AddLabelTool(wx.ID_NEW, '', wx.Bitmap('icons/toolbar/tnew.png'), shortHelp="New")
        self.tb_open = self.toolbar.AddLabelTool(wx.ID_OPEN, '', wx.Bitmap('icons/toolbar/topen.png'), shortHelp="Open")
        self.tb_save = self.toolbar.AddLabelTool(wx.ID_SAVE, '', wx.Bitmap('icons/toolbar/tsave.png'), shortHelp="Save")
        self.toolbar.AddSeparator()
        # undo/redo
        self.tb_undo = self.toolbar.AddLabelTool(wx.ID_UNDO, '', wx.Bitmap('icons/toolbar/tundo.png'), shortHelp="Undo")
        self.tb_redo = self.toolbar.AddLabelTool(wx.ID_REDO, '', wx.Bitmap('icons/toolbar/tredo.png'), shortHelp="Redo")
        self.toolbar.EnableTool(wx.ID_UNDO, False)
        self.toolbar.EnableTool(wx.ID_REDO, False)
        self.toolbar.AddSeparator()
        self.tb_term = self.toolbar.AddLabelTool(wx.ID_ANY, '', wx.Bitmap('icons/toolbar/tterm.png'), shortHelp="Terminal")
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
        font['pointSize'] = 10
        minSize = (10,10)
        padding = (10,10)
        pkgOffset = (50,50)
        msgIcon = wx.Bitmap('icons/model/msgIcon.png')
        srvIcon = wx.Bitmap('icons/model/srvIcon.png')
        tmrIcon = wx.Bitmap('icons/model/tmrIcon.png')
        pubIcon = wx.Bitmap('icons/model/pubIcon.png')
        subIcon = wx.Bitmap('icons/model/subIcon.png')
        clientIcon = wx.Bitmap('icons/model/clientIcon.png')
        serverIcon = wx.Bitmap('icons/model/serverIcon.png')
        compInstIcon = wx.Bitmap('icons/model/compInstIcon.png')

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
                                       padding = (50,25),
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
