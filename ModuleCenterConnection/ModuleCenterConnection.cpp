/*

싱글턴패턴이 아닌 별도 모듈간에 호출을 하기 위해
서비스 로케이터 패턴 + 콜백 패턴 + 명령 패턴 + 싱글턴패턴
을 통해 등록 후 함수포인터 호출 개념으로 사용하는 예제이다.

아직 연구중 ...
*/
#include <functional>
#include <iostream>
#include <unordered_map>            // 조회 및 업데이트를 위한 map
#include <string>
#include <memory>
#include <any>

// 중앙 서비스는 싱글턴패턴으로 모든 클래스의 모든 메서드들을 등록해놓는다
// 구현 중 필요에 따라서 각 모듈의 기능에 다른 클래스의 메서드를 삽입할 수 있도록 CenterService를 통해 주입한다.

enum enumCenterService
{
    NONE = 0,
    METHOD1 = 1,
    METHOD2 = 2,
    METHOD3 = 3,
};

class CenterService
{
public:

    // 함수를 정수 키와 연결하여 저장 (반환 타입이 다양할 수 있으므로 템플릿 구성)
    template <typename T>
    void registerFunction(int key, T func)
    {
        m_functions[key] = [func](const std::any& arg) -> std::any { return std::invoke(func, arg); };
    }

    // 주어진 키에 해당하는 함수 호출
    template <typename returnType, typename ArgType>
    returnType useFunction(int key, ArgType arg)               // 함수 키 입력
    {
        auto it = m_functions.find(key);
        if (it != m_functions.end()) 
        {
            std::any result = it->second(std::any(arg));
            return std::any_cast<returnType>(result);
        }
        return returnType();                // 기본값 반환
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
    std::unordered_map<int, std::function<std::any(const std::any&)>> m_functions;
};

class Class1
{
public:
    std::string method1(int num) {
        std::string result = "0" + num;
        return result;
    }
};

class Class2
{
public:
    int method2(std::string str) {
        return 0;
    }
};

class Class3
{
public:
    void method3() {
        int num = 88;
        std::string method1_return = CenterService::getInstance().useFunction<std::string>(1, std::to_string(num));
        printf("%s\n", method1_return.c_str());
    }

    void method4(int num, double dbNum) {
        printf("기존 구성\n");
    }
};

int main()
{
    Class1 class1;
    Class2* p_class2 = new Class2;
    Class3* p_class3 = new Class3;

    CenterService::getInstance().registerFunction(1, std::function<std::string(int)>(std::bind(&Class1::method1, &class1, std::placeholders::_1)));
    CenterService::getInstance().registerFunction<std::function<int(std::string)>>(2, std::bind(&Class2::method2, p_class2, std::placeholders::_1));            // 매개변수 1개
    CenterService::getInstance().registerFunction<std::function<void(void)>>(3, std::bind(&Class3::method3, p_class3));
    CenterService::getInstance().registerFunction<std::function<void(int, double)>>(4, std::bind(&Class3::method4, p_class3, std::placeholders::_2));


    delete p_class2;
    delete p_class3;

    return 0;
}

