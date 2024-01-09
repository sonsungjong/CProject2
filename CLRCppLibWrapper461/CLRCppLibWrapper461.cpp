#include "pch.h"
#include "CLRCppLibWrapper461.h"

#ifdef _DEBUG
#pragma comment(lib, "Matrixd_x64.lib")
#pragma comment(lib, "MCoordd_x64.lib")
#else
#pragma comment(lib, "Matrix_x64.lib")
#pragma comment(lib, "MCoord_x64.lib")
#endif

void CMCoordUse::LLA2ECEF(double a_dbLatitude, double a_dbLongitude, double a_dbAltitude, double& a_dbOutX, double& a_dbOutY, double& a_dbOutZ)
{
    MODEL_AGENT::CMatrix posLLA(3, 1);
    posLLA.m[0][0] = a_dbLatitude;           // degree(rad)
    posLLA.m[1][0] = a_dbLongitude;           // degree(rad)
    posLLA.m[2][0] = a_dbAltitude;           // meter

    posLLA.m[0][0] *= kDeg2Rad;             // degree to radian
    posLLA.m[1][0] *= kDeg2Rad;             // degree to radian

    // get ECEF
    MODEL_AGENT::CMatrix posECEF(3, 1);
    posECEF = m_CMCoord.LLA2ECEF(posLLA);
    a_dbOutX = posECEF.m[0][0];
    a_dbOutY = posECEF.m[1][0];
    a_dbOutZ = posECEF.m[2][0];
}

void CMCoordUse::ECEF2LLA(double a_dbX, double a_dbY, double a_dbZ, double& a_dbOutLatitude, double& a_dbOutLongitude, double& a_dbOutAltitude)
{
    MODEL_AGENT::CMatrix posECEF(3, 1);
    posECEF.m[0][0] = a_dbX;             // meter
    posECEF.m[1][0] = a_dbY;             // meter
    posECEF.m[2][0] = a_dbZ;             // meter

    MODEL_AGENT::CMatrix posLLA(3, 1);
    posLLA = m_CMCoord.ECEF2LLA(posECEF);

    posLLA.m[0][0] *= kRad2Deg;     ///<radian to degree
    posLLA.m[1][0] *= kRad2Deg;     ///<radian to degree

    a_dbOutLatitude = posLLA.m[0][0];
    a_dbOutLongitude = posLLA.m[1][0];
    a_dbOutAltitude = posLLA.m[2][0];
}

// 래퍼 생성자
CMCoordUseWrapper::CMCoordUseWrapper()
{
    m_CMCoordUse = new CMCoordUse();
}

// 래퍼 소멸자
CMCoordUseWrapper::~CMCoordUseWrapper()
{
    delete m_CMCoordUse;
}

void CMCoordUseWrapper::wrapLLA2ECEF(double a_dbLatitude, double a_dbLongitude, double a_dbAltitude, double% a_dbOutX, double% a_dbOutY, double% a_dbOutZ)
{
    double x = 0.0, y = 0.0, z = 0.0;
    m_CMCoordUse->LLA2ECEF(a_dbLatitude, a_dbLongitude, a_dbAltitude, x, y, z);
    a_dbOutX = x;
    a_dbOutY = y;
    a_dbOutZ = z;
}

void CMCoordUseWrapper::wrapECEF2LLA(double a_dbX, double a_dbY, double a_dbZ, double% a_dbOutLatitude, double% a_dbOutLongitude, double% a_dbOutAltitude)
{
    double lat = 0.0, lon = 0.0, alt = 0.0;
    m_CMCoordUse->ECEF2LLA(a_dbX, a_dbY, a_dbZ, lat, lon, alt);
    a_dbOutLatitude = lat;
    a_dbOutLongitude = lon;
    a_dbOutAltitude = alt;
}

