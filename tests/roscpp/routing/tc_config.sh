#!/bin/bash

TC=/sbin/tc

DEV=eth0

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

$TC qdisc add dev ${DEV} root handle 1: netem delay 100ms
$TC qdisc add dev ${DEV} parent 1:1 handle 11: tbf rate 1Mbit peakrate 1001kbit mtu 8192 latency 100s burst 154000000
$TC qdisc add dev ${DEV} parent 11:1 handle 111: prio
$TC qdisc add dev ${DEV} parent 111:1 handle 1111: pfifo
$TC qdisc add dev ${DEV} parent 111:2 handle 1112: pfifo
$TC qdisc add dev ${DEV} parent 111:3 handle 1113: pfifo
$TC filter add dev ${DEV} protocol ip parent 111: prio 1 u32 match ip src 10.1.1.1 flowid 111:1
$TC filter add dev ${DEV} protocol ip parent 111: prio 1 u32 match ip src 10.1.1.3 flowid 111:2
$TC filter add dev ${DEV} protocol ip parent 111: prio 2 u32 match ip src 192.168.122.0/24 flowid 111:3
