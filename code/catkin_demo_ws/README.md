ROS Orbiter Demo:
-----------------

I have updated the RMUb0 VM to have ROS (only ros-comm) installed and have the ros demo binaries copied over into ~/demo/{satellite_flight_application,cluster_flight_application,wma}/

There is a python program in the demo folder, nodeActorLauncher.py, which you can use to manage the execution of the actors in the demo.  Either you can pass the actors to the program as command line parameters and it will run them for 10 seconds before killing them, or you can tell it to listen on a UDP socket (default is 7777).

The command line options for the nodeActorLauncher are

   Usage:
	python  ./nodeActorLauncher.py 
                		-l (to listen indefinitely on a socket)
                		-N <(N)ode name>
                		-L <program (L)og filename>
                		-r (to redirect program output to log file)
                		-A <actor executable with path and cmd line arguments>

If you tell it to listen on a socket, it will run indefinitely until you send the SHUTDOWN command.  The avialable commands are:

   START <actor executable> <actor command line arguments>
   KILL <actor executable>
   SHUTDOWN

SendIP is a program that is installed on the VM which allows you to send IP packets from the command line like so:

   sudo sendip -p ipv4 -is <your VM's IP> -p udp -us <some port your choose> -ud 7777 -d "<command for the nodeActorLauncher>" -v <destination VM IP>

Example commands:

   sudo sendip -p ipv4 -is 192.168.1.74 -p udp -us 5070 -ud 7777 -d "KILL ./satellite_flight_application/GndActor" -v 192.168.1.77
   sudo sendip -p ipv4 -is 192.168.1.74 -p udp -us 5070 -ud 7777 -d "START ./satellite_flight_application/GndActor Beta 1" -v 192.168.1.77
   sudo sendip -p ipv4 -is 192.168.1.74 -p udp -us 5070 -ud 7777 -d "SHUTDOWN" -v 192.168.1.77

No actors require command line arguments except for the satellite_flight_application actors which have the following signatures:

   GndActor <satellite name> <0,1 to disable/enable publishing>
   BusActor <satellite name>
   OrbitControllerActor <satellite name>

Each of these actors requires the satellite name at minimum because they are the actors which are duplicated accross nodes, so they must create a globally unique actor name using their application actor name + their node name.  