#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

typedef struct MyStruct {
	char name[20];
	int age;
} MS;

int information(MS* no) {
	printf("이름 : %s \n", (*no).name);
	printf("나이 : %d \n", (*no).age);
}

int modify(MS* no) {
	printf("%s의 이름 수정 시작>>", (*no).name);
	scanf("%s", (*no).name);
}

int main1802() {
	MS no1 = {"김철수", 18};
	MS no2 = {"신짱구", 18};	
	//no1.name = "김치맨";
	strcpy(no1.name, "김치맨");
	
	printf("%s \n", no1.name);
	printf("이름을 입력하세요>>");
	//scanf("%s", no1.name);
	scanf_s("%s",no1.name, sizeof(no1.name));

	information(&no1);

	modify(&no1);

	information(&no1);
	return 0;
}
