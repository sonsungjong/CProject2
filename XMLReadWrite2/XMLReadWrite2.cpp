#include <iostream>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#define XML_PATH            "C:\\xml\\output.xml"
#define XML_MAX_SIZE            262144

/*
<root>
    <child>value</child>
</root>
*/
void createXML()
{
    boost::property_tree::ptree pt;
    pt.put("root.child", "value");

    boost::property_tree::write_xml(XML_PATH, pt);
}

// root 안의 child 태그 값을 읽기 (전체 읽기는 전통적인 방법으로 해야함)
void readXML()
{
    boost::property_tree::ptree pt;
    boost::property_tree::read_xml(XML_PATH, pt);

    std::string child_value = pt.get<std::string>("root.child");
    std::cout << "Child value: " << child_value << std::endl;
}

void updateXML()
{
    boost::property_tree::ptree pt;
    boost::property_tree::read_xml(XML_PATH, pt);

    // root안에 child의 값 변경
    pt.put("root.child", "modifiedValue");

    // 새로운 노드 추가
    pt.add("root.newChild", "newChildValue");

    boost::property_tree::write_xml(XML_PATH, pt);
}

int main()
{
    createXML();
    readXML();

    return 0;
}