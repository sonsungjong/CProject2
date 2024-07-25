#pragma once

extern int g_nGlobalVar;			// 전역변수 선언만 함
void memoryRegion();			// 전역함수

static void staticFunction();			// 정적함수 (해당 파일에서만 사용 가능)

class CStatic
{
	static int ms_num;				// 정적멤버변수 선언만 함
public:
	void memberFunction();

	static void staticMemberFunction();			// 정적 멤버 함수는 멤버변수를 사용할 수 없고 정적멤버변수를 사용할 수 있다
	// 객체없이 사용되기 때문에 멤버변수가 생성되지 않음
};
