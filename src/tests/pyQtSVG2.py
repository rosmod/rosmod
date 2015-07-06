import random, sys
from PyQt4.QtCore import QSize, Qt
from PyQt4.QtGui import *
from PyQt4.QtSvg import *

def randomColor():

    red = 205 + random.random() * 50
    green = 205 + random.random() * 50
    blue = 205 + random.random() * 50
    alpha = 91 + random.random() * 100
    
    return QColor(red, green, blue, alpha)


if __name__ == "__main__":

    if len(sys.argv) != 2:
    
        sys.stderr.write("Usage: %s <output path>\n" % sys.argv[0])
        sys.exit(1)
    
    app = QApplication(sys.argv)
    
    svg = QSvgGenerator()
    svg.setFileName(sys.argv[1])
    svg.setResolution(300)
    svg.setSize(QSize(1024, 768))
    
    random.seed()
    
    p = QPainter()
    p.begin(svg)
    
    clipPath = QPainterPath()
    clipPath.addRect(0, 0, 1024, 768)
    
    stroker = QPainterPathStroker()
    stroker.setWidth(16)
    stroker.setDashPattern([0.1, 1.5])
    
    gradient = QLinearGradient(0, 0, 1024, 768)
    gradient.setColorAt(0, QColor(0, 64, 64))
    gradient.setColorAt(1, QColor(0, 0, 64))
    p.fillRect(0, 0, 1024, 768, QBrush(gradient))
    p.setPen(Qt.NoPen)
    
    font = QFont("FreeSans")
    strokeColor = QColor(0, 127, 255)
    
    for i in range(100):
    
        w = 10 + random.random() * 200.0
        h = 0.5 * (1 + random.random()) * w
        color = randomColor()
        x = random.random() * 1024
        y = random.random() * 768
        
        rectPath = QPainterPath()
        rectPath.addRoundedRect(x - w/2, y - h/2, w, h, 20, 20)
        
        strokedPath = stroker.createStroke(rectPath)
        
        # Draw the background of the rounded rectangle.
        p.setBrush(color)
        p.drawPath(clipPath.intersected(rectPath))
        # Draw the outline of the rounded rectangle.
        p.setBrush(strokeColor)
        p.drawPath(clipPath.intersected(strokedPath))
        
        font.setPixelSize(min(w/2, h/2))
        metrics = QFontMetrics(font, svg)
        text = chr(random.randrange(97, 123))
        rect = metrics.boundingRect(x - w/2, y - h/2, w, h, Qt.AlignCenter, text)
        textPath = QPainterPath()
        textPath.addText(x - rect.width()/2, y + rect.height()/2 - metrics.descent(), font, text)
        
        p.setBrush(QColor(0, 0, 0, color.alpha()))
        p.drawPath(clipPath.intersected(textPath))
    
    p.end()
    
    sys.exit()
