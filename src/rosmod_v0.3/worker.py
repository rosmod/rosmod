import wx
from threading import Thread

import deployment

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

    self.updatedHostDict = False

    self.workTimerPeriod = 5.0
    self.workTimerID = wx.NewId()  # pick a number
    self.workTimer = wx.Timer(self, self.workTimerID)  # message will be sent to the panel
    self.workTimer.Start(self.workTimerPeriod*1000)  # x100 milliseconds
    wx.EVT_TIMER(self, self.workTimerID, lambda e : WorkFunction(self,e))  # call the on_timer function

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
        nodes = self.runningDeployment.getChildrenByKind('Node')
        nodeMap = {}
        for n in nodes:
            nodeMap[n.properties['name']] = n
        data = workItem.queue.get(False)
        while data != None:
            #print "GOT DATA: {}".format(data)
            dataList = data[0].split(' ')
            nodeName = dataList[0]
            node = nodeMap[nodeName]
            if dataList[1] == "UP":
                node.style['overlay']['outlineColor']=self.overlayDict['nodeUp']
            else:
                node.style['overlay']['outlineColor']=self.overlayDict['nodeDown']
            updateCanvas = True
            data = workItem.queue.get(False)
    except:
        # if we get here, we've read everything from the q
        if updateCanvas:
            self.DrawModel(self.runningDeployment,self.runningDeploymentCanvas)
    if not workItem.process.is_alive(): # process has terminated
        # update deployment overlays here
        workerThread = WorkerThread(func = lambda : deployment.monitorTest(self,
                                                                           workItem.queue)
                                )
        workerThread.start()
        workItem.data = workerThread
