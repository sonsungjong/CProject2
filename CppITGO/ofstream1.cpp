// 파일 쓰기 예제 (RAII 패턴 적용)

#include <iostream>
#include <fstream>

int main01()
{
    // 파일을 쓰기 위해서는 ofstream 객체를 이용한다
    //std::ofstream os("C:\\Data\\number.txt");       // 절대경로
    std::ofstream os("number.txt");                     // 상대경로

    // 예외처리
    if (!os) {
        std::cerr << "파일 오픈 실패\n";
        exit(1);
    }

    // 0 ~ 99 까지 파일에 쓰기
    for (int i = 0; i < 100; ++i)
    {
        os << i << " ";
    }

    // 명시적으로 close하지 않아도 소멸자가 호출되어 리소스 해제됨

    return 0;
}
