/*
<Jsoncpp ���̺귯�� ���� ���(Release x64), static_lib>
Git Bash �� ��ġ�Ѵ�.
CMake�� ��ġ�Ѵ�.
Visual Studio�� ��ġ�Ѵ� (Visual Studio 17 2022, Visual Studio 16 2019 ��)
C����̺꿡 git bash�� ������ �������� �����ϰ�
git clone https://github.com/open-source-parsers/jsoncpp.git
�� �Է��Ͽ� ������Ʈ ��ġ

cd jsoncpp
mkdir build
cd build

�Ʒ� ��ɾ ���� lib����� �ַ�� ������Ʈ�� �����
cmake -G "Visual Studio 17 2022" -A x64 -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release ..
sln ������Ʈ�� �����ϰ� jsoncpp_static �� �����Ѵ�

����, 64��Ʈ�� �����ϰ� 32��Ʈ�� �����ϰ��� �ϸ� ���� ���� ���ϰ� CMake ĳ�ø� �����ؾ��Ѵ�.
cd C:/jsoncpp/build
rm -rf CMakeCache.txt CMakeFiles/
cmake -G "Visual Studio 17 2022" -A Win32 ..

<���� ���丮>
C:\jsoncpp\include

<���̺귯�� ���丮 : ������ >
C:\jsoncpp\build\lib\Release

<Linker -> Input>
jsoncpp.lib
*/

#include <iostream>
#include <string>
#include <Windows.h>
#include <json/json.h>

std::string g_name;
int g_age;
std::string g_city;

std::string CreateJsonMsg()
{
    Json::Value obj;
    obj["name"] = "John";
    obj["age"] = 30;
    obj["city"] = "New York";

    Json::StreamWriterBuilder writer;
    std::string json_string = Json::writeString(writer, obj);
    return json_string;
}

void ParsingJson(std::string a_json)
{
    Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();
    Json::Value obj;
    std::string errs;

    if (reader->parse(a_json.c_str(), a_json.c_str() + a_json.size(), &obj, &errs))
    {
        g_name = obj["name"].asString();
        g_age = obj["age"].asInt();
        g_city = obj["city"].asString();
    }
    else {
        /* JSON �Ľ� ���� */
        printf("Error parsing JSON: %s\n", errs.c_str());
    }

    delete reader;
}

int main()
{
    _wsetlocale(0, L"");

    // JSON�޽��� �����
    std::string json = CreateJsonMsg();
    printf("%s\n", json.c_str());
    
    // JSON�޽��� �б�
    std::string json_msg = R"({"name":"John","age":30,"city":"New York"})";
    ParsingJson(json_msg);

    printf("%s - %d - %s\n", g_name.c_str(), g_age, g_city.c_str());
}

