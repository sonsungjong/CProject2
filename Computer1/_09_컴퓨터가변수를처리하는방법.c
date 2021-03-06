/*
	컴퓨터가 변수를 처리하는 방법

	프로그램 메모리 주소
		1. 컴퓨터에서 프로그램이 실행되기 위해서는 프로그램이 메모리에 Load되어야 한다.
		2. 프로그램의 크기를 충당할 수 있을 만큼 메모리 공간이 확보되있어야 한다.
		3. 컴퓨터의 운영체제는 메모리 공간을 코드, 데이터, 힙, 스택 네가지 영역으로 구분하여 관리한다.
			Code Segment : 소스코드
			Data Segment : global변수, static변수
			Heap Segment : 동적 할당 변수
			Stack Segment : local변수, 매개변수

		전역변수 : 어디서든 접근 가능한 변수, main함수가 실행되기 전에 프로그램 시작과 동시에 메모리에 할당됨(Data)
		static변수 : 특정한 블록에서 접근, 전역+지역, 프로그램이 실행될때 메모리에 할당되어 프로그램이 종료될때 해제(Data)
		지역변수 : 특정한 블록에서만 접근할 수 있는 변수, 함수가 종료되면 메모리에서 해제(Stack)
		레지스터 변수 : 메인 메모리 대신 CPU의 레지스터를 사용하는 변수, 속도에 유리 register
		매개변수 : 함수를 호출할 때 함수에 필요한 데이터를 전달하기 위한 변수

		동적 == 프로그램 실행 도중
*/
