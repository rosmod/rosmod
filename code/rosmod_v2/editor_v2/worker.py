import wx
from threading import Thread

EVT_DEPLOYMENT_UPDATE_ID = wx.NewId()
EVT_MONITORING_UPDATE_ID = wx.NewId()
EVT_HOSTDICT_UPDATE_ID = wx.NewId()

def EVT_DEPLOYMENT_UPDATE(win, func):
    win.Connect(-1,-1, EVT_DEPLOYMENT_UPDATE_ID, func)

def EVT_MONITORING_UPDATE(win, func):
    win.Connect(-1,-1, EVT_MONITORING_UPDATE_ID, func)

def EVT_HOSTDICT_UPDATE(win, func):
    win.Connect(-1,-1, EVT_HOSTDICT_UPDATE_ID, func)

class DeploymentEvent(wx.PyEvent):
    def __init__(self, data):
        wx.PyEvent.__init__(self)
        self.SetEventType(EVT_DEPLOYMENT_UPDATE_ID)
        self.data = data

class MonitoringEvent(wx.PyEvent):
    def __init__(self, data):
        wx.PyEvent.__init__(self)
        self.SetEventType(EVT_MONITORING_UPDATE_ID)
        self.data = data

class HostDictEvent(wx.PyEvent):
    def __init__(self, data):
        wx.PyEvent.__init__(self)
        self.SetEventType(EVT_HOSTDICT_UPDATE_ID)
        self.data = data

class WorkItem():
    def __init__(self,data,workFunc):
        self.data = data
        self.workFunc = workFunc

class WorkerThread(Thread):
    """Test Worker Thread Class."""
 
    #----------------------------------------------------------------------
    def __init__(self,func):
        """Init Worker Thread Class."""
        Thread.__init__(self)
        self.func = func
 
    #----------------------------------------------------------------------
    def run(self):
        """Run Worker Thread."""
        # This is the code executing in the new thread.
        self.func()

