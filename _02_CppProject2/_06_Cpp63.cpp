#include <iostream>

using namespace std;

void App(void (*afp_user)());
void MyUser();
// 원형을 통일 -> 함수포인터를 배열로 나열할 수 있게됨
void OnLButtonDown(int wParam, int lParam);
void OnLButtonUp(int wParam, int lParam);
void OnMouseMove(int wParam, int lParam);

//#define OnLButtonDown			0
//#define OnLButtonUp				1
//#define OnMouseMove				2

// Stream
int main63() {
	cout.flags(ios::left);		// 왼쪽 정렬
	cout.width(8);				// 8칸 확보
	cout << "hello";
	cout.flags(ios::right);		// 오른쪽 정렬
	cout.width(5);				// 5칸 확보
	cout << "world" << endl;

	void (*p_msg_proc[3])(int, int) = { OnLButtonDown, OnLButtonUp, OnMouseMove};
	int msg_id;

	(*p_msg_proc[msg_id])(2, 3);

	App(MyUser);

	return 0;
}

/*
	Stream : 방향성이 있는 데이터 (입출력)
	데이터의 흐름, 이동하는 데이터
	iostream 클래스로 통합해서 입출력을 관리

	콘솔 : 50년간 명령어를 OS간 통일시키기 위해 사용한 가짜함수 malloc, new
	OS API : OS종속적인 명령어 heapalloc (windows)
	
	리눅스 서브 시스템(우분투)

	API : OS 지원 함수
	SDK : 개발환경
	IDE : 통합개발환경

	H : 핸들
	함수포인터 : 반제품 제작을 위한 C언어기법, 사용하는 곳에서 함수를 매개변수로 넘김 (함수명을 미리 정해놓을 수 없음, OS컴파일 에러 <-> 사용자가 기능을 넣을 수 없음)

	C#이나 python에서 API호출?
	운영체제C (콘솔로 제작?) -> API -> malloc?
	32비트와 64비트 크기변경되는 자료형
	
	windows : 메시지구조 (번호전달)
	다른OS : 질의구조

	WM_ : 윈도우메시지
	WM_LBUTTONDOWN : 왼쪽버튼 눌림
	WM_LBUTTONUP : 왼쪽버튼 떨어짐

	wParam : word
	lParam : long

	ON : 메시지 기능을 처리하는 함수에 붙이는 접두어
	Event : On/Off (가벼움)
	Message : On/Off + 부가정보

	영상처리 (비전 + 그래픽)
	GDI : 그래픽 디바이스 인터페이스 (32비트의 가상 논리그래픽)
	DC : 매개변수를 줄이기 위해서 잘 안바뀌는 그리기속성을 구조체로 만든 것	(Pen + Brush + Font + ...)	 , Device Context
	Pen : DC구조체 안에 있는 선속성 구조체
	Brush : DC구조체 안에 있는 면속성 구조체 (채우기)

	HDC, HPen, HBrush, HFont

*/

void App(void (*afp_user)())			// 함수포인터
{
	// Init();		// 반제품

	if(afp_user != NULL)
	(*afp_user)();			// 나중에 사용자가 만들 함수를 매개변수 형식으로 받음 : 함수명을 미리 정해놓으면 컴파일이 안되고, 정해놓으면 사용자가 해당 기능을 추가할 수 없음

	// Delete();			// 반제품
}

void MyUser()
{

}

void OnLButtonDown(int wParam, int lParam) 
{

}
void OnLButtonUp(int wParam, int lParam)
{

}
void OnMouseMove(int wParam, int lParam) 
{

}