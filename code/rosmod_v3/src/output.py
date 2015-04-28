import wx
import wx.lib.agw.flatnotebook as fnb
from terminal import *            

'''
Build the output notebook for ROSMOD which holds:
* the program output
* a terminal
* any logs requested from deployment
'''
def BuildOutput(self):
    self.output = fnb.FlatNotebook(self.viewSplitter, wx.ID_ANY)
    self.output.AddPage(wx.Panel(self.output), "Console Output")
    self.output.AddPage(TermEmulatorDemo(self.output), "Terminal")
