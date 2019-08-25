/************************************************************************
/* Copyright (c) 2015, �������ͼ���Ӿ����޹�˾ӡ����ҵ��
/* All rights reserved.
/*
/* �ļ����ƣ�  Check.cpp
/* ��Ҫ˵����  ����㷨��ʵ������㷨��������ʵ���ࡣ
/*
/* �����ߣ�  �˺���
/* �汾��    V1.0
/* �޸��ˣ�  ������
/* �汾��    V2.0
/* �޸�ʱ�䣺2015-06-04
/* �޸ļ�¼��1. ��ʽ����ܴ��룬��ǿ�ɶ���
			  2. ����ÿһ���������Ĺ��ܣ��������Ӧ�Ĵ������
				 ˵���������������Check.cpp�еĴ����벻Ҫ����
			  3. ������ÿ��������ע��˵��
/* �޸��ˣ�  ����Ƽ
/* �汾��    V1.2.0.2
/* �޸�ʱ�䣺2017-06-29
/* �޸ļ�¼��1.�޸�halcon13�������
	  2. �޸ĳ�ʼ������������һ��������������
	  3. ���ӻ�ȡ��������GetAlgParam(const s_ErrorTpye sErrorTpye,s_TypeCheckParam  * psCheckParams)
	  4. �������ò�������SetAlgParam(const s_ErrorTpye sErrorTpye,const s_TypeCheckParam  sCheckParams)

/* �޸��ˣ�  �Ų�
/* �汾��    V3.0
/* �޸�ʱ�䣺2018-11-07
/* �޸ļ�¼��1.�����ӿ��ļ�
			 2.���ӽ�ģ�����ȡ��������GetAlgParam
			 3.���ӽ�ģ�������ò�������SetAlgParam
			 4.�����Զ���ģ����AutoCreateAlgModel
			 5.֧��halcon����ֱ�ӵ���,�������Ρ���ģ�����
			 6.���ϸ��Ϊ���������Ϻ��ںϼ��
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
// ����㷨��ʵ����ʵ��
/////////////////////////////////////////////////////////////////////////////

// ����㷨��ʵ���๹�캯������������
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
//�ַ����̶��ָ�
CString * SplitString(CString str, char split, int& iSubStrs)
{
	int iPos = 0; //�ָ��λ��
	int iNums = 0; //�ָ��������
	CString strTemp = str;
	CString strRight;
	//�ȼ������ַ���������
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
	if (iNums == 0) //û���ҵ��ָ��
	{
		//���ַ����������ַ�������
		iSubStrs = 1;
		return NULL;
	}
	//���ַ�������
	iSubStrs = iNums + 1; //�Ӵ������� = �ָ������ + 1
	CString* pStrSplit;
	pStrSplit = new CString[iSubStrs];
	strTemp = str;
	CString strLeft;
	for (int i = 0; i < iNums; i++)
	{
		iPos = strTemp.Find(split);
		//���Ӵ�
		strLeft = strTemp.Left(iPos);
		//���Ӵ�
		strRight = strTemp.Mid(iPos + 1, strTemp.GetLength());
		strTemp = strRight;
		pStrSplit[i] = strLeft;
	}
	pStrSplit[iNums] = strTemp;
	return pStrSplit;
}
//��ʼ������
//******************************************************************************//
//����:��ʼ������
//����:
//	sAlgInitParam    ���룬ȫ��·��
//  nAlgTypID        ���룬�㷨ID
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CCheck::Init(const s_AlgInitParam &sAlgLayerInitParam, int nAlgTypID, s_TypeCheckParam* psCheckParams)
{
	try
	{
		m_sAlgInitParam = sAlgLayerInitParam;
		//������־��¼·��
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
		m_FileOperate.WriteAlgOperateLog(m_strLogPath, _T("������־"), _T("������־"), _T("���:��ʼ��"));

		//����
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
		m_sStatus.strErrorContext = _T("Check-Halcon��ʼ���쳣");

		CString strError;
		strError.Format(_T("Check-Halcon��ʼ���쳣- "));
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Init()�쳣");
		// д�쳣��־
		CString strErr;
		strErr.Format(_T("Check-Init()�쳣"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strErr);
	}
	return m_sStatus;
}


//��������
//******************************************************************************//
//����:��������
//����:
//	strTaskName    ���룬������
//  lstRegion      ���룬�������м����������
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CCheck::LoadTask(const CString strTaskName, const list<s_ModelRegionPro> lstRegion)
{
	//��������·��
	try
	{
		m_strTaskName = strTaskName;
		m_strTaskPath.Format(_T("%s\\%s\\AlgTask"), m_sAlgInitParam.strAlgModePath, strTaskName);
		m_strAlgName.Format(_T("StitchCheck"));

		//��ռ������
		Free();

		//���ɼ������,����ÿ��������г�ʼ��
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


		m_FileOperate.WriteAlgOperateLog(m_strLogPath, _T("������־"), _T("������־"), _T("���:��������"));
	}
	catch (HException &hException)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Halcon-LoadTask�쳣");

		CString strError;
		strError.Format(_T("Check-Halcon-LoadTask�쳣- "));
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-LoadTask()�쳣");
		// д�쳣��־
		CString strErr;
		strErr.Format(_T("Check-LoadTask()�쳣"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strErr);
	}
	return m_sStatus;
}

//��ȡ������
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
					if ((nParamProp == 0)/*&&(i== nNumParam-1)*/)//������
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
					if (nParamProp == 0)//������
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
		m_sStatus.strErrorContext = _T("Check-Halcon-GetAlgParam�쳣");

		CString strError;
		strError.Format(_T("Check-Halcon-GetAlgParam�쳣- "));
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-GetAlgParam()�쳣");
		// д�쳣��־
		CString strErr;
		strErr.Format(_T("Check-GetAlgParam()�쳣"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strErr);
	}
	return m_sStatus;
}
//��ȡ�������
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
					else if (iSubStrs == 1 && (strChildParamName.Find(' ') == -1))//ֻ��һ������
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
		m_sStatus.strErrorContext = _T("Check-Halcon-GetAlgParam-��ģ�����쳣");

		CString strError;
		strError.Format(_T("Check-Halcon-GetAlgParam-��ģ�����쳣- "));
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-GetAlgParam()-��ģ�����쳣");
		// д�쳣��־
		CString strErr;
		strErr.Format(_T("Check-GetAlgParam()-��ģ�����쳣"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strErr);
	}
	return m_sStatus;
}
//���ü�����
s_Status CCheck::SetAlgParam(const s_ErrorTpye sErrorTpye, const s_TypeCheckParam  sCheckParams)
{
	try
	{
		//����ÿ��������򣬲����ü�����
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
		m_sStatus.strErrorContext = _T("Check-Halcon-SetAlgParam�쳣");

		CString strError;
		strError.Format(_T("Check-Halcon-SetAlgParam�쳣- "));
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-SetAlgParam()�쳣");
		// д�쳣��־
		CString strErr;
		strErr.Format(_T("Check-SetAlgParam()�쳣"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strErr);
	}
	return m_sStatus;
}
//���ý������
s_Status CCheck::SetAlgParam(const s_ErrorTpye sErrorTpye, const s_TypeCheckParam  sCheckParams, const s_ModelRegionPro& sSrcRegion)
{
	try
	{
		//��ǰ������򣬲����ü�����
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
		m_sStatus.strErrorContext = _T("Check-Halcon-SetAlgParam-�����쳣");

		CString strError;
		strError.Format(_T("Check-Halcon-SetAlgParam-�����쳣- "));
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-SetAlgParam()-�����쳣");
		// д�쳣��־
		CString strErr;
		strErr.Format(_T("Check-SetAlgParam()-�����쳣"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strErr);
	}
	return m_sStatus;
}
//�Զ���ģ
s_Status CCheck::AutoCreateAlgModel(const s_AlgCheckInputParam& sAlgInputParam,
	const s_ModelRegionPro& sSrcRegion,
	CString strAlgModeName,
	s_TypeCheckParam  sCheckParams, BOOL bIsModelImg,
	s_Status &CreateOrLearnState)
{
	try
	{
		CInnerCheck      *pTempChecker;       // �ڲ�ָ�룬��ʱ������
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

		//�㷨������ֵ
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
		if (bIsModelImg)//����ģ��
		{
			pTempChecker->AutoCreateAlgMode(hModelImgSrc, &hShowReg, pTempChecker->m_htParamVaule, pTempChecker->m_htResolution, &pTempChecker->m_htParamVaule, CreateOrLearnState);
			//��ȡ�����Ƶ���ͼ  hShowRegNew ��ģ������ʾ
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

			//�㷨��������
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
		else//���ģ��
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
		m_sStatus.strErrorContext = _T("Check-Halcon-AutoCreateAlgModel�쳣");

		CString strError;
		strError.Format(_T("Check-Halcon-AutoCreateAlgModel�쳣- "));
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strError);

		CreateOrLearnState.nErrorType = 1;
		CreateOrLearnState.strErrorContext = m_sStatus.strErrorContext;
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-AutoCreateAlgModel()�쳣");
		// д�쳣��־
		CString strErr;
		strErr.Format(_T("Check-AutoCreateAlgModel()�쳣"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strErr);

		CreateOrLearnState.nErrorType = 1;
		CreateOrLearnState.strErrorContext = m_sStatus.strErrorContext;
	}
	return m_sStatus;
}

//�ͷſռ�
//******************************************************************************//
//����:�ͷſռ�
//����:NULL
//����:
//	�㷨ִ��״̬�ṹ��
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
		m_FileOperate.WriteAlgOperateLog(m_strLogPath, _T("������־"), _T("������־"), _T("CCheck->Free():����itInnerCheck->Free()"));
	}

	return m_sStatus;
}

//�����м���
//******************************************************************************//
//����:�����м���
//����:
//  s_MiddleResult �м����ṹ��
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CCheck::SetMiddleResult(const s_MiddleResult &sMiddleResult)
{
	//m_sMiddleResult.vsLocGroupResult = sMiddleResult.vsLocGroupResult;
	m_sMiddleResult = sMiddleResult;
	return m_sStatus;
}


//�������������
//******************************************************************************//
//����:�������������
//����:
//	sAlgSingleCheckInputParam      ���룬�������ṹ��
//  bParallel					   ���룬�Ƿ���
//  bModelTest					   ���룬�Ƿ�ģ�����- ��������
//	sAlgSingleCheckOutputResult    ��������ؼ��������ṹ��
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CCheck::Check(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
    s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
    try
    {
        std::list<s_InputTimeImage>::const_iterator itAlgCInParam, itAlgCInParamEnd;
        itAlgCInParam = sAlgCheckInputParam.lstInputImage.begin();
        itAlgCInParamEnd = sAlgCheckInputParam.lstInputImage.end();
        for (; itAlgCInParam != itAlgCInParamEnd; itAlgCInParam++)//���ͼ��
        {
            HObject hImgSrc;
            CopyObj(*(itAlgCInParam->pChannelHImg), &hImgSrc, 1, -1);
            HTuple htChannelNum;
            int nChannelNum;
            //CountChannels(hImgSrc, &htChannelNum);
            CountObj(hImgSrc, &htChannelNum);
            nChannelNum = htChannelNum[0].I();
            //�ж�ͼ�������Ƿ����Ҫ��
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
                    m_sStatus.strErrorContext = _T("ͼ����������Ϊ��");
                    m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("�����"), m_sStatus.strErrorContext);
                    return m_sStatus;
                }
            }
           else
            {
                m_sStatus.nErrorType = 1;
                m_sStatus.strErrorContext = _T("ͼ����������λ������");
                m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("�����"), m_sStatus.strErrorContext);
                return m_sStatus;
            }

            //���������м�⣬�������㷨����ʱ��
            double dTimeStart, dTimeCount;
            HTuple hv_TimeStart, hv_TimeCount;
            s_InnerCheckInputParam  sAlgInnerCheckInputParam;
            s_InnerSingleCheckOutputParam sInnerSingleCheckOutputResult;

            //�������
            sAlgInnerCheckInputParam.sInputTimeImage = *itAlgCInParam;
            sAlgCheckOutputResult->lst_vec_clear();

            std::list<CInnerCheck>::iterator itInnerCheck;
            for (itInnerCheck = m_listInnerCheck.begin(); itInnerCheck != m_listInnerCheck.end(); itInnerCheck++)//�������
            {
                //������ʱ�俪ʼ
                CountSeconds(&hv_TimeStart);
                dTimeStart = hv_TimeStart[0].D();
                sAlgInnerCheckInputParam.sRegionProperty = itInnerCheck->m_sDstRegionProperty;

                //��ö�λ����Ϣ
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
                //���������ʵ�ʼ��
                itInnerCheck->SingleCheck(sAlgInnerCheckInputParam, hImgSrc, &sInnerSingleCheckOutputResult);

                //������ʱ�����
                CountSeconds(&hv_TimeCount);
                dTimeCount = hv_TimeCount[0].D();
                dTimeCount -= dTimeStart;
                sInnerSingleCheckOutputResult.sSingleErrorInfo.nResvered[0] = dTimeCount * 1000;
                //��������
                sInnerSingleCheckOutputResult.sSingleErrorInfo.TrainResultFlag = (eTrainResultFlag)sInnerSingleCheckOutputResult.nFlagStudy;

                sAlgCheckOutputResult->nSingleErrorCount++;   //��Ч��Ϣ����
                sAlgCheckOutputResult->lstSingleErrorInfo.push_back(sInnerSingleCheckOutputResult.sSingleErrorInfo);
            }
        }

    }
    catch (HException &hException)
    {
        m_sStatus.nErrorType = 1;
        m_sStatus.strErrorContext = _T("Check-Halcon-Check�쳣");

        CString strError;
        strError.Format(_T("Check-Halcon-Check�쳣- "));
        strError += hException.ProcName().Text();
        strError += _T("�쳣");
        strError += hException.ErrorMessage().Text();
        m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strError);
    }
    catch (...)
    {
        m_sStatus.nErrorType = 1;
        m_sStatus.strErrorContext = _T("Check-Check()�쳣");
        // д�쳣��־
        CString strErr;
        strErr.Format(_T("Check-Check()�쳣"));
        m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strErr);
    }
    return m_sStatus;
}

//��λ�㷨�������ؼ�⺯��
//******************************************************************************//
//����:��λ�㷨�������ؼ�⺯��
//����:
//	sAlgCheckInputParam    ���룬�������ṹ��
//	sMiddleResult          ��������ض�λ�㷨�������
//  sAlgCheckOutputResult  ��������ؼ��������ṹ��
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CCheck::Check(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
	s_MiddleResult* sMiddleResult,
	s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
	return m_sStatus;
}
//���ϼ�⺯��
s_Status CCheck::JoinCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
	s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
	try
	{
	}
	catch (HException &hException)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Halcon-JoinCheck�쳣");

		CString strError;
		strError.Format(_T("Check-Halcon-JoinCheck�쳣- "));
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-JoinCheck()�쳣");
		// д�쳣��־
		CString strErr;
		strErr.Format(_T("Check-JoinCheck()�쳣"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strErr);
	}
	return m_sStatus;
}
//��������ϼ��
s_Status CCheck::MutiCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
	s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
	try
	{
	}
	catch (HException &hException)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Halcon-MutiCheck�쳣");

		CString strError;
		strError.Format(_T("Check-Halcon-MutiCheck�쳣- "));
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-MutiCheck()�쳣");
		// д�쳣��־
		CString strErr;
		strErr.Format(_T("Check-MutiCheck()�쳣"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strErr);
	}
	return m_sStatus;
}
//���¼���������
//******************************************************************************//
//����:���¼���������
//����:NULL
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CCheck::UpdateParam()
{
	//���¼�����
	try
	{
		std::list<CInnerCheck>::iterator itInnerCheck;
		for (itInnerCheck = m_listInnerCheck.begin(); itInnerCheck != m_listInnerCheck.end(); itInnerCheck++)
		{
			itInnerCheck->ReadTempPara();// ������Ҫ���µ����� [5/17/2011 Denghl]
		}
	}
	catch (HException &hException)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Halcon-UpdateParam�쳣");

		CString strError;
		strError.Format(_T("Check-Halcon-UpdateParam�쳣- "));
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-UpdateParam()�쳣");
		// д�쳣��־
		CString strErr;
		strErr.Format(_T("Check-UpdateParam()�쳣"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strErr);
	}
	return m_sStatus;
}

//������ģ��
//******************************************************************************//
//����:������ģ��
//����:NULL
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CCheck::SaveCheckModel()
{
	////ɾ��ԭ�е�����ģ�����
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
		//fs.fFlags = FOF_NOCONFIRMATION | FOF_NOERRORUI;   //FOF_ALLOWUNDO[ɾ�����������վ���˴�����]
		//fs.lpszProgressTitle = _T("delefile");
		//strDelete.Format(_T("%s\\%s"),m_strTaskPath,m_strAlgName);
		////strcpy_s(strDelete,strTmp);
		//fs.pFrom  =  strDelete;
		//SHFileOperation(&fs);

		////�����ڴ����ݣ������㷨��Ҫ������ģ�����
		//std::list<CInnerCheck>::iterator itInnerCheck;
		//for (itInnerCheck = m_listInnerCheck.begin();itInnerCheck!=m_listInnerCheck.end();itInnerCheck++)
		//{
		//	CString strRegionPath;
		//	strRegionPath.Format(_T(""));
		//	strRegionPath.Format(_T("%s\\%s\\%d"),m_strTaskPath,m_strAlgName,itInnerCheck->m_sDstRegionProperty.nROIIndex);

		//	m_FileOperate.CreateMultiLevelPath(strRegionPath);
		//	
		//	itInnerCheck->SaveCheckModel();//����ģ��
		//}
	}
	catch (HException &hException)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-Halcon-SaveCheckModel�쳣");

		CString strError;
		strError.Format(_T("Check-Halcon-SaveCheckModel�쳣- "));
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strError);
	}
	catch (...)
	{
		m_sStatus.nErrorType = 1;
		m_sStatus.strErrorContext = _T("Check-SaveCheckModel()�쳣");
		// д�쳣��־
		CString strErr;
		strErr.Format(_T("Check-SaveCheckModel()�쳣"));
		m_FileOperate.WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("Check()��"), strErr);
	}
	return m_sStatus;
}

/////////////////////////////////////////////////////////////////////////////
// �㷨��������ʵ����ʵ��
/////////////////////////////////////////////////////////////////////////////

// ����㷨��ʵ���๹�캯������������
//******************************************************************************//
CSetPROP::CSetPROP()
{

}
CSetPROP::~CSetPROP()
{

}

//�������ó�ʼ��
//******************************************************************************//
//����:�������ó�ʼ��
//����:
//	strTaskName      ���룬������
//	sAlgInitParam    ���룬ȫ��·��
//����:
//	�㷨ִ��״̬�ṹ��
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

//��������������
//******************************************************************************//
//����:��������������
//����:
//	sAlgImageInfo     ���룬ͼ��ṹ��   
//	sRegionProperty   ���룬��������
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CSetPROP::SetAlgParamDlg(const s_AlgCheckInputParam& sAlgInputParam, \
	const s_ModelRegionPro& sRegion, BOOL bAllowModify)
{
	CPropDlg Prodlg;
	Prodlg.m_pChecker = m_pChecker;      //���ָ��
	std::list<s_InputTimeImage>::const_iterator itAlgCInParam;
	itAlgCInParam = sAlgInputParam.lstInputImage.begin();

    CopyImage(*(itAlgCInParam->pChannelHImg), &(Prodlg.m_hImgSrcBig));
    /*HTuple channelsM,channelsP;

    CountChannels(*(itAlgCInParam->pChannelHImg), &channelsP);
    int p = channelsP.I();
    CountChannels(*(itAlgCInParam->pMulHImage), &channelsM);
    
    int m = channelsM.I();*/

	Prodlg.m_sAlgImageInfo = *itAlgCInParam;   //ͼ����Ϣ
	Prodlg.m_sDstRegionProperty = sRegion; //��������
	Prodlg.m_bAllowModify = bAllowModify;    //�Ƿ�ģ��ͼ
    Prodlg.m_regions = &m_regions;
	//���Ի���,������ز���
	if (Prodlg.DoModal() == IDOK)
	{
		;
	}
    
	return m_sStatus;
}

//��������
//******************************************************************************//
//����:��������
//����:
//	sAlgImageInfo        ���룬ͼ��ṹ��   
//	sDstRegionProperty   �����Ŀ����������
//  sSrcRegionProperty   ���룬Դ��������
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CSetPROP::AlgCopyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
	s_ModelRegionPro& sNewRegion, const s_ModelRegionPro& sSrcRegion)
{
	//[Ŀǰδ��]
	return m_sStatus;
}

//�޸������С
//******************************************************************************//
//����:�޸������С
//���ܣ��޸������С�󣬵��ú��������������û�����ģ��ȣ�
//����:
//	sAlgImageInfo        ���룬ͼ��ṹ��   
//	sSrcRegionProperty   ���룬��������
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CSetPROP::AlgModifyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
	const s_ModelRegionPro& sSrcRegion, const s_ModelRegionPro& sNewRegion)
{
	//���������,��ԭ����Ĳ����������޸ĺ��������
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

//ɾ������
//******************************************************************************//
//����:ɾ������
//����:
//	lstnRegionID     ���룬����ID����   

//����:
//	�㷨ִ��״̬�ṹ��
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
		//ɾ������ʱ����ɾ����Ӧ��������Ϣ�ļ��� 
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

//�õ��㷨�ڲ�����
//******************************************************************************//
//����:�õ��㷨�ڲ�����
//����:
//	lstInnerAlgType     ������㷨�ڲ�֧���㷨������Ϣ����
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CSetPROP::GetAlgorithmInfo(list<s_InnerAlgType>& lstInnerAlgType)
{
	s_Status sStatus;              //ִ��״̬
	lstInnerAlgType.clear();
	s_InnerAlgType sInnerAlgType;

	//����ڲ�����㷨
	sInnerAlgType.nInnerTypeID = 1;
	sInnerAlgType.bPopUpDlg = FALSE;
	sInnerAlgType.bSupportLearn = FALSE;
	//���á��ڲ��㷨���ơ�������ʵ����������趨
	sInnerAlgType.strTypeName.Format(_T("��ż��"));
	lstInnerAlgType.push_back(sInnerAlgType);

	return sStatus;
}
