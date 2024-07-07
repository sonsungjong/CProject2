#include "pch.h"
#include <Windows.h>
#include <iostream>

// Office 타입 라이브러리 가져오기
#import "C:\\Program Files (x86)\\Common Files\\Microsoft Shared\\OFFICE16\\MSO.DLL" rename_namespace("Office") exclude("RGB", "DocumentProperties")
// Excel 타입 라이브러리 가져오기
#import "C:\\Program Files\\Microsoft Office\\root\\Office16\\EXCEL.EXE" rename_namespace("Excel") exclude("DocumentProperties", "RGB")


int main()
{
    Excel::_ApplicationPtr pEcelApp;

    printf("hello");
}
