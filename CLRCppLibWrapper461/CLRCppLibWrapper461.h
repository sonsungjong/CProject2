#ifndef CLR_CPP_LIB_WRAPPER_H_
#define CLR_CPP_LIB_WRAPPER_H_

using namespace System;
#include <MCoord.h>

class CMCoordUse
{
public:
	void LLA2ECEF(double a_dbLatitude, double a_dbLongitude, double a_dbAltitude, double& a_dbOutX, double& a_dbOutY, double& a_dbOutZ);
	void ECEF2LLA(double a_dbX, double a_dbY, double a_dbZ, double& a_dbOutLatitude, double& a_dbOutLongitude, double& a_dbOutAltitude);

private:
	MODEL_AGENT::CMCoord m_CMCoord;
};

public ref class CMCoordUseWrapper
{
public:
	CMCoordUseWrapper();
	virtual ~CMCoordUseWrapper();
	void wrapECEF2LLA(double a_dbX, double a_dbY, double a_dbZ, double% a_dbOutLatitude, double% a_dbOutLongitude, double% a_dbOutAltitude);
	void wrapLLA2ECEF(double a_dbLatitude, double a_dbLongitude, double a_dbAltitude, double% a_dbOutX, double% a_dbOutY, double% a_dbOutZ);
private:
	CMCoordUse* m_CMCoordUse;			// 네이티브 클래스 포인터
};

#endif				CLR_CPP_LIB_WRAPPER_H_