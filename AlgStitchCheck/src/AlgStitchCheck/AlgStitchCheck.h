/************************************************************************
/* Copyright (c) 2015, �������ͼ���Ӿ����޹�˾ӡ����ҵ��    
/* All rights reserved.             
/*                  
/* �ļ����ƣ�  AlgStitchCheck.h
/* ��Ҫ˵����  ����㷨��ӿ�����㷨�������ýӿ��ࡣ
/*
/* �����ߣ�  �˺���
/* �汾��    V1.0
/* �޸��ˣ�  ������
/* �汾��    V2.0
/* �޸�ʱ�䣺2015-06-04 
/* �޸ļ�¼��1. ��ʽ����ܴ��룬��ǿ�ɶ���
      2. ��CreateDHAlgStitchCheck()�޸ĳ�CreateDHAlgCheck()
      3. ��CreateDHAlgStitchCheckPROPDLG()�޸ĳ�CreateDHAlgCheckPROPDLG()
	  ˵������2��3�ĵ����������޸Ĺ�����ʱ���ô������޸�
/* �޸��ˣ�  ����Ƽ
/* �汾��    V1.2.0.2
/* �޸�ʱ�䣺2017-06-29 
/* �޸ļ�¼��1.halcon13ȫ������  
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
#if !defined(AFX_ALGNAMECHECK_H__7DB5D3A0_1CF2_466A_86A3_524CF5BE22F3__INCLUDED_)
#define AFX_ALGNAMECHECK_H__7DB5D3A0_1CF2_466A_86A3_524CF5BE22F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"
#include "AlgVirtualBaseClass.h"

/////////////////////////////////////////////////////////////////////////////
// CAlgStitchCheckApp
#define DLL_EXT _declspec(dllexport) 

class CAlgStitchCheckApp : public CWinApp
{
public:
	CAlgStitchCheckApp();
// Overrides
//public:
	//virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
// ����㷨��ӿ���
class DLL_EXT CDHAlgStitchCheck : public CAlgVirtualCheck
{
public:
	
	//���캯������������
	CDHAlgStitchCheck();
	virtual ~CDHAlgStitchCheck();

	//��ʼ������
	s_Status Init(const s_AlgInitParam& sAlgLayerInitParam, int nAlgTypID, s_TypeCheckParam* psCheckParams);

	//�����������
	s_Status LoadTask(const CString strTaskName, const list<s_ModelRegionPro> lstRegion);

	//��ȡ������
	s_Status GetAlgParam(const s_ErrorTpye sErrorTpye,s_TypeCheckParam  * psCheckParams);

	//��ģ�����ȡ����
	 s_Status GetAlgParam(const s_ErrorTpye sErrorTpye, s_TypeCheckParam  * psCheckParams, const s_ModelRegionPro& sSrcRegion, const BOOL bAllowModify);

	//���ü�����
	s_Status SetAlgParam(const s_ErrorTpye sErrorTpye,const s_TypeCheckParam  sCheckParams);

	//���ý�ģ�������
	 s_Status SetAlgParam(const s_ErrorTpye sErrorTpye, const s_TypeCheckParam  sCheckParams, const s_ModelRegionPro& sSrcRegion);

	//�Զ���ģ
	s_Status AutoCreateAlgModel(const s_AlgCheckInputParam& sAlgInputParam,
		const s_ModelRegionPro& sSrcRegion,
		CString strAlgModeName,
		s_TypeCheckParam  sCheckParams,
		BOOL bIsModelImg,
		s_Status &CreateOrLearnState);

	//�ͷź���
	s_Status Free();

	//������Ҫ���м������������еĶ�λ�����,ÿ�μ�⺯��֮ǰ����
	s_Status SetMiddleResult(const s_MiddleResult& sMiddleResult);

	//������ⴿ�麯��
	s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult) ;

	//������λģ���ⴿ�麯��
	s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_MiddleResult* sMiddleResult,
		s_AlgCheckOutputParam *sAlgCheckOutputResult) ;

	//Join��ⴿ�麯��
	s_Status JoinCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult) ;

	//MutiCam��ⴿ�麯��
	s_Status MutiCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult) ;

	//���¼�⺯��
	s_Status UpdateParam();

	//������ģ�庯��
	s_Status SaveCheckModel();
public:
	CAlgVirtualCheck *m_pAlgStitchCheck;
	s_Status m_sStatus;

};

//////////////////////////////////////////////////////////////////////////
// �㷨�������ýӿ���
class DLL_EXT CDHAlgStitchCheckPROPDLG : public CAlgVirtualPROPDLG
{
public:
	
	// �����ָ�룬���ڸ��¶Ի��������õ�����
	CDHAlgStitchCheck *m_pChecker;

	//���캯������������
	CDHAlgStitchCheckPROPDLG();
	virtual ~CDHAlgStitchCheckPROPDLG();

	//��ʼ�����麯��
	s_Status Init(CString strTaskName, const s_AlgInitParam& sAlgInitParam) ;

	//���������㷨���ԶԻ����麯��;
	s_Status SetAlgParamDlg(const s_AlgCheckInputParam& sAlgInputParam, \
		const s_ModelRegionPro& sRegion, BOOL bAllowModify) ;

	//���������麯��
	s_Status AlgCopyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
		s_ModelRegionPro& sNewRegion, const s_ModelRegionPro& sSrcRegion) ;

	//�޸������麯��
	s_Status AlgModifyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
		const s_ModelRegionPro& sSrcRegion, const s_ModelRegionPro& sNewRegion) ;

	//ɾ�������麯��
	s_Status AlgDeleteRegion(const list<s_ModelRegionPro>& lstRegion) ;

	//�õ��㷨���ʹ��麯��
	s_Status GetAlgorithmInfo(list<s_InnerAlgType>& lstInnerAlgType) ;

protected:
	CAlgVirtualPROPDLG *m_pPROPDLG;
	s_Status m_sStatus;

};

//////////////////////////////////////////////////////////////////////////
extern "C" __declspec(dllexport) CDHAlgStitchCheck* CreateDHAlgCheck();
extern "C" __declspec(dllexport) CDHAlgStitchCheckPROPDLG* CreateDHAlgCheckPROPDLG();
extern "C" __declspec(dllexport) BOOL FreeAlgCheckDll(CDHAlgStitchCheck *pCheck,
													  CDHAlgStitchCheckPROPDLG *pPropDlg);
/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_ALGNAMECHECK_H__7DB5D3A0_1CF2_466A_86A3_524CF5BE22F3__INCLUDED_)
