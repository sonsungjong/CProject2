#include <iostream>

using namespace std;

int main56() {
	setlocale(LC_ALL, "ko_KR.utf8");

	const wchar_t* name = L"¾Æ³ç";

	wcout << name << endl;
	return 0;
}