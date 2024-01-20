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

class CenterService
{
public:
    enum enumCenterService
    {
        NONE = 0,
        METHOD1 = 1,
        METHOD2 = 2,
        METHOD3 = 3,
        METHOD4 = 4,
    };

    void registerFunction(int key, const std::function<std::string(int)> func)
    {
        m_getStr_setInt[key] = func;
    }

    void registerFunction(int key, const std::function<int(std::string)> func)
    {
        m_getInt_setStr[key] = func;
    }

    void registerFunction(int key, std::function<void(void)> func)
    {
        m_getVoid_setVoid[key] = func;
    }

    void registerFunction(int key, std::function<void(int, double)> func)
    {
        m_getVoid_setIntDouble[key] = func;
    }

    std::string useFunction(int key, int num)
    {
        return m_getStr_setInt[key](num);
    }

    int useFunction(int key, std::string str)
    {
        return m_getInt_setStr[key](str);
    }

    void useFunction(int key)
    {
        return m_getVoid_setVoid[key]();
    }

    void useFunction(int key, int num, double dbNum)
    {
        return m_getVoid_setIntDouble[key](num, dbNum);
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
    std::function<std::string(int)> m_getStr_setInt[255];
    std::function<int(std::string)> m_getInt_setStr[255];
    std::function<void(void)> m_getVoid_setVoid[255];
    std::function<void(int, double)> m_getVoid_setIntDouble[255];

    
};

class Class1
{
public:
    std::string method1(int num) {
        std::string result = "0" + std::to_string(num);
        printf("method1 ��� : %s\n", result.c_str());
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
        int num = 88;
        std::string method1_return = CenterService::getInstance().useFunction(1, 334455);
        printf("method3 ��� : %s\n", method1_return.c_str());
    }

    void method4(int num, double dbNum) {
        printf("method4 ��� : %d, %lf \n", num, dbNum);
    }
};

int main()
{
    Class1* p_class1 = new Class1;
    Class2 class2;
    Class3* p_class3 = new Class3;

    CenterService& service = CenterService::getInstance();

    service.registerFunction(CenterService::METHOD1, std::function<std::string(int)>(std::bind(&Class1::method1, p_class1, std::placeholders::_1)));
    service.registerFunction(CenterService::METHOD2, std::function<int(std::string)>(std::bind(&Class2::method2, &class2, std::placeholders::_1)));
    service.registerFunction(CenterService::METHOD3, std::function<void(void)>(std::bind(&Class3::method3, p_class3)));
    service.registerFunction(CenterService::METHOD4, std::function<void(int, double)>(std::bind(&Class3::method4, p_class3, std::placeholders::_1, std::placeholders::_2)));

    std::string str = "hello world";

    std::string result1 = service.useFunction(CenterService::METHOD1, 123123);
    int result2 = service.useFunction(CenterService::METHOD2, str);

    printf("main ���� ��� : %s, %d\n", result1.c_str(), result2);

    service.useFunction(CenterService::METHOD3);
    service.useFunction(CenterService::METHOD4, 444, 4.14);

    delete p_class3;
    delete p_class1;
}

