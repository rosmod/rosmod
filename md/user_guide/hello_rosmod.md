# Hello ROSMOD!
===============

This document gives a step-by-step guide to build a "Hello world" application with ROSMOD editor v3 using ROSML (for more information about ROSML Component model, please see [this](../ComponentModelOverview.md) document). This application will consist of two ROS nodes - a Publisher node and a Subscriber node. The Publisher node consists of a ROS Publisher and a ROS Timer, whereas the Subscriber node consists of a ROS Subscriber. ROS Timer is used to periodically generate events, upon which the Publisher node uses ROS Publisher to publish message which the Subscriber node i.e. ROS Subscriber eventually will receive and print on the console.

## Prerequisites
----------------

This tutorial is designed in such a way that there is no need to have any extra hardware setup to run the software which will be developed. Development of this application (with the help of ROSMOD editor), running an instance of 'roscore' and running instances of 'ROSML nodes' (Publisher and Subscriber) will be carried out on one single Linux system (for more information about ROS, roscore, ROS packages, ROS Publisher, ROS Subscriber and related concepts, please visit [ROS tutorials](http://wiki.ros.org/ROS/Tutorials)).

Before starting with developing the application, please check the list of prerequisite softwares which have to be installed on the Linux system -

  1. **ROSMOD editor** - To setup ROSMOD editor, some supporting packages are needed to be installed, the information about which can be found [here](../../code/rosmod_v3/README.md).
  
  2. **ROS** - As said earlier, the system needs to run an instance of 'roscore' and instances of ROSML nodes as well, all of which will require ROS installed on the system. This tutorial is designed in such a way that, the two ROSML nodes (Publisher and Subscriber) and 'roscore' are run on the same machine which is running ROSMOD editor. This is done for keeping the tutorial simple. Of course, there are no constraints on these three components so as to which systems they should run on; they can run on three different systems which have ROS setup and will still work the same way.
  
  3. **openssh-server** - This is required in order to enable ROSMOD editor to open an ssh session with the system on which you want to deploy the executables of ROSML nodes (Publisher and Subscriber) on the system. The systems (in our tutorial, the system running ROSMOD editor) which you want to deploy the executables on are required to have this package installed.
  
  You can install openssh-server using following command on a Debian system -
  ```bash
  $ sudo apt-get install openssh-server
  ```
  
  4. **authenticated_keys in your ~/.ssh directory** - For deploying the ROSML nodes on host systems, ROSML editor needs to remotely login on those systems using ssh. After setting up the ssh session, the ROSML editor will execute i.e. deploy the respective ROSML nodes on the respective target hosts and then use dtach to detach the process from ssh session(s). For establishing this ssh connection, ROSML editor needs to have an ssh key per target host which will be used to access that target host system for spawning the ROSML node(s) on it.
  The `authenticated_keys` file is a file in your `/home/<username>/.ssh` directory, which holds ssh keys which are authenticated to use to log into the system. We first need to generate a public-private key pair per target host system, and put the public key into `authenticated_keys` files.
  To generate the public-private key pair, we use following set of commands -
  ```bash
  $ mkdir ~/.ssh/ROSMOD_keys && cd ~/.ssh/ROSMOD_keys
  $ ssh-keygen
  ```
  You can use any tool like `ssh-keygen` to generate an ssh key pair. If you are using `ssh-keygen`, you can specify appropriate path of a directory to generate keys into, such as `/home/<username>/.ssh/ROSMOD_keys/id_rsa` (because using the default path could result into overwriting the existing keys in `/home/<username>/.ssh`).
  After the keys have been generated, create a text file named `authenticated_keys` in `/home/<username>/.ssh` and copy the content of public key which has just been created into it. The public key is the file with .pub extension.
  
  5. **dtach** - dtach is a program which is used to emulate the detach feature of the screen, as given [here](http://dtach.sourceforge.net/). This is the tool used by ROSMOD editor for deploying the ROSML node executables on the target host systems (again, in our tutorial, the system running ROSMOD editor) and detaching the process from the ssh-session.
  
  You can install dtach using following command on a Debian system -
  ```bash
  $ sudo apt-get install dtach
  ```

## Developing The Application
-----------------------------

We are going to use ROSMOD editor v3 for developing our first "Hello ROSMOD" application. To get more information about how to use ROSML editor, please refer to [this document](../../code/rosmod_v2/editor_v2/README.md) (__TODO__ - change the reference). To develop the application, please follow the given instructions -

  1. Open ROSMOD editor v3.
  
  2. Create a new project by clicking on **"New" icon on the taskbar**. Alternatively, you can create a new project by clicking on **"File" > "New"** OR pressing hotkey **Ctrl+N**.
  
  While creating the project, first browse the location on your hard-disk where you want to setup your project. Then after clicking "Open", give the name of the project as `hello_rosmod`, give the name of the workspace as `my_ws`, give the name of the hardware profile as `my_hw` and give the name of the deployment profile as `my_dp`.
  
  After completing these steps, you will see the editor with an empty package named "New_Package" and an empty workspace "my_ws". If you save the project now by clicking on **"File" > "Save"** OR pressing hotkey **Ctrl+S**, you will see the messages in Console area saying your project is saved in three files viz. hello_rosmod_ws.rml, hello_rosmod_hw.rhw and hello_rosmod_dp.rdp; which represent the three aspects of your ROSML model, namely **Software**, **Hardware** and **Development** respectively. You can browse to these files in your project location, however avoid editing them 'by hand' as they are completely generated by ROSML editor's generator tools.
  
  3. Select the "New Package" tab, right-click on empty pane, select "Edit" and rename the package as `my_package`. This **ROSML package** will serve as a collection of definitions of ROSML Services, ROSML Messages and ROSML Components. For our application, we need only one package which will contain definitions of a ROSML Component for the Publisher node and another ROSML Component for the Subscriber node.
  
  4. In our application, ROSML Publisher Node will periodically communicate with our ROSML Subscriber Node by using **ROS msg object**. These message objects are abstract entities which are defined in files with extension .msg according to ROS specification (for additional info, go to [ROS Messages](http://wiki.ros.org/ROS/Tutorials/CreatingMsgAndSrv)). These abstract entities act as blueprints for communication objects through which communication happens between a Publisher and Subscriber. In ROSML model, a ROSML package can contain a message object, which directly correspond to a ROS message.
  
  Now, for defining a **msg object** in our package, right-click on empty pane of "my_package", click on "Add Message". Give the name of the message as `my_msg` and specify the field of the message as `int32 my_data`. After clicking "OK", the message object will be created and can be seen under "my_package".
  
  5. Now, we would like to have an entity which will constitute a ROS timer and ROS Publisher. To encapsulate these two, we would need a ROSML Component in our package. The ROSML Components are collection of input ports (Subscribers and Servers), output ports (Publishers and Clients), timers and message queues. The ROSML Components are basic building blocks of ROSML model. [This document](../ComponentModelOverview.md) can give an overview of ROSML objects.
  
  For defining a **ROSML Component**, right-click on empty pane of "my_package", click on "Add Component". Specify "name" as `my_pub_comp` and "Component type" as `Base` (for more information about type of Components, please refer to [this document]()(__TODO__)). After clicking "OK", the ROSML Component will be created and can be seen under "my_package".
  
  6. ROSML Component "my_pub_comp" should contain a ROS Publisher and a ROS Timer. To add a Publisher port into "my_pub_comp", right-click on it and click on "Add Publisher". Specify "name" as `my_pub`, select the created message object's name in "Message" dropdown, i.e. "my_msg"; saying that this Publisher port will be publishing messages of this message type. After clicking "OK", the ROSML Publisher port of "my_pub_comp" Component will be created.
  
  Similarly, to add a Timer into "my_pub_comp", right-click on it and click on "Add Timer". Specify "name" as `my_timer`, "period" as `1.0` (which is in seconds by default). After clicking "OK", the ROSML Timer of "my_pub_comp" Component will be created.
  
  7. Now, we would like to have an entity which will constitute a ROS Subscriber. To implement this, we would need another ROSML Component under our package.
  
  For defining another **ROSML Component**, right-click on empty pane of "my_package", click on "Add Component". Specify "name" as `my_sub_comp` and "Component type" as `Base`.
  
  To add a ROS Subscriber into this recently created ROSML Component, right-click on it and click on "Add Subscriber". Specify "name" as `my_sub`, select the created message object's name in "Message" dropdown, i.e. "my_msg"; saying that this Publisher port will be publishing messages of this message type. After clicking "OK", ROSML Subscriber port for "my_sub_comp" Component will be created.
  
  8. After defining the ROSML Components which will act as building blocks of ROSML nodes, we need to specify the hardware platform on which the ROSML nodes will run. ROSML nodes can be thought of as processes running on these hardware platforms, and ROSML Components can be thought of as multiple threads of these processes. Multiple ROSML Components have input-output ports to communicate with each other, timers to generate periodic events and message queues to hold incoming messages to process them with appropriate policy (FIFO, pFIFO or EDF).
  
  In our "Hello world" application, there will be 2 ROSML nodes - a Publisher node and a Subscriber node. The Publisher node will contain a Publisher Component and the Subscriber node will contain a Subscriber Component. A ROSML node can contain more than one ROSML Component in it (which can be thought of having a multithreaded process). These two ROSML nodes will be running on the same machine, which is running ROSML editor. This machine will also run an instance of `roscore`, to support these two ROSML nodes. Thus, we need to specify only one hardware in our **Hardware** aspect - one which will correspond to the system.
  
  For creating a hardware in "Hardware" aspect in the model, go to **"Aspects" > "Hardware"**. Right-click on the empty pane of "my_hw" and click on "Add Hardware". Specify the "Name" of the hardware as `my_hw_1`, "IP Address" as `localhost` and "Username" as your username on the system. In "SSH Key", give global path of the private key which you created on the target host system (which implies that you must have it on the system on which you are running ROSML editor). For this application, our target host system and the system running ROSML editor are the same, thus it is not required for you to copy the private key again. Give the path of the private key i.e. `/home/<username>/.ssh/ROSMOD_keys/id_rsa` as "SSH Key". Give the path on the target host system where you want to deploy the ROSML node as "Deployment Path" (__TODO__).
  
  9. After creating the essential ROSML Components and Hardware profiles describing the hardware systems in your network, we now have to describe the relation between the two - i.e. which ROSML Components will form a ROSML Nodes and on which hardware platform they will run on. We can specify these relationships in **Deployment** aspect of our model.
  
  For creating a deployment in "Deployment" aspect in the model, go to **"Aspects" > "Deployment"**. We will create 2 nodes in this deployment, both runnign on the hardware which we specified in "my_hw" in earlier step.
  
  For creating a ROSML node, right-click on the empty pane of "my_dep" and click on "Add Node". Specify the "Name" as `pub_node`, select the "Hardware" as the hardware system (host) which you want this node to run on i.e. in our application "my_hw_1" and click "OK". Now, for adding a ROSML Component instance in the newly created ROSML node, right-click on the node and click on "Add Component Instance". Specify the "Name" as `my_pub_comp_inst_1`, select "Component" as "my_pub_comp" and select "Scheduling Scheme" as `FIFO` (for more information about type of Components, please refer to [this document]()(__TODO__)) and tick on `Log level - INFO` logging level from 5 different logging levels, which have descending order of priority from top to bottom (for more information about logging levels, please refer to [this document]()(__TODO__)).
  
  For creating another ROSML node, follow the same steps as above to include a component instance of "my_sub_comp" named `my_sub_comp_inst_1` and other relative information.
  
  10. After completing all the above steps, save the model by clicking on **"File" > "Save"** or by using hotkey **Ctrl+S**. By doing this, all the model we have developed so far will be saved in respective .rml, .rhw and .rdp files.
  
### Generating ROS Workspace From ROSML Model

  The steps done so far will create files (.rml file for describing "Software" aspect, .rhw file for describing "Hardware" aspect and .rdp file for describing "Deployment" aspect) which describe ROSML model for your "Hello world" application. These files can be used to generate ROS Workspace which can be further used to generate binary executables for your target host system. For more information on the ROS Workspace, please refer to [this document]().
  
  For the ROS Workspace to be generated, go to **"Aspects" > "Software"**, and click on "Generate ROS Workspace" button on the toolbar or use hotkey **Ctrl+G**. After this, a workspace according to ROS specification will be generated, which will have .msg files, .srv files, .cpp files per component and .hpp files for the includes placed in appropriate places.
  
  After the ROS Workspace is generated, the ROSML project directory will look like following -
  ```bash
	hello_rosmod
	├── 01-Software
	│   ├── abl
	│   ├── msg
	│   ├── my_ws
	│   │   └── src
	│   │       ├── my_package
	│   │       │   ├── CMakeLists.txt
	│   │       │   ├── include
	│   │       │   │   └── my_package
	│   │       │   │       ├── Component.hpp
	│   │       │   │       ├── Logger.hpp
	│   │       │   │       ├── my_pub_comp.hpp
	│   │       │   │       ├── my_sub_comp.hpp
	│   │       │   │       ├── rapidxml.hpp
	│   │       │   │       ├── rapidxml_utils.hpp
	│   │       │   │       └── xmlParser.hpp
	│   │       │   ├── msg
	│   │       │   │   └── my_msg.msg
	│   │       │   ├── package.xml
	│   │       │   └── src
	│   │       │       └── my_package
	│   │       │           ├── Component.cpp
	│   │       │           ├── Logger.cpp
	│   │       │           ├── my_pub_comp.cpp
	│   │       │           └── my_sub_comp.cpp
	│   │       └── node
	│   │           ├── CMakeLists.txt
	│   │           ├── include
	│   │           │   └── node
	│   │           │       ├── Component.hpp
	│   │           │       ├── Logger.hpp
	│   │           │       ├── rapidxml.hpp
	│   │           │       ├── rapidxml_utils.hpp
	│   │           │       └── xmlParser.hpp
	│   │           ├── package.xml
	│   │           └── src
	│   │               └── node
	│   │                   ├── Component.cpp
	│   │                   ├── Logger.cpp
	│   │                   └── node_main.cpp
	│   ├── my_ws.rml
	│   ├── pnp
	│   └── srv
	├── 02-Hardware
	│   ├── my_hw.rhw
	│   └── snp
	├── 03-Deployment
	│   └── my_dep.rdp
	└── hello_rosmod.rosmod
  ```
  
  For more information about a ROSML workspace, please refer to [this document]()(__TODO__).
  
### Writing Business Logic

  After creating a skeleton for our ROSML application, we would need to write some code which will define the business logic of our application. The business logic for publishing messages will go into the .cpp file which is generated for our ROSML Publisher Component i.e. `my_pub_comp.cpp` and the business logic for receiving messages will go into the .cpp file which is generated for our ROSML Subscriber Component i.e. `my_sub_comp.cpp`.
  
  Inside `my_pub_comp.cpp` (which can be found at `<ROSML workspace>/01-Software/my_ws/src/my_package/src/my_package`), there are various functions defined, out of which one is callback of ROSML Timer `my_timer`. Whenever the timer expires, this function will get called with appropriate arguments passed to it in `event` parameter. The function definition looks as follows -
  ```cpp
void my_pub_comp::my_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for my_timer Timer
}
  ```
  
  Inside this timer callback function, we are going to write business logic code which will send message with the help of ROS Publisher object. To achieve this, change this function's definition as follows -
  ```cpp
void my_pub_comp::my_timerCallback(const ros::TimerEvent& event)
{
  // Business Logic for my_timer Timer
  my_package::my_msg msg_obj;

  msg_obj.my_data = counter;
  counter++;

  my_pub.publish(msg_obj);
}
  ```
  As it would be quite clear from code, the first line of the code declares a object which holds a message and which is of the type `my_msg`. This object will have a field inside it called `my_data`, as we declared it in our "Software" aspect. This field is getting assigned with a variable called `counter`, which is getting incremented in the next line of the code. The last line of the code represents actual act of publishing i.e. sending the message object on the network, so that the appropriate subscribers will receive the message object.
  
  For this to work we also need to define a global variable named `counter` in global area. In `my_pub_comp.cpp`, the global variables should be defined inside the area marked with markers (used for code preservation). The file contains markers as follows -
  ```cpp
//# Start User Globals Marker
//# End User Globals Marker
  ```
  Change it to contain a global variable, as follows -
  ```cpp
//# Start User Globals Marker
int counter = 0;
//# End User Globals Marker
  ```
  
  With these two changes, business logic for publishing our messages is in place.
  
  Now, inside `my_sub_comp.cpp` (which can be found at `<ROSML workspace>/01-Software/my_ws/src/my_package/src/my_package`), there are various functions defined, similar to that of `my_pub_comp.cpp`, out of which one is callback of ROSML Subscriber `my_sub`. Whenever the Subscriber gets a message published, this function is called with appropriate message object passed in as the argument. The function definition looks like follows -
  ```cpp
void my_sub_comp::my_sub_OnOneData(const my_package::my_msg::ConstPtr& received_data)
{
  // Business Logic for my_sub Subscriber
}
  ```
  
  Inside this Subscriber callback function, we are going to write business logic code which will receive the message sent by a Publisher and will print the content on the standard output i.e. console. For doing this, change this function's definition as follows -
  ```cpp
void my_sub_comp::my_sub_OnOneData(const my_package::my_msg::ConstPtr& received_data)
{
  // Business Logic for my_sub Subscriber
  ROS_INFO("I heard :: [%d]", (int)received_data->my_data);
}
  ```
  The line added inside the Subscriber callback function is quite self-explanatory - the data which is sent by a Publisher Component is captured by `my_msg` type of object, whose pointer is passed as the argument to this function's call. This object has a field inside it called `my_data` which contains our actual data. The value of this field is printed on the console using `ROS_INFO` function.
  
  With this change, business logic for receiving published messages is in place.

## Building The Application
---------------------------


## Deploying The Application
----------------------------