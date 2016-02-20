#!/bin/bash

NODENUM=$1

printf -v interfaces '# This file describes the network interfaces available on your system
# and how to activate them. For more information, see interfaces(5).

# The loopback network interface
auto lo
iface lo inet loopback

# The primary network interface
auto eth0
iface eth0 inet static
      address 10.1.1.'"${NODENUM}"'
      netmask 255.255.255.0
      network 10.1.1.0

auto eth1
iface eth1 inet static
      address 191.168.127.'"${NODENUM}"'
      netmask 255.255.255.0
      network 191.168.127.0

# Ethernet/RNDIS gadget (g_ether)
# Used by: /opt/scripts/boot/autoconfigure_usb0.sh
iface usb0 inet static
    address 192.168.7.2
    netmask 255.255.255.252
    network 192.168.7.0
    gateway 192.168.7.1
'
echo "$interfaces" > /etc/network/interfaces

printf -v hosts '127.0.0.1	localhost
127.0.1.1	beaglebone.localdomain	beaglebone
127.0.0.1	node'"${NODENUM}"'

# The following lines are desirable for IPv6 capable hosts
::1     localhost ip6-localhost ip6-loopback
ff02::1 ip6-allnodes
ff02::2 ip6-allrouters

191.168.127.100 master
191.168.127.1 node1
191.168.127.2 node2
191.168.127.3 node3
191.168.127.4 node4
191.168.127.5 node5
191.168.127.6 node6
191.168.127.7 node7
191.168.127.8 node8
191.168.127.9 node9
191.168.127.10 node10
191.168.127.11 node11
191.168.127.12 node12
191.168.127.13 node13
191.168.127.14 node14
191.168.127.15 node15
191.168.127.16 node16
191.168.127.17 node17
191.168.127.18 node18
191.168.127.19 node19
191.168.127.20 node20
191.168.127.21 node21
191.168.127.22 node22
191.168.127.23 node23
191.168.127.24 node24
191.168.127.25 node25
191.168.127.26 node26
191.168.127.27 node27
191.168.127.28 node28
191.168.127.29 node29
191.168.127.30 node30
191.168.127.31 node31
191.168.127.32 node32
'
echo "$hosts" > /etc/hosts
echo "node${NODENUM}" > /etc/hostname

