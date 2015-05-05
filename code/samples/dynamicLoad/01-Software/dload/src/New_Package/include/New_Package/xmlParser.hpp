#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <map>

#include "boost/filesystem.hpp"

#include "New_Package/rapidxml.hpp"
#include "New_Package/rapidxml_utils.hpp"
#include "New_Package/Logger.hpp"
using namespace rapidxml;

class GroupXMLParser
{
public:
  std::map<std::string,std::string> portGroupMap;
  Log_Levels logging;

  void Print()
  {
    std::map<std::string,std::string>::iterator it;
    for (it=portGroupMap.begin(); it!=portGroupMap.end(); ++it)
      std::cout << it->first << " => " << it->second << '\n';
  }

  bool Return_Boolean(std::string value) 
  {
    if (value == "true") 
      return true;
    else
      return false;
  }

  bool Parse(std::string fName)
  {
    if ( !boost::filesystem::exists(fName) )
      return false;
    rapidxml::file<> xmlFile(fName.c_str()); // Default template is char
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());

    /*
     * Establish log levels
     */
    xml_node<> *logger = doc.first_node("logging");
    xml_node<> * debug = logger->first_node("debug");
    
    logging.DEBUG = Return_Boolean(logger->first_node("debug")->first_attribute()->value());
    logging.INFO = Return_Boolean(logger->first_node("info")->first_attribute()->value());
    logging.WARNING = Return_Boolean(logger->first_node("warning")->first_attribute()->value());
    logging.ERROR = Return_Boolean(logger->first_node("error")->first_attribute()->value());
    logging.CRITICAL = Return_Boolean(logger->first_node("critical")->first_attribute()->value());

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
