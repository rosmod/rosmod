
import wx
import wx.lib.agw.flatnotebook as fnb
import wx.stc as stc
from collections import OrderedDict

from wx.lib.floatcanvas import NavCanvas, FloatCanvas, Resources, Utilities
try:
    import numpy
    import numpy.random as RandomArray
    haveNumpy = True
except ImportError:
            haveNumpy = False
            errorText = (
            "The FloatCanvas requires the numpy module, version 1.* \n\n"
            "You can get info about it at:\n"
            "http://numpy.scipy.org/\n\n"
            )

import drawable
from toolbar import *

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

def InitAspects(self):
    self.activeAspect = None
    self.activeAspectInfo = None
    self.PackageAspect = None
    self.PackageAspectInfo = None
    self.HardwareAspect = None
    self.HardwareAspectInfo = None
    self.DeploymentAspect = None
    self.DeploymentAspectInfo = None
    BuildAspects(self)

'''
Aspect Menubar Menu functions
'''
def HideAllAspects(self):
    self.PackageAspect.Hide()
    self.HardwareAspect.Hide()
    self.DeploymentAspect.Hide()
    RemovePackageAspectToolbar(self)
    RemoveHardwareAspectToolbar(self)
    RemoveDeploymentAspectToolbar(self)

def ShowAspect(self,aspect):
    if self.shvw.IsChecked():
        aspect.Show()
        self.viewSplitter.ReplaceWindow(self.activeAspect,aspect)
        self.activeAspect = aspect

def OnPackageAspect(self, e):
    self.activeAspectInfo = self.PackageAspectInfo
    HideAllAspects(self)
    ShowAspect(self,self.PackageAspect)
    AddPackageAspectToolbar(self)
    pageChange(self,None)

def OnHardwareAspect(self, e):
    self.activeAspectInfo = self.HardwareAspectInfo
    HideAllAspects(self)
    ShowAspect(self,self.HardwareAspect)
    AddHardwareAspectToolbar(self)
    pageChange(self,None)

def OnDeploymentAspect(self, e):
    self.activeAspectInfo = self.DeploymentAspectInfo
    HideAllAspects(self)
    ShowAspect(self,self.DeploymentAspect)
    AddDeploymentAspectToolbar(self)
    pageChange(self,None)

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
    BuildPackageAspect(self)
    BuildHardwareAspect(self)
    BuildDeploymentAspect(self)
    self.activeAspect = self.PackageAspect
    self.activeAspectInfo = self.PackageAspectInfo

'''
Hardware Aspect: panel with toolbar for configuring system hardware (hosts)
'''
def BuildHardwareAspect(self):
    self.HardwareAspect = fnb.FlatNotebook(self.viewSplitter,
                                           agwStyle=fnb.FNB_NODRAG|fnb.FNB_NO_X_BUTTON)
    self.HardwareAspect.Hide()
    self.HardwareAspect.Bind(fnb.EVT_FLATNOTEBOOK_PAGE_CHANGED, lambda e : OnPageChanged(self,e))
    self.HardwareAspect.Bind(fnb.EVT_FLATNOTEBOOK_PAGE_CHANGING, lambda e : OnPageChanging(self,e))
    
'''
Deployment Aspect: panel with toolbar and notebook for configuring and managing
node deployment onto hosts (and roscore deployment)
'''        
def BuildDeploymentAspect(self):
    self.DeploymentAspect = fnb.FlatNotebook(self.viewSplitter,
                                             agwStyle=fnb.FNB_NODRAG|fnb.FNB_NO_X_BUTTON)
    self.DeploymentAspect.Hide()
    self.DeploymentAspect.Bind(fnb.EVT_FLATNOTEBOOK_PAGE_CHANGED, lambda e : OnPageChanged(self,e))
    self.DeploymentAspect.Bind(fnb.EVT_FLATNOTEBOOK_PAGE_CHANGING, lambda e : OnPageChanging(self,e))

'''
Package Aspect: panel with toolbar and notebook for managing packages
'''
def BuildPackageAspect(self):
    self.PackageAspect = fnb.FlatNotebook(self.viewSplitter, wx.ID_ANY,
                                          agwStyle=fnb.FNB_NODRAG|fnb.FNB_NO_X_BUTTON)
    self.PackageAspect.Bind(fnb.EVT_FLATNOTEBOOK_PAGE_CHANGED, lambda e : OnPageChanged(self,e))
    self.PackageAspect.Bind(fnb.EVT_FLATNOTEBOOK_PAGE_CHANGING, lambda e : OnPageChanging(self,e))



def BuildAspectPages(self):
    BuildPackageAspectPages(self)
    BuildHardwareAspectPages(self)
    BuildDeploymentAspectPages(self)
def BuildPackageAspectPages(self):
    self.PackageAspect.DeleteAllPages()
    for pkg in self.project.getChildrenByKind("Package"):
        BuildModelPage(self, parent = self.PackageAspect,
                             model = pkg,
                             aspectInfo = self.PackageAspectInfo)
    BuildModelPage(self, parent = self.PackageAspect,
                         model = self.project.getChildrenByKind("rml")[0],
                         aspectInfo = self.PackageAspectInfo)
    self.PackageAspect.AdvanceSelection()
def BuildHardwareAspectPages(self):
    self.HardwareAspect.DeleteAllPages()
    for hw in self.project.getChildrenByKind("rhw"):
        BuildModelPage(self, parent = self.HardwareAspect,
                             model = hw,
                             aspectInfo = self.HardwareAspectInfo)
    self.DeploymentAspect.AdvanceSelection()
def BuildDeploymentAspectPages(self):
    self.DeploymentAspect.DeleteAllPages()
    for dep in self.project.getChildrenByKind("rdp"):
        BuildModelPage(self, parent = self.DeploymentAspect,
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
    #canvas.ZoomToBB()
    canvas.Zoom(1,model.textPosition)


'''
Package Aspect Functions
'''
def pageChange(self, event):
    self.activeAspect.Refresh()
    if self.activeAspect == self.PackageAspect:
        PackageAspectPageChange(self,event)
    elif self.activeAspect == self.HardwareAspect:
        HardwareAspectPageChange(self,event)
    elif self.activeAspect == self.DeploymentAspect:
        DeploymentAspectPageChange(self,event)

def PackageAspectPageChange(self, event):
    sel = self.activeAspect.GetSelection()
    numPages = self.activeAspect.GetPageCount()
    if sel >= 0:
        pageName = self.activeAspect.GetPageText(sel)
        info = self.activeAspectInfo.GetPageInfo(pageName)
        pkg = info.obj
        canvas = info.canvas
        deleteTBID = self.activeAspectInfo.GetTBInfo("delete").obj.GetId()
        if pkg.kind == 'rml':
            self.toolbar.EnableTool(deleteTBID, False)
        else:
            self.toolbar.EnableTool(deleteTBID, True)
        drawable.Configure(pkg,self.styleDict)
        self.DrawModel(pkg,canvas)

def HardwareAspectPageChange(self, event):
    sel = self.activeAspect.GetSelection()
    numPages = self.activeAspect.GetPageCount()
    deleteTBID = self.activeAspectInfo.GetTBInfo("delete").obj.GetId()
    if numPages > 1:
        self.toolbar.EnableTool(deleteTBID, True)
    else:
        self.toolbar.EnableTool(deleteTBID, False)
    if sel >= 0:
        pageName = self.activeAspect.GetPageText(sel)
        info = self.activeAspectInfo.GetPageInfo(pageName)
        hw = info.obj
        canvas = info.canvas
        drawable.Configure(hw,self.styleDict)
        self.DrawModel(hw,canvas)

def DeploymentAspectPageChange(self, event):
    sel = self.activeAspect.GetSelection()
    numPages = self.activeAspect.GetPageCount()
    deleteTBID = self.activeAspectInfo.GetTBInfo("delete").obj.GetId()
    if numPages > 1:
        self.toolbar.EnableTool(deleteTBID, True)
    else:
        self.toolbar.EnableTool(deleteTBID, False)
    if sel >= 0:
        pageName = self.activeAspect.GetPageText(sel)
        info = self.activeAspectInfo.GetPageInfo(pageName)
        dep = info.obj
        canvas = info.canvas
        drawable.Configure(dep,self.styleDict)
        self.DrawModel(dep,canvas)
        
def OnPageChanged(self, event):
    pageChange(self,event)
    event.Skip()
    
def OnPageChanging(self, event):
    pageChange(self,event)
    event.Skip()
