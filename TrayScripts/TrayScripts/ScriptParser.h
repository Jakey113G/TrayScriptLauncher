#pragma once
#include <string>
#include "boost/property_tree/ptree_fwd.hpp"
#include "boost/property_tree/xml_parser.hpp"

//We are using the boost property tree because it is easier than creating my own.

typedef boost::property_tree::wptree XMLTree;
typedef std::vector<std::wstring> AttributeCollection;

namespace ScriptParser
{
    XMLTree CreatePropertyTree(std::string);

    const boost::property_tree::wptree& empty_wptree();

    std::wstring GetAttribute(XMLTree& it,std::wstring attribute);
}
