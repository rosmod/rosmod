import wx
import wx.lib.agw.flatnotebook as fnb
import wx.stc as stc
from terminal import *        
import subprocess    

from metaModel import model_dict
import deployment

import time
import re
def is_running(process):
    s = subprocess.Popen(["ps", "axw"],stdout=subprocess.PIPE)
    for x in s.stdout:
        if re.search(process, x):
            return True
    return False

class Log(stc.StyledTextCtrl):
    """
    Subclass the StyledTextCtrl to provide  additions
    and initializations to make it useful as a log window.

    """
    def __init__(self, parent, style=wx.SIMPLE_BORDER):
        """
        Constructor
        
        """
        stc.StyledTextCtrl.__init__(self, parent, style=style)
        self._styles = [None]*32
        self._free = 1
        
    def getStyle(self, c='black'):
        """
        Returns a style for a given colour if one exists.  If no style
        exists for the colour, make a new style.
        
        If we run out of styles, (only 32 allowed here) we go to the top
        of the list and reuse previous styles.

        """
        free = self._free
        if c and isinstance(c, (str, unicode)):
            c = c.lower()
        else:
            c = 'black'
        
        try:
            style = self._styles.index(c)
            return style
            
        except ValueError:
            style = free
            self._styles[style] = c
            self.StyleSetForeground(style, wx.NamedColour(c))

            free += 1
            if free >31:
                free = 0
            self._free = free
            return style

    def write(self, text, c=None):
        """
        Add the text to the end of the control using colour c which
        should be suitable for feeding directly to wx.NamedColour.
        
        'text' should be a unicode string or contain only ascii data.
        """
        self.SetReadOnly(False)
        self.DocumentEnd()
        style = self.getStyle(c)
        lenText = len(text.encode('utf8'))
        end = self.GetLength()
        self.AppendText(text)
        self.StartStyling(end, 31)
        self.SetStyling(lenText, style)
        self.EnsureCaretVisible()
        self.SetReadOnly(True)

    __call__ = write

class log_Wrapper(object):
    def __init__(self,log,color):
        self.color = color
        self.log = log
    def write(self,text):
        wx.CallAfter(self.log.write, text, self.color)
    def flush(self):
        pass #wx.CallAfter(self.log.write, "", self.color)
    __call__ = write

class OutTextThread(threading.Thread):
    def __init__(self, log_wrapper, std_out):
        super(OutTextThread, self).__init__()
        self.std_out = std_out
        self.log_wrapper=log_wrapper

    def run(self):
        text = None
        while text != '':
            if self.std_out != None:
                text = self.std_out.readline()
                self.log_wrapper.write(text)
            else:
                text = ''

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
    redir = Log(panel)
    redir.SetReadOnly(True)
    vbox.Add(redir, proportion=1, flag=wx.EXPAND)
    stdout_log = log_Wrapper(redir,'black')
    stderr_log = log_Wrapper(redir,'red')
    sys.stdout=stdout_log
    sys.stderr=stderr_log
    panel.SetSizer(vbox)

    self.output.AddPage(panel, "Console")
    self.output.AddPage(TermEmulatorDemo(self.output), "Terminal")

def OpenFile(self,kind,e):
    packageName = self.activeObject.parent.properties['name']
    workspaceName = self.activeObject.parent.parent.properties['name']
    componentName = self.activeObject.properties['name']
    command = None
    if kind == 'cpp':
        fileName = self.project.workspace_path + '/' + workspaceName + '/src/' + packageName + '/src/' + packageName + '/' + componentName + '.cpp'
        command = ['emacsclient',fileName]
    elif kind == 'hpp':
        fileName = self.project.workspace_path + '/' + workspaceName + '/src/' + packageName + '/include/' + packageName + '/' + componentName + '.hpp'
        command = ['emacsclient',fileName]
    elif kind == 'all':
        fileName1 = self.project.workspace_path + '/' + workspaceName + '/src/' + packageName + '/src/' + packageName + '/' + componentName + '.cpp'
        fileName2 = self.project.workspace_path + '/' + workspaceName + '/src/' + packageName + '/include/' + packageName + '/' + componentName + '.hpp'
        command = ['emacsclient',fileName1,fileName2]
    if command != None:
        if is_running("emacs"):
            pass
        else:
            p1 = subprocess.Popen(['emacs',
                                   '-mm'],
                                  shell=False)
            time.sleep(2)
        p = subprocess.Popen(command,
                             stdout=subprocess.PIPE,
                             stderr=subprocess.PIPE,
                             shell=False)
        out_thread1 = OutTextThread(sys.stdout,p.stdout)
        out_thread1.start()
        out_thread2 = OutTextThread(sys.stderr,p.stderr)
        out_thread2.start()            

def SSHToHost(self,e):
    host = self.activeObject
    self.shop.Check(True)
    self.UpdateMainWindow(None)
    command = "/usr/bin/ssh"
    args = "-i {} {}@{}".format( os.path.expanduser(host.properties['sshkey']), 
                                 host.properties['username'],
                                 host.properties['ip_address'])
    self.output.AddPage(TermEmulatorDemo(self.output,
                                         command=command,
                                         args=args), 
                        "SSH To {}".format(host.properties['name']), 
                        select=True)

def MonitorNodeLog(self,e):
    node = self.activeObject
    self.shop.Check(True)
    self.UpdateMainWindow(None)
    host = node.properties['hardware_reference']
    fileString = ""
    for compInst in node.children:
        fileString += "{}/{}.{}.log ".format(host.properties["deployment_path"], node.properties['name'],compInst.properties['name'])
    if host.properties["ip_address"] not in deployment.local_ips:
        command = "/usr/bin/ssh"
        args = "-i {} {}@{} tail -f {}".format( 
            os.path.expanduser(host.properties['sshkey']), 
            host.properties['username'],
            host.properties['ip_address'],
            fileString)
    else:
        command = "/usr/bin/tail"
        args = "-f {}".format(fileString)

    self.output.AddPage(TermEmulatorDemo(self.output,
                                         command=command,
                                         args=args,
                                         cols=120),
                        "{} Log".format(node.properties['name']), 
                        select=True)


def MonitorCompInstLog(self,e):
    compInst = self.activeObject
    self.shop.Check(True)
    self.UpdateMainWindow(None)
    host = compInst.parent.properties['hardware_reference']
    if host.properties["ip_address"] not in deployment.local_ips:
        command = "/usr/bin/ssh"
        args = "-i {} {}@{}  tail -f {}/{}.{}.log".format( 
            os.path.expanduser(host.properties['sshkey']), 
            host.properties['username'],
            host.properties['ip_address'],
            host.properties["deployment_path"],
            compInst.parent.properties['name'],
            compInst.properties['name'])
    else:
        command = "/usr/bin/tail"
        args = "-f {}/{}.{}.log".format(host.properties["deployment_path"], compInst.parent.properties["name"], compInst.properties["name"])

    self.output.AddPage(TermEmulatorDemo(self.output,
                                         command=command,
                                         args=args,
                                         cols=120),
                        "{} Log".format(compInst.properties['name']), 
                        select=True)
