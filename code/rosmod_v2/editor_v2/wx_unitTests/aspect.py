#!/usr/bin/python

import wx

class AspectTab(wx.Panel):
    def __init__(self,parent):
        wx.Panel.__init__(self,parent=parent, id=wx.ID_ANY)

class Aspect(wx.Panel):
    def __init__(self,parent,model=None):
        wx.Panel.__init__(self,parent)

        self.toolbar = wx.ToolBar(self)
        self.BuildToolbar()
        self.BuildNotebook()

        self.boxSizer = wx.BoxSizer( wx.VERTICAL )     
        self.boxSizer.Add(self.toolbar, 0, wx.ALL | wx.ALIGN_LEFT | wx.EXPAND, 4)
        self.boxSizer.Add(self.notebook, 1, wx.ALL | wx.ALIGN_LEFT | wx.EXPAND, 4)
        self.SetSizer(self.boxSizer)

    def BuildToolbar(self):
        self.tcreate_ID = wx.NewId()
        self.tdelete_ID = wx.NewId()
        self.tcreate = self.toolbar.AddLabelTool(self.tcreate_ID, '', wx.Bitmap('tnew.png'), shortHelp="New Package")
        self.tdelete = self.toolbar.AddLabelTool(self.tdelete_ID, '', wx.Bitmap('texit.png'), shortHelp="Remove Package")
        self.RegisterToolbarEvents()

    def RegisterToolbarEvents(self):
        wx.EVT_TOOL( self, self.tcreate_ID, self.OnCreate)
        wx.EVT_TOOL( self, self.tdelete_ID, self.OnDelete)

    def BuildNotebook(self):
        self.notebook = wx.Notebook(self, id=wx.ID_ANY, style=
                                     wx.BK_DEFAULT
                                     #wx.BK_TOP 
                                     #wx.BK_BOTTOM
                                     #wx.BK_LEFT
                                     #wx.BK_RIGHT
                                 )
        self.notebook.AddPage(AspectTab(self.notebook), "Package 1")
        self.notebook.AddPage(AspectTab(self.notebook), "Package 2")
        self.notebook.AddPage(AspectTab(self.notebook), "Package 3")
        self.notebook.AddPage(AspectTab(self.notebook), "All Packages")
        self.notebook.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGED, self.OnPageChanged)
        self.notebook.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGING, self.OnPageChanging)

    def OnCreate(self, e):
        newTab = AspectTab(self.notebook)
        newTabName = "New Package!"
        numPages = self.notebook.GetPageCount()
        self.notebook.InsertPage(numPages-1,newTab, newTabName)
        self.notebook.SetSelection(numPages-1)
    
    def OnDelete(self, e):
        selectedPage = self.notebook.GetSelection()
        numPages = self.notebook.GetPageCount()
        if selectedPage != numPages - 1:
            self.notebook.DeletePage(selectedPage)
 
    def OnPageChanged(self, event):
        old = event.GetOldSelection()
        new = event.GetSelection()
        sel = self.notebook.GetSelection()
        numPages = self.notebook.GetPageCount()
        if new == numPages - 1:
            self.toolbar.EnableTool(self.tdelete.GetId(), False)
        else:
            self.toolbar.EnableTool(self.tdelete.GetId(), True)
        #print 'OnPageChanged,  old:%d, new:%d, sel:%d\n' % (old, new, sel)
        event.Skip()
 
    def OnPageChanging(self, event):
        old = event.GetOldSelection()
        new = event.GetSelection()
        sel = self.notebook.GetSelection()
        numPages = self.notebook.GetPageCount()
        if new == numPages - 1:
            self.toolbar.EnableTool(self.tdelete.GetId(), False)
        else:
            self.toolbar.EnableTool(self.tdelete.GetId(), True)
        #print 'OnPageChanging, old:%d, new:%d, sel:%d\n' % (old, new, sel)
        event.Skip()
