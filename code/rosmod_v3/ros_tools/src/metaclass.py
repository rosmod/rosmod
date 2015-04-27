# ROSMOD Grammar MetaClass
# Author: Pranav Srinivas Kumar
# Date: 2015.04.27

from collections import OrderedDict

# Grammar Field class used to generate grammar field-specific listener functions
class Grammar_Field:
    def __init__(self, kind = "", name = "", entry_point = None, exit_point = None, syntax_checker = None):
        self.kind = kind
        self.name = name
        self.entry_point = entry_point
        self.exit_point = exit_point
        self.syntax_checker = syntax_checker

# Enter a Model class type while parsing
# E.g. Package, Component, Timer etc.
def enterModel(self, ctx, kind):
    print "enterModel:: Kind = " + kind
    print "enterModel:: Parsed Text = " + ctx.getText()
    new_object = type( "ROS_" + kind, (object, Drawable_Object,), { })
    self.active_objects.append(new_object)
    print "Active Objects: " + self.active_objects
    print type(new_object)

# Exit a Model class type while parsing
def exitModel(self, ctx):
    child_object = self.active_objects.pop()
    self.active_objects[-1].add(child_object)

# Enter a Atom type while parsing
# E.g. Name, Value, Period, Unit etc.
def enterAtom(self, ctx, kind):
    print "enterPropFunc:: Kind = " + kind
    print "enterPropFunc:: Parsed Text = " + ctx.getText()
    self.active_objects[-1].properties[kind] = ctx.getText()

# Exit a Atom type while parsing
def exitAtom(self, ctx):
    pass

# Setup a Dictionary of recognized & parsed grammar fields
# Use this dictionary to generate the listener functions per field inside the builder classes
meta_class_dict = OrderedDict()
meta_class_dict["Package"] = Grammar_Field("string", "Package", enterModel, exitModel)
meta_class_dict["name"] = Grammar_Field("string", "Name", enterAtom, exitAtom)
meta_class_dict["value"] = Grammar_Field("string", "Value", enterAtom, exitAtom)
meta_class_dict["unit"] = Grammar_Field("string", "Unit", enterAtom, exitAtom)
meta_class_dict["datatype"] = Grammar_Field("string", "Datatype", enterAtom, exitAtom)
meta_class_dict["Component"] = Grammar_Field("string", "Component", enterModel, exitModel)
meta_class_dict["Timer"] = Grammar_Field("string", "Timer", enterModel, exitModel)
meta_class_dict["period"] = Grammar_Field("string", "Period", enterAtom, exitAtom)

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
