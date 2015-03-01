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
import copy

import multiprocessing

from fabric.api import *

sys.path.append("../ros_tools")
import ros_tools
sys.path.append("../ros_tools/deployment/tests")
import fabTest

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

class TBInfo():
    def __init__(self,name, obj):
        self.name = name
        self.obj = obj

class AspectPageInfo():
    def __init__(self, name, obj, canvas, msgWindow):
        self.name = name
        self.obj = obj
        self.canvas = canvas
        self.msgWindow = msgWindow

class AspectInfo():
    def __init__(self):
        self.pages = OrderedDict()
        self.toolbarButtons = OrderedDict()

    def GetTBInfo(self,name):
        if name in self.toolbarButtons.keys():
            return self.toolbarButtons[name]
        else:
            return None
    def AddTBInfo(self,tbInfo):
        self.toolbarButtons[tbInfo.name] = tbInfo
    def DelTBInfo(self,name):
        self.toolbarButtons.pop(name,None)

    def GetPageInfo(self,name):
        if name in self.pages.keys():
            return self.pages[name]
        else:
            return None
    def AddPageInfo(self,pageInfo):
        self.pages[pageInfo.name] = pageInfo
    def DelPageInfo(self,name):
        self.pages.pop(name,None)

class Example(wx.Frame):
    def __init__(self, *args, **kwargs):
        super(Example, self).__init__(*args,**kwargs)
        self.InitUI()
        self.Maximize()
    
    def InitUI(self):

        self.fileTypes = "ROSMOD Project (*.rosmod)|*.rosmod"
        self.project_path = ''
        self.project = ros_tools.ROS_Project()
        self.BuildStyleDict()

        # build the MenuBar,Toolbar, and Statusbar
        self.BuildMenu()
        self.BuildToolbar()
        self.BuildStatusbar()

        # build the main frame (holds viewer in the top and the output in the bottom)
        self.viewSplitter = wx.SplitterWindow(self,wx.ID_NEW,
                                              style=wx.SP_PERMIT_UNSPLIT|wx.SP_BORDER|wx.SP_3DBORDER)
        self.BuildAspects()
        self.BuildOutput()
        self.viewSplitter.SplitHorizontally(self.activeAspect,self.output,-100)
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
        self.PackageAspectInfo = AspectInfo()
        self.HardwareAspectInfo = AspectInfo()
        self.DeploymentAspectInfo = AspectInfo()
        self.BuildPackageAspect()
        self.BuildHardwareAspect()
        self.BuildDeploymentAspect()
        self.activeAspect = self.PackageAspect
        self.activeAspectInfo = self.PackageAspectInfo
    '''
    Hardware Aspect: panel with toolbar for configuring system hardware (hosts)
    '''
    def BuildHardwareAspect(self):
        self.HardwareAspect = fnb.FlatNotebook(self.viewSplitter,
                                              agwStyle=fnb.FNB_NODRAG|fnb.FNB_NO_X_BUTTON)
        self.HardwareAspect.Hide()
        self.HardwareAspect.Bind(fnb.EVT_FLATNOTEBOOK_PAGE_CHANGED, self.OnPageChanged)
        self.HardwareAspect.Bind(fnb.EVT_FLATNOTEBOOK_PAGE_CHANGING, self.OnPageChanging)
    def AddHardwareAspectToolbar(self):
        createTBinfo = TBInfo(
            name="create",
            obj=self.toolbar.AddTool(wx.ID_ANY,
                                     bitmap = wx.Bitmap('icons/toolbar/tnew.png'), 
                                     shortHelpString="New Hardware Configuration"))
        deleteTBinfo = TBInfo(
            name="delete",
            obj=self.toolbar.AddTool(wx.ID_ANY,
                                     bitmap = wx.Bitmap('icons/toolbar/texit.png'), 
                                     shortHelpString="Remove Hardware Configuration"))
        self.HardwareAspectInfo.AddTBInfo(createTBinfo)
        self.HardwareAspectInfo.AddTBInfo(deleteTBinfo)
        self.Bind(wx.EVT_TOOL, self.OnHardwareCreate, createTBinfo.obj)
        self.Bind(wx.EVT_TOOL, self.OnHardwareDelete, deleteTBinfo.obj)
        self.toolbar.Realize()
    def RemoveHardwareAspectToolbar(self):
        for name,tbinfo in self.HardwareAspectInfo.toolbarButtons.iteritems():
            self.toolbar.RemoveTool(tbinfo.obj.GetId())
            self.HardwareAspectInfo.DelTBInfo(name)
        self.toolbar.Realize()

    '''
    Deployment Aspect: panel with toolbar and notebook for configuring and managing
    node deployment onto hosts (and roscore deployment)
    '''        
    def BuildDeploymentAspect(self):
        self.deployed = False
        self.deploying = False
        self.DeploymentAspect = fnb.FlatNotebook(self.viewSplitter,
                                              agwStyle=fnb.FNB_NODRAG|fnb.FNB_NO_X_BUTTON)
        self.DeploymentAspect.Hide()
        self.DeploymentAspect.Bind(fnb.EVT_FLATNOTEBOOK_PAGE_CHANGED, self.OnPageChanged)
        self.DeploymentAspect.Bind(fnb.EVT_FLATNOTEBOOK_PAGE_CHANGING, self.OnPageChanging)
    def AddDeploymentAspectToolbar(self):
        createTBinfo = TBInfo( name="create",
                               obj=self.toolbar.AddTool(wx.ID_ANY,
                                                        bitmap = wx.Bitmap('icons/toolbar/tnew.png'), 
                                                        shortHelpString="New Deployment"))
        deleteTBinfo = TBInfo( name="delete",
                               obj=self.toolbar.AddTool(wx.ID_ANY,
                                                        bitmap = wx.Bitmap('icons/toolbar/texit.png'), 
                                                        shortHelpString="Remove Deployment"))
        deployTBinfo = TBInfo( name='deploy',
                               obj=self.toolbar.AddTool(wx.ID_ANY,
                                                        bitmap = wx.Bitmap('icons/toolbar/tdeploy.png'),
                                                        shortHelpString="Deploy System"))
        stopTBinfo = TBInfo( name='stop',
                               obj=self.toolbar.AddTool(wx.ID_ANY,
                                                        bitmap = wx.Bitmap('icons/toolbar/tstop.png'),
                                                        shortHelpString="Stop Deployed System"))
        self.DeploymentAspectInfo.AddTBInfo(createTBinfo)
        self.DeploymentAspectInfo.AddTBInfo(deleteTBinfo)
        self.DeploymentAspectInfo.AddTBInfo(deployTBinfo)
        self.Bind(wx.EVT_TOOL, self.OnDeploymentCreate, createTBinfo.obj)
        self.Bind(wx.EVT_TOOL, self.OnDeploymentDelete, deleteTBinfo.obj)
        self.Bind(wx.EVT_TOOL, self.OnDeploymentDeploy, deployTBinfo.obj)
        self.Bind(wx.EVT_TOOL, self.OnDeploymentStop, stopTBinfo.obj)
        self.toolbar.Realize()
    def RemoveDeploymentAspectToolbar(self):
        for name,tbinfo in self.DeploymentAspectInfo.toolbarButtons.iteritems():
            self.toolbar.RemoveTool(tbinfo.obj.GetId())
            self.DeploymentAspectInfo.DelTBInfo(name)
        self.toolbar.Realize()

    '''
    Package Aspect: panel with toolbar and notebook for managing packages
    '''
    def BuildPackageAspect(self):
        self.BuildPackageAspectNotebook()
        self.AddPackageAspectToolbar()
    def BuildPackageAspectNotebook(self):
        self.PackageAspect = fnb.FlatNotebook(self.viewSplitter, wx.ID_ANY,
                                              agwStyle=fnb.FNB_NODRAG|fnb.FNB_NO_X_BUTTON)
        self.PackageAspect.Bind(fnb.EVT_FLATNOTEBOOK_PAGE_CHANGED, self.OnPageChanged)
        self.PackageAspect.Bind(fnb.EVT_FLATNOTEBOOK_PAGE_CHANGING, self.OnPageChanging)
    def AddPackageAspectToolbar(self):
        createTBinfo = TBInfo(
            name="create",
            obj=self.toolbar.AddTool(wx.ID_ANY,
                                     bitmap = wx.Bitmap('icons/toolbar/tnew.png'), 
                                     shortHelpString="New Package"))
        deleteTBinfo = TBInfo(
            name="delete",
            obj=self.toolbar.AddTool(wx.ID_ANY,
                                     bitmap = wx.Bitmap('icons/toolbar/texit.png'), 
                                     shortHelpString="Remove Package"))
        self.PackageAspectInfo.AddTBInfo(createTBinfo)
        self.PackageAspectInfo.AddTBInfo(deleteTBinfo)
        self.Bind(wx.EVT_TOOL, self.OnPackageCreate, createTBinfo.obj)
        self.Bind(wx.EVT_TOOL, self.OnPackageDelete, deleteTBinfo.obj)
        self.toolbar.Realize()
    def RemovePackageAspectToolbar(self):
        for name,tbinfo in self.PackageAspectInfo.toolbarButtons.iteritems():
            self.toolbar.RemoveTool(tbinfo.obj.GetId())
            self.PackageAspectInfo.DelTBInfo(name)
        self.toolbar.Realize()

    def BuildAspectPages(self):
        self.BuildPackageAspectPages()
        self.BuildHardwareAspectPages()
        self.BuildDeploymentAspectPages()
    def BuildPackageAspectPages(self):
        self.PackageAspect.DeleteAllPages()
        for pkg in self.project.workspace.children:
            self.BuildModelPage( parent = self.PackageAspect,
                                 model = pkg,
                                 aspectInfo = self.PackageAspectInfo)
        self.BuildModelPage( parent = self.PackageAspect,
                             model = self.project.workspace,
                             aspectInfo = self.PackageAspectInfo)
        self.PackageAspect.AdvanceSelection()
    def BuildHardwareAspectPages(self):
        self.HardwareAspect.DeleteAllPages()
        for hw in self.project.hardware_configurations:
            self.BuildModelPage( parent = self.HardwareAspect,
                                 model = hw,
                                 aspectInfo = self.HardwareAspectInfo)
        self.DeploymentAspect.AdvanceSelection()
    def BuildDeploymentAspectPages(self):
        self.DeploymentAspect.DeleteAllPages()
        for dep in self.project.deployments:
            self.BuildModelPage( parent = self.DeploymentAspect,
                                 model = dep,
                                 aspectInfo = self.DeploymentAspectInfo)
        self.DeploymentAspect.AdvanceSelection()

    def BuildModelPage(self,parent,model,aspectInfo,insertPos=-1):
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

        pageInfo = AspectPageInfo(name=model.properties['name'],obj=model,canvas=canvas,msgWindow=msgWindow)
        aspectInfo.AddPageInfo(pageInfo)
        if insertPos == -1:
            parent.AddPage( newPage, model.properties['name'] )
        else:
            parent.InsertPage( insertPos, newPage, model.properties["name"])
        parent.AdvanceSelection()
        canvas.InitAll()
        drawable.Configure(model,self.styleDict)
        self.DrawModel(model,canvas)

    def DrawModel(self, model, canvas):
        canvas.UnBindAll()
        canvas.ClearAll()
        canvas.SetProjectionFun(None)
        self.BindCanvasMouseEvents(canvas)
        width,height = drawable.Layout(model,(0,0),canvas)
        model.Draw(canvas,self.OnLeftClick,self.OnRightClick)
        canvas.Draw()
        canvas.ZoomToBB()

    def OnLeftClick(self, Object):
        if self.activeAspect == self.PackageAspect:
            self.OnPkgLeftClick(Object)
        elif self.activeAspect == self.HardwareAspect:
            self.OnHWLeftClick(Object)
        elif self.activeAspect == self.DeploymentAspect:
            self.OnDeploymentLeftClick(Object)

    def OnPkgLeftClick(self, Object):
        info = self.GetActivePanelInfo()
        pkg = info.obj
        canvas = info.canvas
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

    def OnHWLeftClick(self, Object):
        pass

    def OnDeploymentLeftClick(self, Object):
        pass

    def OnRightClick(self, Object):
        info = self.GetActivePanelInfo()
        canvas = info.canvas
        self.activeObject = Object.Name
        self.PopupMenu(ContextMenu(canvas,self.AspectContextMenu(self.activeObject)))

    def AspectContextMenu(self, obj):
        cm = OrderedDict()
        cm['Edit'] = self.AspectEdit        # edits the object's properties (name, fields, etc.)
        cm['Delete'] = self.AspectDelete    # deletes the object and all references from the model
        if obj.kind == 'component':
            cm = self.BuildCompContextMenu(cm)
        elif obj.kind == 'node':
            cm = self.BuildNodeContextMenu(cm)
        elif obj.kind == 'package':
            cm = self.BuildPackageContextMenu(cm)
        elif obj.kind == 'workspace':
            cm = self.BuildWorkspaceContextMenu(cm)
        elif obj.kind == 'hardware_configuration':
            cm = self.BuildHardwareContextMenu(cm)
        elif obj.kind == 'host':
            cm = self.BuildHostContextMenu(cm)
        elif obj.kind == 'deployment':
            cm = self.BuildDeploymentContextMenu(cm)
        elif obj.kind == 'host_instance':
            cm = self.BuildHostInstanceContextMenu(cm)
        elif obj.kind == 'node_instance':
            cm = self.BuildNodeInstanceContextMenu(cm)
        return cm

    def BuildCompContextMenu(self,cm):
        cm['Add Timer'] = lambda evt : self.CompAdd(evt,'timer')
        cm['Add Subscriber'] = lambda evt :self.CompAdd(evt,'subscriber')
        cm['Add Publisher'] = lambda evt : self.CompAdd(evt,'publisher')
        cm['Add Client'] = lambda evt : self.CompAdd(evt,'client')
        cm['Add Server'] = lambda evt : self.CompAdd(evt,'server')
        return cm
    def BuildNodeContextMenu(self,cm):
        cm['Add Component Instance'] = lambda evt : self.NodeAdd(evt,'component_instance')
        return cm
    def BuildPackageContextMenu(self, cm):
        cm['Add Message'] = lambda evt : self.PackageAdd(evt,'message')
        cm['Add Service'] = lambda evt : self.PackageAdd(evt,'service')
        cm['Add Component Definition'] = lambda evt : self.PackageAdd(evt,'component')
        cm['Add Node'] = lambda evt : self.PackageAdd(evt,'node')
        return cm
    def BuildWorkspaceContextMenu(self,cm):
        return cm
    def BuildHardwareContextMenu(self,cm):
        cm['Add Host'] = lambda evt : self.HardwareAdd(evt,'host')
        return cm
    def BuildHostContextMenu(self,cm):
        return cm
    def BuildDeploymentContextMenu(self,cm):
        cm['Add Host Instance'] = lambda evt : self.DeploymentAdd(evt,'host_instance')
        return cm
    def BuildHostInstanceContextMenu(self,cm):
        cm['Add Node Instance'] = lambda evt : self.HostInstAdd(evt,'node_instance')
        return cm
    def BuildNodeInstanceContextMenu(self,cm):
        return cm

    def GenericAdd(self,newObj,refs,parent):
        info = self.GetActivePanelInfo()
        pkg = info.obj
        canvas = info.canvas
        msgWindow = info.msgWindow
        kind = newObj.kind
        if newObj != None:
            newObj.properties['name'] = "New" + kind
            ed = EditDialog(self,
                            editDict=newObj.properties,
                            editObj = newObj,
                            title="Edit "+newObj.kind,
                            references = refs,
                            style=wx.RESIZE_BORDER)
            ed.ShowModal()
            inputs = ed.GetInput()
            if inputs != OrderedDict():
                for key,value in inputs.iteritems():
                    newObj.properties[key] = value
                parent.add(newObj)
                self.AspectLog(
                    "Added child {} to parent {}".format(newObj.properties['name'],parent.properties['name']),
                    msgWindow)
                drawable.Configure(pkg,self.styleDict)
                self.DrawModel(pkg,canvas)            

    def CompAdd(self,e,kind):
        comp = self.activeObject
        newObj = None
        references = []
        info = self.GetActivePanelInfo()
        pkg = info.obj
        if kind == 'timer':
            newObj = ros_tools.ROS_Timer()
        elif kind == 'subscriber':
            newObj = ros_tools.ROS_Subscriber()
            newObj.parent = comp
            references = self.project.workspace.getChildrenByKind('message')
        elif kind == 'publisher':
            newObj = ros_tools.ROS_Publisher()
            newObj.parent = comp
            references = self.project.workspace.getChildrenByKind('message')
        elif kind == 'server':
            newObj = ros_tools.ROS_Server()
            newObj.parent = comp
            references = self.project.workspace.getChildrenByKind('service')
        elif kind == 'client':
            newObj = ros_tools.ROS_Client()
            newObj.parent = comp
            references = self.project.workspace.getChildrenByKind('service')
        if newObj != None:
            self.GenericAdd(newObj,references,comp)


    def NodeAdd(self,e,kind):
        node = self.activeObject
        newObj = None
        references = []
        info = self.GetActivePanelInfo()
        pkg = info.obj
        if kind == 'component_instance':
            newObj = ros_tools.ROS_Component_Instance()
            references = node.parent.getChildrenByKind('component')
        if newObj != None:
            self.GenericAdd(newObj,references,node)
    
    def PackageAdd(self,e,kind):
        package = self.activeObject
        newObj = None
        references = []
        if kind == 'message':
            newObj = ros_tools.ROS_Message()
        elif kind == 'service':
            newObj = ros_tools.ROS_Service()
        elif kind == 'component':
            newObj = ros_tools.ROS_Component()
        elif kind == 'node':
            newObj = ros_tools.ROS_Node()
        if newObj != None:
            self.GenericAdd(newObj,references,package)

    def HardwareAdd(self,e,kind):
        hw = self.activeObject
        newObj = None
        references = []
        if kind == 'host':
            newObj = ros_tools.ROS_Host()
        if newObj != None:
            self.GenericAdd(newObj,references,hw)

    def DeploymentAdd(self,e,kind):
        dep = self.activeObject
        newObj = None
        references = []
        if kind == 'host_instance':
            newObj = ros_tools.ROS_Host_Instance()
            references = dep.properties['hardware_configuration_reference'].children
        if newObj != None:
            self.GenericAdd(newObj,references,dep)

    def HostInstAdd(self,e,kind):
        host = self.activeObject
        newObj = None
        references = []
        if kind == 'node_instance':
            newObj = ros_tools.ROS_Node_Instance()
            references = self.project.workspace.getChildrenByKind('node')
        if newObj != None:
            self.GenericAdd(newObj,references,host)

    def AspectEdit(self, e):
        info = self.GetActivePanelInfo()
        pkg = info.obj
        canvas = info.canvas
        msgWindow = info.msgWindow
        self.AspectLog(
            "Editing {} of type {}".format(self.activeObject.properties['name'],self.activeObject.kind),
            msgWindow)
        references = []
        if self.activeObject.kind == 'publisher' or self.activeObject.kind == 'subscriber':
            references = self.project.workspace.getChildrenByKind('message')
        elif self.activeObject.kind == 'server' or self.activeObject.kind == 'client':
            references = self.project.workspace.getChildrenByKind('service')
        elif self.activeObject.kind == 'component_instance':
            references = self.activeObject.parent.parent.getChildrenByKind('component')
        elif self.activeObject.kind == 'deployment':
            references = self.project.hardware_configurations
        elif self.activeObject.kind == 'host_instance':
            references = pkg.properties['hardware_configuration_reference'].children
        elif self.activeObject.kind == 'node_instance':
            references = self.project.workspace.getChildrenByKind('node')
        ed = EditDialog(canvas,
                        editDict=self.activeObject.properties,
                        editObj = self.activeObject,
                        title="Edit "+self.activeObject.kind,
                        references = references,
                        style=wx.RESIZE_BORDER)
        ed.ShowModal()
        prevProps = copy.copy(self.activeObject.properties)
        inputs = ed.GetInput()
        if inputs != OrderedDict():
            for key,value in inputs.iteritems():
                self.activeObject.properties[key] = value
            if self.activeObject.kind == 'package' or \
               self.activeObject.kind == 'workspace' or \
               self.activeObject.kind == 'hardware_configuration' or \
               self.activeObject.kind == 'deployment':
                newName = self.activeObject.properties['name']
                prevName = prevProps['name']
                info.name = newName
                self.activeAspectInfo.AddPageInfo(info)
                if newName != prevName:
                    self.activeAspectInfo.DelPageInfo(prevName)
            if self.activeObject.kind == 'deployment':
                prevRef = prevProps['hardware_configuration_reference']
                newRef = self.activeObject.properties['hardware_configuration_reference']
                if newRef != prevRef:
                    self.activeObject.children = []
            drawable.Configure(pkg,self.styleDict)
            selectedPage = self.activeAspect.GetSelection()
            self.activeAspect.SetPageText(selectedPage,pkg.properties['name'])
            self.DrawModel(pkg,canvas)

    def AspectDelete(self, e):
        info = self.GetActivePanelInfo()
        model = info.obj
        canvas = info.canvas
        msgWindow = info.msgWindow
        if model.kind != 'workspace':
            if self.activeObject.kind == 'package':
                wx.CallAfter(self.OnPackageDelete,e)
            elif self.activeObject.kind == 'hardware_configuration':
                wx.CallAfter(self.OnHardwareDelete,e)
            elif self.activeObject.kind == 'deployment':
                wx.CallAfter(self.OnDeploymentDelete,e)
            else:
                if ConfirmDialog(canvas,"Delete {}?".format(self.activeObject.properties['name'])):
                    self.AspectLog("Deleting {}".format(self.activeObject.properties['name']),msgWindow)
                    self.activeObject.deleteAllRefs(self.project)
                    self.activeObject.delete()
                    drawable.Configure(model,self.styleDict)
                    self.DrawModel(model,canvas)
        else:
            dialogs.ErrorDialog(self,"Cannot delete workspace!")
        self.activeObject = None

    def OnHardwareCreate(self, e):
        newObj = ros_tools.ROS_HW()
        newObj.properties['name'] = "New Hardware Configuration"
        ed = EditDialog(self,
                        editDict=newObj.properties,
                        title="Edit "+newObj.kind,
                        references = [],
                        style=wx.RESIZE_BORDER)
        ed.ShowModal()
        inputs = ed.GetInput()
        if inputs != OrderedDict():
            for key,value in inputs.iteritems():
                newObj.properties[key] = value
            self.project.hardware_configurations.append(newObj)
            numPages = self.HardwareAspect.GetPageCount()
            if numPages <= 0:
                numPages = 1
            self.BuildModelPage(self.HardwareAspect,newObj,self.HardwareAspectInfo,numPages-1)
            self.HardwareAspect.SetSelection(numPages - 1)
    def OnHardwareDelete(self, e):
        selectedPage = self.HardwareAspect.GetSelection()
        numPages = self.HardwareAspect.GetPageCount()
        objName = self.HardwareAspect.GetPageText(selectedPage)
        info = self.HardwareAspectInfo.GetPageInfo(objName)
        obj = info.obj
        if ConfirmDialog(self,"Delete {}?".format(objName)):
            info.canvas.ClearAll()
            self.project.hardware_configurations = [x for x in self.project.hardware_configurations if x != obj]
            self.HardwareAspect.GetPage(selectedPage).DestroyChildren()
            self.HardwareAspectInfo.DelPageInfo(obj.properties['name'])
            self.HardwareAspect.DeletePage(selectedPage)
        
    def OnDeploymentCreate(self, e):
        newObj = ros_tools.ROS_Deployment()
        newObj.properties['name'] = "New Deployment"
        references = self.project.hardware_configurations
        ed = EditDialog(self,
                        editDict=newObj.properties,
                        title="Edit "+newObj.kind,
                        references = references,
                        style=wx.RESIZE_BORDER)
        ed.ShowModal()
        inputs = ed.GetInput()
        if inputs != OrderedDict():
            for key,value in inputs.iteritems():
                newObj.properties[key] = value
            self.project.deployments.append(newObj)
            numPages = self.DeploymentAspect.GetPageCount()
            if numPages <= 0:
                numPages = 1
            self.BuildModelPage(self.DeploymentAspect,newObj,self.DeploymentAspectInfo,numPages-1)
            self.DeploymentAspect.SetSelection(numPages - 1)
                
    def OnDeploymentDelete(self, e):
        selectedPage = self.DeploymentAspect.GetSelection()
        objName = self.DeploymentAspect.GetPageText(selectedPage)
        info = self.DeploymentAspectInfo.GetPageInfo(objName)
        obj = info.obj
        if ConfirmDialog(self,"Delete {}?".format(objName)):
            info.canvas.ClearAll()
            self.project.deployments = [x for x in self.project.deployments if x != obj]
            self.DeploymentAspect.GetPage(selectedPage).DestroyChildren()
            self.DeploymentAspectInfo.DelPageInfo(obj.properties['name'])
            self.DeploymentAspect.DeletePage(selectedPage)

    def OnDeploymentDeploy(self,e):
        if self.deployed == False:
            self.deploying = True
            selectedPage = self.DeploymentAspect.GetSelection()
            objName = self.DeploymentAspect.GetPageText(selectedPage)
            info = self.DeploymentAspectInfo.GetPageInfo(objName)
            dep = info.obj
            env.use_ssh_config = False
            hostDict = {}
            env.hosts = []
            for host in dep.children:
                nodeList = []
                for node in host.children:
                    nodeList.append(fabTest.deployed_node(
                        executable = node.properties['node_reference'].parent.properties['name'] + '/' + node.properties['node_reference'].properties['name'],
                        cmdArgs = node.properties['cmdline_arguments']
                    ))
                hostDict[host.properties['name']] = fabTest.deployed_host(
                    userName = host.properties['username'],
                    ipAddress = host.properties['host_reference'].properties['ip_address'],
                    keyFile = host.properties['sshkey'],
                    nodes = nodeList,
                    envVars = copy.copy(host.properties['env_variables'])
                )
                hostDict[host.properties['name']].envVars.append(['ROS_MASTER_URI','http://129.59.79.169:11311/'])
                hostDict[host.properties['name']].envVars.append(
                    ['ROS_IP',host.properties['host_reference'].properties['ip_address']]
                )
                env.hosts.append(host.properties['name'])
            self.depQ = multiprocessing.Queue()
            p = multiprocessing.Process(target=fabTest.deployTest, args=(self.depQ,))
            self.depQ.put(hostDict)
            p.start()
            p.join()
            self.deployed = True
            self.deploying = False
        else:
            ErrorDialog(self,"System is already running a deployment!")

    def OnDeploymentStop(self,e):
        if self.deployed == True:
            p = multiprocessing.Process(target=fabTest.stopTest, args=(self.depQ,))
            p.start()
            p.join()
            self.deployed = False
            self.deploying = False
        else:
            ErrorDialog(self,"System is not running a deployment")

    def OnPackageCreate(self, e):
        newPkg = ros_tools.ROS_Package()
        newPkg.properties['name'] = "New Package"
        ed = EditDialog(self,
                        editDict=newPkg.properties,
                        title="Edit "+newPkg.kind,
                        references = [],
                        style=wx.RESIZE_BORDER)
        ed.ShowModal()
        inputs = ed.GetInput()
        if inputs != OrderedDict():
            for key,value in inputs.iteritems():
                newPkg.properties[key] = value
            self.project.workspace.add(newPkg)
            numPages = self.PackageAspect.GetPageCount()
            self.BuildModelPage(self.PackageAspect,newPkg,self.PackageAspectInfo,numPages-1)
            self.PackageAspect.SetSelection(numPages - 1)
    
    def OnPackageDelete(self, e):
        selectedPage = self.PackageAspect.GetSelection()
        numPages = self.PackageAspect.GetPageCount()
        pkgName = self.PackageAspect.GetPageText(selectedPage)
        info = self.PackageAspectInfo.GetPageInfo(pkgName)
        pkg = info.obj
        if pkg.kind != 'workspace':
            if ConfirmDialog(self,"Delete {}?".format(pkgName)):
                info.canvas.ClearAll()
                pkg.delete()
                self.PackageAspect.GetPage(selectedPage).DestroyChildren()
                self.PackageAspectInfo.DelPageInfo(pkg.properties['name'])
                self.PackageAspect.DeletePage(selectedPage)
        else:
            dialogs.ErrorDialog(self,"Cannot Delete Workspace!")

    def BindCanvasMouseEvents(self,canvas):
        canvas.Bind(FloatCanvas.EVT_MOUSEWHEEL, self.OnMouseWheel)
        canvas.Bind(FloatCanvas.EVT_RIGHT_UP, self.OnRightUp) 

    def UnBindCanvasMouseEvents(self,canvas):
        canvas.Unbind(FloatCanvas.EVT_MOUSEWHEEL)
        canvas.Unbind(FloatCanvas.EVT_RIGHT_UP)

    def GetActivePanelInfo(self):
        selectedAspect = self.activeAspect
        selectedAspectInfo = self.activeAspectInfo
        selectedPage = selectedAspect.GetSelection()
        pageName = selectedAspect.GetPageText(selectedPage)
        pageInfo = selectedAspectInfo.GetPageInfo(pageName)
        return pageInfo
    def AspectLog(self, text, msgWindow):
        msgWindow.SetReadOnly(False)
        msgWindow.AppendText(text)
        if not text[-1] == "\n":
            msgWindow.AppendText("\n")
        msgWindow.SetReadOnly(True)
        msgWindow.ScrollToLine(msgWindow.GetLineCount())
    def OnMouseWheel(self,event):
        info = self.GetActivePanelInfo()
        canvas = info.canvas
        Rot = event.GetWheelRotation()
        Rot = Rot / abs(Rot) * 0.1
        if event.ControlDown(): # move left-right
            canvas.MoveImage( (Rot, 0), "Panel" )
        else: # move up-down
            canvas.MoveImage( (0, -Rot), "Panel" )
    def OnRightUp(self,event):
        info = self.GetActivePanelInfo()
        canvas = info.canvas
        self.activeObject = info.obj
        self.PopupMenu(ContextMenu(canvas,self.AspectContextMenu(self.activeObject)))

    '''
    Aspect Menubar Menu functions
    '''
    def HideAllAspects(self):
        self.PackageAspect.Hide()
        self.HardwareAspect.Hide()
        self.DeploymentAspect.Hide()
        self.RemovePackageAspectToolbar()
        self.RemoveHardwareAspectToolbar()
        self.RemoveDeploymentAspectToolbar()

    def ShowAspect(self,aspect):
        if self.shvw.IsChecked():
            aspect.Show()
            self.viewSplitter.ReplaceWindow(self.activeAspect,aspect)
            self.activeAspect = aspect

    def OnPackageAspect(self, e):
        self.activeAspectInfo = self.PackageAspectInfo
        self.HideAllAspects()
        self.ShowAspect(self.PackageAspect)
        self.AddPackageAspectToolbar()
        self.pageChange(None)

    def OnHardwareAspect(self, e):
        self.activeAspectInfo = self.HardwareAspectInfo
        self.HideAllAspects()
        self.ShowAspect(self.HardwareAspect)
        self.AddHardwareAspectToolbar()
        self.pageChange(None)

    def OnDeploymentAspect(self, e):
        self.activeAspectInfo = self.DeploymentAspectInfo
        self.HideAllAspects()
        self.ShowAspect(self.DeploymentAspect)
        self.AddDeploymentAspectToolbar()
        self.pageChange(None)
        
    '''
    View Menu Functions
    '''
    def UpdateMainWindow(self, e):
        self.viewSplitter.Show()
        self.viewSplitter.SplitHorizontally(self.activeAspect,self.output,self.viewSplitter.GetSashPosition())
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
        if ConfirmDialog(self,"Really quit ROSMOD?"):
            self.Close()

    def OnNew(self, e):
        project_path = dialogs.RMLDirectoryDialog(
            frame = self,
            prompt ="Choose New Project Location", 
            path = self.project_path,
        )
        if project_path != None:
            dlgDict = OrderedDict([('name','New Project')])
            ed = dialogs.EditDialog( self,
                                     editDict = dlgDict,
                                     title = 'Choose Project Name',
                                     style = wx.RESIZE_BORDER)
            ed.ShowModal()
            inputs = ed.GetInput()
            if inputs != OrderedDict():
                self.filename = inputs['name']
                self.project_path = project_path
                self.project = ros_tools.ROS_Project()
                self.project.new(self.filename,self.project_path)
                self.project.workspace.properties['name'] = "Workspace"
                newHW = ros_tools.ROS_HW()
                newHW.properties['name'] = "Hardware"
                self.project.hardware_configurations.append(newHW)
                newDeployment = ros_tools.ROS_Deployment()
                newDeployment.properties['name'] = "Deployment"
                newDeployment.properties['hardware_configuration_reference'] = newHW
                self.project.deployments.append(newDeployment)
                self.BuildAspectPages()
                self.statusbar.SetStatusText('Created new project: {} in {}'.format(self.filename,self.project_path))

    def OnOpen(self, e):
        filename, model_path = dialogs.RMLFileDialog(
            frame = self,
            fileTypes = self.fileTypes,
            path = self.project_path,
            prompt = "Choose a ROS Project",
            fd_flags = wx.FD_OPEN | wx.FD_FILE_MUST_EXIST
        )
        if filename != None and model_path != None:
            self.filename = filename
            self.project_path = model_path
            self.project.open(self.project_path)
            self.BuildAspectPages()
            self.statusbar.SetStatusText('Loaded {} from {}'.format(self.filename,self.project_path))

    def OnSave(self, e):
        self.project.save()

    def OnSaveAs(self, e):
        dlgDict = OrderedDict([('name',self.project.project_name)])
        ed = dialogs.EditDialog( self,
                                 editDict = dlgDict,
                                 title = 'Choose New Project Name',
                                 style = wx.RESIZE_BORDER)
        ed.ShowModal()
        inputs = ed.GetInput()
        if inputs != OrderedDict():
            project_path = dialogs.RMLDirectoryDialog(
                frame = self,
                prompt ="Choose New Project Location", 
                path = self.project_path,
            )
            if project_path != None:
                self.filename = inputs['name']
                self.project_path = project_path
                self.project.save(self.filename,self.project_path)
                self.BuildAspectPages()
                self.statusbar.SetStatusText('Saved project as: {} in {}'.format(self.filename,self.project_path))

    def OnUndo(self, e):
        pass
    def OnRedo(self, e):
        pass

    def OnTerminal(self, e):
        self.shop.Check(True)
        self.UpdateMainWindow(e)
        self.output.AddPage(TermEmulatorDemo(self.output), "Terminal")

    '''
    Package Aspect Functions
    '''
    def pageChange(self, event):
        selectedAspect = self.activeAspect
        selectedAspectInfo = self.activeAspectInfo
        selectedAspect.Refresh()
        sel = selectedAspect.GetSelection()
        numPages = selectedAspect.GetPageCount()
        if sel >= 0:
            deleteTBID = selectedAspectInfo.GetTBInfo("delete").obj.GetId()
            if sel >= numPages - 1:
                self.toolbar.EnableTool(deleteTBID, False)
            else:
                self.toolbar.EnableTool(deleteTBID, True)
                
            pageName = selectedAspect.GetPageText(sel)
            info = selectedAspectInfo.GetPageInfo(pageName)
            pkg = info.obj
            canvas = info.canvas
            drawable.Configure(pkg,self.styleDict)
            self.DrawModel(pkg,canvas)
        
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
        self.project.generate_workspace()
        dialogs.InfoDialog(self,"Generated ROS Workspace.")
        self.statusbar.SetStatusText('Generated ROS Workspace')
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
        self.newMI = self.fileMenu.Append(wx.ID_NEW, '&New', 'New Project')
        self.openMI = self.fileMenu.Append(wx.ID_OPEN, '&Open', 'Open existing Project')
        self.saveMI = self.fileMenu.Append(wx.ID_SAVE, '&Save', 'Save current Project')
        self.saveAsMI = self.fileMenu.Append(wx.ID_SAVEAS, 'Save &As', 'Save current Project As...')
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
        self.Bind(wx.EVT_MENU, self.OnSaveAs, self.saveAsMI)
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
        self.toolbar.AddSeparator()
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
        font['pointSize'] = 20
        minSize = (30,30)
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
        hostIcon = wx.Bitmap('icons/model/bbb.png')
        hostInstIcon = wx.Bitmap('icons/model/bbb.png')

        '''
        STYLES USED FOR HARDWARE CONFIGURATION OBJECTS
        '''
        HardwareStyle = drawable.Draw_Style(icon=None, 
                              font=font, 
                                       method=drawable.Draw_Method.ICON, 
                                       offset = pkgOffset,
                              placement=drawable.Text_Placement.TOP,
                                       overlay = OrderedDict() )
        HostStyle = drawable.Draw_Style(icon=hostIcon,
                                   font=font, 
                                   method=drawable.Draw_Method.ICON, 
                                       minSize = minSize,
                                   placement=drawable.Text_Placement.TOP,
                                   overlay = OrderedDict() )
        '''
        STYLES USED FOR DEPLOYMENT CONFIGURATION OBJECTS
        '''
        DeploymentStyle = drawable.Draw_Style(icon=None, 
                              font=font, 
                                       method=drawable.Draw_Method.ICON, 
                                       offset = pkgOffset,
                              placement=drawable.Text_Placement.TOP,
                                       overlay = OrderedDict() )
        NodeInstStyle = drawable.Draw_Style(icon=None,
                               font=font, 
                               method=drawable.Draw_Method.ROUND_RECT, 
                                       minSize = minSize,
                               placement=drawable.Text_Placement.RIGHT,
                                        overlay = OrderedDict([('fillColor','TURQUOISE')]) )
        HostInstStyle = drawable.Draw_Style(icon=None,
                                   font=font, 
                                   method=drawable.Draw_Method.ROUND_RECT, 
                                       minSize = minSize,
                                   placement=drawable.Text_Placement.TOP,
                                            overlay = OrderedDict([('fillColor','WHITE')]) )
        '''
        STYLES USED FOR SOFTWARE CONFIGURATION OBJECTS
        '''
        WrkStyle = drawable.Draw_Style(icon=None, 
                              font=font, 
                                       method=drawable.Draw_Method.ICON, 
                                       offset = pkgOffset,
                              placement=drawable.Text_Placement.TOP,
                                       overlay = OrderedDict() )
        PkgStyle = drawable.Draw_Style(icon=None, 
                                       font=font, 
                                       method=drawable.Draw_Method.ICON, 
                                       placement=drawable.Text_Placement.TOP,
                                       overlay = OrderedDict(),
                                       minSize = minSize,
                                       padding = (50,25),
                                       offset = pkgOffset )
        MsgStyle = drawable.Draw_Style(icon=msgIcon, 
                              font=font, 
                              method=drawable.Draw_Method.ICON, 
                              placement=drawable.Text_Placement.TOP,
                                       minSize = minSize,
                              overlay = OrderedDict() )
        SrvStyle = drawable.Draw_Style(icon=srvIcon,
                              font=font, 
                              method=drawable.Draw_Method.ICON, 
                              placement=drawable.Text_Placement.TOP,
                                       minSize = minSize,
                              overlay = OrderedDict() )
        CompStyle = drawable.Draw_Style(icon=None,
                               font=font, 
                               method=drawable.Draw_Method.ROUND_RECT, 
                               placement=drawable.Text_Placement.TOP,
                                       minSize = minSize,
                                        overlay = OrderedDict([('fillColor','STEEL BLUE')]) )
        TmrStyle = drawable.Draw_Style(icon=tmrIcon,
                              font=font, 
                              method=drawable.Draw_Method.ICON, 
                                       minSize = minSize,
                              placement=drawable.Text_Placement.RIGHT,
                              overlay = OrderedDict() )
        PubStyle = drawable.Draw_Style(icon=pubIcon,
                              font=font, 
                              method=drawable.Draw_Method.ICON, 
                                       minSize = minSize,
                              placement=drawable.Text_Placement.RIGHT,
                              overlay = OrderedDict() )
        SubStyle = drawable.Draw_Style(icon=subIcon,
                              font=font, 
                              method=drawable.Draw_Method.ICON, 
                                       minSize = minSize,
                              placement=drawable.Text_Placement.RIGHT,
                              overlay = OrderedDict() )
        CliStyle = drawable.Draw_Style(icon=clientIcon,
                              font=font, 
                              method=drawable.Draw_Method.ICON, 
                                       minSize = minSize,
                              placement=drawable.Text_Placement.RIGHT,
                              overlay = OrderedDict() )
        SerStyle = drawable.Draw_Style(icon=serverIcon,
                              font=font, 
                              method=drawable.Draw_Method.ICON, 
                                       minSize = minSize,
                              placement=drawable.Text_Placement.RIGHT,
                                       overlay = OrderedDict() )
        NodeStyle = drawable.Draw_Style(icon=None,
                               font=font, 
                               method=drawable.Draw_Method.ROUND_RECT, 
                                       minSize = minSize,
                               placement=drawable.Text_Placement.TOP,
                                        overlay = OrderedDict([('fillColor','TURQUOISE')]) )
        CompInstStyle = drawable.Draw_Style(icon=compInstIcon,
                                   font=font, 
                                   method=drawable.Draw_Method.ICON, 
                                       minSize = minSize,
                                   placement=drawable.Text_Placement.RIGHT,
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
        self.styleDict["hardware_configuration"] = HardwareStyle
        self.styleDict["host"] = HostStyle
        self.styleDict["deployment"] = DeploymentStyle
        self.styleDict["host_instance"] = HostInstStyle
        self.styleDict["node_instance"] = NodeInstStyle

def main():
    ex = wx.App()
    Example(None)
    ex.MainLoop()    


if __name__ == '__main__':
    main()
