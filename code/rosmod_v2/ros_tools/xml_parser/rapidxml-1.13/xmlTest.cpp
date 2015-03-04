#include <iostream>
#include <fstream>
#include <vector>

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"

using namespace rapidxml;

int main(){
  rapidxml::file<> xmlFile("testGroups.xml"); // Default template is char
  rapidxml::xml_document<> doc;
  doc.parse<0>(xmlFile.data());

  for (xml_node<> *node = doc.first_node("group"); node; node = node->next_sibling())
    {
      for (xml_attribute<> *attr = node->first_attribute();
	   attr; attr = attr->next_attribute())
	{
	  std::cout << "Group " << attr->name() << " = ";
	  std::cout << attr->value() << "\n";
	}
      std::cout << "has ports:\n";
      for (xml_node<> *child = node->first_node("port"); child; child = child->next_sibling())
	{
	  for (xml_attribute<> *attr = child->first_attribute();
	       attr; attr = attr->next_attribute())
	    {
	      std::cout << "\tPort " << attr->name() << " =  ";
	      std::cout << attr->value() << "\n";
	    }	    
	}
    }
}
