#!/usr/bin/python

import wx
import wx.stc as stc
from wx.lib.pubsub import Publisher
from wx.lib.scrolledpanel import ScrolledPanel

from collections import OrderedDict

from metaclass import meta_class_dict
from metaModel import model_dict

class RMLProgressDialog(wx.Dialog):
    """
    Shows a Progres Gauge while an operation is taking place. May be cancellable
    which is possible when converting pdf/ps or loading models
    """
    def __init__(self, parent, title, progress_q, numItems=100, cancellable=False):
        """Defines a gauge and a timer which updates the gauge."""
        wx.Dialog.__init__(self, parent=parent, title=title, style = 0)
        self.count = 0
        self.numItems = numItems
        self.progress = wx.Gauge(self, range=self.numItems)
        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(self.progress, 0, wx.EXPAND)

        self.progressText = wx.StaticText(self, wx.NewId())
        sizer.Add(self.progressText, 0, wx.EXPAND)

        self.ok = wx.Button(self, wx.ID_OK)
        self.ok.Bind(wx.EVT_BUTTON, self.on_cancel)
        btnSizer = wx.StdDialogButtonSizer()
        btnSizer.AddButton(self.ok)
        self.ok.Disable()

        if cancellable:
            cancel = wx.Button(self, wx.ID_CANCEL)
            cancel.SetDefault()
            cancel.Bind(wx.EVT_BUTTON, self.on_cancel)
            btnSizer.AddButton(cancel)
        
        btnSizer.Realize()
        sizer.Add(btnSizer, 0, wx.ALIGN_CENTER | wx.TOP | wx.BOTTOM, 10)

        self.SetSizer(sizer)

        self.progress_q = progress_q
        
        self.TIMER_ID = wx.NewId()
        self.timer = wx.Timer(self, self.TIMER_ID)
        self.timer.Start(100)
        wx.EVT_TIMER(self, self.TIMER_ID, self.OnTimer)
        self.MakeModal(True)

    def OnTimer(self, event):
        try:
            message = self.progress_q.get(False)
            if message != None:
                self.progressText.SetLabel( message[0] )
                if len(message) == 1:
                    self.count += 1
                    self.progress.SetValue(self.count)
                elif len(message) == 2:
                    self.count += message[1]
                    self.progress.SetValue(self.count)
                elif len(message) == 3:
                    self.progress.SetRange( message[2] )
                    self.progress.SetValue( message[1] )
                    self.count = message[1]
                print message[0]
                if self.count >= self.progress.GetRange():
                    self.progress.SetValue(self.progress.GetRange())
                    self.ok.Enable()
                    self.timer.Stop()
        except:
            pass

    def on_cancel(self, event):
        self.timer.Stop()
        self.MakeModal(False)
        self.Destroy()

def RMLFileDialog(parent,fileTypes,path,prompt,fd_flags):
    modelPath = None
    fileName = None
    dlg = wx.FileDialog(parent, prompt, path, "", fileTypes, fd_flags)
    dlg.MakeModal(True)
    if dlg.ShowModal() == wx.ID_OK:
        fileName = dlg.GetFilename()
        modelPath = dlg.GetDirectory()
    dlg.MakeModal(False)
    dlg.Destroy()
    return fileName, modelPath

def RMLDirectoryDialog(parent,path,prompt):
    workspacePath = None
    dlg = wx.DirDialog(parent, prompt, path)
    dlg.MakeModal(True)
    if dlg.ShowModal() == wx.ID_OK:
        workspacePath = dlg.GetPath()
    dlg.MakeModal(False)
    dlg.Destroy()
    return workspacePath

def InfoDialog(parent, info):
    dlg = wx.MessageDialog(parent, info, 'Info', wx.OK )
    dlg.MakeModal(True)
    dlg.ShowModal()
    dlg.MakeModal(False)
    dlg.Destroy()

def ErrorDialog(parent, msg):
    dlg = wx.MessageDialog(parent, msg, 'Error', wx.OK | wx.ICON_ERROR)
    dlg.MakeModal(True)
    dlg.ShowModal()
    dlg.MakeModal(False)
    dlg.Destroy()

def ConfirmDialog(parent, msg):
    dlg = wx.MessageDialog(parent, msg, 'Confirm', wx.YES_NO | wx.NO_DEFAULT | wx.ICON_QUESTION)
    dlg.MakeModal(True)
    retVal = dlg.ShowModal()
    dlg.MakeModal(False)
    dlg.Destroy()
    if retVal == wx.ID_YES:
        retVal = True
    else:
        retVal = False
    return retVal


class EditDialog(wx.Dialog):
    
    def __init__(self, parent = None, title = "ROSMOD", style = wx.RESIZE_BORDER | wx.SIMPLE_BORDER,  editDict = OrderedDict(), editObj = None, invalidNames =[], references = []):
        wx.Dialog.__init__(self, parent=parent, title=title, style = style)
        self.editDict = editDict
        self.editObj = editObj
        self.invalidNames = invalidNames
        self.references = references
        self.returnDict = OrderedDict()
        self.InitUI()
        self.MakeModal(True)

    def InitUI(self):
        self.panel = wx.lib.scrolledpanel.ScrolledPanel(parent = self, id = -1)
        self.panel.SetupScrolling()
        self.vbox = wx.BoxSizer(wx.VERTICAL)
        self.pbox = wx.FlexGridSizer(rows=len(self.editDict.keys()),cols=2,vgap=9,hgap=25)
        self.pbox.SetFlexibleDirection(wx.BOTH)
        self.pbox.SetNonFlexibleGrowMode(wx.FLEX_GROWMODE_ALL)
        rNum = 0
        self.inputs = OrderedDict()
        for key,value in self.editDict.iteritems():
            field = None
            label = None
            growRow = False
            if meta_class_dict[key].kind == "string":
                label = wx.StaticText(self.panel, label=meta_class_dict[key].display_name + ":")
                field = wx.TextCtrl(self.panel)
                if value != "" and value != None and value != []:
                    field.AppendText(value)
            elif meta_class_dict[key].kind == "boolean":
                label = wx.StaticText(self.panel, label=meta_class_dict[key].display_name + ":")
                field = wx.CheckBox(self.panel)
                if type(value) is bool:
                    field.SetValue(value)
                else:
                    field.SetValue(False)
            elif meta_class_dict[key].kind == "list":
                label = wx.StaticText(self.panel, label=meta_class_dict[key].display_name + ":")
                possibles = meta_class_dict[key].input_validator
                field = wx.ComboBox(self.panel, choices = [], style=wx.CB_READONLY)
                for possible in possibles:
                    field.Append(possible)
                if value in possibles:
                    field.SetStringSelection(value)
                else:
                    field.SetSelection(0)
            elif meta_class_dict[key].kind == "code":
                label = wx.StaticText(self.panel, label=meta_class_dict[key].display_name + ":")
                growRow=True
                field = stc.StyledTextCtrl(self.panel)

                kwList = ["int32","string","int64","bool","float32","float64"]
                field.SetSizeHints(50, 50)
                field.SetLexer(stc.STC_LEX_CPP)
                field.SetKeyWords(0, " ".join(kwList))
                field.SetMarginType(1, stc.STC_MARGIN_NUMBER)
                # Python styles
                field.StyleSetSpec(wx.stc.STC_P_DEFAULT, 'fore:#000000')
                # Comments
                field.StyleSetSpec(wx.stc.STC_P_COMMENTLINE,  'fore:#008000,back:#F0FFF0')
                field.StyleSetSpec(wx.stc.STC_P_COMMENTBLOCK, 'fore:#008000,back:#F0FFF0')
                # Numbers
                field.StyleSetSpec(wx.stc.STC_P_NUMBER, 'fore:#008080')
                # Strings and characters
                field.StyleSetSpec(wx.stc.STC_P_STRING, 'fore:#800080')
                field.StyleSetSpec(wx.stc.STC_P_CHARACTER, 'fore:#800080')
                # Keywords
                field.StyleSetSpec(wx.stc.STC_P_WORD, 'fore:#000080,bold')
                # Triple quotes
                field.StyleSetSpec(wx.stc.STC_P_TRIPLE, 'fore:#800080,back:#FFFFEA')
                field.StyleSetSpec(wx.stc.STC_P_TRIPLEDOUBLE, 'fore:#800080,back:#FFFFEA')
                # Class names
                field.StyleSetSpec(wx.stc.STC_P_CLASSNAME, 'fore:#0000FF,bold')
                # Function names
                field.StyleSetSpec(wx.stc.STC_P_DEFNAME, 'fore:#008080,bold')
                # Operators
                field.StyleSetSpec(wx.stc.STC_P_OPERATOR, 'fore:#800000,bold')
                # Identifiers. I leave this as not bold because everything seems
                # to be an identifier if it doesn't match the above criterae
                field.StyleSetSpec(wx.stc.STC_P_IDENTIFIER, 'fore:#000000')
                # Caret color
                field.SetCaretForeground("BLUE")
                # Selection background
                field.SetSelBackground(1, '#66CCFF')

                if (value!=None):
                    field.SetText(value.encode("utf8"))
            elif meta_class_dict[key].kind == 'reference':
                label = wx.StaticText(self.panel, label=meta_class_dict[key].display_name + ":")
                refObjTypes = model_dict[self.editObj.kind].out_refs
                field = wx.ComboBox(self.panel, choices = [], style=wx.CB_READONLY)
                for ref in self.references:
                    field.Append(ref.properties['name'],ref)
                if value != None:
                    field.SetStringSelection(value.properties['name'])
                else:
                    field.SetSelection(0)
            if label != None and field != None:
                self.inputs[key] = field
                self.pbox.AddMany([(label),(field,1,wx.EXPAND)])
                if growRow:
                    self.pbox.AddGrowableRow(rNum,1)
                else:
                    self.pbox.RemoveGrowableRow(rNum)
                rNum += 1
            else:
                del label
        self.pbox.AddGrowableCol(1,1)

        self.panel.SetSizer(self.pbox)

        self.hbox = wx.BoxSizer(wx.HORIZONTAL)
        okButton = wx.Button(self, label='Ok')
        closeButton = wx.Button(self, label='Close')
        self.hbox.Add(okButton)
        self.hbox.Add(closeButton, flag=wx.LEFT, border=5)

        self.vbox.Add(self.panel, proportion=1, 
            flag=wx.ALL|wx.EXPAND, border=5)
        self.vbox.Add(self.hbox, 
            flag=wx.ALIGN_CENTER|wx.BOTTOM, border=10)

        self.SetSizer(self.vbox)
        
        okButton.Bind(wx.EVT_BUTTON, self.OnOk)
        closeButton.Bind(wx.EVT_BUTTON, self.OnClose)

    def UpdateInputs(self):
        for key,field in self.inputs.iteritems():
            if meta_class_dict[key].kind == "string":
                self.returnDict[key] = field.GetValue()
            elif meta_class_dict[key].kind == "list":
                self.returnDict[key] = field.GetValue()
            elif meta_class_dict[key].kind == "boolean":
                self.returnDict[key] = field.GetValue()
            elif meta_class_dict[key].kind == "code":
                fieldTxt = field.GetText()
                self.returnDict[key] = fieldTxt
            elif meta_class_dict[key].kind == "reference":
                obj = field.GetClientData(field.GetSelection())
                if obj == None:
                    ErrorDialog(self, "You must select a reference object!")
                    return False
                self.returnDict[key] = obj
        return True

    def GetInput(self):
        return self.returnDict

    def OnOk(self,e):
        if self.UpdateInputs() == False:
            self.returnDict = OrderedDict()
        self.MakeModal(False)
        self.Close()
        self.Destroy()

    def OnClose(self, e):
        self.returnDict = OrderedDict()
        self.MakeModal(False)
        self.Close()
        self.Destroy()

def EditorWindow(parent = None, editDict = OrderedDict(), editObj = None, title = "ROSMOD", references = [], style = wx.RESIZE_BORDER | wx.STAY_ON_TOP):
    ed = EditDialog( parent = parent,
                     title = title,
                     style = style,
                     editObj = editObj,
                     editDict = editDict,
                     references = references )
    ed.ShowModal()
    inputs = ed.GetInput()
    ed.Destroy()
    return inputs

class Wizard:
    def __init__(self, parent, propDictDict):
        self.parent = parent
        self.propDictDict = propDictDict

    def GetInput(self):
        for objName,propDict in self.propDictDict.iteritems():
            inputs = EditorWindow( parent = self.parent,
                                   editObj = None,
                                   editDict = propDict,
                                   title = "Configure {}".format(objName),
                                   style = wx.RESIZE_BORDER)
            if inputs != OrderedDict():
                self.propDictDict[objName] = inputs
            else:
                return None
        return self.propDictDict
