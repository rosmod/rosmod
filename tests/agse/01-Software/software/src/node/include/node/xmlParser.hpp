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
  uint64_t num_comps_to_sync;
  double comp_sync_timeout;
  std::map<std::string,uint64_t> uuidMap;
  std::map<std::string,std::string> profileMap;
  std::map<std::string,std::map<uint64_t,std::string>> portSenderMap;
  bool is_periodic_logging;
  int periodic_log_unit;
};

using namespace rapidxml;

class XMLParser
{
public:
  std::vector<ComponentConfig> compConfigList;
  std::vector<std::string> libList;
  std::string nodeName;
  int node_priority = -1;

  bool Return_Boolean(std::string value) { return (value == "True"); }

  void PrintNode(xml_node<> *node, std::string& prepend)
  {
    std::string local_prepend = prepend;
    printf("%s%s:\n",local_prepend.c_str(),node->name());
    for (xml_attribute<> *tmpAttr = node->first_attribute();
	 tmpAttr; tmpAttr = tmpAttr->next_attribute())
      {
	printf("%s\t%s: %s\n",
	       local_prepend.c_str(),
	       tmpAttr->name(),
	       tmpAttr->value());
      }
    local_prepend += "\t";
    for (xml_node<> *tmpNode = node->first_node();
	 tmpNode; tmpNode = tmpNode->next_sibling())
      {
	PrintNode(tmpNode, local_prepend);
      }
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
    
    xml_node<> *node_prio = node->first_node("priority");
    if (node_prio != NULL)
      node_priority = atoi(node_prio->first_attribute()->value());

    for (xml_node<> *lib_location = node->first_node("library");
	 lib_location; lib_location = lib_location->next_sibling("library"))
      {
	libList.push_back(lib_location->first_attribute()->value());
      }

    for (xml_node<> *comp_inst = node->first_node("component_instance"); 
	 comp_inst; comp_inst = comp_inst->next_sibling("component_instance"))
      {
	ComponentConfig config;
	config.num_comps_to_sync = 1;
	config.comp_sync_timeout = 1.0;
	config.compName = comp_inst->first_attribute()->value();
	config.nodeName = nodeName;

	xml_node<> *nCompsSync = comp_inst->first_node("numCompsToSync");
	if (nCompsSync != NULL)
	  config.num_comps_to_sync = atoi(nCompsSync->first_attribute()->value());
	
	xml_node<> *syncTimeout = comp_inst->first_node("syncTimeout");
	if (syncTimeout != NULL)
	  config.comp_sync_timeout = atof(syncTimeout->first_attribute()->value());
	
	for (xml_node<> *port = comp_inst->first_node("port");
	     port; port = port->next_sibling("port"))
	  {
	    std::string portName = port->first_attribute()->value();
	    uint64_t u = atoi(port->first_node("uuid")->value());
	    std::string p = port->first_node("profile")->value();
	    config.uuidMap.insert(std::pair<std::string,uint64_t>(portName,u));
	    config.profileMap.insert(std::pair<std::string,std::string>(portName,p));
	    
	    std::map<uint64_t,std::string> upM;
	    for (xml_node<> *sender = port->first_node("sender");
		 sender; sender = sender->next_sibling("sender"))
	      {
		uint64_t u = atoi(sender->first_attribute()->value());
		std::string p = sender->first_node("profile")->value();
		upM.insert(std::pair<uint64_t,std::string>(u,p));
	      }
	    config.portSenderMap.insert(std::pair<std::string,
					std::map<uint64_t,
					std::string>>(portName,upM));
	  }
	
	xml_node<> *lib_location = comp_inst->first_node("library");
	config.libraryLocation = lib_location->first_attribute()->value();
	
	xml_node<> *sched_scheme = comp_inst->first_node("scheduling_scheme");
	config.schedulingScheme = sched_scheme->first_attribute()->value();
	
	xml_node<> *logger = comp_inst->first_node("logging");
	config.is_periodic_logging = 
	  Return_Boolean(logger->first_node("is_periodic_logging")->first_attribute()->value());
	xml_node<> *plu = logger->first_node("periodic_log_unit");
	if (plu != NULL)
	  config.periodic_log_unit = 
	    atoi(plu->first_attribute()->value());
	
	for (xml_node<> *port_inst = comp_inst->first_node("port_instance"); 
	     port_inst; port_inst = port_inst->next_sibling("port_instance"))
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
