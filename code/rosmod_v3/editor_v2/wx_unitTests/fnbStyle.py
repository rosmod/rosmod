import wx
import wx.lib.agw.flatnotebook as fnb

########################################################################
class TabPanel(wx.Panel):
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
class FlatNotebookDemo(fnb.FlatNotebook):
    """
    Flatnotebook class
    """
 
    #----------------------------------------------------------------------
    def __init__(self, parent):
        """Constructor"""
        fnb.FlatNotebook.__init__(self, parent, wx.ID_ANY)
 
        pageOne = TabPanel(self)
        pageTwo = TabPanel(self)
        pageThree = TabPanel(self)
 
        self.AddPage(pageOne, "PageOne")
        self.AddPage(pageTwo, "PageTwo")
        self.AddPage(pageThree, "PageThree")
 
 
########################################################################
class DemoFrame(wx.Frame):
    """
    Frame that holds all other widgets
    """
 
    #----------------------------------------------------------------------
    def __init__(self):
        """Constructor"""
        wx.Frame.__init__(self, None, wx.ID_ANY,
                          "FlatNotebook Tutorial with Style",
                          size=(600,400)
                          )
        self.styleDict = {"Default":self.OnDefaultStyle,
                          "VC71":self.OnVC71Style,
                          "VC8":self.OnVC8Style,
                          "Fancy":self.OnFancyStyle,
                          "Firefox 2":self.OnFF2Style}
        choices = self.styleDict.keys()
 
        panel = wx.Panel(self)
        self.notebook = FlatNotebookDemo(panel)
        self.styleCbo = wx.ComboBox(panel, wx.ID_ANY, "Default",
                                    wx.DefaultPosition, wx.DefaultSize,
                                    choices=choices, style=wx.CB_DROPDOWN)
        styleBtn = wx.Button(panel, wx.ID_ANY, "Change Style")
        styleBtn.Bind(wx.EVT_BUTTON, self.onStyle)
 
        # create some sizers
        sizer = wx.BoxSizer(wx.VERTICAL)
        hSizer = wx.BoxSizer(wx.HORIZONTAL)
 
        # add the widgets to the sizers
        sizer.Add(self.notebook, 1, wx.ALL|wx.EXPAND, 5)
        hSizer.Add(self.styleCbo, 0, wx.ALL|wx.CENTER, 5)
        hSizer.Add(styleBtn, 0, wx.ALL, 5)
        sizer.Add(wx.StaticLine(panel), 0, wx.ALL|wx.EXPAND, 5)
        sizer.Add(hSizer, 0, wx.ALL, 5)
 
        panel.SetSizer(sizer)
        self.Layout()
 
        self.Show()
 
    #----------------------------------------------------------------------
    def onStyle(self, event):
        """
        Changes the style of the tabs
        """
        print "in onStyle"
        style = self.styleCbo.GetValue()
        print style
        self.styleDict[style]()
 
    # The following methods were taken from the wxPython 
    # demo for the FlatNotebook
    def OnFF2Style(self):
 
        style = self.notebook.GetWindowStyleFlag()
 
        # remove old tabs style
        mirror = ~(fnb.FNB_VC71 | fnb.FNB_VC8 | fnb.FNB_FANCY_TABS | fnb.FNB_FF2)
        style &= mirror
 
        style |= fnb.FNB_FF2
 
        self.notebook.SetWindowStyleFlag(style)
 
 
    def OnVC71Style(self):
 
        style = self.notebook.GetWindowStyleFlag()
 
        # remove old tabs style
        mirror = ~(fnb.FNB_VC71 | fnb.FNB_VC8 | fnb.FNB_FANCY_TABS | fnb.FNB_FF2)
        style &= mirror
 
        style |= fnb.FNB_VC71
 
        self.notebook.SetWindowStyleFlag(style)
 
 
    def OnVC8Style(self):
 
        style = self.notebook.GetWindowStyleFlag()
 
        # remove old tabs style
        mirror = ~(fnb.FNB_VC71 | fnb.FNB_VC8 | fnb.FNB_FANCY_TABS | fnb.FNB_FF2)
        style &= mirror
 
        # set new style
        style |= fnb.FNB_VC8
 
        self.notebook.SetWindowStyleFlag(style)
 
 
    def OnDefaultStyle(self):
 
        style = self.notebook.GetWindowStyleFlag()
 
        # remove old tabs style
        mirror = ~(fnb.FNB_VC71 | fnb.FNB_VC8 | fnb.FNB_FANCY_TABS | fnb.FNB_FF2)
        style &= mirror
 
        self.notebook.SetWindowStyleFlag(style)
 
 
    def OnFancyStyle(self):
 
        style = self.notebook.GetWindowStyleFlag()
 
        # remove old tabs style
        mirror = ~(fnb.FNB_VC71 | fnb.FNB_VC8 | fnb.FNB_FANCY_TABS | fnb.FNB_FF2)
        style &= mirror
 
        style |= fnb.FNB_FANCY_TABS
        self.notebook.SetWindowStyleFlag(style)
 
#----------------------------------------------------------------------
if __name__ == "__main__":
    app = wx.PySimpleApp()
    frame = DemoFrame()
    app.MainLoop()
