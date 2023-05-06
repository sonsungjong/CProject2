#include <iostream>
#include "Singleton1.h"

using namespace std;

int main()
{
    Singleton1& s1 = Singleton1::getInstance();
    Singleton1& s2 = Singleton1::getInstance();

    s1.Say();               // 0
    s2.Say();               // 0

    s1.SetValue(3);

    s1.Say();               // 3
    s2.Say();               // 3

    s1.SetValue(7);
    cout << s1.GetValue() << endl;              // 7
    cout << s2.GetValue() << endl;              // 7
}
