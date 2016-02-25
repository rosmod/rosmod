#!/usr/bin/python
from time import sleep
from mininet.topo import Topo
from mininet.net import Mininet
from mininet.node import CPULimitedHost
from mininet.link import TCLink
from mininet.util import dumpNodeConnections
from mininet.log import setLogLevel

class SingleSwitchTopo(Topo):
    "Single switch connected to n hosts."
    def build(self, n=3):
        switch = self.addSwitch('s1')
        for h in range(n):
            host = self.addHost('h%s' % (h + 1))
            self.addLink(host, switch, bw=0.07)

topos = { 'singleswitchtopo': ( lambda: SingleSwitchTopo(n=3) ) }

def perfTest():
    "Create network and run simple performance test"
    topo = SingleSwitchTopo(n=3)
    net = Mininet(topo=topo, 
                  link=TCLink)
    net.start()
    print "Dumping host connections"
    dumpNodeConnections(net.hosts)
    print "Testing network connectivity"
    net.pingAll()
    print "Testing bandwidth between h1 and h2"
    h1, h2 = net.get('h1', 'h2')
    net.stop()

def rosTest():
    "Create network and run simple performance test"
    topo = SingleSwitchTopo(n=3)
    net = Mininet(topo=topo, 
                  link=TCLink)
    net.start()
    print "Dumping host connections"
    dumpNodeConnections(net.hosts)
    print "Testing network connectivity"
    net.pingAll()
    h1, h2, h3 = net.get('h1', 'h2', 'h3')
    print "Starting roscore"
    output = h3.cmd('source /opt/ros/indigo/setup.bash && '
                    'export ROS_IP='+h3.IP()+
                    ' && export ROS_MASTER_URI=http://'+h3.IP()+
                    ':11311 && roscore &')
    print output
    print "Starting receiver"
    output = h2.cmd('export LD_LIBRARY_PATH=/home/jeb/:$LD_LIBRARY_PATH && export ROS_IP='+h2.IP()+
                    ' && export ROS_MASTER_URI=http://'+h3.IP()+
                    ':11311 && /home/jeb/node_main -config /home/jeb/receiver.xml --tg_time 50 &')
    print output
    print "Starting sender"
    output = h1.cmd('export LD_LIBRARY_PATH=/home/jeb/:$LD_LIBRARY_PATH && export ROS_IP='+h1.IP()+
                    ' && export ROS_MASTER_URI=http://'+h3.IP()+
                    ':11311 && /home/jeb/node_main -config /home/jeb/sender.xml --tg_time 50 --max_data_length_bits 6000 &')
    print output
    sleep(70)
    net.stop()

if __name__ == '__main__':
    setLogLevel('info')
    rosTest()
