ROSMOD_Loader
=============

.. py:class:: ROSMOD_Loader

   Loads an existing ROS Workspace for Code Preservation

   .. py:method:: load(workspace, path)

      Load an existing ROS workspace and preserve code blocks between code-preservation markers so that regeneration does not completely overwrite existing work

      :param workspace: The ROS_Workspace object to traverse
      :param str path: The absolute path to the ROS workspace directory
