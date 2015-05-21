#!/usr/bin/python

import wx

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
