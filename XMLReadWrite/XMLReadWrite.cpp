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

    // XML 파일을 읽어 문자열로 저장
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

    // 문자열을 XML파일로 저장
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
    // XML파일을 읽어 문자열로 저장
    XMLManager xml;

    xml.xml_size = xml.ReadXmlFileToString("C:\\xml\\scenario1.xml");
    printf("%s\n xml크기: %d\n", xml.xml_content, xml.xml_size);
    

    // 소켓을 통해 xml_size와 xml_content를 전송
    int result = xml.WriteStringToXmlFile("C:\\xml\\시나리오 파일을 새로 생성1.xml");          // xml_size와 xml_content를 멤버변수에 담아 WriteStringToXmlFile을 사용
    printf("생성결과 : %d", result);
}

