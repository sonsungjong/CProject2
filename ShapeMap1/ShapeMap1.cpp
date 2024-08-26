#include "pch.h"
#include <shapefil.h>

#ifdef _DEBUG
#pragma comment(lib, "shpd.lib")
#else
#pragma comment(lib, "shp.lib")
#endif

void readShapeFile(const char* filename) {
    SHPHandle hSHP = SHPOpen(filename, "rb");
    if (hSHP == NULL) {
        printf("Failed to open SHP file: %s\n", filename);
        return;
    }

    int nEntities, nShapeType;
    SHPGetInfo(hSHP, &nEntities, &nShapeType, NULL, NULL);
    printf("���ϸ�: %s, ��ƼƼ(��ó) ����: %d\n", filename, nEntities);
    // [0]Null Shape(��ó����), [1]Point(�� ��ó), [3]PolyLine(�� ��ó), [5]Polygon(�ٰ��� ��ó), [8]MultiPoint(�������� ����ó �ѹ���)
    // [11]PointZ(�� with Z��), [13]PolyLineZ(�� with Z��), [15]PolygonZ(�ٰ��� with Z��), [21]PointM(�� with �߰�����������), [23]PolyLineM(�� with �߰�����������), [25]PolygonM(�ٰ��� with �߰�����������)
    printf("���� Ÿ��: %d\n", nShapeType);

    std::unordered_map<int, std::vector<std::pair<float, float>>> mapVertices;

    for (int i = 0; i < nEntities; i++) {
        SHPObject* pShape = SHPReadObject(hSHP, i);
        if (pShape == NULL) {
            printf("Failed to read shape at index %d\n", i);
            continue;
        }

        //printf("Entity %d, ��������: %d\n", i, pShape->nVertices);
        ////printf("�� ��ƼƼ�� Bounding box: (%f, %f) to (%f, %f)\n", pShape->dfXMin, pShape->dfYMin, pShape->dfXMax, pShape->dfYMax);
        std::vector<std::pair<float, float>> vecVertices;
        vecVertices.resize(pShape->nVertices);
        for (int j = 0; j < pShape->nVertices; j++) {
            vecVertices[j] = std::make_pair(static_cast<float>(pShape->padfX[j]), static_cast<float>(pShape->padfY[j]));
        }

        mapVertices.emplace(i, std::move(vecVertices));         // ������ ����

        SHPDestroyObject(pShape);
    }

    SHPClose(hSHP);

    // mapVertices ���
    for (const auto& entity : mapVertices)
    {
        int nEntityID = entity.first;
        const std::vector<std::pair<float, float>>& vertices = entity.second;

        
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6);              // �����Ҽ��� ���� 6�ڸ�����

        oss << "\n";
        //printf("\n");
        for (size_t k = 0; k < vertices.size(); ++k) {
            //printf("(%f, %f)\t", vertices[k].first, vertices[k].second);
            oss << "(" << vertices[k].first << ", " << vertices[k].second << ")\t";
        }
        //printf("\n");
        oss << "\n";
        //std::cout << oss.str();             // ���۸����� ��Ƽ� ���
    }
}

void readDBFFile(const char* filename) {
    DBFHandle hDBF = DBFOpen(filename, "rb");
    if (hDBF == NULL) {
        printf("Failed to open DBF file: %s\n", filename);
        return;
    }

    int nRecordCount = DBFGetRecordCount(hDBF);
    int nFieldCount = DBFGetFieldCount(hDBF);
    printf("File: %s, Records: %d, Fields: %d\n", filename, nRecordCount, nFieldCount);

    for (int i = 0; i < nFieldCount; i++) 
    {
        char szFieldName[12]{ 0 };
        int nWidth, nDecimals;
        DBFFieldType enumType = DBFGetFieldInfo(hDBF, i, szFieldName, &nWidth, &nDecimals);
        printf("Field %d: Name = %s, Type = %d, Width = %d, Decimals = %d\n", i, szFieldName, enumType, nWidth, nDecimals);
    }

    DBFClose(hDBF);
}

/*
    filesystem���� ���� �ȿ� �ִ� ��� ������ ã��
    shp�� dbf �� ó���Ѵ�
*/
void readShapeDirectory(const std::string& _strDirectoryPath)
{
    static int shpCount = 0;
    static int dbfCount = 0;

    //std::vector<std::future<void>> vecFuture;

    for (const auto& entry : std::filesystem::directory_iterator(_strDirectoryPath))
    {
        if (entry.is_regular_file()) 
        {
            std::string fileFullPath = entry.path().string();
            if (entry.path().extension() == ".shp") 
            {
                readShapeFile(fileFullPath.c_str());
                //vecFuture.emplace_back(std::async(std::launch::async, readShapeFile, fileFullPath.c_str()));
                shpCount++;
            }
            else if (entry.path().extension() == ".dbf") 
            {
                readDBFFile(fileFullPath.c_str());
                //vecFuture.emplace_back(std::async(std::launch::async, readDBFFile, fileFullPath.c_str()));
                dbfCount++;
            }
        }
    }

    // �۾��� �Ϸ�ɶ����� ���
    //for (auto& future : vecFuture) {
    //    future.get();
    //}

    printf("SHP : %d��, DBF : %d��\n", shpCount, dbfCount);
}

int main()
{
    auto startTime = std::chrono::high_resolution_clock::now();
    //const char* shp_filepath = "C:\\kmap_2023_120_korean_shp\\KOR_ADMIN_AS_����.shp";
    //const char* dbf_filepath = "C:\\kmap_2023_120_korean_shp\\KOR_ADMIN_AS_����.dbf";
    //readShapeFile(shp_filepath);
    //readDBFFile(dbf_filepath);

    //std::string strWorldShapeDirectoryPath = "C:\\world_m2023_english_shp";
    std::string strKorShapeDirectoryPath = "C:\\kmap_2023_120_korean_shp";
    readShapeDirectory(strKorShapeDirectoryPath);

    auto endTime = std::chrono::high_resolution_clock::now();
    printf("�ҿ�ð�: %fms\n", std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count());

    return 0;
}
