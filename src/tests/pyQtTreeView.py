import sys
from PyQt4.QtCore import *
from PyQt4.QtGui import *

tree_data = [
    ("Alice", [
        ("Keys", []),
        ("Purse", [
            ("Cellphone", [])
            ])
        ]),
    ("Bob", [
        ("Wallet", [
            ("Credit card", []),
            ("Money", [])
            ])
        ])
    ]

class Window(QWidget):

    def __init__(self):
    
        QWidget.__init__(self)
        
        self.treeView = QTreeView()
        self.tableView = QTableView()
        self.tableView.horizontalHeader().setStretchLastSection(True)
        
        self.model = QStandardItemModel()
        self.addTreeItems(self.model, tree_data)
        self.treeView.setModel(self.model)
        
        treeSelectionModel = self.treeView.selectionModel()
        treeSelectionModel.selectionChanged.connect(self.addTableItems)
        
        layout = QHBoxLayout(self)
        layout.addWidget(self.treeView)
        layout.addWidget(self.tableView)
    
    def addTreeItems(self, parent, elements):
    
        for text, children in elements:
            item = QStandardItem(text)
            parent.appendRow(item)
            if children:
                self.addTreeItems(item, children)
    
    def addTableItems(self, selected, deselected):
    
        # Find the top-level item in the tree.
        treeIndex = selected.indexes()[0]
        
        parent = treeIndex.parent()
        while parent.isValid():
            treeIndex = parent
            parent = parent.parent()
        
        # treeIndex is now a model index corresponding to a top-level item
        
        self.tableModel = QStandardItemModel()
        
        for row in range(self.model.rowCount(treeIndex)):
            items = []
            for column in range(self.model.columnCount(treeIndex)):
                text = self.model.index(row, column, treeIndex).data().toString()
                items.append(QStandardItem(text))
            
            self.tableModel.appendRow(items)
        
        self.tableView.setModel(self.tableModel)


if __name__ == "__main__":

    app = QApplication(sys.argv)
    window = Window()
    window.show()
    sys.exit(app.exec_())
