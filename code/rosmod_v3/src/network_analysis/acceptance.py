#!/usr/bin/python
"""
This program is designed to do admissibilty tests for admission of an application
or set of applications to the F6 satellite cluster.  Each application may be 
split accross multiple nodes of the cluster.  Each node has its own network
interface and as such, each node's bandwidth is independent of the other nodes'
network utilization.  Therefore each node's bandwidth is modeled as a network
"link" which connects from that node to all other nodes.  
""" 

from networkProfile import *

orbital_period = (90*60)    # orbital period in seconds
num_periods = 1
selected_node = ''
selected_interface = ''

def parse_args(args):
    global orbital_period
    global num_periods
    global selected_node
    global selected_interface

    argind = 1
    while argind < len(args):
        if args[argind] == "-P":
            orbital_period = int(args[argind+1])
            if orbital_period <= 0:
                print "Error! You must specify a time period > 0"
                return -1
            argind += 2
        elif args[argind] == "-n":
            num_periods = int(args[argind+1])
            if num_periods <= 0:
                print "Error! You must specify a number of periods > 0"
                return -1
            argind += 2
        elif args[argind] == "-N":
            selected_node = args[argind+1]
            argind += 2
        elif args[argind] == "-I":
            selected_interface = args[argind+1]
            argind += 2
        elif args[argind] == "-?" or args[argind] == "-h":
            print "Usage:\n\tpython ",args[0],"""
            \t\t-N <node name>
            \t\t-I <node interface name>
            \t\t-P <period (s)>
            \t\t-n <number of periods to analyze>\n"""
            return -1
        else:
            print """Usage:\n\t""",args[0],"""
            \t\t-N <node name>
            \t\t-I <node interface name>
            \t\t-P <period (s)>
            \t\t-n <number of periods to analyze>\n"""
            return -1
    return 0

def main():
    global selected_node
    global selected_interface
    global orbital_period
    global num_periods
    args = sys.argv

    if parse_args(args):
        return -1

    nodes = get_nodeProfiles('scripts')
    if nodes == {}:
        return -1
    apps = get_appProfiles('profiles')
    if apps == {}:
        return -1
    app_node_map = get_app_node_map(nodes,apps)
    networkProfile = NetworkProfile(orbital_period)
    for node,profile in nodes.iteritems():
        nodeProfile = NodeProfile(orbital_period,num_periods)
        nodeProfile.addProvidedProfile(profile)
        if node in app_node_map.keys():
            for app in app_node_map[node]:
                if "," in apps[app]:
                    nodeProfile.addRequiredProfile(apps[app])
        networkProfile.addNodeProfile(node,nodeProfile)
    networkProfile.calcData()

    if selected_node == '':
        selected_node=nodes.keys()[0]
    if selected_node not in nodes:
        print 'ERROR: node {0} not found in system!'.format(selected_node)
        return -1

    if selected_interface == '':
        if len(networkProfile.nodeProfiles[selected_node].interfaces) > 0:
            selected_interface = networkProfile.nodeProfiles[selected_node].interfaces[0]
        else:
            print 'ERROR: node {0} has no interfaces that can be analyzed!'.format(selected_node)
            return -1
    if selected_interface not in networkProfile.nodeProfiles[selected_node].interfaces:
        print 'ERROR: node {0} has no interface named {1}!'.format(selected_node,selected_interface)
        return -1

    print 'Using node: interface {0} on node {1}'.format(selected_interface,selected_node)
    print "Using period ",orbital_period," over ",num_periods," periods"

    if networkProfile.convolve(selected_node,selected_interface) == -1:
        print 'Node {0} has cannot be analyzed for interface {1}: no usable profile'.format(selected_node,selected_interface)

    buff = networkProfile.nodeProfiles[selected_node].buffer
    print "\n[Time location, buffersize]:",[buff[0],buff[2]]

    delay = networkProfile.nodeProfiles[selected_node].delay
    print "[Time location, delay]:",[delay[0],delay[2]]

    return
  
if __name__ == "__main__":
    main()
