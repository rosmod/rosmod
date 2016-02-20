#!/bin/bash

TC=/sbin/tc

DEV=eth0

USETBF="true"

BW=60

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

if [[ "$1" ]]
then
    BW=$1
fi

###### uplink

$TC qdisc add dev ${DEV} root handle 1: prio

if [[ "$USETBF" = "true" ]]
then
    $TC qdisc add dev ${DEV} parent 1:1 handle 2: tbf \
	rate ${BW}Kbit burst 10kb latency 100000ms peakrate 71Kbit mtu 1540
else
    $TC qdisc add dev ${DEV} parent 1:1 handle 2: htb # 11: netem delay 100ms
    $TC class add dev ${DEV} parent 2: classid 2:1 htb rate ${BW}Kbit ceil ${BW}Kbit burst 100Kbit cburst 10Kbit
    $TC qdisc add dev ${DEV} parent 2:1 handle 21: pfifo
fi

$TC qdisc add dev ${DEV} parent 1:2 handle 12: pfifo

echo "set qdiscs up"

# FILTER APPLICATION TRAFFIC VERSUS NON APP TRAFIC
$TC filter add dev ${DEV} protocol ip parent 1: prio 1 u32 \
    match ip dst 10.1.1.0/24 flowid 1:1

if [[ "$USETBF" = "true" ]]
then
    echo ""
else
    $TC filter add dev ${DEV} protocol ip parent 2: prio 1 u32 \
	match ip dst 10.1.1.0/24 flowid 2:1
fi

echo "set priority filters up"

$TC filter add dev ${DEV} protocol ip parent 1: prio 2 u32 \
    match ip src 192.168.122.0/24 flowid 1:2
#$TC filter add dev ${DEV} protocol ip parent 1: prio 2 u32 \
#    match ip src 10.1.1.0/24 flowid 1:2
echo "set other filters up"
