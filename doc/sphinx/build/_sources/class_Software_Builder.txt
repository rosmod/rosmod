ROSMOD_Software_Builder
=======================

.. py:class:: ROSMOD_Software_Builder(ROSMOD_SoftwareListener)

   Builds a Software Object Tree from parsing the .rml file in 01-Software/*

   :ivar __metaclass__: :ref:`Grammar_MetaClass <grammar_metaclass>`

   .. py:method:: __init__(project)

      Initialize the root of the Software Object Tree, a "ROS_RML" class object

      :param project: The ROSMOD_Project object that is the parent of the Software model
