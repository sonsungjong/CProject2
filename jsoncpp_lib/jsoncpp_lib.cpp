/*
<Jsoncpp 라이브러리 빌드 방법(Release x64), static_lib>
Git Bash 를 설치한다.
CMake를 설치한다.
Visual Studio를 설치한다 (Visual Studio 17 2022, Visual Studio 16 2019 등)
C드라이브에 git bash를 관리자 권한으로 실행하고
git clone https://github.com/open-source-parsers/jsoncpp.git
를 입력하여 프로젝트 설치

cd jsoncpp
mkdir build
cd build

아래 명령어를 통해 lib기반의 솔루션 프로젝트를 만든다
cmake -G "Visual Studio 17 2022" -A x64 -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release ..
sln 프로젝트를 실행하고 jsoncpp_static 을 빌드한다

만약, 64비트로 빌드하고 32비트도 빌드하고자 하면 이전 빌드 파일과 CMake 캐시를 제거해야한다.
cd C:/jsoncpp/build
rm -rf CMakeCache.txt CMakeFiles/
cmake -G "Visual Studio 17 2022" -A Win32 ..

<포함 디렉토리>
C:\jsoncpp\include

<라이브러리 디렉토리 : 릴리즈 >
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
        /* JSON 파싱 에러 */
        printf("Error parsing JSON: %s\n", errs.c_str());
    }

    delete reader;
}

int main()
{
    _wsetlocale(0, L"");

    // JSON메시지 만들기
    std::string json = CreateJsonMsg();
    printf("%s\n", json.c_str());
    
    // JSON메시지 읽기
    std::string json_msg = R"({"name":"John","age":30,"city":"New York"})";
    ParsingJson(json_msg);

    printf("%s - %d - %s\n", g_name.c_str(), g_age, g_city.c_str());
}

