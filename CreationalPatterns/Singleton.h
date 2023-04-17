// Singleton 객체생성 패턴 (유연성, 재사용성)
#pragma once
#include <tchar.h>

class Singleton
{
public:
	static Singleton& getInstance() {
		static Singleton instance;			// 정적으로 싱글톤 인스턴스 생성
		return instance;
	}

	// 복사생성자와 대입연산자를 삭제
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

private:
	// 생성자를 private으로 만들어 객체 생성을 방지
	Singleton() {}


// 멤버변수와 메서드
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

		// 안전한 크기로 복사
		int input_len = _tcslen(str);
		int safe_len = 0;

		if (input_len < (sizeof(this->str)/sizeof((this->str)[0])) - 1) {
			safe_len = input_len;
		}
		else {
			// 크기 초과시
			safe_len = (sizeof(this->str) / sizeof((this->str)[0])) - 1;
		}

		memcpy(this->str, str, safe_len * sizeof(TCHAR));
		(this->str)[safe_len] = _T('\0');			// 끝에 NULL문자 추가
	}

	const TCHAR* getStr() {
		return str;
	}

};

