// Factory Method ���� ���� (Ȯ�强, ���뼺, �پ��� ��ü�� �����ؾ��ϴ� ���)
#pragma once
#include <tchar.h>

// �⺻ �������̽� ����
class Basic_Interface
{
public:
	virtual ~Basic_Interface() = default;

public:
	// ���� �޼��� ����
	virtual void DoIt() = 0;
};

// ����Ŭ����1 ����
class SubClass1 : public Basic_Interface
{
public:
	void DoIt() override
	{
		_tprintf(_T("����Ŭ����1 ���� ������ ����\n"));
	}
};

// ����Ŭ����2 ����
class SubClass2 : public Basic_Interface
{
public:
	void DoIt() override
	{
		_tprintf(_T("����Ŭ����2 ���� ������ ����\n"));
	}
};

// ���丮 �޼��� �������̽� ����
class FactoryMethod
{
public:
	virtual ~FactoryMethod() = default;

	virtual Basic_Interface* factoryMethod() = 0;
};

// SubClass1 ��ü�� �����ϴ� ���丮
class SubClass3 : public FactoryMethod
{
public:
	Basic_Interface* factoryMethod() override
	{
		return new SubClass1();
	}
};

// SubClass2 ��ü�� �����ϴ� ���丮
class SubClass4 : public FactoryMethod
{
public:
	Basic_Interface* factoryMethod() override
	{
		return new SubClass2();
	}
};
