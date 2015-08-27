# ROSMOD Builder
# Author: Pranav Srinivas Kumar
# Date: 2015.07.23

import json, jsonpickle
from classes import *

with open('model.txt', 'r') as input_model:
    decoder_output = jsonpickle.decode(input_model.read())
    print "Done decoding"
    print type(decoder_output)
    print decoder_output

    # The following prints will point to the same object!
    # Service defined in a package
    print "Service: " + str(decoder_output.children._inner[0].children[0].children[1])
    # Client's service_reference 
    print "Service Reference: " + str(decoder_output.children._inner[0].children[0].children[2].children[0]["service_reference"])
    print "Service Reference Value: " + str(decoder_output.children._inner[0].children[0].children[2].children[0]["service_reference"].value)
