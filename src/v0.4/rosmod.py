#!/usr/bin/python

from PyQt4 import QtGui

from PyQt4.QtCore import *
from PyQt4.QtGui import *

from worker import *

class Rosmod(QtGui.QWidget):
    def __init__(self, parent = None):
        QtGui.QWidget.__init__(self, parent)

        self.meta_model = None
        self.model = None

        # These are for handling background work
        # such as parts of rendering, deployment, I/O
        self.thread = Worker()

        # These are for rendering and display
        self.modelTree = None
        self.tabbedViewer = None
        self.output = None
        self.toolbar = None
        self.menu = None

if __name__ == '__main__':
    import sys
    app = QtGui.QApplication(sys.argv)
    window = Rosmod()
    window.show()
    sys.exit(app.exec_())
