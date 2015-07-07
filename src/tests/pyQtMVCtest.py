# Written by Robin Burchell 
# No licence specified or required, but please give credit where it's due, and please let me know if this helped you.
# Feel free to contact with corrections or suggestions.
#
# We're using PySide, Nokia's official LGPL bindings.
# You can however easily use PyQt (Riverside Computing's GPL bindings) by commenting these and fixing the appropriate imports.
#from PySide.QtCore import *
#from PySide.QtGui import *
from PyQt4 import *
from PyQt4.QtCore import *
from PyQt4.QtGui import *
import sys

class SimpleListModel(QAbstractListModel):
    def __init__(self, mlist):
        QAbstractListModel.__init__(self)

        # Cache the passed data list as a class member.
        self._items = mlist

    def rowCount(self, parent = QModelIndex()):
        return len(self._items)

    def data(self, index, role = Qt.DisplayRole):
        if role == Qt.DisplayRole:
            # The view is asking for the actual data, so, just return the item it's asking for.
            return QVariant(self._items[index.row()])
        elif role == Qt.BackgroundRole:
            # Implementation is up to you.
            if index.row() % 2 == 0:
                return QVariant(QColor(Qt.gray))
            else:
                return QVariant(QColor(Qt.lightGray))
        else:
            return QVariant()

# This widget is our view of the readonly list.
# Obviously, in a real application, this will be more complex, with signals/etc usage, but
# for the scope of this tutorial, let's keep it simple, as always.
#
# For more information, see:
# http://doc.trolltech.com/4.6/qlistview.html
class SimpleListView(QListView):
    def __init__(self, parent = None):
        QListView.__init__(self, parent)

# Our main application window.
# You should be used to this from previous tutorials.
class MyMainWindow(QWidget):
    def __init__(self):
        QWidget.__init__(self, None)

        # main section of the window
        vbox = QVBoxLayout()

        m = SimpleListModel(["test", "tes1t", "t3est", "t5est", "t3est"])

        v = SimpleListView()
        v.setModel(m)
        vbox.addWidget(v)
        
        v = SimpleListView()
        v.setModel(m)
        vbox.addWidget(v)
        
        hbox = QHBoxLayout()

        vbox.addLayout(hbox)

        self.setLayout(vbox)

# set things up, and run it. :)
if __name__ == '__main__':
    app = QApplication(sys.argv)
    w = MyMainWindow()
    w.show()
    app.exec_()
    sys.exit()
