#include <iostream>
#include <string>

#define XML_MAX_SIZE            15000

class XMLManager
{
private:

public:
    char xml_content[XML_MAX_SIZE];
    int xml_size = 0;
    const int xml_max_size = XML_MAX_SIZE;
    std::string xml_title = "";

    XMLManager() {
        for (int i = 0; i < XML_MAX_SIZE; i++) {
            xml_content[i] = 0;
        }
    }
    virtual ~XMLManager() {}

    // XML ������ �о� ���ڿ��� ����
    size_t ReadXmlFileToString(const char* a_filename)
    {
        xml_size = 0;
        for (int i = 0; i < XML_MAX_SIZE; i++) {
            xml_content[i] = 0;
        }

        FILE* p_file = NULL;
        int file_open = -1;
        file_open = fopen_s(&p_file, a_filename, "rb");
        if (file_open != 0)
        {
            printf("Failed to open XML file : %s\n", a_filename);
            return 0;
        }

        size_t bytes_read = fread(xml_content, 1, xml_max_size - 1, p_file);
        xml_content[bytes_read] = '\0';

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

        size_t length = strlen(xml_content);
        size_t written = fwrite(xml_content, 1, length, p_file);

        fclose(p_file);

        if (written != length) {
            printf("Failed to write XML content to file.\n");
            return 0;
        }

        return 1;
    }
};

int main()
{
    // XML������ �о� ���ڿ��� ����
    XMLManager xml;

    xml.xml_size = xml.ReadXmlFileToString("C:\\xml\\scenario1.xml");
    printf("%s\n xmlũ��: %d\n", xml.xml_content, xml.xml_size);
    

    // ������ ���� xml_size�� xml_content�� ����
    int result = xml.WriteStringToXmlFile("C:\\xml\\�ó����� ������ ���� ����1.xml");          // xml_size�� xml_content�� ��������� ��� WriteStringToXmlFile�� ���
    printf("������� : %d", result);
}

