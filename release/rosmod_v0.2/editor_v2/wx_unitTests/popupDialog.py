#!/usr/bin/python
# -*- coding: utf-8 -*-

'''
ZetCode wxPython tutorial

In this code example, we create a
custom dialog.

author: Jan Bodnar
website: www.zetcode.com
last modified: July 2012
'''

import wx

class ChangeDepthDialog(wx.Dialog):
    
    def __init__(self, *args, **kw):
        super(ChangeDepthDialog, self).__init__(*args, **kw) 
            
        self.InitUI()
        self.SetSize((250, 200))
        self.SetTitle("Change Color Depth")
        
        
    def InitUI(self):

        pnl = wx.Panel(self)
        vbox = wx.BoxSizer(wx.VERTICAL)

        sb = wx.StaticBox(pnl, label='Colors')
        sbs = wx.StaticBoxSizer(sb, orient=wx.VERTICAL)        
        sbs.Add(wx.RadioButton(pnl, label='256 Colors', 
            style=wx.RB_GROUP))
        sbs.Add(wx.RadioButton(pnl, label='16 Colors'))
        sbs.Add(wx.RadioButton(pnl, label='2 Colors'))
        
        hbox1 = wx.BoxSizer(wx.HORIZONTAL)        
        hbox1.Add(wx.RadioButton(pnl, label='Custom'))
        hbox1.Add(wx.TextCtrl(pnl), flag=wx.LEFT, border=5)
        sbs.Add(hbox1)
        
        pnl.SetSizer(sbs)
       
        hbox2 = wx.BoxSizer(wx.HORIZONTAL)
        okButton = wx.Button(self, label='Ok')
        closeButton = wx.Button(self, label='Close')
        hbox2.Add(okButton)
        hbox2.Add(closeButton, flag=wx.LEFT, border=5)

        vbox.Add(pnl, proportion=1, 
            flag=wx.ALL|wx.EXPAND, border=5)
        vbox.Add(hbox2, 
            flag=wx.ALIGN_CENTER|wx.TOP|wx.BOTTOM, border=10)

        self.SetSizer(vbox)
        
        okButton.Bind(wx.EVT_BUTTON, self.OnClose)
        closeButton.Bind(wx.EVT_BUTTON, self.OnClose)
        
        
    def OnClose(self, e):
        
        self.Destroy()
        
        
class Example(wx.Frame):
    
    def __init__(self, *args, **kw):
        super(Example, self).__init__(*args, **kw) 
            
        self.InitUI()
        
        
    def InitUI(self):    
    
        ID_DEPTH = wx.NewId()

        tb = self.CreateToolBar()
        tb.AddLabelTool(id=ID_DEPTH, label='', 
            bitmap=wx.Bitmap('tnew.png'))
        
        tb.Realize()

        self.Bind(wx.EVT_TOOL, self.OnChangeDepth, 
            id=ID_DEPTH)

        self.SetSize((300, 200))
        self.SetTitle('Custom dialog')
        self.Centre()
        self.Show(True)
        
        
    def OnChangeDepth(self, e):
        
        chgdep = ChangeDepthDialog(None, 
            title='Change Color Depth')
        chgdep.ShowModal()
        chgdep.Destroy()        


def main():
    
    ex = wx.App()
    Example(None)
    ex.MainLoop()    


if __name__ == '__main__':
    main()
