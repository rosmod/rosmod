import wx
from wx import stc

kwList = ["int32","string","int64","bool","float32","float64"]

class PyDialog(wx.Dialog):
    def __init__(self):
        wx.Dialog.__init__(self, None, -1, 'Python Code')
        sizer = wx.BoxSizer(wx.VERTICAL)

        self.stc = stc.StyledTextCtrl(self, -1)
        self.stc.SetSizeHints(400, 400)
        self.stc.SetLexer(stc.STC_LEX_CPP)
        self.stc.SetKeyWords(0, " ".join(kwList))
        self.stc.SetMarginType(1, stc.STC_MARGIN_NUMBER)
        # Python styles
        self.stc.StyleSetSpec(wx.stc.STC_P_DEFAULT, 'fore:#000000')
        # Comments
        self.stc.StyleSetSpec(wx.stc.STC_P_COMMENTLINE,  'fore:#008000,back:#F0FFF0')
        self.stc.StyleSetSpec(wx.stc.STC_P_COMMENTBLOCK, 'fore:#008000,back:#F0FFF0')
        # Numbers
        self.stc.StyleSetSpec(wx.stc.STC_P_NUMBER, 'fore:#008080')
        # Strings and characters
        self.stc.StyleSetSpec(wx.stc.STC_P_STRING, 'fore:#800080')
        self.stc.StyleSetSpec(wx.stc.STC_P_CHARACTER, 'fore:#800080')
        # Keywords
        self.stc.StyleSetSpec(wx.stc.STC_P_WORD, 'fore:#000080,bold')
        # Triple quotes
        self.stc.StyleSetSpec(wx.stc.STC_P_TRIPLE, 'fore:#800080,back:#FFFFEA')
        self.stc.StyleSetSpec(wx.stc.STC_P_TRIPLEDOUBLE, 'fore:#800080,back:#FFFFEA')
        # Class names
        self.stc.StyleSetSpec(wx.stc.STC_P_CLASSNAME, 'fore:#0000FF,bold')
        # Function names
        self.stc.StyleSetSpec(wx.stc.STC_P_DEFNAME, 'fore:#008080,bold')
        # Operators
        self.stc.StyleSetSpec(wx.stc.STC_P_OPERATOR, 'fore:#800000,bold')
        # Identifiers. I leave this as not bold because everything seems
        # to be an identifier if it doesn't match the above criterae
        self.stc.StyleSetSpec(wx.stc.STC_P_IDENTIFIER, 'fore:#000000')

        # Caret color
        self.stc.SetCaretForeground("BLUE")
        # Selection background
        self.stc.SetSelBackground(1, '#66CCFF')

        sizer.Add(self.stc, 0, wx.EXPAND)

        button = wx.Button(self, -1, 'Open...')
        self.Bind(wx.EVT_BUTTON, self.OnOpen, button)
        sizer.Add(button)

        self.SetSizer(sizer)
        sizer.Fit(self)

    def OnOpen(self, evt):
        dlg = wx.FileDialog(
            self,
            message = 'Choose File',
            wildcard = 'Python source (*.py)|*.py',
            style = wx.OPEN)

        if dlg.ShowModal() != wx.ID_OK:
            return

        with open(dlg.GetPath()) as fo:
            self.stc.SetText(fo.read())

        dlg.Destroy()


if __name__ == '__main__':
    app = wx.PySimpleApp()
    dlg = PyDialog()
    with open(__file__) as fo:
        dlg.stc.SetText(fo.read())
    dlg.ShowModal()
