from PyQt4 import QtGui

class Window(QtGui.QWidget):
    def __init__(self):
        QtGui.QWidget.__init__(self)
        button = QtGui.QPushButton('Open Dialog', self)
        button.clicked.connect(self.handleOpenDialog)
        self.resize(300, 200)
        self._dialog = None

    def handleOpenDialog(self):
        if self._dialog is None:
            self._dialog = QtGui.QDialog(self)
            self._dialog.resize(200, 100)
        self._dialog.exec_()

if __name__ == '__main__':

    import sys
    app = QtGui.QApplication(sys.argv)
    win = Window()
    win.show()
    sys.exit(app.exec_())
