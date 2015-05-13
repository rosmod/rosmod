import wx
import wx.lib.agw.flatnotebook as fnb
import wx.stc as stc
from terminal import *            

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
        pass
    __call__ = write

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
