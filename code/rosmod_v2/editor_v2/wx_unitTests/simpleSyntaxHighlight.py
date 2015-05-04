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
        self.SetLexer(wx.stc.STC_LEX_CONTAINER)
        self.SetStyleBits(5) # how many styles do we need?
        self.Bind(wx.stc.EVT_STC_STYLENEEDED, self.OnStyling)
        style_control(self)

        # === More styling ===
        self.SetEdgeMode(wx.stc.STC_EDGE_LINE)
        self.SetEdgeColumn(213)
        self.SetMarginWidth(1, 0)
        self.SetWrapMode(wx.stc.STC_WRAP_WORD)
        
        # === Dummy controls as lexers ===
        self.dummyROS = wx.stc.StyledTextCtrl(self, -1)
        self.dummyROS.Show(False)
        self.dummyROS.SetLexer(wx.stc.STC_LEX_HTML)
        self.dummyROS.SetStyleBits(7)
        self.dummyROS.SetKeyWords(0, HTML_KEYWORDS)

    def _parseHtml(self, fragment):
        self.dummyHtml.SetText(fragment.decode("utf8"))
        fl = len(fragment)
        self.dummyHtml.Colourise(0, fl)
        multiplexed = self.dummyHtml.GetStyledText(0, fl)
        return multiplexed

    def OnStyling(self, e):
        start = self.GetEndStyled()
        end = e.GetPosition()
        self.StartStyling(start,31)
        for pos in range(start,end):
            self.SetStyling(1, 0)

        text = self.GetText().encode("utf8")
        
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
                        states.append(STATE_DELIM + state + newstate)
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
            elif type == STATE_ROUND:
                parsed += self._parseTex(fragment, OFFSET_ROUND)
            elif type == STATE_SQUARE:
                parsed += self._parseTex(fragment, OFFSET_SQUARE)
            elif type == STATE_ROUND + STATE_DELIM:
                parsed += MATH_ROUND
            elif type == STATE_SQUARE + STATE_DELIM:
                parsed += MATH_SQUARE
                
        # === style the complete control ===
        self.StartStyling(0, 127)
        parsed = "".join([parsed[i] for i in range(1, len(parsed), 2)])
        self.SetStyleBytes(len(parsed), parsed)

_TESTSTRING = ur"""agse_package/stateVar test = True"""

class _TestApp(wx.App):

    def OnInit(self):
        frame = wx.Frame(None,-1, "Test Ctrl")
        ROSInput(frame, -1).SetText(_TESTSTRING)
        frame.Show()
        return True

if __name__ == "__main__": _TestApp(0).MainLoop()
