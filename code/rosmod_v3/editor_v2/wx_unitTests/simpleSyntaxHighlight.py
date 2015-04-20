#!/usr/bin/python

import wx, wx.stc

STATE_TYPE = 0
STATE_ARRAY = 1
STATE_NAME = 2
STATE_EQUAL = 3
STATE_VALUE = 4
STATE_END = 5

STATE_TRANSITIONS = {
    STATE_TYPE: [("[", STATE_ARRAY, True), 
                    (" ", STATE_NAME, False)],
    STATE_ARRAY: [("[", STATE_ARRAY, False),
                  ("]", STATE_NAME, False)],
    STATE_NAME: [(" =", STATE_EQUAL, True),
                 ("", STATE_END, True)],
    STATE_EQUAL: [(" ", STATE_VALUE, True)],
    STATE_VALUE: [("", STATE_END, True)],
}

class ROSInput(wx.stc.StyledTextCtrl):
    def __init__(self,parent, ID=-1,
                 pos=wx.DefaultPosition, size=wx.DefaultSize, style=0):
        wx.stc.StyledTextCtrl.__init__(self,parent,ID,pos,size,style)
        
        self.SetLexer(wx.stc.STC_LEX_CPP)
        self.SetStyleBits(5) # how many styles do we need?
        self.Bind(wx.stc.EVT_STC_STYLENEEDED, self.OnStyling)

    def OnStyling(self, e):
        start = self.GetEndStyled()
        end = e.GetPosition()
        self.StartStyling(start,31)
        for pos in range(start,end):
            self.SetStyling(1, 0)

_TESTSTRING = ur"""agse_package/stateVar test = True"""

class _TestApp(wx.App):

    def OnInit(self):
        frame = wx.Frame(None,-1, "Test Ctrl")
        ROSInput(frame, -1).SetText(_TESTSTRING)
        frame.Show()
        return True

if __name__ == "__main__": _TestApp(0).MainLoop()
