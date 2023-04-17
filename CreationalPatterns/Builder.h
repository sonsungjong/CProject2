// Builder ��ü���� ���� (������)
#pragma once
#include <tchar.h>

class Builder
{
private:
	// �����ڸ� private ���� ����
	Builder(int num, const TCHAR* str) : num(num)
	{
		memset(this->str, 0, (sizeof(this->str) / sizeof(this->str[0])));
		_tcsncpy_s(this->str, str, (sizeof(this->str) / sizeof(this->str[0])) -1 );
	}

	// ��ü�� ������ Ŭ������ friend�� �����Ͽ� ���� ���
	friend class BuilderCreational;


// ��������� �޼���
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
	// ������ ������ ���� Ŭ���� ��ü�� �����ϴ� �Լ�
	Builder build() {
		return Builder(num, str);
	}
private:
	// ���� Ŭ������ �������
	int num;
	TCHAR str[256];

// ��������� �޼���
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