import "HeaderUnits.h";

using namespace std;

namespace mycode
{
	void foo()
	{
		std::cout << "foo() called in the mycode namespace" << std::endl;
	}
}

using namespace mycode;
using std::cout;					// std 네임스페이스 중 cout만 사용하겠다

// Nested namespace
namespace MyLibraries::Networking::FTP {
	/* 중첩 네임스페이스 C++17 이상*/
}

namespace MyLib {
	namespace Networking {
		namespace FTP {
			/* 기존의 중첩 네임스페이스 */
		}
	}
}

// 네임스페이스 앨리어스
namespace MyFTP = MyLibraries::Networking::FTP;

int main0065(int argc, char* argv[])				// int argc : 프로그램에 전달할 인수의 개수, char* argv[] : 프로그램에 전달할 인수의 값
{
	mycode::foo();				// mycode 네임스페이스에 정의된 foo() 함수를 호출한다
	foo();			// mycode::foo();

	cout << "Hello, World!" << std::endl;

	return 0;
}
