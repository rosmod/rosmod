ROSMOD Project
==============

.. py:class:: ROSMOD_Project(Drawable_Object)

   The high-level interface to a ROSMOD Project

   .. py:method:: __init__(**kwargs)

      Initialize an empty ROSMOD Project.

   .. py:method:: new(project_name, project_path, workspace_name, hardware_name, deployment_name, [q=None])

      Create a new ROSMOD Project.

      :param str project_name: The name of the ROSMOD Project
      :param str project_path: The absolute path to the root directory of the ROSMOD Project
      :param str workspace_name: The name of the ROSMOD Software Model and the generated ROS Workspace
      :param str hardware_name: The name of the ROSMOD Hardware Model
      :param str deployment_name: The name of the ROSMOD Deployment Model

				  
   .. py:method:: open(project_path, [progressQ = None])

      Open an existing ROSMOD Project

      :param str project_Path: The absolute path to the root directory of the ROSMOD Project

   .. py:method:: parse_msg(dirname)

      Parse all msg (message) files in the Software Model

      :param str dirname: The path to the msg directory in the ROSMOD Software Model

   .. py:method:: parse_srv(dirname)

      Parse all srv (service) files in the Software Model

      :param str dirname: The path to the srv directory in the ROSMOD Software Model

   .. py:method:: parse_abl(dirname)

      Parse all abl (abstract business logic) files in the Software Model

      :param str dirname: The path to the abl directory in the ROSMOD Software Model

   .. py:method:: parse_pnp(dirname)

      Parse all pnp (port network profiles) files in the Software Model

      :param str dirname: The path to the pnp directory in the ROSMOD Software Model

   .. py:method:: parse_snp(dirname)

      Parse all snp (system network profiles) files in the Hardware Model

      :param str dirname: The path to the snp directory in the ROSMOD Hardware Model

   .. py:method:: parse_rml(filename)

      Parse the provided rml file (ROSMOD Software Model)

      :param str filename: The name of the .rml file 

   .. py:method:: parse_rhw(filename)

      Parse the provided rhw file (ROSMOD Hardware Model)

      :param str filename: The name of the .rhw file 

   .. py:method:: parse_rdp(filename)

      Parse the provided rdp file (ROSMOD Deployment Model)

      :param str filename: The name of the .rdp file

   .. py:method:: parse_models([progressQ=None])

      Parse all Software, Hardware and Deployment models in the ROSMOD Project

   .. py:method:: check_workspace

      Check for an existing ROS Workspace in the ROSMOD Project in order to preserve any code between code-preservation markers

   .. py:method:: generate_workspace

      Generate a ROS Workspace from the Software Model. The workspace is generated at PROJECT_ROOT/01-Software/<workspace_name>

   .. py:method:: generate_xml

      Generate Deployment XML files for all ROS nodes in the Deployment Model

   .. py:method:: generate_cpn

      Generate a Colored Petri Net timing analysis model for the ROSMOD Project

   .. py:method:: resolve_references([progressQ = None])

      Resolve all missing object references after parsing models

   .. py:method:: save_rml([path=""])

      Save a modified workspace (.rml) file 

      :param str path: Optional path where the .rml file will be saved

   .. py:method:: save_rhw([path=""])

      Save a modified workspace (.rhw) file 

      :param str path: Optional path where the .rhw file will be saved

   .. py:method:: save_rdp([path=""])

      Save a modified workspace (.rdp) file 

      :param str path: Optional path where the .rdp file will be saved

   .. py:method:: save_msg(msg_object)

      Save a modified msg object to file 

      :param msg_object: Message Object to be saved

   .. py:method:: save_srv(srv_object)

      Save a modified srv object to file 

      :param srv_object: Service Object to be saved

   .. py:method:: save_abl(port_object)

      Save a modified abstract business logic to file

      :param port_object: Port Object that contains an abstract business logic

   .. py:method:: save_pnp(port_object)

      Save a modified port network profile to file

      :param port_object: Port Object that contains a port network profile

   .. py:method:: save_snp(port_object)

      Save a modified system network profile to file

      :param port_object: Port Object that contains a system network profile

   .. py:method:: save([project_name=""], [project_path=""])

      Used to "Save" or "Save-As" an entire ROSMOD Project

      :param str project_name: Name of ROSMOD Project used for Save-As
      :param str project_path: Absolute path to ROSMOD Project used for Save-As








