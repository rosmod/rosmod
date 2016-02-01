import wx
import os,sys
from collections import OrderedDict
from pkg_resources import resource_filename

#from aspect import *

class TBItem:
    def __init__(self, name, icon, helpStr, func, _id = wx.ID_ANY):
        self.name = name
        self.icon = icon
        self.helpStr = helpStr
        self.func = func
        self._id = _id

aspectToolbarDict = OrderedDict()

class TBInfo():
    def __init__(self,name, obj):
        self.name = name
        self.obj = obj

def BuildToolbar(self):
    aspectToolbarDict["Software"] = [
        TBItem("create",resource_filename('icons.toolbar', 'new.png'),"New Package",lambda e : self.OnAspectCreate("Package",e)),
        TBItem("delete",resource_filename('icons.toolbar', 'delete.png'),"Remove Package", self.OnAspectDelete),
        TBItem("generate",resource_filename('icons.toolbar', 'generate.png'),"Generate Workspace", self.OnPackageGenerate),
        TBItem("build",resource_filename('icons.toolbar', 'build.png'),"Build Workspace", self.OnPackageBuild)
    ]
    aspectToolbarDict["Hardware"] = [
        TBItem("create",resource_filename('icons.toolbar', 'new.png'),"New Hardware",lambda e : self.OnAspectCreate("rhw",e)),
        TBItem("delete",resource_filename('icons.toolbar', 'delete.png'),"Remove Hardware", self.OnAspectDelete),
    ]
    aspectToolbarDict["Deployment"] = [
        TBItem("create",resource_filename('icons.toolbar', 'new.png'),"New Deployment",lambda e : self.OnAspectCreate("rdp",e)),
        TBItem("delete",resource_filename('icons.toolbar', 'delete.png'),"Remove Deployment", self.OnAspectDelete),
        TBItem("analyze",resource_filename('icons.toolbar', 'analyze_network.png'),"Run Network Analysis Tool", self.OnDeploymentAnalyze),
        TBItem("timing",resource_filename('icons.toolbar', 'analyze_timing.png'), "Generate Timing Analysis Model", self.OnDeploymentTiming),
        TBItem("generate",resource_filename('icons.toolbar', 'generate.png'),"Generate Deployment XML Files", self.OnDeploymentGenerate),
        TBItem("move",resource_filename('icons.toolbar', 'copy.png'),"Copy Deployment Files", self.OnDeploymentMove),
        TBItem("deploy",resource_filename('icons.toolbar', 'start.png'),"Start Deployment", self.OnDeploymentDeploy),
        TBItem("stop",resource_filename('icons.toolbar', 'stop.png'),"Stop Deployment", self.OnDeploymentStop),
        TBItem("run",resource_filename('icons.toolbar', 'run.png'),"Run <Command> on all Hardware Devices", self.OnDeploymentRun)
    ]

    self.toolbar = self.CreateToolBar()
    # file operations
    self.tb_new = self.toolbar.AddLabelTool(
        wx.ID_NEW, '', wx.Bitmap(resource_filename('icons.toolbar', 'new.png')), shortHelp="New")
    self.tb_open = self.toolbar.AddLabelTool(
        wx.ID_OPEN, '', wx.Bitmap(resource_filename('icons.toolbar', 'open.png')), shortHelp="Open")
    self.tb_save = self.toolbar.AddLabelTool(
        wx.ID_SAVE, '', wx.Bitmap(resource_filename('icons.toolbar', 'save.png')), shortHelp="Save")
    self.tb_print = self.toolbar.AddLabelTool(
        wx.ID_ANY, '', wx.Bitmap(resource_filename('icons.toolbar', 'print.png')), shortHelp="Print Page to File")
    self.toolbar.AddSeparator()
    # undo/redo
    self.tb_undo = self.toolbar.AddLabelTool(
        wx.ID_UNDO, '', wx.Bitmap(resource_filename('icons.toolbar', 'undo.png')), shortHelp="Undo")
    self.tb_redo = self.toolbar.AddLabelTool(
        wx.ID_REDO, '', wx.Bitmap(resource_filename('icons.toolbar', 'redo.png')), shortHelp="Redo")
    self.toolbar.EnableTool(wx.ID_UNDO, False)
    self.toolbar.EnableTool(wx.ID_REDO, False)
    self.toolbar.AddSeparator()
    self.tb_term = self.toolbar.AddLabelTool(
        wx.ID_ANY, '', wx.Bitmap(resource_filename('icons.toolbar', 'terminal.png')), shortHelp="Terminal")
    self.toolbar.AddSeparator()
    self.Bind(wx.EVT_TOOL, self.OnNew, self.tb_new)
    self.Bind(wx.EVT_TOOL, self.OnOpen, self.tb_open)
    self.Bind(wx.EVT_TOOL, self.OnSave, self.tb_save)
    self.Bind(wx.EVT_TOOL, self.OnPrint, self.tb_print)
    self.Bind(wx.EVT_TOOL, self.OnUndo, self.tb_undo)
    self.Bind(wx.EVT_TOOL, self.OnRedo, self.tb_redo)
    self.Bind(wx.EVT_TOOL, self.OnTerminal, self.tb_term)


def AddAspectToolbar(self,kind):
    tbi = TBInfo( name="label",
                  obj = wx.StaticText( self.toolbar, wx.ID_ANY, kind+":"))
    self.toolbar.AddControl(tbi.obj)
    self.AspectInfos[kind].AddTBInfo(tbi)
    for item in aspectToolbarDict[kind]:
        tbi = TBInfo( name = item.name,
                      obj = self.toolbar.AddTool( item._id,
                                                  bitmap = wx.Bitmap(item.icon),
                                                  shortHelpString = item.helpStr))
        self.AspectInfos[kind].AddTBInfo(tbi)
        self.Bind(wx.EVT_TOOL, item.func, tbi.obj)
    self.toolbar.Realize()

def RemoveAspectToolbar(self,kind):
    for name,tbinfo in self.AspectInfos[kind].toolbarButtons.iteritems():
        self.toolbar.RemoveTool(tbinfo.obj.GetId())
        self.AspectInfos[kind].DelTBInfo(name)
    self.toolbar.Realize()

