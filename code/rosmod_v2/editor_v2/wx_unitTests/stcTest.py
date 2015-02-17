# wxstc_basics1.py
# styled text using wxPython's
# wx.StyledTextCtrl(parent, id, pos, size, style, name)
# used the scintilla programming editor for guidance
# set up for line numbers, folding and Python code highlighting
# tested with Python26 and wxPython28 by vegaseat

import  wx
import  wx.stc  as  stc

ros_kwList = ["bool", 
              "int8",
              "uint8",
              "int16",
              "uint16",
              "int32",
              "uint32",
              "int64",
              "uint64",
              "float32",
              "float64"]

faces = { 
    'times': 'Times',
    'mono' : 'Courier',
    'helv' : 'Helvetica',
    'other': 'new century schoolbook',
    'size' : 12,
    'size2': 10,
}

class MySTC(stc.StyledTextCtrl):
    """
    set up for folding and Python code highlighting
    """
    def __init__(self, parent):
        stc.StyledTextCtrl.__init__(self, parent, wx.ID_ANY, style=wx.SUNKEN_BORDER)

        #self.SetMarginWidth(1,0) # to remove margin
        self.SetMargins(0,0)

        # use Python code highlighting
        self.SetLexer(stc.STC_LEX_PYTHON)
        self.SetKeyWords(0, " ".join(ros_kwList))
        
        self.SetEdgeColumn(78)
        
        self.Bind(stc.EVT_STC_UPDATEUI, self.onUpdateUI)

        # make some general styles ...
        # the lexer defines what each style is used for 

        # global default styles for all languages
        self.StyleSetSpec(stc.STC_STYLE_DEFAULT,
            "face:%(helv)s,size:%(size)d" % faces)
        # reset all to be like the default
        self.StyleClearAll()  

        # global default styles for all languages
        self.StyleSetSpec(stc.STC_STYLE_DEFAULT,
            "face:%(helv)s,size:%(size)d" % faces)
        self.StyleSetSpec(stc.STC_STYLE_CONTROLCHAR,
            "face:%(other)s" % faces)
        self.StyleSetSpec(stc.STC_STYLE_BRACELIGHT,
            "fore:#FFFFFF,back:#0000FF,bold")
        self.StyleSetSpec(stc.STC_STYLE_BRACEBAD,
            "fore:#000000,back:#FF0000,bold")

        # make the Python styles ...
        # Default 
        self.StyleSetSpec(stc.STC_P_DEFAULT,
            "fore:#000000,face:%(helv)s,size:%(size)d" % faces)
        # Comments
        self.StyleSetSpec(stc.STC_P_COMMENTLINE,
            "fore:#007F00,face:%(other)s,size:%(size)d" % faces)
        # Number
        self.StyleSetSpec(stc.STC_P_NUMBER,
            "fore:#007F7F,size:%(size)d" % faces)
        # String
        self.StyleSetSpec(stc.STC_P_STRING,
            "fore:#7F007F,face:%(helv)s,size:%(size)d" % faces)
        # Single quoted string
        self.StyleSetSpec(stc.STC_P_CHARACTER,
            "fore:#7F007F,face:%(helv)s,size:%(size)d" % faces)
        # Keyword
        self.StyleSetSpec(stc.STC_P_WORD,
            "fore:#00007F,bold,size:%(size)d" % faces)
        # Triple quotes
        self.StyleSetSpec(stc.STC_P_TRIPLE,
            "fore:#7F0000,size:%(size)d" % faces)
        # Triple double quotes
        self.StyleSetSpec(stc.STC_P_TRIPLEDOUBLE,
            "fore:#7F0000,size:%(size)d" % faces)
        # Class name definition
        self.StyleSetSpec(stc.STC_P_CLASSNAME,
            "fore:#0000FF,bold,underline,size:%(size)d" % faces)
        # Function or method name definition
        self.StyleSetSpec(stc.STC_P_DEFNAME,
            "fore:#007F7F,bold,size:%(size)d" % faces)
        # Operators
        self.StyleSetSpec(stc.STC_P_OPERATOR,
            "bold,size:%(size)d" % faces)
        # Identifiers
        self.StyleSetSpec(stc.STC_P_IDENTIFIER,
            "fore:#000000,face:%(helv)s,size:%(size)d" % faces)
        # Comment-blocks
        self.StyleSetSpec(stc.STC_P_COMMENTBLOCK,
            "fore:#7F7F7F,size:%(size)d" % faces)
        # End of line where string is not closed
        self.StyleSetSpec(stc.STC_P_STRINGEOL,
            "fore:#000000,face:%(mono)s,back:#E0C0E0,eol,size:%(size)d"\
                % faces)

        self.SetCaretForeground("BLUE")

    def onUpdateUI(self, evt):
        # check for matching braces
        braceAtCaret = -1
        braceOpposite = -1
        charBefore = None
        caretPos = self.GetCurrentPos()

        if caretPos > 0:
            charBefore = self.GetCharAt(caretPos - 1)
            styleBefore = self.GetStyleAt(caretPos - 1)

        # check before
        if charBefore and chr(charBefore) in "[]{}()"\
                and styleBefore == stc.STC_P_OPERATOR:
            braceAtCaret = caretPos - 1

        # check after
        if braceAtCaret < 0:
            charAfter = self.GetCharAt(caretPos)
            styleAfter = self.GetStyleAt(caretPos)

            if charAfter and chr(charAfter) in "[]{}()"\
                    and styleAfter == stc.STC_P_OPERATOR:
                braceAtCaret = caretPos

        if braceAtCaret >= 0:
            braceOpposite = self.BraceMatch(braceAtCaret)

        if braceAtCaret != -1  and braceOpposite == -1:
            self.BraceBadLight(braceAtCaret)
        else:
            self.BraceHighlight(braceAtCaret, braceOpposite)

class MyFrame(wx.Frame):
    def __init__(self, parent, mytitle, mysize):
        wx.Frame.__init__(self, parent, wx.ID_ANY, mytitle, size=mysize)
        
        stc_edit = MySTC(self)
        
        # open a Python code file you have in the working folder
        # or give full path name
        py_file = "stc3.py"
        stc_edit.SetText(open(py_file).read())
        stc_edit.EmptyUndoBuffer()
        stc_edit.Colourise(0, -1)

        # line numbers in the margin
        stc_edit.SetMarginType(1, stc.STC_MARGIN_NUMBER)
        stc_edit.SetMarginWidth(1, 30)


app = wx.App(0)
# create a MyFrame instance and show the frame
mytitle = 'wx.StyledTextCtrl (rightclick for menu)'
MyFrame(None, mytitle, (800, 500)).Show()
app.MainLoop()
