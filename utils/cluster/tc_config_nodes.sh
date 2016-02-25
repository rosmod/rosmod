#!/bin/bash

TC=/sbin/tc
DEV=eth0

USETBF="true"
BW=70000
DELAY=0
BUCKET=100
BUFFER=10000

PRINT_STATUS="false"
CLEAR="false"

for i in "$@"
do
    case $i in
	-p|--print)
	    PRINT_STATUS="true"
	    shift # past argument=value
	    ;;
	--clear)
	    CLEAR="true"
	    shift # past argument=value
	    ;;
	-d=*|--delay=*)
	    DELAY="${i#*=}"
	    shift # past argument=value
	    ;;
	-b=*|--bandwidth=*)
	    BW="${i#*=}"
	    shift # past argument=value
	    ;;
	-k=*|--bucket=*)
	    BUCKET="${i#*=}"
	    shift # past argument=value
	    ;;
	-f=*|--buffer=*)
	    BUFFER="${i#*=}"
	    shift # past argument=value
	    ;;
	--use_tbf)
	    USETBF="true"
	    shift # past argument with no value
	    ;;
	--use_htb)
	    USETBF="false"
	    shift # past argument with no value
	    ;;
	*)
	    # unknown option
	    ;;
    esac
    done

echo "Using options:"
echo "  use_tbf:   ${USETBF}"
echo "  bandwidth: ${BW}"
echo "  delay:     ${DELAY}"
echo "  bucket:    ${BUCKET}"
echo "  buffer:    ${BUFFER}"

if [[ "$PRINT_STATUS" = "true" ]]
then
    $TC -s qdisc ls dev $DEV
    $TC -s class ls dev $DEV
exit
fi

# clean existing down- and uplink qdiscs, hide errors
$TC qdisc del dev $DEV root    2> /dev/null > /dev/null
$TC qdisc del dev $DEV ingress 2> /dev/null > /dev/null

if [[ "$CLEAR" = "true" ]]
then
    exit
fi

let "PEAKBW=${BW}+10"
let "BW2=${BW}+${BW}"

###### uplink

$TC qdisc add dev ${DEV} root handle 1: prio
$TC qdisc add dev ${DEV} parent 1:1 handle 11: netem delay ${DELAY}ms
$TC qdisc add dev ${DEV} parent 1:2 handle 12: pfifo

if [[ "$USETBF" = "true" ]]
then
    $TC qdisc add dev ${DEV} parent 11:1 handle 2: tbf \
	rate ${BW}bit limit ${BUFFER}k burst ${BUCKET}
else
    $TC qdisc add dev ${DEV} parent 11:1 handle 2: htb 
    $TC class add dev ${DEV} parent 2: classid 2:1 htb rate ${BW2}bit # ceil ${PEAKBW}bit # burst ${BUCKET # cburst 10
    $TC class add dev ${DEV} parent 2:1 classid 2:10 htb rate ${BW}bit ceil ${PEAKBW}bit # burst ${BUCKET} # cburst 10
    $TC qdisc add dev ${DEV} parent 2:10 handle 21: pfifo
fi

echo "set qdiscs up"

# FILTER APPLICATION TRAFFIC VERSUS NON APP TRAFIC
$TC filter add dev ${DEV} protocol ip parent 1: prio 1 u32 \
    match ip dst 10.1.1.0/24 flowid 1:1

if [[ "$USETBF" = "true" ]]
then
    echo ""
else
    $TC filter add dev ${DEV} protocol ip parent 2: prio 1 u32 \
	match ip dst 10.1.1.0/24 flowid 2:10
fi

echo "set priority filters up"

$TC filter add dev ${DEV} protocol ip parent 1: prio 2 u32 \
    match ip src 192.168.122.0/24 flowid 1:2
#$TC filter add dev ${DEV} protocol ip parent 1: prio 2 u32 \
#    match ip src 10.1.1.0/24 flowid 1:2
echo "set other filters up"
