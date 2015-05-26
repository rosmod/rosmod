import wx
import os,sys
from collections import OrderedDict

#from aspect import *

exeName = sys.argv[0]
dirName = os.path.abspath(exeName)
head,tail = os.path.split(dirName)
editorPath=head
rootIconPath= editorPath + '/icons'
toolbarIconPath= rootIconPath + '/toolbar'

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
        TBItem("create","/new.png","New Package",lambda e : self.OnAspectCreate("Package",e)),
        TBItem("delete","/delete.png","Remove Package", self.OnAspectDelete),
        TBItem("generate","/generate.png","Generate Workspace", self.OnPackageGenerate)
    ]
    aspectToolbarDict["Hardware"] = [
        TBItem("create","/new.png","New Hardware",lambda e : self.OnAspectCreate("rhw",e)),
        TBItem("delete","/delete.png","Remove Hardware", self.OnAspectDelete),
    ]
    aspectToolbarDict["Deployment"] = [
        TBItem("create","/new.png","New Deployment",lambda e : self.OnAspectCreate("rdp",e)),
        TBItem("delete","/delete.png","Remove Deployment", self.OnAspectDelete),
        TBItem("analyze","/analyze_network.png","Run Network Analysis Tool", self.OnDeploymentAnalyze),
        TBItem("timing", "/analyze_timing.png", "Generate Timing Analysis Model", self.OnDeploymentTiming),
        TBItem("generate","/generate.png","Generate Deployment XML Files", self.OnDeploymentGenerate),
        TBItem("move","/copy.png","Copy Deployment Files", self.OnDeploymentMove),
        TBItem("roscore", "/roscore.png", "Start ROSCORE", self.OnDeploymentROSCORE),
        TBItem("deploy","/start.png","Start Deployment", self.OnDeploymentDeploy),
        TBItem("stop","/stop.png","Stop Deployment", self.OnDeploymentStop),
        TBItem("run","/run.png","Run <Command> on all Hardware Devices", self.OnDeploymentRun)
    ]

    self.toolbar = self.CreateToolBar()
    # file operations
    self.tb_new = self.toolbar.AddLabelTool(
        wx.ID_NEW, '', wx.Bitmap(toolbarIconPath + '/new.png'), shortHelp="New")
    self.tb_open = self.toolbar.AddLabelTool(
        wx.ID_OPEN, '', wx.Bitmap(toolbarIconPath + '/open.png'), shortHelp="Open")
    self.tb_save = self.toolbar.AddLabelTool(
        wx.ID_SAVE, '', wx.Bitmap(toolbarIconPath + '/save.png'), shortHelp="Save")
    self.tb_print = self.toolbar.AddLabelTool(
        wx.ID_ANY, '', wx.Bitmap(toolbarIconPath + '/print.png'), shortHelp="Print Page to File")
    self.toolbar.AddSeparator()
    # undo/redo
    self.tb_undo = self.toolbar.AddLabelTool(
        wx.ID_UNDO, '', wx.Bitmap(toolbarIconPath + '/undo.png'), shortHelp="Undo")
    self.tb_redo = self.toolbar.AddLabelTool(
        wx.ID_REDO, '', wx.Bitmap(toolbarIconPath + '/redo.png'), shortHelp="Redo")
    self.toolbar.EnableTool(wx.ID_UNDO, False)
    self.toolbar.EnableTool(wx.ID_REDO, False)
    self.toolbar.AddSeparator()
    self.tb_term = self.toolbar.AddLabelTool(
        wx.ID_ANY, '', wx.Bitmap(toolbarIconPath + '/terminal.png'), shortHelp="Terminal")
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
                                                  bitmap = wx.Bitmap(toolbarIconPath + item.icon),
                                                  shortHelpString = item.helpStr))
        self.AspectInfos[kind].AddTBInfo(tbi)
        self.Bind(wx.EVT_TOOL, item.func, tbi.obj)
    self.toolbar.Realize()

def RemoveAspectToolbar(self,kind):
    for name,tbinfo in self.AspectInfos[kind].toolbarButtons.iteritems():
        self.toolbar.RemoveTool(tbinfo.obj.GetId())
        self.AspectInfos[kind].DelTBInfo(name)
    self.toolbar.Realize()

