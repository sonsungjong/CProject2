/*

싱글턴패턴이 아닌 서로 다른 모듈간에 결합을 하기 위해
생성자 사용을 피하고 중앙관리를 통해 결합하기 위해
서비스 로케이터 패턴 + 콜백 패턴 + 명령 패턴 + 싱글턴패턴
을 통해 등록 후 함수포인터 호출 개념으로 사용하는 예제이다.

아직 연구중 ...
*/
#include <string>
#include <functional>
#include <iostream>
#include <unordered_map>            // 조회 및 업데이트를 위한 map
#include <memory>
#include <any>
#include <tuple>
#include <variant>

// 중앙 서비스는 싱글턴패턴으로 모든 클래스의 모든 메서드들을 등록해놓는다
// 구현 중 필요에 따라서 각 모듈의 기능에 다른 클래스의 메서드를 삽입할 수 있도록 CenterService를 통해 주입한다.

enum enumCenterService
{
    NONE = 0,
    METHOD1 = 1,
    METHOD2 = 2,
    METHOD3 = 3,
    METHOD4 = 4,
};


class CenterService
{
public:
    using T = std::variant <
        std::function<std::string(int)>,
        std::function<int(std::string)>,
        std::function<void(void)>,
        std::function<void(int, double)>
    >;

    void registerFunction(int key, std::function<std::string(int)> func)
    {
        m_functions[key] = func;
    }

    void registerFunction(int key, std::function<int(std::string)> func)
    {
        m_functions[key] = func;
    }

    void registerFunction(int key, std::function<void(void)> func)
    {
        m_functions[key] = func;
    }

    //void registerFunction(int key, std::function<void(int, double)> func)
    //{
    //    m_functions[key] = func;
    //}

    std::string useFunction(int key, int num)
    {
        auto it = m_functions.find(key);
        if (it != m_functions.end())
        {
            auto& func = it->second;
            if (auto strFunc = std::get_if<std::function<std::string(int)>>(&func))
            {
                return (*strFunc)(num);
            }
        }
        return "";
    }

    int useFunction(int key, std::string str)
    {
        auto it = m_functions.find(key);
        auto& func = it->second;
        if (auto intFunc = std::get_if<std::function<int(std::string)>>(&func))
        {
            return (*intFunc)(str);
        }
        return 0;
    }

    void useFunction(int key)
    {
        auto it = m_functions.find(key);
        auto& func = it->second;
        auto voidFunc = std::get_if<std::function<void(void)>>(&func);
        (*voidFunc)();
    }

    void useFunction(int key, int num, double dbNum)
    {
        auto it = m_functions.find(key);
        auto& func = it->second;
        auto voidFunc = std::get_if<std::function<void(int, double)>>(&func);
        (*voidFunc)(num, dbNum);
    }

    // 싱글턴구성
    static CenterService& getInstance() 
    {
        static CenterService instance;
        return instance;
    }
    CenterService(const CenterService&) = delete;
    void operator=(const CenterService&) = delete;
private:
    CenterService() {}          // 싱글턴구성

private:
    std::unordered_map<int, T> m_functions;
};

class Class1
{
public:
    std::string method1(int num) {
        std::string result = "0" + std::to_string(num);
        printf("method1 사용 : %s\n", result.c_str());
        return result;
    }
};

class Class2
{
public:
    int method2(std::string str) {
        printf("method2 사용 %s\n", str.c_str());
        return 2;
    }
};

class Class3
{
public:
    void method3() {
        int num = 88;
        std::string method1_return = CenterService::getInstance().useFunction(1, 334455);
        printf("method3 사용 : %s\n", method1_return.c_str());
    }

    void method4(int num, double dbNum) {
        printf("method4 사용 \n");
    }
};

int main()
{
    Class1* p_class1 = new Class1;
    Class2* p_class2 = new Class2;
    Class3* p_class3 = new Class3;

    CenterService& service = CenterService::getInstance();

    service.registerFunction(1, std::bind(&Class1::method1, p_class1, std::placeholders::_1));
    service.registerFunction(2, std::bind(&Class2::method2, p_class2, std::placeholders::_1));
    service.registerFunction(3, std::bind(&Class3::method3, p_class3));
    //service.registerFunction(4, std::bind(&Class3::method4, p_class3, std::placeholders::_1, std::placeholders::_2));

    std::string str = "hello world";

    std::string result1 = service.useFunction(1, 123123);
    int result2 = service.useFunction(2, str);
    service.useFunction(3);
    //service.useFunction(4, 444, 4.14);

    delete p_class3;
    delete p_class2;
    delete p_class1;
}

