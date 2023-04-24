// 중복 인클루드를 막는 용도
#ifndef MYHEADER_H_
#define MY_HEADER_H_

// import <iostream>;
//#include <iostream>             // 현재 파일에서 사용할 수 있게 iostream 헤더 파일에 있는 내용을 모두 가져오도록 지시
#include "HeaderUnits.h"

int main0059()
{
    // 주석 : 컴파일러는 이 부분을 무시
    // 빌드 : 전처리 -> 컴파일 -> 링크
    printf("전처리 : 소스 코드에 담긴 메타 정보를 처리\n");
    printf("컴파일 : 소스 코드를 머신이 읽을 수 있는 객체 파일로 변환\n");
    printf("링크 : 객체 파일을 애플리케이션으로 엮는다\n");

    std::cout << "There are " << 219 << " ways I love you." << std::endl;                   // 출력 스트림
    // std::cout << std::format("There are {} ways I love you.", 219) << std::endl;

    int value = 0;
    std::cin >> value;              // 입력 스트림

    return 0;
}

#endif