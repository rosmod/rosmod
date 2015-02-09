#!/usr/bin/python

import wx

app = wx.App(False)

frame = wx.Frame(None, title="Draw on Panel")
panel = wx.Panel(frame)

def draw_model(dc):
    dc.SetPen(wx.Pen("BLACK",2))
    dc.SetBrush(wx.Brush("BLUE",wx.BDIAGONAL_HATCH))
    dc.DrawLine(0,0,50,50)
    dc.DrawImageLabel("component",wx.Bitmap("texit.png"),(60,60,60,60))
    dc.DrawRoundedRectangle(10,30,50,50,10)
    dc.DrawSpline([wx.Point(60,60),wx.Point(100,100),wx.Point(10,150)])

def on_paint(event):
    dc = wx.PaintDC(event.GetEventObject())
    dc.Clear()
    draw_model(dc)

panel.Bind(wx.EVT_PAINT, on_paint)

frame.Show(True)
app.MainLoop()
