#include <iostream>

using namespace std;

class Mobile
{
protected:
	const wchar_t* name;
	int number;
public:
	Mobile(const wchar_t* ap_name, int a_number) {
		this->name = ap_name;
		this->number = a_number;
	}
	~Mobile() {

	}
	virtual int Call(int a_callNumber) { return 0; }

	virtual int Msg(int a_callNumber, const wchar_t* a_msgContent) { return 0; }
};

class FastMobile : public Mobile
{
protected:

public:
	FastMobile(const wchar_t* ap_name, int a_number) : Mobile(ap_name, a_number){

	}
	~FastMobile() {

	}
	int Call(int a_callNumber) {
		wcout << number << L"���� " << a_callNumber << L"���� ������ ��ȭ ��" << endl;
		return 1;
	}
	int Msg(int a_callNumber, const wchar_t* a_msgContent) {
		wcout << number << L"���� " << a_callNumber << L"�Կ��� ���� : " << a_msgContent << endl;
		return 1;
	}
};

class SmartMobile : public Mobile
{
protected:

public:
	SmartMobile(const wchar_t* ap_name, int a_number) : Mobile(ap_name, a_number) {}
	~SmartMobile() {}

	int Call(int a_callNumber) {
		wcout << number << L"���� " << a_callNumber << L"���� ����Ʈ�ϰ� ��ȭ ��" << endl;
		return 1;
	}
	int Msg(int a_callNumber, const wchar_t* a_msgContent) {
		wcout << number << L"���� " << a_callNumber << L"�Կ��� ī�� : " << a_msgContent << endl;
		return 1;
	}
};

#define MAX_COUNT		32

int main55() {
	setlocale(LC_ALL, "ko_KR.utf8");

	Mobile* p_mobile_list[MAX_COUNT];
	int select, type, num;
	int mobile_count = 0;
	wchar_t name[32];

	while (1) {
		wcout << endl << L"[  �޴�  ]" << endl;
		wcout << L"1. ����" << endl;
		wcout << L"2. ��ȭ" << endl;
		wcout << L"3. ����" << endl;
		wcout << L"4. ����" << endl;

		wcout << L"��� ���� : " << endl;
		cin >> select;

		if (select == 1) {
			wcout << L"1: ������, 2: ����Ʈ�� ? ";
			cin >> type;
			if (type == 1) {
				p_mobile_list[mobile_count] = new FastMobile(L"�ռ���", 2580);
				mobile_count++;
			}
			else if (type == 2) {
				p_mobile_list[mobile_count] = new SmartMobile(L"ȫ�浿", 1234);
				mobile_count++;
			}
			
		}
		else if (select == 2) {
			for (int i = 0; i < mobile_count; i++)
			{
				p_mobile_list[i]->Call(8282);
			}
		}
		else if (select == 3) {
			for (int i = 0; i < mobile_count; i++)
			{
				p_mobile_list[i]->Msg(8282, L"�ݰ����ϴ�.");
			}
		}
		else if (select == 4) {
			break;
		}
	}

	for (int i = 0; i < mobile_count; i++)
	{
		delete p_mobile_list[i];
	}

	return 0;
}