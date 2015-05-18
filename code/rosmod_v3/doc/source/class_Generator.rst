ROSMOD_Generator
================

.. py:class:: ROSMOD_Generator

   Primary Generator class for ROSMOD 
   
   .. py:method:: generate_workspace(workspace, path)

      Generate the ROS Workspace and build system from the Software Model

      :param workspace: The ROS_Workspace object to traverse
      :param str path: The absolute path to the to-be-generated workspace directory

   .. py:method:: generate_xml(deployments, deployment_path)

      Generate Deployment-specific ROS Node XML files from the Deployment Model

      :param deployments: The list of all ROS_Deployment objects in 03-Deployment directory
      :param deployment_path: The absolute path to the Deployment directory

   .. py:method:: generate_cpn(workspace, deployments, deployment_path)

      Generate a Colored Petri Net-based Timing Analysis Model for the Project

      :param workspace: The ROS_Workspace object to traverse 
      :param deployments: The list of all ROS_Deployment objects in 03-Deployment directory
      :param deployment_path: The absolute path to the Deployment directory
     
      
