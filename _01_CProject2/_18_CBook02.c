#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

typedef struct MyStruct {
	char name[20];
	int age;
} MS;

int information(MS* no) {
	printf("�̸� : %s \n", (*no).name);
	printf("���� : %d \n", (*no).age);
}

int modify(MS* no) {
	printf("%s�� �̸� ���� ����>>", (*no).name);
	scanf("%s", (*no).name);
}

int main1802() {
	MS no1 = {"��ö��", 18};
	MS no2 = {"��¯��", 18};	
	//no1.name = "��ġ��";
	strcpy(no1.name, "��ġ��");
	
	printf("%s \n", no1.name);
	printf("�̸��� �Է��ϼ���>>");
	//scanf("%s", no1.name);
	scanf_s("%s",no1.name, sizeof(no1.name));

	information(&no1);

	modify(&no1);

	information(&no1);
	return 0;
}
