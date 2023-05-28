// 버퍼사이즈가 꽉 차면 읽기를 일시정지하고 DB에 입력
// 테이블에 맞게 코드를 수정해야함
// MariaDB 서버와 MariaDB Connector C 64bit를 사용

/*
<Include Directories>
C:\Program Files\MariaDB\MariaDB Connector C 64-bit\include

<Libraries Directories>
C:\Program Files\MariaDB\MariaDB Connector C 64-bit\lib

<Additional Dependencies>
libmariadb.lib
*/

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>
#include <tchar.h>
#include <mysql.h>

typedef std::basic_string<TCHAR> TSTRING;
typedef std::basic_stringstream<TCHAR> TSTRINGSTREAM;

struct Sensor
{
    TCHAR target_sn[50];
    TCHAR time[50];
    float temp;
    int humi;
    float vib;
    float lat;
    float lon;
};

struct Military1
{
    TCHAR name[50];
    TCHAR target_sn[50];
    int operation_time;
    int total_operation_time;
    TCHAR broken[50];
};

struct BitInfo
{
    TCHAR datetime[20];
    TCHAR name[50];
    TCHAR str1[50];
    TCHAR str2[50];
    float float1;
    int int1;
};

struct UserAccount
{
    TCHAR user_id[50];
    TCHAR user_pw[50];
};

std::string wstring_to_utf8(const std::wstring& wstr)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
    return convert.to_bytes(wstr);
}

std::wstring utf8_to_wstring(const char* utf8str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wstr = converter.from_bytes(utf8str);
    return wstr;
}


int main()
{
    
}
