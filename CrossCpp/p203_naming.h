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

	std::string strFileName;					// ���ϸ�� Ȯ����
	std::string strFilePath;					// ��ü��� 
	std::string strFileNameWithoutExt;			// ��ο� Ȯ���� ���� ���ϸ�
	std::string strFileExt;						// Ȯ����
	std::string strDirName;					// �����̸�
	std::string DirPath;							// �������

	struct ST_ELEMENT {
		int nNum;
	};

	class CClass203 {
	public:
		int m_nNum;					// ����: ���
		static int ms_num;
		ST_ELEMENT stElement;

		int makeIntFromString(const std::string& str)			// �Լ�: ��ɹ�
		{
			static int c_num;

			return c_num;
		}
	};
}