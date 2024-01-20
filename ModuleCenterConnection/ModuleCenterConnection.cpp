/*

�̱��������� �ƴ� ���� �ٸ� ��Ⱓ�� ������ �ϱ� ����
������ ����� ���ϰ� �߾Ӱ����� ���� �����ϱ� ����
���� �������� ���� + �ݹ� ���� + ��� ���� + �̱�������
�� ���� ��� �� �Լ������� ȣ�� �������� ����ϴ� �����̴�.

���� ������ ...
*/
#include <string>
#include <functional>
#include <iostream>
#include <unordered_map>            // ��ȸ �� ������Ʈ�� ���� map
#include <memory>
#include <any>
#include <tuple>
#include <variant>
#include <chrono>
#include <thread>               // ��ȯ���� ������ ��Ƽ������
#include <future>

class CenterService
{
public:
    // �߾Ӽ��� �Լ� ���ι�ȣ
    enum enumCenterService
    {
        NONE = 0,
        METHOD1 = 1,
        METHOD2 = 2,
        METHOD3 = 3,
        METHOD4 = 4,
    };

    // ��ϰ����� �Լ� �ڷ���
    using FuncVariant = 
        std::variant<
            std::function<std::string(int)>,
            std::function<int(std::string)>,
            std::function<void(void)>,
            std::function<void(int, double)>
        >;

    // �Լ� ���
    void registerFunction(int key, const FuncVariant& func)
    {
        m_functions[key] = func;
    }

    // �Լ� ��� (�����ε�)
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

    // ��Ƽ������� �Լ� ��� (���ϰ����� ������и��ؼ� ���)
    void threadFunction(int key)
    {
        auto typeFunc = std::get<std::function<void()>>(m_functions[key]);
        std::thread t(typeFunc);
        t.detach();         // ������ �и�
    }

    // �񵿱�� �Լ� ��� (future�� ��ȯ���� ������� �ʴ��� �ݵ�� ������ �����Ͽ� ���Ϲ��� ��!!(������ ����), ��ȯ���� ����Ϸ��� ȣ�� �� ���ϰ����� get()���� �޾ƿ;���)
    std::future<std::string> asyncFunction(int key, int num)
    {
        auto typeFunc = std::get<std::function<std::string(int)>>(m_functions[key]);
        return std::async(std::launch::async, typeFunc, num);
    }

    // �񵿱�� �Լ� ��� (future�� ��ȯ���� ������� �ʴ��� �ݵ�� ������ �����Ͽ� ���Ϲ��� ��!!(������ ����), ��ȯ���� ����Ϸ��� ȣ�� �� ���ϰ����� get()���� �޾ƿ;���)
    std::future<int> asyncFunction(int key, std::string str)
    {
        auto typeFunc = std::get<std::function<int(std::string)>>(m_functions[key]);
        return std::async(std::launch::async, typeFunc, str);
    }

    // ��Ƽ������� �Լ� ��� (���ϰ����� ������и��ؼ� ���)
    void threadFunction(int key, int num, double dbNum)
    {
        auto typeFunc = std::get<std::function<void(int, double)>>(m_functions[key]);
        std::thread t([typeFunc, num, dbNum]() {
            typeFunc(num, dbNum);
        });
        t.detach();
    }

    // �̱��ϱ���
    static CenterService& getInstance() 
    {
        static CenterService instance;
        return instance;
    }
    CenterService(const CenterService&) = delete;
    void operator=(const CenterService&) = delete;
private:
    CenterService() {}          // �̱��ϱ���

private:
    FuncVariant m_functions[65535];                       // �Լ��� ���� �������
};

class Class1
{
public:
    std::string method1(int num) {
        std::string result = "method1���ϰ�: " + std::to_string(num);
        printf("method1 ��� : %d\n", num);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        printf("method1 ��\n");
        return result;
    }
};

class Class2
{
public:
    int method2(std::string str) {
        printf("method2 ��� : %s\n", str.c_str());
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
            std::string wait_method1_result = "�ȱ�ٸ�";
            auto method1_return = CenterService::getInstance().asyncFunction(1, 334455);               // ���� �� �ٸ� ����� �޼��带 �߾� ���񽺷� ��ħ
            wait_method1_result = method1_return.get();                 // future : ������ ��ٸ��� (�� �κ�(get)�� �����ϸ� �ȱ�ٸ��� ���ϰ� ����)
            printf("method3 ��� : %s\n", wait_method1_result.c_str());
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

    }

    void method4(int num, double dbNum) {
        printf("method4 ��� : %d, %lf \n", num, dbNum);
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
    
    auto method1_result = service.asyncFunction(CenterService::METHOD1, 9999);                            // �񵿱�(���ϰ� get������)
    int result2 = service.useFunction(CenterService::METHOD2, str);                 // ����
    
    printf("main ���� ��� : %d\n", result2);
    service.threadFunction(CenterService::NONE);                               // �߰�������
    service.threadFunction(CenterService::METHOD3);                        // �߰�������
    service.useFunction(CenterService::METHOD4, 444, 4.14);             // ����

    while (true) {
        printf("====main thread====\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
    }

    delete p_class3;
    delete p_class1;
}

