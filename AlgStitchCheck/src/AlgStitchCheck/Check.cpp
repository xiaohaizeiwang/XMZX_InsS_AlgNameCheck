/************************************************************************
/* Copyright (c) 2015, 北京大恒图像视觉有限公司印钞事业部
/* All rights reserved.
/*
/* 文件名称：  Check.cpp
/* 简要说明：  检测算法库实现类和算法属性设置实现类。
/*
/* 创建者：  邓红丽
/* 版本：    V1.0
/* 修改人：  田仁赞
/* 版本：    V2.0
/* 修改时间：2015-06-04
/* 修改记录：1. 格式化框架代码，增强可读性
			  2. 根据每一个处理函数的功能，添加了相应的处理代码
				 说明：无特殊情况，Check.cpp中的代码请不要更改
			  3. 完善了每个函数的注释说明
/* 修改人：  高丽萍
/* 版本：    V1.2.0.2
/* 修改时间：2017-06-29
/* 修改记录：1.修改halcon13相关算子
	  2. 修改初始化函数，增加一个检测参数类的输出
	  3. 增加获取参数函数GetAlgParam(const s_ErrorTpye sErrorTpye,s_TypeCheckParam  * psCheckParams)
	  4. 增加设置参数函数SetAlgParam(const s_ErrorTpye sErrorTpye,const s_TypeCheckParam  sCheckParams)

/* 修改人：  张波
/* 版本：    V3.0
/* 修改时间：2018-11-07
/* 修改记录：1.升级接口文件
			 2.增加建模界面获取参数函数GetAlgParam
			 3.增加建模界面设置参数函数SetAlgParam
			 4.增加自动建模函数AutoCreateAlgModel
			 5.支持halcon例程直接导入,包含读参、建模、检测
			 6.检测细分为独立、联合和融合检测
************************************************************************/
#include "stdafx.h"
#include "AlgStitchCheck.h"
#include "Check.h"
#include "PropDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// 检测算法库实现类实现
/////////////////////////////////////////////////////////////////////////////

// 检测算法库实现类构造函数和析构函数
//******************************************************************************//
CCheck::CCheck()
{
	m_strLogPath = _T("");
	if (m_listInnerCheck.size() > 0)
	{
		std::list<CInnerCheck>::iterator itInnerCheck;
		for (itInnerCheck = m_listInnerCheck.begin();
			itInnerCheck != m_listInnerCheck.end();
			itInnerCheck++)
		{
			itInnerCheck->Free();
		}
		m_listInnerCheck.clear();
	}
}
CCheck::~CCheck()
{
	m_strLogPath = _T("");

	if (m_listInnerCheck.size() > 0)
	{
		std::list<CInnerCheck>::iterator itInnerCheck;
		for (itInnerCheck = m_listInnerCheck.begin();
			itInnerCheck != m_listInnerCheck.end();
			itInnerCheck++)
		{
			itInnerCheck->Free();
		}
		m_listInnerCheck.clear();
	}
	m_FileOperate.FreeFile();
}
//字符串固定分割
CString * SplitString(CString str, char split, int& iSubStrs)
{
	int iPos = 0; //分割符位置
	int iNums = 0; //分割符的总数
	CString strTemp = str;
	CString strRight;
	//先计算子字符串的数量
	while (iPos != -1)
	{
		iPos = strTemp.Find(split);
		if (iPos == -1)
		{
			break;
		}
		strRight = strTemp.Mid(iPos + 1, str.GetLength());
		strTemp = strRight;
		iNums++;
	}
	if (iNums == 0) //没有找到分割符
	{
		//子字符串数就是字符串本身
		iSubStrs = 1;
		return NULL;
	}
	//子字符串数组
	iSubStrs = iNums + 1; //子串的数量 = 分割符数量 + 1
	CString* pStrSplit;
	pStrSplit = new CString[iSubStrs];
	strTemp = str;
	CString strLeft;
	for (int i = 0; i < iNums; i++)
	{
		iPos = strTemp.Find(split);
		//左子串
		strLeft = strTemp.Left(iPos);
		//右子串
		strRight = strTemp.Mid(iPos + 1, strTemp.GetLength());
		strTemp = strRight;
		pStrSplit[i] = strLeft;
	}
	pStrSplit[iNums] = strTemp;
	return pStrSplit;
}
//初始化函数
//******************************************************************************//
//函数:初始化函数
//参数:
//	sAlgInitParam    输入，全局路径
//  nAlgTypID        输入，算法ID
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CCheck::Init(const s_AlgInitParam &sAlgLayerInitParam, int nAlgTypID, s_TypeCheckParam* psCheckParams)
{
	try
	{
		m_sAlgInitParam = sAlgLayerInitParam;
		//创建日志记录路径
		CString str;
		str = m_sAlgInitParam.strAlglogPath;
		if (str.GetAt(str.GetLength() - 1) == '\\')
		{
			m_strLogPath.Format(_T("%s\\StitchCheck\\"), m_sAlgInitParam.strAlglogPath);
		}
		else
		{
			m_strLogPath.Format(_T("%s\\StitchCheck\\"), m_sAlgInitParam.strAlglogPath);
		}
		m_FileOperate.CreateMultiLevelPath(m_strLogPath);
		m_FileOperate.InitFile(m_strLogPath);
		m_FileOperate.WriteAlgOperateLog(m_strLogPath, _T("操作日志"), _T("操作日志"), _T("检测:初始化"));

		//参数
		psCheckParams->strAlgTypeName.Format(_T("StitchCheck"));

		s_ModelRegionPro itRegionProperty;
		CInnerCheck  itInnerCheck;
		itInnerCheck.m_sDstRegionProperty = itRegionProperty;
		itInnerCheck.m_strLogPath = m_strLogPath;

		itInnerCheck.m_FileOperate = &m_FileOperate;
		itInnerCheck.Init(m_strTaskName, m_sAlgInitParam, itRegionProperty, TRUE);

		m_htParamProp.Clear();
		m_htParamName.Clear();
		m_htParamMeaning.Clear();
		m_htParamVaule.Clear();

		m_htParamType.Clear();
		m_htParamMinVaule.Clear();
		m_htParamMaxVaule.Clear();
		m_htParamStep.Clear();
		m_htParamChildNames.Clear();

		m_htParamProp = itInnerCheck.m_htParamProp;
		m_htParamName = itInnerCheck.m_htParamName;
		m_htParamMeaning = itInnerCheck.m_htParamMeaning;
		m_htParamVaule = itInnerCheck.m_htParamVaule;

		m_htParamType = itInnerCheck.m_htParamType;
		m_htParamMinVaule = itInnerCheck.m_htParamMinVaule;
		m_htParamMaxVaule = itInnerCheck.m_htParamMaxVaule;
		m_htParamStep = itInnerCheck.m_htParamStep;
		m_htParamChildNames = itInnerCheck.m_htParamChildNames;

		HTuple htNumParam;
		int nNumParam;
		TupleLength(m_htParamProp, &htNumParam);
		nNumParam = htNumParam[0]/*.I()*/;
		s_TypeParam  sTypeParam;
		CString   strParamName, strParamMeaning;
		double dParamVaule, dParamMinVaule, dParamMaxVaule, dParamStep;
		int nTypeFlag;

		for (int i = 0; i < nNumParam; i++)
		{
			int nParamProp;
			nParamProp = m_htParamProp[i]/*.I()*/;
			if (nParamProp == 0)
			{
				strParamName = m_htParamName[i]/*.S()*/;
				strParamMeaning = m_htParamMeaning[i]/*.S()*/;
				dParamVaule = m_htParamVaule[i]/*.D()*/;
				nTypeFlag = m_htParamType[i]/*.I()*/;
				dParamMinVaule = m_htParamMinVaule[i]/*.D()*/;
				dParamMaxVaule = m_htParamMaxVaule[i]/*.D()*/;
				dParamStep = m_htParamStep[i]/*.I()*/;

				sTypeParam.nParamProp = nParamProp;
				sTypeParam.strParamName = strParamName;
				sTypeParam.strParamUnit = strParamMeaning;
				sTypeParam.dCurValue = dParamVaule;
				sTypeParam.nTypeFlag = nTypeFlag;
				sTypeParam.dMinValue = dParamMinVaule;
				sTypeParam.dMaxValue = dParamMaxVaule;
				sTypeParam.dMinStep = dParamStep;

				if (nTypeFlag == 3)
				{
					CString   strChildParamName;
					strChildParamName = m_htParamChildNames[i];
					if (strChildParamName.GetLength() > 0)
					{
						CString* pStr;
						int iSubStrs;
						pStr = SplitString(strChildParamName, ';', iSubStrs);
						if (iSubStrs > 1)
						{
							for (int i = 0; i < iSubStrs; i++)
							{
								/*CString str1;
								str1 = pStr[i];*/
								sTypeParam.vsParamEnumValues.push_back(pStr[i]);
							}

						}
						delete[]pStr;
					}
				}
				psCheckParams->lstTypeParam.push_back(sTypeParam);
			}
		}
	}
	catch (HException &hException)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Halcon初始化异常");

		CString strError;
		strError.Format(_T("Check-Halcon初始化异常- "));
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Init()异常");
		// 写异常日志
		CString strErr;
		strErr.Format(_T("Check-Init()异常"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strErr);
	}
	return m_sStatus;
}


//加载任务
//******************************************************************************//
//函数:加载任务
//参数:
//	strTaskName    输入，任务名
//  lstRegion      输入，任务所有检测区域链表
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CCheck::LoadTask(const CString strTaskName, const list<s_ModelRegionPro> lstRegion)
{
	//生成任务路径
	try
	{
		m_strTaskName = strTaskName;
		m_strTaskPath.Format(_T("%s\\%s\\AlgTask"), m_sAlgInitParam.strAlgModePath, strTaskName);
		m_strAlgName.Format(_T("StitchCheck"));

		//清空检测链表
		Free();

		//生成检测链表,并对每个区域进行初始化
		std::list<s_ModelRegionPro>::const_iterator itRegionProperty;
		for (itRegionProperty = lstRegion.begin(); itRegionProperty != lstRegion.end(); itRegionProperty++)
		{
			CInnerCheck  cInnerCheck;
			cInnerCheck.m_sDstRegionProperty = *itRegionProperty;
			cInnerCheck.m_strLogPath = m_strLogPath;

			cInnerCheck.m_FileOperate = &m_FileOperate;
			cInnerCheck.Init(m_strTaskName, m_sAlgInitParam, *itRegionProperty, TRUE);

			m_listInnerCheck.push_back(cInnerCheck);
		}


		m_FileOperate.WriteAlgOperateLog(m_strLogPath, _T("操作日志"), _T("操作日志"), _T("检测:加载任务"));
	}
	catch (HException &hException)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Halcon-LoadTask异常");

		CString strError;
		strError.Format(_T("Check-Halcon-LoadTask异常- "));
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-LoadTask()异常");
		// 写异常日志
		CString strErr;
		strErr.Format(_T("Check-LoadTask()异常"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strErr);
	}
	return m_sStatus;
}

//获取检测参数
s_Status CCheck::GetAlgParam(const s_ErrorTpye sErrorTpye, s_TypeCheckParam  * psCheckParams)
{
	try
	{
		//psCheckParams->nALGTypeID = ALG_TYPE_ID;
		psCheckParams->strAlgTypeName = _T("AlgStitchCheck");
		if (m_listInnerCheck.size() > 0)
		{
			std::list<CInnerCheck>::iterator itInnerCheck;
			itInnerCheck = m_listInnerCheck.begin();
			itInnerCheck->ReadPara();

			m_htParamProp.Clear();
			m_htParamName.Clear();
			m_htParamMeaning.Clear();
			m_htParamVaule.Clear();

			m_htParamType.Clear();
			m_htParamMinVaule.Clear();
			m_htParamMaxVaule.Clear();
			m_htParamStep.Clear();
			m_htParamChildNames.Clear();

			m_htParamProp = itInnerCheck->m_htParamProp;
			m_htParamName = itInnerCheck->m_htParamName;
			m_htParamMeaning = itInnerCheck->m_htParamMeaning;
			m_htParamVaule = itInnerCheck->m_htParamVaule;

			m_htParamType = itInnerCheck->m_htParamType;
			m_htParamMinVaule = itInnerCheck->m_htParamMinVaule;
			m_htParamMaxVaule = itInnerCheck->m_htParamMaxVaule;
			m_htParamStep = itInnerCheck->m_htParamStep;
			m_htParamChildNames = itInnerCheck->m_htParamChildNames;

			HTuple htNumParam;
			int nNumParam;
			TupleLength(m_htParamProp, &htNumParam);
			nNumParam = htNumParam[0]/*.I()*/;

			CString   strParamName, strParamMeaning;
			double dParamVaule, dParamMinVaule, dParamMaxVaule, dParamStep;
			int nTypeFlag;

			switch (sErrorTpye.nErrorType)
			{
			case 2:
				for (int i = 0; i < nNumParam; i++)
				{
					s_TypeParam  sTypeParam;
					int nParamProp;
					nParamProp = m_htParamProp[i]/*.I()*/;
					if ((nParamProp == 0)/*&&(i== nNumParam-1)*/)//检测参数
					{
						strParamName = m_htParamName[i]/*.S()*/;
						strParamMeaning = m_htParamMeaning[i]/*.S()*/;
						dParamVaule = m_htParamVaule[i]/*.D()*/;
						nTypeFlag = m_htParamType[i]/*.I()*/;
						dParamMinVaule = m_htParamMinVaule[i]/*.D()*/;
						dParamMaxVaule = m_htParamMaxVaule[i]/*.D()*/;
						dParamStep = m_htParamStep[i]/*.I()*/;

						sTypeParam.nParamProp = nParamProp;
						sTypeParam.strParamName = strParamName;
						sTypeParam.strParamUnit = strParamMeaning;
						sTypeParam.dCurValue = dParamVaule;
						sTypeParam.nTypeFlag = nTypeFlag;
						sTypeParam.dMinValue = dParamMinVaule;
						sTypeParam.dMaxValue = dParamMaxVaule;
						sTypeParam.dMinStep = dParamStep;

						if (nTypeFlag == 3)
						{
							CString   strChildParamName;
							strChildParamName = m_htParamChildNames[i];
							if (strChildParamName.GetLength() > 0)
							{
								CString* pStr;
								int iSubStrs;
								pStr = SplitString(strChildParamName, ';', iSubStrs);
								if (iSubStrs >= 1)
								{
									for (int j = 0; j < iSubStrs; j++)
									{
										/*CString str1;
										str1 = pStr[i];*/
										sTypeParam.vsParamEnumValues.push_back(pStr[j]);
									}

								}
								delete[]pStr;
							}
						}
						psCheckParams->lstTypeParam.push_back(sTypeParam);
					}
				}
				break;
			default:
				for (int i = 0; i < nNumParam; i++)
				{
					s_TypeParam  sTypeParam;
					int nParamProp;
					nParamProp = m_htParamProp[i]/*.I()*/;
					if (nParamProp == 0)//检测参数
					{
						strParamName = m_htParamName[i]/*.S()*/;
						strParamMeaning = m_htParamMeaning[i]/*.S()*/;
						dParamVaule = m_htParamVaule[i]/*.D()*/;
						nTypeFlag = m_htParamType[i]/*.I()*/;
						dParamMinVaule = m_htParamMinVaule[i]/*.D()*/;
						dParamMaxVaule = m_htParamMaxVaule[i]/*.D()*/;
						dParamStep = m_htParamStep[i]/*.D()*/;

						sTypeParam.nParamProp = nParamProp;
						sTypeParam.strParamName = strParamName;
						sTypeParam.strParamUnit = strParamMeaning;
						sTypeParam.dCurValue = dParamVaule;
						sTypeParam.nTypeFlag = nTypeFlag;
						sTypeParam.dMinValue = dParamMinVaule;
						sTypeParam.dMaxValue = dParamMaxVaule;
						sTypeParam.dMinStep = dParamStep;

						if (nTypeFlag == 3)
						{
							CString   strChildParamName;
							strChildParamName = m_htParamChildNames[i];
							if (strChildParamName.GetLength() > 0)
							{
								CString* pStr;
								int iSubStrs;
								pStr = SplitString(strChildParamName, ';', iSubStrs);
								if (iSubStrs >= 1)
								{
									for (int i = 0; i < iSubStrs; i++)
									{
										/*CString str1;
										str1 = pStr[i];*/
										sTypeParam.vsParamEnumValues.push_back(pStr[i]);
									}

								}
								delete[]pStr;
							}
						}
						psCheckParams->lstTypeParam.push_back(sTypeParam);
					}
				}
				break;

			}
		}
	}
	catch (HException &hException)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Halcon-GetAlgParam异常");

		CString strError;
		strError.Format(_T("Check-Halcon-GetAlgParam异常- "));
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-GetAlgParam()异常");
		// 写异常日志
		CString strErr;
		strErr.Format(_T("Check-GetAlgParam()异常"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strErr);
	}
	return m_sStatus;
}
//获取界面参数
s_Status CCheck::GetAlgParam(const s_ErrorTpye sErrorTpye, s_TypeCheckParam  * psCheckParams, const s_ModelRegionPro& sSrcRegion, const BOOL bAllowModify)
{
	try
	{
		psCheckParams->strAlgTypeName = _T("AlgStitchCheck");

		CInnerCheck  itInnerCheck;
		itInnerCheck.m_sDstRegionProperty = sSrcRegion;
		itInnerCheck.m_strLogPath = m_strLogPath;

		itInnerCheck.m_FileOperate = &m_FileOperate;
		itInnerCheck.Init(m_strTaskName, m_sAlgInitParam, sSrcRegion, TRUE);
		itInnerCheck.ReadPara();

		m_htParamProp.Clear();
		m_htParamName.Clear();
		m_htParamMeaning.Clear();
		m_htParamVaule.Clear();
		m_htParamType.Clear();
		m_htParamMinVaule.Clear();
		m_htParamMaxVaule.Clear();
		m_htParamStep.Clear();
		m_htParamChildNames.Clear();

		m_htParamProp = itInnerCheck.m_htParamProp;
		m_htParamName = itInnerCheck.m_htParamName;
		m_htParamMeaning = itInnerCheck.m_htParamMeaning;
		m_htParamVaule = itInnerCheck.m_htParamVaule;
		m_htParamType = itInnerCheck.m_htParamType;
		m_htParamMinVaule = itInnerCheck.m_htParamMinVaule;
		m_htParamMaxVaule = itInnerCheck.m_htParamMaxVaule;
		m_htParamStep = itInnerCheck.m_htParamStep;
		m_htParamChildNames = itInnerCheck.m_htParamChildNames;

		HTuple htNumParam;
		int nNumParam;
		TupleLength(m_htParamProp, &htNumParam);
		nNumParam = htNumParam[0];


		CString   strParamName, strParamMeaning;
		double dParamVaule, dParamMinVaule, dParamMaxVaule, dParamStep;
		int nTypeFlag;

		for (int i = 0; i < nNumParam; i++)
		{
			s_TypeParam  sTypeParam;
			int nParamProp;
			nParamProp = m_htParamProp[i];

			strParamName = m_htParamName[i];
			strParamMeaning = m_htParamMeaning[i];
			dParamVaule = m_htParamVaule[i];
			nTypeFlag = m_htParamType[i];
			dParamMinVaule = m_htParamMinVaule[i];
			dParamMaxVaule = m_htParamMaxVaule[i];
			dParamStep = m_htParamStep[i];

			sTypeParam.nParamProp = nParamProp;
			sTypeParam.strParamName = strParamName;
			sTypeParam.strParamUnit = strParamMeaning;
			sTypeParam.dCurValue = dParamVaule;
			sTypeParam.nTypeFlag = nTypeFlag;
			sTypeParam.dMinValue = dParamMinVaule;
			sTypeParam.dMaxValue = dParamMaxVaule;
			sTypeParam.dMinStep = dParamStep;

			if (nTypeFlag == 3)
			{
				CString   strChildParamName;
				strChildParamName = m_htParamChildNames[i];
				if (strChildParamName.GetLength() > 0)
				{
					CString* pStr;
					int iSubStrs;
					pStr = SplitString(strChildParamName, ';', iSubStrs);
					if (iSubStrs > 1)
					{
						for (int j = 0; j < iSubStrs; j++)
						{
							/*CString str1;
							str1 = pStr[i];*/
							sTypeParam.vsParamEnumValues.push_back(pStr[j]);
						}

					}
					else if (iSubStrs == 1 && (strChildParamName.Find(' ') == -1))//只有一个参数
					{
						sTypeParam.vsParamEnumValues.push_back(strChildParamName);
					}
					delete[]pStr;
				}
			}
			psCheckParams->lstTypeParam.push_back(sTypeParam);
		}
	}
	catch (HException &hException)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Halcon-GetAlgParam-建模界面异常");

		CString strError;
		strError.Format(_T("Check-Halcon-GetAlgParam-建模界面异常- "));
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-GetAlgParam()-建模界面异常");
		// 写异常日志
		CString strErr;
		strErr.Format(_T("Check-GetAlgParam()-建模界面异常"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strErr);
	}
	return m_sStatus;
}
//设置检测参数
s_Status CCheck::SetAlgParam(const s_ErrorTpye sErrorTpye, const s_TypeCheckParam  sCheckParams)
{
	try
	{
		//遍历每个检测区域，并设置检测参数
		if (m_listInnerCheck.size() > 0 && sCheckParams.lstTypeParam.size() > 0)
		{
			HTuple htNumParam;
			int nNumParam;
			TupleLength(m_htParamProp, &htNumParam);
			nNumParam = htNumParam[0]/*.I()*/;

			std::list<CInnerCheck>::iterator itInnerCheck;
			for (itInnerCheck = m_listInnerCheck.begin(); itInnerCheck != m_listInnerCheck.end(); itInnerCheck++)
			{
				std::list<s_TypeParam> lstTemp;
				lstTemp = sCheckParams.lstTypeParam;
				std::list<s_TypeParam>::iterator itTypeParam;
				for (itTypeParam = lstTemp.begin(); itTypeParam != lstTemp.end(); itTypeParam++)
				{
					CString strParamName = itTypeParam->strParamName;
					for (int i = 0; i < nNumParam; i++)
					{
						CString strParamName0;
						strParamName0 = m_htParamName[i]/*.S()*/;
						if (strParamName0 == strParamName)
						{
							itInnerCheck->m_htParamVaule[i] = itTypeParam->dCurValue;
						}
					}
				}
				itInnerCheck->WritePara();
			}
		}
	}
	catch (HException &hException)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Halcon-SetAlgParam异常");

		CString strError;
		strError.Format(_T("Check-Halcon-SetAlgParam异常- "));
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-SetAlgParam()异常");
		// 写异常日志
		CString strErr;
		strErr.Format(_T("Check-SetAlgParam()异常"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strErr);
	}
	return m_sStatus;
}
//设置界面参数
s_Status CCheck::SetAlgParam(const s_ErrorTpye sErrorTpye, const s_TypeCheckParam  sCheckParams, const s_ModelRegionPro& sSrcRegion)
{
	try
	{
		//当前检测区域，并设置检测参数
		if (/*m_listInnerCheck.size() > 0 &&*/ sCheckParams.lstTypeParam.size() > 0)
		{
			CInnerCheck  itInnerCheck;
			itInnerCheck.m_sDstRegionProperty = sSrcRegion;

			itInnerCheck.m_strLogPath = m_strLogPath;
			itInnerCheck.m_FileOperate = &m_FileOperate;
			itInnerCheck.Init(m_strTaskName, m_sAlgInitParam, sSrcRegion, TRUE);

			HTuple htNumParam;
			int nNumParam;
			TupleLength(m_htParamProp, &htNumParam);
			nNumParam = htNumParam[0].I();

			std::list<s_TypeParam> lstTemp;
			lstTemp = sCheckParams.lstTypeParam;
			std::list<s_TypeParam>::iterator itTypeParam;
			for (itTypeParam = lstTemp.begin(); itTypeParam != lstTemp.end(); itTypeParam++)
			{
				CString strParamName = itTypeParam->strParamName;
				for (int i = 0; i < nNumParam; i++)
				{
					CString strParamName0;
					strParamName0 = m_htParamName[i];
					if (strParamName0 == strParamName)
					{
						itInnerCheck.m_htParamVaule[i] = itTypeParam->dCurValue;
					}
				}
			}
			itInnerCheck.WritePara();
		}
	}
	catch (HException &hException)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Halcon-SetAlgParam-界面异常");

		CString strError;
		strError.Format(_T("Check-Halcon-SetAlgParam-界面异常- "));
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-SetAlgParam()-界面异常");
		// 写异常日志
		CString strErr;
		strErr.Format(_T("Check-SetAlgParam()-界面异常"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strErr);
	}
	return m_sStatus;
}
//自动建模
s_Status CCheck::AutoCreateAlgModel(const s_AlgCheckInputParam& sAlgInputParam,
	const s_ModelRegionPro& sSrcRegion,
	CString strAlgModeName,
	s_TypeCheckParam  sCheckParams, BOOL bIsModelImg,
	s_Status &CreateOrLearnState)
{
	try
	{
		CInnerCheck      *pTempChecker;       // 内部指针，临时测试用
		pTempChecker = NULL;
		pTempChecker = new CInnerCheck;
		pTempChecker->m_strLogPath = m_strLogPath;
		pTempChecker->m_FileOperate = &m_FileOperate;
		pTempChecker->Init(strAlgModeName, m_sAlgInitParam, sSrcRegion, TRUE);
		HObject hImgSrc, hShowReg;
		GenEmptyObj(&hShowReg);
		std::list<s_InputTimeImage>::const_iterator itAlgCInParam;
		itAlgCInParam = sAlgInputParam.lstInputImage.begin();
		//CopyObj(*(itAlgCInParam->pChannelHImg), &hImgSrc, 1, -1);

		//算法参数赋值
		HTuple htNumParam;
		int nNumParam;
		TupleLength(m_htParamProp, &htNumParam);
		nNumParam = htNumParam[0].I();

		std::list<s_TypeParam> lstTemp;
		lstTemp = sCheckParams.lstTypeParam;
		std::list<s_TypeParam>::iterator itTypeParam;
		for (itTypeParam = lstTemp.begin(); itTypeParam != lstTemp.end(); itTypeParam++)
		{
			CString strParamName = itTypeParam->strParamName;
			for (int i = 0; i < nNumParam; i++)
			{
				CString strParamName0;
				strParamName0 = m_htParamName[i];
				if (strParamName0 == strParamName)
				{
					pTempChecker->m_htParamVaule[i] = itTypeParam->dCurValue;
				}
			}
		}
		HObject hModelImgSrc;
		ReduceDomain(hImgSrc, sSrcRegion.hRegion, &hModelImgSrc);
		CropDomain(hModelImgSrc, &hModelImgSrc);
		//s_Status CreateOrLearnState;
		if (bIsModelImg)//创建模板
		{
			pTempChecker->AutoCreateAlgMode(hModelImgSrc, &hShowReg, pTempChecker->m_htParamVaule, pTempChecker->m_htResolution, &pTempChecker->m_htParamVaule, CreateOrLearnState);
			//提取区域移到大图  hShowRegNew 建模界面显示
			HTuple htNumObj;
			CountObj(hShowReg, &htNumObj);
			if (htNumObj > 0)
			{
				HObject hShowRegNew;
				HTuple htRow11, htColumn11, htRow21, htColumn21;
				SmallestRectangle1(sSrcRegion.hRegion, &htRow11, &htColumn11, &htRow21, &htColumn21);
				for (int i = 0; i < htNumObj; i++)
				{
					HObject hShowR;
					SelectObj(hShowReg, &hShowR, i + 1);
					if (i == 0)
					{
						MoveRegion(hShowR, &hShowRegNew, htRow11, htColumn11);
					}
					else
					{
						HObject hShowRNew;
						MoveRegion(hShowR, &hShowRNew, htRow11, htColumn11);
						ConcatObj(hShowRegNew, hShowRNew, &hShowRegNew);
					}
				}
			}

			//算法参数传出
			std::list<s_TypeParam> lstTemp1;
			lstTemp1 = sCheckParams.lstTypeParam;
			std::list<s_TypeParam>::iterator itTypeParam1;
			for (itTypeParam1 = lstTemp1.begin(); itTypeParam1 != lstTemp1.end(); itTypeParam1++)
			{
				CString strParamName = itTypeParam1->strParamName;
				for (int i = 0; i < nNumParam; i++)
				{
					CString strParamName0;
					//strParamName0 = m_htParamName[i].S();
					if (strParamName0 == strParamName)
					{
						itTypeParam1->dCurValue = pTempChecker->m_htParamVaule[i];
					}
				}
			}
		}
		else//添加模板
		{
			pTempChecker->AutoLearnAlgMode(hModelImgSrc, CreateOrLearnState);
		}

		if (pTempChecker)
		{
			pTempChecker->Free();
			delete pTempChecker;
			pTempChecker = NULL;
		}
	}
	catch (HException &hException)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Halcon-AutoCreateAlgModel异常");

		CString strError;
		strError.Format(_T("Check-Halcon-AutoCreateAlgModel异常- "));
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strError);

		CreateOrLearnState.nErrorType = 1;
		CreateOrLearnState.strErrorContext = m_sStatus.strErrorContext;
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-AutoCreateAlgModel()异常");
		// 写异常日志
		CString strErr;
		strErr.Format(_T("Check-AutoCreateAlgModel()异常"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strErr);

		CreateOrLearnState.nErrorType = 1;
		CreateOrLearnState.strErrorContext = m_sStatus.strErrorContext;
	}
	return m_sStatus;
}

//释放空间
//******************************************************************************//
//函数:释放空间
//参数:NULL
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CCheck::Free()
{
	if (m_listInnerCheck.size() > 0)
	{
		std::list<CInnerCheck>::iterator itInnerCheck;
		for (itInnerCheck = m_listInnerCheck.begin();
			itInnerCheck != m_listInnerCheck.end();
			itInnerCheck++)
		{
			itInnerCheck->Free();
		}
		m_listInnerCheck.clear();
		m_FileOperate.WriteAlgOperateLog(m_strLogPath, _T("操作日志"), _T("操作日志"), _T("CCheck->Free():调用itInnerCheck->Free()"));
	}

	return m_sStatus;
}

//设置中间结果
//******************************************************************************//
//函数:设置中间结果
//参数:
//  s_MiddleResult 中间结果结构体
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CCheck::SetMiddleResult(const s_MiddleResult &sMiddleResult)
{
	//m_sMiddleResult.vsLocGroupResult = sMiddleResult.vsLocGroupResult;
	m_sMiddleResult = sMiddleResult;
	return m_sStatus;
}


//独立检测主函数
//******************************************************************************//
//函数:独立检测主函数
//参数:
//	sAlgSingleCheckInputParam      输入，检测输入结构体
//  bParallel					   输入，是否并行
//  bModelTest					   输入，是否模板测试- 管理层界面
//	sAlgSingleCheckOutputResult    输出，返回检测结果输出结构体
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CCheck::Check(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
    s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
    try
    {
        std::list<s_InputTimeImage>::const_iterator itAlgCInParam, itAlgCInParamEnd;
        itAlgCInParam = sAlgCheckInputParam.lstInputImage.begin();
        itAlgCInParamEnd = sAlgCheckInputParam.lstInputImage.end();
        for (; itAlgCInParam != itAlgCInParamEnd; itAlgCInParam++)//多幅图像
        {
            HObject hImgSrc;
            CopyObj(*(itAlgCInParam->pChannelHImg), &hImgSrc, 1, -1);
            HTuple htChannelNum;
            int nChannelNum;
            //CountChannels(hImgSrc, &htChannelNum);
            CountObj(hImgSrc, &htChannelNum);
            nChannelNum = htChannelNum[0].I();
            //判断图像数据是否符合要求
            if (nChannelNum == 3)
            {
                HTuple htImgW, htImgH;
                GetImageSize(hImgSrc, &htImgW, &htImgH);
                int nImgWR, nImgHR, nImgWG, nImgHG, nImgWB, nImgHB;
                nImgWR = htImgW[0].I();
                nImgHR = htImgH[0].I();
                nImgWG = htImgW[1].I();
                nImgHG = htImgH[1].I();
                nImgWB = htImgW[2].I();
                nImgHB = htImgH[2].I();


                if (nImgWR != itAlgCInParam->sTimeImgConf.cx
                    || nImgHR != itAlgCInParam->sTimeImgConf.cy
                    || nImgWG != itAlgCInParam->sTimeImgConf.cx
                    || nImgHG != itAlgCInParam->sTimeImgConf.cy
                    || nImgWB != itAlgCInParam->sTimeImgConf.cx
                    || nImgHB != itAlgCInParam->sTimeImgConf.cy)
                {
                    m_sStatus.nErrorType = 1;
                    m_sStatus.strErrorContext = _T("图像输入数据为空");
                    m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("检测类"), m_sStatus.strErrorContext);
                    return m_sStatus;
                }
            }
           else
            {
                m_sStatus.nErrorType = 1;
                m_sStatus.strErrorContext = _T("图像输入数据位数不对");
                m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("检测类"), m_sStatus.strErrorContext);
                return m_sStatus;
            }

            //逐个区域进行检测，并计算算法处理时间
            double dTimeStart, dTimeCount;
            HTuple hv_TimeStart, hv_TimeCount;
            s_InnerCheckInputParam  sAlgInnerCheckInputParam;
            s_InnerSingleCheckOutputParam sInnerSingleCheckOutputResult;

            //输入参数
            sAlgInnerCheckInputParam.sInputTimeImage = *itAlgCInParam;
            sAlgCheckOutputResult->lst_vec_clear();

            std::list<CInnerCheck>::iterator itInnerCheck;
            for (itInnerCheck = m_listInnerCheck.begin(); itInnerCheck != m_listInnerCheck.end(); itInnerCheck++)//多个区域
            {
                //计算检测时间开始
                CountSeconds(&hv_TimeStart);
                dTimeStart = hv_TimeStart[0].D();
                sAlgInnerCheckInputParam.sRegionProperty = itInnerCheck->m_sDstRegionProperty;

                //获得定位组信息
                if (m_sMiddleResult.vsLocGroupResult.size() > 0)
                {
                    for (int i = 0; i < (int)m_sMiddleResult.vsLocGroupResult.size(); i++)
                    {
                        if (m_sMiddleResult.vsLocGroupResult[i].nLocGroupID == sAlgInnerCheckInputParam.sRegionProperty.nLocGroupID)
                        {
                            sAlgInnerCheckInputParam.sLocGroupResult = m_sMiddleResult.vsLocGroupResult[i];
                            break;
                        }
                    }
                }
                //单个区域的实际检测
                itInnerCheck->SingleCheck(sAlgInnerCheckInputParam, hImgSrc, &sInnerSingleCheckOutputResult);

                //计算检测时间结束
                CountSeconds(&hv_TimeCount);
                dTimeCount = hv_TimeCount[0].D();
                dTimeCount -= dTimeStart;
                sInnerSingleCheckOutputResult.sSingleErrorInfo.nResvered[0] = dTimeCount * 1000;
                //输出检测结果
                sInnerSingleCheckOutputResult.sSingleErrorInfo.TrainResultFlag = (eTrainResultFlag)sInnerSingleCheckOutputResult.nFlagStudy;

                sAlgCheckOutputResult->nSingleErrorCount++;   //有效信息个数
                sAlgCheckOutputResult->lstSingleErrorInfo.push_back(sInnerSingleCheckOutputResult.sSingleErrorInfo);
            }
        }

    }
    catch (HException &hException)
    {
        m_sStatus.nErrorType = 1;
        m_sStatus.strErrorContext = _T("Check-Halcon-Check异常");

        CString strError;
        strError.Format(_T("Check-Halcon-Check异常- "));
        strError += hException.ProcName().Text();
        strError += _T("异常");
        strError += hException.ErrorMessage().Text();
        m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strError);
    }
    catch (...)
    {
        m_sStatus.nErrorType = 1;
        m_sStatus.strErrorContext = _T("Check-Check()异常");
        // 写异常日志
        CString strErr;
        strErr.Format(_T("Check-Check()异常"));
        m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strErr);
    }
    return m_sStatus;
}

//定位算法调用重载检测函数
//******************************************************************************//
//函数:定位算法调用重载检测函数
//参数:
//	sAlgCheckInputParam    输入，检测输入结构体
//	sMiddleResult          输出，返回定位算法结果链表
//  sAlgCheckOutputResult  输出，返回检测结果输出结构体
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CCheck::Check(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
	s_MiddleResult* sMiddleResult,
	s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
	return m_sStatus;
}
//联合检测函数
s_Status CCheck::JoinCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
	s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
	try
	{
	}
	catch (HException &hException)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Halcon-JoinCheck异常");

		CString strError;
		strError.Format(_T("Check-Halcon-JoinCheck异常- "));
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-JoinCheck()异常");
		// 写异常日志
		CString strErr;
		strErr.Format(_T("Check-JoinCheck()异常"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strErr);
	}
	return m_sStatus;
}
//多相机联合检测
s_Status CCheck::MutiCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
	s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
	try
	{
	}
	catch (HException &hException)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Halcon-MutiCheck异常");

		CString strError;
		strError.Format(_T("Check-Halcon-MutiCheck异常- "));
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-MutiCheck()异常");
		// 写异常日志
		CString strErr;
		strErr.Format(_T("Check-MutiCheck()异常"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strErr);
	}
	return m_sStatus;
}
//更新检测参数函数
//******************************************************************************//
//函数:更新检测参数函数
//参数:NULL
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CCheck::UpdateParam()
{
	//更新检测参数
	try
	{
		std::list<CInnerCheck>::iterator itInnerCheck;
		for (itInnerCheck = m_listInnerCheck.begin(); itInnerCheck != m_listInnerCheck.end(); itInnerCheck++)
		{
			itInnerCheck->ReadTempPara();// 更新需要更新的区域 [5/17/2011 Denghl]
		}
	}
	catch (HException &hException)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Halcon-UpdateParam异常");

		CString strError;
		strError.Format(_T("Check-Halcon-UpdateParam异常- "));
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-UpdateParam()异常");
		// 写异常日志
		CString strErr;
		strErr.Format(_T("Check-UpdateParam()异常"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strErr);
	}
	return m_sStatus;
}

//保存检测模板
//******************************************************************************//
//函数:保存检测模板
//参数:NULL
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CCheck::SaveCheckModel()
{
	////删除原有的区域模板参数
	try
	{
		//SHFILEOPSTRUCT   fs;   
		////char strDelete[DH_MAX_PATH]={0};
		//CString /*strTmp*/strDelete;
		//strDelete.Format(_T(""));
		//ZeroMemory(&fs,sizeof(SHFILEOPSTRUCT));   
		//fs.hwnd   = NULL;   
		//fs.wFunc  = FO_DELETE;   
		//fs.pTo    = NULL;   
		//fs.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI;   //FOF_ALLOWUNDO[删除后留存回收站，此处屏蔽]
		//fs.lpszProgressTitle = _T("delefile");
		//strDelete.Format(_T("%s\\%s"),m_strTaskPath,m_strAlgName);
		////strcpy_s(strDelete,strTmp);
		//fs.pFrom  =  strDelete;
		//SHFileOperation(&fs);

		////根据内存数据，保存算法需要的区域模板参数
		//std::list<CInnerCheck>::iterator itInnerCheck;
		//for (itInnerCheck = m_listInnerCheck.begin();itInnerCheck!=m_listInnerCheck.end();itInnerCheck++)
		//{
		//	CString strRegionPath;
		//	strRegionPath.Format(_T(""));
		//	strRegionPath.Format(_T("%s\\%s\\%d"),m_strTaskPath,m_strAlgName,itInnerCheck->m_sDstRegionProperty.nROIIndex);

		//	m_FileOperate.CreateMultiLevelPath(strRegionPath);
		//	
		//	itInnerCheck->SaveCheckModel();//保存模板
		//}
	}
	catch (HException &hException)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Halcon-SaveCheckModel异常");

		CString strError;
		strError.Format(_T("Check-Halcon-SaveCheckModel异常- "));
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-SaveCheckModel()异常");
		// 写异常日志
		CString strErr;
		strErr.Format(_T("Check-SaveCheckModel()异常"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("Check()类"), strErr);
	}
	return m_sStatus;
}

/////////////////////////////////////////////////////////////////////////////
// 算法属性设置实现类实现
/////////////////////////////////////////////////////////////////////////////

// 检测算法库实现类构造函数和析构函数
//******************************************************************************//
CSetPROP::CSetPROP()
{

}
CSetPROP::~CSetPROP()
{

}

//属性设置初始化
//******************************************************************************//
//函数:属性设置初始化
//参数:
//	strTaskName      输入，任务名
//	sAlgInitParam    输入，全局路径
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CSetPROP::Init(CString strTaskName, const s_AlgInitParam& sAlgInitParam)
{
	m_pChecker = (CCheck*)((CDHAlgStitchCheck*)pChecker)->m_pAlgStitchCheck;
	m_sAlgInitParam = sAlgInitParam;
	m_strTaskName = strTaskName;
	m_pChecker->m_strTaskName = strTaskName;

	m_strAlgName = _T("AlgStitchCheck");
	return m_sStatus;
}

//属性设置主函数
//******************************************************************************//
//函数:属性设置主函数
//参数:
//	sAlgImageInfo     输入，图像结构体   
//	sRegionProperty   输入，区域属性
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CSetPROP::SetAlgParamDlg(const s_AlgCheckInputParam& sAlgInputParam, \
	const s_ModelRegionPro& sRegion, BOOL bAllowModify)
{
	CPropDlg Prodlg;
	Prodlg.m_pChecker = m_pChecker;      //检测指针
	std::list<s_InputTimeImage>::const_iterator itAlgCInParam;
	itAlgCInParam = sAlgInputParam.lstInputImage.begin();

    CopyImage(*(itAlgCInParam->pChannelHImg), &(Prodlg.m_hImgSrcBig));
    /*HTuple channelsM,channelsP;

    CountChannels(*(itAlgCInParam->pChannelHImg), &channelsP);
    int p = channelsP.I();
    CountChannels(*(itAlgCInParam->pMulHImage), &channelsM);
    
    int m = channelsM.I();*/

	Prodlg.m_sAlgImageInfo = *itAlgCInParam;   //图像信息
	Prodlg.m_sDstRegionProperty = sRegion; //区域属性
	Prodlg.m_bAllowModify = bAllowModify;    //是否模板图
    Prodlg.m_regions = &m_regions;
	//弹对话框,进行相关操作
	if (Prodlg.DoModal() == IDOK)
	{
		;
	}
    
	return m_sStatus;
}

//复制区域
//******************************************************************************//
//函数:复制区域
//参数:
//	sAlgImageInfo        输入，图像结构体   
//	sDstRegionProperty   输出，目的区域属性
//  sSrcRegionProperty   输入，源区域属性
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CSetPROP::AlgCopyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
	s_ModelRegionPro& sNewRegion, const s_ModelRegionPro& sSrcRegion)
{
	//[目前未用]
	return m_sStatus;
}

//修改区域大小
//******************************************************************************//
//函数:修改区域大小
//功能：修改区域大小后，调用函数（可重新设置或生成模板等）
//参数:
//	sAlgImageInfo        输入，图像结构体   
//	sSrcRegionProperty   输入，区域属性
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CSetPROP::AlgModifyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
	const s_ModelRegionPro& sSrcRegion, const s_ModelRegionPro& sNewRegion)
{
	//区域调整后,将原区域的参数拷贝到修改后的新区域
	int nSrcRegionID = sSrcRegion.nROIIndex;
	int nDstRegionID = sNewRegion.nROIIndex;

	SHFILEOPSTRUCT   fs;
	CString /*strTmp,*/ strSrc, strDst;
	//char strSrc[DH_MAX_PATH] = {0};
	//char strDst[DH_MAX_PATH] = {0};

	//ZeroMemory(strSrc, MAX_PATH);
	//ZeroMemory(strDst, MAX_PATH);
	ZeroMemory(&fs, sizeof(SHFILEOPSTRUCT));
	strSrc.Format(_T(""));
	strDst.Format(_T(""));

	strSrc.Format(_T("%s\\%s\\AlgTask\\%s\\%d"), m_sAlgInitParam.strAlgModePath, m_strTaskName, m_strAlgName, nSrcRegionID);
	//memcpy(strSrc, strTmp, strTmp.GetLength());

	//strTmp.Empty();
	strDst.Format(_T("%s\\%s\\AlgTask\\%s\\%d"), m_sAlgInitParam.strAlgModePath, m_strTaskName, m_strAlgName, nDstRegionID);
	//memcpy(strDst, strTmp, strTmp.GetLength());

	fs.hwnd = NULL;
	fs.wFunc = FO_COPY;
	fs.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI | FOF_MULTIDESTFILES; //FOF_ALLOWUNDO  
	fs.lpszProgressTitle = _T("copyfile");
	fs.pFrom = strSrc;
	fs.pTo = strDst;

	m_FileOperate.CreateMultiLevelPath(strDst);
	SHFileOperation(&fs);

	return m_sStatus;
}

//删除区域
//******************************************************************************//
//函数:删除区域
//参数:
//	lstnRegionID     输入，区域ID链表   

//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CSetPROP::AlgDeleteRegion(const list<s_ModelRegionPro>& lstRegion)
{
	std::list<s_ModelRegionPro>::const_iterator itRegion;
	if (lstRegion.size() <= 0)
	{
		return m_sStatus;
	}
	for (itRegion = lstRegion.begin(); itRegion != lstRegion.end(); itRegion++)
	{
		//删除区域时，将删除对应的区域信息文件夹 
		SHFILEOPSTRUCT   fs;
		//char strDelete[DH_MAX_PATH]={0};
		CString /*strTmp*/strDelete;
		strDelete.Format(_T(""));
		ZeroMemory(&fs, sizeof(SHFILEOPSTRUCT));
		fs.hwnd = NULL;
		fs.wFunc = FO_DELETE;
		fs.pTo = NULL;
		fs.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI;//FOF_ALLOWUNDO
		fs.lpszProgressTitle = _T("delefile");
		strDelete.Format(_T("%s\\%s\\AlgTask\\%s\\%d"), m_sAlgInitParam.strAlgModePath, m_strTaskName, m_strAlgName, itRegion->nROIIndex);
		//int nLength=strTmp.GetLength();
		//strcpy(strDelete,strTmp);
		fs.pFrom = strDelete;
		SHFileOperation(&fs);
	}
	return m_sStatus;
}

//得到算法内部类型
//******************************************************************************//
//函数:得到算法内部类型
//参数:
//	lstInnerAlgType     输出，算法内部支持算法类型信息链表
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CSetPROP::GetAlgorithmInfo(list<s_InnerAlgType>& lstInnerAlgType)
{
	s_Status sStatus;              //执行状态
	lstInnerAlgType.clear();
	s_InnerAlgType sInnerAlgType;

	//添加内部检测算法
	sInnerAlgType.nInnerTypeID = 1;
	sInnerAlgType.bPopUpDlg = FALSE;
	sInnerAlgType.bSupportLearn = FALSE;
	//设置“内部算法名称”，根据实际情况进行设定
	sInnerAlgType.strTypeName.Format(_T("针脚检测"));
	lstInnerAlgType.push_back(sInnerAlgType);

	return sStatus;
}
