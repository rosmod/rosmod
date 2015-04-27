#!/usr/bin/python

class propDef:
    def __init__(self, 
                 kind = "",
                 grammarName = "",
                 grammarEnterFunc = None,
                 grammarExitFunc = None,
                 inputValidator = None 
    ):
        self.kind = kind
        self.grammarName = grammarName
        self.grammarEnterFunc = grammarEnterFunc
        self.grammarExitFunc = grammarExitFunc
        self.intputValidator = inputValidator



def makeEnterProp(kind):
    def enterPropFunc(self,ctx):
        print "PROP::ENTER::" + ctx
        print kind
    return enterPropFunc

def makeExitProp():
    def exitPropFunc(self,ctx):
        print "PROP::EXIT::" + ctx
    return exitPropFunc

def makeEnterContainer(kind):
    def enterContainerFunc(self,ctx):
        print "CONTAINER::ENTER::" + ctx
        print kind
    return enterContainerFunc

def makeExitContainer():
    def exitContainerFunc(self,ctx):
        print "CONTAINER::EXIT::" + ctx
    return exitContainerFunc

objDefDict = {}
objDefDict['name'] = propDef("string","Name",makeEnterProp,makeExitProp)
objDefDict['type'] = propDef("string","Type",makeEnterProp,makeExitProp)
objDefDict['value'] = propDef("string","Value",makeEnterProp,makeExitProp)
objDefDict['package'] = propDef("obj","Package",makeEnterContainer,makeExitContainer)
objDefDict['message'] = propDef("obj","Message",makeEnterContainer,makeExitContainer)
objDefDict['field'] = propDef("obj","Field",makeEnterContainer,makeExitContainer)

class myMetaClass(type):
    def __new__(cls, name, bases, attrs):
        if name.startswith('None'):
            return None
        for name,value in objDefDict.iteritems():
            attrs['enter'+value.grammarName] = value.grammarEnterFunc(name)
            attrs['exit'+value.grammarName] = value.grammarExitFunc()
        return super(myMetaClass,cls).__new__(cls,name,bases,attrs)
    def __init__(self, name, bases, attrs):
        super(myMetaClass, self).__init__(name, bases, attrs)

class myObj:

    __metaclass__ = myMetaClass
    def __init__(self):
        self.activeObjectList = []

testObj = myObj()
testObj.enterName("prop")
testObj.exitName("prop")
testObj.enterMessage("container")
testObj.exitMessage("container")
testObj.enterField(ctx="HELLO WORLD")
testObj.exitField(ctx="GOODBYE WORLD")
