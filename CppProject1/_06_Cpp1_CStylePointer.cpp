#include <cstdio>
#include <malloc.h>
// 빠진내용 + 보충이필요한내용

typedef struct Node
{
	int num;
	struct Node* p_next;
} N;

// this포인터로 포인터 사용을 줄이고자함
void AddNodeC(N **ap_head, N **ap_tail, int a_num)
{
	if (*ap_head == NULL) {
		*ap_tail = *ap_head = (N*)malloc(sizeof(N));
	}
	else {
		(*ap_tail)->p_next = (N*)malloc(sizeof(N));
		(*ap_tail) = (*ap_tail)->p_next;
	}
	(*ap_tail)->num = a_num;
	(*ap_tail)->p_next = NULL;
}

int mainCStyle() {
	N* p_head = NULL;
	N* p_tail = NULL;
	int num = 0;

	printf("입력을 종료하고 싶다면 999를 입력하세요.\n");
	while (1) {
		printf("값을 입력하세요 : ");
		scanf_s("%d", &num);

		if (num != 999) {
			AddNodeC(&p_head, &p_tail, num);
		}
		else break;
	}

	while (p_head) {
		p_tail = p_head;
		p_head = p_head->p_next;
		free(p_tail);
	}
	p_tail = NULL;

	return 0;
}