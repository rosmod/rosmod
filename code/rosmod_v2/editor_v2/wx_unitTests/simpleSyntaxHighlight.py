#!/usr/bin/python

import wx, wx.stc

class ROSInput(wx.stc.StyledTextCtrl):
    def __init__(self,parent, ID=-1,
                 pos=wx.DefaultPosition, size=wx.DefaultSize, style=0):
        wx.stc.StyledTextCtrl.__init__(self,parent,ID,pos,size,style)
        
        self.SetLexer(wx.stc.STC_LEX_CONTAINER)
        #self.SetSytleBits(7) # how many styles do we need? 7 => more than 32 styles
        self.Bind(wx.stc.EVT_STC_STYLENEEDED, self.OnStyling)


    def OnStyling(self, e):
        text = self.GetText().encode("utf8")

_TESTSTRING = ur"""
agse_package/stateVar test = True
"""

class _TestApp(wx.App):

    def OnInit(self):
        frame = wx.Frame(None,-1, "Test Ctrl")
        ROSInput(frame, -1).SetText(_TESTSTRING)
        frame.Show()
        return True

if __name__ == "__main__": _TestApp(0).MainLoop()
