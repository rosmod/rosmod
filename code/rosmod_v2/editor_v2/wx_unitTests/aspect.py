#!/usr/bin/python

import wx

class AspectTab(wx.Panel):
    def __init__(self,parent):
        wx.Panel.__init__(self,parent=parent, id=wx.ID_ANY)

class Aspect(wx.Panel):
    def __init__(self,parent,model=None):
        wx.Panel.__init__(self,parent)

        self.BuildToolbar()
        self.BuildNotebook()

        self.boxSizer = wx.BoxSizer( wx.VERTICAL )     
        self.boxSizer.Add(self.toolbar, 0, wx.ALL | wx.ALIGN_LEFT | wx.EXPAND, 4)
        self.boxSizer.Add(self.notebook, 1, wx.ALL | wx.ALIGN_LEFT | wx.EXPAND, 4)
        self.SetSizer(self.boxSizer)

    def BuildToolbar(self):
        self.toolbar = wx.ToolBar(self)

        # file operations
        self.tnew_ID = wx.NewId()
        self.topen_ID = wx.NewId()
        self.tsave_ID = wx.NewId()
        self.tnew = self.toolbar.AddLabelTool(self.tnew_ID, 'New', wx.Bitmap('tnew.png'), shortHelp="New")
        self.topen = self.toolbar.AddLabelTool(self.topen_ID, '', wx.Bitmap('topen.png'), shortHelp="Open")
        self.tsave = self.toolbar.AddLabelTool(self.tsave_ID, '', wx.Bitmap('tsave.png'), shortHelp="Save")
        self.toolbar.AddSeparator()
        # undo/redo
        self.tundo_ID = wx.NewId()
        self.tredo_ID = wx.NewId()
        self.tundo = self.toolbar.AddLabelTool(self.tundo_ID, '', wx.Bitmap('tundo.png'), shortHelp="Undo")
        self.tredo = self.toolbar.AddLabelTool(self.tredo_ID, '', wx.Bitmap('tredo.png'), shortHelp="Redo")
        self.toolbar.EnableTool(self.tundo_ID, False)
        self.toolbar.EnableTool(self.tredo_ID, False)
        self.toolbar.AddSeparator()
        # create / delete packages
        self.tcreate_ID = wx.NewId()
        self.tdelete_ID = wx.NewId()
        self.tcreate = self.toolbar.AddLabelTool(self.tcreate_ID, '', wx.Bitmap('tnew.png'), shortHelp="New Package")
        self.tdelete = self.toolbar.AddLabelTool(self.tdelete_ID, '', wx.Bitmap('texit.png'), shortHelp="Remove Package")
        self.RegisterToolbarEvents()

    def RegisterToolbarEvents(self):
        wx.EVT_TOOL( self, self.tnew_ID, self.OnNew)
        wx.EVT_TOOL( self, self.topen_ID, self.OnOpen)
        wx.EVT_TOOL( self, self.tsave_ID, self.OnSave)
        wx.EVT_TOOL( self, self.tcreate_ID, self.OnCreate)
        wx.EVT_TOOL( self, self.tdelete_ID, self.OnDelete)
        wx.EVT_TOOL( self, self.tundo_ID, self.OnUndo)
        wx.EVT_TOOL( self, self.tredo_ID, self.OnRedo)

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
        if self.notebook.GetSelection() == numPages - 2: # deleted into last page
            self.toolbar.EnableTool(self.tdelete.GetId(), False)

    def OnNew(self, e):
        pass
    def OnOpen(self, e):
        pass
    def OnSave(self, e):
        pass

    def OnUndo(self, e):
        pass
    def OnRedo(self, e):
        pass
 
    def pageChange(self, event):
        try:
            old = event.GetOldSelection()
            new = event.GetSelection()
            sel = self.notebook.GetSelection()
            numPages = self.notebook.GetPageCount()
            if new == numPages - 1:
                self.toolbar.EnableTool(self.tdelete_ID, False)
            else:
                self.toolbar.EnableTool(self.tdelete_ID, True)
            #print 'OnPageChanged,  old:%d, new:%d, sel:%d\n' % (old, new, sel)
        except wx.PyDeadObjectError:
            test = None #do nothing

    def OnPageChanged(self, event):
        self.pageChange(event)
        event.Skip()
 
    def OnPageChanging(self, event):
        self.pageChange(event)
        event.Skip()
