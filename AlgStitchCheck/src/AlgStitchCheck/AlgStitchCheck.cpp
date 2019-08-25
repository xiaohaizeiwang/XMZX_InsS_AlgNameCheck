/************************************************************************
/* Copyright (c) 2015, �������ͼ���Ӿ����޹�˾ӡ����ҵ��    
/* All rights reserved.             
/*                  
/* �ļ����ƣ�  AlgStitchCheck.cpp
/* ��Ҫ˵����  ����㷨��ӿ�����㷨�������ýӿ��ࡣ
/*
/* �����ߣ�  �˺���
/* �汾��    V1.0
/* �޸��ˣ�  ������
/* �汾��    V2.0
/* �޸�ʱ�䣺2015-06-04 
/* �޸ļ�¼��1. ��ʽ����ܴ��룬��ǿ�ɶ���
              2. ������ÿ��������ע��˵��
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//	Note!
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//
/////////////////////////////////////////////////////////////////////////////
// CAlgStitchCheckApp
BEGIN_MESSAGE_MAP(CAlgStitchCheckApp, CWinApp)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CAlgStitchCheckApp construction
CAlgStitchCheckApp::CAlgStitchCheckApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}
CAlgStitchCheckApp theApp;

/////////////////////////////////////////////////////////////////////////////
// ����㷨��ӿ���ʵ��
/////////////////////////////////////////////////////////////////////////////

// ����㷨��ӿ��๹�캯������������
//******************************************************************************//
CDHAlgStitchCheck::CDHAlgStitchCheck()
{
	m_pAlgStitchCheck = new CCheck;
}
CDHAlgStitchCheck::~CDHAlgStitchCheck()
{
	if (m_pAlgStitchCheck != NULL)
	{
		delete m_pAlgStitchCheck;
		m_pAlgStitchCheck = NULL;
	}
}

// ������ʼ��
//******************************************************************************//
//����:������ʼ��
//����:
//	sAlgInitParam    ���룬ȫ��·��
//  nAlgTypID        ���룬�㷨ID
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgStitchCheck::Init(const s_AlgInitParam &sAlgLayerInitParam,int nAlgTypID, s_TypeCheckParam* psCheckParams)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_sStatus = m_pAlgStitchCheck->Init(sAlgLayerInitParam,nAlgTypID,psCheckParams);
	
	return m_sStatus;
}

// �ͷſռ� 
//******************************************************************************//
//����:�ͷſռ�
//����: NULL
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgStitchCheck::Free()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	
	m_sStatus = m_pAlgStitchCheck->Free();	
	
	return m_sStatus;
}

//��������
//******************************************************************************//
//����:��������
//����:
//	strTaskName      ���룬������
//	lstRegion        ���룬��������ṹ������
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgStitchCheck::LoadTask(const CString strTaskName,const list<s_ModelRegionPro> lstRegion)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	
	m_sStatus = m_pAlgStitchCheck->LoadTask(strTaskName,lstRegion);	
	
	////����
	/*s_ErrorTpye sErrorTpye;
	s_TypeCheckParam psCheckParams;*/

	/*m_sStatus = m_pAlgStitchCheck->GetAlgParam(sErrorTpye, &psCheckParams);
	m_sStatus = m_pAlgStitchCheck->SetAlgParam(sErrorTpye, psCheckParams);*/

	/*s_ModelRegionPro sSrcRegion;
	sSrcRegion.nROIIndex = 1;
	BOOL bAllowModify;
	bAllowModify = TRUE;
	GetAlgParam(sErrorTpye,&psCheckParams, sSrcRegion, bAllowModify);
	SetAlgParam(sErrorTpye,psCheckParams, sSrcRegion);*/

	return m_sStatus;
}

//��ȡ������
s_Status CDHAlgStitchCheck::GetAlgParam(const s_ErrorTpye sErrorTpye,s_TypeCheckParam  * psCheckParams)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	

	m_sStatus = m_pAlgStitchCheck->GetAlgParam(sErrorTpye,psCheckParams);	

	return m_sStatus;
}

//��ȡ�������
s_Status CDHAlgStitchCheck::GetAlgParam(const s_ErrorTpye sErrorTpye, s_TypeCheckParam  * psCheckParams, const s_ModelRegionPro& sSrcRegion, const BOOL bAllowModify)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_sStatus = m_pAlgStitchCheck->GetAlgParam(sErrorTpye, psCheckParams, sSrcRegion, bAllowModify);

	return m_sStatus;
}

//���ü�����
s_Status CDHAlgStitchCheck::SetAlgParam(const s_ErrorTpye sErrorTpye,const s_TypeCheckParam  sCheckParams)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	

	m_sStatus = m_pAlgStitchCheck->SetAlgParam(sErrorTpye,sCheckParams);	

	return m_sStatus;
}

//���ý������
s_Status CDHAlgStitchCheck::SetAlgParam(const s_ErrorTpye sErrorTpye, const s_TypeCheckParam  sCheckParams, const s_ModelRegionPro& sSrcRegion)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_sStatus = m_pAlgStitchCheck->SetAlgParam(sErrorTpye, sCheckParams, sSrcRegion);

	return m_sStatus;
}
//�Զ���ģ
s_Status CDHAlgStitchCheck::AutoCreateAlgModel(const s_AlgCheckInputParam& sAlgInputParam, const s_ModelRegionPro& sSrcRegion,
	CString strAlgModeName, s_TypeCheckParam  sCheckParams, BOOL bIsModelImg, s_Status &CreateOrLearnState)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_sStatus = m_pAlgStitchCheck->AutoCreateAlgModel(sAlgInputParam, sSrcRegion,strAlgModeName, sCheckParams, bIsModelImg, CreateOrLearnState);

	return m_sStatus;

}
////������Ҫ���м������������еĶ�λ�����,ÿ�μ�⺯��֮ǰ����
//******************************************************************************//
//����:������Ҫ���м�����
//����:
//	sMiddleResult    ���룬�м����ṹ��
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgStitchCheck::SetMiddleResult(const s_MiddleResult &sMiddleResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	
	m_sStatus = m_pAlgStitchCheck->SetMiddleResult(sMiddleResult);	
		
	return m_sStatus;
}
// ��⺯��������
//******************************************************************************//
//����:���������
//����:
//	sAlgCheckInputParam      ���룬�������ṹ��
//	sAlgCheckOutputResult    ��������ؼ��������ṹ��
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgStitchCheck::Check(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParallel,
	s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	try
	{
		m_sStatus = m_pAlgStitchCheck->Check(sAlgCheckInputParam, bParallel, sAlgCheckOutputResult);
	}
	catch (...)
	{
		;// д�쳣��־��������Ҫ��Ӵ���		
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
s_Status CDHAlgStitchCheck::Check(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParallel,
	s_MiddleResult *sMiddleResult,
	s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	try
	{
		m_sStatus = m_pAlgStitchCheck->Check(sAlgCheckInputParam, bParallel, sMiddleResult, sAlgCheckOutputResult);
	}
	catch (...)
	{
		;// д�쳣��־��������Ҫ��Ӵ���		
	}

	return m_sStatus;
}
//Join��ⴿ�麯��
s_Status CDHAlgStitchCheck::JoinCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParallel,
	s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	try
	{
		m_sStatus = m_pAlgStitchCheck->JoinCheck(sAlgCheckInputParam, bParallel, sAlgCheckOutputResult);
	}
	catch (...)
	{
		;// д�쳣��־��������Ҫ��Ӵ���		
	}

	return m_sStatus;
}

//MutiCam��ⴿ�麯��
s_Status CDHAlgStitchCheck::MutiCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParallel,
	s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	try
	{
		m_sStatus = m_pAlgStitchCheck->MutiCheck(sAlgCheckInputParam, bParallel,  sAlgCheckOutputResult);
	}
	catch (...)
	{
		;// д�쳣��־��������Ҫ��Ӵ���		
	}

	return m_sStatus;
}
//���¼�����
//******************************************************************************//
//����:���¼�����
//����:NULL
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgStitchCheck::UpdateParam()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	
	m_sStatus = m_pAlgStitchCheck->UpdateParam();	
	
	return m_sStatus;
}

//������ģ��
//******************************************************************************//
//����:������ģ��
//����:NULL
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgStitchCheck::SaveCheckModel()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	
	m_sStatus = m_pAlgStitchCheck->SaveCheckModel();	
	
	return m_sStatus;
}

/////////////////////////////////////////////////////////////////////////////
// �㷨�������ýӿ���ʵ��
CDHAlgStitchCheckPROPDLG::CDHAlgStitchCheckPROPDLG()
{
	m_pPROPDLG = new CSetPROP;
}
CDHAlgStitchCheckPROPDLG::~CDHAlgStitchCheckPROPDLG()
{
	if (m_pPROPDLG != NULL)
	{
		/*CSetPROP *pPROPDLG;
		pPROPDLG = (CSetPROP*)m_pPROPDLG;*/

		delete m_pPROPDLG;
		//pPROPDLG = NULL;
		m_pPROPDLG = NULL;
	}
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
s_Status CDHAlgStitchCheckPROPDLG::Init(const CString strTaskName,const s_AlgInitParam& sAlgLayerInitParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_pPROPDLG->pChecker = pChecker;
	m_sStatus = m_pPROPDLG->Init(strTaskName, sAlgLayerInitParam);	
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
s_Status CDHAlgStitchCheckPROPDLG::SetAlgParamDlg(const s_AlgCheckInputParam& sAlgInputParam, \
	const s_ModelRegionPro& sRegion, BOOL bAllowModify)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_sStatus = m_pPROPDLG->SetAlgParamDlg(sAlgInputParam,sRegion,bAllowModify);	
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
s_Status CDHAlgStitchCheckPROPDLG::AlgCopyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
	s_ModelRegionPro& sNewRegion, const s_ModelRegionPro& sSrcRegion)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	
	m_sStatus = m_pPROPDLG->AlgCopyRegion(sAlgInputParam,sNewRegion,sSrcRegion);
	
	return m_sStatus;
}

//�޸������С
//******************************************************************************//
//����:�޸������С
//���ܣ��޸������С�󣬵��ú��������������û�����ģ��ȣ�
//����:
//	sAlgImageInfo     ���룬ͼ��ṹ��   
//	hNewRegion        ���룬��������
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgStitchCheckPROPDLG::AlgModifyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
	const s_ModelRegionPro& sSrcRegion, const s_ModelRegionPro& sNewRegion)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_sStatus = m_pPROPDLG->AlgModifyRegion(sAlgInputParam,sSrcRegion,sNewRegion);
	
	return m_sStatus;
}

//ɾ������
//******************************************************************************//
//����:ɾ������
//����:
//	lstRegion     ���룬������������   
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CDHAlgStitchCheckPROPDLG::AlgDeleteRegion(const list<s_ModelRegionPro>& lstRegion)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_sStatus = m_pPROPDLG->AlgDeleteRegion(lstRegion);
	
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
s_Status CDHAlgStitchCheckPROPDLG::GetAlgorithmInfo(list<s_InnerAlgType>& lstInnerAlgType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pPROPDLG->GetAlgorithmInfo(lstInnerAlgType);

	return m_sStatus;
}

//��̬���ü����
extern "C" __declspec(dllexport) CDHAlgStitchCheck* CreateDHAlgCheck()
{
	CDHAlgStitchCheck *A;
	A  = new CDHAlgStitchCheck;
	return A;
}

//��̬����������
extern "C" __declspec(dllexport) CDHAlgStitchCheckPROPDLG* CreateDHAlgCheckPROPDLG()
{
	CDHAlgStitchCheckPROPDLG *A;
	A  = new CDHAlgStitchCheckPROPDLG;
	return A;
}

//�ͷż�����������
extern "C" __declspec(dllexport) BOOL FreeAlgCheckDll(CDHAlgStitchCheck *pCheck,
													  CDHAlgStitchCheckPROPDLG *pPropDlg)
{
	try
	{
		if (pCheck)
		{
			delete pCheck;
			pCheck = NULL;
		}
		if (pPropDlg)
		{
			delete pPropDlg;
			pPropDlg = NULL;
		}
	}
	catch (...)
	{
		return TRUE;
	}
	return FALSE;
}
