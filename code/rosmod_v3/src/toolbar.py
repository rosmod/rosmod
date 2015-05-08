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
        TBItem("create","/tnew.png","New Package",lambda e : self.OnAspectCreate("Package",e)),
        TBItem("delete","/texit.png","Remove Package", self.OnAspectDelete),
        TBItem("generate","/tgenerate.png","Generate Executable Files", self.OnPackageGenerate)
    ]
    aspectToolbarDict["Hardware"] = [
        TBItem("create","/tnew.png","New Hardware",lambda e : self.OnAspectCreate("rhw",e)),
        TBItem("delete","/texit.png","Remove Hardware", self.OnAspectDelete),
    ]
    aspectToolbarDict["Deployment"] = [
        TBItem("create","/tnew.png","New Deployment",lambda e : self.OnAspectCreate("rdp",e)),
        TBItem("delete","/texit.png","Remove Deployment", self.OnAspectDelete),
        TBItem("analyze","/tanalyze.png","Analyze Deployment timing and network properties", self.OnDeploymentAnalyze),
        TBItem("generate","/tgenerate.png","Generate Deployment Files", self.OnDeploymentGenerate),
        TBItem("move","/tmove.png","Copy Deployment Files", self.OnDeploymentMove),
        TBItem("deploy","/tdeploy.png","Deploy onto System", self.OnDeploymentDeploy),
        TBItem("stop","/tstop.png","Stop Deployed System", self.OnDeploymentStop),
        TBItem("run","/trun.png","Run <Command> on All Hosts", self.OnDeploymentRun)
    ]

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

