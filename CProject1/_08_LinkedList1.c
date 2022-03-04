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

	printf("�Է��� �����Ϸ��� 999�� �Է��ϼ���.\n");
	// �� �Է�
	while (1) {
		printf("���� �Է��ϼ���>>>");
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

	// ��ȸ
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

	// �޸� ����
	while (p_head != NULL) {
		p_tail = p_head;					// p_tail�� ��Ȱ��
		p_head = p_head->p_next;
		free(p_tail);						// �տ����� ����
	}
	p_tail = NULL;

	return 0;
}