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
    printf("파일명: %s, 엔티티(피처) 갯수: %d\n", filename, nEntities);
    // [0]Null Shape(피처없음), [1]Point(점 피처), [3]PolyLine(선 피처), [5]Polygon(다각형 피처), [8]MultiPoint(여러개의 점피처 한번에)
    // [11]PointZ(점 with Z값), [13]PolyLineZ(선 with Z값), [15]PolygonZ(다각형 with Z값), [21]PointM(점 with 추가정보측정값), [23]PolyLineM(선 with 추가정보측정값), [25]PolygonM(다각형 with 추가정보측정값)
    printf("도형 타입: %d\n", nShapeType);

    std::unordered_map<int, std::vector<std::pair<float, float>>> mapVertices;

    for (int i = 0; i < nEntities; i++) {
        SHPObject* pShape = SHPReadObject(hSHP, i);
        if (pShape == NULL) {
            printf("Failed to read shape at index %d\n", i);
            continue;
        }

        //printf("Entity %d, 꼭지점수: %d\n", i, pShape->nVertices);
        ////printf("한 엔티티의 Bounding box: (%f, %f) to (%f, %f)\n", pShape->dfXMin, pShape->dfYMin, pShape->dfXMax, pShape->dfYMax);
        std::vector<std::pair<float, float>> vecVertices;
        vecVertices.resize(pShape->nVertices);
        for (int j = 0; j < pShape->nVertices; j++) {
            vecVertices[j] = std::make_pair(static_cast<float>(pShape->padfX[j]), static_cast<float>(pShape->padfY[j]));
        }

        mapVertices.emplace(i, std::move(vecVertices));         // 소유권 이전

        SHPDestroyObject(pShape);
    }

    SHPClose(hSHP);

    // mapVertices 출력
    for (const auto& entity : mapVertices)
    {
        int nEntityID = entity.first;
        const std::vector<std::pair<float, float>>& vertices = entity.second;

        
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(6);              // 고정소수점 형식 6자리까지

        oss << "\n";
        //printf("\n");
        for (size_t k = 0; k < vertices.size(); ++k) {
            //printf("(%f, %f)\t", vertices[k].first, vertices[k].second);
            oss << "(" << vertices[k].first << ", " << vertices[k].second << ")\t";
        }
        //printf("\n");
        oss << "\n";
        //std::cout << oss.str();             // 버퍼링으로 모아서 출력
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
    filesystem으로 폴더 안에 있는 모든 파일을 찾아
    shp와 dbf 만 처리한다
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

    // 작업이 완료될때까지 대기
    //for (auto& future : vecFuture) {
    //    future.get();
    //}

    printf("SHP : %d개, DBF : %d개\n", shpCount, dbfCount);
}

int main()
{
    auto startTime = std::chrono::high_resolution_clock::now();
    //const char* shp_filepath = "C:\\kmap_2023_120_korean_shp\\KOR_ADMIN_AS_국문.shp";
    //const char* dbf_filepath = "C:\\kmap_2023_120_korean_shp\\KOR_ADMIN_AS_국문.dbf";
    //readShapeFile(shp_filepath);
    //readDBFFile(dbf_filepath);

    //std::string strWorldShapeDirectoryPath = "C:\\world_m2023_english_shp";
    std::string strKorShapeDirectoryPath = "C:\\kmap_2023_120_korean_shp";
    readShapeDirectory(strKorShapeDirectoryPath);

    auto endTime = std::chrono::high_resolution_clock::now();
    printf("소요시간: %fms\n", std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(endTime - startTime).count());

    return 0;
}
