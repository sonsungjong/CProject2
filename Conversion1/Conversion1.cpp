#include <iostream>
#include <string>
#include <tchar.h>
#include <locale>
#include <atlconv.h>

// UTF-8�� ��� CP_ACP�� CP_UTF8�� �����ϱ�

/*
��Ƽ����Ʈ���� �����ڵ�� ��ȯ (char)
*/
void A2Wpchar(const char* const p_char, wchar_t* const p_wchar)
{
	int origin_len = strlen(p_char);
	int len = MultiByteToWideChar(CP_ACP, 0, p_char, origin_len, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, p_char, origin_len, p_wchar, len);			
}

/*
�����ڵ忡�� ��Ƽ����Ʈ�� ��ȯ (char)
*/
void W2Apchar(const wchar_t* const p_wchar, char* const p_char)
{
	int len = WideCharToMultiByte(CP_ACP, 0, p_wchar, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, p_wchar, -1, p_char, len, NULL, NULL);
}

/*
UTF-8���� �����ڵ�� ��ȯ
*/
void Utf2Wpchar(const char* const p_char, wchar_t* const p_wchar)
{
	int origin_len = strlen(p_char);
	int len = MultiByteToWideChar(CP_UTF8, 0, p_char, origin_len, NULL, NULL);
	MultiByteToWideChar(CP_UTF8, 0, p_char, origin_len, p_wchar, len);
}

/*
�����ڵ忡�� UTF-8�� ��ȯ
*/
void W2Utfpchar(const wchar_t* const p_wchar, char* const p_char)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, p_wchar, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, p_wchar, -1, p_char, len, NULL, NULL);
}

/*
char* �� std::string���� ��ȯ
*/
void pchar2string(const char* const p_char, std::string& str)
{
	str.assign(p_char);
}

/*
std::string�� char*�� ��ȯ
*/
void string2pchar(std::string& str, char* p_char)
{
	std::copy(str.begin(), str.end(), p_char);
}

/*
wchar_t* �� std::wstring���� ��ȯ
*/
void pWchar2Wstring(const wchar_t* const p_wchar, std::wstring& wstr)
{
	wstr.assign(p_wchar);
}

/*
std::wstring�� wchar_t*�� ��ȯ
*/
void Wstring2pWchar(std::wstring& wstr, wchar_t* p_wchar)
{
	std::copy(wstr.begin(), wstr.end(), p_wchar);
}

/*
std::string�� std::wstring���� ��ȯ
*/
void A2Wstring(std::string& str, std::wstring& wstr)
{
	const char* p_char = str.c_str();
	int origin_len = str.length();
	int len = strlen(p_char) *sizeof(wchar_t);
	wchar_t* p_wchar = new wchar_t[len];
	memset(p_wchar, 0, len);
	MultiByteToWideChar(CP_ACP, 0, p_char, origin_len, p_wchar, len);
	wstr.assign(p_wchar);

	delete[] p_wchar;
}

/*
std::wstring�� std::string���� ��ȯ
*/
void W2Astring(std::wstring& wstr, std::string& str)
{
	const wchar_t* p_wchar = wstr.c_str();
	int len = wstr.length() *sizeof(wchar_t);
	char* p_char = new char[len];
	WideCharToMultiByte(CP_ACP, 0, p_wchar, -1, p_char, len, NULL, NULL);
	str.assign(p_char);

	delete[] p_char;
}

int main()
{
	setlocale(LC_ALL, "");                  // �����ڵ� �ѱ����

	const char* multibyte1 = "��Ƽ����Ʈ ĳ����1.123.123";
	std::string multibyte2 = "��Ƽ����Ʈ ��Ʈ��2.456.456";
	char multibyte3[256] = "��Ƽ����Ʈ �迭3.789.789";
	const wchar_t* unicode1 = L"�����ڵ� ĳ����1.123.123";
	std::wstring unicode2 = L"�����ڵ� ��Ʈ��2.456.456";
	wchar_t unicode3[256] = L"�����ڵ� �迭3.789.789";
	
// ================================================== //

	// char*�� wchar_t* ��
	//A2Wpchar(multibyte1, unicode3);
	//wprintf(_T("%s\n"), unicode3);
	
	// wchar_t* �� char* ��
	//W2Apchar(unicode1, multibyte3);
	//printf("%s", multibyte3);

	// string�� wchar_t* ��
	//A2Wpchar(multibyte2.c_str(), unicode3);
	//std::wcout << unicode3 << std::endl;

	// wstring�� char*��
	//W2Apchar(unicode2.c_str(), multibyte3);
	//printf("%s", multibyte3);
	
	// string�� wstring����
	//A2Wstring(multibyte2, unicode2);
	//wprintf(_T("%s\n"), unicode2.c_str());

	// wstring�� string����
	//W2Astring(unicode2, multibyte2);
	//printf("%s\n", multibyte2.c_str());
	
// ================================================= //

	// char*�� string����
	//pchar2string(multibyte1, multibyte2);
	//printf("%s", multibyte2.c_str());
	
	// string�� char*��
	//string2pchar(multibyte2, multibyte3);
	//printf("%s", multibyte3);

	// wchar_t* �� wstring����
	//pWchar2Wstring(unicode1, unicode2);
	//wprintf(_T("%s\n"), unicode2.c_str());
	 
	// wstring�� wchar_t* ��
	//Wstring2pWchar(unicode2, unicode3);
	//wprintf(_T("%s\n"), unicode3);


	return 0;
}