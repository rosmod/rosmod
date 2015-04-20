#!/usr/bin/python

import wx
import wx.lib.scrolledpanel as scrolled

app = wx.App(False)

frame = wx.Frame(None, title="Draw on Panel")
panel = scrolled.ScrolledPanel(frame)

def draw_model(dc):
    dc.SetPen(wx.Pen("BLACK",2))
    dc.SetBrush(wx.Brush("BLUE",wx.BDIAGONAL_HATCH))
    sx,sy = panel.CalcScrolledPosition(0,0)
    ex,ey = panel.CalcScrolledPosition(500,50)
    dc.DrawLine(
        sx,sy,
        ex,ey
    )

    sx,sy = panel.CalcScrolledPosition(60,60)
    ex,ey = panel.CalcScrolledPosition(60,60)
    dc.DrawImageLabel("component",wx.Bitmap("texit.png"),(sx,sy,ex,ey))

    sx,sy = panel.CalcScrolledPosition(10,30)
    dc.DrawRoundedRectangle(sx,sy,50,50,10)
    
    sx,sy = panel.CalcScrolledPosition(60,60)
    ex,ey = panel.CalcScrolledPosition(100,100)
    x,y = panel.CalcScrolledPosition(10,150)
    dc.DrawSpline([wx.Point(sx,sy),wx.Point(ex,ey),wx.Point(x,y)])

def on_paint(event):
    dc = wx.PaintDC(event.GetEventObject())
    dc.Clear()
    draw_model(dc)

panel.Bind(wx.EVT_PAINT, on_paint)

panel.SetupScrolling()

frame.Show(True)
app.MainLoop()
