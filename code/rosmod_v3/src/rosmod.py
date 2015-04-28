#!/usr/bin/python
'''
This Program is designed to be the user interface
for the ROSMOD modeling language.  It has the ability
to create, load, display, edit, and export ROSMOD models.

Additionally, it supports the interaction with analysis
tools through the use of toolbar buttons and subprocesses.
'''

import wx
import wx.stc as stc
from wx.lib.pubsub import Publisher
import os,sys
import copy
import multiprocessing
from threading import Thread
from fabric.api import *

from collections import OrderedDict
from wx.lib.floatcanvas import NavCanvas, FloatCanvas, Resources, Utilities
try:
    import numpy
    import numpy.random as RandomArray
    haveNumpy = True
except ImportError:
            haveNumpy = False
            errorText = (
            "The FloatCanvas requires the numpy module, version 1.* \n\n"
            "You can get info about it at:\n"
            "http://numpy.scipy.org/\n\n"
            )

# THESE ARE ALL FROM OUR CODE
import project
from terminal import *
import dialogs
import drawable
from aspect import *
from output import *
from worker import *
from toolbar import *
from deployment import *
from contextMenu import *
from style import *

class Example(wx.Frame):
    def __init__(self, *args, **kwargs):
        super(Example, self).__init__(*args,**kwargs)
        self.InitUI()
        self.Maximize()
    
    def InitUI(self):

        self.activeObject = None

        self.fileTypes = "ROSMOD Project (*.rosmod)|*.rosmod"
        self.project_path = os.getcwd()
        self.project = project.ROS_Project()

        self.undoList = []
        self.redoList = []

        # build the main frame (holds viewer in the top and the output in the bottom)
        self.viewSplitter = wx.SplitterWindow(self,wx.ID_NEW,
                                              style=wx.SP_PERMIT_UNSPLIT|wx.SP_BORDER|wx.SP_3DBORDER)
        InitAspects(self)
        InitDeployment(self)
        BuildStyleDict(self)
        # build the MenuBar,Toolbar, and Statusbar
        self.BuildMenu()
        BuildToolbar(self)
        self.BuildStatusbar()

        BuildOutput(self)
        AddPackageAspectToolbar(self)
        self.viewSplitter.SplitHorizontally(self.activeAspect,self.output,-100)
        self.viewSplitter.Bind(wx.EVT_SPLITTER_DCLICK,self.OnSplitterDClick)
        
        self.toolbar.Realize()
        self.Layout()
        
        self.SetSize((800, 600))
        self.SetTitle('ROSMOD Editor V2')
        self.Centre()
        self.Show(True)

        InitWorkQueue(self)

        wx.EVT_CLOSE(self, self.OnQuit)


    def DrawModel(self, model, canvas):
        c = canvas.ViewPortCenter
        canvas.UnBindAll()
        canvas.ClearAll(ResetBB=False)
        canvas.SetProjectionFun(None)
        self.BindCanvasMouseEvents(canvas)
        width,height = drawable.Layout(model,(0,0),canvas)
        model.Draw(canvas,self.OnLeftClick,self.OnRightClick,self.OnLeftDoubleClick)
        canvas.Draw()
        if c != None:
            canvas.Zoom(1,c)

    def OnLeftClick(self, Object):
        if self.activeAspect == self.PackageAspect:
            self.OnPkgLeftClick(Object)
        elif self.activeAspect == self.HardwareAspect:
            self.OnHWLeftClick(Object)
        elif self.activeAspect == self.DeploymentAspect:
            self.OnDeploymentLeftClick(Object)

    def OnLeftDoubleClick(self, Object):
        self.AspectEdit(None)

    def OnPkgLeftClick(self, Object):
        info = self.GetActivePanelInfo()
        pkg = info.obj
        canvas = info.canvas
        self.activeObject = Object.Name
        drawable.Configure(pkg,self.styleDict)
        self.activeObject.style.overlay['overlayColor']="RED"
        kind = self.activeObject.kind
        keys = []
        if kind == 'Message':
            keys = [['Publisher','datatype'],['Subscriber','datatype']]
        elif kind == 'Service':
            keys = [['Server','datatype'],['Client','datatype']]
        elif kind == 'Component':
            keys = [['Component_Instance','Component_Reference']]
        for key in keys:
            children = pkg.getChildrenByKind(key[0])
            for child in children:
                if child.properties[key[1]] == self.activeObject:
                    child.style.overlay['overlayColor']='RED'
        self.DrawModel(pkg,canvas)

    def OnHWLeftClick(self, Object):
        info = self.GetActivePanelInfo()
        dep = info.obj
        canvas = info.canvas
        self.activeObject = Object.Name

    def OnDeploymentLeftClick(self, Object):
        info = self.GetActivePanelInfo()
        dep = info.obj
        canvas = info.canvas
        self.activeObject = Object.Name
        if self.deployed == True and dep == self.runningDeployment:
            return # DO SOMETHING ELSE HERE?
        drawable.Configure(dep,self.styleDict)
        self.activeObject.style.overlay['overlayColor']="RED"
        kind = self.activeObject.kind
        if kind == 'Node_Instance' or kind == 'Hardware_Instance':
            keys = []
            if kind == 'Node_Instance':
                keys = [['Node_Instance','Node_Reference']]
            elif kind == 'Hardware_Instance':
                keys = [['Hardware_Instance','Hardware_Reference']]
            for key in keys:
                children = dep.getChildrenByKind(key[0])
                for child in children:
                    if child.properties[key[1]] == self.activeObject.properties[key[1]]:
                        child.style.overlay['overlayColor']='RED'
        elif kind == 'Group':
            children = dep.getChildrenByKind('Node_Instance')
            myKeys = [x.properties['Node_Instance_Reference'] for x in self.activeObject.children]
            for child in children:
                if child in myKeys:
                    child.style.overlay['overlayColor'] = 'RED'
        elif kind == 'Port_Instance':
            children = dep.getChildrenByKind('Node_Instance')
            for child in children:
                if child == self.activeObject.properties['Node_Instance_Reference']:
                    child.style.overlay['overlayColor'] = 'RED'
        self.DrawModel(dep,canvas)

    def OnRightClick(self, Object):
        info = self.GetActivePanelInfo()
        canvas = info.canvas
        self.activeObject = Object.Name
        self.PopupMenu(ContextMenu(canvas,AspectContextMenu(self,self.activeObject)))

    def GenericAdd(self,newObj,refs,parent):
        info = self.GetActivePanelInfo()
        pkg = info.obj
        canvas = info.canvas
        msgWindow = info.msgWindow
        kind = newObj.kind
        if newObj != None:
            newObj.properties['name'] = "New" + kind
            ed = dialogs.EditDialog(self,
                                    editDict=newObj.properties,
                                    editObj = newObj,
                                    title="Edit "+newObj.kind,
                                    references = refs,
                                    style=wx.RESIZE_BORDER)
            ed.ShowModal()
            inputs = ed.GetInput()
            if inputs != OrderedDict():
                self.UpdateUndo()
                for key,value in inputs.iteritems():
                    newObj.properties[key] = value
                parent.add(newObj)
                self.AspectLog(
                    "Added child {} to parent {}".format(newObj.properties['name'],parent.properties['name']),
                    msgWindow)
                drawable.Configure(pkg,self.styleDict)
                self.DrawModel(pkg,canvas)            

    def CompAdd(self,e,kind):
        comp = self.activeObject
        newObj = None
        references = []
        info = self.GetActivePanelInfo()
        pkg = info.obj
        if kind == 'Timer':
            newObj = project.ROS_Timer()
        elif kind == 'Subscriber':
            newObj = project.ROS_Subscriber()
            newObj.parent = comp
            references = self.project.workspace.getChildrenByKind('Message')
        elif kind == 'Publisher':
            newObj = project.ROS_Publisher()
            newObj.parent = comp
            references = self.project.workspace.getChildrenByKind('Message')
        elif kind == 'Server':
            newObj = project.ROS_Server()
            newObj.parent = comp
            references = self.project.workspace.getChildrenByKind('Service')
        elif kind == 'Client':
            newObj = project.ROS_Client()
            newObj.parent = comp
            references = self.project.workspace.getChildrenByKind('Service')
        if newObj != None:
            self.GenericAdd(newObj,references,comp)


    def NodeAdd(self,e,kind):
        node = self.activeObject
        newObj = None
        references = []
        info = self.GetActivePanelInfo()
        pkg = info.obj
        if kind == 'Component_Instance':
            newObj = project.ROS_Component_Instance()
            references = node.parent.getChildrenByKind('Component')
        if newObj != None:
            self.GenericAdd(newObj,references,node)
    
    def PackageAdd(self,e,kind):
        package = self.activeObject
        newObj = None
        references = []
        if kind == 'message':
            newObj = project.ROS_Message()
        elif kind == 'service':
            newObj = project.ROS_Service()
        elif kind == 'component':
            newObj = project.ROS_Component()
        elif kind == 'node':
            newObj = project.ROS_Node()
        if newObj != None:
            self.GenericAdd(newObj,references,package)

    def HardwareAdd(self,e,kind):
        hw = self.activeObject
        newObj = None
        references = []
        if kind == 'Hardware':
            newObj = project.ROS_Host()
        if newObj != None:
            self.GenericAdd(newObj,references,hw)

    def DeploymentAdd(self,e,kind):
        dep = self.activeObject
        newObj = None
        references = []
        if kind == 'Hardware_Instance':
            newObj = project.ROS_Host_Instance()
            references = dep.properties['rdp_hardware'].children
        elif kind == 'Group':
            newObj = project.ROS_Group()
            references = self.project.workspace.getChildrenByKind('Node')
        if newObj != None:
            self.GenericAdd(newObj,references,dep)

    def HostInstAdd(self,e,kind):
        host = self.activeObject
        newObj = None
        references = []
        if kind == 'Node_Instance':
            newObj = project.ROS_Node_Instance()
            references = self.project.workspace.getChildrenByKind('Node')
        if newObj != None:
            self.GenericAdd(newObj,references,host)

    def GroupAdd(self,e, kind):
        group = self.activeObject
        newObj = None
        if kind == 'Port_Instance':
            newObj = project.ROS_Port_Instance()
            del newObj.properties['name']
            references = group.parent.getChildrenByKind('Node_Instance')
        if newObj != None:
            self.GenericAdd(newObj,references,group)

    def AspectEdit(self, e):
        info = self.GetActivePanelInfo()
        obj = info.obj
        canvas = info.canvas
        msgWindow = info.msgWindow
        self.AspectLog(
            "Editing {} of type {}".format(self.activeObject.properties['name'],self.activeObject.kind),
            msgWindow)
        references = []
        if self.activeObject.kind == 'Publisher' or self.activeObject.kind == 'Subscriber':
            references = self.project.workspace.getChildrenByKind('Message')
        elif self.activeObject.kind == 'Server' or self.activeObject.kind == 'Client':
            references = self.project.workspace.getChildrenByKind('Service')
        elif self.activeObject.kind == 'Component_Instance':
            references = self.activeObject.parent.parent.getChildrenByKind('Component')
        elif self.activeObject.kind == 'rdp':
            references = self.project.hardware_files
        elif self.activeObject.kind == 'Hardware_Instance':
            references = obj.properties['rdp_hardware'].children
        elif self.activeObject.kind == 'Node_Instance':
            references = self.project.workspace.getChildrenByKind('Node')
        elif self.activeObject.kind == 'Port_Instance':
            references = obj.getChildrenByKind('Node_Instance')

        prevProps = copy.copy(self.activeObject.properties)
        ed = dialogs.EditDialog(canvas,
                                editDict=self.activeObject.properties,
                                editObj = self.activeObject,
                                title="Edit "+self.activeObject.kind,
                                references = references,
                                style=wx.RESIZE_BORDER)
        ed.ShowModal()
        inputs = ed.GetInput()
        if inputs != OrderedDict():
            self.UpdateUndo()
            for key,value in inputs.iteritems():
                self.activeObject.properties[key] = value
            if self.activeObject.kind == 'Package' or \
               self.activeObject.kind == 'rml' or \
               self.activeObject.kind == 'rhw' or \
               self.activeObject.kind == 'rdp':
                newName = self.activeObject.properties['name']
                prevName = prevProps['name']
                info.name = newName
                self.activeAspectInfo.AddPageInfo(info)
                if newName != prevName:
                    self.activeAspectInfo.DelPageInfo(prevName)
            if self.activeObject.kind == 'rdp':
                prevRef = prevProps['rdp_hardware']
                newRef = self.activeObject.properties['rdp_hardware']
                if newRef != prevRef:
                    self.activeObject.children = []
            drawable.Configure(obj,self.styleDict)
            selectedPage = self.activeAspect.GetSelection()
            self.activeAspect.SetPageText(selectedPage,obj.properties['name'])
            self.DrawModel(obj,canvas)
        else:
            self.activeObject.properties = prevProps

    def AspectDelete(self, e):
        info = self.GetActivePanelInfo()
        model = info.obj
        canvas = info.canvas
        msgWindow = info.msgWindow
        if model.kind != 'rml':
            if self.activeObject.kind == 'Package':
                wx.CallAfter(self.OnPackageDelete,e)
            elif self.activeObject.kind == 'rhw':
                wx.CallAfter(self.OnHardwareDelete,e)
            elif self.activeObject.kind == 'rdp':
                wx.CallAfter(self.OnDeploymentDelete,e)
            else:
                if dialogs.ConfirmDialog(canvas,"Delete {}?".format(self.activeObject.properties['name'])):
                    self.UpdateUndo()
                    self.AspectLog("Deleting {}".format(self.activeObject.properties['name']),msgWindow)
                    self.activeObject.deleteAllRefs(self.project)
                    self.activeObject.delete()
                    drawable.Configure(model,self.styleDict)
                    self.DrawModel(model,canvas)
        else:
            dialogs.ErrorDialog(self,"Cannot delete workspace!")
        self.activeObject = None

    def OnHardwareCreate(self, e):
        newObj = project.ROS_HW()
        newObj.properties = {}
        newObj.properties['name'] = "New Hardware Configuration"
        existingNames = [x.properties['name'] for x in self.project.hardware_files.children]
        ed = dialogs.EditDialog(self,
                                editObj=newObj,
                                editDict=newObj.properties,
                                title="Edit "+newObj.kind,
                                references = [],
                                invalidNames = existingNames,
                                style=wx.RESIZE_BORDER)
        ed.ShowModal()
        inputs = ed.GetInput()
        if inputs != OrderedDict():
            self.UpdateUndo()
            for key,value in inputs.iteritems():
                newObj.properties[key] = value
            self.project.hardware_files.append(newObj)
            numPages = self.HardwareAspect.GetPageCount()
            if numPages <= 0:
                numPages = 1
            self.BuildModelPage(self.HardwareAspect,newObj,self.HardwareAspectInfo,numPages-1)
            self.HardwareAspect.SetSelection(numPages - 1)
    def OnHardwareDelete(self, e):
        selectedPage = self.HardwareAspect.GetSelection()
        numPages = self.HardwareAspect.GetPageCount()
        objName = self.HardwareAspect.GetPageText(selectedPage)
        info = self.HardwareAspectInfo.GetPageInfo(objName)
        obj = info.obj
        if dialogs.ConfirmDialog(self,"Delete {}?".format(objName)):
            self.UpdateUndo()
            info.canvas.ClearAll()
            self.project.hardware_files = [x for x in self.project.hardware_files if x != obj]
            self.HardwareAspect.GetPage(selectedPage).DestroyChildren()
            self.HardwareAspectInfo.DelPageInfo(obj.properties['name'])
            self.HardwareAspect.DeletePage(selectedPage)
            os.remove(self.project.hardware_files_path + '/' + obj.properties['name'] + '.rhw')
        
    def OnDeploymentCreate(self, e):
        newObj = project.ROS_Deployment()
        newObj.properties = OrderedDict()
        newObj.properties['name'] = "New Deployment"
        references = self.project.hardware_files
        existingNames = [x.properties['name'] for x in self.project.deployments.children]
        ed = dialogs.EditDialog(self,
                                editObj=newObj,
                                editDict=newObj.properties,
                                title="Edit "+newObj.kind,
                                references = references,
                                invalidNames = existingNames,
                                style=wx.RESIZE_BORDER)
        ed.ShowModal()
        inputs = ed.GetInput()
        if inputs != OrderedDict():
            self.UpdateUndo()
            for key,value in inputs.iteritems():
                newObj.properties[key] = value
            self.project.deployments.append(newObj)
            numPages = self.DeploymentAspect.GetPageCount()
            if numPages <= 0:
                numPages = 1
            self.BuildModelPage(self.DeploymentAspect,newObj,self.DeploymentAspectInfo,numPages-1)
            self.DeploymentAspect.SetSelection(numPages - 1)
                
    def OnDeploymentDelete(self, e):
        selectedPage = self.DeploymentAspect.GetSelection()
        objName = self.DeploymentAspect.GetPageText(selectedPage)
        info = self.DeploymentAspectInfo.GetPageInfo(objName)
        obj = info.obj
        if dialogs.ConfirmDialog(self,"Delete {}?".format(objName)):
            self.UpdateUndo()
            info.canvas.ClearAll()
            self.project.deployments = [x for x in self.project.deployments if x != obj]
            self.DeploymentAspect.GetPage(selectedPage).DestroyChildren()
            self.DeploymentAspectInfo.DelPageInfo(obj.properties['name'])
            self.DeploymentAspect.DeletePage(selectedPage)
            os.remove(self.project.deployment_path + '/' + obj.properties['name'] + '.rdp')

    def OnDeploymentGenerate(self,e):
        pass

    def OnDeploymentMove(self,e):
        if self.deployed == False:
            selectedPage = self.DeploymentAspect.GetSelection()
            objName = self.DeploymentAspect.GetPageText(selectedPage)
            info = self.DeploymentAspectInfo.GetPageInfo(objName)
            dep = info.obj
            canvas = info.canvas
            env.use_ssh_config = False
            self.hostDict = {}
            env.hosts = []
            for host in dep.getChildrenByKind("Hardware_Instance"):
                nodeList = []
                self.hostDict[host.properties['name']] = fabTest.deployed_host(
                    userName = host.properties['username'],
                    ipAddress = host.properties['host_reference'].properties['ip_address'],
                    keyFile = host.properties['sshkey'],
                    nodes = nodeList,
                    envVars = copy.copy(host.properties['env_variables'])
                )
                env.hosts.append(host.properties['name'])
            copyProgressQ = multiprocessing.Queue()
            dlg = dialogs.RMLProgressDialog( title="Copy Progress",
                                             progress_q = copyProgressQ,
                                             numItems=len(self.hostDict))
            workerThread = WorkerThread(func = lambda : fabTest.copyTest(self.hostDict,
                                                                         self.project.workspace_dir + "/devel/lib/",
                                                                         self.project.deployment_path +"/"+ dep.properties['name'],
                                                                         copyProgressQ)
                                    )
            workerThread.start()
            dlg.ShowModal()
        else:
            dialogs.ErrorDialog(self,"Can't copy deployment files when system is running a deployment!")

    def OnDeploymentRun(self,e):
        newObj = drawable.Drawable_Object()
        newObj.properties = OrderedDict()
        newObj.properties['command'] = ""
        ed = dialogs.EditDialog(self,
                                editDict=newObj.properties,
                                editObj = newObj,
                                title="Input Command To Run",
                                references = [],
                                style=wx.RESIZE_BORDER)
        ed.ShowModal()
        inputs = ed.GetInput()
        if inputs != OrderedDict():
            for key,value in inputs.iteritems():
                newObj.properties[key] = value
            command = newObj.properties['command']
            selectedPage = self.DeploymentAspect.GetSelection()
            objName = self.DeploymentAspect.GetPageText(selectedPage)
            info = self.DeploymentAspectInfo.GetPageInfo(objName)
            dep = info.obj
            canvas = info.canvas
            env.use_ssh_config = False
            self.hostDict = {}
            env.hosts = []
            for host in dep.getChildrenByKind("Hardware_Instance"):
                nodeList = []
                self.hostDict[host.properties['name']] = fabTest.deployed_host(
                    userName = host.properties['username'],
                    ipAddress = host.properties['host_reference'].properties['ip_address'],
                    keyFile = host.properties['sshkey'],
                    nodes = nodeList,
                    envVars = copy.copy(host.properties['env_variables'])
                )
                env.hosts.append(host.properties['name'])
            copyProgressQ = multiprocessing.Queue()
            dlg = dialogs.RMLProgressDialog( title="Copy Progress",
                                             progress_q = copyProgressQ,
                                             numItems=len(self.hostDict))
            workerThread = WorkerThread(func = lambda : fabTest.runCommandTest(self.hostDict,
                                                                               command,
                                                                               copyProgressQ)
                                    )
            workerThread.start()
            dlg.ShowModal()

    def OnDeploymentDeploy(self,e):
        if self.deployed == False:
            selectedPage = self.DeploymentAspect.GetSelection()
            objName = self.DeploymentAspect.GetPageText(selectedPage)
            info = self.DeploymentAspectInfo.GetPageInfo(objName)
            dep = info.obj
            canvas = info.canvas
            env.use_ssh_config = False
            self.hostDict = {}
            env.hosts = []
            #env.warn_only = False
            numNodes = 0 # 1 # roscore
            rosCoreIP = ""
            testName = ""
            newObj = project.Drawable_Object()
            newObj.properties = OrderedDict()
            newObj.properties['name'] = testName
            newObj.properties['hardware_reference'] = None
            references = dep.properties['hardware_configuration_reference'].children
            ed = dialogs.EditDialog(self,
                                    editDict=newObj.properties,
                                    editObj = newObj,
                                    title="Deployment Options",
                                    references = references,
                                    style=wx.RESIZE_BORDER)
            ed.ShowModal()
            inputs = ed.GetInput()
            if inputs != OrderedDict():
                for key,value in inputs.iteritems():
                    newObj.properties[key] = value
                rosCoreIP = newObj.properties['host_reference'].properties['ip_address']
                testName = newObj.properties['name']
                for host in dep.getChildrenByKind("host_instance"):
                    nodeList = []
                    for node in host.children:
                        numNodes += 1
                        nodeList.append(fabTest.deployed_node(
                            name = node.properties['name'],
                            executable = '/home/' + host.properties['username'] + '/' + node.properties['node_reference'].parent.properties['name'] + '/' + node.properties['node_reference'].properties['name'],
                            cmdArgs = node.properties['cmdline_arguments']
                        ))
                        nodeList[-1].cmdArgs += " -nodename {}".format(node.properties['name'])
                        nodeList[-1].cmdArgs += " -hostname {}".format(host.properties['name'])
                    self.hostDict[host.properties['name']] = fabTest.deployed_host(
                        userName = host.properties['username'],
                        ipAddress = host.properties['host_reference'].properties['ip_address'],
                        keyFile = host.properties['sshkey'],
                        nodes = nodeList,
                        envVars = copy.copy(host.properties['env_variables'])
                    )
                    self.hostDict[host.properties['name']].envVars.append(
                        ['ROS_MASTER_URI','http://{}:11311/'.format(rosCoreIP)])
                    self.hostDict[host.properties['name']].envVars.append(
                        ['ROS_IP',host.properties['host_reference'].properties['ip_address']]
                    )
                    env.hosts.append(host.properties['name'])
                deploymentProgressQ = multiprocessing.Queue()
                dlg = dialogs.RMLProgressDialog( title="Deployment Progress",
                                                 progress_q = deploymentProgressQ,
                                                 numItems=numNodes)
                workerThread = WorkerThread(func = lambda : fabTest.deployTest(self.hostDict,
                                                                               self.hostDictTopic,
                                                                               deploymentProgressQ)
                                        )
                self.updatedHostDict = False
                workerThread.start()
                dlg.ShowModal()
                self.runningDeployment = dep
                self.runningDeploymentCanvas = canvas
                self.runningNodes = numNodes
                self.deployed = True
                while not self.updatedHostDict:
                    pass
                # START MONITORING INFRASTRUCTURE
                #env.warn_only = True
                monitorQ = multiprocessing.Queue()
                workerThread = WorkerThread(func = lambda : fabTest.monitorTest(self.hostDict,
                                                                                self.hostDictTopic,
                                                                                monitorQ)
                                        )
                monitorWorkItem = WorkItem(process = workerThread,
                                           queue = monitorQ,
                                           workFunc = self.MonitorWorkFunc)
                self.workQueue.append(monitorWorkItem)
                workerThread.start()
        else:
            dialogs.ErrorDialog(self,"System is already running a deployment!")

    def OnDeploymentStop(self,e):
        if self.deployed == True: 
            self.deployed = False
            self.deploying = False
            deploymentProgressQ = multiprocessing.Queue()
            dlg = dialogs.RMLProgressDialog(title="Stop Deployment Progress",
                                            progress_q = deploymentProgressQ,
                                            numItems=self.runningNodes)
            workerThread = WorkerThread(func = lambda : fabTest.stopTest(self.hostDict,
                                                                         self.hostDictTopic,
                                                                         deploymentProgressQ)
            )
            workerThread.start()
            dlg.ShowModal()
            self.runningNodes = 0
            drawable.Configure(self.runningDeployment,self.styleDict)
            self.DrawModel(self.runningDeployment,self.runningDeploymentCanvas)
        else:
            dialogs.ErrorDialog(self,"System is not running a deployment")

    def OnPackageCreate(self, e):
        newPkg = project.ROS_Package()
        newPkg.properties = {}
        newPkg.properties['name'] = "New Package"
        existingNames = [x.properties['name'] for x in self.project.workspace.children]
        ed = dialogs.EditDialog(self,
                                editObj=newPkg,
                                editDict=newPkg.properties,
                                title="Edit "+newPkg.kind,
                                references = [],
                                invalidNames = existingNames,
                                style=wx.RESIZE_BORDER)
        ed.ShowModal()
        inputs = ed.GetInput()
        if inputs != OrderedDict():
            self.UpdateUndo()
            for key,value in inputs.iteritems():
                newPkg.properties[key] = value
            self.project.workspace.add(newPkg)
            numPages = self.PackageAspect.GetPageCount()
            self.BuildModelPage(self.PackageAspect,newPkg,self.PackageAspectInfo,numPages-1)
            self.PackageAspect.SetSelection(numPages - 1)
    
    def OnPackageDelete(self, e):
        selectedPage = self.PackageAspect.GetSelection()
        numPages = self.PackageAspect.GetPageCount()
        pkgName = self.PackageAspect.GetPageText(selectedPage)
        info = self.PackageAspectInfo.GetPageInfo(pkgName)
        pkg = info.obj
        if pkg.kind != 'rml':
            if dialogs.ConfirmDialog(self,"Delete {}?".format(pkgName)):
                self.UpdateUndo()
                info.canvas.ClearAll()
                pkg.delete()
                self.PackageAspect.GetPage(selectedPage).DestroyChildren()
                self.PackageAspectInfo.DelPageInfo(pkg.properties['name'])
                self.PackageAspect.DeletePage(selectedPage)
        else:
            dialogs.ErrorDialog(self,"Cannot Delete Workspace!")

    def OnPackageGenerate(self,e):
        pass

    def BindCanvasMouseEvents(self,canvas):
        canvas.Bind(FloatCanvas.EVT_MOUSEWHEEL, self.OnMouseWheel)
        canvas.Bind(FloatCanvas.EVT_RIGHT_UP, self.OnRightUp) 

    def UnBindCanvasMouseEvents(self,canvas):
        canvas.Unbind(FloatCanvas.EVT_MOUSEWHEEL)
        canvas.Unbind(FloatCanvas.EVT_RIGHT_UP)

    def GetActivePanelInfo(self):
        selectedAspect = self.activeAspect
        selectedAspectInfo = self.activeAspectInfo
        selectedPage = selectedAspect.GetSelection()
        pageName = selectedAspect.GetPageText(selectedPage)
        pageInfo = selectedAspectInfo.GetPageInfo(pageName)
        return pageInfo
    def AspectLog(self, text, msgWindow):
        msgWindow.SetReadOnly(False)
        msgWindow.AppendText(text)
        if not text[-1] == "\n":
            msgWindow.AppendText("\n")
        msgWindow.SetReadOnly(True)
        msgWindow.ScrollToLine(msgWindow.GetLineCount())
    def OnMouseWheel(self,event):
        info = self.GetActivePanelInfo()
        canvas = info.canvas
        Rot = event.GetWheelRotation()
        Rot = Rot / abs(Rot) * 0.1
        if event.ControlDown(): # move left-right
            canvas.MoveImage( (Rot, 0), "Panel" )
        elif event.ShiftDown():
            canvasPos = canvas.ViewPortCenter#event.GetCoords()
            if Rot > 0:
                canvas.Zoom(1.1,canvasPos)
            else:
                canvas.Zoom(0.9,canvasPos)
        else: # move up-down
            canvas.MoveImage( (0, -Rot), "Panel" )
    def OnRightUp(self,event):
        info = self.GetActivePanelInfo()
        canvas = info.canvas
        self.activeObject = info.obj
        self.PopupMenu(ContextMenu(canvas,AspectContextMenu(self,self.activeObject)))
        
    '''
    View Menu Functions
    '''
    def UpdateMainWindow(self, e):
        self.viewSplitter.Show()
        self.viewSplitter.SplitHorizontally(self.activeAspect,self.output,self.viewSplitter.GetSashPosition())
        if self.shvw.IsChecked() and self.shop.IsChecked():
            pass
        elif self.shvw.IsChecked() and not self.shop.IsChecked():
            self.viewSplitter.Unsplit(self.output)
        elif not self.shvw.IsChecked() and self.shop.IsChecked():
            self.viewSplitter.Unsplit(self.activeAspect)
        else:
            self.viewSplitter.Hide()
        self.viewSplitter.UpdateSize()
    def OnSplitterDClick(self, e):
        self.shop.Check(False)
        self.UpdateMainWindow(e)
    def ToggleAspectView(self, e):
        self.UpdateMainWindow(e)    
    def ToggleOutputView(self, e):
        self.UpdateMainWindow(e)
    def ToggleStatusBar(self, e):
        self.GetStatusBar().Show(e.IsChecked())
    def ToggleToolBar(self, e):
        self.toolbar.Show(e.IsChecked())
        self.SendSizeEvent()

    '''
    Toolbar and File Menubar Menu Functions
    '''
    def OnPrint(self, e):
        imgName, imgPath = dialogs.RMLFileDialog(
            frame = self,
            fileTypes = "PNG Images (*.png)|*.png",
            path = self.project_path,
            prompt = "Save Aspect View As Image...",
            fd_flags = wx.FD_SAVE | wx.FD_OVERWRITE_PROMPT
        )
        if imgName != None and imgPath != None:
            info = self.GetActivePanelInfo()
            canvas = info.canvas
            model = info.obj
            msgWindow = info.msgWindow
            canvas.Scale = 1
            canvas.SetToNewScale(False)
            canvas._ResetBoundingBox()
            box = canvas.BoundingBox
            canvas.ViewPortCenter -= (canvas.PixelToWorld((0,0)) - numpy.array((box[0,0],box[1,1])))
            bmp = wx.EmptyBitmap(box.Width,box.Height)
            dc = wx.MemoryDC()
            dc.SelectObject(bmp)
            dc.Clear()
            canvas._DrawObjects(dc,canvas._DrawList,dc,canvas.BoundingBox)
            dc.SelectObject(wx.NullBitmap)
            if imgName[-4:] != ".png":
                imgName += ".png"
            bmp.SaveFile(imgPath+'/'+imgName,wx.BITMAP_TYPE_PNG)
            self.statusbar.SetStatusText('Saved {} to {}'.format(imgName,imgPath))

    def OnQuit(self, e):
        if dialogs.ConfirmDialog(self,"Really quit ROSMOD?"):
            self.workTimer.Stop()
            exit()

    def OnNew(self, e):
        project_path = dialogs.RMLDirectoryDialog(
            frame = self,
            prompt ="Choose New Project Location", 
            path = self.project_path,
        )
        if project_path != None:
            dlgObj = project.ROS_Project()
            dlgObj.properties = OrderedDict()
            dlgObj.properties['name'] = "New Project"
            ed = dialogs.EditDialog( self,
                                     editObj = dlgObj,
                                     editDict = dlgObj.properties,
                                     title = 'Choose Project Name',
                                     style = wx.RESIZE_BORDER)
            ed.ShowModal()
            inputs = ed.GetInput()
            if inputs != OrderedDict():
                self.filename = inputs['name']
                self.project_path = project_path
                self.project = project.ROS_Project()
                self.project.new(self.filename,self.project_path)
                self.project.workspace.properties['name'] = "Workspace"
                newHW = project.ROS_HW()
                newHW.properties['name'] = "Hardware"
                self.project.hardware_files.append(newHW)
                newDeployment = project.ROS_Deployment()
                newDeployment.properties['name'] = "Deployment"
                newDeployment.properties['hardware_configuration_reference'] = newHW
                self.project.deployments.append(newDeployment)
                BuildAspectPages(self)
                self.statusbar.SetStatusText('Created new project: {} in: {}'.format(self.filename,self.project_path))

    def OnOpen(self, e):
        filename, model_path = dialogs.RMLFileDialog(
            frame = self,
            fileTypes = self.fileTypes,
            path = self.project_path,
            prompt = "Choose a ROS Project",
            fd_flags = wx.FD_OPEN | wx.FD_FILE_MUST_EXIST
        )
        if filename != None and model_path != None:
            self.filename = filename
            self.project_path = model_path
            self.project.open(self.project_path)
            BuildAspectPages(self)
            self.statusbar.SetStatusText('Loaded project: {} from: {}'.format(self.filename,self.project_path))

    def OnSave(self, e):
        self.project.save()
        self.statusbar.SetStatusText('Saved project as: {} in: {}'.format(self.filename,self.project_path))

    def OnSaveAs(self, e):
        dlgObj = project.ROS_Project()
        dlgObj.properties = OrderedDict()
        dlgObj.properties['name'] = self.project.project_name
        ed = dialogs.EditDialog( self,
                                 editObj = dlgObj,
                                 editDict = dlgObj.properties,
                                 title = 'Choose New Project Name',
                                 style = wx.RESIZE_BORDER)
        ed.ShowModal()
        inputs = ed.GetInput()
        if inputs != OrderedDict():
            project_path = dialogs.RMLDirectoryDialog(
                frame = self,
                prompt ="Choose New Project Location", 
                path = self.project_path,
            )
            if project_path != None:
                self.filename = inputs['name']
                self.project_path = project_path
                self.project.save(self.filename,self.project_path)
                BuildAspectPages(self)
                self.statusbar.SetStatusText('Saved project as: {} in: {}'.format(self.filename,self.project_path))

    def UpdateUndo(self):
        self.undoList.append(copy.copy(self.project))
        self.toolbar.EnableTool(wx.ID_UNDO, True)

    def OnUndo(self, e):
        if len(self.undoList) > 0:
            self.project = self.undoList.pop()
            self.redoList.append(self.project)
            self.toolbar.EnableTool(wx.ID_REDO, True)
            if len(self.undoList) == 0:
                self.toolbar.EnableTool(wx.ID_UNDO, False)
    def OnRedo(self, e):
        if len(self.redoList) > 0:
            self.project = self.redoList.pop()
            self.undoList.append(self.project)
            self.toolbar.EnableTool(wx.ID_UNDO, True)
            if len(self.redoList) == 0:
                self.toolbar.EnableTool(wx.ID_REDO, False)

    def OnTerminal(self, e):
        self.shop.Check(True)
        self.UpdateMainWindow(e)
        self.output.AddPage(TermEmulatorDemo(self.output), "Terminal",select=True)

    '''
    Tools Menubar Menu Functions
    '''
    def GenerateCode(self, e):
        self.project.generate_workspace()
        dialogs.InfoDialog(self,"Generated ROS Workspace.")
        self.statusbar.SetStatusText('Generated ROS Workspace')
    def AnalyzeNetwork(self, e):
        pass

    def AnalyzeTiming(self, e):
        pass

    '''
    Build the menubar which allows for operations on
    * Files: New, Open, Save, Quit
    * Aspects: Packages, Hardware, Deployment
    * Views: Toolbar, Statusbar, Aspects, Output
    * Tools: Generator, Network QoS Analysis, Timing Analysis
    '''
    def BuildMenu(self):
        self.menubar = wx.MenuBar()

        # normal file operations menu
        self.fileMenu = wx.Menu()
        self.newMI = self.fileMenu.Append(wx.ID_NEW, '&New', 'New Project')
        self.openMI = self.fileMenu.Append(wx.ID_OPEN, '&Open', 'Open existing Project')
        self.saveMI = self.fileMenu.Append(wx.ID_SAVE, '&Save', 'Save current Project')
        self.saveAsMI = self.fileMenu.Append(wx.ID_SAVEAS, 'Save &As', 'Save current Project As...')
        self.fileMenu.AppendSeparator()
        self.quitMI = wx.MenuItem(self.fileMenu, wx.ID_EXIT, '&Quit\tCtrl+W', 'Quit ROSMOD')
        self.fileMenu.AppendItem(self.quitMI)

        # aspects (of the viewer) menu for ROSMOD: packages, hardware, deployment
        self.aspectsMenu = wx.Menu()
        self.packageAMI = self.aspectsMenu.Append(wx.ID_ANY,
                                                  "Packages",
                                                  "View/Edit the packages in the model.",
                                                  kind=wx.ITEM_RADIO
                                              )
        self.hardwareAMI = self.aspectsMenu.Append(wx.ID_ANY,
                                                   "Hardware",
                                                   "View/Edit the hardware in the model.",
                                                   kind=wx.ITEM_RADIO
                                               )
        self.deploymentAMI = self.aspectsMenu.Append(wx.ID_ANY,
                                                     "Deployment",
                                                     "View/Manage the deployment of the model.",
                                                     kind=wx.ITEM_RADIO
                                                 )

        # tools menu for ROSMOD: generate code, analyze network and timing
        self.toolMenu = wx.Menu()
        self.generateMI = self.toolMenu.Append(wx.ID_ANY, 
                                               "Generate ROS Code\tCtrl+G", 
                                               "Generate ROS application code and workspace.")
        self.networkQoSMI = self.toolMenu.Append(wx.ID_ANY, 
                                                 "Analyze Network", 
                                                 "Analyze application and system network resource utilization.",)
        self.blTimingMI = self.toolMenu.Append(wx.ID_ANY, 
                                               "Analyze Timing", 
                                               "Generate CPN Tokens and Analyze Business Logic Model.")

        # view menu: show/hide statusbar/toolbar/viewer/output
        self.viewMenu = wx.Menu()
        self.shst = self.viewMenu.Append(wx.ID_ANY, 'Show Statusbar', 'Show Statusbar', kind=wx.ITEM_CHECK)
        self.shtl = self.viewMenu.Append(wx.ID_ANY, 'Show Toolbar', 'Show Toolbar', kind=wx.ITEM_CHECK)
        self.shvw = self.viewMenu.Append(wx.ID_ANY, 'Show Viewer', 'Show Viewer', kind=wx.ITEM_CHECK)
        self.shop = self.viewMenu.Append(wx.ID_ANY, 'Show Output', 'Show Output', kind=wx.ITEM_CHECK)
        self.viewMenu.Check(self.shst.GetId(), True)
        self.viewMenu.Check(self.shtl.GetId(), True)
        self.viewMenu.Check(self.shvw.GetId(), True)
        self.viewMenu.Check(self.shop.GetId(), True)

        # add the menus to the menubar
        self.menubar.Append(self.fileMenu, '&File')
        self.menubar.Append(self.viewMenu, '&View')
        self.menubar.Append(self.aspectsMenu, '&Aspects')
        self.menubar.Append(self.toolMenu, '&Tools')
        self.SetMenuBar(self.menubar)
        
        # set up the events for the items in the menubar
        self.RegisterMenuEvents()

    '''
    Bind menu items to functions for event processing
    '''
    def RegisterMenuEvents(self):
        # file menu
        self.Bind(wx.EVT_MENU, self.OnNew, self.newMI)
        self.Bind(wx.EVT_MENU, self.OnOpen, self.openMI)
        self.Bind(wx.EVT_MENU, self.OnSave, self.saveMI)
        self.Bind(wx.EVT_MENU, self.OnSaveAs, self.saveAsMI)
        self.Bind(wx.EVT_MENU, self.OnQuit, self.quitMI)
        # aspect menu
        self.Bind(wx.EVT_MENU, lambda e : OnPackageAspect(self,e), self.packageAMI)
        self.Bind(wx.EVT_MENU, lambda e : OnHardwareAspect(self,e), self.hardwareAMI)
        self.Bind(wx.EVT_MENU, lambda e : OnDeploymentAspect(self,e), self.deploymentAMI)
        # tools menu
        self.Bind(wx.EVT_MENU, self.GenerateCode, self.generateMI)
        self.Bind(wx.EVT_MENU, self.AnalyzeNetwork, self.networkQoSMI)
        self.Bind(wx.EVT_MENU, self.AnalyzeTiming, self.blTimingMI)
        # view menu
        self.Bind(wx.EVT_MENU, self.ToggleStatusBar, self.shst)
        self.Bind(wx.EVT_MENU, self.ToggleToolBar, self.shtl)
        self.Bind(wx.EVT_MENU, self.ToggleAspectView, self.shvw)
        self.Bind(wx.EVT_MENU, self.ToggleOutputView, self.shop)

    '''
    Build the Statusbar which provides extra information about
    all the objects and menus in ROSMOD.  It also displays short
    info from the output about results of operations.
    '''
    def BuildStatusbar(self):
        self.statusbar = self.CreateStatusBar()
        self.statusbar.SetStatusText('Ready')

def main():
    ex = wx.App()
    Example(None)
    ex.MainLoop()    


if __name__ == '__main__':
    main()
