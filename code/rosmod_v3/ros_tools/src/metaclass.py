# ROSMOD Grammar MetaClass
# Author: Pranav Srinivas Kumar
# Date: 2015.04.27

from collections import OrderedDict

class Grammar_Property:
    def __init__(self, kind = "", name = "", entry_point = None, exit_point = None, checker = None):
        self.kind = kind
        self.name = name
        self.entry_point = entry_point
        self.exit_point = exit_point
        self.checker = checker

def enterContainerFunc(self, ctx, kind):
    print "enterContainerFunc:: Kind = " + kind
    print "enterContainerFunc:: Parsed Text = " + ctx.getText()
    new_object = type( "ROS_" + kind, (object, Drawable_Object,), { })
    self.active_objects.append(new_object)
    print "Active Objects: " + self.active_objects
    print type(new_object)

def exitContainerFunc(self, ctx):
    pass
    child_object = self.active_objects.pop()
    self.active_objects[-1].add(child_object)

def enterPropFunc(self, ctx, kind):
    print "enterPropFunc:: Kind = " + kind
    print "enterPropFunc:: Parsed Text = " + ctx.getText()
    self.active_objects[-1].properties[kind] = ctx.getText()

def exitPropFunc(self, ctx):
    pass

meta_class_dict = OrderedDict()
meta_class_dict["Package"] = Grammar_Property("string", "Package", enterContainerFunc, exitContainerFunc)
meta_class_dict["name"] = Grammar_Property("string", "Name", enterPropFunc, exitPropFunc)
meta_class_dict["value"] = Grammar_Property("string", "Value", enterPropFunc, exitPropFunc)
meta_class_dict["unit"] = Grammar_Property("string", "Unit", enterPropFunc, exitPropFunc)
meta_class_dict["datatype"] = Grammar_Property("string", "Datatype", enterPropFunc, exitPropFunc)
meta_class_dict["Component"] = Grammar_Property("string", "Component", enterContainerFunc, exitContainerFunc)
meta_class_dict["Timer"] = Grammar_Property("string", "Timer", enterContainerFunc, exitContainerFunc)
meta_class_dict["period"] = Grammar_Property("string", "Period", enterPropFunc, exitPropFunc)

class Grammar_MetaClass(type):
    # Create new class
    def __new__(cls, name, bases, attrs):
        if name.startswith('None'):
            return None
        for key, value in meta_class_dict.iteritems():
            attrs['enter' + value.name] = lambda s, ctx: value.entry_point(s, ctx, key)
            attrs['exit' + value.name] = value.exit_point
        return super(Grammar_MetaClass, cls).__new__(cls, name, bases, attrs)

    # Initialize new class
    def __init__(self, name, bases, attrs):
        super(Grammar_MetaClass, self).__init__(name, bases, attrs)
