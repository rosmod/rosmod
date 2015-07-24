from PyQt4 import QtCore, QtGui, QtXml

class Tree(QtGui.QTreeWidget):

    def __init__(self, parent):
        # maybe init your data here too
        super(Tree, self).__init__(parent)

    def populate(self, data):
        # populate the tree with QTreeWidgetItem items
        for row in data:
            # is attached to the root (parent) widget
            rowItem = QtGui.QTreeWidgetItem(parent)
            rowItem.setText(0, row)
            for subRow in row:
                 # is attached to the current row (rowItem) widget
                 subRowItem = QtGui.QTreeWidgetItem(rowItem)
                 subRowItem.setText(0, subRow)
	
class XmlHandler(QtXml.QXmlDefaultHandler):
    qnames = [
        "package", "component", "client", "timer", "server", "publisher",
        "subscriber", "rhw", "hardware", "rdp", "node", "component_instance", "logging"
    ]
    def __init__(self, root):
        QtXml.QXmlDefaultHandler.__init__(self)
        self._root = root
        self._item = None
        self._text = ''
        self._error = ''

    def startElement(self, namespace, name, qname, attributes):
        if self._item is not None:
            self._item = QtGui.QTreeWidgetItem(self._item)
        else:
            self._item = QtGui.QTreeWidgetItem(self._root)
        self._item.setData(0, QtCore.Qt.UserRole, qname)
        setText = attributes.value('name')
        if not setText:
            setText = qname
        self._item.setText(0, setText)
        if self._item is not None:
            self._item.setText(1, attributes.value('value'))
        self._text = ''
        return True

    def endElement(self, namespace, name, qname):
        if qname == 'name':
            if self._item is not None:
                self._item.setText(0, self._text)
        self._item = self._item.parent()
        return True

    def characters(self, text):
        self._text += text
        return True

    def fatalError(self, exception):
        print('Parse Error: line %d, column %d:n  %s' % (
              exception.lineNumber(),
              exception.columnNumber(),
              exception.message(),
              ))
        return False

    def errorString(self):
        return self._error

class Window(QtGui.QTreeWidget):
    def __init__(self):
        QtGui.QTreeWidget.__init__(self)
        self.header().setResizeMode(QtGui.QHeaderView.Stretch)
        self.setHeaderLabels(['Title', 'Value'])
        source = QtXml.QXmlInputSource()
        source.setData(xml)
        handler = XmlHandler(self)
        reader = QtXml.QXmlSimpleReader()
        reader.setContentHandler(handler)
        reader.setErrorHandler(handler)
        reader.parse(source)

xml = """
<project name="project-name-here">

  <package name="client_server_package">
    <component name="Client">
      <client name="client_port" reference="client_server_package/Power"/>
      <timer name="client_timer">
	<period value="1.0"/>
	<priority value="50"/>
	<deadline value="3.0"/>
      </timer>
    </component>
    <component name="Server">
      <server name="server_port" reference="client_server_package/Power">
	<priority value="50"/>
	<deadline value="3.0"/>
      </server>
    </component>
  </package>
  
  <rhw name="hardware">
    <hardware name="laptop">
      <ip_address value="localhost"/>
      <username value="kelsier"/>
      <sshkey value="path/to/key"/>
      <deployment_path value="/path/to/deploy"/>
      <arch value="x86"/>
    </hardware>
  </rhw>

  <rdp name="deployment">
    <hardware_reference value="hardware"/>
    <node name="Client_Server_Node">
      <reference value="hardware/laptop"/>
      <priority value="50"/>
      <component_instance name="Client_Instance">
	<reference value="client_server_package/Client"/>
	<scheduling_scheme value="FIFO"/>
	<logging>
	  <DEBUG value="True"/>
	  <INFO value="True"/>
	  <WARNING value="True"/>
	  <ERROR value="True"/>
	  <CRITICAL value="True"/>
	</logging>
      </component_instance>
      <component_instance name="Server_Instance">
	<reference value="client_server_package/Server"/>
	<scheduling_scheme value="FIFO"/>
	<logging>
	  <DEBUG value="True"/>
	  <INFO value="True"/>
	  <WARNING value="True"/>
	  <ERROR value="True"/>
	  <CRITICAL value="True"/>
	</logging>
      </component_instance>
    </node>
  </rdp>

</project>
"""

if __name__ == '__main__':

    import sys
    app = QtGui.QApplication(sys.argv)
    window = Window()
    window.resize(400, 300)
    window.show()
    sys.exit(app.exec_())
