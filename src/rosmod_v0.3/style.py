import wx
import os,sys
from collections import OrderedDict
import copy
import drawable

exeName = sys.argv[0]
dirName = os.path.abspath(exeName)
head,tail = os.path.split(dirName)
editorPath=head
rootIconPath= editorPath + '/icons'
modelIconPath= rootIconPath + '/model'

def EditIcons(self):
    print "EDITING ICONS"

def EditOverlay(self):
    print "EDITING OVERLAY"

def EditStyle(self,kind):
    def Generic(e):
        print "EDITING ",kind
    return Generic

def BuildStyleDict(self):
    self.overlayDict = OrderedDict()
    self.overlayDict['active'] = "BLACK"
    self.overlayDict['nodeDown'] = "RED"
    self.overlayDict['nodeUp'] = "GREEN"
    self.overlayDict['reference'] = "WHITE"
    self.overlayDict['similar'] = "BLACK"

    self.iconDict = OrderedDict()
    self.iconDict['Message'] = wx.Bitmap(modelIconPath + '/msg.png')
    self.iconDict['Service'] = wx.Bitmap(modelIconPath + '/srv.png')
    self.iconDict['Timer'] = wx.Bitmap(modelIconPath + '/timer.png')
    self.iconDict['Publisher'] = wx.Bitmap(modelIconPath + '/publisher.png')
    self.iconDict['Subscriber'] = wx.Bitmap(modelIconPath + '/subscriber.png')
    self.iconDict['Client'] = wx.Bitmap(modelIconPath + '/client.png')
    self.iconDict['Server'] = wx.Bitmap(modelIconPath + '/server.png')
    self.iconDict['Hardware'] = wx.Bitmap(modelIconPath + '/computer.png')

    minSize = (30, 30)
    offset = (10, 50)
    padding = (30, 30)

    self.styleDict = OrderedDict()
    defaultStyle = OrderedDict()
    defaultStyle['minSize'] = minSize
    defaultStyle['padding'] = (10,10)
    defaultStyle['offset'] = (10,10)
    defaultStyle['textPlacement'] = 'TOP'
    '''
    STYLES USED FOR HARDWARE CONFIGURATION OBJECTS
    '''
    rhwStyle = copy.deepcopy(defaultStyle)
    rhwStyle['font'] = {'pointSize' : 25,
                        'color' : "#9A3334",
                        'prefix' : "Hardware Model: ",
                        'weight' : wx.BOLD}
    rhwStyle['method'] = 'ICON'
    rhwStyle['childLayout'] = 'SQUARE'
    rhwStyle['offset'] = offset
    rhwStyle['placement'] = 'TOP'
    rhwStyle['overlay'] = OrderedDict()
    
    HardwareStyle = copy.deepcopy(defaultStyle)
    HardwareStyle['font'] = {'pointSize' : 20,
                             'color' : "#217C7E",
                             'prefix' : "Device: ",
                             'weight' : wx.BOLD}
    HardwareStyle['method'] = 'ICON'
    HardwareStyle['childLayout'] = 'HIDDEN'
    HardwareStyle['minSize'] = (200, 200)
    HardwareStyle['textPlacement'] = 'TOP'
    HardwareStyle['overlay'] = OrderedDict()
    '''
    STYLES USED FOR DEPLOYMENT CONFIGURATION OBJECTS
    '''
    DeploymentStyle = copy.deepcopy(defaultStyle)
    DeploymentStyle['font'] = {'pointSize' : 25,
                               'color' : "#9A3334",
                               'prefix' : "Deployment Model: ",
                               'weight' : wx.BOLD}
    DeploymentStyle['method'] = 'ICON'
    DeploymentStyle['childLayout'] = 'SQUARE'
    DeploymentStyle['offset'] = offset
    DeploymentStyle['textPlacement'] = 'TOP'
    DeploymentStyle['overlay'] = OrderedDict()

    NodeStyle = copy.deepcopy(defaultStyle)
    NodeStyle['font'] = {'pointSize' : 20,
                         'color' : "#217C7E",
                         'prefix' : "Node: ",
                         'weight' : wx.BOLD}
    NodeStyle['method'] = 'ROUND_RECT'
    NodeStyle['childLayout'] = 'STACK'   
    NodeStyle['textPlacement'] = 'TOP'
    NodeStyle['overlay'] = OrderedDict([('fillColor','#217C7E')])
    NodeStyle['minSize'] = (30, 30)

    CompInstStyle = copy.deepcopy(defaultStyle)
    CompInstStyle['font'] = {'pointSize' : 20,
                             'color' : "white",
                             'prefix' : " ",
                             'weight' : wx.NORMAL}
    CompInstStyle['method'] = 'ROUND_RECT'
    CompInstStyle['childLayout'] = 'STACK'
    CompInstStyle['minSize'] = minSize
    CompInstStyle['textPlacement'] = 'RIGHT'
    CompInstStyle['overlay'] = OrderedDict([('fillColor','#F3EFE0')])
    
    PortInstStyle = copy.deepcopy(defaultStyle)
    PortInstStyle['font'] = {'pointSize' : 20,
                             'color' : "black",
                             'prefix' : " ",
                             'weight' : wx.NORMAL}
    PortInstStyle['method'] = 'ROUND_RECT'
    PortInstStyle['childLayout'] = 'HIDDEN'
    PortInstStyle['minSize'] = minSize
    PortInstStyle['textPlacement'] = 'RIGHT'
    PortInstStyle['overlay'] = OrderedDict([('fillColor','#9A3334')])
    '''
    STYLES USED FOR SOFTWARE CONFIGURATION OBJECTS
    '''
    WrkStyle = copy.deepcopy(defaultStyle)
    WrkStyle['font'] = {'pointSize' : 25,
                        'color' : "black",
                        'prefix' : "Hardware Model: ",
                        'weight' : wx.BOLD}
    WrkStyle['method'] = 'ICON'
    WrkStyle['childLayout'] = 'STACK'
    WrkStyle['offset'] = offset
    WrkStyle['textPlacement'] = 'TOP'
    WrkStyle['overlay'] = OrderedDict()

    PkgStyle = copy.deepcopy(defaultStyle)
    PkgStyle['font'] = {'pointSize' : 25,
                        'color' : "#9A3334",
                        'prefix' : "Package: ",
                        'weight' : wx.BOLD}
    PkgStyle['method'] = 'ICON'
    PkgStyle['childLayout'] = 'COLUMNS'
    PkgStyle['offset'] = offset
    PkgStyle['textPlacement'] = 'TOP'
    PkgStyle['overlay'] = OrderedDict()
    PkgStyle['minSize'] = minSize
    PkgStyle['padding'] = (50,25)

    MsgStyle = copy.deepcopy(defaultStyle)
    MsgStyle['font'] = {'pointSize' : 20,
                        'color' : "black",
                        'prefix' : "",
                        'weight' : wx.BOLD}
    MsgStyle['method'] = 'ICON'
    MsgStyle['childLayout'] = 'HIDDEN'
    MsgStyle['textPlacement'] = 'TOP'
    MsgStyle['overlay'] = OrderedDict()
    MsgStyle['minSize'] = (108, 79)

    SrvStyle = copy.deepcopy(defaultStyle)
    SrvStyle['font'] = {'pointSize' : 20,
                        'color' : "black",
                        'prefix' : "",
                        'weight' : wx.BOLD}
    SrvStyle['method'] = 'ICON'
    SrvStyle['childLayout'] = 'HIDDEN'
    SrvStyle['textPlacement'] = 'TOP'
    SrvStyle['overlay'] = OrderedDict()
    SrvStyle['minSize'] = (108, 79)

    CompStyle = copy.deepcopy(defaultStyle)
    CompStyle['icon'] = None
    CompStyle['font'] = {'pointSize' : 20,
                         'color' : "#217C7E",
                         'prefix' : "Component: ",
                         'weight' : wx.BOLD}
    CompStyle['method'] = 'ROUND_RECT'
    CompStyle['childLayout'] = 'STACK'   
    CompStyle['textPlacement'] = 'TOP'
    CompStyle['minSize'] = minSize
    CompStyle['overlay'] = OrderedDict([('fillColor','#217C7E')])

    TmrStyle = copy.deepcopy(defaultStyle)
    TmrStyle['font'] = {'pointSize' : 20,
                        'color' : "white",
                        'prefix' : " ",
                        'weight' : wx.NORMAL}
    TmrStyle['method'] = 'ICON'
    TmrStyle['childLayout'] = 'HIDDEN'
    TmrStyle['textPlacement'] = 'RIGHT'
    TmrStyle['overlay'] = OrderedDict()
    TmrStyle['minSize'] = (108, 79)

    PubStyle = copy.deepcopy(defaultStyle)
    PubStyle['font'] = {'pointSize' : 20,
                        'color' : "white",
                        'prefix' : " ",
                        'weight' : wx.NORMAL}
    PubStyle['method'] = 'ICON'
    PubStyle['childLayout'] = 'HIDDEN'
    PubStyle['textPlacement'] = 'RIGHT'
    PubStyle['overlay'] = OrderedDict()
    PubStyle['minSize'] = (108, 79)

    SubStyle = copy.deepcopy(defaultStyle)
    SubStyle['font'] = {'pointSize' : 20,
                        'color' : "white",
                        'prefix' : " ",
                        'weight' : wx.NORMAL}
    SubStyle['method'] = 'ICON'
    SubStyle['childLayout'] = 'HIDDEN'
    SubStyle['textPlacement'] = 'RIGHT'
    SubStyle['overlay'] = OrderedDict()
    SubStyle['minSize'] = (108, 79)

    CliStyle = copy.deepcopy(defaultStyle)
    CliStyle['font'] = {'pointSize' : 20,
                        'color' : "white",
                        'prefix' : " ",
                        'weight' : wx.NORMAL}
    CliStyle['method'] = 'ICON'
    CliStyle['childLayout'] = 'HIDDEN'
    CliStyle['textPlacement'] = 'RIGHT'
    CliStyle['overlay'] = OrderedDict()
    CliStyle['minSize'] = (108, 79)
    
    SerStyle = copy.deepcopy(defaultStyle)
    SerStyle['font'] = {'pointSize' : 20,
                        'color' : "white",
                        'prefix' : " ",
                        'weight' : wx.NORMAL}
    SerStyle['method'] = 'ICON'
    SerStyle['childLayout'] = 'HIDDEN'
    SerStyle['textPlacement'] = 'RIGHT'
    SerStyle['overlay'] = OrderedDict()
    SerStyle['minSize'] = (108, 79)

    self.styleDict["rml"] = WrkStyle

    self.styleDict["Package"] = PkgStyle
    self.styleDict["Message"] = MsgStyle
    self.styleDict["Service"] = SrvStyle
    self.styleDict["Component"] = CompStyle
    self.styleDict["Timer"] = TmrStyle
    self.styleDict["Publisher"] = PubStyle
    self.styleDict["Subscriber"] = SubStyle
    self.styleDict["Client"] = CliStyle
    self.styleDict["Server"] = SerStyle

    self.styleDict["rhw"] = rhwStyle
    self.styleDict["Hardware"] = HardwareStyle

    self.styleDict["rdp"] = DeploymentStyle
    self.styleDict["Node"] = NodeStyle
    self.styleDict["Component_Instance"] = CompInstStyle
    self.styleDict["Port_Instance"] = PortInstStyle

