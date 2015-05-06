#!/usr/bin/env python




##################################################
## DEPENDENCIES
import sys
import os
import os.path
try:
    import builtins as builtin
except ImportError:
    import __builtin__ as builtin
from os.path import getmtime, exists
import time
import types
from Cheetah.Version import MinCompatibleVersion as RequiredCheetahVersion
from Cheetah.Version import MinCompatibleVersionTuple as RequiredCheetahVersionTuple
from Cheetah.Template import Template
from Cheetah.DummyTransaction import *
from Cheetah.NameMapper import NotFound, valueForName, valueFromSearchList, valueFromFrameOrSearchList
from Cheetah.CacheRegion import CacheRegion
import Cheetah.Filters as Filters
import Cheetah.ErrorCatchers as ErrorCatchers

##################################################
## MODULE CONSTANTS
VFFSL=valueFromFrameOrSearchList
VFSL=valueFromSearchList
VFN=valueForName
currentTime=time.time
__CHEETAH_version__ = '2.4.4'
__CHEETAH_versionTuple__ = (2, 4, 4, 'development', 0)
__CHEETAH_genTime__ = 1430950960.786106
__CHEETAH_genTimestamp__ = 'Wed May  6 17:22:40 2015'
__CHEETAH_src__ = '/home/kelsier/Repositories/rosmod/code/rosmod_v3/src/templates/rdp.tmpl'
__CHEETAH_srcLastModified__ = 'Wed May  6 17:03:48 2015'
__CHEETAH_docstring__ = 'Autogenerated by Cheetah: The Python-Powered Template Engine'

if __CHEETAH_versionTuple__ < RequiredCheetahVersionTuple:
    raise AssertionError(
      'This template was compiled with Cheetah version'
      ' %s. Templates compiled before version %s must be recompiled.'%(
         __CHEETAH_version__, RequiredCheetahVersion))

##################################################
## CLASSES

class rdp(Template):

    ##################################################
    ## CHEETAH GENERATED METHODS


    def __init__(self, *args, **KWs):

        super(rdp, self).__init__(*args, **KWs)
        if not self._CHEETAH__instanceInitialized:
            cheetahKWArgs = {}
            allowedKWs = 'searchList namespaces filter filtersLib errorCatcher'.split()
            for k,v in KWs.items():
                if k in allowedKWs: cheetahKWArgs[k] = v
            self._initCheetahInstance(**cheetahKWArgs)
        

    def respond(self, trans=None):



        ## CHEETAH: main method generated for this template
        if (not trans and not self._CHEETAH__isBuffering and not callable(self.transaction)):
            trans = self.transaction # is None unless self.awake() was called
        if not trans:
            trans = DummyTransaction()
            _dummyTrans = True
        else: _dummyTrans = False
        write = trans.response().write
        SL = self._CHEETAH__searchList
        _filter = self._CHEETAH__currentFilter
        
        ########################################
        ## START - generated method body
        
        write(u'''/*
 * ROSMOD Deployment Model
 */

// ROSMOD Hardware Model - ''')
        _v = VFN(VFN(VFFSL(SL,"deployment",True),"properties",True)["rhw_reference"],"properties",True)["name"] # u'$deployment.properties["rhw_reference"].properties["name"]' on line 5, col 28
        if _v is not None: write(_filter(_v, rawExpr=u'$deployment.properties["rhw_reference"].properties["name"]')) # from line 5, col 28.
        write(u'''
using ''')
        _v = VFN(VFN(VFFSL(SL,"deployment",True),"properties",True)["rhw_reference"],"properties",True)["name"] # u'$deployment.properties["rhw_reference"].properties["name"]' on line 6, col 7
        if _v is not None: write(_filter(_v, rawExpr=u'$deployment.properties["rhw_reference"].properties["name"]')) # from line 6, col 7.
        write(u''';

''')
        if VFFSL(SL,"deployment.children",True) != []: # generated from line 8, col 1
            for hardware_instance in VFN(VFFSL(SL,"deployment",True),"getChildrenByKind",False)("Hardware_Instance"): # generated from line 9, col 1
                write(u'''// ROSMOD Hardware Instance - ''')
                _v = VFN(VFFSL(SL,"hardware_instance",True),"properties",True)["name"] # u'$hardware_instance.properties["name"]' on line 10, col 31
                if _v is not None: write(_filter(_v, rawExpr=u'$hardware_instance.properties["name"]')) # from line 10, col 31.
                write(u'''
hardware_instance ''')
                _v = VFN(VFFSL(SL,"hardware_instance",True),"properties",True)["name"] # u'$hardware_instance.properties["name"]' on line 11, col 19
                if _v is not None: write(_filter(_v, rawExpr=u'$hardware_instance.properties["name"]')) # from line 11, col 19.
                write(u'''
{
    properties
    {
        ref = "''')
                _v = VFN(VFN(VFFSL(SL,"deployment",True),"properties",True)["rhw_reference"],"properties",True)["name"] # u'${deployment.properties["rhw_reference"].properties["name"]}' on line 15, col 16
                if _v is not None: write(_filter(_v, rawExpr=u'${deployment.properties["rhw_reference"].properties["name"]}')) # from line 15, col 16.
                write(u'''/''')
                _v = VFN(VFN(VFFSL(SL,"hardware_instance",True),"properties",True)["hardware_reference"],"properties",True)["name"] # u'$hardware_instance.properties["hardware_reference"].properties["name"]' on line 15, col 77
                if _v is not None: write(_filter(_v, rawExpr=u'$hardware_instance.properties["hardware_reference"].properties["name"]')) # from line 15, col 77.
                write(u'''";
        username = "''')
                _v = VFN(VFFSL(SL,"hardware_instance",True),"properties",True)["username"] # u'$hardware_instance.properties["username"]' on line 16, col 21
                if _v is not None: write(_filter(_v, rawExpr=u'$hardware_instance.properties["username"]')) # from line 16, col 21.
                write(u'''";
\tsshkey = "''')
                _v = VFN(VFFSL(SL,"hardware_instance",True),"properties",True)["sshkey"] # u'$hardware_instance.properties["sshkey"]' on line 17, col 12
                if _v is not None: write(_filter(_v, rawExpr=u'$hardware_instance.properties["sshkey"]')) # from line 17, col 12.
                write(u'''";
\tdeployment_path = "''')
                _v = VFN(VFFSL(SL,"hardware_instance",True),"properties",True)["deployment_path"] # u'$hardware_instance.properties["deployment_path"]' on line 18, col 21
                if _v is not None: write(_filter(_v, rawExpr=u'$hardware_instance.properties["deployment_path"]')) # from line 18, col 21.
                write(u'''";
''')
                if VFN(VFFSL(SL,"hardware_instance",True),"properties",True)["init"] != "": # generated from line 19, col 1
                    write(u'''\tinit = "''')
                    _v = VFN(VFFSL(SL,"hardware_instance",True),"properties",True)["init"] # u'$hardware_instance.properties["init"]' on line 20, col 10
                    if _v is not None: write(_filter(_v, rawExpr=u'$hardware_instance.properties["init"]')) # from line 20, col 10.
                    write(u'''";
''')
                write(u'''    }
''')
                if VFFSL(SL,"hardware_instance.children",True) != []: # generated from line 23, col 1
                    for node in VFFSL(SL,"hardware_instance.children",True): # generated from line 24, col 1
                        write(u'''    // ROSMOD Node - ''')
                        _v = VFN(VFFSL(SL,"node",True),"properties",True)["name"] # u'$node.properties["name"]' on line 25, col 22
                        if _v is not None: write(_filter(_v, rawExpr=u'$node.properties["name"]')) # from line 25, col 22.
                        write(u''' 
    node ''')
                        _v = VFN(VFFSL(SL,"node",True),"properties",True)["name"] # u'$node.properties["name"]' on line 26, col 10
                        if _v is not None: write(_filter(_v, rawExpr=u'$node.properties["name"]')) # from line 26, col 10.
                        write(u'''
    {
        properties
\t{
\t    priority = ''')
                        _v = VFN(VFFSL(SL,"node",True),"properties",True)["priority"] # u'$node.properties["priority"]' on line 30, col 17
                        if _v is not None: write(_filter(_v, rawExpr=u'$node.properties["priority"]')) # from line 30, col 17.
                        write(u''';
''')
                        if VFN(VFFSL(SL,"node",True),"properties",True)["cmd_args"] != "": # generated from line 31, col 1
                            write(u'''    \t    cmd_args = "''')
                            _v = VFN(VFFSL(SL,"node",True),"properties",True)["cmd_args"] # u'$node.properties["cmd_args"]' on line 32, col 22
                            if _v is not None: write(_filter(_v, rawExpr=u'$node.properties["cmd_args"]')) # from line 32, col 22.
                            write(u'''";\t\t   
''')
                        write(u'''        }
''')
                        if VFFSL(SL,"node.children",True) != []: # generated from line 35, col 1
                            for comp_instance in VFFSL(SL,"node.children",True): # generated from line 36, col 1
                                write(u'''        component_instance ''')
                                _v = VFN(VFFSL(SL,"comp_instance",True),"properties",True)["name"] # u'$comp_instance.properties["name"]' on line 37, col 28
                                if _v is not None: write(_filter(_v, rawExpr=u'$comp_instance.properties["name"]')) # from line 37, col 28.
                                write(u'''
\t{
            properties
\t    {
 \t        ref = "''')
                                _v = VFN(VFN(VFN(VFFSL(SL,"comp_instance",True),"properties",True)["component_reference"],"parent",True),"properties",True)["name"] # u'$comp_instance.properties["component_reference"].parent.properties["name"]' on line 41, col 18
                                if _v is not None: write(_filter(_v, rawExpr=u'$comp_instance.properties["component_reference"].parent.properties["name"]')) # from line 41, col 18.
                                write(u'''/''')
                                _v = VFN(VFN(VFFSL(SL,"comp_instance",True),"properties",True)["component_reference"],"properties",True)["name"] # u'$comp_instance.properties["component_reference"].properties["name"]' on line 41, col 93
                                if _v is not None: write(_filter(_v, rawExpr=u'$comp_instance.properties["component_reference"].properties["name"]')) # from line 41, col 93.
                                write(u'''";
                scheduling_scheme = ''')
                                _v = VFN(VFFSL(SL,"comp_instance",True),"properties",True)["scheduling_scheme"] # u'$comp_instance.properties["scheduling_scheme"]' on line 42, col 37
                                if _v is not None: write(_filter(_v, rawExpr=u'$comp_instance.properties["scheduling_scheme"]')) # from line 42, col 37.
                                write(u''';
\t        logging
\t        {
''')
                                if VFN(VFFSL(SL,"comp_instance",True),"properties",True)["logging_debug"] != False: # generated from line 45, col 1
                                    write(u'''\t            DEBUG = true;
''')
                                else: # generated from line 47, col 1
                                    write(u'''\t            DEBUG = false;
''')
                                if VFN(VFFSL(SL,"comp_instance",True),"properties",True)["logging_info"] != False: # generated from line 50, col 1
                                    write(u'''\t            INFO = true;
''')
                                else: # generated from line 52, col 1
                                    write(u'''\t            INFO = false;
''')
                                if VFN(VFFSL(SL,"comp_instance",True),"properties",True)["logging_warning"] != False: # generated from line 55, col 1
                                    write(u'''\t            WARNING = true;
''')
                                else: # generated from line 57, col 1
                                    write(u'''\t            WARNING = false;
''')
                                if VFN(VFFSL(SL,"comp_instance",True),"properties",True)["logging_error"] != False: # generated from line 60, col 1
                                    write(u'''\t            ERROR = true;
''')
                                else: # generated from line 62, col 1
                                    write(u'''\t            ERROR = false;
''')
                                if VFN(VFFSL(SL,"comp_instance",True),"properties",True)["logging_critical"] != False: # generated from line 65, col 1
                                    write(u'''                    CRITICAL = true;
''')
                                else: # generated from line 67, col 1
                                    write(u'''\t            CRITICAL = false;
''')
                                write(u'''\t        }
            }
''')
                                if VFFSL(SL,"comp_instance.children",True) != []: # generated from line 72, col 1
                                    for port_instance in VFFSL(SL,"comp_instance.children",True): # generated from line 73, col 1
                                        write(u'''            port_instance ''')
                                        _v = VFN(VFFSL(SL,"port_instance",True),"properties",True)["name"] # u'$port_instance.properties["name"]' on line 74, col 27
                                        if _v is not None: write(_filter(_v, rawExpr=u'$port_instance.properties["name"]')) # from line 74, col 27.
                                        write(u''' 
\t    {
\t          ref = "''')
                                        _v = VFN(VFN(VFN(VFFSL(SL,"port_instance",True),"properties",True)["port_reference"],"parent",True),"properties",True)["name"] # u'$port_instance.properties["port_reference"].parent.properties["name"]' on line 76, col 19
                                        if _v is not None: write(_filter(_v, rawExpr=u'$port_instance.properties["port_reference"].parent.properties["name"]')) # from line 76, col 19.
                                        write(u'''/''')
                                        _v = VFN(VFN(VFFSL(SL,"port_instance",True),"properties",True)["port_reference"],"properties",True)["name"] # u'$port_instance.properties["port_reference"].properties["name"]' on line 76, col 89
                                        if _v is not None: write(_filter(_v, rawExpr=u'$port_instance.properties["port_reference"].properties["name"]')) # from line 76, col 89.
                                        write(u'''";
                  group = "''')
                                        _v = VFN(VFFSL(SL,"port_instance",True),"properties",True)["group"] # u'$port_instance.properties["group"]' on line 77, col 28
                                        if _v is not None: write(_filter(_v, rawExpr=u'$port_instance.properties["group"]')) # from line 77, col 28.
                                        write(u'''";
            }
''')
                                write(u'''        }
''')
                        write(u'''    }
''')
                write(u'''}
''')
        write(u'''
''')
        
        ########################################
        ## END - generated method body
        
        return _dummyTrans and trans.response().getvalue() or ""
        
    ##################################################
    ## CHEETAH GENERATED ATTRIBUTES


    _CHEETAH__instanceInitialized = False

    _CHEETAH_version = __CHEETAH_version__

    _CHEETAH_versionTuple = __CHEETAH_versionTuple__

    _CHEETAH_genTime = __CHEETAH_genTime__

    _CHEETAH_genTimestamp = __CHEETAH_genTimestamp__

    _CHEETAH_src = __CHEETAH_src__

    _CHEETAH_srcLastModified = __CHEETAH_srcLastModified__

    _mainCheetahMethod_for_rdp= 'respond'

## END CLASS DEFINITION

if not hasattr(rdp, '_initCheetahAttributes'):
    templateAPIClass = getattr(rdp, '_CHEETAH_templateClass', Template)
    templateAPIClass._addCheetahPlumbingCodeToClass(rdp)


# CHEETAH was developed by Tavis Rudd and Mike Orr
# with code, advice and input from many other volunteers.
# For more information visit http://www.CheetahTemplate.org/

##################################################
## if run from command line:
if __name__ == '__main__':
    from Cheetah.TemplateCmdLineIface import CmdLineIface
    CmdLineIface(templateObj=rdp()).run()


