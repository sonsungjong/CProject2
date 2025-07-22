
// Boost ���̺귯�� 1.83.0 ����
/*
��ġ
https://www.boost.org/ -> Download -> boost_1_83_0.zip -> ��������
boost_1_83_0 ���� -> ������ �������� cmd ���� -> .\bootstrap.bat �Է��Ͽ� ����
.\b2.exe �Է��Ͽ� ����

���� ���丮 : C:\boost_1_83_0
���̺귯�� ���丮 : C:\boost_1_83_0\stage\lib
�ʿ��� ���̺귯���� �Է¿� �־����

���̺귯�� �Է�(������64) : libboost_json-vc143-mt-x64-1_83.lib
���̺귯�� �Է�(�����64) : libboost_json-vc143-mt-gd-x64-1_83.lib
���̺귯�� �Է�(������32) : libboost_json-vc143-mt-x32-1_83.lib
���̺귯�� �Է�(�����32) : libboost_json-vc143-mt-gd-x32-1_83.lib
*/

#include <iostream>
#include <tchar.h>
#include <vector>
#include <boost/json.hpp>

// ���̺귯�� �� // ��η� �ȵǴ� ��찡 ����
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
    boost::json::object data;               // ��ü ����
    data["number"] = 20240131;
    data["name"] = "harry kim";

    // phone ��ü �߰�
    boost::json::object phone;
    phone["kinds"] = "IPhone8";
    phone["number"] = "010-3333-4444";
    data["phone"] = phone;              // ��ü�� ��ü�� ����

    // Field �迭 �߰�
    boost::json::array field;
    field.push_back("developer");
    field.push_back("Machine Learning");
    data["Field"] = field;                  // �迭�� ��ü�� ����

    // num_arr �迭 �߰�
    boost::json::array num_arr;
    num_arr.push_back(1);
    num_arr.push_back(2);
    num_arr.push_back(3);
    num_arr.push_back(4);
    num_arr.push_back(5);
    data["num_arr"] = num_arr;              // �迭�� ��ü�� ����

    // ���ڿ��� �ٲ� ���
    std::string result = boost::json::serialize(data);
    //printf("%s\n", result.c_str());

    return data;            // JSON ����
}

/*
    boost::json::object �� ������ �м��ϰ� �Ľ��Ͽ� �����Ѵ�.
*/
void parseJson(boost::json::object a_obj)
{
    // num_arr Ű�� �迭 ã��
    boost::json::array num_arr = a_obj["num_arr"].as_array();

    // �迭 ũ�� ���
    std::size_t array_size = num_arr.size();
    printf("num_arr�� �迭 ���� : %lld\n", array_size);

    // �迭����� �ڷ��� ���
    if (num_arr[0].kind() == boost::json::kind::int64) {
        printf("num_arr ����� �ڷ��� long long\n");
    }
    else if (num_arr[0].kind() == boost::json::kind::double_) {
        printf("num_arr ����� �ڷ��� double\n");
    }
    else if (num_arr[0].kind() == boost::json::kind::uint64) {
        printf("num_arr ����� �ڷ��� unsigned long long\n");
    }
    else if (num_arr[0].kind() == boost::json::kind::string) {
        printf("num_arr ����� �ڷ��� std::string\n");
    }

    // ���ͷ� �ű��
    std::vector<long long> numbers;
    for (auto& value : num_arr)
    {
        if (value.is_int64()) {
            numbers.push_back(value.as_int64());
        }
        else {
            printf("num_arr�� �ڷ����� ��ġ���� �ʽ��ϴ�!!\n");
        }
    }

    // ���Ͱ� ��� ���
    for (auto& num : numbers)
    {
        printf("%lld, ", num);
    }
    printf("\n");

}

int main()
{
    boost::json::object json_object = createJson();                             // json ����

    printf("%s\n", boost::json::serialize(json_object).c_str());                // ���ڿ��� �ٲ� ���
    
    parseJson(json_object);             // json �Ľ�

    return 0;
}
