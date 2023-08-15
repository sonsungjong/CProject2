// COM클라이언트 입장에서는 COM컴포넌트(블랙박스)의 인터페이스를 알기위한 방법이 필요하다 == QueryInterface
// QueryInterface == COM 컴포넌트의 interface 이름을 획득

#include <iostream>
#include <objbase.h>

interface IUnknown69
{
	virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv) = 0;
	virtual ULONG __stdcall AddRef() = 0;
	virtual ULONG __stdcall Release() = 0;
};

interface ISetValue73 : public IUnknown
{
	virtual void __stdcall SetValue(int, int) = 0;
};

interface ISimpleCal73 : public IUnknown
{
	virtual int __stdcall Sum() = 0;
	virtual int __stdcall Sub() = 0;
};

extern const IID IID_ISetValue73;
extern const IID IID_ISimpleCal73;

class CSimpleCalc73 : public ISetValue73, public ISimpleCal73
{
	int m_x;
	int m_y;

	virtual HRESULT __stdcall QueryInterface(const IID& iid, void** ppv);
	virtual ULONG __stdcall AddRef() { return 0; }
	virtual ULONG __stdcall Release() { return 0; }

	// ISetValue73
	virtual void __stdcall SetValue(int _x, int _y)
	{
		m_x = _x;
		m_y = _y;
	}

	// ISimpleCal73
	virtual int __stdcall Sum()
	{
		return m_x + m_y;
	}

	virtual int __stdcall Sub()
	{
		return m_x - m_y;
	}
};

HRESULT __stdcall CSimpleCalc73::QueryInterface(const IID& iid, void** ppv)
{
	if (iid == IID_IUnknown)
	{
		printf("QueryInterface: Return pointer to IUnknown.\n");
		*ppv = (IUnknown69*)(ISetValue73*)(this);
	}
	else if (iid == IID_ISetValue73) {
		printf("QueryInterface: Return pointer to ISetValue.\n");
		*ppv = (ISetValue73*)(this);
	}
	else if (iid == IID_ISimpleCal73) {
		printf("QueryInterface: Return pointer to ISimpleCalc.\n");
		*ppv = (ISimpleCal73*)(this);
	}
	else {
		printf("QueryInterface: Interface not supported.\n");
		*ppv = NULL;
		return E_NOINTERFACE;
	}

	((IUnknown*)(*ppv))->AddRef();
	//static_cast<IUnknown*>(*ppv)->AddRef();
	return S_OK;
}

IUnknown* CreateInstance()
{
	IUnknown* pi = (ISetValue73*)(new CSimpleCalc73);
	pi->AddRef();				// 컴포넌트의 개수를 하나 증가시킨다
	return pi;
}

// {0CF9BAC4-79AB-444e-B5E6-B084ABE011D3}
static const IID IID_ISetValue73 = { 0xcf9bac4, 0x79ab, 0x444e, {0xb5, 0xe6, 0xb0, 0x84, 0xab, 0xe0, 0x11, 0xd3} };

// {C8D5E047-DC09-4af0-965D-A4815ADA2A50}
static const IID IID_ISimpleCal73 = { 0xc8d5e047, 0xdc09, 0x4af0, {0x96, 0x5d, 0xa4, 0x81, 0x5a, 0xda, 0x2a, 0x50} };

void foo069(IUnknown* _pi)
{
	// 인터페이스 포인터 정의
	ISimpleCal73* pi_sc = NULL;

	// ISimpleCalc 인터페이스 질의
	HRESULT hr = _pi->QueryInterface(IID_ISimpleCal73, (void**)&pi_sc);

	// Check return value
	if (SUCCEEDED(hr)) {
		pi_sc->Sum();				// Use interface
	}
}

int main84()
{
	// 인터페이스ID : 인터페이스 이름의 중복을 막기위해 정한 고유의 이름
	// IUnknwon의 QueryInterface : 인터페이스ID를 전달하면 인터페이스 이름을 반환
	HRESULT hr;
	int x1, y1;

	printf("Please enter an integer x1\n");
	std::cin >> x1;
	printf("Please enter an interface y1\n");
	std::cin >> y1;
	
	printf("\n\nClient:        Get an IUnknown pointer.\n\n");
	IUnknown* pi_unknown = CreateInstance();				// (내부)객체화

	printf("Client:        Get Interface ISetValue.\n");
	ISetValue73* pi_set_value = NULL;								// QueryInterface로 인터페이스에 접근

	hr = pi_unknown->QueryInterface(IID_ISetValue73, (void**)&pi_set_value);

	if (SUCCEEDED(hr)) {
		printf("Client: Succeeded getting ISetValue.\n\n");
		pi_set_value->SetValue(x1, y1);
	}

	printf("Client:        Get Interface ISimpleCal.\n");
	ISimpleCal73* pi_sc = NULL;					// QueryInterface로 인터페이스에 접근

	hr = pi_unknown->QueryInterface(IID_ISimpleCal73, (void**)&pi_sc);

	if (SUCCEEDED(hr)) {
		printf("Client: Succeeded getting ISimpleCal.\n");
		printf("Client: The Sum of x + y = %d\n", pi_sc->Sum());
		printf("Client: The Sub of x - y = %d\n\n\n", pi_sc->Sub());
	}

	printf("============================================\n");
	printf("Client: Get Interface ISimpleCal from interface ISetValue.\n");
	ISimpleCal73* pi_sc2 = NULL;
	hr = pi_set_value->QueryInterface(IID_ISimpleCal73, (void**)&pi_sc2);
	if (SUCCEEDED(hr)) {
		printf("Client: Succeeded getting ISimpleCal from ISetValue.\n\n");
	}

	printf("Client: Get Interface IUnknown from interface ISimpleCalc.\n");
	IUnknown* pi_unknown2 = NULL;
	hr = pi_sc->QueryInterface(IID_IUnknown, (void**)&pi_unknown2);

	if (SUCCEEDED(hr)) {
		if (pi_unknown2 == pi_unknown) {
			printf("pi_unknown2 is equal to pi_unknown.\n");
		}
		else {
			printf("pi_unknown2 is not equal to pi_unknown.\n");
		}
	}
	
	delete pi_unknown;					// 객체 해제
	return 0;
}

// 컴포넌트 인스턴스는 항상 동일한 IUnknown 인터페이스를 가진다
BOOL IsSameComponents(ISetValue73* _pi_set_value, ISimpleCal73* _pi_sc)
{
	IUnknown* pi1 = NULL;
	IUnknown* pi2 = NULL;

	_pi_set_value->QueryInterface(IID_IUnknown, (void**)&pi1);
	_pi_sc->QueryInterface(IID_IUnknown, (void**)&pi2);

	return pi1 == pi2;					// pi1 과 pi2는 항상 동일한 값을 가진다
}

// 이미 획득한 인터페이스로 동일한 인터페이스를 다시 획득하는 것은 항상 성공
// 역으로 획득하는 것도 항상 성공
void f083(ISetValue73* _pi_set_value)
{
	HRESULT hr;
	ISetValue73* pi2 = NULL;
	ISimpleCal73* pi_sc = NULL;

	// ISetValue로부터 ISimpleCalc 인터페이스 획득
	hr = _pi_set_value->QueryInterface(IID_ISimpleCal73, (void**)&pi_sc);
	if (SUCCEEDED(hr)) {
		// ISimpleCalc로부터 ISetValue를 획득하는 것은 항상 가능하다
		hr = pi_sc->QueryInterface(IID_ISetValue73, (void**)&pi2);

		if (SUCCEEDED(hr)) {
			printf("항상 성공\n");
		}
	}
}

// 인터페이스 IX로부터 IY를 획득하고, IY로부터 IZ를 획득할 수 있다면, IX로부터 IZ를 획득하는 것도 항상 성공한다
void SampleFunction84(IUnknown* p_ix)
{
	HRESULT hr;
	ISetValue73* p_iy = NULL;
	ISimpleCal73* p_iz = NULL;

	hr = p_ix->QueryInterface(IID_ISetValue73, (void**)&p_iy);				// p_ix로 p_iy를 획득
	hr = p_iy->QueryInterface(IID_ISimpleCal73, (void**)&p_iz);				// p_iy로 p_iz를 획득

	// 위 두 QueryInterface가 성공이라면, 다음 코드도 성공
	hr = p_ix->QueryInterface(IID_ISimpleCal73, (void**)&p_iz);				// p_ix로 p_iz를 획득
}
