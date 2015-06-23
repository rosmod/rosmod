import sys

from PyQt4.QtCore import *
from PyQt4.QtGui import *

class Widget(QWidget):

    def __init__(self, parent = None):
    
        QWidget.__init__(self, parent)
        self.message = ""
    
    def paintEvent(self, event):
    
        painter = QPainter()
        painter.begin(self)
        painter.fillRect(event.rect(), QBrush(Qt.white))
        painter.drawText(self.rect(), Qt.AlignCenter, self.message)
        painter.end()
    
    def mousePressEvent(self, event):
    
        self.last = "Click"
    
    def mouseReleaseEvent(self, event):
    
        if self.last == "Click":
            QTimer.singleShot(QApplication.instance().doubleClickInterval(),
                              self.performSingleClickAction)
        else:
            # Perform double click action.
            self.message = "Double Click"
            self.update()
    
    def mouseDoubleClickEvent(self, event):
    
        self.last = "Double Click"
    
    def performSingleClickAction(self):
    
        if self.last == "Click":
            self.message = "Click"
            self.update()


if __name__ == "__main__":

    app = QApplication(sys.argv)
    window = Widget()
    window.resize(400, 400)
    window.show()
    sys.exit(app.exec_())
