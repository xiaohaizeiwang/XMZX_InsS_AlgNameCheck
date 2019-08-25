/************************************************************************/
/* Copyright (c) 2008,北京图像视觉技术有限公司视觉技术部     
/* All rights reserved.													
/*																		
/* 文件名称： FileOperate.h												
/* 文件标识： 见配置管理计划书
/* 摘要： 对涉及文件操作的各种函数进行汇总, 提供文件及路径常用操作,
/*    保存及打开图像文件, 保存文本文件等功能
/*
/* 初始版本：1.0
/* 修改版本： 2.0
/* 作者： 江卓佳
/* 完成日期： 2005年10月10日

/* 修改记录： 
///////////////////////////////////////////V2.4
/*       将原写入文件方式变为数据流方式   //V2.0 赵亮 20151013
/*       日志文件后缀由ini改为log，日志文件名增加日志类型前缀 //V2.0 赵亮 20151013
/*       算法增加一个文件初始化函数，算法需对应调用释放函数  //V2.0 赵亮 20151013
/*       系统异常和操作日志使用方法不变，去掉生成目录中年月日的汉字  //V2.0 赵亮 20151013
/*       处理换天时路径改变导致异常 在所有文件打开前生成文件路径 处理路径有无//的问题
/*        在对文件做操作前对文件指针是否为空和文件是否打开过进行判断 //V2.1 赵亮 20160616
/*		 处理写系统操作日志时文件关闭的判断条件bug //V2.2 赵亮 20160622
/*		 修改获取索引方式//V2.3 赵亮 20170309
/*		 修改日志文件名称，支持1000个文件//V2.4 张波 20170314
///////////////////////////////////
/************************************************************************/

#if !defined(AFX_FILEOPERATE1_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9367__INCLUDED_)
#define AFX_FILEOPERATE1_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9367__INCLUDED_

#include <afxtempl.h>//为了使用CList
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

	CFile *m_pfileCheck;//算法检测日志文件
	CFile* m_pfileOperate;//算法操作日志文件
	CFile* m_pfileExpect;//算法异常日志文件

	CFile* m_pfileSystemOperate;//系统操作日志文件
	CFile* m_pfileSystemExcept;//系统异常日志文件
	CFile* m_pfileSystemSql;//系统SQL日志文件
	int m_nSqlLogNum;//Sql日志个数

	CString m_strLogPath;
	//算法日志条数ID
	int m_nCheckTxtID;
	int m_nOperateTxtID;
	int m_nExpectTxtID;
	//系统日志条数ID
	int m_nSystemOperateTxtID;
	int m_nSystemExceptTxtID;

	BOOL InitFile(CString strLogPath);
	 int  FindMaxFileIndex(CFile* pfile,const CString strInput);
	 int CalFileRowNum(CFile *file,CString strFile);//计算文件行数
	BOOL WriteLogFile(int nLogStyle,const CString& str);
	BOOL FreeFile();
	//保存BMP图像的函数
	BOOL SaveBMPFile(LPCWSTR lpFileName, const BITMAPINFO *pBmpInfo, const BYTE *pImageBuffer);
	
	// 获取本执行程序所在的路径
	CString GetCurrentAppPath();
	
	//判断路径是否存在
	BOOL IsPathExist(const CString &strPath);
	
	//判断文件是否存在
	BOOL IsFileExist(const CString &strFileName);
	
	//创建一个多层目录，如果存在就不创建
	//(strPath  合法: "D:\\abc\\me" 或 "me\\you" 或 ""; 
	//			不合法: "D:\\abc\\me\\" 或 "me\\you\\" 或 "\\")
	BOOL CreateMultiLevelPath(const CString &strPath,BOOL bEveryone = FALSE);
	
	//写BMP文件
	BOOL MyWriteBMPFile(const CString& strFileFullName, const PBITMAPINFO pBmpInfo, const BYTE *pImageBuffer);
	BOOL WriteChongzhangLog(const CString& strFilePath,const CString& strSession, const CString& str);
	//写将链表中的字符串顺序写入文件
	BOOL MyWriteStringFile(const CString& strFilePath, const CString& strFileName, const CList<CString,CString&>& lCheckRltStrList);
	
	// 写系统操作日志
	BOOL WriteSystemOperateLog(const CString& strFilePath,const CString& strVehicle, const CString& strSession, const CString& str);
	
	// 写系统异常日志
	BOOL WriteExceptionLog(const CString& strFilePath,const CString& strVehicle,const CString& strSession, const CString& str);
	
	//写算法日志
	BOOL WriteAlgLog(const CString& strFilePath,const CString& strVehicle,const CString& strSession, const CString& str);
	
	BOOL WriteAlgOperateLog(const CString& strFilePath,const CString& strVehicle,const CString& strSession, const CString& str);
	BOOL WriteAlgExceptionLog(const CString& strFilePath,const CString& strVehicle,const CString& strSession, const CString& str);
	BOOL WriteAlgCheckLog(const CString& strFilePath,const CString& strVehicle,const CString& strSession, const CString& str);
	//打开图像文件函数
	BOOL OpenBMPFile(LPCWSTR lpFileName, BITMAPINFO *pBmpInfo, BYTE *pImageBuffer);
	
	// 添加参数标识数据源 [8/5/2013 ANRAN] 写SQL日志语句 [10/2/2011 ANRAN]
	BOOL WriteSqlExcuteLog(const CString& strFilePath,const CString& strVehicle, const CString& strSession, const CString& str, int iDatabaseSource = 0);
	//获得内存剩余容量函数
	int GetSurplusMemorySize();
	
	void WINAPI GetDlgItemRect(int nID,CWnd *wndParent,LPRECT Client);
	//获得当前硬盘分区的剩余容量
	int GetSurplusCurrentPartitionSize(LPCTSTR szDrivePath);

	BOOL MyWriteIniFile(const CString& strFilePath, const CString& strSession, const CString& str);

	//删除当前目录下所有文件及文件夹
	BOOL DeleteDirectory(const CString &strDirName);

};

#endif // !defined(AFX_FILEOPERATE1_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9367__INCLUDED_)