# Printer
#
# Print information about a ROS workspace
# in: listener object
#
# Author: Pranav Srinivas Kumar
# Date: 2014.12.16

class Printer:

    # Print information about the ROS workspace
    def print_ros_workspace(self, listener):

	print "Workspace: ", listener.workspace.name
	print "---------\n"
	# For each package in the workspace
	for package in listener.workspace.packages:
	    # Print package name
	    print "Package: ", package.name
	    print "-------\n"

	    # For each message in the package
	    if package.messages != []:
	        for message in package.messages:
		    print "Message: ", message.name
		    print "-------"
                    # Print all message fields
		    for field in message.fields:
		        print "* ", field[0], field[1]
		    print "\n"

	    # For each service in the package
	    if package.services != []:
	        for service in package.services:
		    print "Service: ", service.name
		    print "-------"
                    # Print all service request fields
		    if service.request_fields != []:
		        print "Request Fields:"
		        print "--------------"
		        for req in service.request_fields:
			    if len(req) > 2:
			        print "* ", req[0], req[1], " = ", req[2]
			    else:
			        print "* ", req[0], req[1]
                    # Print all service response fields
		    if service.response_fields != []:
		        print "Response Fields:"
		        print "---------------"
		        for res in service.response_fields:
			    if len(res) > 2:
			        print "* ", res[0], res[1], " = ", res[2]
			    else:
			        print "* ", res[0], res[1]
		    print "\n"
            
            # For each component in the package
            if package.components != []:
                for component in package.components:
                    print "Component: ", component.name
                    print "---------"
                    # Print all provided services
                    if component.provided_services != []:
                        print "Provided Services: "
                        print "-----------------"
                        for provided in component.provided_services:
                            print "* ", provided
                    # Print all required services
                    if component.required_services != []:
                        print "Required Services: "
                        print "-----------------"
                        for required in component.required_services:
                            print "* ", required
                    # Print all publisher ports
                    if component.publishers != []:
                        print "Publishers: "
                        print "----------"
                        for publisher in component.publishers:
                            print "* Name: ", publisher.name, "; Topic: ", publisher.topic
                    # Print all subscriber ports
                    if component.subscribers != []:
                        print "Subscribers: "
                        print "-----------"
                        for subscriber in component.subscribers:
                            print "* Name: ", subscriber.name, "; Topic: ", subscriber.topic
                    # Print all component timers
                    if component.timers != []:
                        print "Timers: "
                        print "------"
                        for timer in component.timers:
                            print "* ", timer.name, "; Period: ", timer.period, timer.period_unit
                    print "\n"
            
                    
            # For each node in the package
            if package.nodes != []:
                for node in package.nodes:
                    print "Node: ", node.name
                    print "----"
                    for component in node.components:
                        print "* Instance: ", component[1], "; Type: ", component[0]
                    print "\n"
