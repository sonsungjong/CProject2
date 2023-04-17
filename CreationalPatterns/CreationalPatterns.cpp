#include <iostream>
#include <tchar.h>

#include "Singleton.h"
#include "Builder.h"
#include "FactoryMethod.h"

int main()
{
    _tsetlocale(0, _T("korean"));

    // ½Ì±ÛÅæ ÆÐÅÏ
    Singleton& singleton = Singleton::getInstance();
    singleton.setStr(_T("Hello, Singleton!"));
    _tprintf(_T(">>%s \n"), singleton.getStr());

    // ºô´õ ÆÐÅÏ
    BuilderCreational builder;
    Builder build = builder.setNum(10).setStr(_T("Hello, Builder!")).build();
    build.print();

    // ÆÑÅä¸®¸Þ¼­µå ÆÐÅÏ
    SubClass3 sub_class3;
    SubClass4 sub_class4;
    Basic_Interface* sub_class1 = sub_class3.factoryMethod();
    Basic_Interface* sub_class2 = sub_class4.factoryMethod();
    sub_class1->DoIt();
    sub_class2->DoIt();
    delete sub_class1;
    delete sub_class2;


    return 0;
}
