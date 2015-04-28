#!/usr/bin/python

# this code: http://zetcode.com/wxpython/menustoolbars/

import wx

from collections import OrderedDict


class ContextMenu(wx.Menu):
    
    def __init__(self, parent, functionsDict):
        super(ContextMenu, self).__init__()
        
        self.parent = parent

        for name,func in functionsDict.iteritems():
            mi = wx.MenuItem(self, wx.NewId(), name)
            self.AppendItem(mi)
            self.Bind(wx.EVT_MENU, func, mi)
        

def AspectContextMenu(self, obj):
    cm = OrderedDict()
    cm['Edit'] = self.AspectEdit        # edits the object's properties (name, fields, etc.)
    cm['Delete'] = self.AspectDelete    # deletes the object and all references from the model
    if obj.kind == 'component':
        cm = BuildCompContextMenu(self,cm)
    elif obj.kind == 'node':
        cm = BuildNodeContextMenu(self,cm)
    elif obj.kind == 'package':
        cm = BuildPackageContextMenu(self,cm)
    elif obj.kind == 'workspace':
        cm = BuildWorkspaceContextMenu(self,cm)
    elif obj.kind == 'hardware_configuration':
        cm = BuildHardwareContextMenu(self,cm)
    elif obj.kind == 'host':
        cm = BuildHostContextMenu(self,cm)
    elif obj.kind == 'deployment':
        cm = BuildDeploymentContextMenu(self,cm)
    elif obj.kind == 'host_instance':
        cm = BuildHostInstanceContextMenu(self,cm)
    elif obj.kind == 'node_instance':
        cm = BuildNodeInstanceContextMenu(self,cm)
    elif obj.kind == 'group':
        cm = BuildGroupContextMenu(self,cm)
    elif obj.kind == 'port_instance':
        cm = BuildPortInstanceContextMenu(self,cm)
    return cm

def BuildCompContextMenu(self,cm):
    cm['Add Timer'] = lambda evt : CompAdd(self,evt,'timer')
    cm['Add Subscriber'] = lambda evt :CompAdd(self,evt,'subscriber')
    cm['Add Publisher'] = lambda evt : CompAdd(self,evt,'publisher')
    cm['Add Client'] = lambda evt : CompAdd(self,evt,'client')
    cm['Add Server'] = lambda evt : CompAdd(self,evt,'server')
    return cm
def BuildNodeContextMenu(self,cm):
    cm['Add Component Instance'] = lambda evt : NodeAdd(self,evt,'component_instance')
    return cm
def BuildPackageContextMenu(self, cm):
    cm['Add Message'] = lambda evt : PackageAdd(self,evt,'message')
    cm['Add Service'] = lambda evt : PackageAdd(self,evt,'service')
    cm['Add Component Definition'] = lambda evt : PackageAdd(self,evt,'component')
    cm['Add Node'] = lambda evt : PackageAdd(self,evt,'node')
    return cm
def BuildWorkspaceContextMenu(self,cm):
    return cm
def BuildHardwareContextMenu(self,cm):
    cm['Add Host'] = lambda evt : HardwareAdd(self,evt,'host')
    return cm
def BuildHostContextMenu(self,cm):
    return cm
def BuildDeploymentContextMenu(self,cm):
    cm['Add Host Instance'] = lambda evt : DeploymentAdd(self,evt,'host_instance')
    cm['Add Group'] = lambda evt : DeploymentAdd(self,evt,'group')
    return cm
def BuildHostInstanceContextMenu(self,cm):
    cm['Add Node Instance'] = lambda evt : HostInstAdd(self,evt,'node_instance')
    cm['Open SSH Terminal'] = lambda _: SSHToHostInst(self,self.activeObject)
    return cm
def BuildNodeInstanceContextMenu(self,cm):
    if self.deployed == True and self.activeObject.parent.parent == self.runningDeployment:
        cm['Monitor Log'] = lambda _: MonitorNodeInstLog(self,self.activeObject)
    return cm
def BuildGroupContextMenu(self,cm):
    cm['Add Port Instance'] = lambda evt : GroupAdd(self,evt,'port_instance')
    return cm
def BuildPortInstanceContextMenu(self,cm):
    return cm


class ContextMenuExample(wx.Frame):
    
    def __init__(self, *args, **kwargs):
        super(ContextMenuExample, self).__init__(*args, **kwargs) 
            
        self.InitUI()
        
    def InitUI(self):

        self.Bind(wx.EVT_RIGHT_DOWN, self.OnRightDown)

        self.SetSize((250, 200))
        self.SetTitle('Context menu')
        self.Centre()
        self.Show(True)
        
    def OnRightDown(self, e):
        cm = OrderedDict()
        cm['Edit'] = self.OnEdit
        cm['Delete'] = self.OnDelete
        self.PopupMenu(ContextMenu(self,cm), e.GetPosition())

    def OnEdit(self, e):
        print "In OnEdit()!"

    def OnDelete(self, e):
        print "In OnDelete()!"
        
def context_menu_main():
    
    ex = wx.App()
    ContextMenuExample(None)
    ex.MainLoop()    


if __name__ == '__main__':
    context_menu_main()
