
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
    def __init__(self, name, obj, canvas, msgWindow, deletable=True):
        self.name = name
        self.obj = obj
        self.canvas = canvas
        self.msgWindow = msgWindow
        self.deletable = deletable

class AspectInfo():
    def __init__(self, kind, deletableTypes):
        self.kind = kind
        self.deletableTypes = deletableTypes
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
    self.Aspects = OrderedDict()
    self.AspectInfos = OrderedDict()
    BuildAspects(self)

'''
Aspect Menubar Menu functions
'''
def HideAllAspects(self):
    for key,value in self.Aspects.iteritems():
        value.Hide()
        RemoveAspectToolbar(self,key)

def ShowAspect(self,aspect):
    if self.shvw.IsChecked():
        aspect.Show()
        self.viewSplitter.ReplaceWindow(self.activeAspect,aspect)
        self.activeAspect = aspect

def OnAspect(self,kind,e):
    self.activeAspectInfo = self.AspectInfos[kind]
    HideAllAspects(self)
    ShowAspect(self,self.Aspects[kind])
    AddAspectToolbar(self,kind)
    pageChange(self,None)

'''
Build all the Aspects required for ROSMOD:
* Packages aspect : used for setting up mgs,srv,comp,node,etc.
* Hardware aspect : used for configure the system hardware (hosts)
* Deployment aspect : used for assigning nodes to hosts
'''
def BuildAspects(self):
    self.AspectTypes = OrderedDict()
    self.AspectTypes["Software"] = ["Package","rml"]
    self.AspectTypes["Hardware"] = ["rhw"]
    self.AspectTypes["Deployment"] = ["rdp"]

    self.AspectInfos["Software"] = AspectInfo("Software",["Package"])
    self.AspectInfos["Hardware"] = AspectInfo("Hardware",["rhw"])
    self.AspectInfos["Deployment"] = AspectInfo("Deployment",["rdp"])
    for key in self.AspectInfos.keys():
        BuildAspect(self,key)
    self.activeAspect = self.Aspects["Software"]
    self.activeAspectInfo = self.AspectInfos["Software"]

def BuildAspect(self,kind):
    self.Aspects[kind] = fnb.FlatNotebook(self.viewSplitter,
                                          agwStyle=fnb.FNB_NODRAG|fnb.FNB_NO_X_BUTTON)
    self.Aspects[kind].Hide()
    self.Aspects[kind].Bind(fnb.EVT_FLATNOTEBOOK_PAGE_CHANGED, lambda e : OnPageChanged(self,e))
    self.Aspects[kind].Bind(fnb.EVT_FLATNOTEBOOK_PAGE_CHANGING, lambda e : OnPageChanging(self,e))

def BuildAspectPages(self):
    for key in self.Aspects.keys():
        kinds = self.AspectTypes[key]
        for kind in kinds:
            pages = self.project.getChildrenByKind(kind)
            for page in pages:
                BuildModelPage(self, parent = self.Aspects[key],
                               model = page,
                               aspectInfo = self.AspectInfos[key])
        self.Aspects[key].AdvanceSelection()

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
    if model.kind not in aspectInfo.deletableTypes:
        pageInfo.deletable = False
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
    AspectPageChange(self,self.activeAspectInfo.kind,event)

def AspectPageChange(self,kind,event):
    sel = self.activeAspect.GetSelection()
    numPages = self.activeAspect.GetPageCount()
    deleteTBID = self.activeAspectInfo.GetTBInfo("delete").obj.GetId()
    info = None
    if sel >= 0:
        pageName = self.activeAspect.GetPageText(sel)
        info = self.activeAspectInfo.GetPageInfo(pageName)
        model = info.obj
        canvas = info.canvas
        drawable.Configure(model,self.styleDict)
        self.DrawModel(model,canvas)
    if numPages > 1 and info != None and info.deletable == True:
        self.toolbar.EnableTool(deleteTBID, True)
    else:
        self.toolbar.EnableTool(deleteTBID, False)

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
        
def OnPageChanged(self, event):
    pageChange(self,event)
    event.Skip()
    
def OnPageChanging(self, event):
    pageChange(self,event)
    event.Skip()
