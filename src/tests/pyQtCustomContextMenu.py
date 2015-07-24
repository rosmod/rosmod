import sys
from PyQt4 import QtGui, QtCore
class MainForm(QtGui.QMainWindow):
    def __init__(self, parent=None):
        super(MainForm, self).__init__(parent)

        # create button
        self.button = QtGui.QPushButton("test button",self)       
        self.button.resize(100, 30)

        # set button context menu policy
        self.button.setContextMenuPolicy(QtCore.Qt.CustomContextMenu)
        self.connect(self.button, QtCore.SIGNAL('customContextMenuRequested(const QPoint&)'), self.on_context_menu)

    def on_context_menu(self, point):
        self.popMenu = QtGui.QMenu(self)
        #some test list for test
        testItems = ['itemA', 'itemB', 'itemC']
        for item in testItems:
            action = self.popMenu.addAction('Selected %s' % item)
            action.triggered[()].connect(
                lambda item=item: self.printItem(item))
        self.popMenu.exec_(self.button.mapToGlobal(point))

    @QtCore.pyqtSlot(str)
    def printItem(self, item):
        print item

def main():
    app = QtGui.QApplication(sys.argv)
    form = MainForm()
    form.show()
    app.exec_()

if __name__ == '__main__':
    main()
