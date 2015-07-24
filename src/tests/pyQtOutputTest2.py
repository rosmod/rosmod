from PyQt4 import QtCore, QtGui

class StdErrHandler(QtCore.QObject):
    '''
    This class provides an alternate write() method for stderr messages.
    Messages are sent by pyqtSignal to the pyqtSlot in the main window.
    '''
    err_msg = QtCore.pyqtSignal(str)

    def __init__(self, parent=None):
        QtCore.QObject.__init__(self)

    def write(self, msg):
        # stderr messages are sent to this method.
        self.err_msg.emit(msg)

class AppMainWindow(QtGui.QMainWindow):
    '''
    Main window with button to create an error
    '''
    def __init__(self, parent=None):
        # initialization of the superclass
        super(AppMainWindow, self).__init__(parent)

        # To avoid creating multiple error boxes
        self.err_box = None

        # Single button, connect to button handler
        self.create_err = QtGui.QPushButton(self)
        self.create_err.setText("Create Error")
        self.create_err.clicked.connect(self.err_btn_clicked)

    def err_btn_clicked(self):
        # Deliberately create a stderr output
        oopsie

    def std_err_post(self, msg):
        '''
        This method receives stderr text strings as a pyqtSlot.
        '''
        if self.err_box is None:
            self.err_box = QtGui.QMessageBox()
            # Both OK and window delete fire the 'finished' signal
            self.err_box.finished.connect(self.clear)
        # A single error is sent as a string of separate stderr .write() messages,
        # so concatenate them.
        self.err_box.setText(self.err_box.text() + msg)
        # .show() is used here because .exec() or .exec_() create multiple
        # MessageBoxes.
        self.err_box.show()

    def clear(self):
        # QMessageBox doesn't seem to be actually destroyed when closed, just hidden.
        # This is true even if destroy() is called or if the Qt.WA_DeleteOnClose
        # attribute is set.  Clear text for next time.
        self.err_box.setText('')

if __name__ == '__main__':
    import sys

    app = QtGui.QApplication(sys.argv)
    amw = AppMainWindow()
    amw.show()

    # Create the stderr handler and point stderr to it
    std_err_handler = StdErrHandler()
    sys.stderr = std_err_handler

    # Connect err_msg signal to message box method in main window
    std_err_handler.err_msg.connect(amw.std_err_post)

    sys.exit(app.exec_())
