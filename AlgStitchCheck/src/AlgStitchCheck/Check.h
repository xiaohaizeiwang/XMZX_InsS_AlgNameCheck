/************************************************************************
/* Copyright (c) 2015, �������ͼ���Ӿ����޹�˾ӡ����ҵ��    
/* All rights reserved.             
/*                  
/* �ļ����ƣ�  Check.h
/* ��Ҫ˵����  ����㷨��ʵ������㷨��������ʵ���ࡣ
/*
/* �����ߣ�  �˺���
/* �汾��    V1.0
/* �޸��ˣ�  �����޹����Ұ��㡣
/* �޸ļ�¼��1. ��ʽ����ܴ��룬��ǿ�ɶ���
              2. ����ڲ��������list<CInnerCheck> m_listInnerCheck
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

#if !defined(AFX_CHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_)
#define AFX_CHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InnerCheck.h"
#include "FileOperate.h"
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

//////////////////////////////////////////////////////////////////////////
// ����㷨��ʵ����
class CCheck : public CAlgVirtualCheck 
{
public:
	CCheck();
	virtual ~CCheck();

	s_Status m_sStatus;             // ִ��״̬
	CString  m_strTaskPath;         // ����ȫ·��
	CString  m_strTaskName;         // ������
	CString  m_strDefaultParaIni;   // ȫ��Ĭ�ϲ���·��
	CString  m_strAlgName;          // �㷨��
    s_MiddleResult m_sMiddleResult; // �м���
	s_AlgInitParam m_sAlgInitParam; // ȫ���㷨·��

	std::list<CInnerCheck> m_listInnerCheck; // ���ɵļ������
	CFileOperate           m_FileOperate;    //�ļ�����
	CString                m_strLogPath;     //��־·��
	
	//int m_nRegionNum;     //�������
	//int m_nStudyRegNum;   //ÿ����ѧϰ�ɹ�������
	//int m_nAlgTypID;      //�㷨ID
	//s_AlgImageInfo m_sAlgImageInfo;//�㷨�����Ҫ��ͼ����Ϣ

	HTuple m_htParamProp;   //�������� 1-���Բ�����0-������
	HTuple m_htParamName;   //��������
	HTuple m_htParamVaule;  //������С
	HTuple m_htParamMeaning;//��������

	//��ʾ��
	HTuple m_htParamType;     //�������� 0-bool 1-int 2-double
	HTuple m_htParamMinVaule; //������Сֵ
	HTuple m_htParamMaxVaule; //�������ֵ
	HTuple m_htParamStep;     //��������
	HTuple m_htParamChildNames;//���Ӳ�����

public:
	//��ʼ������
	s_Status Init(const s_AlgInitParam& sAlgLayerInitParam, int nAlgTypID, s_TypeCheckParam* psCheckParams);

	//����������
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
	s_Status Check(const s_AlgCheckInputParam& sAlgSingleCheckInputParam, BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult);

	//������λģ���ⴿ�麯��
	s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_MiddleResult* sMiddleResult,
		s_AlgCheckOutputParam *sAlgCheckOutputResult);

	//Join��ⴿ�麯��
	s_Status JoinCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult);

	//MutiCam��ⴿ�麯��
	s_Status MutiCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult);
	//���¼���������
	s_Status UpdateParam();

	//������ģ�庯��
	s_Status SaveCheckModel();
};

//////////////////////////////////////////////////////////////////////////
// �㷨��������ʵ����
class CSetPROP :public CAlgVirtualPROPDLG
{
public:
	CSetPROP();
	virtual ~CSetPROP();
	s_Status m_sStatus;              //ִ��״̬
	CString  m_strTaskName;          //������
	CString  m_strAlgName;           //�㷨��
	CFileOperate    m_FileOperate;   //�ļ�����
	s_AlgInitParam  m_sAlgInitParam; //ȫ���㷨·��
	CCheck          *m_pChecker;     //��������ָ��
    std::list<s_ModelRegionPro> m_regions;//�洢��Ҫ��������

public:
	//��ʼ��
	s_Status Init(CString strTaskName, const s_AlgInitParam& sAlgInitParam);

	//���������㷨���ԶԻ����麯��;
	s_Status SetAlgParamDlg(const s_AlgCheckInputParam& sAlgInputParam, \
		const s_ModelRegionPro& sRegion, BOOL bAllowModify);

	//��������
	s_Status AlgCopyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
		s_ModelRegionPro& sNewRegion, const s_ModelRegionPro& sSrcRegion);

	//�޸�����
	s_Status AlgModifyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
		const s_ModelRegionPro& sSrcRegion, const s_ModelRegionPro& sNewRegion);

	//ɾ������
	s_Status AlgDeleteRegion(const list<s_ModelRegionPro>& lstRegion);

	//�õ��㷨����
	s_Status GetAlgorithmInfo(list<s_InnerAlgType>& lstInnerAlgType);
};
#endif // !defined(AFX_CHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_)


