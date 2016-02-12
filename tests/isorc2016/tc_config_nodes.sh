#!/bin/bash                                                                                                      

TC=/sbin/tc

DEV=eth0

USETBF="true"

if [[ "$1" = "status" ]]
then
    $TC -s qdisc ls dev $DEV
    $TC -s class ls dev $DEV
exit
fi

# clean existing down- and uplink qdiscs, hide errors                                                            
$TC qdisc del dev $DEV root    2> /dev/null > /dev/null
$TC qdisc del dev $DEV ingress 2> /dev/null > /dev/null

echo "Cleared qdiscs"

if [[ "$1" = "stop" ]]
then
    exit
fi

###### uplink                                                                                                    

#$TC qdisc add dev ${DEV} root handle 1: prio
#$TC qdisc add dev ${DEV} parent 1:1 handle 11: netem delay 1ms
#$TC qdisc add dev ${DEV} parent 1:2 handle 12: pfifo
$TC qdisc add dev ${DEV} root handle 2: tbf \
	rate 1Mbit burst 10kb latency 100000ms peakrate 1.01mbit mtu 1540
echo "set up top level qdiscs"

if [[ "$USETBF" = "true" ]]
then
    echo ""
    #$TC qdisc add dev ${DEV} parent 1:1 handle 2: tbf \
    #rate 1Mbit burst 10kb latency 100000ms peakrate 1.01mbit mtu 1540
else
    #$TC qdisc add dev ${DEV} parent 11:1 handle 2: htb
    $TC class add dev ${DEV} parent 2: classid 2:1 htb rate 10Kbit ceil 10Kbit
    $TC qdisc add dev ${DEV} parent 2:1 handle 21: pfifo
fi
echo "set limiting qdiscs up"

