#include <iostream>
#include <unordered_map>
#include "ExcelCreatorNet48.h"
#include <Windows.h>
#include <string>

#ifdef _DEBUG
#pragma comment(lib, "ExcelCreatorNet48D.lib")
#else
#pragma comment(lib, "ExcelCreatorNet48.lib")
#endif

// 새로운 엑셀에 옮겨야하는 항목들
typedef struct Telcordia
{
    std::string strCategory;
    std::string strSubcategory;
    std::string strComponentsType;
    std::string strTempCurve;                         // Temperature Curve
    double dbModelNValue;                            // 모델 N값
    double dbModelEnergy;                           // 모델 활성화에너지값
    double dbReliability;                                // 신뢰도 종합 (고장률)
    double dbPeckValue;                                 // Peck모델 기반 부품 가속계수 산출값 AF (=>고장률과 함께 시스템 가속계수 산출에 사용)
} stTelcordia;

// 볼츠만 상수 k
const double k = 8.617333262145e-5;

double calculateAFi(double H1, double H2, double ni, double Eai, double T1, double T2) {
    double humidityTerm = pow(H1 / H2, -ni);
    double exponentTerm = (Eai / k) * (1.0 / T1 - 1.0 / T2);
    return humidityTerm * exp(exponentTerm);
}

void printMap(const std::unordered_map<std::string, std::string>& map) {
    for (const auto& pair : map) {
        std::cout << "Category: " << pair.second << ", Subcategory: " << pair.first << "\n";
    }
    printf("%lld\n", map.size());
}

int main()
{
    InteropExcel::ExcelCreator app;
    std::unordered_map<std::string, stTelcordia> mapComponents;
    //mapCurveEV["1"] = 0.05;
    //mapCurveEV["2"] = 0.1;
    //mapCurveEV["3"] = 0.15;
    //mapCurveEV["4"] = 0.22;
    //mapCurveEV["5"] = 0.28;
    //mapCurveEV["6"] = 0.35;
    //mapCurveEV["7"] = 0.4;
    //mapCurveEV["8"] = 0.45;
    //mapCurveEV["9"] = 0.56;
    //mapCurveEV["10"] = 0.7;
    //mapCurveEV["No"] = 0.0;

    std::string filepath = "C:\\KTL\\KTL자동화 관련 데이터(활성화에너지_매칭_ 등등) 자료 정리.v4_20240407.xlsx";
    std::string Category;
    std::string Subcategory;
    std::string Division;

    app.readFile(filepath, "1. SR-332 부품 분류 및 능동_수동 구분");
    for (int i = 3; i < 255; i++) {
        Category = app.readCell("A", i);
        Subcategory = app.readCell("B", i);
        Division = app.readCell("J", i);

        if (Division.find("Active") != std::string::npos) 
        {
            stTelcordia stTelcordia{ Category, Subcategory, Division, "No", 0.0, 0.0, 0.0, 0.0};
            mapComponents[Subcategory] = stTelcordia;
        }
        else if (Division.find("Passive") != std::string::npos) 
        {
            stTelcordia stTelcordia{ Category, Subcategory, Division, "No", 3.0, 0.0, 0.0, 0.0};
            mapComponents[Subcategory] = stTelcordia;
        }
        else if (Division.find("etc") != std::string::npos) 
        {
            stTelcordia stTelcordia{ Category, Subcategory, Division, "No", 3.0, 0.0, 0.0, 0.0};
            mapComponents[Subcategory] = stTelcordia;
        }
        else {
            std::wstring err_msg = L"찾을 수 없는 구분자입니다 : " + std::to_wstring(i) + L"행";
            ::MessageBox(GetActiveWindow(), err_msg.c_str(), L"구분자 오류", MB_OK | MB_ICONERROR);
        }
    }
    app.readEnd();

    //printf("==========ACTIVE=============\n");
    //printMap(mapActive);
    //printf("===========PASSIVE============\n");
    //printMap(mapPassive);
    //printf("============ETC===========\n");
    //printMap(mapETC);
    app.readFile(filepath, "2. 활성화에너지 매칭");
    std::unordered_map<std::string, double> mapCurveEV;
    for (int i = 2; i < 11; i++) {
        std::string mapEVKey = app.readCell("J", i);
        std::string mapEVValue = app.readCell("K", i);
        try {
            mapCurveEV[mapEVKey] = std::stod(mapEVValue);
        }
        catch (const std::invalid_argument& e) {
            ::MessageBoxA(GetActiveWindow(), e.what(), "활성화에너지값 변환오류(숫자가 아닙니다)", MB_OK | MB_ICONERROR);
        }
    }

    int sheet2Row = 2;      // 2부터 시작
    while (true) 
    {
        std::string cellData = app.readCell("F", sheet2Row);
        if (cellData != "") 
        {
            mapComponents[cellData].strTempCurve = app.readCell("G", sheet2Row);
            if (mapComponents[cellData].strComponentsType.find("Active") == std::string::npos) {
                mapComponents[cellData].dbModelEnergy = mapCurveEV[mapComponents[cellData].strTempCurve];       // Active가 아닐때 텔코디아EV 적용
            }

            sheet2Row++;
        }
        else {
            break;
        }
    }
    app.readEnd();

    // Active에 대해서 N값과 활성화에너지 반영
    int sheet3Row = 4;          // 4부터 시작
    app.readFile(filepath, "3-1. JEP122 Ea_n 값 모음_부분(사용)");
    while (true) 
    {
        break;
    }
    app.readEnd();

    // 부품별 dbPeckValue 계산
    for (const auto& pair : mapComponents) {
        // pair.first
        // pair.second
        //mapComponents[pair.first] = calculateAFi(H1, H2, -ni, Eai, T1, T2);
        
    }
    
    // 시스템 가속계수 산출 (double)


    return 0;
}

