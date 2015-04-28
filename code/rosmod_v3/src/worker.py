import wx
from wx.lib.pubsub import Publisher
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
    def __init__(self,process,queue,workFunc):
        self.queue = queue
        self.process = process
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

def InitWorkQueue(self):
    self.workQueue = []
    self.workTimerPeriod = 5.0
    self.workTimerID = wx.NewId()  # pick a number
    self.workTimer = wx.Timer(self, self.workTimerID)  # message will be sent to the panel
    self.workTimer.Start(self.workTimerPeriod*1000)  # x100 milliseconds
    wx.EVT_TIMER(self, self.workTimerID, self.WorkFunction)  # call the on_timer function

    self.hostDictTopic = "hostDictTopic"                      # used for updating the host Dict from fabric
    self.monitorStatusTopic = "monitorStatusTopic"            # used for updating the gui from monitor
    self.deploymentProgressTopic = "deploymentProgressTopic"  # used for progress bars
    Publisher().subscribe(self.OnSubscribeMonitorStatus, self.monitorStatusTopic)
    Publisher().subscribe(self.OnSubscribeHostDictChange, self.hostDictTopic)


def OnSubscribeMonitorStatus(self,message):
    pass

def OnSubscribeHostDictChange(self,message):
    self.updatedHostDict = True
    self.hostDict = message.data

'''
This function is what handles the work of updating the gui by communicating with other processes
that the gui has started, e.g. the deployment or monitoring processes.
'''
def WorkFunction(self,e):
    if len(self.workQueue) > 0:
        for workItem in self.workQueue:
            workItem.workFunc(workItem)

def MonitorWorkFunc(self,workItem):
    # for this function, data is the parallel multiprocess started for monitoring
    if self.deployed != True:
        self.workQueue.remove(workItem)
        return
    # get data from queue
    updateCanvas = False
    try:
        nodes = self.runningDeployment.getChildrenByKind('node_instance')
        nodeMap = {}
        for n in nodes:
            nodeMap[n.properties['name']] = n
        data = workItem.queue.get(False)
        while data != None:
            #print "GOT DATA: {}".format(data)
            dataList = data.split(' ')
            nodeName = dataList[0]
            node = nodeMap[nodeName]
            if dataList[1] == "UP":
                node.style.overlay['overlayColor']='GREEN'
            else:
                node.style.overlay['overlayColor']='RED'
            updateCanvas = True
            data = workItem.queue.get(False)
    except:
        # if we get here, we've read everything from the q
        if updateCanvas:
            self.DrawModel(self.runningDeployment,self.runningDeploymentCanvas)
    if not workItem.process.is_alive(): # process has terminated
        # update deployment overlays here
        workerThread = WorkerThread(func = lambda : fabTest.monitorTest(self.hostDict,
                                                                        self.hostDictTopic,
                                                                        workItem.queue)
                                )
        workerThread.start()
        workItem.data = workerThread
