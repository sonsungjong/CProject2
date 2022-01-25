// union 공용체 : 구조체보다 메모리를 절약할 수 있음
#include <iostream>
using namespace std;

int main0606() {
	struct Product0
	{
		int idType;
		int idInteger;
		char idChars[10];
	};

	union ID 
	{
		int idType;
		int integer;
		char chars[10];
	};

	struct Product1
	{
		int idType;
		ID id;
	};

	Product0 p0 = { 0, 12 };
	if (p0.idType == 0)
		cout << p0.idInteger << endl;
	else
		cout << p0.idChars << endl;

	Product1 p1 = { 1, {.chars = "abc"} };
	if (p1.idType == 0)
		cout << p1.id.integer << endl;
	else
		cout << p1.id.chars << endl;

	return 0;
}