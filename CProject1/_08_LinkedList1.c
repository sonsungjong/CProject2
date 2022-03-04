#include <stdio.h>
#include <malloc.h>

typedef struct Node
{
	int num;
	struct Node* p_next;
} N;

int main() {
	
	N *p_head = NULL, *p_tail = NULL;
	int num = 0;

	printf("입력을 종료하려면 999를 입력하세요.\n");
	// 값 입력
	while (1) {
		printf("값을 입력하세요>>>");
		scanf_s("%d", &num);

		if (num != 999) {
			if (p_head == NULL) {
				p_tail = p_head = (N*)malloc(sizeof(N));
			} else {
				p_tail->p_next = (N*)malloc(sizeof(N));
				p_tail = p_tail->p_next;
			}
			p_tail->num = num;
			p_tail->p_next = NULL;
		}
		else { break; }
	}

	// 조회
	p_tail = p_head;
	while (1) {
		if (p_head != NULL) {
			printf("%d ", p_head->num);
			p_head = p_head->p_next;
		}
		else {
			p_head = p_tail;
			break;
		}
	}

	// 메모리 해제
	while (p_head != NULL) {
		p_tail = p_head;					// p_tail을 재활용
		p_head = p_head->p_next;
		free(p_tail);						// 앞에부터 지움
	}
	p_tail = NULL;

	return 0;
}