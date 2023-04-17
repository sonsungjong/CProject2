// Singleton ��ü���� ���� (������, ���뼺)
#pragma once
#include <tchar.h>

class Singleton
{
public:
	static Singleton& getInstance() {
		static Singleton instance;			// �������� �̱��� �ν��Ͻ� ����
		return instance;
	}

	// ��������ڿ� ���Կ����ڸ� ����
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

private:
	// �����ڸ� private���� ����� ��ü ������ ����
	Singleton() {}


// ��������� �޼���
private:
	int num;
	TCHAR str[256];

public:
	void setNum(int num) {
		this->num = num;
	}

	int getNum() {
		return num;
	}

	void setStr(const TCHAR* str) {
		memset(this->str, 0, sizeof(this->str));

		// ������ ũ��� ����
		int input_len = _tcslen(str);
		int safe_len = 0;

		if (input_len < (sizeof(this->str)/sizeof((this->str)[0])) - 1) {
			safe_len = input_len;
		}
		else {
			// ũ�� �ʰ���
			safe_len = (sizeof(this->str) / sizeof((this->str)[0])) - 1;
		}

		memcpy(this->str, str, safe_len * sizeof(TCHAR));
		(this->str)[safe_len] = _T('\0');			// ���� NULL���� �߰�
	}

	const TCHAR* getStr() {
		return str;
	}

};

