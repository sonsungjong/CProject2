/*

�̱��������� �ƴ� ���� ��Ⱓ�� ȣ���� �ϱ� ����
���� �������� ���� + �ݹ� ���� + ��� ���� + �̱�������
�� ���� ��� �� �Լ������� ȣ�� �������� ����ϴ� �����̴�.

���� ������ ...
*/
#include <functional>
#include <iostream>
#include <unordered_map>            // ��ȸ �� ������Ʈ�� ���� map
#include <string>
#include <memory>
#include <any>

// �߾� ���񽺴� �̱����������� ��� Ŭ������ ��� �޼������ ����س��´�
// ���� �� �ʿ信 ���� �� ����� ��ɿ� �ٸ� Ŭ������ �޼��带 ������ �� �ֵ��� CenterService�� ���� �����Ѵ�.

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

    // �Լ��� ���� Ű�� �����Ͽ� ���� (��ȯ Ÿ���� �پ��� �� �����Ƿ� ���ø� ����)
    template <typename T>
    void registerFunction(int key, T func)
    {
        m_functions[key] = [func](const std::any& arg) -> std::any { return std::invoke(func, arg); };
    }

    // �־��� Ű�� �ش��ϴ� �Լ� ȣ��
    template <typename returnType, typename ArgType>
    returnType useFunction(int key, ArgType arg)               // �Լ� Ű �Է�
    {
        auto it = m_functions.find(key);
        if (it != m_functions.end()) 
        {
            std::any result = it->second(std::any(arg));
            return std::any_cast<returnType>(result);
        }
        return returnType();                // �⺻�� ��ȯ
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
        printf("���� ����\n");
    }
};

int main()
{
    Class1 class1;
    Class2* p_class2 = new Class2;
    Class3* p_class3 = new Class3;

    CenterService::getInstance().registerFunction(1, std::function<std::string(int)>(std::bind(&Class1::method1, &class1, std::placeholders::_1)));
    CenterService::getInstance().registerFunction<std::function<int(std::string)>>(2, std::bind(&Class2::method2, p_class2, std::placeholders::_1));            // �Ű����� 1��
    CenterService::getInstance().registerFunction<std::function<void(void)>>(3, std::bind(&Class3::method3, p_class3));
    CenterService::getInstance().registerFunction<std::function<void(int, double)>>(4, std::bind(&Class3::method4, p_class3, std::placeholders::_2));


    delete p_class2;
    delete p_class3;

    return 0;
}

