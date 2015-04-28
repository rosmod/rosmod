import wx
import os,sys

from aspect import *

exeName = sys.argv[0]
dirName = os.path.abspath(exeName)
head,tail = os.path.split(dirName)
editorPath=head
rootIconPath= editorPath + '/icons'
toolbarIconPath= rootIconPath + '/toolbar'

def BuildToolbar(self):
    self.toolbar = self.CreateToolBar()
    # file operations
    self.tb_new = self.toolbar.AddLabelTool(
        wx.ID_NEW, '', wx.Bitmap(toolbarIconPath + '/tnew.png'), shortHelp="New")
    self.tb_open = self.toolbar.AddLabelTool(
        wx.ID_OPEN, '', wx.Bitmap(toolbarIconPath + '/topen.png'), shortHelp="Open")
    self.tb_save = self.toolbar.AddLabelTool(
        wx.ID_SAVE, '', wx.Bitmap(toolbarIconPath + '/tsave.png'), shortHelp="Save")
    self.tb_print = self.toolbar.AddLabelTool(
        wx.ID_ANY, '', wx.Bitmap(toolbarIconPath + '/tprint.png'), shortHelp="Print Page to File")
    self.toolbar.AddSeparator()
    # undo/redo
    self.tb_undo = self.toolbar.AddLabelTool(
        wx.ID_UNDO, '', wx.Bitmap(toolbarIconPath + '/tundo.png'), shortHelp="Undo")
    self.tb_redo = self.toolbar.AddLabelTool(
        wx.ID_REDO, '', wx.Bitmap(toolbarIconPath + '/tredo.png'), shortHelp="Redo")
    self.toolbar.EnableTool(wx.ID_UNDO, False)
    self.toolbar.EnableTool(wx.ID_REDO, False)
    self.toolbar.AddSeparator()
    self.tb_term = self.toolbar.AddLabelTool(
        wx.ID_ANY, '', wx.Bitmap(toolbarIconPath + '/tterm.png'), shortHelp="Terminal")
    self.toolbar.AddSeparator()
    self.Bind(wx.EVT_TOOL, self.OnNew, self.tb_new)
    self.Bind(wx.EVT_TOOL, self.OnOpen, self.tb_open)
    self.Bind(wx.EVT_TOOL, self.OnSave, self.tb_save)
    self.Bind(wx.EVT_TOOL, self.OnPrint, self.tb_print)
    self.Bind(wx.EVT_TOOL, self.OnUndo, self.tb_undo)
    self.Bind(wx.EVT_TOOL, self.OnRedo, self.tb_redo)
    self.Bind(wx.EVT_TOOL, self.OnTerminal, self.tb_term)

def AddHardwareAspectToolbar(self):
    labelTBinfo = TBInfo( name="label",
                          obj = wx.StaticText( self.toolbar, wx.ID_ANY, "Hardware:"))
    self.toolbar.AddControl(labelTBinfo.obj)
    createTBinfo = TBInfo(
        name="create",
        obj=self.toolbar.AddTool(wx.ID_ANY,
                                 bitmap = wx.Bitmap(toolbarIconPath + '/tnew.png'), 
                                 shortHelpString="New Hardware Configuration"))
    deleteTBinfo = TBInfo(
        name="delete",
        obj=self.toolbar.AddTool(wx.ID_ANY,
                                 bitmap = wx.Bitmap(toolbarIconPath + '/texit.png'), 
                                 shortHelpString="Remove Hardware Configuration"))
    self.HardwareAspectInfo.AddTBInfo(labelTBinfo)
    self.HardwareAspectInfo.AddTBInfo(createTBinfo)
    self.HardwareAspectInfo.AddTBInfo(deleteTBinfo)
    self.Bind(wx.EVT_TOOL, self.OnHardwareCreate, createTBinfo.obj)
    self.Bind(wx.EVT_TOOL, self.OnHardwareDelete, deleteTBinfo.obj)
    self.toolbar.EnableTool(deleteTBinfo.obj.GetId(),False)
    self.toolbar.Realize()

def RemoveHardwareAspectToolbar(self):
    for name,tbinfo in self.HardwareAspectInfo.toolbarButtons.iteritems():
        self.toolbar.RemoveTool(tbinfo.obj.GetId())
        self.HardwareAspectInfo.DelTBInfo(name)
    self.toolbar.Realize()


def AddDeploymentAspectToolbar(self):
    labelTBinfo = TBInfo( name="label",
                              obj = wx.StaticText( self.toolbar, wx.ID_ANY, "Deployment:"))
    self.toolbar.AddControl(labelTBinfo.obj)
    createTBinfo = TBInfo( name="create",
                           obj=self.toolbar.AddTool(wx.ID_ANY,
                                                    bitmap = wx.Bitmap(toolbarIconPath + '/tnew.png'), 
                                                    shortHelpString="New Deployment"))
    deleteTBinfo = TBInfo( name="delete",
                           obj=self.toolbar.AddTool(wx.ID_ANY,
                                                    bitmap = wx.Bitmap(toolbarIconPath + '/texit.png'), 
                                                    shortHelpString="Remove Deployment"))
    generateTBinfo = TBInfo (name='generate',
                             obj=self.toolbar.AddTool(wx.ID_ANY,
                                                      bitmap = wx.Bitmap(toolbarIconPath + '/tgenerate.png'),
                                                      shortHelpString="Generate Deployment Files"))
    moveTBinfo = TBInfo (name='move',
                         obj=self.toolbar.AddTool(wx.ID_ANY,
                                                  bitmap = wx.Bitmap(toolbarIconPath + '/tmove.png'),
                                                  shortHelpString="Copy Deployment Files"))
    deployTBinfo = TBInfo( name='deploy',
                           obj=self.toolbar.AddTool(wx.ID_ANY,
                                                    bitmap = wx.Bitmap(toolbarIconPath + '/tdeploy.png'),
                                                    shortHelpString="Deploy System"))
    stopTBinfo = TBInfo( name='stop',
                         obj=self.toolbar.AddTool(wx.ID_ANY,
                                                  bitmap = wx.Bitmap(toolbarIconPath + '/tstop.png'),
                                                  shortHelpString="Stop Deployed System"))
    runTBinfo = TBInfo( name='run',
                        obj=self.toolbar.AddTool(wx.ID_ANY,
                                                 bitmap = wx.Bitmap(toolbarIconPath + '/trun.png'),
                                                 shortHelpString="Run <Command> on All Hosts"))
    self.DeploymentAspectInfo.AddTBInfo(labelTBinfo)
    self.DeploymentAspectInfo.AddTBInfo(createTBinfo)
    self.DeploymentAspectInfo.AddTBInfo(deleteTBinfo)
    self.DeploymentAspectInfo.AddTBInfo(generateTBinfo)
    self.DeploymentAspectInfo.AddTBInfo(moveTBinfo)
    self.DeploymentAspectInfo.AddTBInfo(deployTBinfo)
    self.DeploymentAspectInfo.AddTBInfo(stopTBinfo)
    self.DeploymentAspectInfo.AddTBInfo(runTBinfo)
    self.Bind(wx.EVT_TOOL, self.OnDeploymentCreate, createTBinfo.obj)
    self.Bind(wx.EVT_TOOL, self.OnDeploymentDelete, deleteTBinfo.obj)
    self.Bind(wx.EVT_TOOL, self.OnDeploymentGenerate, generateTBinfo.obj)
    self.Bind(wx.EVT_TOOL, self.OnDeploymentMove, moveTBinfo.obj)
    self.Bind(wx.EVT_TOOL, self.OnDeploymentDeploy, deployTBinfo.obj)
    self.Bind(wx.EVT_TOOL, self.OnDeploymentStop, stopTBinfo.obj)
    self.Bind(wx.EVT_TOOL, self.OnDeploymentRun, runTBinfo.obj)
    self.toolbar.EnableTool(deleteTBinfo.obj.GetId(),False)
    self.toolbar.Realize()

def RemoveDeploymentAspectToolbar(self):
    for name,tbinfo in self.DeploymentAspectInfo.toolbarButtons.iteritems():
        self.toolbar.RemoveTool(tbinfo.obj.GetId())
        self.DeploymentAspectInfo.DelTBInfo(name)
    self.toolbar.Realize()


def AddPackageAspectToolbar(self):
    labelTBinfo = TBInfo( name="label",
                          obj = wx.StaticText( self.toolbar, wx.ID_ANY, "Software:"))
    self.toolbar.AddControl(labelTBinfo.obj)
    createTBinfo = TBInfo(
        name="create",
        obj=self.toolbar.AddTool(wx.ID_ANY,
                                 bitmap = wx.Bitmap(toolbarIconPath + '/tnew.png'), 
                                 shortHelpString="New Package"))
    deleteTBinfo = TBInfo(
        name="delete",
        obj=self.toolbar.AddTool(wx.ID_ANY,
                                 bitmap = wx.Bitmap(toolbarIconPath + '/texit.png'), 
                                 shortHelpString="Remove Package"))
    generateTBinfo = TBInfo(
        name="generate",
        obj=self.toolbar.AddTool(wx.ID_ANY,
                                 bitmap = wx.Bitmap(toolbarIconPath + '/tgenerate.png'), 
                                 shortHelpString="Generate Software"))
    self.PackageAspectInfo.AddTBInfo(labelTBinfo)
    self.PackageAspectInfo.AddTBInfo(createTBinfo)
    self.PackageAspectInfo.AddTBInfo(deleteTBinfo)
    self.PackageAspectInfo.AddTBInfo(generateTBinfo)
    self.Bind(wx.EVT_TOOL, self.OnPackageCreate, createTBinfo.obj)
    self.Bind(wx.EVT_TOOL, self.OnPackageDelete, deleteTBinfo.obj)
    self.Bind(wx.EVT_TOOL, self.OnPackageGenerate, generateTBinfo.obj)
    self.toolbar.EnableTool(deleteTBinfo.obj.GetId(),False)
    self.toolbar.Realize()
def RemovePackageAspectToolbar(self):
    for name,tbinfo in self.PackageAspectInfo.toolbarButtons.iteritems():
        self.toolbar.RemoveTool(tbinfo.obj.GetId())
        self.PackageAspectInfo.DelTBInfo(name)
    self.toolbar.Realize()
