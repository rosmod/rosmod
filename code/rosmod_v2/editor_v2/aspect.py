from collections import OrderedDict

class TBInfo():
    def __init__(self,name, obj):
        self.name = name
        self.obj = obj

class AspectPageInfo():
    def __init__(self, name, obj, canvas, msgWindow):
        self.name = name
        self.obj = obj
        self.canvas = canvas
        self.msgWindow = msgWindow

class AspectInfo():
    def __init__(self):
        self.pages = OrderedDict()
        self.toolbarButtons = OrderedDict()

    def GetTBInfo(self,name):
        if name in self.toolbarButtons.keys():
            return self.toolbarButtons[name]
        else:
            return None
    def AddTBInfo(self,tbInfo):
        self.toolbarButtons[tbInfo.name] = tbInfo
    def DelTBInfo(self,name):
        self.toolbarButtons.pop(name,None)

    def GetPageInfo(self,name):
        if name in self.pages.keys():
            return self.pages[name]
        else:
            return None
    def AddPageInfo(self,pageInfo):
        self.pages[pageInfo.name] = pageInfo
    def DelPageInfo(self,name):
        self.pages.pop(name,None)

