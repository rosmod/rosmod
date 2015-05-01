import wx
import wx.lib.agw.flatnotebook as fnb
import wx.stc as stc
from terminal import *            

class RedirectText(object):
    def __init__(self,aWxTextCtrl):
        self.out=aWxTextCtrl

    def write(self,string):
        self.out.SetReadOnly(False)
        self.out.AddText(string)
        #self.out.SaveFile("output.txt")
        self.out.LineScroll(0,10)
        self.out.SetReadOnly(True)

'''
Build the output notebook for ROSMOD which holds:
* the program output
* a terminal
* any logs requested from deployment
'''
def BuildOutput(self):
    self.output = fnb.FlatNotebook(self.viewSplitter, wx.ID_ANY)

    panel = wx.Panel(self.output)
    vbox = wx.BoxSizer(wx.VERTICAL)
    tc = stc.StyledTextCtrl(panel)
    tc.SetReadOnly(True)
    vbox.Add(tc, proportion=1, flag=wx.EXPAND)
    redir=RedirectText(tc)
    sys.stdout=redir
    sys.stderr=redir
    panel.SetSizer(vbox)

    self.output.AddPage(panel, "Console Output")
    self.output.AddPage(TermEmulatorDemo(self.output), "Terminal")


def SSHToHostInst(self,hostInst):
    self.shop.Check(True)
    self.UpdateMainWindow(None)
    command = "/usr/bin/ssh"
    args = "-i {} {}@{}".format( hostInst.properties['sshkey'], 
                                 hostInst.properties['username'],
                                 hostInst.properties['host_reference'].properties['ip_address'])
    self.output.AddPage(TermEmulatorDemo(self.output,
                                         command=command,
                                         args=args), 
                        "SSH To {}".format(hostInst.properties['name']), 
                        select=True)

def MonitorNodeInstLog(self,nodeInst):
    self.shop.Check(True)
    self.UpdateMainWindow(None)
    command = "/usr/bin/ssh"
    args = "-i {} {}@{} source /opt/ros/indigo/setup.bash; tail -f `roslaunch-logs`/rosout.log".format( 
        nodeInst.parent.properties['sshkey'], 
        nodeInst.parent.properties['username'],
        nodeInst.parent.properties['host_reference'].properties['ip_address'])
    self.output.AddPage(TermEmulatorDemo(self.output,
                                         command=command,
                                         args=args,
                                         cols=120),
                        "{} Log".format(nodeInst.properties['name']), 
                        select=True)
