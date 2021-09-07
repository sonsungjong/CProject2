#include <iostream>

using namespace std;

class String
{
protected:
	const char* m_str;
public:
	String(const char* str) { this->m_str = str; }
	
	operator const char* () {
		return m_str;
	}
};


int main13()
{
	String s("Hello!");
	const char* sz = s;

	cout << sz << endl;

	return 0;
}