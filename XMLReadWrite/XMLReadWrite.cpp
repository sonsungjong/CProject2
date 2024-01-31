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

    // XML 파일을 읽어 문자열로 저장
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
간단히 읽고 카피하는 예제
*/
int main()
{
    // XML파일을 읽어 문자열로 저장
    XMLManager xml;

    // XML 파일 전체를 읽어 문자열로 저장한다
    size_t xml_size = xml.ReadXmlFileToString("C:\\xml\\OTS.xml");
    printf("xml크기: %lld\n%s\n", xml_size, xml.mp_xml_content);

    // 읽은 내용을 새로운 xml 파일로 카피한다
    int result = xml.WriteStringToXmlFile("C:\\xml\\시나리오 파일을 새로 생성1.xml");          // xml_size와 xml_content를 멤버변수에 담아 WriteStringToXmlFile을 사용
    printf("생성결과 : %d", result);
}

