Grammar_MetaClass
=================

.. _grammar_metaclass:

.. py:class:: Grammar_MetaClass(type)

   Metaclass for generating on-the-fly ANTLR 4 grammar-specific listener functions to parse model files ( \*.rml | \*.rhw | \*.rdp )

   .. py:method:: __new__(name, bases, attrs)

      Intercept ROSMOD Object creation and add attributes to the objects based on model parsing

      :param name: Name of the to-be-created class
      :param bases: List of all base classes of the to-be-created class
      :param attrs: Dictionary of all attributes in the to-be-created class



