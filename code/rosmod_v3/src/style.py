


def BuildStyleDict(self):
    self.styleDict = OrderedDict()
    font = OrderedDict()
    font['pointSize'] = 20
    minSize = (50,50)
    padding = (10,10)
    pkgOffset = (50,50)
    msgIcon = wx.Bitmap(modelIconPath + '/msgIcon.png')
    srvIcon = wx.Bitmap(modelIconPath + '/srvIcon.png')
    tmrIcon = wx.Bitmap(modelIconPath + '/tmrIcon.png')
    pubIcon = wx.Bitmap(modelIconPath + '/pubIcon.png')
    subIcon = wx.Bitmap(modelIconPath + '/subIcon.png')
    clientIcon = wx.Bitmap(modelIconPath + '/clientIcon.png')
    serverIcon = wx.Bitmap(modelIconPath + '/serverIcon.png')
    compInstIcon = wx.Bitmap(modelIconPath + '/compInstIcon.png')
    hostIcon = wx.Bitmap(modelIconPath + '/bbb.png')
    hostInstIcon = wx.Bitmap(modelIconPath + '/bbb.png')
    '''
    STYLES USED FOR HARDWARE CONFIGURATION OBJECTS
    '''
    HardwareStyle = drawable.Draw_Style(icon=None, 
                                        font=font, 
                                        method=drawable.Draw_Method.ICON, 
                                        offset = pkgOffset,
                                        placement=drawable.Text_Placement.TOP,
                                        overlay = OrderedDict() )
    HostStyle = drawable.Draw_Style(icon=hostIcon,
                                    font=font, 
                                    method=drawable.Draw_Method.ICON, 
                                    minSize = minSize,
                                    placement=drawable.Text_Placement.TOP,
                                    overlay = OrderedDict() )
    '''
    STYLES USED FOR DEPLOYMENT CONFIGURATION OBJECTS
    '''
    DeploymentStyle = drawable.Draw_Style(icon=None, 
                                          font=font, 
                                          method=drawable.Draw_Method.ICON, 
                                          offset = pkgOffset,
                                          placement=drawable.Text_Placement.TOP,
                                          overlay = OrderedDict() )
    PortInstStyle = drawable.Draw_Style(icon=None,
                                        font=font, 
                                        method=drawable.Draw_Method.ROUND_RECT, 
                                        minSize = minSize,
                                        placement=drawable.Text_Placement.RIGHT,
                                        overlay = OrderedDict([('fillColor','BLUE')]) )
    GroupStyle = drawable.Draw_Style(icon=None,
                                     font=font, 
                                     method=drawable.Draw_Method.ROUND_RECT, 
                                     minSize = minSize,
                                     placement=drawable.Text_Placement.TOP,
                                     overlay = OrderedDict([('fillColor','GREEN')]) )
    NodeInstStyle = drawable.Draw_Style(icon=None,
                                        font=font, 
                                        method=drawable.Draw_Method.ROUND_RECT, 
                                        minSize = minSize,
                                        placement=drawable.Text_Placement.RIGHT,
                                        overlay = OrderedDict([('fillColor','TURQUOISE')]) )
    HostInstStyle = drawable.Draw_Style(icon=None,
                                        font=font, 
                                        method=drawable.Draw_Method.ROUND_RECT, 
                                        minSize = minSize,
                                        placement=drawable.Text_Placement.TOP,
                                        overlay = OrderedDict([('fillColor','WHITE')]) )
    '''
    STYLES USED FOR SOFTWARE CONFIGURATION OBJECTS
    '''
    WrkStyle = drawable.Draw_Style(icon=None, 
                                   font=font, 
                                   method=drawable.Draw_Method.ICON, 
                                   offset = pkgOffset,
                                   placement=drawable.Text_Placement.TOP,
                                   overlay = OrderedDict() )
    PkgStyle = drawable.Draw_Style(icon=None, 
                                   font=font, 
                                   method=drawable.Draw_Method.ICON, 
                                   placement=drawable.Text_Placement.TOP,
                                   overlay = OrderedDict(),
                                   minSize = minSize,
                                   padding = (50,25),
                                   offset = pkgOffset )
    MsgStyle = drawable.Draw_Style(icon=msgIcon, 
                                   font=font, 
                                   method=drawable.Draw_Method.ICON, 
                                   placement=drawable.Text_Placement.TOP,
                                   minSize = minSize,
                                   overlay = OrderedDict() )
    SrvStyle = drawable.Draw_Style(icon=srvIcon,
                                   font=font, 
                                   method=drawable.Draw_Method.ICON, 
                                   placement=drawable.Text_Placement.TOP,
                                   minSize = minSize,
                                   overlay = OrderedDict() )
    CompStyle = drawable.Draw_Style(icon=None,
                                    font=font, 
                                    method=drawable.Draw_Method.ROUND_RECT, 
                                    placement=drawable.Text_Placement.TOP,
                                    minSize = minSize,
                                    overlay = OrderedDict([('fillColor','STEEL BLUE')]) )
    TmrStyle = drawable.Draw_Style(icon=tmrIcon,
                                   font=font, 
                                   method=drawable.Draw_Method.ICON, 
                                   minSize = minSize,
                                   placement=drawable.Text_Placement.RIGHT,
                                   overlay = OrderedDict() )
    PubStyle = drawable.Draw_Style(icon=pubIcon,
                                   font=font, 
                                   method=drawable.Draw_Method.ICON, 
                                   minSize = minSize,
                                   placement=drawable.Text_Placement.RIGHT,
                                   overlay = OrderedDict() )
    SubStyle = drawable.Draw_Style(icon=subIcon,
                                   font=font, 
                                   method=drawable.Draw_Method.ICON, 
                                   minSize = minSize,
                                   placement=drawable.Text_Placement.RIGHT,
                                   overlay = OrderedDict() )
    CliStyle = drawable.Draw_Style(icon=clientIcon,
                                   font=font, 
                                   method=drawable.Draw_Method.ICON, 
                                   minSize = minSize,
                                   placement=drawable.Text_Placement.RIGHT,
                                   overlay = OrderedDict() )
    SerStyle = drawable.Draw_Style(icon=serverIcon,
                                   font=font, 
                                   method=drawable.Draw_Method.ICON, 
                                   minSize = minSize,
                                   placement=drawable.Text_Placement.RIGHT,
                                   overlay = OrderedDict() )
    NodeStyle = drawable.Draw_Style(icon=None,
                                    font=font, 
                                    method=drawable.Draw_Method.ROUND_RECT, 
                                    minSize = minSize,
                                    placement=drawable.Text_Placement.TOP,
                                    overlay = OrderedDict([('fillColor','TURQUOISE')]) )
    CompInstStyle = drawable.Draw_Style(icon=compInstIcon,
                                        font=font, 
                                        method=drawable.Draw_Method.ICON, 
                                        minSize = minSize,
                                        placement=drawable.Text_Placement.RIGHT,
                                        overlay = OrderedDict() )

    self.styleDict["workspace"] = WrkStyle
    self.styleDict["package"] = PkgStyle
    self.styleDict["message"] = MsgStyle
    self.styleDict["service"] = SrvStyle
    self.styleDict["component"] = CompStyle
    self.styleDict["timer"] = TmrStyle
    self.styleDict["publisher"] = PubStyle
    self.styleDict["subscriber"] = SubStyle
    self.styleDict["client"] = CliStyle
    self.styleDict["server"] = SerStyle
    self.styleDict["node"] = NodeStyle
    self.styleDict["component_instance"] = CompInstStyle
    self.styleDict["hardware_configuration"] = HardwareStyle
    self.styleDict["host"] = HostStyle
    self.styleDict["deployment"] = DeploymentStyle
    self.styleDict["group"] = GroupStyle
    self.styleDict["port_instance"] = PortInstStyle
    self.styleDict["host_instance"] = HostInstStyle
    self.styleDict["node_instance"] = NodeInstStyle
