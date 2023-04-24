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
using std::cout;					// std ���ӽ����̽� �� cout�� ����ϰڴ�

// Nested namespace
namespace MyLibraries::Networking::FTP {
	/* ��ø ���ӽ����̽� C++17 �̻�*/
}

namespace MyLib {
	namespace Networking {
		namespace FTP {
			/* ������ ��ø ���ӽ����̽� */
		}
	}
}

// ���ӽ����̽� �ٸ��
namespace MyFTP = MyLibraries::Networking::FTP;

int main0065(int argc, char* argv[])				// int argc : ���α׷��� ������ �μ��� ����, char* argv[] : ���α׷��� ������ �μ��� ��
{
	mycode::foo();				// mycode ���ӽ����̽��� ���ǵ� foo() �Լ��� ȣ���Ѵ�
	foo();			// mycode::foo();

	cout << "Hello, World!" << std::endl;

	return 0;
}
