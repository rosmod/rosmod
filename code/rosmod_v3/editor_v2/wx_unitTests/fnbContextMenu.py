import random
import sys
import wx
import wx.lib.agw.flatnotebook as fnb
import wx.lib.mixins.listctrl  as  listmix

musicdata = {
1 : ("Bad English", "The Price Of Love", "Rock"),
2 : ("DNA featuring Suzanne Vega", "Tom's Diner", "Rock"),
3 : ("George Michael", "Praying For Time", "Rock"),
4 : ("Gloria Estefan", "Here We Are", "Rock"),
5 : ("Linda Ronstadt", "Don't Know Much", "Rock"),
6 : ("Michael Bolton", "How Am I Supposed To Live Without You", "Blues"),
7 : ("Paul Young", "Oh Girl", "Rock"),
8 : ("Paula Abdul", "Opposites Attract", "Rock"),
9 : ("Richard Marx", "Should've Known Better", "Rock"),
10: ("Rod Stewart", "Forever Young", "Rock"),
11: ("Roxette", "Dangerous", "Rock"),
12: ("Sheena Easton", "The Lover In Me", "Rock"),
13: ("Sinead O'Connor", "Nothing Compares 2 U", "Rock"),
14: ("Stevie B.", "Because I Love You", "Rock"),
15: ("Taylor Dayne", "Love Will Lead You Back", "Rock"),
16: ("The Bangles", "Eternal Flame", "Rock"),
17: ("Wilson Phillips", "Release Me", "Rock"),
18: ("Billy Joel", "Blonde Over Blue", "Rock"),
19: ("Billy Joel", "Famous Last Words", "Rock"),
20: ("Billy Joel", "Lullabye (Goodnight, My Angel)", "Rock"),
21: ("Billy Joel", "The River Of Dreams", "Rock"),
22: ("Billy Joel", "Two Thousand Years", "Rock")
}

########################################################################
class TabPanelOne(wx.Panel):
    """
    This will be the first notebook tab
    """
    #----------------------------------------------------------------------
    def __init__(self, parent):
        """"""

        wx.Panel.__init__(self, parent=parent, id=wx.ID_ANY)

        sizer = wx.BoxSizer(wx.VERTICAL)
        txtOne = wx.TextCtrl(self, wx.ID_ANY, "")
        txtTwo = wx.TextCtrl(self, wx.ID_ANY, "")

        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(txtOne, 0, wx.ALL, 5)
        sizer.Add(txtTwo, 0, wx.ALL, 5)

        self.SetSizer(sizer)

########################################################################
class TestListCtrl(wx.ListCtrl, listmix.ListCtrlAutoWidthMixin):
    def __init__(self, parent, ID, pos=wx.DefaultPosition,
                 size=wx.DefaultSize, style=0):
        wx.ListCtrl.__init__(self, parent, ID, pos, size, style)
        listmix.ListCtrlAutoWidthMixin.__init__(self)

########################################################################
class TabPanelTwo(wx.Panel, listmix.ColumnSorterMixin):
    """
    This will be the second notebook tab
    """
    #----------------------------------------------------------------------
    def __init__(self, parent):
        """"""
        wx.Panel.__init__(self, parent=parent, id=wx.ID_ANY)
        self.createAndLayout()
        
    def createAndLayout(self):
        sizer = wx.BoxSizer(wx.VERTICAL)
        self.list = TestListCtrl(self, wx.ID_ANY, style=wx.LC_REPORT
                                 | wx.BORDER_NONE
                                 | wx.LC_EDIT_LABELS
                                 | wx.LC_SORT_ASCENDING)
        sizer.Add(self.list, 1, wx.EXPAND)
        self.populateList()
        # Now that the list exists we can init the other base class,
        # see wx/lib/mixins/listctrl.py
        self.itemDataMap = musicdata
        listmix.ColumnSorterMixin.__init__(self, 3)
        self.SetSizer(sizer)
        self.SetAutoLayout(True)
        
    def populateList(self):
        self.list.InsertColumn(0, "Artist")
        self.list.InsertColumn(1, "Title", wx.LIST_FORMAT_RIGHT)
        self.list.InsertColumn(2, "Genre")
        items = musicdata.items()
        
        for key, data in items:
            index = self.list.InsertStringItem(sys.maxint, data[0])
            self.list.SetStringItem(index, 1, data[1])
            self.list.SetStringItem(index, 2, data[2])
            self.list.SetItemData(index, key)

        self.list.SetColumnWidth(0, wx.LIST_AUTOSIZE)
        self.list.SetColumnWidth(1, wx.LIST_AUTOSIZE)
        self.list.SetColumnWidth(2, 100)

        # show how to select an item
        self.list.SetItemState(5, wx.LIST_STATE_SELECTED, wx.LIST_STATE_SELECTED)

        # show how to change the colour of a couple items
        item = self.list.GetItem(1)
        item.SetTextColour(wx.BLUE)
        self.list.SetItem(item)
        item = self.list.GetItem(4)
        item.SetTextColour(wx.RED)
        self.list.SetItem(item)

        self.currentItem = 0
        
    # Used by the ColumnSorterMixin, see wx/lib/mixins/listctrl.py
    def GetListCtrl(self):
        return self.list

########################################################################
class FlatNotebookDemo(fnb.FlatNotebook):
    """
    Flatnotebook class
    """
    #----------------------------------------------------------------------
    def __init__(self, parent):
        """Constructor"""
        fnb.FlatNotebook.__init__(self, parent, wx.ID_ANY)
 
########################################################################
class DemoFrame(wx.Frame):
    """
    Frame that holds all other widgets
    """
 
    #----------------------------------------------------------------------
    def __init__(self, title="FlatNotebook Add/Remove Page Tutorial"):
        """Constructor"""
        wx.Frame.__init__(self, None, wx.ID_ANY,
                          title=title,
                          size=(600,400)
                          )
        self._newPageCounter = 0
        panel = wx.Panel(self)
        self.createRightClickMenu()
 
        # create some widgets
        self.notebook = FlatNotebookDemo(panel)
        addPageBtn = wx.Button(panel, label="Add Page")
        addPageBtn.Bind(wx.EVT_BUTTON, self.onAddPage)
        removePageBtn = wx.Button(panel, label="Remove Page")
        removePageBtn.Bind(wx.EVT_BUTTON, self.onDeletePage)
        self.notebook.SetRightClickMenu(self._rmenu)
 
        # create some sizers
        sizer = wx.BoxSizer(wx.VERTICAL)
        btnSizer = wx.BoxSizer(wx.HORIZONTAL)
 
        # layout the widgets
        sizer.Add(self.notebook, 1, wx.ALL|wx.EXPAND, 5)
        btnSizer.Add(addPageBtn, 0, wx.ALL, 5)
        btnSizer.Add(removePageBtn, 0, wx.ALL, 5)
        sizer.Add(btnSizer)
        panel.SetSizer(sizer)
        self.Layout()
 
        self.Show()
 
    #----------------------------------------------------------------------
    def createRightClickMenu(self):
        """
        Based on method from flatnotebook demo
        """
        self._rmenu = wx.Menu()
        item = wx.MenuItem(self._rmenu, wx.ID_ANY,
                           "Close Tab\tCtrl+F4",
                           "Close Tab")
        self.Bind(wx.EVT_MENU, self.onDeletePage, item)
        self._rmenu.AppendItem(item)
 
    #----------------------------------------------------------------------
    def onAddPage(self, event):
        """
        This method is based on the flatnotebook demo
 
        It adds a new page to the notebook
        """
        caption = "New Page Added #" + str(self._newPageCounter)
        self.Freeze()
 
        self.notebook.AddPage(self.createPage(caption), caption, True)
        self.Thaw()
        self._newPageCounter = self._newPageCounter + 1
 
    #----------------------------------------------------------------------
    def createPage(self, caption):
        """
        Creates a notebook page from one of three
        panels at random and returns the new page
        """
        panel_list = [TabPanelOne, TabPanelTwo]
        tab = random.choice(panel_list)
        page = tab(self.notebook)
        return page
 
    #----------------------------------------------------------------------
    def onDeletePage(self, event):
        """
        This method is based on the flatnotebook demo
 
        It removes a page from the notebook
        """
        print "IN DELETE PAGE"
        self.notebook.DeletePage(self.notebook.GetSelection())
 
#----------------------------------------------------------------------
if __name__ == "__main__":
    app = wx.PySimpleApp()
    frame = DemoFrame()
    app.MainLoop()
