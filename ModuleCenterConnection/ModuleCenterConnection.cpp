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
#include <chrono>
#include <thread>               // 반환값이 없을때 멀티쓰레딩
#include <future>

class CenterService
{
public:
    // 중앙서비스 함수 매핑번호
    enum enumCenterService
    {
        NONE = 0,
        METHOD1 = 1,
        METHOD2 = 2,
        METHOD3 = 3,
        METHOD4 = 4,
    };

    // 등록가능한 함수 자료형
    using FuncVariant = 
        std::variant<
            std::function<std::string(int)>,
            std::function<int(std::string)>,
            std::function<void(void)>,
            std::function<void(int, double)>
        >;

    // 함수 등록
    void registerFunction(int key, const FuncVariant& func)
    {
        m_functions[key] = func;
    }

    // 함수 사용 (오버로딩)
    void useFunction(int key)
    {
        auto typeFunc = std::get<std::function<void(void)>>(m_functions[key]);
        return typeFunc();
    }

    std::string useFunction(int key, int num)
    {
        auto typeFunc = std::get<std::function<std::string(int)>>(m_functions[key]);
        return typeFunc(num);
    }

    int useFunction(int key, std::string str)
    {
        auto typeFunc = std::get<std::function<int(std::string)>>(m_functions[key]);
        return typeFunc(str);
    }

    void useFunction(int key, int num, double dbNum)
    {
        auto typeFunc = std::get<std::function<void(int, double)>>(m_functions[key]);
        return typeFunc(num, dbNum);
    }

    // 멀티쓰레드로 함수 사용 (리턴값없이 쓰레드분리해서 사용)
    void threadFunction(int key)
    {
        auto typeFunc = std::get<std::function<void()>>(m_functions[key]);
        std::thread t(typeFunc);
        t.detach();         // 스레드 분리
    }

    // 비동기로 함수 사용 (future는 반환값을 사용하지 않더라도 반드시 변수를 선언하여 리턴받을 것!!(쓰레드 멈춤), 반환값을 사용하려면 호출 후 리턴값에서 get()으로 받아와야함)
    std::future<std::string> asyncFunction(int key, int num)
    {
        auto typeFunc = std::get<std::function<std::string(int)>>(m_functions[key]);
        return std::async(std::launch::async, typeFunc, num);
    }

    // 비동기로 함수 사용 (future는 반환값을 사용하지 않더라도 반드시 변수를 선언하여 리턴받을 것!!(쓰레드 멈춤), 반환값을 사용하려면 호출 후 리턴값에서 get()으로 받아와야함)
    std::future<int> asyncFunction(int key, std::string str)
    {
        auto typeFunc = std::get<std::function<int(std::string)>>(m_functions[key]);
        return std::async(std::launch::async, typeFunc, str);
    }

    // 멀티쓰레드로 함수 사용 (리턴값없이 쓰레드분리해서 사용)
    void threadFunction(int key, int num, double dbNum)
    {
        auto typeFunc = std::get<std::function<void(int, double)>>(m_functions[key]);
        std::thread t([typeFunc, num, dbNum]() {
            typeFunc(num, dbNum);
        });
        t.detach();
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
    FuncVariant m_functions[65535];                       // 함수를 담을 멤버변수
};

class Class1
{
public:
    std::string method1(int num) {
        std::string result = "method1리턴값: " + std::to_string(num);
        printf("method1 사용 : %d\n", num);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        printf("method1 끝\n");
        return result;
    }
};

class Class2
{
public:
    int method2(std::string str) {
        printf("method2 사용 : %s\n", str.c_str());
        return 2;
    }
};

class Class3
{
public:
    void method3() {
        while (true) 
        {
            int num = 88;
            std::string wait_method1_result = "안기다림";
            auto method1_return = CenterService::getInstance().asyncFunction(1, 334455);               // 구현 시 다른 모듈의 메서드를 중앙 서비스로 합침
            wait_method1_result = method1_return.get();                 // future : 리턴을 기다린다 (이 부분(get)을 제거하면 안기다리고 리턴값 무시)
            printf("method3 사용 : %s\n", wait_method1_result.c_str());
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    }

    void method4(int num, double dbNum) {
        printf("method4 사용 : %d, %lf \n", num, dbNum);
    }
};

void globalPrint()
{
    while (true) {
        printf("global function!\n");
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

int main()
{
    Class1* p_class1 = new Class1;
    Class2 class2;
    Class3* p_class3 = new Class3;

    CenterService& service = CenterService::getInstance();

    service.registerFunction(CenterService::METHOD2, std::function<int(std::string)>(std::bind(&Class2::method2, &class2, std::placeholders::_1)));
    service.registerFunction(CenterService::METHOD1, std::function<std::string(int)>(std::bind(&Class1::method1, p_class1, std::placeholders::_1)));
    service.registerFunction(CenterService::METHOD3, std::function<void(void)>(std::bind(&Class3::method3, p_class3)));
    service.registerFunction(CenterService::METHOD4, std::function<void(int, double)>(std::bind(&Class3::method4, p_class3, std::placeholders::_1, std::placeholders::_2)));
    service.registerFunction(CenterService::NONE, std::function<void(void)>(globalPrint));
    
    std::string str = "hello world";
    
    auto method1_result = service.asyncFunction(CenterService::METHOD1, 9999);                            // 비동기(리턴값 get사용안함)
    int result2 = service.useFunction(CenterService::METHOD2, str);                 // 동기
    
    printf("main 리턴 결과 : %d\n", result2);
    service.threadFunction(CenterService::NONE);                               // 추가쓰레드
    service.threadFunction(CenterService::METHOD3);                        // 추가쓰레드
    service.useFunction(CenterService::METHOD4, 444, 4.14);             // 동기

    while (true) {
        printf("====main thread====\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }

    delete p_class3;
    delete p_class1;
}

