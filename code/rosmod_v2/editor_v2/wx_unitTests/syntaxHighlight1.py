#!/usr/bin/env python
# -*- coding: utf-8 -*-

u"""
Provides a StyledTextCtrl that understands TeX embedded in HTML.

This is useful for editing source code of HTML pages using MathJax_. MathJax
is a Javascript library which parses snippets of the form ``\\(\\LaTeX\\)`` or
``\\[\\LaTeX\\]`` and displays them either as MathML or some combination of
HTML and CSS.

The ``HtmlWithTexInput`` behaves like a ``StyledTextCtrl`` and will lex and
style source code as HTML. TeX snippets will be recognized as such and styled
differently. If you want to provide your own styles for the TeX snippets,
use the constants ``wx.stc.STC_L_*`` + ``OFFSET_ROUND`` for the styles of
the snippets delimited with round brackets, and ``wx.stc.STC_L_*`` + 
``OFFSET_SQUARE`` for square brackets. The style of the delimiters themselves
are stored in the style of ``wx.stc.STC_L_MATH`` + ``OFFSET_*``.

The styles for ASP VBScript code get overwritten and canâ€™t be used in
conjunction with ``HtmlWithTexInput``.

Example::

    my_ctrl = html_with_tex_ctrl.HtmlWithTexInput(my_frame, -1)
    my_ctrl.StyleSetSpec(wx.stc.STC_L_DEFAULT 
            + html_with_tex_ctrl.OFFSET_ROUND,
            "fore:#000099,back:#6666ff,face:Courier New,size:10,eolfilled")

This sets the foreground color of the default style of TeX fragments in round
brackets to dark blue, the background color to light blue.

If the module is called as a stand-alone application, a test frame is
produced.

.. _MathJax: http://www.mathjax.org
"""

__author__ = u"Jan Thor"
__docformat__ = u"restructuredtext de"

import wx, wx.stc
from html_styles import style_control, HTML_KEYWORDS


OFFSET_ROUND = 80 # ASP VBScript styles will be replaced with TeX styles
OFFSET_SQUARE = OFFSET_ROUND + 5
MATH_ROUND = chr(wx.stc.STC_L_MATH + OFFSET_ROUND) * 4
MATH_SQUARE = chr(wx.stc.STC_L_MATH + OFFSET_SQUARE) * 4

STATE_DEFAULT = 0
STATE_TAG = 1
STATE_ROUND = 2
STATE_SQUARE = 3
STATE_DELIM = 100

STATE_TRANSITIONS = {
    STATE_DEFAULT: [("\\(", STATE_ROUND, True), 
                    ("\\[", STATE_SQUARE, True), 
                    ("<", STATE_TAG, False)],
    STATE_TAG: [(">", STATE_DEFAULT, False)],
    STATE_ROUND: [("\\)", STATE_DEFAULT, True)],
    STATE_SQUARE: [("\\]", STATE_DEFAULT, True)],
}

class HtmlWithTexInput(wx.stc.StyledTextCtrl):
    u"""StyledTextCtrl with lexing for HTML source with TeX snippets."""

    def __init__(self, parent, ID=-1,
                 pos=wx.DefaultPosition, size=wx.DefaultSize, style=0):
        u"""Same parameters as ``wx.stc.StyledTextCtrl``."""
        wx.stc.StyledTextCtrl.__init__(self, parent, ID, pos, size, style)
        self.SetLexer(wx.stc.STC_LEX_CONTAINER)
        self.SetStyleBits(7)
        self.Bind(wx.stc.EVT_STC_STYLENEEDED, self.OnStyling)
        style_control(self)

        # === Styles for TeX ===
        self.StyleSetSpec(wx.stc.STC_L_DEFAULT + OFFSET_ROUND,
            "fore:#333333,back:#ffef66,face:Courier New,size:10,eolfilled")
        self.StyleSetSpec(wx.stc.STC_L_COMMAND + OFFSET_ROUND,
            "fore:#000099,back:#ffef66,face:Courier New,size:10,bold")
        self.StyleSetSpec(wx.stc.STC_L_TAG + OFFSET_ROUND,
            "fore:#7f007f,back:#ffef66,face:Courier New,size:10")
        self.StyleSetSpec(wx.stc.STC_L_MATH + OFFSET_ROUND,
            "fore:#bb0000,back:#ffdf66,face:Courier New,size:10,bold")
        self.StyleSetSpec(wx.stc.STC_L_COMMENT + OFFSET_ROUND,
            "fore:#007f00,back:#efff66,face:Courier New,size:10,italic")

        self.StyleSetSpec(wx.stc.STC_L_DEFAULT + OFFSET_SQUARE,
            "fore:#333333,back:#fff8bb,face:Courier New,size:10,eolfilled")
        self.StyleSetSpec(wx.stc.STC_L_COMMAND + OFFSET_SQUARE,
            "fore:#000099,back:#fff8bb,face:Courier New,size:10,bold")
        self.StyleSetSpec(wx.stc.STC_L_TAG + OFFSET_SQUARE,
            "fore:#7f007f,back:#fff8bb,face:Courier New,size:10")
        self.StyleSetSpec(wx.stc.STC_L_MATH + OFFSET_SQUARE,
            "fore:#bb0000,back:#ffdfaa,face:Courier New,size:10,bold")
        self.StyleSetSpec(wx.stc.STC_L_COMMENT + OFFSET_SQUARE,
            "fore:#007f00,back:#f8ffaa,face:Courier New,size:10,italic")

        # === More stylinâ€™ ===
        self.SetEdgeMode(wx.stc.STC_EDGE_LINE)
        self.SetEdgeColumn(213)
        self.SetMarginWidth(1, 0)
        self.SetWrapMode(wx.stc.STC_WRAP_WORD)
        
        # === Dummy controls as lexers ===
        self.dummyHtml = wx.stc.StyledTextCtrl(self, -1)
        self.dummyHtml.Show(False)
        self.dummyHtml.SetLexer(wx.stc.STC_LEX_HTML)
        self.dummyHtml.SetStyleBits(7)
        self.dummyHtml.SetKeyWords(0, HTML_KEYWORDS)
        self.dummyTex = wx.stc.StyledTextCtrl(self, -1)
        self.dummyTex.Show(False)
        self.dummyTex.SetLexer(wx.stc.STC_LEX_LATEX)
    
    def _parseHtml(self, fragment):
        self.dummyHtml.SetText(fragment.decode("utf8"))
        fl = len(fragment)
        self.dummyHtml.Colourise(0, fl)
        multiplexed = self.dummyHtml.GetStyledText(0, fl)
        return multiplexed
    
    def _parseTex(self, fragment, offset):
        self.dummyTex.SetText(fragment.decode("utf8").replace("\n", " "))
        fl = len(fragment)
        self.dummyTex.Colourise(0, fl)
        multiplexed = self.dummyTex.GetStyledText(0, fl)
        multiplexed = [s for s in multiplexed]
        for i in range(1, len(multiplexed), 2):
            multiplexed[i] = chr(ord(multiplexed[i]) + offset)
        return "".join(multiplexed)
    
    def OnStyling(self, evt):
        u"""Called when the control needs styling."""
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


# ============================================================================
#
# The remainder of this modul is for testing purposes and not really needed.
#                  You may cut if off in your own projects.
#
# ============================================================================

_TESTSTRING = ur"""\(\)<!DOCTYPE>
<p>This $i$s <error/> \(\LaTeX\) &amp;!</p>
<!--Remark \(x\)-->
\[(\frac{a}{c})^2+(\frac{b}{c})^2=1 \error
\no comment %Comment Ã¼ble ÃœmlÃ¤utÃ«
\This \is \still \a \comment\]
<?Whatâ€™s this?>
<?php echo "$this funny string"; 3++ ?>
<script type="text/javacript">
function f(x) {if("unclos" + '!' return 2*x;}
</script>
\[These\) are \(confusing\) delims\]\(right?\)"""

class _TestApp(wx.App):

    def OnInit(self):
        frame = wx.Frame(None,-1, "Test Ctrl")
        HtmlWithTexInput(frame, -1).SetText(_TESTSTRING)
        frame.Show()
        return True

if __name__ == "__main__": _TestApp(0).MainLoop()
