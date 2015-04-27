# ROSMOD Grammar MetaClass
# Author: Pranav Srinivas Kumar
# Date: 2015.04.27

from collections import OrderedDict

# Grammar Property class used to generate grammar field-specific listener functions
class Grammar_Property:
    def __init__(self, kind = "", name = "", entry_point = None, exit_point = None, checker = None):
        self.kind = kind
        self.name = name
        self.entry_point = entry_point
        self.exit_point = exit_point
        self.checker = checker

# Enter a Container class type while parsing
# E.g. Package, Component, Timer etc.
def enterContainer(self, ctx, kind):
    print "enterContainerFunc:: Kind = " + kind
    print "enterContainerFunc:: Parsed Text = " + ctx.getText()
    new_object = type( "ROS_" + kind, (object, Drawable_Object,), { })
    self.active_objects.append(new_object)
    print "Active Objects: " + self.active_objects
    print type(new_object)

# Exit a Container class type while parsing
def exitContainer(self, ctx):
    pass
    child_object = self.active_objects.pop()
    self.active_objects[-1].add(child_object)

# Enter a Property type while parsing
# E.g. Name, Value, Period, Unit etc.
def enterProperty(self, ctx, kind):
    print "enterPropFunc:: Kind = " + kind
    print "enterPropFunc:: Parsed Text = " + ctx.getText()
    self.active_objects[-1].properties[kind] = ctx.getText()

# Exit a Property type while parsing
def exitProperty(self, ctx):
    pass

# Setup a Dictionary of Grammar fields that are recognized and visited while parsing input models
# Use this dictionary to generate the listener functions inside the Builder classes
meta_class_dict = OrderedDict()
meta_class_dict["Package"] = Grammar_Property("string", "Package", enterContainer, exitContainer)
meta_class_dict["name"] = Grammar_Property("string", "Name", enterProperty, exitProperty)
meta_class_dict["value"] = Grammar_Property("string", "Value", enterProperty, exitProperty)
meta_class_dict["unit"] = Grammar_Property("string", "Unit", enterProperty, exitProperty)
meta_class_dict["datatype"] = Grammar_Property("string", "Datatype", enterProperty, exitProperty)
meta_class_dict["Component"] = Grammar_Property("string", "Component", enterContainer, exitContainer)
meta_class_dict["Timer"] = Grammar_Property("string", "Timer", enterContainer, exitContainer)
meta_class_dict["period"] = Grammar_Property("string", "Period", enterProperty, exitProperty)

# Grammar Metaclass to generate listener functions as part of the builder classes
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
