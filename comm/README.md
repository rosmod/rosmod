## ROSMOD Communication Layer

* The ROSMOD Communication Layer is a modified version of the ros_comm communication layer in ROS. 
* ROSMOD introduces PFIFO and EDF-based scheduling schemes to the ROS callback queue.
* ROSMOD also facilitates deadline monitoring i.e. deadline violation detection and logging for all callbacks.

```bash
$ catkin_make -DCMAKE_INSTALL_PREFIX=<ROS_INSTALL_LOCATION> install
```

e.g. 

```bash
$ catkin_make -DCMAKE_INSTALL_PREFIX=/opt/ros/indigo install
```
