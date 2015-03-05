#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <stdio.h>

#include "boost/filesystem.hpp"

#include "satellite_flight_application/rapidxml.hpp"
#include "satellite_flight_application/rapidxml_utils.hpp"

using namespace rapidxml;

class Group
{
public:
  std::string ID;
  std::vector<std::string> ports;

  void Print()
  {
    std::cout << "Group ID: " << ID << "\n";
    std::cout << "Ports:\n";
    for (int i=0;i<ports.size();i++)
      std::cout << "\tPort ID: " << ports[i] << "\n";
  }
};

class GroupXMLParser
{
public:
  std::vector<Group> groups;

  void Print()
  {
    for (int i=0;i<groups.size();i++)
      groups[i].Print();
  }

  bool Parse(std::string fName)
  {
    if ( !boost::filesystem::exists(fName) )
      return false;
    rapidxml::file<> xmlFile(fName.c_str()); // Default template is char
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());

    for (xml_node<> *node = doc.first_node("group"); node; node = node->next_sibling())
      {
	Group group;
	for (xml_attribute<> *attr = node->first_attribute();
	     attr; attr = attr->next_attribute())
	  {
	    if ( !strcmp(attr->name(),"ID") )
	      group.ID = attr->value();
	  }
	for (xml_node<> *child = node->first_node("port"); child; child = child->next_sibling())
	  {
	    for (xml_attribute<> *attr = child->first_attribute();
		 attr; attr = attr->next_attribute())
	      {
		if ( !strcmp(attr->name(),"ID") )
		  group.ports.push_back(attr->value());
	      }	    
	  }
	groups.push_back(group);
      }
    return true;
  }
};
