import wx

class MainFrame(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, parent = None, size = (500,500))
        self.frameZ = frameZ = wx.BoxSizer(wx.VERTICAL) # Create main sizer
        
        ## (1) ## If this SetMinSize() is omitted, the frame will resize totally
        frameZ.SetMinSize((500,500)) # Lock the size of the frame
        self.SetSizer(frameZ)
        
        # Create CollapsePane wrapper objects, pass frame as arg
        cp1 = ColPaneWrapper(self)
        cp2 = ColPaneWrapper(self)
        
        # Do Layout() and SetSizeHints() for main frame
        frameZ.Layout()
        frameZ.SetSizeHints(self)

class ColPaneWrapper:
    def __init__(self, frame):

        self.cp = cp = wx.CollapsiblePane(frame) # Create CollapsePane object, frame as parent
        self.frameZ = frameZ = frame.frameZ # Get frame sizer
        self.frame = frame # Get frame
        frame.Bind(wx.EVT_COLLAPSIBLEPANE_CHANGED, self.OnCollapse, cp) # Bind OnCollapse event
        
        cp_pane = cp.GetPane() # Get pane to use as parent for controls and sizer
        self.cpZ = cpZ = wx.BoxSizer(wx.VERTICAL) # Create CP sizer
        cp_pane.SetSizer(cpZ) # Assign sizer to CPs pane
        
        button = wx.Button(cp_pane, label = 'Canvas') # Create button, use CPs pane as parent
        cpZ.Add(button, 1, wx.EXPAND | wx.ALL, 0) # Add button to CP sizer
        frameZ.Add(cp, 1, wx.EXPAND | wx.ALL, 0) # Add CP to frame sizer
        
        # Do OnCollapse to refresh the state of the panes
        self.OnCollapse()

    def OnCollapse(self, evt = None):
    # OnCollapse:
    # Change the Sizer-Proportion-state of the CP to match the Expanded/Collapsed state
        if self.cp.IsCollapsed():
            self.frameZ.GetItem(self.cp).SetProportion(0)
        else:
            self.frameZ.GetItem(self.cp).SetProportion(1)
        
        # Do Layout() for CPs sizer
        self.cpZ.Layout()
        
        ## (2) ## If this SetSizeHints() is omitted, the frame will resize totally.
        self.frameZ.SetSizeHints(self.frame) # SetSizeHints for frame sizer

if __name__ == "__main__":
    app = wx.App()
    MainFrame().Show()
    app.MainLoop()
