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

// root ���� child �±� ���� �б� (��ü �б�� �������� ������� �ؾ���)
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

    // root�ȿ� child�� �� ����
    pt.put("root.child", "modifiedValue");

    // ���ο� ��� �߰�
    pt.add("root.newChild", "newChildValue");

    boost::property_tree::write_xml(XML_PATH, pt);
}

int main()
{
    createXML();
    readXML();

    return 0;
}