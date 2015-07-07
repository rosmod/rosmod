from PyQt4.QtCore import *
from PyQt4.QtGui import *

class Worker(QThread):

    def __init__(self, parent = None):
        QThread.__init__(self, parent)
        self.exiting = False

    def __del__(self):
        self.exiting = True
        self.wait()

    def render(self):
        self.start()

    def run(self):
        # Note: This is never called directly. It is called by Qt once the
        # thread environment has been set up.
        while not self.exiting:
            self.emit(SIGNAL("output(QRect)"),
                      QRect(10,10,20,20) )
