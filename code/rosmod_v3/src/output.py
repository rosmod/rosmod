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
