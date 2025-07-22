
// Boost 라이브러리 1.83.0 버전
/*
설치
https://www.boost.org/ -> Download -> boost_1_83_0.zip -> 압축해제
boost_1_83_0 폴더 -> 관리자 권한으로 cmd 실행 -> .\bootstrap.bat 입력하여 실행
.\b2.exe 입력하여 빌드

포함 디렉토리 : C:\boost_1_83_0
라이브러리 디렉토리 : C:\boost_1_83_0\stage\lib
필요한 라이브러리를 입력에 넣어야함

라이브러리 입력(릴리즈64) : libboost_json-vc143-mt-x64-1_83.lib
라이브러리 입력(디버그64) : libboost_json-vc143-mt-gd-x64-1_83.lib
라이브러리 입력(릴리즈32) : libboost_json-vc143-mt-x32-1_83.lib
라이브러리 입력(디버그32) : libboost_json-vc143-mt-gd-x32-1_83.lib
*/

#include <iostream>
#include <tchar.h>
#include <vector>
#include <boost/json.hpp>

// 라이브러리 중 // 경로로 안되는 경우가 있음
#ifdef _DEBUG
#pragma comment(lib, "libboost_container-vc142-mt-gd-x64-1_83.lib")
#pragma comment(lib, "libboost_json-vc142-mt-gd-x64-1_83.lib")
#else
#pragma comment(lib, "libboost_container-vc142-mt-x64-1_83.lib")
#pragma comment(lib, "libboost_json-vc142-mt-x64-1_83.lib")
#endif

/*
data =
{
    "number" : 20240131,
    "name" : "harry kim",
    "phone" : {
        "kinds" : "IPhone8",
        "number" : "010-3333-4444"
    },
    "Field" : ["developer", "Machine Learning"],
    "num_arr" : [1, 2, 3, 4, 5]
}
*/
boost::json::object createJson()
{
    boost::json::object data;               // 객체 생성
    data["number"] = 20240131;
    data["name"] = "harry kim";

    // phone 객체 추가
    boost::json::object phone;
    phone["kinds"] = "IPhone8";
    phone["number"] = "010-3333-4444";
    data["phone"] = phone;              // 객체를 객체에 삽입

    // Field 배열 추가
    boost::json::array field;
    field.push_back("developer");
    field.push_back("Machine Learning");
    data["Field"] = field;                  // 배열을 객체에 삽입

    // num_arr 배열 추가
    boost::json::array num_arr;
    num_arr.push_back(1);
    num_arr.push_back(2);
    num_arr.push_back(3);
    num_arr.push_back(4);
    num_arr.push_back(5);
    data["num_arr"] = num_arr;              // 배열을 객체에 삽입

    // 문자열로 바꿔 출력
    std::string result = boost::json::serialize(data);
    //printf("%s\n", result.c_str());

    return data;            // JSON 리턴
}

/*
    boost::json::object 를 받으면 분석하고 파싱하여 저장한다.
*/
void parseJson(boost::json::object a_obj)
{
    // num_arr 키로 배열 찾기
    boost::json::array num_arr = a_obj["num_arr"].as_array();

    // 배열 크기 얻기
    std::size_t array_size = num_arr.size();
    printf("num_arr의 배열 갯수 : %lld\n", array_size);

    // 배열요소의 자료형 얻기
    if (num_arr[0].kind() == boost::json::kind::int64) {
        printf("num_arr 요소의 자료형 long long\n");
    }
    else if (num_arr[0].kind() == boost::json::kind::double_) {
        printf("num_arr 요소의 자료형 double\n");
    }
    else if (num_arr[0].kind() == boost::json::kind::uint64) {
        printf("num_arr 요소의 자료형 unsigned long long\n");
    }
    else if (num_arr[0].kind() == boost::json::kind::string) {
        printf("num_arr 요소의 자료형 std::string\n");
    }

    // 벡터로 옮기기
    std::vector<long long> numbers;
    for (auto& value : num_arr)
    {
        if (value.is_int64()) {
            numbers.push_back(value.as_int64());
        }
        else {
            printf("num_arr의 자료형이 일치하지 않습니다!!\n");
        }
    }

    // 벡터값 모두 출력
    for (auto& num : numbers)
    {
        printf("%lld, ", num);
    }
    printf("\n");

}

int main()
{
    boost::json::object json_object = createJson();                             // json 생성

    printf("%s\n", boost::json::serialize(json_object).c_str());                // 문자열로 바꿔 출력
    
    parseJson(json_object);             // json 파싱

    return 0;
}
