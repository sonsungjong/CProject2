/*
CPU : Central Processing Unit, 명령을 해석해서 부품을 제어하는 장치
	- 연산장치 + 제어장치(중심) + 레지스터(CPU내부연산메모리) + 외부인터페이스(통신)

1. 연산장치 : 산술연산을 하는 장치, 정수연산과 논리연산을 하는 ALU + 실수연산을 하는 FPU
2. 제어장치 : 명령을 해석해서 동작할 수 있도록 CPU내부 혹은 외부에 있는 장치를 제어하는 장치
3. 레지스터 : CPU에서 작업하기 위한 값이나 상태를 저장할 때 사용하는 CPU내부메모리
4. 외부 인터페이스 : CPU내부 장치들이 CPU외부의 장치들과 데이터 또는 제어신호를 주고받게하는 장치
	- CPU내부: 빠름, CPU외부 : 느림

<CPU 동작 순서>
(1) 제어장치가 현재 처리할 명령어를 메모리에서 가져옴
(2) 제어 장치가 명령어를 해석
(3) 단순 명령일 때
	- 값읽기 : 메모리값을 읽어 레지스터에 저장
    - 값저장 : 레지스터의 값을 메모리에 저장
	- 연산명령: 레지스터에 저장된 값을 연산 장치를 사용해서 연산하고 결과를 레지스터에 다시 저장
*/