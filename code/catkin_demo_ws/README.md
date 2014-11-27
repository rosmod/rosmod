ROS Orbiter Demo:
-----------------


	sudo sendip -p ipv4 -is 127.0.0.1 -p udp -us 5070 -ud 7777 -d "KILL ./devel/lib/satellite_flight_application/GndActor" -v 127.0.0.1

	sudo sendip -p ipv4 -is 127.0.0.1 -p udp -us 5070 -ud 7777 -d "START ./devel/lib/satellite_flight_application/GndActor Beta 1" -v 127.0.0.1

	sudo sendip -p ipv4 -is 127.0.0.1 -p udp -us 5070 -ud 7777 -d "SHUTDOWN" -v 127.0.0.1