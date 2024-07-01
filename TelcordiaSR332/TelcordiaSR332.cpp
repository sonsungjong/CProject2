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

// ���ο� ������ �Űܾ��ϴ� �׸��
typedef struct Telcordia
{
    std::string strCategory;
    std::string strSubcategory;
    std::string strComponentsType;
    std::string strTempCurve;                         // Temperature Curve
    double dbModelNValue;                            // �� N��
    double dbModelEnergy;                           // �� Ȱ��ȭ��������
    double dbReliability;                                // �ŷڵ� ���� (�����)
    double dbPeckValue;                                 // Peck�� ��� ��ǰ ���Ӱ�� ���Ⱚ AF (=>������� �Բ� �ý��� ���Ӱ�� ���⿡ ���)
} stTelcordia;

// ������ ��� k
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

    std::string filepath = "C:\\KTL\\KTL�ڵ�ȭ ���� ������(Ȱ��ȭ������_��Ī_ ���) �ڷ� ����.v4_20240407.xlsx";
    std::string Category;
    std::string Subcategory;
    std::string Division;

    app.readFile(filepath, "1. SR-332 ��ǰ �з� �� �ɵ�_���� ����");
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
            std::wstring err_msg = L"ã�� �� ���� �������Դϴ� : " + std::to_wstring(i) + L"��";
            ::MessageBox(GetActiveWindow(), err_msg.c_str(), L"������ ����", MB_OK | MB_ICONERROR);
        }
    }
    app.readEnd();

    //printf("==========ACTIVE=============\n");
    //printMap(mapActive);
    //printf("===========PASSIVE============\n");
    //printMap(mapPassive);
    //printf("============ETC===========\n");
    //printMap(mapETC);
    app.readFile(filepath, "2. Ȱ��ȭ������ ��Ī");
    std::unordered_map<std::string, double> mapCurveEV;
    for (int i = 2; i < 11; i++) {
        std::string mapEVKey = app.readCell("J", i);
        std::string mapEVValue = app.readCell("K", i);
        try {
            mapCurveEV[mapEVKey] = std::stod(mapEVValue);
        }
        catch (const std::invalid_argument& e) {
            ::MessageBoxA(GetActiveWindow(), e.what(), "Ȱ��ȭ�������� ��ȯ����(���ڰ� �ƴմϴ�)", MB_OK | MB_ICONERROR);
        }
    }

    int sheet2Row = 2;      // 2���� ����
    while (true) 
    {
        std::string cellData = app.readCell("F", sheet2Row);
        if (cellData != "") 
        {
            mapComponents[cellData].strTempCurve = app.readCell("G", sheet2Row);
            if (mapComponents[cellData].strComponentsType.find("Active") == std::string::npos) {
                mapComponents[cellData].dbModelEnergy = mapCurveEV[mapComponents[cellData].strTempCurve];       // Active�� �ƴҶ� ���ڵ��EV ����
            }

            sheet2Row++;
        }
        else {
            break;
        }
    }
    app.readEnd();

    // Active�� ���ؼ� N���� Ȱ��ȭ������ �ݿ�
    int sheet3Row = 4;          // 4���� ����
    app.readFile(filepath, "3-1. JEP122 Ea_n �� ����_�κ�(���)");
    while (true) 
    {
        break;
    }
    app.readEnd();

    // ��ǰ�� dbPeckValue ���
    for (const auto& pair : mapComponents) {
        // pair.first
        // pair.second
        //mapComponents[pair.first] = calculateAFi(H1, H2, -ni, Eai, T1, T2);
        
    }
    
    // �ý��� ���Ӱ�� ���� (double)


    return 0;
}

