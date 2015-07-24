# ROSMOD Builder
# Author: Pranav Srinivas Kumar
# Date: 2015.07.23

import os
import xml.etree.ElementTree as ET
from classes import Project

class Builder():
    def __init__(self, xml):
        self.xml = xml

    def run(self):

        model = ET.parse('model.xml')
        project = model.getroot()

        new_project = Project(project.find("name").text)
        print "Add attributes test" + new_project["name"]
        
        for submodel in project:
            if submodel.tag == "software":
                workspace_name = Attribute("string", submodel.find("name").text)
                new_software_model = Software(workspace_name)
                for message in submodel.findall("message"):
                    print "Message: " + message.find("name").text
                    print message.find("definition").text
                for component in submodel.findall("component"):
                    print component.find("name").text
            elif submodel.tag == "hardware":
                print "Hardware Name: " + submodel.find("name").text
            elif submodel.tag == "deployment":
                print "Deployment Name: " + submodel.find("name").text

my_builder = Builder(os.path.abspath("model.xml"))
my_builder.run()
