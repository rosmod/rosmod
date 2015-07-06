'''
combine 2 pixmaps into one.
'''

from PyQt4 import QtCore, QtGui

IMAGE1 = "client.png"
IMAGE2 = "build.png"

app = QtGui.QApplication([])

pm1 = QtGui.QPixmap(IMAGE1)
pm2 = QtGui.QPixmap(IMAGE2)

pm = QtGui.QPixmap(400,200)

label = QtGui.QLabel()

left_rectF = QtCore.QRectF(0,0,200,200)    #the left half
right_rectF = QtCore.QRectF(200,0,400,200) #the right half

painter = QtGui.QPainter(pm)
painter.drawPixmap(left_rectF, pm1, QtCore.QRectF(pm1.rect()))
painter.drawPixmap(right_rectF, pm2, QtCore.QRectF(pm2.rect()))

label.setPixmap(pm)
label.show()

app.exec_()
