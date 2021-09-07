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
		wcout << number << L"님이 " << a_callNumber << L"에게 빠르게 통화 중" << endl;
		return 1;
	}
	int Msg(int a_callNumber, const wchar_t* a_msgContent) {
		wcout << number << L"님이 " << a_callNumber << L"님에게 문자 : " << a_msgContent << endl;
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
		wcout << number << L"님이 " << a_callNumber << L"에게 스마트하게 통화 중" << endl;
		return 1;
	}
	int Msg(int a_callNumber, const wchar_t* a_msgContent) {
		wcout << number << L"님이 " << a_callNumber << L"님에게 카톡 : " << a_msgContent << endl;
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
		wcout << endl << L"[  메뉴  ]" << endl;
		wcout << L"1. 구매" << endl;
		wcout << L"2. 통화" << endl;
		wcout << L"3. 문자" << endl;
		wcout << L"4. 종료" << endl;

		wcout << L"기능 선택 : " << endl;
		cin >> select;

		if (select == 1) {
			wcout << L"1: 폴더폰, 2: 스마트폰 ? ";
			cin >> type;
			if (type == 1) {
				p_mobile_list[mobile_count] = new FastMobile(L"손성종", 2580);
				mobile_count++;
			}
			else if (type == 2) {
				p_mobile_list[mobile_count] = new SmartMobile(L"홍길동", 1234);
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
				p_mobile_list[i]->Msg(8282, L"반갑습니다.");
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