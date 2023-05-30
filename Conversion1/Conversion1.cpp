#include <iostream>
#include <string>
#include <tchar.h>
#include <locale>
#include <atlconv.h>
#include <codecvt>

// UTF-8일 경우 CP_ACP를 CP_UTF8로 변경하기
std::string wstring_to_utf8_second(const std::wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
	return convert.to_bytes(wstr);
}

std::wstring utf8_to_wstring_second(const char* utf8str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::wstring wstr = converter.from_bytes(utf8str);
	return wstr;
}

std::wstring utf8_to_wstring(const std::string& utf8str)
{
	int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8str.c_str(), -1, NULL, 0);
	std::wstring wstr(wlen, 0);
	MultiByteToWideChar(CP_UTF8, 0, utf8str.c_str(), -1, &wstr[0], wlen);
	return wstr;
}

std::string wstring_to_utf8(const std::wstring& wstr)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	std::string str(len, 0);
	WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], len, NULL, NULL);
	return str;
}

/*
멀티바이트에서 유니코드로 변환 (char)
*/
void A2Wpchar(const char* const p_char, wchar_t* const p_wchar)
{
	int origin_len = strlen(p_char);
	int len = MultiByteToWideChar(CP_ACP, 0, p_char, origin_len, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, p_char, origin_len, p_wchar, len);			
}

/*
유니코드에서 멀티바이트로 변환 (char)
*/
void W2Apchar(const wchar_t* const p_wchar, char* const p_char)
{
	int len = WideCharToMultiByte(CP_ACP, 0, p_wchar, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, p_wchar, -1, p_char, len, NULL, NULL);
}

/*
UTF-8에서 유니코드로 변환
*/
void Utf2Wpchar(const char* const p_char, wchar_t* const p_wchar)
{
	int origin_len = strlen(p_char);
	int len = MultiByteToWideChar(CP_UTF8, 0, p_char, origin_len, NULL, NULL);
	MultiByteToWideChar(CP_UTF8, 0, p_char, origin_len, p_wchar, len);
}

/*
유니코드에서 UTF-8로 변환
*/
void W2Utfpchar(const wchar_t* const p_wchar, char* const p_char)
{
	int len = WideCharToMultiByte(CP_UTF8, 0, p_wchar, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, p_wchar, -1, p_char, len, NULL, NULL);
}

/*
char* 를 std::string으로 변환
*/
void pchar2string(const char* const p_char, std::string& str)
{
	str.assign(p_char);
}

/*
std::string을 char*로 변환
*/
void string2pchar(std::string& str, char* p_char)
{
	std::copy(str.begin(), str.end(), p_char);
}

/*
wchar_t* 를 std::wstring으로 변환
*/
void pWchar2Wstring(const wchar_t* const p_wchar, std::wstring& wstr)
{
	wstr.assign(p_wchar);
}

/*
std::wstring을 wchar_t*로 변환
*/
void Wstring2pWchar(std::wstring& wstr, wchar_t* p_wchar)
{
	std::copy(wstr.begin(), wstr.end(), p_wchar);
}

/*
std::string을 std::wstring으로 변환
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
std::wstring을 std::string으로 변환
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
	setlocale(LC_ALL, "");                  // 유니코드 한글출력

	const char* multibyte1 = "멀티바이트 캐릭터1.123.123";
	std::string multibyte2 = "멀티바이트 스트링2.456.456";
	char multibyte3[256] = "멀티바이트 배열3.789.789";
	const wchar_t* unicode1 = L"유니코드 캐릭터1.123.123";
	std::wstring unicode2 = L"유니코드 스트링2.456.456";
	wchar_t unicode3[256] = L"유니코드 배열3.789.789";
	
// ================================================== //

	// char*를 wchar_t* 로
	//A2Wpchar(multibyte1, unicode3);
	//wprintf(_T("%s\n"), unicode3);
	
	// wchar_t* 를 char* 로
	//W2Apchar(unicode1, multibyte3);
	//printf("%s", multibyte3);

	// string을 wchar_t* 로
	//A2Wpchar(multibyte2.c_str(), unicode3);
	//std::wcout << unicode3 << std::endl;

	// wstring을 char*로
	//W2Apchar(unicode2.c_str(), multibyte3);
	//printf("%s", multibyte3);
	
	// string을 wstring으로
	//A2Wstring(multibyte2, unicode2);
	//wprintf(_T("%s\n"), unicode2.c_str());

	// wstring을 string으로
	//W2Astring(unicode2, multibyte2);
	//printf("%s\n", multibyte2.c_str());
	
// ================================================= //

	// char*를 string으로
	//pchar2string(multibyte1, multibyte2);
	//printf("%s", multibyte2.c_str());
	
	// string을 char*로
	//string2pchar(multibyte2, multibyte3);
	//printf("%s", multibyte3);

	// wchar_t* 를 wstring으로
	//pWchar2Wstring(unicode1, unicode2);
	//wprintf(_T("%s\n"), unicode2.c_str());
	 
	// wstring을 wchar_t* 로
	//Wstring2pWchar(unicode2, unicode3);
	//wprintf(_T("%s\n"), unicode3);


	return 0;
}