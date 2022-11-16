#include <stdio.h>

// ���Ḯ��Ʈ : ������ ������ ������ �� ���� ������ ũ�� Ȯ���ϱ⵵ ���� Ȯ���ϱ⵵ �����ϱ� ����� �� ����ϴ� �ڷᱸ��
// ���������1 -> ����1/������2 -> ����2/������3 -> ...

// ���ڸ� �����ϰ� ���� ��带 ����ų �����͸� Ȯ���Ѵ�

typedef struct node
{
	int number;
	struct node* p_next;
} NODE;

// 32bit ����
void AddNumber(NODE** pp_head, NODE** pp_tail, int data)
{
	if (NULL != *pp_head)				// NULL�̸� ��尡 ���� ��������͸� �����Ѵٴ� ��
	{
		(*pp_tail)->p_next = (NODE*)malloc(sizeof(NODE));				// Ȯ���ص� �����ͺ����� �Ҵ�� ����� �ּҸ� ����
		*pp_tail = (*pp_tail)->p_next;												// ������ ����� �ּҸ� ����Ű���� ����
	}
	else 
	{
		// ��������Ϳ� ù ��� �߰�
		*pp_head = (NODE*)malloc(sizeof(NODE));					// ��� �Ҵ� �� �ּ� ����
		*pp_tail = *pp_head;													// �� ����� �ּҰ��� p_tail(*pp_tail)�� ����
	}
	(*pp_tail)->number = data;					// ������ ��忡 data���� ����
	(*pp_tail)->p_next = NULL;					// ���� ����� �ּҸ� ������ �� �ִ� ������ NULL�� Ȯ���ص�
}

int main484()
{
	/*
	NODE* p_head = NULL;					// ù ��带 ����ų ���������
	
	p_head = (NODE*)malloc(sizeof(NODE));			// ��������Ϳ� ���ο� ��带 �Ҵ�

	p_head->number = 12;				// ����� number�� 12 ����
	p_head->p_next = NULL;			// ���� ��带 ����ų ����Ȯ��

	p_head->p_next = (NODE*)malloc(sizeof(NODE));				// ���� ����� �޸� �Ҵ�
	p_head->p_next->number = 15;							// ���� ������ ���� ��忡 ���� ����
	p_head->p_next->p_next = NULL;						// �� ���� ��带 ������ �� �ִ� ���� Ȯ��
	*/

	// ���޾Ƽ� ��� �þ�� ������ �ݺ���/�ڷᱸ�� Ȱ��

	NODE *p_head2 = NULL, *p_tail = NULL, *p;
	int sum = 0, temp;
	while (1) {
		printf("���ڸ� �Է��ϼ��� (9999�� ������ ����) : ");
		scanf_s("%d", &temp);
		if (9999 == temp) {
			break;
		}
		AddNumber(&p_head2, &p_tail, temp);
	}

	p = p_head2;
	while (NULL != p) {
		if (p != p_head2) {
			printf(" + ");
		}
		printf(" %d ", p->number);
		sum = sum + p->number;
		p = p->p_next;
	}
	printf(" = %d\n", sum);

	while (NULL != p_head2) {
		p = p_head2;
		p_head2 = p_head2->p_next;
		free(p);
	}
	p_tail = p_head2;

	return 0;
}