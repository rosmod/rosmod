import sys
from PyQt4 import QtGui, QtCore, Qt
import math

class Example(QtGui.QWidget):
    
    def __init__(self):
        super(Example, self).__init__()
        
        self.setGeometry(0, 0,700,700)
        self.move(300, 200)
        self.setWindowTitle('Dial Guage')

        layout = QtGui.QVBoxLayout(self)
        lbl = QtGui.QLabel("A Label")
        layout.addWidget(lbl)

        self.gauge = GaugeWidget(0.5)
        self.gauge.setSizePolicy(QtGui.QSizePolicy(
            QtGui.QSizePolicy.Expanding, 
            QtGui.QSizePolicy.Expanding))
        layout.addWidget(self.gauge)


class GaugeWidget(QtGui.QWidget):

    def __init__(self, initialValue=0, *args, **kwargs):
        super(GaugeWidget, self).__init__(*args, **kwargs)
        self._bg = QtGui.QPixmap("dial.png")
        self.setValue(initialValue)

    def setValue(self, val):
        val = float(min(max(val, 0), 1))
        self._value = -270 * val
        self.update()

    def paintEvent(self, e):
        painter = QtGui.QPainter(self)
        painter.setRenderHint(painter.Antialiasing)
        rect = e.rect()

        gauge_rect = QtCore.QRect(rect)
        size = gauge_rect.size()
        pos = gauge_rect.center()
        gauge_rect.moveCenter( QtCore.QPoint(pos.x()-size.width(), pos.y()-size.height()) )
        gauge_rect.setSize(size*.9)
        gauge_rect.moveCenter(pos)

        refill_rect = QtCore.QRect(gauge_rect)
        size = refill_rect.size()
        pos = refill_rect.center()
        refill_rect.moveCenter( QtCore.QPoint(pos.x()-size.width(), pos.y()-size.height()) )
        # smaller than .9 == thicker gauge
        refill_rect.setSize(size*.9)
        refill_rect.moveCenter(pos)

        painter.setPen(QtCore.Qt.NoPen)

        painter.drawPixmap(rect, self._bg )

        painter.save()
        grad = QtGui.QConicalGradient(QtCore.QPointF(gauge_rect.center()), 270.0)
        grad.setColorAt(.75, QtCore.Qt.green)
        grad.setColorAt(.5, QtCore.Qt.yellow)
        grad.setColorAt(.1, QtCore.Qt.red)
        painter.setBrush(grad)
        #painter.drawPie(gauge_rect, 225.0*16, self._value*16)
        painter.drawPie(gauge_rect, 225.0*16, self._value*16)
        painter.restore()

        painter.setBrush(QtGui.QBrush(self._bg.scaled(rect.size())))
        painter.drawEllipse(refill_rect)

        painter.end()
        super(GaugeWidget,self).paintEvent(e)
        
        
def main():
    app = QtGui.QApplication(sys.argv)
    ex = Example()
    ex.show()
    ex.raise_()
    sys.exit(app.exec_())


if __name__ == '__main__':
    main()
