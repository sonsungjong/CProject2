/*
	●API : 운영체제에서 제공하는 함수의 집합체, Application Programming Interface
	●SDK : 개발에 필요한 소프트웨어 개발 키트, Software Development Kit
	●IDE : 통합개발환경, API + SDK, (visual studio)

	●Handle : 리소스변경 요청 시 해킹의 위험 방지
		1. 원하는 리소스 사용 요청
		2. 요청한 리소스의 메모리 주소 확인
		3. 핸들 테이블에 리소스의 주소를 기록하고 함께 사용할 핸들 값(임의의 주소값) 생성
		4. 핸들 값만 알려줌(안심번호)
		5. 핸들 값 전달
		6. 사용자는 100이라는 핸들값을 사용해 API함수로 전달
		
		2차 포인터와 유사하나 값을 상수로 사용

	+과부화된 리소스를 대신해 다른 리소스의 주소값으로 바꿔 사용할 수 있음 (은행의 번호표)
	typedef void* HANDLE;

	●HINSTANCE : HANDLE + Instance, instance handle을 저장, 프로세스 속 리소스접근용
	typedef HANDLE HINSTANCE;
	typedef HANDLE HFONT;
	typedef HANDLE HPEN;

	h_pen = h_font 같은 잘못된 값을 복사하려는 상황을 막기위해 HANDLE을 세분화

	.c ->컴파일-> .obj ->링크-> .exe
	리소스 : 아이콘, 글꼴, 이미지, 커서 등

	.exe파일만 가져가서 실행시킬 수 있도록 리소스를 실행파일 안에 넣음
	.rc ->컴파일 -> .res -> .res+.exe -> 윈도우용 실행파일.exe

	exe -> 실행 -> process (HPROCESS)
	HPROCESS를 사용하기엔 부담스럽기 떄문에
	HINSTANCE 로 .rc리소스 쪽에만 접근할 수 있는 핸들을 따로 만들음

*/