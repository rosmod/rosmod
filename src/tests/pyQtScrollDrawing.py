import sys
from PyQt4 import QtGui, QtCore

class MyView(QtGui.QGraphicsView):
    def __init__(self):
        QtGui.QGraphicsView.__init__(self)

        self.setGeometry(QtCore.QRect(100, 100, 600, 250))

        self.scene = QtGui.QGraphicsScene(self)
        self.scene.setSceneRect(QtCore.QRectF())

        self.setScene(self.scene)

        for i in range(5):
            self.item = QtGui.QGraphicsEllipseItem(i*75, 10, 60, 40)
            self.scene.addItem(self.item)


if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    view = MyView()
    view.show()
    sys.exit(app.exec_())
