#!/usr/bin/python

from rosmod import Example
from cmdline import *

def main(argv):
    no_gui = False
    for arg in argv:
        if arg == "-ng" or arg == "--no-gui" or arg == "-nogui" or arg == "--nogui":
            no_gui = True

    # Is ROSMOD invoked with "no gui" option?
    if no_gui == True:
        interface = ROSMOD_Commandline()
        interface.run()
    else:
        # If GUI required, check if DISPLAY exists
        if 'DISPLAY' in os.environ.keys():
            ex = wx.App()
            Example(None)
            ex.MainLoop()    
        else:
            print "ERROR::No Display Found! Please run ROSMOD with no GUI option \"-ng\""

if __name__ == '__main__':
    main(sys.argv)
