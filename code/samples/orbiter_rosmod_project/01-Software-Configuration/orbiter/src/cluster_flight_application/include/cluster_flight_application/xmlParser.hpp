#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <map>

#include "boost/filesystem.hpp"

#include "cluster_flight_application/rapidxml.hpp"
#include "cluster_flight_application/rapidxml_utils.hpp"

using namespace rapidxml;

class GroupXMLParser
{
public:
  std::map<std::string,std::string> portGroupMap;

  void Print()
  {
    std::map<std::string,std::string>::iterator it;
    for (it=portGroupMap.begin(); it!=portGroupMap.end(); ++it)
      std::cout << it->first << " => " << it->second << '\n';
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
	std::string groupID;
	for (xml_attribute<> *attr = node->first_attribute();
	     attr; attr = attr->next_attribute())
	  {
	    if ( !strcmp(attr->name(),"ID") )
	      groupID = attr->value();
	  }
	for (xml_node<> *child = node->first_node("port"); child; child = child->next_sibling())
	  {
	    for (xml_attribute<> *attr = child->first_attribute();
		 attr; attr = attr->next_attribute())
	      {
		if ( !strcmp(attr->name(),"ID") )
		    portGroupMap.insert( std::pair<std::string,std::string>(attr->value(),groupID) );
	      }	    
	  }
      }
    return true;
  }
};
