#!/usr/bin/python

import wx
import wx.stc as stc

from collections import OrderedDict

def RMLFileDialog(frame,fileTypes,path,prompt,fd_flags):
    modelPath = None
    fileName = None
    dlg = wx.FileDialog(frame, prompt, path, "", fileTypes, fd_flags)
    if dlg.ShowModal() == wx.ID_OK:
        fileName = dlg.GetFilename()
        modelPath = dlg.GetDirectory()
    dlg.Destroy()
    return fileName, modelPath

def RMLGenerateDirDialog(frame,path):
    workspacePath = None
    dlg = wx.DirDialog(frame, "Choose workspace directory", path)
    if dlg.ShowModal() == wx.ID_OK:
        workspacePath = dlg.GetPath()
    dlg.Destroy()
    return workspacePath

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
            #print key, value
            label = None
            field = None
            if key == 'name' or key == 'period' or key == 'unit':
                # anything that takes a string and shouldn't have a newline
                label = wx.StaticText(panel, label=key + ":")
                field = wx.TextCtrl(panel)
                field.AppendText(value)
                self.inputs[key] = field
            elif key == 'fields' or key == 'request' or key == 'response':
                # anything that takes a multi-line string
                # supports code completion and syntax highlighting
                label = wx.StaticText(panel, label=key + ":")
                field = stc.StyledTextCtrl(panel)
                fieldStr = self.GenerateFieldString(value)
                field.SetText(fieldStr)
                field.EmptyUndoBuffer()
                field.Colourise(0,-1)
                field.SetMarginType(1, stc.STC_MARGIN_NUMBER)
                pbox.AddGrowableRow(rNum,1)
                self.inputs[key] = field
            elif key == 'service_reference' or key == 'message_reference' or key == 'component_reference':
                label = wx.StaticText(panel, label=key + ":")
                refNames = [x.properties['name'] for x in self.references]
                field = wx.ComboBox(panel, choices = refNames, style=wx.CB_READONLY)
                field.SetValue(value.properties['name'])
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
        retField = fieldLine.split(" ")
        retField = [x for x in retField if x != "="]
        return retField

    def ParseFields(self,fieldStr):
        retFields = []
        fields = fieldStr.split('\n')
        for field in fields:
            retFields.append(self.ParseFieldLine(field))
        return retFields

    def UpdateInputs(self):
        for key,field in self.inputs.iteritems():
            if key == 'name' or key == 'period' or key == 'unit':
                self.returnDict[key] = field.GetValue()
            elif key == 'fields' or key == 'request' or key == 'response':
                fieldTxt = field.GetText()
                retFields = self.ParseFields(fieldTxt)
                self.returnDict[key] = retFields
            elif key == 'service_reference' or key == 'message_reference' or key == 'component_reference':
                objName = field.GetValue()
                obj = [x for x in self.references if x.properties['name'] == objName]
                self.returnDict[key] = obj[0]

    def GetInput(self):
        return self.returnDict

    def OnOk(self,e):
        self.UpdateInputs()
        self.Destroy()

    def OnClose(self, e):
        self.returnDict = OrderedDict()
        self.Destroy()
