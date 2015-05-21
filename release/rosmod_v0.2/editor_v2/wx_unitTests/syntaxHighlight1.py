#!/usr/bin/env python
# -*- coding: utf-8 -*-

import wx, wx.stc
from c_styles import style_control

STATE_DEFAULT= -1
STATE_TYPE = 0
STATE_ARRAY = 1
STATE_NAME = 2
STATE_EQUAL = 3
STATE_VALUE = 4
STATE_END = 5

STATE_TRANSITIONS = {
    STATE_DEFAULT: [("", STATE_TYPE, True)],
    STATE_TYPE: [("[", STATE_ARRAY, True), 
                 (" ", STATE_NAME, True)],
    STATE_ARRAY: [("[", STATE_ARRAY, True),
                  ("]", STATE_NAME, True)],
    STATE_NAME: [("=", STATE_EQUAL, True),
                ("\n", STATE_END, True)],
    STATE_EQUAL: [(" ", STATE_VALUE, True)],
    STATE_VALUE: [("\n", STATE_END, True)],
    STATE_END: [("", STATE_VALUE, True)]
}

kwList = ["int32","string","int64","bool","float32","float64"]

class StyledInput(wx.stc.StyledTextCtrl):
    u"""StyledTextCtrl with lexing for HTML source with TeX snippets."""

    def __init__(self, parent, ID=-1,
                 pos=wx.DefaultPosition, size=wx.DefaultSize, style=0):
        u"""Same parameters as ``wx.stc.StyledTextCtrl``."""
        wx.stc.StyledTextCtrl.__init__(self, parent, ID, pos, size, style)
        self.SetLexer(wx.stc.STC_LEX_CONTAINER)
        self.SetStyleBits(5)
        self.Bind(wx.stc.EVT_STC_STYLENEEDED, self.OnStyling)
        style_control(self)

        # === More stylinâ€™ ===
        self.SetEdgeMode(wx.stc.STC_EDGE_LINE)
        self.SetEdgeColumn(213)
        self.SetMarginWidth(1, 0)
        self.SetWrapMode(wx.stc.STC_WRAP_WORD)
        
        # === Dummy controls as lexers ===
        self.dummyHtml = wx.stc.StyledTextCtrl(self, -1)
        self.dummyHtml.Show(False)
        self.dummyHtml.SetLexer(wx.stc.STC_LEX_CPP)
        self.dummyHtml.SetStyleBits(5)
        self.dummyHtml.SetKeyWords(0, " ".join(kwList))
        self.dummyTex = wx.stc.StyledTextCtrl(self, -1)
        self.dummyTex.Show(False)
        self.dummyTex.SetLexer(wx.stc.STC_LEX_LATEX)
    
    def _parseHtml(self, fragment):
        self.dummyHtml.SetText(fragment.decode("utf8"))
        fl = len(fragment)
        self.dummyHtml.Colourise(0, fl)
        multiplexed = self.dummyHtml.GetStyledText(0, fl)
        return multiplexed
    
    def OnStyling(self, evt):
        u"""Called when the control needs styling."""
        text = self.GetText().encode("utf8")

        start = self.GetEndStyled()
        end = evt.GetPosition()
        
        # === split text into chunks ===
        splitpoints = [0]
        states = [STATE_DEFAULT]
        state = STATE_DEFAULT
        for i in range(0, len(text)):
            transitions = STATE_TRANSITIONS[state]
            for delim, newstate, bsplit in transitions:
                nd = len(delim)
                if i >= nd - 1 and text[i+1-nd:i+1] == delim:
                    if bsplit:
                        splitpoints.append(i-1)
                        splitpoints.append(i+1)
                        states.append(state)
                        states.append(newstate)
                    state = newstate
        if splitpoints[-1] != len(text):
            splitpoints.append(len(text))
        parts = [text[splitpoints[i]:splitpoints[i+1]] 
                 for i in range(len(splitpoints) - 1)]

        # === lex and style each part ===
        parsed = ""
        for i in range(len(parts)):
            type = states[i]
            fragment = parts[i]
            if type == STATE_DEFAULT:
                parsed += self._parseHtml(fragment)
            elif type == STATE_TYPE:
                parsed += self._parseHtml(fragment)
            elif type == STATE_ARRAY:
                parsed += MATH_ROUND
            elif type == STATE_NAME:
                parsed += self._parseHtml(fragment)
            elif type == STATE_EQUAL:
                parsed += MATH_SQUARE
            elif type == STATE_END:
                parsed += self._parseHtml(fragment)
                
        # === style the complete control ===
        self.StartStyling(0, 127)
        parsed = "".join([parsed[i] for i in range(1, len(parsed), 2)])
        self.SetStyleBytes(len(parsed), parsed)


# ============================================================================
#
# The remainder of this modul is for testing purposes and not really needed.
#                  You may cut if off in your own projects.
#
# ============================================================================

_TESTSTRING = ur"""int32 helloWorld = 32
"""

class _TestApp(wx.App):

    def OnInit(self):
        frame = wx.Frame(None,-1, "Test Ctrl")
        StyledInput(frame, -1).SetText(_TESTSTRING)
        frame.Show()
        return True

if __name__ == "__main__": _TestApp(0).MainLoop()
