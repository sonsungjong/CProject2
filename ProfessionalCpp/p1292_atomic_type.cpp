import "HeaderUnits.h";

int g_count1292 = 0;										// ����!! ��������� mutex ����ȭ�� �ʿ�
std::atomic<int> ga_count1292 = 0;				// ��������� ����ȭ���� �ʾƵ� �� (����� Ÿ��)

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

// ��, ����ü�� Ŭ������ ���� ������ �� ���� Ÿ���̶�� ��������� ����ȭ ��Ŀ������ ����ؾ��� ���� �ִ�
// std::is_trivially_copyable_v<Ÿ��>     ==> �ش� Ÿ���� ���� ������ �� �ִ� Ÿ������ �˻� true/false    (atomic���� ����� �� �ִ� Ÿ���ΰ�?)
// is_lock_free() : �� �������� �˻� true/false      (atomic�� ����ص� �߰����� ����ȭ ��Ŀ������ �ʿ��Ѱ�?)

// ���� ������ �� �ִ� Ŭ������
class Foo1292
{
	int m_array[123];
};				// �������� �ƴϴ� ==> �߰����� ����ȭ ��Ŀ���� ������ �ʿ� (�迭)

class Bar1292
{
	int m_int;
};				// ������ ==> �߰����� ����ȭ ��Ŀ������ �ʿ���� (�⺻ �ڷ���)

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

// ���������� std::atomic�� �ϴ°� �� ����
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
	// std::is_trivially_copyable_v ==> ���� ������ �����Ѱ�? 1
	// is_lock_free()    ==> �� �����ΰ�? 0
	std::atomic<Foo1292> foo;
	std::cout << std::is_trivially_copyable_v<Foo1292> << " " << foo.is_lock_free() << "\n";				// 1    0

	// std::is_trivially_copyable_v ==> ���� ������ �����Ѱ�? 1
	// is_lock_free()    ==> �� �����ΰ�? 1
	std::atomic<Bar1292> bar;
	std::cout << std::is_trivially_copyable_v<Bar1292> << " " << bar.is_lock_free() << "\n";				// 1    1

	std::atomic<MyTile1292> tile;
	std::cout << std::is_trivially_copyable_v<MyTile1292> << " " << tile.is_lock_free() << "\n";				// 1    0
	
	// ��ü�� ���ؼ��� ��ȣ���� ���Ѵ� (��ü ����ü�� ���� ������Ʈ�� ���� ���纻�� ����� �����Ѵ��� �����ɰ� ����� ���� Ǯ����Ѵ� [�Ǵ� ���ؼ� �ٸ� �κи� ������Ʈ])
	// {std::lock_guard<std::mutex> lock(mtx); memcpy(); }
	MyStruct1292 st;			
	std::cout << std::is_trivially_copyable_v<char> << " " << st.e.is_lock_free() << "\n";				// 1    1

	return 0;
}