#include <stdio.h>

// 연결리스트 : 데이터 개수를 예상할 수 없어 공간을 크게 확보하기도 적게 확보하기도 결정하기 어려울 때 사용하는 자료구조
// 헤드포인터1 -> 숫자1/포인터2 -> 숫자2/포인터3 -> ...

// 숫자를 저장하고 다음 노드를 가리킬 포인터를 확보한다

typedef struct node
{
	int number;
	struct node* p_next;
} NODE;

// 32bit 전용
void AddNumber(NODE** pp_head, NODE** pp_tail, int data)
{
	if (NULL != *pp_head)				// NULL이면 노드가 없이 헤드포인터만 존재한다는 뜻
	{
		(*pp_tail)->p_next = (NODE*)malloc(sizeof(NODE));				// 확보해둔 포인터변수에 할당된 노드의 주소를 저장
		*pp_tail = (*pp_tail)->p_next;												// 생성된 노드의 주소를 가리키도록 변경
	}
	else 
	{
		// 헤드포인터에 첫 노드 추가
		*pp_head = (NODE*)malloc(sizeof(NODE));					// 노드 할당 후 주소 저장
		*pp_tail = *pp_head;													// 새 노드의 주소값을 p_tail(*pp_tail)에 저장
	}
	(*pp_tail)->number = data;					// 생성된 노드에 data값을 저장
	(*pp_tail)->p_next = NULL;					// 다음 노드의 주소를 저장할 수 있는 공간을 NULL로 확보해둠
}

int main484()
{
	/*
	NODE* p_head = NULL;					// 첫 노드를 가리킬 헤드포인터
	
	p_head = (NODE*)malloc(sizeof(NODE));			// 헤드포인터에 새로운 노드를 할당

	p_head->number = 12;				// 노드의 number에 12 저장
	p_head->p_next = NULL;			// 다음 노드를 가리킬 변수확보

	p_head->p_next = (NODE*)malloc(sizeof(NODE));				// 다음 노드의 메모리 할당
	p_head->p_next->number = 15;							// 새로 생성된 힙의 노드에 숫자 저장
	p_head->p_next->p_next = NULL;						// 그 다음 노드를 저장할 수 있는 공간 확보
	*/

	// 연달아서 계속 늘어나기 때문에 반복문/자료구조 활용

	NODE *p_head2 = NULL, *p_tail = NULL, *p;
	int sum = 0, temp;
	while (1) {
		printf("숫자를 입력하세요 (9999를 누르면 종료) : ");
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