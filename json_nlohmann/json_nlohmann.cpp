#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

/*
    https://github.com/nlohmann/json
*/

using json = nlohmann::json;


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
json createJson()
{
    // 객체 생성 및 값 할당
    json data;
    data["number"] = 20240131;
    data["name"] = "harry kim";

    // phone 객체 추가
    data["phone"] = {
        {"kinds", "IPhone8"},
        {"number", "010-3333-4444"}
    };

    // Field 배열 추가
    data["Field"] = { "developer", "Machine Learning" };

    // num_arr 배열 추가
    data["num_arr"] = { 1, 2, 3, 4, 5 };

    // JSON 객체 리턴
    return data;
}

void parseJson(const json& a_obj)
{
    // num_arr 키로 배열 찾기
    if (a_obj.contains("num_arr") && a_obj["num_arr"].is_array())
    {
        const json& num_arr = a_obj["num_arr"];

        // 배열 크기 얻기
        std::size_t array_size = num_arr.size();
        std::printf("num_arr의 배열 갯수 : %zu\n", array_size);

        // 배열요소의 자료형 얻기 (첫번째 요소 기준)
        if (num_arr[0].is_number_integer()) {
            std::printf("num_arr 요소의 자료형: integer\n");
        }
        else if (num_arr[0].is_number_float()) {
            std::printf("num_arr 요소의 자료형: float\n");
        }
        else if (num_arr[0].is_string()) {
            std::printf("num_arr 요소의 자료형: string\n");
        }

        // 벡터로 옮기기
        std::vector<int> numbers;
        for (const auto& value : num_arr)
        {
            if (value.is_number_integer()) {
                numbers.push_back(value.get<int>());
            }
            else {
                std::printf("num_arr의 자료형이 일치하지 않습니다!!\n");
            }
        }

        // 벡터값 모두 출력
        for (const auto& num : numbers)
        {
            std::printf("%d, ", num);
        }
        std::printf("\n");
    }
    else
    {
        std::printf("num_arr 키가 존재하지 않거나 배열이 아닙니다.\n");
    }
}


int main()
{
    // JSON 객체 생성
    json json_object = createJson();

    // JSON 객체를 문자열로 출력
    std::string dump = json_object.dump();              // 직렬화
    std::printf("%s\n", dump.c_str());

    // JSON 파싱
    parseJson(json_object);

    return 0;
}
