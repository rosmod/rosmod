ROS Orbiter Demo:
-----------------

I have updated the RMUb0 VM to have ROS (only ros-comm, following http://wiki.ros.org/indigo/Installation/Ubuntu ) installed and have the ros demo binaries copied over into ~/demo/{satellite_flight_application,cluster_flight_application,wam_application}/

There is a python program in the demo folder, nodeActorLauncher.py, which you can use to manage the execution of the actors in the demo.  Either you can pass the actors to the program as command line parameters and it will run them for the specified duration (default=10 seconds) before killing them, or you can tell it to listen on a UDP socket (default is 7777).

Additionally, there is a python program, cleanup.py, which you can use to kill any actor processes left behind if the nodeActorLauncher crashes (or if you just want to kill processes for fun...).

The steps required to run the demo on multiple VMs are:

    0. Make copies of the main VM
    1. Ensure you have host-only networking between your VMs and they can all ping each other
    2. Start roscore on one of the vm's

       $ roscore

    3. on every VM that you want to run actors:

       $ export ROS_MASTER_URI=http://<roscore VM ip address>:11311
       $ export ROS_IP=<this VM address>
       $ ~/demo/nodeActorLauncher.py -l

    4. Then from another VM or another terminal, you can use sendip (described below) to start/kill actors on each node.

The command line options for the nodeActorLauncher are

    Usage:
	 ./nodeActorLauncher.py
        		-l (to (l)isten indefinitely on a socket)
        		-A <(A)ctor executable with path and cmd line arguments>
        		-D <(D)uration to run cli-provided actors>
        		-L <program (L)og filename>
        		-r (to (r)edirect program output to log file)
        		-?, -h, --help (to display this help information)

For example, if you wanted to give it a list of arguments you might

    ./nodeActorLauncher.py -A ./satellite_flight_application/GroundInterface_a Beta 1 -A ./cluster_flight_application/TargetOrbitActor

If you tell it to listen on a socket, it will run indefinitely until you send the SHUTDOWN command.  The avialable commands are:

       START <actor executable> <actor command line arguments>
       KILL <actor executable>
       SHUTDOWN

SendIP is a program that is installed on the VM which allows you to send IP packets from the command line like so:

	sudo sendip -p ipv4 -is <your VM's IP> -p udp -us <some port your choose> -ud 7777 -d "<command for the nodeActorLauncher>" -v <destination VM IP>

Example commands:

	sudo sendip -p ipv4 -is 192.168.1.74 -p udp -us 5070 -ud 7777 -d "KILL ./satellite_flight_application/GroundInterface_a" -v 192.168.1.77
	sudo sendip -p ipv4 -is 192.168.1.74 -p udp -us 5070 -ud 7777 -d "START ./satellite_flight_application/GroundInterface_a Beta 1" -v 192.168.1.77
	sudo sendip -p ipv4 -is 192.168.1.74 -p udp -us 5070 -ud 7777 -d "SHUTDOWN" -v 192.168.1.77

No actors require command line arguments except for the satellite_flight_application actors which have the following signatures:

   	  GroundInterface_a <satellite name> <0,1 to disable/enable publishing>
	  SatelliteBusInterface_a <satellite name>
	  OrbitController_a <satellite name>

Each of these actors requires the satellite name at minimum because they are the actors which are duplicated accross nodes, so they must create a globally unique actor name using their application actor name + their node name.  