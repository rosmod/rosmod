#!/bin/bash

TC=/sbin/tc

DEV=eth0

USETBF="false"

if [[ "$1" = "status" ]]
then
    $TC -s qdisc ls dev $DEV
    $TC -s class ls dev $DEV
exit
fi

# clean existing down- and uplink qdiscs, hide errors
$TC qdisc del dev $DEV root    2> /dev/null > /dev/null
$TC qdisc del dev $DEV ingress 2> /dev/null > /dev/null

if [[ "$1" = "stop" ]]
then
    exit
fi

###### uplink

$TC qdisc add dev ${DEV} root handle 1: prio

$TC qdisc add dev ${DEV} parent 1:1 handle 11: netem delay 100ms
$TC qdisc add dev ${DEV} parent 1:2 handle 12: pfifo

if [[ "$USETBF" = "true" ]]
then
    $TC qdisc add dev ${DEV} parent 11:1 handle 2: tbf \
	rate 1Mbit burst 10kb latency 100000ms peakrate 1.01mbit mtu 1540

    $TC qdisc add dev ${DEV} parent 2: handle 21: prio

    $TC qdisc add dev ${DEV} parent 21:1 handle 211: pfifo
    $TC qdisc add dev ${DEV} parent 21:2 handle 212: pfifo
else
    $TC qdisc add dev ${DEV} parent 11:1  handle 2: htb
    
    $TC class add dev ${DEV} parent 2: classid 2:1 htb rate 10Mbit ceil 10Mbit
    $TC class add dev ${DEV} parent 2:1 classid 2:10 htb rate 10Mbit ceil 10Mbit prio 0
    $TC class add dev ${DEV} parent 2:1 classid 2:20 htb rate 10bit ceil 10Mbit prio 1
    
    $TC qdisc add dev ${DEV} parent 2:10 handle 21: pfifo
    $TC qdisc add dev ${DEV} parent 2:20 handle 22: pfifo
fi
echo "set qdiscs up"

# FILTER APPLICATION TRAFFIC VERSUS NON APP TRAFIC
$TC filter add dev ${DEV} protocol ip parent 1: prio 1 u32 \
    match ip src 10.1.1.1 flowid 1:1
$TC filter add dev ${DEV} protocol ip parent 1: prio 1 u32 \
    match ip src 10.1.1.3 flowid 1:1
echo "set priority filters up"

$TC filter add dev ${DEV} protocol ip parent 1: prio 2 u32 \
    match ip src 10.1.1.0/24 flowid 1:2
$TC filter add dev ${DEV} protocol ip parent 1: prio 2 u32 \
    match ip src 192.168.122.0/24 flowid 1:2
echo "set other filters up"

# PRIORITIZE CERTAIN APPLICATION TRAFFIC
if [[ "$USETBF" = "true" ]]
then
    $TC filter add dev ${DEV} protocol ip parent 21: prio 1 u32 \
	match ip src 10.1.1.1 flowid 21:1

    $TC filter add dev ${DEV} protocol ip parent 21: prio 2 u32 \
	match ip src 10.1.1.3 flowid 21:2
else
    $TC filter add dev ${DEV} protocol ip parent 2: prio 1 u32 \
	match ip src 10.1.1.1 flowid 2:10

    $TC filter add dev ${DEV} protocol ip parent 2: prio 2 u32 \
	match ip src 10.1.1.3 flowid 2:20
fi
echo "set priority filters up"
