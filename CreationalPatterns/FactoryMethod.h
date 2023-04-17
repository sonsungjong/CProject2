// Factory Method 생성 패턴 (확장성, 재사용성, 다양한 객체를 생성해야하는 경우)
#pragma once
#include <tchar.h>

// 기본 인터페이스 정의
class Basic_Interface
{
public:
	virtual ~Basic_Interface() = default;

public:
	// 가상 메서드 생성
	virtual void DoIt() = 0;
};

// 서브클래스1 정의
class SubClass1 : public Basic_Interface
{
public:
	void DoIt() override
	{
		_tprintf(_T("서브클래스1 에서 구현할 무언가\n"));
	}
};

// 서브클래스2 정의
class SubClass2 : public Basic_Interface
{
public:
	void DoIt() override
	{
		_tprintf(_T("서브클래스2 에서 구현할 무언가\n"));
	}
};

// 팩토리 메서드 인터페이스 정의
class FactoryMethod
{
public:
	virtual ~FactoryMethod() = default;

	virtual Basic_Interface* factoryMethod() = 0;
};

// SubClass1 객체를 생성하는 팩토리
class SubClass3 : public FactoryMethod
{
public:
	Basic_Interface* factoryMethod() override
	{
		return new SubClass1();
	}
};

// SubClass2 객체를 생성하는 팩토리
class SubClass4 : public FactoryMethod
{
public:
	Basic_Interface* factoryMethod() override
	{
		return new SubClass2();
	}
};
