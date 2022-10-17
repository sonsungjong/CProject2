#include <iostream>
using namespace std;

class String
{
	const char* m_sz;
public:
	String(const char* a_sz) : m_sz(a_sz) {}

	// 4번
	operator const char*()
	{
		return m_sz;
	}

	// 5번
	
};

int main055()
{
	// 4번
	// String s("Hello!");
	// const char* sz = s;

	// 5번
	const char* sz = "Hello!";
	String s("Hi~!");
	s = sz;

	return 0;
}