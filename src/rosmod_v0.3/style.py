import wx
import os,sys
from collections import OrderedDict
import copy
import drawable

import dialogs

from metaModel import model_dict
from metaclass import *

exeName = sys.argv[0]
dirName = os.path.abspath(exeName)
head,tail = os.path.split(dirName)
editorPath=head
rootIconPath= editorPath + '/icons'
modelIconPath= rootIconPath + '/model'

def BuildStyleObjects(self):
    for key in model_dict.keys():
        meta_class_dict[key+'Icon'] = Grammar_Field(
            kind = "file",
            display_name = key + ' Icon')
    BuildIconList(self)
    BuildIcons(self)
    BuildOverlay(self)
    BuildStyle(self)

def BuildIconList(self):
    self.iconList = OrderedDict()
    for key in model_dict.keys():
        self.iconList[key+'Icon'] = modelIconPath + '/' + key + '.png'

def BuildIcons(self):
    self.iconDict = OrderedDict()
    for key in model_dict.keys():
        if os.path.isfile(self.iconList[key+'Icon']):
            self.iconDict[key] = wx.Bitmap(self.iconList[key+'Icon'])
        else:
            self.iconDict[key] = None

def EditIcons(self):
    inputs = dialogs.EditorWindow( parent = self,
                                   editObj = None,
                                   editDict = self.iconList,
                                   title = 'Select Object Icons')
    if inputs != OrderedDict():
        for key,value in inputs.iteritems():
            self.iconList[key] = value
        BuildIcons(self)
        info = self.GetActivePanelInfo()
        if info != None:
            model = info.obj
            canvas = info.canvas
            self.DrawModel(model,canvas)

def BuildOverlay(self):
    self.overlayDict = OrderedDict()
    self.overlayDict['Active Overlay'] = "BLACK"
    self.overlayDict['Node Down Overlay'] = "RED"
    self.overlayDict['Node Up Overlay'] = "GREEN"
    self.overlayDict['Reference Overlay'] = "WHITE"
    self.overlayDict['Similar Overlay'] = "BLACK"
    for key in self.overlayDict.keys():
        meta_class_dict[key] = Grammar_Field(
            kind = "string",
            display_name = key)

def EditOverlay(self):
    inputs = dialogs.EditorWindow( parent = self,
                                   editObj = None,
                                   editDict = self.overlayDict,
                                   title = 'Select Overlays')
    if inputs != OrderedDict():
        for key,value in inputs.iteritems():
            self.overlayDict[key] = value
        info = self.GetActivePanelInfo()
        if info != None:
            model = info.obj
            canvas = info.canvas
            self.DrawModel(model,canvas)

def EditStyle(self,kind):
    def Generic(e):
        inputs = dialogs.EditorWindow( parent = self,
                                       editObj = None,
                                       editDict = self.styleDict[kind],
                                       title = 'Edit {} Style'.format(kind))
        if inputs != OrderedDict():
            for key,value in inputs.iteritems():
                self.styleDict[kind][key] = value
            info = self.GetActivePanelInfo()
            if info != None:
                model = info.obj
                canvas = info.canvas
                drawable.Configure(model,self.styleDict)
                self.DrawModel(model,canvas)
    return Generic

def BuildStyle(self):
    # style related options which can be edited
    valid_textPlacement = ["NONE","TOP","BOTTOM","CENTER","LEFT","RIGHT"]
    valid_drawMethod = ["ICON","RECT","ROUND_RECT","HIDDEN"]
    valid_childLayout = ["STACK","COLUMNS","LINE","ROWS","SQUARE","HIDDEN"]
    meta_class_dict['textPlacement'] = Grammar_Field(
        kind="list",
        display_name="Text Placement",
        input_validator=lambda p,o,s,k : list_validator(valid_textPlacement,p,o,s,k))
    meta_class_dict['method'] = Grammar_Field(
        kind="list",
        display_name="Draw Method",
        input_validator=lambda p,o,s,k : list_validator(valid_drawMethod,p,o,s,k))
    meta_class_dict['childLayout'] = Grammar_Field(
        kind="list",
        display_name="Child Layout",
        input_validator=lambda p,o,s,k : list_validator(valid_childLayout,p,o,s,k))
    meta_class_dict['padding'] = Grammar_Field(
        kind="tuple",
        display_name="Padding (x,y)")
    meta_class_dict['minSize'] = Grammar_Field(
        kind="tuple",
        display_name="Minimum Size (x,y)")
    meta_class_dict['offset'] = Grammar_Field(
        kind="tuple",
        display_name="Offset (x,y)")
    meta_class_dict['overlay'] = Grammar_Field(
        kind="dictionary",
        display_name="Overlay")
    meta_class_dict['fillColor'] = Grammar_Field(
        kind="string",
        display_name="Fill Color")
    meta_class_dict['outlineColor'] = Grammar_Field(
        kind="string",
        display_name="Outline Color")
    meta_class_dict['font'] = Grammar_Field(
        kind="dictionary",
        display_name="Font")
    meta_class_dict['pointSize'] = Grammar_Field(
        kind="number",
        display_name="Point Size")
    meta_class_dict['color'] = Grammar_Field(
        kind="string",
        display_name="Color")
    meta_class_dict['prefix'] = Grammar_Field(
        kind="string",
        display_name="Prefix")
    meta_class_dict['weight'] = Grammar_Field(
        kind="hidden",
        display_name="Weight")

    meta_class_dict['Index 0'] = Grammar_Field(
        kind="number",
        display_name="Index 0")
    meta_class_dict['Index 1'] = Grammar_Field(
        kind="number",
        display_name="Index 1")
    meta_class_dict['Index 2'] = Grammar_Field(
        kind="number",
        display_name="Index 2")
    meta_class_dict['Index 3'] = Grammar_Field(
        kind="number",
        display_name="Index 3")

    minSize = [30, 30]
    offset = [10, 50]
    padding = [30, 30]

    self.styleDict = OrderedDict()
    defaultStyle = OrderedDict()
    defaultStyle['minSize'] = minSize
    defaultStyle['padding'] = [10,10]
    defaultStyle['offset'] = [10,10]
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
    HardwareStyle['minSize'] = [200, 200]
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
    NodeStyle['minSize'] = [30, 30]

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
    PkgStyle['padding'] = [50,25]

    MsgStyle = copy.deepcopy(defaultStyle)
    MsgStyle['font'] = {'pointSize' : 20,
                        'color' : "black",
                        'prefix' : "",
                        'weight' : wx.BOLD}
    MsgStyle['method'] = 'ICON'
    MsgStyle['childLayout'] = 'HIDDEN'
    MsgStyle['textPlacement'] = 'TOP'
    MsgStyle['overlay'] = OrderedDict()
    MsgStyle['minSize'] = [108, 79]

    SrvStyle = copy.deepcopy(defaultStyle)
    SrvStyle['font'] = {'pointSize' : 20,
                        'color' : "black",
                        'prefix' : "",
                        'weight' : wx.BOLD}
    SrvStyle['method'] = 'ICON'
    SrvStyle['childLayout'] = 'HIDDEN'
    SrvStyle['textPlacement'] = 'TOP'
    SrvStyle['overlay'] = OrderedDict()
    SrvStyle['minSize'] = [108, 79]

    CompStyle = copy.deepcopy(defaultStyle)
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
    TmrStyle['minSize'] = [108, 79]

    PubStyle = copy.deepcopy(defaultStyle)
    PubStyle['font'] = {'pointSize' : 20,
                        'color' : "white",
                        'prefix' : " ",
                        'weight' : wx.NORMAL}
    PubStyle['method'] = 'ICON'
    PubStyle['childLayout'] = 'HIDDEN'
    PubStyle['textPlacement'] = 'RIGHT'
    PubStyle['overlay'] = OrderedDict()
    PubStyle['minSize'] = [108, 79]

    SubStyle = copy.deepcopy(defaultStyle)
    SubStyle['font'] = {'pointSize' : 20,
                        'color' : "white",
                        'prefix' : " ",
                        'weight' : wx.NORMAL}
    SubStyle['method'] = 'ICON'
    SubStyle['childLayout'] = 'HIDDEN'
    SubStyle['textPlacement'] = 'RIGHT'
    SubStyle['overlay'] = OrderedDict()
    SubStyle['minSize'] = [108, 79]

    CliStyle = copy.deepcopy(defaultStyle)
    CliStyle['font'] = {'pointSize' : 20,
                        'color' : "white",
                        'prefix' : " ",
                        'weight' : wx.NORMAL}
    CliStyle['method'] = 'ICON'
    CliStyle['childLayout'] = 'HIDDEN'
    CliStyle['textPlacement'] = 'RIGHT'
    CliStyle['overlay'] = OrderedDict()
    CliStyle['minSize'] = [108, 79]
    
    SerStyle = copy.deepcopy(defaultStyle)
    SerStyle['font'] = {'pointSize' : 20,
                        'color' : "white",
                        'prefix' : " ",
                        'weight' : wx.NORMAL}
    SerStyle['method'] = 'ICON'
    SerStyle['childLayout'] = 'HIDDEN'
    SerStyle['textPlacement'] = 'RIGHT'
    SerStyle['overlay'] = OrderedDict()
    SerStyle['minSize'] = [108, 79]

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

