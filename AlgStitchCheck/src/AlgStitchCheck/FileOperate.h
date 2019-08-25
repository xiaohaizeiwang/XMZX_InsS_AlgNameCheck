/************************************************************************/
/* Copyright (c) 2008,����ͼ���Ӿ��������޹�˾�Ӿ�������     
/* All rights reserved.													
/*																		
/* �ļ����ƣ� FileOperate.h												
/* �ļ���ʶ�� �����ù���ƻ���
/* ժҪ�� ���漰�ļ������ĸ��ֺ������л���, �ṩ�ļ���·�����ò���,
/*    ���漰��ͼ���ļ�, �����ı��ļ��ȹ���
/*
/* ��ʼ�汾��1.0
/* �޸İ汾�� 2.0
/* ���ߣ� ��׿��
/* ������ڣ� 2005��10��10��

/* �޸ļ�¼�� 
///////////////////////////////////////////V2.4
/*       ��ԭд���ļ���ʽ��Ϊ��������ʽ   //V2.0 ���� 20151013
/*       ��־�ļ���׺��ini��Ϊlog����־�ļ���������־����ǰ׺ //V2.0 ���� 20151013
/*       �㷨����һ���ļ���ʼ���������㷨���Ӧ�����ͷź���  //V2.0 ���� 20151013
/*       ϵͳ�쳣�Ͳ�����־ʹ�÷������䣬ȥ������Ŀ¼�������յĺ���  //V2.0 ���� 20151013
/*       ������ʱ·���ı䵼���쳣 �������ļ���ǰ�����ļ�·�� ����·������//������
/*        �ڶ��ļ�������ǰ���ļ�ָ���Ƿ�Ϊ�պ��ļ��Ƿ�򿪹������ж� //V2.1 ���� 20160616
/*		 ����дϵͳ������־ʱ�ļ��رյ��ж�����bug //V2.2 ���� 20160622
/*		 �޸Ļ�ȡ������ʽ//V2.3 ���� 20170309
/*		 �޸���־�ļ����ƣ�֧��1000���ļ�//V2.4 �Ų� 20170314
///////////////////////////////////
/************************************************************************/

#if !defined(AFX_FILEOPERATE1_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9367__INCLUDED_)
#define AFX_FILEOPERATE1_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9367__INCLUDED_

#include <afxtempl.h>//Ϊ��ʹ��CList
#include <afxmt.h>
//int FileExists(CString strFileName, bool& bIsExist) ;
BOOL FormatList(CStringArray& m_aryDwBitmaps,CString csPath,CString strHeader,CString strExten);

class CFileOperate
{
public:
	CFileOperate();
	~CFileOperate();
	CString m_strLastSysOpIniPath;
	CString m_strLastSysExpIniPath;
	CString m_strLastSqlExcuteIniPath;

	CFile *m_pfileCheck;//�㷨�����־�ļ�
	CFile* m_pfileOperate;//�㷨������־�ļ�
	CFile* m_pfileExpect;//�㷨�쳣��־�ļ�

	CFile* m_pfileSystemOperate;//ϵͳ������־�ļ�
	CFile* m_pfileSystemExcept;//ϵͳ�쳣��־�ļ�
	CFile* m_pfileSystemSql;//ϵͳSQL��־�ļ�
	int m_nSqlLogNum;//Sql��־����

	CString m_strLogPath;
	//�㷨��־����ID
	int m_nCheckTxtID;
	int m_nOperateTxtID;
	int m_nExpectTxtID;
	//ϵͳ��־����ID
	int m_nSystemOperateTxtID;
	int m_nSystemExceptTxtID;

	BOOL InitFile(CString strLogPath);
	 int  FindMaxFileIndex(CFile* pfile,const CString strInput);
	 int CalFileRowNum(CFile *file,CString strFile);//�����ļ�����
	BOOL WriteLogFile(int nLogStyle,const CString& str);
	BOOL FreeFile();
	//����BMPͼ��ĺ���
	BOOL SaveBMPFile(LPCWSTR lpFileName, const BITMAPINFO *pBmpInfo, const BYTE *pImageBuffer);
	
	// ��ȡ��ִ�г������ڵ�·��
	CString GetCurrentAppPath();
	
	//�ж�·���Ƿ����
	BOOL IsPathExist(const CString &strPath);
	
	//�ж��ļ��Ƿ����
	BOOL IsFileExist(const CString &strFileName);
	
	//����һ�����Ŀ¼��������ھͲ�����
	//(strPath  �Ϸ�: "D:\\abc\\me" �� "me\\you" �� ""; 
	//			���Ϸ�: "D:\\abc\\me\\" �� "me\\you\\" �� "\\")
	BOOL CreateMultiLevelPath(const CString &strPath,BOOL bEveryone = FALSE);
	
	//дBMP�ļ�
	BOOL MyWriteBMPFile(const CString& strFileFullName, const PBITMAPINFO pBmpInfo, const BYTE *pImageBuffer);
	BOOL WriteChongzhangLog(const CString& strFilePath,const CString& strSession, const CString& str);
	//д�������е��ַ���˳��д���ļ�
	BOOL MyWriteStringFile(const CString& strFilePath, const CString& strFileName, const CList<CString,CString&>& lCheckRltStrList);
	
	// дϵͳ������־
	BOOL WriteSystemOperateLog(const CString& strFilePath,const CString& strVehicle, const CString& strSession, const CString& str);
	
	// дϵͳ�쳣��־
	BOOL WriteExceptionLog(const CString& strFilePath,const CString& strVehicle,const CString& strSession, const CString& str);
	
	//д�㷨��־
	BOOL WriteAlgLog(const CString& strFilePath,const CString& strVehicle,const CString& strSession, const CString& str);
	
	BOOL WriteAlgOperateLog(const CString& strFilePath,const CString& strVehicle,const CString& strSession, const CString& str);
	BOOL WriteAlgExceptionLog(const CString& strFilePath,const CString& strVehicle,const CString& strSession, const CString& str);
	BOOL WriteAlgCheckLog(const CString& strFilePath,const CString& strVehicle,const CString& strSession, const CString& str);
	//��ͼ���ļ�����
	BOOL OpenBMPFile(LPCWSTR lpFileName, BITMAPINFO *pBmpInfo, BYTE *pImageBuffer);
	
	// ��Ӳ�����ʶ����Դ [8/5/2013 ANRAN] дSQL��־��� [10/2/2011 ANRAN]
	BOOL WriteSqlExcuteLog(const CString& strFilePath,const CString& strVehicle, const CString& strSession, const CString& str, int iDatabaseSource = 0);
	//����ڴ�ʣ����������
	int GetSurplusMemorySize();
	
	void WINAPI GetDlgItemRect(int nID,CWnd *wndParent,LPRECT Client);
	//��õ�ǰӲ�̷�����ʣ������
	int GetSurplusCurrentPartitionSize(LPCTSTR szDrivePath);

	BOOL MyWriteIniFile(const CString& strFilePath, const CString& strSession, const CString& str);

	//ɾ����ǰĿ¼�������ļ����ļ���
	BOOL DeleteDirectory(const CString &strDirName);

};

#endif // !defined(AFX_FILEOPERATE1_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9367__INCLUDED_)