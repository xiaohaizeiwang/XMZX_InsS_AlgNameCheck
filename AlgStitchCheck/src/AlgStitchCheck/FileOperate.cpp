/************************************************************************/
/* Copyright (c) 2005, �й���㼯�ű���ͼ���Ӿ������ֹ�˾�Ӿ�������     
/* All rights reserved.													
/*																		
/* �ļ����ƣ� FileOperate.cpp											
/* �ļ���ʶ�� �����ù���ƻ���
/* ժҪ�� ���漰�ļ������ĸ��ֺ������л���, �ṩ�ļ���·�����ò���,
/*    ���漰��ͼ���ļ�, �����ı��ļ��ȹ��� 
/*
/* ��ǰ�汾�� 1.0
/* ���ߣ� ��׿��
/* �޸ģ�����1.1  2015-10-08
/*       dhl 1.2  �޸��㷨��־�ļ��������½��ļ�����bug
/*       �Ų�2.4  �޸���־�ļ����ƣ�֧��1000���ļ�//20170314
/* ������ڣ� 2005��10��10��
/************************************************************************/

#include "stdafx.h"
#include "FileOperate.h"
#include "io.h"
//int FileExists(CString strFileName, bool& bIsExist) 
//{
//	//char *filename;
//	int nRet = 0;
//	bIsExist = false;
//	if (_access(strFileName, 0) == 0)
//	{
//		bIsExist = true;
//	}
//	return nRet;
//}

BOOL FormatList(CStringArray& m_aryDwBitmaps,CString csPath,CString strHeader,CString strExten)
{
	if (csPath.GetAt(csPath.GetLength()-1) == '\\')
	{
		CString strFind;
		strFind.Format(_T("%s*%s"),strHeader,strExten);
		csPath += strFind;
	}
	else
	{
		CString strFind;
		strFind.Format(_T("\\%s*%s"),strHeader,strExten);
		csPath += strFind;
	}
	m_aryDwBitmaps.RemoveAll();
	CFileFind FoundFiles;
	BOOL bContinue = FoundFiles.FindFile(csPath);
	while (bContinue == TRUE)
	{
		bContinue = FoundFiles.FindNextFile();

		if (FoundFiles.IsDirectory())
		{
			if (FoundFiles.IsDots())
				continue;
		}
		CString str;
		CString strFileName=FoundFiles.GetFileName();
		CString strExt = strFileName.Right(strExten.GetLength());		 
		if(strFileName.Find(strExten)>=0&&strFileName.Find(strHeader)>=0)
		{
			str+=strFileName;
			m_aryDwBitmaps.Add(str);
		}
	}
	FoundFiles.Close();
	return bContinue;
}
CFileOperate::CFileOperate()
{
	m_nCheckTxtID=0;
	m_nOperateTxtID=0;
	m_nExpectTxtID=0;
	m_nSystemOperateTxtID=0;
	m_nSystemExceptTxtID=0;
	m_strLastSysOpIniPath= _T("");
	m_strLastSysExpIniPath= _T("");
	m_strLastSqlExcuteIniPath= _T("");
	m_pfileCheck=NULL;
	m_pfileOperate=NULL;
	m_pfileExpect=NULL;
	m_pfileSystemExcept=NULL;
	m_pfileSystemOperate=NULL;
	m_pfileSystemSql=NULL;
	m_nSqlLogNum=0;
}
CFileOperate::~CFileOperate()
{
	
}
 BOOL CFileOperate::InitFile(CString strLogPath)
 {
	 FreeFile();
	 m_pfileCheck=new CFile;
	 m_pfileOperate=new CFile;
	 m_pfileExpect=new CFile;
	
	 m_pfileSystemExcept=new CFile;
	 m_pfileSystemOperate=new CFile;
	 m_pfileSystemSql=new CFile;

	 m_strLogPath=strLogPath;
	  CString strCheckLog,strOperateLog,strExpLog,strTimeCheck,strTimeOperate,strTimeExp;
	  CString strTimeSystemOperation,strTimeSystemException;
	  CString strSystemOperationLog,strSystemExceptionLog;
	  SYSTEMTIME curTime;
	  GetLocalTime(&curTime);
	  CString strTime,strTimeSystem;
	  strTime.Format(_T("%04d\\%02d\\%02d\\"),curTime.wYear,curTime.wMonth, curTime.wDay);
	  strCheckLog=strLogPath+strTime+ _T("CheckLog");
	  strOperateLog=strLogPath+strTime+ _T("OperateLog");
	  strExpLog=strLogPath+strTime+ _T("ExceptLog");

	  //strTimeSystem.Format("%04d-%02d\\%02d\\",curTime.wYear,curTime.wMonth, curTime.wDay);
	  //CString strEmpty;
	  //strEmpty.Format("");
	  //if(m_strLastSysOpIniPath==strEmpty)
	  //{
		 // strSystemOperationLog=strLogPath+"SystemOperationLogFile\\"+strTimeSystem;
		 // strSystemExceptionLog=strLogPath+"ExceptionLogFile\\"+strTimeSystem;
	  //}
	  //else
	  //{
		 // strSystemOperationLog=m_strLastSysOpIniPath;
	  //}
	 

	  BOOL bRet=IsPathExist(strCheckLog);
	  if(!bRet)
	  {
		  bRet=CreateMultiLevelPath(strCheckLog);
		  if(!bRet)
		  {
			  return FALSE;
		  }
	  }
	  else
	  {
		  m_nCheckTxtID=FindMaxFileIndex(m_pfileCheck,strCheckLog);
	  }
	  bRet=IsPathExist(strOperateLog);
	  if(!bRet)
	  {
		  bRet=CreateMultiLevelPath(strOperateLog);
		  if(!bRet)
		  {
			  return FALSE;
		  }
	  }
	  else
	  {
		   m_nOperateTxtID=FindMaxFileIndex(m_pfileOperate,strOperateLog);
	  }
	  bRet=IsPathExist(strExpLog);
	  if(!bRet)
	  {
		  bRet=CreateMultiLevelPath(strExpLog);
		  if(!bRet)
		  {
			  return FALSE;
		  }
	  }
	  else
	  {
		  m_nExpectTxtID=FindMaxFileIndex(m_pfileExpect,strExpLog);
	  }

	  //����ϵͳ��־Ŀ¼
	 // bRet=IsPathExist(strSystemOperationLog);
	 // if(!bRet)
	 // {
		//  bRet=CreateMultiLevelPath(strSystemOperationLog);
		//  if(!bRet)
		//  {
		//	  return FALSE;
		//  }
	 // }
	 // else
	 // {
		//  m_nSystemOperateTxtID=FindMaxFileIndex(strSystemOperationLog);
	 // }
	 // bRet=IsPathExist(strSystemExceptionLog);
	 // if(!bRet)
	 // {
		//  bRet=CreateMultiLevelPath(strSystemExceptionLog);
		//  if(!bRet)
		//  {
		//	  return FALSE;
		//  }
	 // }
	 // else
	 // {
		// m_nSystemExceptTxtID=FindMaxFileIndex(strSystemExceptionLog);
	 // }

	  strTimeCheck.Format(_T("CheckLog-%4d%s%02d%s%02d%s%02d%s%03d%s"), curTime.wYear, _T("-"), curTime.wMonth, _T("-"), curTime.wDay, _T("-"),curTime.wHour, _T("-"),m_nCheckTxtID, _T(".log"));
	  strTimeOperate.Format(_T("OperateLog-%4d%s%02d%s%02d%s%02d%s%03d%s"), curTime.wYear, _T("-"), curTime.wMonth, _T("-"), curTime.wDay, _T("-"),curTime.wHour, _T("-"),m_nOperateTxtID, _T(".log"));
	  strTimeExp.Format(_T("ExceptLog-%4d%s%02d%s%02d%s%02d%s%03d%s"), curTime.wYear, _T("-"), curTime.wMonth, _T("-"), curTime.wDay, _T("-"),curTime.wHour,_T("-"),m_nExpectTxtID, _T(".log"));
	  strCheckLog=strCheckLog+ _T("\\")+strTimeCheck;
	  strOperateLog=strOperateLog+ _T("\\")+strTimeOperate;
	  strExpLog=strExpLog+ _T("\\")+strTimeExp;
	  //����ϵͳ��־�ļ�
	 /* strTimeSystemOperation.Format("%4d%s%02d%s%02d%s%02d%s", curTime.wYear, "-", curTime.wMonth, "-", curTime.wDay,"-",m_nSystemOperateTxtID,".log");
	  strTimeSystemException.Format("%4d%s%02d%s%02d%s%02d%s", curTime.wYear, "-", curTime.wMonth, "-", curTime.wDay,"-",m_nSystemExceptTxtID,".log");
	  strSystemOperationLog=strSystemOperationLog+strTimeSystemOperation;
	  strSystemExceptionLog=strSystemExceptionLog+strTimeSystemException;*/
	  try
	  {
		  if(*m_pfileCheck==CFile::hFileNull)
		  {
			  m_pfileCheck->Open (strCheckLog,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone);
		  }
		  if(*m_pfileExpect==CFile::hFileNull)
		  {
			  m_pfileExpect->Open (strExpLog,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone);
		  }
		  if(*m_pfileOperate==CFile::hFileNull)
		  {
			 m_pfileOperate->Open (strOperateLog,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone);
		  }
		  //ϵͳ��־
		 /* if(m_pfileSystemOperate==CFile::hFileNull)
		  {
			  m_pfileSystemOperate.Open (strSystemOperationLog,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone);
		  }
		  if(m_pfileSystemExcept==CFile::hFileNull)
		  {
			  m_pfileSystemExcept.Open (strSystemExceptionLog,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone);
		  }*/
	  }
	  catch(...)
	  {
			return FALSE;
	  }
	  return TRUE;
 }
 int  CFileOperate::FindMaxFileIndex(CFile* pfile,const CString strInput)
 {
	 int maxIndex=-1;
	 try
	 {
		 CFileFind finder;
		 CString str,strDir;
		 strDir=strInput;
		 if( strDir.Right(1) != _T("\\") )
			 strDir += "\\";
		 strDir += _T("*.log");
		 bool bWorking = finder.FindFile( strDir) ;
		 int nFileRowNum=0;
		 CString DataFile=_T("");
		 while (bWorking)
		 {
			 bWorking= finder.FindNextFile();
			 DataFile= finder.GetFilePath() ; //GetFilePath���õ�ȫ·���� 
			 CString FileTitle = finder.GetFileTitle() ;//GetFileTitle:�õ�������׺�ļ���  
			 CString FileName = finder.GetFileName();   //GetFileName:�õ�����׺���ļ��� 
			 int nposR=FileName.Find('.');
			 int nposL=FileName.ReverseFind('-');
			 CString strIndex=FileName.Mid(nposL+1, nposR-1);
			 int Index=_ttoi(strIndex);
			 if(Index>maxIndex)
			 {
				 maxIndex=Index;
			 }
		 }
		 if(maxIndex>=0)
		 {
			 try
			 {
				 nFileRowNum=CalFileRowNum(pfile,DataFile);
			 }
			 catch(...)
			 {
				 AfxMessageBox(_T("CalFileRowNum�쳣"));
			 }
		 }
		 else
		 {
			 maxIndex=0;
		 }
		 finder.Close();
	 }
	 catch(...)
	 {
		 AfxMessageBox(_T("FindMaxFileIndex�쳣"));
	 }
	 return maxIndex;
 }
  BOOL CFileOperate::WriteLogFile(int logStyle,const CString& str)
  {
	  try
	  {
		  static int iWriteNumCheck = 0;
		  static int iWriteNumOp = 0;
		  static int iWriteNumExp = 0;
		  SYSTEMTIME curTime;
		  GetLocalTime(&curTime);
		   CString strTime;
		   CString strEnter;
		   strEnter.Format(_T("\r\n"));
		  int  nStrLength;
		  //����1
		  //int len = WideCharToMultiByte(CP_ACP, 0, str, str.GetLength(), NULL, 0, NULL, NULL); //��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����
		  //nStrLength = len + 1;	  
		  //char * pcStr = new char[len + 1];
		  //WideCharToMultiByte(CP_ACP, 0, str, str.GetLength() + 1, pcStr, len + 1, NULL, NULL);		  //const char* pcStr=(LPSTR)(LPCTSTR)str;
		  //pcStr[len] = '\0';   //���ֽ��ַ���'\0'����
		//����2
		  int len = WideCharToMultiByte(CP_ACP, 0, str, str.GetLength(), NULL, 0, NULL, NULL); //��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����
		  nStrLength = len + 1;
		  CStringA strPathA(str);
		  const char* pcStr = strPathA.GetString();

		  int nStrEnterLength/* = strEnter.GetLength()*/;
		  //const char* pcEnter = (LPSTR)(LPCTSTR)strEnter;

		  /*int*/ len = WideCharToMultiByte(CP_ACP, 0, strEnter, strEnter.GetLength(), NULL, 0, NULL, NULL); //��ȡ���ֽ��ַ��Ĵ�С����С�ǰ��ֽڼ����
		  nStrEnterLength = len;
		  CStringA strPathAEn(strEnter);
		  const char* pcEnter = strPathAEn.GetString();

		  CString strFileName;
		  CString strTimeLog,strCheckLog,strOperateLog,strExpLog;
		  strTimeLog.Format(_T("%04d\\%02d\\%02d\\"),curTime.wYear,curTime.wMonth, curTime.wDay);
		  strCheckLog=m_strLogPath+strTimeLog+ _T("CheckLog");
		  strOperateLog=m_strLogPath+strTimeLog+ _T("OperateLog");
		  strExpLog=m_strLogPath+strTimeLog+ _T("ExceptLog");

		  if(logStyle==1)
		  {
			  if(m_pfileCheck==NULL)
			  {
				  m_pfileCheck=new CFile;
			  }
			  if(iWriteNumCheck>=4000)
			  {
				  if(*m_pfileCheck!=CFile::hFileNull)
				  {
					   m_pfileCheck->Close();
				  }
				  m_nCheckTxtID++;
				  strFileName.Format(_T("CheckLog-%4d%s%02d%s%02d%s%02d%s%03d%s"), curTime.wYear, _T("-"), curTime.wMonth, _T("-"), curTime.wDay, _T("-"),curTime.wHour, _T("-"),m_nCheckTxtID, _T(".log"));
				 // InitFile(m_strLogPath);
				  strFileName=strCheckLog+ _T("\\")+strFileName;
				  CreateMultiLevelPath(strCheckLog);
				  if(*m_pfileCheck==CFile::hFileNull)
				  {
					  m_pfileCheck->Open(strFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone);
				  }
				  iWriteNumCheck=0;
			  }
			  strTime.Format(_T("%02d%s%02d%s%02d%s%03d_%02d="),curTime.wHour, _T(":"), curTime.wMinute, _T(":"), curTime.wSecond, _T(":"),
				  curTime.wMilliseconds,iWriteNumCheck);
		/*	  int nStrTimeLength=strTime.GetLength();
			 const char* pcLogTime=(LPSTR)(LPCTSTR)strTime;*/

			 int nStrTimeLength;
			 nStrTimeLength = strTime.GetLength() + 1;//��һλ
			 CStringA strPathAT(strTime);
			 const char* pcLogTime = strPathAT.GetString();

			  m_pfileCheck->Write(pcLogTime,sizeof(char)*nStrTimeLength);

			  m_pfileCheck->Write(pcStr,sizeof(char)*nStrLength);

			  m_pfileCheck->Write(pcEnter,sizeof(char)*nStrEnterLength);

			   iWriteNumCheck++;
		  }
		  if(logStyle==2)
		  {
			  if(m_pfileOperate==NULL)
			  {
				  m_pfileOperate=new CFile;
			  }
			  if(iWriteNumOp>=4000)
			  {
				  if(*m_pfileOperate!=CFile::hFileNull)
				  {
					  m_pfileOperate->Close();
				  }
				  
				  m_nOperateTxtID++;
				 // InitFile(m_strLogPath);
				  strFileName.Format(_T("OperateLog-%4d%s%02d%s%02d%s%02d%s%03d%s"), curTime.wYear, _T("-"), curTime.wMonth, _T("-"), curTime.wDay, _T("-"), curTime.wHour, _T("-"),m_nOperateTxtID, _T(".log"));
				  strFileName=strOperateLog+ _T("\\")+strFileName;
				  CreateMultiLevelPath(strOperateLog);
				  if(*m_pfileOperate==CFile::hFileNull)
				  {
					  m_pfileOperate->Open(strFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone);
				  }
				  iWriteNumOp=0;
			  }
			  strTime.Format(_T("%02d%s%02d%s%02d%s%03d_%02d="),curTime.wHour, _T(":"), curTime.wMinute, _T(":"), curTime.wSecond, _T(":"),
				  curTime.wMilliseconds,iWriteNumOp);
			  int nStrTimeLength;
			  nStrTimeLength = strTime.GetLength()+1;//��һλ
			  CStringA strPathAT(strTime);
			  const char* pcLogTime = strPathAT.GetString();

			  m_pfileOperate->Write(pcLogTime,sizeof(char)*nStrTimeLength);

			  m_pfileOperate->Write(pcStr,sizeof(char)*nStrLength);
			  m_pfileOperate->Write(pcEnter,sizeof(char)*nStrEnterLength);
			  iWriteNumOp++;
		  }
		  if(logStyle==3)
		  {
			  if(m_pfileExpect==NULL)
			  {
				  m_pfileExpect=new CFile;
			  }
			  if(iWriteNumExp>=4000)
			  {
				   if(*m_pfileExpect!=CFile::hFileNull)
				   {
					   m_pfileExpect->Close();
				   }
				   
				   m_nExpectTxtID++;
				  // InitFile(m_strLogPath);
				   strFileName.Format(_T("ExceptLog-%4d%s%02d%s%02d%s%02d%s%03d%s"), curTime.wYear, _T("-"), curTime.wMonth, _T("-"), curTime.wDay, _T("-"), curTime.wHour, _T("-"),m_nExpectTxtID, _T(".log"));
				   strFileName=strExpLog+ _T("\\")+strFileName;
				   CreateMultiLevelPath(strExpLog);
				   if(*m_pfileExpect==CFile::hFileNull)
				   {
					    m_pfileExpect->Open(strFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone);
				   }
				   iWriteNumExp=0;
			  }
			  strTime.Format(_T("%02d%s%02d%s%02d%s%03d_%02d="),curTime.wHour, _T(":"), curTime.wMinute, _T(":"), curTime.wSecond, _T(":"),
				  curTime.wMilliseconds,iWriteNumExp);
			  int nStrTimeLength;
			  nStrTimeLength = strTime.GetLength() + 1;//��һλ
			  CStringA strPathAT(strTime);
			  const char* pcLogTime = strPathAT.GetString();

			  m_pfileExpect->Write(pcLogTime,sizeof(char)*nStrTimeLength);

			  m_pfileExpect->Write(pcStr,sizeof(char)*nStrLength);
			  m_pfileExpect->Write(pcEnter,sizeof(char)*nStrEnterLength);
			  iWriteNumExp++;
		  }
		  //�ͷ�
		 // delete[] pcStr;
	  } 
	  catch(...)
	  {
		  return FALSE;
	  }
		return TRUE;
  }
  BOOL CFileOperate::FreeFile()
  {
	  if(m_pfileCheck!=NULL)
	  {
		  if(*m_pfileCheck!=CFile::hFileNull)
		  {
			  m_nCheckTxtID=0;
			  m_pfileCheck->Close();
		  }
	  }
	  if(m_pfileExpect!=NULL)
	  {
		  if(*m_pfileExpect!=CFile::hFileNull)
		  {
			  m_nExpectTxtID=0;
			  m_pfileExpect->Close();
		  }
	  }
	  if(m_pfileOperate!=NULL)
	  {
		  if(*m_pfileOperate!=CFile::hFileNull)
		  {
			  m_nOperateTxtID=0;
			  m_pfileOperate->Close();
		  }
	  }
	 
	  if(m_pfileCheck)
	  {
		  delete m_pfileCheck;
		  m_pfileCheck=NULL;
	  }
	  if(m_pfileOperate)
	  {
		  delete m_pfileOperate;
		  m_pfileOperate=NULL;
	  }
	  if(m_pfileExpect)
	  {
		  delete m_pfileExpect;
		  m_pfileExpect=NULL;
	  }
	  
	if(m_pfileSystemOperate!=NULL)
	{
		if(*m_pfileSystemOperate!=CFile::hFileNull)
		{
			m_nSystemOperateTxtID=0;
			m_pfileSystemOperate->Close();
		}
	}
	if(m_pfileSystemExcept!=NULL)
	{
		if(*m_pfileSystemExcept!=CFile::hFileNull)
		{
			m_nSystemExceptTxtID=0;
			m_pfileSystemExcept->Close();
		}
	}
	if(m_pfileSystemSql)
	{
		if(*m_pfileSystemSql!=CFile::hFileNull)
		{
			CString strContent;
			strContent.Format(_T("����sql����=%d"),m_nSqlLogNum);
			int  nStrContentLength=strContent.GetLength();
			const char* pcStrContent=(LPSTR)(LPCTSTR)strContent;
			m_pfileSystemSql->Write(pcStrContent,sizeof(char)*nStrContentLength);
			m_pfileSystemSql->Close();
		}
	}
	if(m_pfileSystemExcept)
	{
		delete m_pfileSystemExcept;
		m_pfileSystemExcept=NULL;
	}
	if(m_pfileSystemOperate)
	{
		delete m_pfileSystemOperate;
		m_pfileSystemOperate=NULL;
	}
	if(m_pfileSystemSql)
	{
		delete m_pfileSystemSql;
		m_pfileSystemSql=NULL;
	} 
		 
		return TRUE;
  }
//����BMPͼ��ĺ���
BOOL CFileOperate::SaveBMPFile(LPCWSTR lpFileName, const BITMAPINFO *pBmpInfo, const BYTE *pImageBuffer)
{
	BOOL bRVal				= TRUE;
	DWORD dwBytesRead		= 0;
	DWORD dwSize			= 0;
	BITMAPFILEHEADER bfh	= {0};
	int nTable				= 0;
	DWORD dwImageSize		= 0;
	
	if (pBmpInfo->bmiHeader.biBitCount > 8) 
	{
		nTable = 0;
	}
	else
	{
		nTable = 256;
	}
	
	dwImageSize =  (pBmpInfo->bmiHeader.biWidth * pBmpInfo->bmiHeader.biHeight) * ((pBmpInfo->bmiHeader.biBitCount + 7) / 
		
		8);
	
	if (dwImageSize <= 0) 
	{
		bRVal = FALSE;
	}
	else{
		bfh.bfType		= (WORD)'M' << 8 | 'B';
		bfh.bfOffBits	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nTable * sizeof(RGBQUAD);
		bfh.bfSize		= bfh.bfOffBits + dwImageSize;			
		
		HANDLE hFile = ::CreateFile(lpFileName,
			GENERIC_WRITE ,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
		if (hFile == INVALID_HANDLE_VALUE) {
			bRVal = FALSE;
		}
		else{
			dwSize = sizeof(BITMAPFILEHEADER);
			::WriteFile(hFile, &bfh, dwSize, &dwBytesRead, NULL );
			
			dwSize = sizeof(BITMAPINFOHEADER) + nTable * sizeof(RGBQUAD);
			::WriteFile(hFile, pBmpInfo, dwSize, &dwBytesRead, NULL );
			
			dwSize = dwImageSize;
			WriteFile(hFile, pImageBuffer, dwSize, &dwBytesRead, NULL );
			
			CloseHandle(hFile);
		}
	}
	
	return bRVal;
}

//******************************************************************************//
//������
//  ��ÿռ�Ŀͻ�����
//������
// nID   �ؼ�ID
// wndParent ����
// Client  �ͻ�����
//���أ�
//******************************************************************************//
void WINAPI CFileOperate::GetDlgItemRect(int nID,CWnd *wndParent,LPRECT Client)
{
	CRect WinRect,ClientRect;
	wndParent->GetClientRect(&WinRect);
	wndParent->ClientToScreen(&WinRect);
	wndParent->GetDlgItem(nID)->GetWindowRect(&ClientRect);
	Client->left=ClientRect.left-WinRect.left;
	Client->top=ClientRect.top-WinRect.top;
	Client->right=ClientRect.right-WinRect.left;
	Client->bottom=ClientRect.bottom-WinRect.top;
}

// ��ȡ��ִ�г������ڵ�·��
CString CFileOperate::GetCurrentAppPath()
{
	CString strAppName;
	GetModuleFileName(NULL, strAppName.GetBuffer(_MAX_PATH), _MAX_PATH);
	strAppName.ReleaseBuffer();
	int nPos = strAppName.ReverseFind(_T('\\'));
	strAppName = strAppName.Left(nPos + 1);

	return strAppName;
}
//#include <io.h>//ͷ�ļ�

//�ж�·���Ƿ����
BOOL CFileOperate::IsPathExist(const CString &strPath)
{
	
#ifdef _UNICODE
#define IsDirectoryExist(path) ::_waccess(path,0) == 0
	//_waccess�ĵڶ���������ʾ���жϴ����ԣ�����0��ʾ���ڣ�����-1��ʾ������
#else
#define IsDirectoryExist(path) ::_access(path,0) == 0//_access��MBCS�汾
#endif

	if(IsDirectoryExist(strPath))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//�ж��ļ��Ƿ����
BOOL CFileOperate::IsFileExist(const CString &strFileName)
{
	CFileFind searcher;

	if(searcher.FindFile(strFileName))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//����һ�����Ŀ¼��������ھͲ�����
//(strPath  �Ϸ�: "D:\\abc\\me" �� "me\\you" �� ""; 
//			���Ϸ�: "D:\\abc\\me\\" �� "me\\you\\" �� "\\")
BOOL CFileOperate::CreateMultiLevelPath(const CString &strPath,BOOL bEveryone)
{
	CString tmpPath;// = strPath;
	tmpPath.Format(_T("%s"),strPath);
	CList<CString,CString&> FolderStrList;
	int nStrLen = 0;

	for(;;)
	{
		nStrLen = tmpPath.GetLength();
		if(nStrLen <= 0)
		{
			break;//��ͷ���ļ���
		}		

		BOOL bPathExist = FALSE;//��·���Ƿ��Ѿ�����

		if(tmpPath.Right(1) == _T(":"))
		{
			bPathExist = IsPathExist(tmpPath + _T("\\"));
		}
		else
		{
			bPathExist = IsPathExist(tmpPath);
		}

		if(bPathExist)//���·������
		{
			break;//���Ѵ��ڵ�·���½��ļ���
		}

		//���·��������		
		//ÿ�ν�����һ���ļ��������

		int nPos = 0;
		nPos = tmpPath.ReverseFind('\\');//�Ӻ���ǰȷ���ļ�����
		if(nPos == -1)//�ѵ����һ���ļ�����
		{
			nPos = 0;//����һ���ļ��������
		}

		//���ļ��������
		CString tmpStr = tmpPath.Right(nStrLen - nPos);
		FolderStrList.AddHead(tmpStr);
		tmpPath.Delete(nPos, nStrLen - nPos);		
	}	

	//�����ļ���������˳����
	while(FolderStrList.IsEmpty() == FALSE)
	{
		tmpPath += FolderStrList.RemoveHead();
		BOOL bRet = FALSE;

		if(bEveryone)
		{
			SECURITY_ATTRIBUTES sa;
			SECURITY_DESCRIPTOR sd1;
			InitializeSecurityDescriptor( & sd1,SECURITY_DESCRIPTOR_REVISION);
			SetSecurityDescriptorDacl( & sd1,TRUE,NULL,FALSE);
			sa.nLength  =   sizeof (SECURITY_ATTRIBUTES);
			sa.bInheritHandle  =  TRUE;
			sa.lpSecurityDescriptor  =   & sd1;  
			bRet = CreateDirectory(tmpPath,&sa);  

		}
		else
		{
			bRet = CreateDirectory(tmpPath, NULL);

		}
		if(bRet == FALSE)
		{
			return FALSE;
		}
	}

	return TRUE;
}

//дINI�ļ�
BOOL CFileOperate::MyWriteIniFile(const CString& strFilePath, const CString& strSession, const CString& str)
{
	//����INI�ļ�Ŀ¼��ini�ļ���
	CString strIniPath, strIniFileName, strTime;
	CTime curTime = CTime::GetCurrentTime();
	
	strIniPath.Format(_T("%s%s%4d%s%02d%s"), strFilePath, _T("\\Ini\\"), curTime.GetYear(), _T("��"),curTime.GetMonth(), _T("��"));
	strIniFileName.Format(_T("%s%s%4d%02d%02d%s"), strIniPath, _T("\\"), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), _T(".ini"));
	strTime.Format(_T("%02d%s%02d%s%02d"), curTime.GetHour(), _T(":"), curTime.GetMinute(), _T(":"), curTime.GetSecond());
	
	//���Ŀ¼�����ھʹ���Ŀ¼
	BOOL bRet = FALSE;
	bRet = CreateMultiLevelPath(strIniPath);
	if(bRet == FALSE)
	{
		return FALSE;
	}

	//дini�ļ�
	bRet = WritePrivateProfileString(strSession, strTime, str, strIniFileName);
	return bRet;
}


//дBMP�ļ�
BOOL CFileOperate::MyWriteBMPFile(const CString& strFileFullName, const PBITMAPINFO pBmpInfo, const BYTE *pImageBuffer)
{
	if(pBmpInfo == NULL || pImageBuffer == NULL)
	{
		return FALSE;
	}

	CString strFilePath;
	int nPos = strFileFullName.ReverseFind('\\');
	if(nPos == -1)
	{
		strFilePath = "";
	}
	else
	{
		strFilePath = strFileFullName.Left(nPos);
	}

	//���Ŀ¼�����ھʹ���Ŀ¼
	BOOL bRet = FALSE;
	bRet = CreateMultiLevelPath(strFilePath);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	
	//дBMP�ļ�
	bRet = SaveBMPFile(strFileFullName, pBmpInfo, pImageBuffer);
	return bRet;	
}

//д�������е��ַ���˳��д���ļ�
BOOL CFileOperate::MyWriteStringFile(const CString& strFilePath, const CString& strFileName, const CList<CString,CString&>& lCheckRltStrList)
{ 
	//////////////////////////////////////////////////////////////////////////
	BOOL bRet = FALSE;
	bRet = CreateMultiLevelPath(strFilePath);
	if(bRet == FALSE)
	{
		return FALSE;
	}

	CString strFileFullName;
	strFileFullName.Format(_T("%s\\%s"), strFilePath, strFileName);
	
	CString cstr ;
	POSITION pos = lCheckRltStrList.GetHeadPosition();
	int nSumL = lCheckRltStrList.GetCount() ;
	for (int i = 0; i < nSumL ; i++)
	{
		cstr.Format(_T("NO.%d"), i) ;
		bRet = WritePrivateProfileString(_T("Start"),cstr,/*(LPCSTR)*/ lCheckRltStrList.GetNext(pos), strFileFullName);
		if(bRet == FALSE)
		{
			return FALSE;
		}
	}

	return TRUE;
	//
	//////////////////////////////////////////////////////////////////////////
}
CCriticalSection g_csWriteSysLog;       // дϵͳ������־�� [4/28/2008 Dufeng] 
CCriticalSection g_csWriteExcuteLog;
// ���ϵͳ������־���� [2/14/2008 DuFeng]

// ������ż�¼��־[05/28/2010 jincai]
BOOL CFileOperate::WriteChongzhangLog(const CString& strFilePath,const CString& strSession, const CString& str)
{
	g_csWriteSysLog.Lock();//  [5/5/2008 Dufeng]
	static int iWriteChongNumber = 0;
	CString strIniPath, strIniFileName, strTime,strFileNamePath;
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	iWriteChongNumber += 1;
	strIniPath.Format(_T("%s%s%4d%s%02d%s"), strFilePath, _T("\\"),curTime.wYear, _T("��"),curTime.wMonth, _T("��"));
	strIniFileName.Format(_T("%s%s%4d%02d%02d-%02d%s"), strIniPath, _T("\\"), curTime.wYear, curTime.wMonth, curTime.wDay, iWriteChongNumber/4000, _T(".txt"));
	// ʹ�þ���ʱ��_����˳��ķ�ʽ��¼��� [3/15/2008 Dufeng]
	strTime.Format(_T("%02d%s%02d%s%02d%s%03d_%d"),curTime.wHour, _T(":"), curTime.wMinute, _T(":"), curTime.wSecond, _T(":"),curTime.wMilliseconds,iWriteChongNumber);
	
	//���Ŀ¼�����ھʹ���Ŀ¼
	BOOL bRet = FALSE;
	bRet = CreateMultiLevelPath(strIniPath);
	if(bRet == FALSE)
	{
		g_csWriteSysLog.Unlock();//  [5/5/2008 Dufeng]
		return FALSE;
	}	
	//дini�ļ�
	bRet = WritePrivateProfileString(strSession, strTime, str, strIniFileName);
	g_csWriteSysLog.Unlock();//  [5/5/2008 Dufeng]
	return bRet;
}
// ���ϵͳ������־���� [2/14/2008 DuFeng]
BOOL CFileOperate::WriteExceptionLog(const CString& strFilePath,const CString& strVehicle,const CString& strSession, const CString& str)
{
	g_csWriteSysLog.Lock();

	if(m_pfileSystemExcept==NULL)
	{
		m_pfileSystemExcept=new CFile;
	}

	BOOL bRet = TRUE;
	try
	{
	static int iWriteNumber = 0;
	static int iTxtID = 0;
	//static CString strLastIniPath = "";
	CString strIniFileName = _T("");
	CString strIniPath, strTime,strFileNamePath,strTemp;
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);
	if(strFilePath.GetLength()>0)
	{
		if(strFilePath.GetAt(strFilePath.GetLength()-1)=='\\')
		{
			strIniPath.Format(_T("%s%s%4d-%02d%s%02d"), strFilePath, _T(""),curTime.wYear, curTime.wMonth, _T("\\"),curTime.wDay);
		}
		else
		{
			strIniPath.Format(_T("%s%s%4d-%02d%s%02d"), strFilePath, _T("\\"),curTime.wYear, curTime.wMonth, _T("\\"),curTime.wDay);
		}
	}
	if (strVehicle == _T(""))
	{
	}
	else
	{
		if(strIniPath.GetAt(strIniPath.GetLength()-1)=='\\')
		{
			strIniPath = strIniPath + strVehicle;
		}
		else
		{
			strIniPath = strIniPath + _T("\\")+strVehicle;
		}

	}
	if(m_strLastSysExpIniPath != strIniPath)
	{
		iWriteNumber = 0;
		iTxtID = 0;		
		//���Ŀ¼�����ھʹ���Ŀ¼
		bRet = FALSE;
		bRet = CreateMultiLevelPath(strIniPath);
		if(bRet == FALSE)
		{
			g_csWriteSysLog.Unlock();//
			return FALSE;
		}
	}
	strIniFileName.Format(_T("%s%sExceptLog-%4d%02d%02d%02d-%03d%s"), strIniPath, _T("\\"), curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour,iTxtID, _T(".log"));


	int iTxtIDTemp = 0;

	if (iWriteNumber==0)
	{
		while(IsFileExist(strIniFileName) )
		{

			iTxtID++;
			strIniFileName.Format(_T("%s%sExceptLog-%4d%02d%02d%02d-%03d%s"), strIniPath, _T("\\"), curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, iTxtID, _T(".log"));
			if (iTxtID == 1000)
			{
				strIniFileName.Format(_T("%s%sExceptLog-%4d%02d%02d%02d-%03d-Ĭ��%s"), strIniPath, _T("\\"), curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, iTxtID, _T(".log"));
				break;
			}
		}
	}
	

	iWriteNumber += 1;
	

	if(m_strLastSysExpIniPath != strIniPath)
	{
		if(m_strLastSysExpIniPath!= _T("")&&(*m_pfileSystemExcept!=CFile::hFileNull))
		{
				m_pfileSystemExcept->Close();	
		}
		//  [6/16/2016 zl]��֤·���ļ��д���
		CreateMultiLevelPath(strIniPath);
		m_strLastSysExpIniPath = strIniPath;
		iWriteNumber = 0;
		iTxtID = 0;	
		
		if(*m_pfileSystemExcept==CFile::hFileNull)
		{
			m_pfileSystemExcept->Open (strIniFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone);
		}
	}
	
	if (iWriteNumber >=4000)
	{
		iWriteNumber = 0;
		iTxtID++;
		
		m_nSystemExceptTxtID=iTxtID;
		if(*m_pfileSystemExcept!=CFile::hFileNull)
		{
			m_pfileSystemExcept->Close();
		}
		if(*m_pfileSystemExcept==CFile::hFileNull)
		{
			m_pfileSystemExcept->Open (strIniFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone);
		}
	}
	// ʹ�þ���ʱ��_����˳��ķ�ʽ��¼��� [3/15/2008 Dufeng]
	strTime.Format(_T("%02d%s%02d%s%02d%s%03d_%d="),curTime.wHour, _T(":"), curTime.wMinute, _T(":"), curTime.wSecond, _T(":"),curTime.wMilliseconds,iWriteNumber);
	CString strEnter;
	strEnter.Format(_T("\r\n"));
	int  nStrLength=str.GetLength();
	const char* pcStr=(LPSTR)(LPCTSTR)str;
	int nStrEnterLength=strEnter.GetLength();
	const char* pcEnter=(LPSTR)(LPCTSTR)strEnter;
	int nStrTimeLength=strTime.GetLength();
	const char* pcLogTime=(LPSTR)(LPCTSTR)strTime;

	m_pfileSystemExcept->Write(pcLogTime,sizeof(char)*nStrTimeLength);

	m_pfileSystemExcept->Write(pcStr,sizeof(char)*nStrLength);

	m_pfileSystemExcept->Write(pcEnter,sizeof(char)*nStrEnterLength);
	//bRet = WritePrivateProfileString(strSession, strTime, str, strIniFileName);
	}
	catch(...)
	{
		return FALSE;
	}
	g_csWriteSysLog.Unlock();//
	return bRet;
}
BOOL CFileOperate::WriteSystemOperateLog(const CString& strFilePath,const CString& strVehicle,const CString& strSession, const CString& str)
{
	g_csWriteSysLog.Lock();
	if(m_pfileSystemOperate==NULL)
	{
		m_pfileSystemOperate=new CFile;
	}
	BOOL bRet=TRUE;
	try
	{
		static int iWriteNumber = 0;
		static int iTxtID = 0;
		//static CString strLastIniPath = "";
		CString strIniFileName = _T("");
		CString strIniPath, strTime,strFileNamePath,strTemp;
		SYSTEMTIME curTime;
		GetLocalTime(&curTime);
		//  [6/16/2016 zl] //������·��������
		if(strFilePath.GetLength()>0)
		{
			if(strFilePath.GetAt(strFilePath.GetLength()-1)=='\\')
			{
				strIniPath.Format(_T("%s%s%4d-%02d%s%02d"), strFilePath, _T(""),curTime.wYear, curTime.wMonth, _T("\\"),curTime.wDay);
			}
			else
			{
				strIniPath.Format(_T("%s%s%4d-%02d%s%02d"), strFilePath, _T("\\"),curTime.wYear, curTime.wMonth, _T("\\"),curTime.wDay);
			}
		}
		
		
		if (strVehicle == _T(""))
		{
		}
		else
		{
			if(strIniPath.GetAt(strIniPath.GetLength()-1)=='\\')
			{
				strIniPath = strIniPath + strVehicle;
			}
			else
			{
				strIniPath = strIniPath + _T("\\")+strVehicle;
			}

		}
		if(m_strLastSysOpIniPath != strIniPath)
		{
			//strLastIniPath = strIniPath;
			iWriteNumber = 0;
			iTxtID = 0;	

			//���Ŀ¼�����ھʹ���Ŀ¼
			bRet = FALSE;
			bRet = CreateMultiLevelPath(strIniPath);
			if(bRet == FALSE)
			{
				g_csWriteSysLog.Unlock();//
				return FALSE;
			}
			//m_pfileSystemOperate.Close();
			//m_pfileSystemOperate.Open (strIniFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone);
		}
		strIniFileName.Format(_T("%s%sOperateLog-%4d%02d%02d%02d-%03d%s"), strIniPath, _T("\\"), curTime.wYear, curTime.wMonth, curTime.wDay, curTime.wHour, iTxtID, _T(".log"));


		int iTxtIDTemp = 0;
		//	BOOL bflag=FALSE;

		if (iWriteNumber==0)
		{
			while(IsFileExist(strIniFileName) )
			{
				//	bflag = TRUE;
				
				iTxtID++;
				strIniFileName.Format(_T("%s%sOperateLog-%4d%02d%02d%02d-%03d%s"), strIniPath, _T("\\"), curTime.wYear, curTime.wMonth, curTime.wDay,curTime.wHour, iTxtID, _T(".log"));
				if (iTxtID == 1000)
				{
					strIniFileName.Format(_T("%s%sOperateLog-%4d%02d%02d%02d-%03d-Ĭ��%s"), strIniPath, _T("\\"), curTime.wYear, curTime.wMonth, curTime.wDay,curTime.wHour, iTxtID, _T(".log"));
					break;
				}		
			}
		}


		iWriteNumber += 1;


		if(m_strLastSysOpIniPath != strIniPath)
		{
			if(m_strLastSysOpIniPath!= _T("")&&(*m_pfileSystemOperate!=CFile::hFileNull))
			{
				m_pfileSystemOperate->Close();
			}
			//  [6/16/2016 zl]��֤·���ļ��д���
			CreateMultiLevelPath(strIniPath);
			m_strLastSysOpIniPath = strIniPath;
			iWriteNumber = 0;
			iTxtID = 0;	
			if((*m_pfileSystemOperate==CFile::hFileNull))
			{
				m_pfileSystemOperate->Open (strIniFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone);
			}
		}



		if (iWriteNumber >=4000)
		{
			iWriteNumber = 0;
			iTxtID++;
			m_nSystemOperateTxtID=iTxtID;
			
			if(*m_pfileSystemOperate!=CFile::hFileNull)
			{
				m_pfileSystemOperate->Close();
			}
			if((*m_pfileSystemOperate==CFile::hFileNull))
			{
				m_pfileSystemOperate->Open (strIniFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone);
			}
		}
		// ʹ�þ���ʱ��_����˳��ķ�ʽ��¼��� [3/15/2008 Dufeng]
		strTime.Format(_T("%02d%s%02d%s%02d%s%03d_%d="),curTime.wHour, _T(":"), curTime.wMinute, _T(":"), curTime.wSecond, _T(":"),curTime.wMilliseconds,iWriteNumber);
		CString strEnter;
		strEnter.Format(_T("\r\n"));
		int  nStrLength=str.GetLength();
		const char* pcStr=(LPSTR)(LPCTSTR)str;
		int nStrEnterLength=strEnter.GetLength();
		const char* pcEnter=(LPSTR)(LPCTSTR)strEnter;
		int nStrTimeLength=strTime.GetLength();
		const char* pcLogTime=(LPSTR)(LPCTSTR)strTime;
		m_pfileSystemOperate->Write(pcLogTime,sizeof(char)*nStrTimeLength);

		m_pfileSystemOperate->Write(pcStr,sizeof(char)*nStrLength);

		m_pfileSystemOperate->Write(pcEnter,sizeof(char)*nStrEnterLength);

		//bRet = WritePrivateProfileString(strSession, strTime, str, strIniFileName);
	}
	catch(...)
	{
			return FALSE;
	}
	
	g_csWriteSysLog.Unlock();//
	return bRet;
}

BOOL CFileOperate::WriteAlgExceptionLog(const CString& strFilePath,const CString& strVehicle,const CString& strSession, const CString& str)
{
	g_csWriteSysLog.Lock();//  [5/5/2008 Dufeng]
	//  [10/12/2015 zl]
	BOOL bRetAlg=WriteLogFile(3,str);
	g_csWriteSysLog.Unlock();//  [5/5/2008 Dufeng]
	return bRetAlg;
	/*
	static int iWriteNumber = 0;
	static int iTxtID = 0;
	static CString strLastIniPath = "";
	CString strIniFileName = "";
	CString strIniPath, strTime,strFileNamePath,strTemp;
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);

	strIniPath = strFilePath;
	if (strVehicle == "")
	{
	}
	else
	{
		strIniPath = strIniPath + "\\"+strVehicle;

	}

	if(strLastIniPath != strIniPath)
	{
		strLastIniPath = strIniPath;
		iWriteNumber = 0;
		iTxtID = 0;
	}
	strIniFileName.Format("%s%s%4d%02d%02d-%02d%s", strIniPath, "\\", curTime.wYear, curTime.wMonth, curTime.wDay, iTxtID,".txt");

	int iTxtIDTemp = 0;

	while(IsFileExist(strIniFileName) && iWriteNumber == 0)
	{

		iTxtID++;
		strIniFileName.Format("%s%s%4d%02d%02d-%02d%s", strIniPath, "\\", curTime.wYear, curTime.wMonth, curTime.wDay, iTxtID,".txt");
		if (iTxtID == 100)
		{
			strIniFileName.Format("%s%s%4d%02d%02d-%02d-Ĭ��%s", strIniPath, "\\", curTime.wYear, curTime.wMonth, curTime.wDay, iTxtID,".txt");
			break;
		}
	}

	iWriteNumber += 1;
	if (iWriteNumber >=4000)
	{
		iWriteNumber = 0;
		iTxtID++;
	}
	// ʹ�þ���ʱ��_����˳��ķ�ʽ��¼��� [3/15/2008 Dufeng]
	strTime.Format("%02d%s%02d%s%02d%s%03d_%d",curTime.wHour, ":", curTime.wMinute, ":", curTime.wSecond,":",curTime.wMilliseconds,iWriteNumber);

	//���Ŀ¼�����ھʹ���Ŀ¼
	BOOL bRet = FALSE;
	bRet = CreateMultiLevelPath(strIniPath);
	if(bRet == FALSE)
	{
		g_csWriteSysLog.Unlock();//  [5/5/2008 Dufeng]
		return FALSE;
	}	
	//дini�ļ�
	bRet = WritePrivateProfileString(strSession, strTime, str, strIniFileName);

	return bRet;
	*/

}
BOOL CFileOperate::WriteAlgOperateLog(const CString& strFilePath,const CString& strVehicle,const CString& strSession, const CString& str)
{
	g_csWriteSysLog.Lock();//  [5/5/2008 Dufeng]
	BOOL bRetAlg=WriteLogFile(2,str);
	g_csWriteSysLog.Unlock();//  [5/5/2008 Dufeng]
	return bRetAlg;

	//static int iWriteNumber = 0;
 //   static int iTxtID = 0;
	//static CString strLastIniPath = "";
	//CString strIniFileName = "";
	//CString strIniPath, strTime,strFileNamePath,strTemp;
	//SYSTEMTIME curTime;
	//GetLocalTime(&curTime);
	//
	//strIniPath = strFilePath;
	//if (strVehicle == "")
	//{
	//}
	//else
	//{
	//	strIniPath = strIniPath + "\\"+strVehicle;
	//	
	//}
	//
	//if(strLastIniPath != strIniPath)
	//{
	//	strLastIniPath = strIniPath;
	//	iWriteNumber = 0;
	//	iTxtID = 0;
	//}
	//strIniFileName.Format("%s%s%4d%02d%02d-%02d%s", strIniPath, "\\", curTime.wYear, curTime.wMonth, curTime.wDay, iTxtID,".txt");
	//
	//int iTxtIDTemp = 0;
	//
	//while(IsFileExist(strIniFileName) && iWriteNumber == 0)
	//{
	//	
	//	iTxtID++;
	//	strIniFileName.Format("%s%s%4d%02d%02d-%02d%s", strIniPath, "\\", curTime.wYear, curTime.wMonth, curTime.wDay, iTxtID,".txt");
	//	if (iTxtID == 100)
	//	{
	//		strIniFileName.Format("%s%s%4d%02d%02d-%02d-Ĭ��%s", strIniPath, "\\", curTime.wYear, curTime.wMonth, curTime.wDay, iTxtID,".txt");
	//		break;
	//	}
	//}
	//
	//iWriteNumber += 1;
	//if (iWriteNumber >=4000)
	//{
	//	iWriteNumber = 0;
	//	iTxtID++;
	//}
	//// ʹ�þ���ʱ��_����˳��ķ�ʽ��¼��� [3/15/2008 Dufeng]
	//strTime.Format("%02d%s%02d%s%02d%s%03d_%d",curTime.wHour, ":", curTime.wMinute, ":", curTime.wSecond,":",curTime.wMilliseconds,iWriteNumber);
	//
	////���Ŀ¼�����ھʹ���Ŀ¼
	//BOOL bRet = FALSE;
	//bRet = CreateMultiLevelPath(strIniPath);
	//if(bRet == FALSE)
	//{
	//	g_csWriteSysLog.Unlock();//  [5/5/2008 Dufeng]
	//	return FALSE;
	//}	
	////дini�ļ�
	//bRet = WritePrivateProfileString(strSession, strTime, str, strIniFileName);
	//
	//return bRet;

}
BOOL CFileOperate::WriteAlgCheckLog(const CString& strFilePath,const CString& strVehicle,const CString& strSession, const CString& str)
{
	g_csWriteSysLog.Lock();//  [5/5/2008 Dufeng]
	//  [10/12/2015 zl]
	BOOL bRetAlg=WriteLogFile(1,str);
	g_csWriteSysLog.Unlock();//  [5/5/2008 Dufeng]
	return bRetAlg;

	//static int iWriteNumber = 0;
	//static int iTxtID = 0;
	//static CString strLastIniPath = "";
	//CString strIniFileName = "";
	//CString strIniPath, strTime,strFileNamePath,strTemp;
	//SYSTEMTIME curTime;
	//GetLocalTime(&curTime);

	//strIniPath = strFilePath;
	//if (strVehicle == "")
	//{
	//}
	//else
	//{
	//	strIniPath = strIniPath + "\\"+strVehicle;

	//}

	//if(strLastIniPath != strIniPath)
	//{
	//	strLastIniPath = strIniPath;
	//	iWriteNumber = 0;
	//	iTxtID = 0;
	//}
	//strIniFileName.Format("%s%s%4d%02d%02d-%02d%s", strIniPath, "\\", curTime.wYear, curTime.wMonth, curTime.wDay, iTxtID,".txt");

	//int iTxtIDTemp = 0;

	//while(IsFileExist(strIniFileName) && iWriteNumber == 0)
	//{

	//	iTxtID++;
	//	strIniFileName.Format("%s%s%4d%02d%02d-%02d%s", strIniPath, "\\", curTime.wYear, curTime.wMonth, curTime.wDay, iTxtID,".txt");
	//	if (iTxtID == 100)
	//	{
	//		strIniFileName.Format("%s%s%4d%02d%02d-%02d-Ĭ��%s", strIniPath, "\\", curTime.wYear, curTime.wMonth, curTime.wDay, iTxtID,".txt");
	//		break;
	//	}
	//}

	//iWriteNumber += 1;
	//if (iWriteNumber >=4000)
	//{
	//	iWriteNumber = 0;
	//	iTxtID++;
	//}
	//// ʹ�þ���ʱ��_����˳��ķ�ʽ��¼��� [3/15/2008 Dufeng]
	//strTime.Format("%02d%s%02d%s%02d%s%03d_%d",curTime.wHour, ":", curTime.wMinute, ":", curTime.wSecond,":",curTime.wMilliseconds,iWriteNumber);

	////���Ŀ¼�����ھʹ���Ŀ¼
	//BOOL bRet = FALSE;
	//bRet = CreateMultiLevelPath(strIniPath);
	//if(bRet == FALSE)
	//{
	//	g_csWriteSysLog.Unlock();//  [5/5/2008 Dufeng]
	//	return FALSE;
	//}	
	////дini�ļ�
	//bRet = WritePrivateProfileString(strSession, strTime, str, strIniFileName);
	//return bRet;

}
//////////////////////////////////////////////////////////////////////////
//��ͼ���ļ�����
#define WIDTH_BYTES(bits) (((bits) + 31) / 32 * 4)
BOOL CFileOperate::OpenBMPFile(LPCWSTR lpFileName, BITMAPINFO *pBmpInfo, BYTE *pImageBuffer)
{
	BOOL bRVal = TRUE;
	DWORD dwBytesRead = 0;
	DWORD dwSize = 0;
	BITMAPFILEHEADER bfh;
	
	HANDLE hFile = ::CreateFile(lpFileName,
								GENERIC_READ,
								FILE_SHARE_READ,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL
								);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		bRVal = FALSE;
	}
	else
	{
		dwSize = sizeof(BITMAPFILEHEADER);
		::ReadFile(hFile, &bfh, dwSize, &dwBytesRead, NULL);
		if (bfh.bfType != ((WORD)'M' << 8 | 'B')) 
		{
			bRVal = FALSE;
		}
		else
		{
			dwSize = sizeof(BITMAPINFOHEADER);
			::ReadFile(hFile, pBmpInfo, dwSize, &dwBytesRead, NULL);
			if (pBmpInfo->bmiHeader.biBitCount <= 8)
			{
				dwSize = bfh.bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
				ReadFile(hFile,
					(char *)pBmpInfo + sizeof(BITMAPINFOHEADER),
					dwSize,
					&dwBytesRead,
					NULL
					);
			}			
			dwSize = WIDTH_BYTES(pBmpInfo->bmiHeader.biWidth * pBmpInfo->bmiHeader.biBitCount) *
				pBmpInfo->bmiHeader.biHeight;			
			ReadFile(hFile, pImageBuffer, dwSize, &dwBytesRead, NULL);
		}
		
		CloseHandle(hFile);
	}
	
	return bRVal;
}

int CFileOperate::GetSurplusMemorySize()
{
	MEMORYSTATUS MemoryStatus;
	GlobalMemoryStatus(&MemoryStatus);
    return (int)(MemoryStatus.dwAvailPhys/(1024*1024));
}

int CFileOperate::GetSurplusCurrentPartitionSize(LPCTSTR szDrivePath)//�����޸ģ��Զ���ȡ��ǰ�������е��ļ�·����ͳ��Ӳ������
{
	BOOL bResult;
	ULARGE_INTEGER lpFreeBytesAvailableToCaller;
	ULARGE_INTEGER lpTotalNumberOfBytes;
	ULARGE_INTEGER lpTotalNumberOfFreeBytes;
	bResult = GetDiskFreeSpaceEx(szDrivePath,&lpFreeBytesAvailableToCaller,&lpTotalNumberOfBytes,
		&lpTotalNumberOfFreeBytes);
	if(bResult == FALSE)
	{
		AfxMessageBox(_T("��ȡӲ��ʣ������ʧ��!"));
	}
    return (int)(lpTotalNumberOfFreeBytes.QuadPart/(1024*1024));
}

BOOL CFileOperate::DeleteDirectory(const CString &strDirName)
{
	if (strDirName.IsEmpty())
	{
		return FALSE;
	}

	SHFILEOPSTRUCT sfo;
	ZeroMemory(&sfo, sizeof(SHFILEOPSTRUCT));

	CString szSrc;
	//ZeroMemory(szSrc, MAX_PATH);

	//memcpy(szSrc, strDirName, strDirName.GetLength());
	szSrc = strDirName;

	sfo.pFrom = szSrc;
	sfo.wFunc = FO_DELETE;
	sfo.fFlags = FOF_SILENT | FOF_NOCONFIRMATION;

	int nError = SHFileOperation(&sfo);

	if(nError != 0)
	{
		if (sfo.fAnyOperationsAborted)
		{
			return FALSE;
		}

		return FALSE;
	}
	else
	{
		return TRUE;
	}
} 
// ��Ӳ�������ʶ����Դ [8/5/2013 ANRAN] дSQL��־��� [10/2/2011 ANRAN]
BOOL CFileOperate::WriteSqlExcuteLog(const CString& strFilePath,const CString& strVehicle, const CString& strSession, const CString& str, int iDatabaseSource)
{
	
	BOOL bRet=TRUE;
	try
	{
	g_csWriteExcuteLog.Lock();
	if(m_pfileSystemSql==NULL)
	{
		m_pfileSystemSql=new CFile;
	}
	static int iWriteNumber = 0;
	static int iTxtID = 0;
	//static int end=0;
	//static CString strLastIniPath = "";
	CString strIniFileName = _T("");
	CString strIniPath, strTime,strFileNamePath,strTemp;
	SYSTEMTIME curTime;
	GetLocalTime(&curTime);

	if(strFilePath.GetLength()>0)
	{
		if(strFilePath.GetAt(strFilePath.GetLength()-1)=='\\')
		{
			strIniPath.Format(_T("%s%s%4d-%02d%s%02d"), strFilePath, _T(""),curTime.wYear, curTime.wMonth, _T("\\"),curTime.wDay);
		}
		else
		{
			strIniPath.Format(_T("%s%s%4d-%02d%s%02d"), strFilePath, _T("\\"),curTime.wYear, curTime.wMonth, _T("\\"),curTime.wDay);
		}
	}

	if (strVehicle == _T(""))
	{
	}
	else
	{
		if(strIniPath.GetAt(strIniPath.GetLength()-1)=='\\')
		{
			strIniPath = strIniPath + strVehicle;
		}
		else
		{
			strIniPath = strIniPath + _T("\\")+strVehicle;
		}
	}

	if(m_strLastSqlExcuteIniPath != strIniPath)
	{
		//strLastIniPath = strIniPath;
		iWriteNumber = 0;
		iTxtID = 0;
		m_nSqlLogNum=0;
		bRet = FALSE;
		bRet = CreateMultiLevelPath(strIniPath);
		if(bRet == FALSE)
		{
			g_csWriteExcuteLog.Unlock();//
			return FALSE;
		}
	}
	strIniFileName.Format(_T("%s%s\\SqlLog-%4d-%02d-%02d_%d-%03d%s"), strIniPath, _T("\\"), curTime.wYear, curTime.wMonth, curTime.wDay, iDatabaseSource, iTxtID, _T(".log"));

	int iTxtIDTemp = 0;
	if (iWriteNumber==0)
	{
		while(IsFileExist(strIniFileName))
		{

			iTxtID++;
			strIniFileName.Format(_T("%s%sSqlLog-%4d-%02d-%02d_%d-%03d%s"), strIniPath, _T("\\"), curTime.wYear, curTime.wMonth, curTime.wDay, iDatabaseSource, iTxtID, _T(".log"));
			if (iTxtID == 1000)
			{
				strIniFileName.Format(_T("%s%sSqlLog-%4d-%02d-%02d_%d-%03d-Ĭ��%s"), strIniPath, _T("\\"), curTime.wYear, curTime.wMonth, curTime.wDay, iDatabaseSource, iTxtID, _T(".log"));
				break;
			}
		}
	}

	iWriteNumber += 1;
	if(m_strLastSqlExcuteIniPath != strIniPath)
	{
		if(m_strLastSqlExcuteIniPath!= _T(""))
		{
			CString strContent;
			strContent.Format(_T("����sql����=%d"),m_nSqlLogNum);
			int  nStrContentLength=strContent.GetLength();
			const char* pcStrContent=(LPSTR)(LPCTSTR)strContent;
			if(*m_pfileSystemSql!=CFile::hFileNull)
			{
				m_pfileSystemSql->Write(pcStrContent,sizeof(char)*nStrContentLength);
				m_pfileSystemSql->Close();
			}
		}
		//  [6/16/2016 zl]��֤·���ļ��д���
		CreateMultiLevelPath(strIniPath);
		m_strLastSqlExcuteIniPath = strIniPath;
		iWriteNumber = 0;
		iTxtID = 0;	
		m_nSqlLogNum=0;
		if(*m_pfileSystemSql==CFile::hFileNull)
		{
			m_pfileSystemSql->Open (strIniFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone);
		}
	}

	// sql�����־����1000���ͻ�һ���ļ�����ֹ�ļ�����Ӱ��ϵͳ������ [8/5/2013 ANRAN]
	if (iWriteNumber >=1000)
	{
		iWriteNumber = 0;
		iTxtID++;

		CString strContent;
		strContent.Format(_T("����sql����=%d"),m_nSqlLogNum);
		int  nStrContentLength=strContent.GetLength();
		const char* pcStrContent=(LPSTR)(LPCTSTR)strContent;

		if(*m_pfileSystemSql!=CFile::hFileNull)
		{
			m_pfileSystemSql->Write(pcStrContent,sizeof(char)*nStrContentLength);
			m_pfileSystemSql->Close();
		}

		m_nSqlLogNum=0;
		
		if(*m_pfileSystemSql==CFile::hFileNull)
		{
			m_pfileSystemSql->Open (strIniFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone);
		}
		
	}

	m_nSqlLogNum++;

	// ʹ�þ���ʱ��_����˳��ķ�ʽ��¼��� [3/15/2008 Dufeng]
	strTime.Format(_T("%02d%s%02d%s%02d%s%03d_%d:"),curTime.wHour, _T(":"), curTime.wMinute, _T(":"), curTime.wSecond, _T(":"),curTime.wMilliseconds,iWriteNumber);
	int nStrTimeLength=strTime.GetLength();
	const char* pcLogTime=(LPSTR)(LPCTSTR)strTime;
	m_pfileSystemSql->Write(pcLogTime,sizeof(char)*nStrTimeLength);
	//дini�ļ�
	//////////////////////////////////////////////////////////////////////////
	//end=GetPrivateProfileIntA("setup","����sql����",0,strIniFileName);
	CString strEnter;
	strEnter.Format(_T("\r\n"));
	CString strContent;
	CString strHead;
	strHead.Format(_T("��%d��"),m_nSqlLogNum);
	strContent.Format(_T("%s=%s"),strHead,str);

	int  nStrLength=strContent.GetLength();
	const char* pcStr=(LPSTR)(LPCTSTR)strContent;
	m_pfileSystemSql->Write(pcStr,sizeof(char)*nStrLength);

	int nStrEnterLength=strEnter.GetLength();
	const char* pcEnter=(LPSTR)(LPCTSTR)strEnter;
	m_pfileSystemSql->Write(pcEnter,sizeof(char)*nStrEnterLength);

	//CString strHead;
	//strHead.Format("��%d��",end);
	//bRet = WritePrivateProfileString(strSession,strHead,str,strIniFileName);
	//strHead.Format("%d",end);
	//bRet = WritePrivateProfileString("setup","����sql����",strHead,strIniFileName);
	//////////////////////////////////////////////////////////////////////////
	g_csWriteExcuteLog.Unlock();//

	}
	catch(...)
	{
		return FALSE;
	}
	return bRet;
}

int CFileOperate::CalFileRowNum(CFile *file,CString strFile)
{
	if(*file==CFile::hFileNull)
	{
		file->Open (strFile,CFile::modeCreate|CFile::modeReadWrite|CFile::modeNoTruncate|CFile::shareDenyNone);
	}
	char  cFileChar;
	int nRowNum=0;
	while(file->Read(&cFileChar,1))
	{
		if(cFileChar=='\n')
		{
			nRowNum++;
		}
	}
	int num=file->GetLength();
	file->SeekToEnd();
	return nRowNum;
}
