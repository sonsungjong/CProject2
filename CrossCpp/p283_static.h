#pragma once

extern int g_nGlobalVar;			// �������� ���� ��
void memoryRegion();			// �����Լ�

static void staticFunction();			// �����Լ� (�ش� ���Ͽ����� ��� ����)

class CStatic
{
	static int ms_num;				// ����������� ���� ��
public:
	void memberFunction();

	static void staticMemberFunction();			// ���� ��� �Լ��� ��������� ����� �� ���� ������������� ����� �� �ִ�
	// ��ü���� ���Ǳ� ������ ��������� �������� ����
};
