#!/usr/bin/env python

import random, sys
from PyQt4.QtCore import pyqtSignal, QObject, Qt, QTimer, QVariant
from PyQt4.QtGui import *

class Delegate(QItemDelegate):

    needsRedraw = pyqtSignal()
    
    def __init__(self, movie, parent = None):
    
        QItemDelegate.__init__(self, parent)
        self.movie = movie
        self.movie.frameChanged.connect(self.needsRedraw)
        self.playing = False
    
    def startMovie(self):
        self.movie.start()
        self.playing = True
    
    def stopMovie(self):
        self.movie.stop()
        self.playing = False
    
    def paint(self, painter, option, index):
    
        waiting = index.data(Qt.UserRole).toBool()
        if waiting:
            option = option.__class__(option)
            pixmap = self.movie.currentPixmap()
            painter.drawPixmap(option.rect.topLeft(), pixmap)
            option.rect = option.rect.translated(pixmap.width(), 0)
        
        QItemDelegate.paint(self, painter, option, index)


class Model(QStandardItemModel):

    finished = pyqtSignal()
    
    def __init__(self, parent = None):
    
        QStandardItemModel.__init__(self, parent)
        self.pendingItems = {}
    
    def appendRow(self, item):
    
        if item.data(Qt.UserRole).toBool():
        
            timer = QTimer()
            timer.timeout.connect(self.checkPending)
            timer.setSingleShot(True)
            self.pendingItems[timer] = item
            timer.start(2000 + random.randrange(0, 2000))
        
        QStandardItemModel.appendRow(self, item)
    
    def checkPending(self):
    
        # Check when items are updated so that we can emit the finished()
        # signal when the list is cleared.
        item = self.pendingItems[self.sender()]
        del self.pendingItems[self.sender()]
        item.setData(QVariant(False), Qt.UserRole)
        if not self.pendingItems:
            self.finished.emit()


if __name__ == "__main__":

    random.seed()
    
    app = QApplication(sys.argv)
    view = QListView()
    model = Model()
    waiting = True
    
    for i in range(5):
    
        item = QStandardItem("Test %i" % i)
        item.setData(QVariant(waiting), Qt.UserRole)
        waiting = not waiting
        model.appendRow(item)
    
    view.setModel(model)
    
    delegate = Delegate(QMovie("animation.mng"))
    view.setItemDelegate(delegate)
    delegate.needsRedraw.connect(view.viewport().update)
    delegate.startMovie()
    
    model.finished.connect(delegate.stopMovie)
    model.finished.connect(view.viewport().update)
    
    view.show()
    sys.exit(app.exec_())
