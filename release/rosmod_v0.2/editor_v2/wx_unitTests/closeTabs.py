#!/usr/bin/env python
#-*- coding:utf-8 -*-

import wx

class Page(wx.Panel):
    def __init__(self, parent):
        wx.Panel.__init__(self, parent)
        t = wx.StaticText(self, -1, "THIS IS A PAGE OBJECT", (20,20))

class MainFrame(wx.Frame):
    def __init__(self):
        wx.Frame.__init__(self, None, title="Notebook Remove Pages Example")

        pannel  = wx.Panel(self)
        vbox    = wx.BoxSizer(wx.VERTICAL)
        hbox    = wx.BoxSizer(wx.HORIZONTAL)

        self.buttonRemove = wx.Button(pannel, id=wx.ID_ANY, label="DELETE", size=(80, 25))
        self.buttonRemove.Bind(wx.EVT_BUTTON, self.onButtonRemove)
        hbox.Add(self.buttonRemove)

        self.buttonInsert = wx.Button(pannel, id=wx.ID_ANY, label="CREATE", size=(80, 25))
        self.buttonInsert.Bind(wx.EVT_BUTTON, self.onButtonInsert)
        hbox.Add(self.buttonInsert)

        vbox.Add(hbox)

        self.Notebook3 = wx.Notebook(pannel)
        vbox.Add(self.Notebook3, 2, flag=wx.EXPAND)

        pannel.SetSizer(vbox)

        self.pageCounter = 0
        self.addPage()

    def addPage(self):
        self.pageCounter += 1
        page      = Page(self.Notebook3)
        pageTitle = "Page: {0}".format(str(self.pageCounter))
        self.Notebook3.AddPage(page, pageTitle)

    def onButtonRemove(self, event):   
        self.Notebook3.DeletePage(0)

    def onButtonInsert(self, event):   
        self.addPage()

if __name__ == "__main__":
    app = wx.App()
    MainFrame().Show()
    app.MainLoop()
