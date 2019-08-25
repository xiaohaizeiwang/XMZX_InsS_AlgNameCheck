/************************************************************************
/* Copyright (c) 2015, �������ͼ���Ӿ����޹�˾ӡ����ҵ��  
/* All rights reserved.             
/*                  
/* �ļ����ƣ�  InnerCheck.h
/* ��Ҫ˵����  �ڲ������
/*
/* �����ߣ�  �˺���
/* �汾��    V1.0
/* �޸��ˣ�  ������
/* �汾��    V2.0
/* �޸�ʱ�䣺2015-06-04 
/* �޸ļ�¼��1. ��ʽ����ܴ��룬��ǿ�ɶ���
     2. ���halcon�쳣�׳�����
	 3. �޸�GenCheckImage()����������βα���Hobject& hCheckImage��
	    ��ߺ����Ŀɶ��ԡ�
/* �޸��ˣ�  ����Ƽ
/* �汾��    V1.2.0.2
/* �޸�ʱ�䣺2017-06-29 
/* �޸ļ�¼���޸�halcon13������� 

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

#if !defined(AFX_CINNERCHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_)
#define AFX_CINNERCHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FileOperate.h"
#include "AlgControlInterface.h"
#include "AlgVirtualBaseClass.h"
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

#define    NORMAL         0       
#define    EMPTY          1         
#define    WRONG          2         
#define    ABNORMAL       3 
#define    LOCATE       3 
//�ڲ����������������
typedef struct 
{
	s_ModelRegionPro   sRegionProperty;   //��������
	s_LocGroupResult   sLocGroupResult;   //��λ����
	s_InputTimeImage   sInputTimeImage;	  //ͼ����Ϣ
	//CString strExpCode;	                  //Ԥ�ں���
}s_InnerCheckInputParam;

//�ڲ�������-�������
typedef struct 
{
	s_sSingleErrorInfo sSingleErrorInfo;   //�������Ϣ
	UINT         nFlagStudy;   //ѧϰ�ɹ�0��ѧϰʧ��1
}s_InnerSingleCheckOutputParam;

//�ڲ�������-���ϼ��
typedef struct
{
	s_UnionErrorInfo sUnionErrorInfo;   //�������Ϣ
	UINT         nFlagStudy;   //ѧϰ�ɹ�0��ѧϰʧ��1
}s_InnerUnionCheckOutputParam;

//halcon�쳣�׳�����
void inline MyHalconExceptionHandler(const HException& hException)
{
	throw hException;
}
//////////////////////////////////////////////////////////////////////////
// �ڲ�����㷨ʵ����
class CInnerCheck 
{
public:
	CInnerCheck();
	virtual ~CInnerCheck();

	CString  m_strTaskPath;                //����·��
	CString  m_strParaIni;                 //���������ļ�·��
	CString  m_strDefaultParaIni;          //Ĭ�ϲ��������ļ�·��
	CString  m_strRegionPath;              //����·��
	CString  m_strAlgName;                 //�㷨��
	CString  m_strAlgPublicLib;            //�㷨���ÿ�·��
	CString  m_strDefaultParaPath;         //�㷨Ĭ�ϲ���·��
	CString  m_strLogPath;                 //��־��¼·��
	s_AlgInitParam   m_sAlgInitParam;      //ȫ���㷨·��
    s_ModelRegionPro m_sDstRegionProperty; //������Ϣ
	
	CFileOperate *m_FileOperate;              //�ļ�����
	BOOL m_bAllowModify; //�Ƿ�Ϊģ��ͼ��
	int m_nChannel;     //ͼ����ɫͨ��

	BOOL m_bFileExist; //�ļ��Ƿ����

	HObject m_hModelImg;//����Сģ��
    HObject m_hModelReg;

    HObject m_hShowReg;
    HObject m_hErrorReg;

    HTuple m_hMathId;//ģ��ƥ��Id
    HObject m_RegionDetection;//ʵ�ʼ������
    std::list<s_ModelRegionPro> *m_regions;//�洢��Ҫ��������

    HTuple m_htHalconHandle;
    //����
	HTuple m_htResolution;

	HTuple m_htParamProp;
	HTuple m_htParamName;
	HTuple m_htParamVaule;
	HTuple m_htParamMeaning;
	
	//�ⲿ��ʾ��
	HTuple m_htParamType;
	HTuple m_htParamMinVaule;
	HTuple m_htParamMaxVaule;
	HTuple m_htParamStep; 
	HTuple m_htParamChildNames;

	//����
	HTuple m_htParam;

public:
	
	//��ʼ������
	s_Status Init(const CString strTaskName, const s_AlgInitParam &sAlgInitParam,\
		const s_ModelRegionPro &sDstRegionProperty,BOOL bIsNewRegion);
	
	// �ͷź���
	s_Status Free();

    
    // ��⺯��
	s_Status SingleCheck(const s_InnerCheckInputParam& sAlgInnerCheckInputParam, const HObject hImgSrc,\
		s_InnerSingleCheckOutputParam *sAlgInnerCheckOutputResult);
	
	//������ģ�庯��
	s_Status SaveCheckModel();

	//��������
	s_Status AlgCopyRegion(const s_InputTimeImage& sAlgImageInfo,
		s_ModelRegionPro &sDstRegionProperty,const s_ModelRegionPro &sSrcRegionProperty);

	//�������ڼ���HALCONͼ��
	s_Status GenCheckImage(const s_InnerCheckInputParam& sAlgInnerCheckInputParam, const HObject hImgSrc, HObject& hCheckImage);

    //����ģ��ƥ��ģ��
    s_Status GenMatchTem(HObject &hv_ImgSrcBig, HObject& hv_ShowReg);

public:
	
	// д�����ļ�
	void WritePara();

	// �������ļ�
	void ReadPara();

	// �����Ѿ��޸��������ʱ�ļ� 
	int SaveTempPara();

	// ��ȡ�Ѿ��޸��������ʱ�ļ� 	
	int ReadTempPara();
    
	// �����ڴ�ռ�
	void RequistSpace();
	
	// �ͷ��ڴ�ռ�
	void FreeTMPSpace();

	//��ģ�������ļ�
	void ReadModelRegion(CString strRegionPath,const s_ModelRegionPro &sDstRegionProperty);

    void getDetectionRegion();
	
	// Short Description: �Զ���ģ 
	int AutoCreateAlgMode(HObject ho_hModelImg, HObject *ho_hShowReg, const HTuple hv_htParamVaule,
		const HTuple hv_htResolution, HTuple *hv_htParamVauleNew, s_Status &CreateOrLearnState);
	
	// Short Description: �Զ�ѧϰ
	int AutoLearnAlgMode( HObject ho_hCheckImg, s_Status &CreateOrLearnState);
};
#endif // !defined(AFX_CINNERCHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_)


