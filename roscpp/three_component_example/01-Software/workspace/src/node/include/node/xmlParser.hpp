#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <stdio.h>
#include <map>

#include "boost/filesystem.hpp"

#include "node/rapidxml.hpp"
#include "node/rapidxml_utils.hpp"
#include "node/Logger.hpp"

class ComponentConfig
{
public:
  std::string libraryLocation;
  std::string schedulingScheme;
  std::string hostName;
  std::string nodeName;
  std::string compName;
  std::map<std::string,std::string> portGroupMap;
  Log_Levels logLevels;
};

using namespace rapidxml;

class XMLParser
{
public:
  std::vector<ComponentConfig> compConfigList;
  std::string nodeName;

  bool Return_Boolean(std::string value) 
  {
    if (value == "True") 
      return true;
    else
      return false;
  }

  bool Parse(std::string fName)
  {
    if (!boost::filesystem::exists(fName))
      return false;
    rapidxml::file<> xmlFile(fName.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());

    xml_node<> *node = doc.first_node("node");
    nodeName = node->first_attribute()->value();

    for (xml_node<> *comp_inst = node->first_node("component_instance"); 
	 comp_inst; comp_inst = comp_inst->next_sibling())
      {
	ComponentConfig config;
	config.compName = comp_inst->first_attribute()->value();
	config.nodeName = nodeName;
	
	xml_node<> *lib_location = comp_inst->first_node("library");
	config.libraryLocation = lib_location->first_attribute()->value();
	
	xml_node<> *sched_scheme = comp_inst->first_node("scheduling_scheme");
	config.schedulingScheme = sched_scheme->first_attribute()->value();
	
	xml_node<> *logger = comp_inst->first_node("logging");
	config.logLevels.DEBUG = 
	  Return_Boolean(logger->first_node("debug")->first_attribute()->value());
	config.logLevels.INFO = 
	  Return_Boolean(logger->first_node("info")->first_attribute()->value());
	config.logLevels.WARNING = 
	  Return_Boolean(logger->first_node("warning")->first_attribute()->value());
	config.logLevels.ERROR = 
	  Return_Boolean(logger->first_node("error")->first_attribute()->value());
	config.logLevels.CRITICAL = 
	  Return_Boolean(logger->first_node("critical")->first_attribute()->value());
	
	for (xml_node<> *port_inst = comp_inst->first_node("port_instance"); 
	     port_inst; port_inst = port_inst->next_sibling())
	  {
	    std::string portInstName = port_inst->first_attribute()->value();
	    xml_node<> *port = port_inst->first_node("port");
	    std::string portName = port->first_attribute()->value();
	    xml_node<> *group = port_inst->first_node("group");
	    std::string groupID = group->first_attribute()->value();
	    config.portGroupMap.insert(std::pair<std::string,std::string>(portName,groupID));
	  }
	compConfigList.push_back(config);
      }
    return true;
  }
};



