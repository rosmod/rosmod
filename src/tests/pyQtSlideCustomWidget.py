# -*- coding: utf-8 -*-
# Written by Robin Burchell
# No licence specified or required, but please give credit where it's due,
# and please let me know if this helped you. Feel free to contact with corrections or suggestions.
#
# We're using PySide, Nokia's official LGPL bindings.
# You can however easily use PyQt (Riverside Computing's GPL bindings) by
# commenting these and fixing the appropriate imports.
from PySide.QtCore import *
from PySide.QtGui import *
#from PyQt4 import *
#from PyQt4.QtCore import *
#from PyQt4.QtGui import *
import sys

# As always, Qt allows us to subclass objects to override behaviour and generally monkey around
# with them how we want. This is the exact way that custom widget painting operates:
# you subclass the widget that you want as your base, and override paintEvent() to do your own painting.
class CustomWidget(QWidget):
    def __init__(self, parent, anumber):
        QWidget.__init__(self, parent)
        self._number = anumber

    def paintEvent(self, ev):
        p = QPainter(self)
        p.fillRect(self.rect(), QBrush(Qt.red, Qt.Dense2Pattern))
        p.drawText(self.rect(), Qt.AlignLeft | Qt.AlignVCenter, str(self._number))
        p.setPen(QColor(220, 220, 220))
        p.drawText(self.rect(), Qt.AlignRight | Qt.AlignVCenter, str(self._number * 2))

class MyMainWindow(QMainWindow):
    def __init__(self, parent):
        QMainWindow.__init__(self, parent)
        
        central = CustomWidget(self, 666)
        self.setCentralWidget(central)

if __name__ == '__main__':
    app = QApplication(sys.argv)
    sw = MyMainWindow(None)
    sw.show()
    app.exec_()
    sys.exit()
