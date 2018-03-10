#include "ScriptParser.h"
#include <map>

XMLTree ScriptParser::CreatePropertyTree(std::string xmlPath)
{
    XMLTree tree;
    boost::property_tree::read_xml(xmlPath, tree);

    return tree;
}

const boost::property_tree::wptree& ScriptParser::empty_wptree() 
{
    static boost::property_tree::wptree t;
    return t;
}

//A wrapper around some stupid logic to extract an attribute from an xml element
std::wstring ScriptParser::GetAttribute(XMLTree& it, std::wstring attribute)
{
    std::wstring at = L"<xmlattr>";

    //Look in the child trees for the xmlattr tag
    const XMLTree& attributes = it.get_child(at);

    std::wstring attributeValue;
    for (auto& Attributeit : attributes)
    {
        if (Attributeit.first.data() == attribute)
        {
            attributeValue = Attributeit.second.data();
            break;
        }
    }
    
    return attributeValue;
}
