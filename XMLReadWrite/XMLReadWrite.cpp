#include <iostream>
#include <string>

#define XML_MAX_SIZE            262144

class XMLManager
{
private:

public:
    char* mp_xml_content;
    const int xml_max_size = XML_MAX_SIZE;
    std::string xml_title = "";

    XMLManager() 
        : mp_xml_content(new char[XML_MAX_SIZE] {0})
    {
    }

    virtual ~XMLManager() {
        delete[] mp_xml_content;
    }

    // XML ������ �о� ���ڿ��� ����
    size_t ReadXmlFileToString(const char* a_filename)
    {
        for (int i = 0; i < XML_MAX_SIZE; i++) {
            mp_xml_content[i] = 0;
        }

        FILE* p_file = NULL;
        int file_open = -1;
        file_open = fopen_s(&p_file, a_filename, "rb");
        if (file_open != 0)
        {
            printf("Failed to open XML file : %s\n", a_filename);
            return 0;
        }

        size_t bytes_read = fread(mp_xml_content, 1, xml_max_size - 1, p_file);
        mp_xml_content[bytes_read] = '\0';

        fclose(p_file);
        return bytes_read;
    }

    // ���ڿ��� XML���Ϸ� ����
    int WriteStringToXmlFile(const char* a_filename)
    {
        FILE* p_file = NULL;
        int file_open = -1;
        file_open = fopen_s(&p_file, a_filename, "wb");
        if (file_open != 0) {
            printf("Failed to create XML file: %s\n", a_filename);
            return 0;
        }

        size_t length = strlen(mp_xml_content);
        size_t written = fwrite(mp_xml_content, 1, length, p_file);

        fclose(p_file);

        if (written != length) {
            printf("Failed to write XML content to file.\n");
            return 0;
        }

        return 1;
    }
};

/*
������ �а� ī���ϴ� ����
*/
int main()
{
    // XML������ �о� ���ڿ��� ����
    XMLManager xml;

    // XML ���� ��ü�� �о� ���ڿ��� �����Ѵ�
    size_t xml_size = xml.ReadXmlFileToString("C:\\xml\\OTS.xml");
    printf("xmlũ��: %lld\n%s\n", xml_size, xml.mp_xml_content);

    // ���� ������ ���ο� xml ���Ϸ� ī���Ѵ�
    int result = xml.WriteStringToXmlFile("C:\\xml\\�ó����� ������ ���� ����1.xml");          // xml_size�� xml_content�� ��������� ��� WriteStringToXmlFile�� ���
    printf("������� : %d", result);
}

