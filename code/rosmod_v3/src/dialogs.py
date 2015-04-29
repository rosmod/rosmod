#!/usr/bin/python

import wx
import wx.stc as stc
from wx.lib.pubsub import Publisher

from collections import OrderedDict

from metaclass import meta_class_dict
from metaModel import reference_dict,model_dict

class RMLProgressDialog(wx.Dialog):
    """
    Shows a Progres Gauge while an operation is taking place. May be cancellable
    which is possible when converting pdf/ps or loading models
    """
    def __init__(self, title, progress_q, numItems=100, cancellable=False):
        """Defines a gauge and a timer which updates the gauge."""
        wx.Dialog.__init__(self, None, title=title)
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

    def OnTimer(self, event):
        try:
            message = self.progress_q.get(False)
            if message != None:
                self.count += 1
                self.progress.SetValue(self.count)
                self.progressText.SetLabel( message )
                if self.count >= self.numItems:
                    self.ok.Enable()
                    self.timer.Stop()
        except:
            pass

    def on_cancel(self, event):
        self.timer.Stop()
        wx.CallAfter(self.Destroy)

def ProgressBarDialog(title,topic,numItems,cancellable=False):
    dlg = RMLProgressDialog(title=title,progress_topic=topic,numItems=numItems,cancellable=cancellable)
    dlg.ShowModal()
    dlg.Destroy()

def RMLFileDialog(frame,fileTypes,path,prompt,fd_flags):
    modelPath = None
    fileName = None
    dlg = wx.FileDialog(frame, prompt, path, "", fileTypes, fd_flags)
    if dlg.ShowModal() == wx.ID_OK:
        fileName = dlg.GetFilename()
        modelPath = dlg.GetDirectory()
    dlg.Destroy()
    return fileName, modelPath

def RMLDirectoryDialog(frame,path,prompt):
    workspacePath = None
    dlg = wx.DirDialog(frame, prompt, path)
    if dlg.ShowModal() == wx.ID_OK:
        workspacePath = dlg.GetPath()
    dlg.Destroy()
    return workspacePath

def InfoDialog(frame, info):
    dlg = wx.MessageDialog(frame, info, 'Info', wx.OK )
    dlg.ShowModal()
    dlg.Destroy()

def ErrorDialog(frame, msg):
    dlg = wx.MessageDialog(frame, msg, 'Error', wx.OK | wx.ICON_ERROR)
    dlg.ShowModal()
    dlg.Destroy()

def ConfirmDialog(frame, msg):
    dlg = wx.MessageDialog(frame, msg, 'Confirm', wx.YES_NO | wx.NO_DEFAULT | wx.ICON_QUESTION)
    retVal = dlg.ShowModal()
    dlg.Destroy()
    if retVal == wx.ID_YES:
        retVal = True
    else:
        retVal = False
    return retVal


class EditDialog(wx.Dialog):
    
    def __init__(self, *args, **kw):
        self.editDict = kw.pop('editDict', OrderedDict())
        self.editObj = kw.pop('editObj', None)
        self.invalidNames = kw.pop('invalidNames',[])
        title = kw.pop('title', "ROSMOD V2")
        self.references = kw.pop('references',[])
        super(EditDialog, self).__init__(*args,**kw)
        self.returnDict = OrderedDict()
        self.InitUI()
        self.SetTitle(title)

    def InitUI(self):
        
        panel = wx.Panel(self)
        vbox = wx.BoxSizer(wx.VERTICAL)
        pbox = wx.FlexGridSizer(rows=len(self.editDict.keys()),cols=2,vgap=9,hgap=25)

        rNum = 0
        self.inputs = OrderedDict()
        for key,value in self.editDict.iteritems():
            label = None
            field = None
            if meta_class_dict[key].kind == "string":
                # anything that takes a string and shouldn't have a newline
                label = wx.StaticText(panel, label=key + ":")
                field = wx.TextCtrl(panel)
                if value != "" and value != None and value != []:
                    field.AppendText(value)
                self.inputs[key] = field
            elif meta_class_dict[key].kind == "code":
                # anything that takes a multi-line string
                # supports code completion and syntax highlighting
                label = wx.StaticText(panel, label=key + ":")
                field = stc.StyledTextCtrl(panel)
                fieldStr = value
                if value != None and value != []:
                    fieldStr = self.GenerateFieldString(value)
                field.SetText(fieldStr)
                field.EmptyUndoBuffer()
                field.Colourise(0,-1)
                field.SetMarginType(1, stc.STC_MARGIN_NUMBER)
                pbox.AddGrowableRow(rNum,1)
                self.inputs[key] = field
            elif meta_class_dict[key].kind == 'reference':
                refObjTypes = model_dict[self.editObj.kind].out_refs
                label = wx.StaticText(panel, label=key + ":")
                field = wx.ComboBox(panel, choices = [], style=wx.CB_READONLY)
                for ref in self.references:
                    field.Append(ref.properties['name'],ref)
                if value != None:
                    field.SetStringSelection(value.properties['name'])
                else:
                    field.SetSelection(0)
                self.inputs[key] = field
            if label != None and field != None:
                pbox.AddMany([(label),(field,1,wx.EXPAND)])
            rNum += 1

        pbox.AddGrowableCol(1,1)

        panel.SetSizer(pbox)

        hbox = wx.BoxSizer(wx.HORIZONTAL)
        okButton = wx.Button(self, label='Ok')
        closeButton = wx.Button(self, label='Close')
        hbox.Add(okButton)
        hbox.Add(closeButton, flag=wx.LEFT, border=5)

        vbox.Add(panel, proportion=1, 
            flag=wx.ALL|wx.EXPAND, border=5)
        vbox.Add(hbox, 
            flag=wx.ALIGN_CENTER|wx.BOTTOM, border=10)

        self.SetSizer(vbox)
        
        okButton.Bind(wx.EVT_BUTTON, self.OnOk)
        closeButton.Bind(wx.EVT_BUTTON, self.OnClose)

    def GenerateFieldString(self,fieldsList):
        retStr = ""
        for field in fieldsList:
            fStr = ""
            if len(field) == 2:
                fStr = ' '.join(field)
            elif len(field) == 3:
                fStr = '{} {} = {}'.format(field[0],field[1],field[2])
            if fStr != "":
                retStr += fStr + '\n'
        return retStr

    def ParseFieldLine(self,fieldLine):
        retField = []
        tmp = fieldLine.split(" ")
        if len(tmp) >= 2:
            retField = [x for x in tmp if x != "="]
        return retField

    def ParseFields(self,fieldStr):
        retFields = []
        fields = fieldStr.split('\n')
        for field in fields:
            fLst = self.ParseFieldLine(field)
            if fLst != []:
                retFields.append(fLst)
        return retFields

    def UpdateInputs(self):
        for key,field in self.inputs.iteritems():
            if meta_class_dict[key].kind == "string":
                self.returnDict[key] = field.GetValue()
            elif meta_class_dict[key].kind == "code":
                fieldTxt = field.GetText()
                retFields = self.ParseFields(fieldTxt)
                self.returnDict[key] = retFields
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
            return False
        self.Destroy()

    def OnClose(self, e):
        self.returnDict = OrderedDict()
        self.Destroy()
