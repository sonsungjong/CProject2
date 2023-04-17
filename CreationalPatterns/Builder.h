// Builder 객체생성 패턴 (유연성)
#pragma once
#include <tchar.h>

class Builder
{
private:
	// 생성자를 private 으로 설정
	Builder(int num, const TCHAR* str) : num(num)
	{
		memset(this->str, 0, (sizeof(this->str) / sizeof(this->str[0])));
		_tcsncpy_s(this->str, str, (sizeof(this->str) / sizeof(this->str[0])) -1 );
	}

	// 객체를 생성할 클래스를 friend로 설정하여 접근 허용
	friend class BuilderCreational;


// 멤버변수와 메서드
private:
	int num;
	TCHAR str[256];
public:
	void print(){
		_tprintf(_T("num: %d, str: %s\n"), num, str);
	}
};

class BuilderCreational
{
public:
	// 설정된 값으로 원본 클래스 객체를 생성하는 함수
	Builder build() {
		return Builder(num, str);
	}
private:
	// 원본 클래스의 멤버변수
	int num;
	TCHAR str[256];

// 멤버변수와 메서드
public:
	BuilderCreational& setNum(int num) {
		this->num = num;
		return *this;
	}

	BuilderCreational& setStr(const TCHAR* str) {
		memset(this->str, 0, (sizeof(this->str) / sizeof(this->str[0])));
		_tcsncpy_s(this->str, str, (sizeof(this->str) / sizeof(this->str[0])) - 1);
		return *this;
	}

};