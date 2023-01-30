#include <cstdio>
// �ߺ��ڵ� ���̱�
// ������ ��ġ�� �ߺ��ڵ�� '�ݺ���'�� ���� ���δ�
// �ݺ����� �������ִ� �ߺ��ڵ忡 ���ؼ��� ��ó�� �Ұ����ϴ�
// �������ִ� �ߺ��ڵ�� '�Լ�'�� ���� ���δ�
// �Լ��� �ڵ庯ȭ�� ��ó�� �����ϴ�
// �Ű������� ���� �Ǵ� �ڷ����� �ٲ�� �Լ� �Ӹ��ƴ϶� �Լ�ȣ��α��� ����Ǿ���ϴ� ������ �ִ�
// �Ű������� '����ü'�� ����ϸ� �ذ��� �� �ִ�
// ����ü ������ �Ű������� ������ ������ 12����Ʈ�� �޸𸮺��簡 �߻��ϰ� ���������� ������ �Ұ����ϴ�
// �޸� ����� ���������� ������ ���ؼ� �Ű������� '����ü ������' �� ����Ѵ�.
// *C��� ��Ÿ�� : �Ű������� ����ü �����ͷ� ���ϴ� �Լ�

struct ValueList
{
	int a, b, c;
};

void ModifyValue(int* p_value, const char* p_var_name)
{
	if (*p_value < 0)
		printf("%s������ ���� %d���� %d�� ������\n", p_var_name, -*p_value, *p_value = -*p_value);
}

void MultiplyValue(struct ValueList* values)
{
	ModifyValue(&values->a, "a");
	ModifyValue(&values->b, "b");
	ModifyValue(&values->c, "c");

	printf("%d * %d * %d = %d\n", values->a, values->b, values->c, values->a * values->b * values->c);
}

int main()
{
	// ����ü ����
	struct ValueList values;
	printf("������ ���� �Է��ϼ���>>");
	scanf_s("%d %d %d", &values.a, &values.b, &values.c);

	// ����ü�� �ּҸ� ����
	MultiplyValue(&values);

	return 0;
}
