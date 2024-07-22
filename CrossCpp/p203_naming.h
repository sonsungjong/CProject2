#pragma once
#include <iostream>

namespace P203 {
	char chFlag;
	bool bFlag;
	short nShort;
	int nInt;
	long long llNum;
	float fNum;
	double dbNum;
	char szBuf[50];
	std::string strBuf;
	int arrNum[3];
	int* pNum;
	static int gs_num;

	std::string strFileName;					// 파일명과 확장자
	std::string strFilePath;					// 전체경로 
	std::string strFileNameWithoutExt;			// 경로와 확장자 없이 파일명만
	std::string strFileExt;						// 확장자
	std::string strDirName;					// 폴더이름
	std::string DirPath;							// 폴더경로

	struct ST_ELEMENT {
		int nNum;
	};

	class CClass203 {
	public:
		int m_nNum;					// 변수: 명사
		static int ms_num;
		ST_ELEMENT stElement;

		int makeIntFromString(const std::string& str)			// 함수: 명령문
		{
			static int c_num;

			return c_num;
		}
	};
}