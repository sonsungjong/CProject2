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
};				// �������� �ƴϴ� ==> �߰����� ����ȭ ��Ŀ���� ������ �ʿ�

class Bar1292
{
	int m_int;
};				// ������ ==> �߰����� ����ȭ ��Ŀ������ �ʿ����

int main()
{
	// std::is_trivially_copyable_v ==> ���� ������ �����Ѱ�? 1
	// is_lock_free()    ==> �� �����ΰ�? 0
	std::atomic<Foo1292> foo;
	cout << std::is_trivially_copyable_v<Foo1292> << " " << foo.is_lock_free() << "\n";				// 1    0

	// std::is_trivially_copyable_v ==> ���� ������ �����Ѱ�? 1
	// is_lock_free()    ==> �� �����ΰ�? 1
	std::atomic<Bar1292> bar;
	cout << std::is_trivially_copyable_v<Bar1292> << " " << bar.is_lock_free() << "\n";				// 1    1

	return 0;
}