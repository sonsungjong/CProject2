#include <iostream>
#include <objbase.h>

// 인터페이스 (추상 클래스/순수 가상 함수) == 약속, 추가는 되지만 제거는 하지 않는다
interface ISimpleCalc
{
    virtual int __stdcall sum() = 0;
    virtual int __stdcall sub() = 0;
};

// 구현
class CSimpleCalc : public ISimpleCalc
{
    int x;
    int y;
public:
    CSimpleCalc(int _x, int _y)
    {
        x = _x;
        y = _y;
    }
    virtual int __stdcall sum() { return x + y; }
    virtual int __stdcall sub() { return x - y; }
};

int main059()
{
    int x1, y1;
    printf("Please enter an interger x1\n");
    std::cin >> x1;

    printf("Please enter an interger y1\n");
    std::cin >> y1;

    CSimpleCalc *p_sc = new CSimpleCalc(x1, y1);

    ISimpleCalc* pi_sc = p_sc;                  // 인터페이스를 통한 접근 (컴포넌트화)

    printf("x1 + y1 = %d\n", pi_sc->sum());             // vtable[0] = &sum
    printf("x1 - y1 = %d\n", pi_sc->sub());                 // vtable[1] = &sub

    delete p_sc;
}
