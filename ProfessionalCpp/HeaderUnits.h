#ifndef HEADER_UNIT_H_
#define HEADER_UNIT_H_
import <iostream>;
import <optional>;
import <utility>;
import <format>;
import <memory>;
import <cstddef>;

using namespace std;

#endif

/*
컴파일러가 C++20 모듈을 완벽히 지원하지 않는다면 별도의 헤더파일(HeaderUnits.h)을 만든다음 임포트문을 헤더에 추가한다.
헤더 파일(HeaderUnits.h)을 우클릭 후 속성(Properties)을 선택한다.
구성 속성 -> 일반 -> Item Type -> C/C++ 컴파일러 -> 적용
구성 속성 -> C/C++ -> 고급 -> 컴파일 옵션 -> C++ 헤더 단위로 컴파일 (Complie as C++ Header Unit)

모듈 구현 파티션 (내부 파티션)을 사용하려면 관련된 모든 파일을 우클릭하여
구성 속성 -> C/C++ -> 고급 -> 컴파일 옵션 -> C++ 모듈 내부 파티션 단위로 컴파일 (Compile as C++ Module Internal Partition)
*/