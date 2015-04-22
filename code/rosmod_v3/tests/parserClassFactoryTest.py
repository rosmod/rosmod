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

def enterPropFunc(self,*args,**kwargs):
    print "ENTERING PROP FUNCTION"
    print args
    print kwargs
def exitPropFunc(self,*args,**kwargs):
    print "EXITING PROP FUNCTION"
    print args
    print kwargs
def enterContainerFunc(self,*args,**kwargs):
    print "ENTERING CONTAINER FUNCTION"
    print args
    print kwargs
def exitContainerFunc(self,ctx):
    print "EXITING CONTAINER FUNCTION"
    print ctx

objDefDict = {}
objDefDict['name'] = propDef("string","Name",enterPropFunc,exitPropFunc)
objDefDict['package'] = propDef("string","Package",enterContainerFunc,exitContainerFunc)
objDefDict['message'] = propDef("string","Message",enterContainerFunc,exitContainerFunc)
objDefDict['field'] = propDef("string","Field",enterContainerFunc,exitContainerFunc)
objDefDict['type'] = propDef("string","Type",enterPropFunc,exitPropFunc)
objDefDict['value'] = propDef("string","Value",enterPropFunc,exitPropFunc)

class myMetaClass(type):
    def __new__(cls, name, bases, attrs):
        if name.startswith('None'):
            return None
        for name,value in objDefDict.iteritems():
            attrs['enter'+value.grammarName] = value.grammarEnterFunc
            attrs['exit'+value.grammarName] = value.grammarExitFunc
        return super(myMetaClass,cls).__new__(cls,name,bases,attrs)
    def __init__(self, name, bases, attrs):
        super(myMetaClass, self).__init__(name, bases, attrs)

class myObj:
    __metaclass__ = myMetaClass

print type(myObj), repr(myObj)
testObj = myObj()
testObj.enterName()
testObj.exitName()
testObj.enterMessage("hi pranav")
testObj.exitField(ctx="HELLO WORLD")
