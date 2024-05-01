import "HeaderUnits.h";

int g_count1292 = 0;										// 위험!! 명시적으로 mutex 동기화가 필요
std::atomic<int> ga_count1292 = 0;				// 명시적으로 동기화하지 않아도 됨 (아토믹 타입)

std::atomic_flag ga_flag1292;
std::atomic_bool ga_bool1292;
std::atomic_char ga_char1292;
std::atomic_uchar ga_uchar1292;
std::atomic_int ga_int1292;
std::atomic_uint ga_uint1292;
std::atomic_long ga_long1292;
std::atomic_ulong ga_ulong1292;
std::atomic_llong ga_llong1292;
std::atomic_ullong ga_ullong1292;
std::atomic_wchar_t ga_wchar1292;

// 단, 구조체나 클래스가 쉽게 복제할 수 없는 타입이라면 명시적으로 동기화 메커니즘을 사용해야할 수도 있다
// std::is_trivially_copyable_v<타입>     ==> 해당 타입이 쉽게 복제할 수 있는 타입인지 검사 true/false    (atomic으로 사용할 수 있는 타입인가?)
// is_lock_free() : 락 프리인지 검사 true/false      (atomic을 사용해도 추가적인 동기화 메커니즘이 필요한가?)

// 쉽게 복제할 수 있는 클래스들
class Foo1292
{
	int m_array[123];
};				// 락프리가 아니다 ==> 추가적인 동기화 메커니즘 구현이 필요 (배열)

class Bar1292
{
	int m_int;
};				// 락프리 ==> 추가적인 동기화 메커니즘이 필요없음 (기본 자료형)

#pragma pack(push, 1)
struct MyTile1292
{
	char a;
	char b;
	char c;
	char d;
	char e;
	char f;
};

// 개별적으로 std::atomic을 하는게 더 좋음
struct MyStruct1292
{
	std::atomic<int> a;
	std::atomic<int> b;
	std::atomic<double> c;
	std::atomic<float> d;
	std::atomic<char> e;
	std::atomic<bool> f;
};
#pragma pack(pop)

int main1292()
{
	// std::is_trivially_copyable_v ==> 쉽게 복제가 가능한가? 1
	// is_lock_free()    ==> 락 프리인가? 0
	std::atomic<Foo1292> foo;
	std::cout << std::is_trivially_copyable_v<Foo1292> << " " << foo.is_lock_free() << "\n";				// 1    0

	// std::is_trivially_copyable_v ==> 쉽게 복제가 가능한가? 1
	// is_lock_free()    ==> 락 프리인가? 1
	std::atomic<Bar1292> bar;
	std::cout << std::is_trivially_copyable_v<Bar1292> << " " << bar.is_lock_free() << "\n";				// 1    1

	std::atomic<MyTile1292> tile;
	std::cout << std::is_trivially_copyable_v<MyTile1292> << " " << tile.is_lock_free() << "\n";				// 1    0
	
	// 전체에 대해서는 보호받지 못한다 (전체 구조체에 대해 업데이트할 때는 복사본을 만들어 수정한다음 락을걸고 덮어쓰고 락을 풀어야한다 [또는 비교해서 다른 부분만 업데이트])
	// {std::lock_guard<std::mutex> lock(mtx); memcpy(); }
	MyStruct1292 st;			
	std::cout << std::is_trivially_copyable_v<char> << " " << st.e.is_lock_free() << "\n";				// 1    1

	return 0;
}