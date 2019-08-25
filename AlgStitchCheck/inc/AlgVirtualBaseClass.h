//AlgVirtualBaseClass.h   �㷨�����������
/************************************************************************/
/* Copyright (c) 2015, �������ͼ���Ӿ����޹�˾ӡ����ҵ��  
/* All rights reserved.													
/*																		
/* �ļ����ƣ� AlgVirtualBaseClass.h												
/* ժҪ���㷨�����������
/*
/* ���ߣ�    �˺���
/* �汾��    V1.0
/* �޸��ˣ�  ������
/* �汾��    V2.0
/* �޸�ʱ�䣺2015-06-04 
/* �޸ļ�¼��1. ��ʽ����ܴ��룬��ǿ�ɶ��� 
              2. �����м����ṹ��_MIDDLE_RESULT

/* �޸��ˣ�  �˺������Ų�
/* �汾��    V3.0
/* �޸�ʱ�䣺2018-11-07
/* �޸ļ�¼��1.��������ṹ�塢������ṹ��
			 2.���ӽ�ģ�����Զ���ģ����ȡ�����ò����ṹ��
/************************************************************************/
#if !defined(_AlgVirtualBaseClass)
#define _AlgVirtualBaseClass
#include "AlgControlInterface.h"
#include "afxmt.h"   // [2018/09/10 WZS]
//���㷨�����㷨��Ϣ�ṹ��
typedef struct _INNER_ALG_TYPE
{
	UINT nInnerTypeID;					//�㷨����ID(���㷨��Ψһ��ʶ)
	BOOL bSupportLearn;					//�Ƿ�֧��ѧϰ TRUE��֧�� FASLE����֧�� 
	BOOL bPopUpDlg;						//ѧϰ�Ƿ񵯳��Ի��� TRUE������ FASLE��������
	CString strTypeName;	//�㷨��������

	// ��ӳ�ʼ���͵Ⱥ����������
	_INNER_ALG_TYPE()
	{
		nInnerTypeID  = 0;
		bSupportLearn = FALSE;
		bPopUpDlg     = FALSE;
		strTypeName.Format(_T(""));		
	}
	~_INNER_ALG_TYPE()
	{
		nInnerTypeID = 0;
		bSupportLearn = FALSE;
		bPopUpDlg = FALSE;
		strTypeName.Format(_T(""));
	}
	_INNER_ALG_TYPE(const _INNER_ALG_TYPE& sIAT)
	{
		*this = sIAT;
	}
	_INNER_ALG_TYPE& operator=(const _INNER_ALG_TYPE& sIAType)
	{
		nInnerTypeID  = sIAType.nInnerTypeID;
		bSupportLearn = sIAType.bSupportLearn;
		bPopUpDlg     = sIAType.bPopUpDlg;

		strTypeName.Format(_T("%s"), sIAType.strTypeName);

		return (*this);
	}
}s_InnerAlgType;

//���㷨��Ϣ�ṹ��
typedef struct _ALG_TYPE
{
	UINT nAlgTypeID;                             //���㷨����IDΨһ��ʶ
	UINT nCheckType;                             //�㷨���ϼ�����ͣ�0ΪSingle��1ΪJoin��2ΪMuti
	CString strTypeName;             //���㷨��������
	CString strDllName;             //���㷨��������
	std::list< s_InnerAlgType > lstInnerAlgType; //���㷨��Ϣ����

	// ��ӳ�ʼ���͵Ⱥ����������
	_ALG_TYPE()
	{
		nAlgTypeID = 0;
		lstInnerAlgType.clear();
		strTypeName.Format(_T(""));
		strDllName.Format(_T(""));
	}
	~_ALG_TYPE()
	{
		lstInnerAlgType.clear();
	}
	_ALG_TYPE(const _ALG_TYPE& sAT)
	{
		*this = sAT;
	}
	_ALG_TYPE& operator=(const _ALG_TYPE& sAType)
	{
		nAlgTypeID = sAType.nAlgTypeID;
		nCheckType = sAType.nCheckType;
		lstInnerAlgType.assign(sAType.lstInnerAlgType.begin(), sAType.lstInnerAlgType.end());
		strTypeName.Format(_T("%s"),sAType.strTypeName);
		strDllName.Format(_T("%s"), sAType.strDllName);
		
		return (*this);
	}
	bool operator<(const _ALG_TYPE& sType)const
	{
		return (nCheckType < sType.nCheckType);
	}
	bool operator>(const _ALG_TYPE& sType)const
	{
		return (nCheckType > sType.nCheckType);
	}
	void lst_vec_clear()
	{
		if (lstInnerAlgType.size() > 0)
		{
			lstInnerAlgType.clear();
		}
	}
}s_AlgType;

//��λ�����ṹ��
typedef  struct _LOC_GROUP_RESULT
{
	UINT   nLocGroupID;      //��λ���
	float  fLineShiftLoc;    //����ƫ��λ��
	float  fColShiftLoc;     //����ƫ��λ��
	float  fRoundAngle;      //��ת�Ƕȣ�������
	HTuple htHomMat;         //�������
	HTuple htHomMatInv;      //���������
	int    nLocCount;        //�ö�λ������Ķ�λ����
	BOOL   bHaveError;       //True����ʾ��λ���д�

	// ��ӳ�ʼ���͵Ⱥ���������� [5/17/2011 ChenLiang]
	_LOC_GROUP_RESULT()
	{
		nLocGroupID   = 0;
		fLineShiftLoc = 0.0;
		fColShiftLoc  = 0.0;
		fRoundAngle   = 0.0;
		nLocCount     = 0;
		bHaveError    = FALSE;
	
		htHomMat[0] = HTuple(1);
		htHomMat[1] = HTuple(0);
		htHomMat[2] = HTuple(0);
		htHomMat[3] = HTuple(0);
		htHomMat[4] = HTuple(1);
		htHomMat[5] = HTuple(0);
		HomMat2dInvert(htHomMat, &htHomMatInv);
		
	}
	_LOC_GROUP_RESULT(const _LOC_GROUP_RESULT& sLGR)
	{
		*this = sLGR;
	}
	_LOC_GROUP_RESULT& operator=(const _LOC_GROUP_RESULT& sLocGroReuslt)
	{
		nLocGroupID   = sLocGroReuslt.nLocGroupID;
		fLineShiftLoc = sLocGroReuslt.fLineShiftLoc;
		fColShiftLoc  = sLocGroReuslt.fColShiftLoc;
		fRoundAngle   = sLocGroReuslt.fRoundAngle;
		nLocCount     = sLocGroReuslt.nLocCount;
		bHaveError    = sLocGroReuslt.bHaveError;

		htHomMat      = sLocGroReuslt.htHomMat;
		htHomMatInv   = sLocGroReuslt.htHomMatInv;

		return (*this);
	}
}s_LocGroupResult;

////�������Խṹ��
//typedef struct _REGION_PROPERTY
//{
//	UINT    nROIIndex;                   //�������
//	UINT    nAlgorithmType;              //���㷨���
//	UINT    nInnerAlgType;               //С�㷨���
//	UINT    nLocGroupID;                 //��λ���	
//	HObject hRegion;                     //����������Ϣ
//	int     nResvered[DH_MAX_NAMESIZE];  //����
//	_REGION_PROPERTY()
//	{
//		nROIIndex      = 0;
//		nAlgorithmType = 0;
//		nInnerAlgType  = 0;
//		nLocGroupID    = 0;
//		GenEmptyObj(&hRegion);
//	
//		memset(nResvered,0,DH_MAX_NAMESIZE*sizeof(int));
//	}
//	_REGION_PROPERTY(const _REGION_PROPERTY& sReg)
//	{
//		*this = sReg;
//	}
//	_REGION_PROPERTY& operator=(const _REGION_PROPERTY& sReg)
//	{
//		nROIIndex      = sReg.nROIIndex;       //�������
//		nAlgorithmType = sReg.nAlgorithmType;  //���㷨���
//		nInnerAlgType  = sReg.nInnerAlgType;   //С�㷨���
//		nLocGroupID    = sReg.nLocGroupID;     //��λ���
//		hRegion        = sReg.hRegion;         //����������Ϣ
//		memcpy(nResvered,sReg.nResvered,DH_MAX_NAMESIZE*sizeof(int));	
//		return (*this);
//	}
//}s_RegionProperty;


//ģ���������Խṹ��
typedef struct _MODEl_REGION_PROPERTY
{
	UINT nCamGroupID;//������
	int nTimeImgNum; //��Ҫ������ʱ��ͼ����	
	std::list<s_CamPosTimeID> lstCamPosTime;
	UINT nROIIndex;     //�������
	UINT nCheckType;    //�������
	UINT nAlgorithmType;//���㷨���
	UINT nInnerAlgType; //С�㷨���
	UINT nLocGroupID;   //��λ���	
	int  nResvered[DH_MAX_NAMESIZE];//����	
	HObject hRegion;
	CString strDescription;    // ����[2018/12/16 WZS]
	

	_MODEl_REGION_PROPERTY()
	{
		nROIIndex = 0;
		nAlgorithmType = 0;
		nInnerAlgType = 0;
		nCheckType = 0;
		nLocGroupID = 0;
		memset(nResvered, 0, DH_MAX_NAMESIZE * sizeof(int));
		lstCamPosTime.clear();
		nTimeImgNum = 0;
		GenEmptyObj(&hRegion);
		nCamGroupID = 0;
	}

	~_MODEl_REGION_PROPERTY()
	{
		nROIIndex = 0;
		nAlgorithmType = 0;
		nInnerAlgType = 0;
		nCheckType = 0;
		nLocGroupID = 0;
		memset(nResvered, 0, DH_MAX_NAMESIZE * sizeof(int));
		lstCamPosTime.clear();
		nTimeImgNum = 0;
		GenEmptyObj(&hRegion);
		nCamGroupID = 0;
		strDescription.Format(_T(""));    // ����[2018/12/16 WZS]
	}

	_MODEl_REGION_PROPERTY(const _MODEl_REGION_PROPERTY& sReg)
	{
		*this = sReg;
	}
	_MODEl_REGION_PROPERTY& operator=(const _MODEl_REGION_PROPERTY& sReg)
	{
		nROIIndex = sReg.nROIIndex;     //�������
		nCheckType = sReg.nCheckType;
		nAlgorithmType = sReg.nAlgorithmType;//���㷨���
		nInnerAlgType = sReg.nInnerAlgType; //С�㷨���
		nLocGroupID = sReg.nLocGroupID;   //��λ���
		memcpy(nResvered, sReg.nResvered, DH_MAX_NAMESIZE * sizeof(int));
		lstCamPosTime.assign(sReg.lstCamPosTime.begin(), sReg.lstCamPosTime.end());
		nTimeImgNum = sReg.nTimeImgNum;
		// [2018/09/20 WZS]hRegion = sReg.hRegion;
		CopyObj(sReg.hRegion, &hRegion, 1, -1);
		nCamGroupID = sReg.nCamGroupID;
		strDescription.Format(_T("%s"), sReg.strDescription);    // ����[2018/12/16 WZS]
		return (*this);
	}
	void lst_vec_clear()
	{
		nROIIndex = 0;
		nAlgorithmType = 0;
		nInnerAlgType = 0;
		nCheckType = 0;
		nLocGroupID = 0;
		memset(nResvered, 0, DH_MAX_NAMESIZE * sizeof(int));
		lstCamPosTime.clear();
		nTimeImgNum = 0;
		GenEmptyObj(&hRegion);
		nCamGroupID = 0;
	}
}s_ModelRegionPro;

//�м����ṹ��[�������ж�λ��Ϣ]
typedef struct _MIDDLE_RESULT
{
    std::vector<s_LocGroupResult> vsLocGroupResult;  //���еĶ�λ��Ϣ
	UINT   nSingleErrorCount;						 //���������Ч�������������������ļ���ȡ��
	std::list<s_sSingleErrorInfo> lstSingleErrorInfo;	 //����������������Ϣ����
    _MIDDLE_RESULT()
	{
		vsLocGroupResult.clear();
		nSingleErrorCount = 0;
		lstSingleErrorInfo.clear();
	}
    _MIDDLE_RESULT(const _MIDDLE_RESULT& sMr)
	{
        *this = sMr;
	}
	~_MIDDLE_RESULT()
	{
        vsLocGroupResult.clear();
		nSingleErrorCount = 0;
		lstSingleErrorInfo.clear();
	}
	_MIDDLE_RESULT& operator=(const _MIDDLE_RESULT& sMr)
	{
		vsLocGroupResult.assign(sMr.vsLocGroupResult.begin(), sMr.vsLocGroupResult.end());
		nSingleErrorCount = sMr.nSingleErrorCount;
		lstSingleErrorInfo.assign(sMr.lstSingleErrorInfo.begin(), sMr.lstSingleErrorInfo.end());
		return (*this);
	}
	void lst_vec_clear()
	{
		vsLocGroupResult.clear();
		lstSingleErrorInfo.clear();
		nSingleErrorCount = 0;
	}
}s_MiddleResult;

typedef struct _ALG_OUTPUT_PARAM
{
	UINT   nSingleErrorCount;						 //���������Ч�������������������ļ���ȡ��
	std::list<s_sSingleErrorInfo> lstSingleErrorInfo;	 //����������������Ϣ����
	UINT   nUnionErrorCount;						 //���������Ч�������������������ļ���ȡ��
	std::list<s_UnionErrorInfo> lstUnionErrorInfo;	 //����������������Ϣ����
	_ALG_OUTPUT_PARAM()
	{
		nSingleErrorCount = 0;
		lstSingleErrorInfo.clear();
		nUnionErrorCount = 0;
		lstUnionErrorInfo.clear();
	}
	~_ALG_OUTPUT_PARAM()
	{
		nSingleErrorCount = 0;
		lstSingleErrorInfo.clear();
		nUnionErrorCount = 0;
		lstUnionErrorInfo.clear();
	}
	_ALG_OUTPUT_PARAM(const _ALG_OUTPUT_PARAM& sSCCR)
	{
		*this = sSCCR;
	}
	_ALG_OUTPUT_PARAM& operator=(const _ALG_OUTPUT_PARAM& sSCCResult)
	{
		nSingleErrorCount = sSCCResult.nSingleErrorCount;
		lstSingleErrorInfo = sSCCResult.lstSingleErrorInfo;
		nUnionErrorCount = sSCCResult.nUnionErrorCount;
		lstUnionErrorInfo = sSCCResult.lstUnionErrorInfo;
		return (*this);
	}
	void lst_vec_clear()
	{
		if (lstSingleErrorInfo.size() > 0)
		{
			std::list<s_sSingleErrorInfo>::iterator itsEI, itsEIEnd;
			itsEI = lstSingleErrorInfo.begin();
			itsEIEnd = lstSingleErrorInfo.end();
			for (; itsEI != itsEIEnd; ++itsEI)
			{
				itsEI->lst_vec_clear();
			}
			lstSingleErrorInfo.clear();
		}
		nSingleErrorCount = 0;

		if (lstUnionErrorInfo.size() > 0)
		{
			std::list<s_UnionErrorInfo>::iterator itsEI, itsEIEnd;
			itsEI = lstUnionErrorInfo.begin();
			itsEIEnd = lstUnionErrorInfo.end();
			for (; itsEI != itsEIEnd; ++itsEI)
			{
				//itsEI->lst_vec_clear();
			}
			lstUnionErrorInfo.clear();
		}
		nUnionErrorCount = 0;
	}
}s_AlgCheckOutputParam;
typedef struct _CHECK_INPUT_PARAM
{
	std::list<s_InputTimeImage> lstInputImage;		  //���������һ������???  Ϊ�ļ���ͼ����Ϊһ������ͨ������������ļ�ȷ���� �����������ֻ��һ��
}s_AlgCheckInputParam;


//////////////////////////////////////////////////////////////////////////
//������������[������]
class CAlgVirtualCheck  
{
public:
	//���캯��
	CAlgVirtualCheck()
	{
	}	
	//��������
	virtual ~CAlgVirtualCheck()
	{
	}
	//��ʼ�����麯��
	virtual s_Status Init(const s_AlgInitParam& sAlgInitParam,int nAlgTypID,s_TypeCheckParam* psCheckParams)=0;
	
	//���������麯��
	virtual s_Status LoadTask(const CString strTaskName,const list<s_ModelRegionPro> lstRegion)=0;

	//��ȡ������
	virtual s_Status GetAlgParam(const s_ErrorTpye sErrorTpye,s_TypeCheckParam  * psCheckParams)=0;

	//��ģ�����ȡ����
	virtual s_Status GetAlgParam(const s_ErrorTpye sErrorTpye, s_TypeCheckParam  * psCheckParams,const s_ModelRegionPro& sSrcRegion, const BOOL bAllowModify) = 0;

	//���ü�����
	virtual s_Status SetAlgParam(const s_ErrorTpye sErrorTpye,const s_TypeCheckParam  sCheckParams)=0;
	
	//���ý�ģ�������
	virtual s_Status SetAlgParam(const s_ErrorTpye sErrorTpye, const s_TypeCheckParam  sCheckParams, const s_ModelRegionPro& sSrcRegion) = 0;
	
	//�Զ���ģ
	virtual s_Status AutoCreateAlgModel(const s_AlgCheckInputParam& sAlgInputParam,
		const s_ModelRegionPro& sSrcRegion,
		CString strAlgModeName,
		s_TypeCheckParam  sCheckParams,
		BOOL bIsModelImg,
		s_Status &CreateOrLearnState)=0;

	//�ͷŴ��麯��
	virtual s_Status Free()=0;

	//������Ҫ���м������������еĶ�λ�����,ÿ�μ�⺯��֮ǰ����
 	virtual s_Status SetMiddleResult(const s_MiddleResult& sMiddleResult)=0;

	//������ⴿ�麯��,bParall�Ƿ������ڲ�����
	virtual s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam,BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult)=0;
	
	//������λģ���ⴿ�麯��
	virtual s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam,  BOOL bParall,
		s_MiddleResult* sMiddleResult,
		s_AlgCheckOutputParam *sAlgCheckOutputResult) = 0;
	
	//Join��ⴿ�麯��
	virtual s_Status JoinCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult)=0;

	//MutiCam��ⴿ�麯��
	virtual s_Status MutiCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult)=0;
	
	//���´��麯��
	virtual s_Status UpdateParam()=0;

	//������ģ�崿�麯��
	virtual s_Status SaveCheckModel()=0;	
};

//////////////////////////////////////////////////////////////////////////
// �������������
class CAlgVirtualPROPDLG 
{
public:
	CAlgVirtualPROPDLG()
	{
	}
	virtual ~CAlgVirtualPROPDLG()
	{
	}

	//��ʼ�����麯��
	virtual s_Status Init(CString strTaskName, const s_AlgInitParam& sAlgInitParam)=0;
	
	//���������㷨���ԶԻ����麯��;
	virtual s_Status SetAlgParamDlg(const s_AlgCheckInputParam& sAlgInputParam, \
		const s_ModelRegionPro& sRegion, BOOL bAllowModify)=0;
	
	//���������麯��
	virtual s_Status AlgCopyRegion(const s_AlgCheckInputParam& sAlgInputParam,\
		s_ModelRegionPro& sNewRegion, const s_ModelRegionPro& sSrcRegion)=0;

	//�޸������麯��
	virtual s_Status AlgModifyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
		const s_ModelRegionPro& sSrcRegion, const s_ModelRegionPro& sNewRegion)=0;

	//ɾ�������麯��
	virtual s_Status AlgDeleteRegion(const list<s_ModelRegionPro>& lstRegion)=0;

	//�õ��㷨���ʹ��麯��
	virtual s_Status GetAlgorithmInfo(list<s_InnerAlgType>& lstInnerAlgType)=0;

	//��������������Ķ���
	CAlgVirtualCheck * pChecker; 

};

//�ۺϲ���㷨����Ϣ�ṹ
typedef   struct _ALG_CHECK_INFO
{
	s_AlgType           sAlgType;//�㷨���ͽṹ��
	CAlgVirtualCheck*   AlgCheck;
	CAlgVirtualPROPDLG* AlgProDlg;
	BOOL                bPropInit;
	list<s_ModelRegionPro> lstAlgRegion;        //ÿ���㷨��Ӧ����������
												//	s_AlgCheckOutputParam  sAlgCheckOutputParam;// �㷨�쳣���߲����ʱ����д�㷨������� [2/27/2012 Denghl]
	CCriticalSection csAlgSection;

	BOOL pbTaskAlg;
	HINSTANCE hDll;

	_ALG_CHECK_INFO()
	{
		AlgCheck = NULL;
		bPropInit = FALSE;
		AlgProDlg = NULL;
		lstAlgRegion.clear();
		pbTaskAlg = NULL;
		hDll = NULL;
	}
	~_ALG_CHECK_INFO()
	{
		AlgCheck = NULL;
		bPropInit = FALSE;
		AlgProDlg = NULL;
		lstAlgRegion.clear();
		pbTaskAlg = NULL;
		hDll = NULL;
	}
	_ALG_CHECK_INFO(const _ALG_CHECK_INFO& sSynInfo)
	{
		*this = sSynInfo;
	}
	_ALG_CHECK_INFO& operator=(const _ALG_CHECK_INFO& sSynInfo)
	{
		sAlgType = sSynInfo.sAlgType;
		AlgCheck = sSynInfo.AlgCheck;
		AlgProDlg = sSynInfo.AlgProDlg;
		bPropInit = sSynInfo.bPropInit;
		lstAlgRegion.assign(sSynInfo.lstAlgRegion.begin(), sSynInfo.lstAlgRegion.end());
		pbTaskAlg = sSynInfo.pbTaskAlg;
		hDll = sSynInfo.hDll;
		//	sAlgCheckOutputParam = sSynInfo.sAlgCheckOutputParam;
		return (*this);
	}
}s_AlgCheckInfo;

typedef   struct _ALG_POS_TIME_CHECK
{
	UINT nCamGroupID;
	s_CamPosTimeID sCamPosTimeID;
	std::vector<s_AlgCheckInfo> vcsAlgSingleCheckInfo;
	_ALG_POS_TIME_CHECK()
	{
		vcsAlgSingleCheckInfo.clear();
	}
	~_ALG_POS_TIME_CHECK()
	{
		vcsAlgSingleCheckInfo.clear();
	}
	_ALG_POS_TIME_CHECK(const _ALG_POS_TIME_CHECK& sSynInfo)
	{
		*this = sSynInfo;
	}
	_ALG_POS_TIME_CHECK& operator=(const _ALG_POS_TIME_CHECK& sSynInfo)
	{
		sCamPosTimeID = sSynInfo.sCamPosTimeID;
		vcsAlgSingleCheckInfo.assign(sSynInfo.vcsAlgSingleCheckInfo.begin(), sSynInfo.vcsAlgSingleCheckInfo.end());
		nCamGroupID = sSynInfo.nCamGroupID;
		//	sAlgCheckOutputParam = sSynInfo.sAlgCheckOutputParam;
		return (*this);
	}
}s_AlgPosTimCheck;//ĳʱ��ͼ��������㷨�͵�ǰ������ϼ��

typedef   struct _ALG_MUTI_JOIN_CHECK
{
	UINT nCamGroupID;
	std::list<s_CamPosTimeID> lstCamPosTimeID;
	s_AlgCheckInfo sAlgMutiJoinCheckInfo;
	_ALG_MUTI_JOIN_CHECK()
	{
		lstCamPosTimeID.clear();
		nCamGroupID = 0;

	}
	_ALG_MUTI_JOIN_CHECK(const _ALG_MUTI_JOIN_CHECK& sSynInfo)
	{
		*this = sSynInfo;
	}
	_ALG_MUTI_JOIN_CHECK& operator=(const _ALG_MUTI_JOIN_CHECK& sSynInfo)
	{
		lstCamPosTimeID = sSynInfo.lstCamPosTimeID;
		sAlgMutiJoinCheckInfo = sSynInfo.sAlgMutiJoinCheckInfo;
		nCamGroupID = sSynInfo.nCamGroupID;
		//	sAlgCheckOutputParam = sSynInfo.sAlgCheckOutputParam;
		return (*this);
	}

}s_AlgMutiJoinCheck;//ĳʱ��ͼ��������㷨�͵�ǰ������ϼ��

typedef struct _CAMERA_GROUP_ALG_MOD
{
	int nGroupID; //��λID	
	int nUnionType;//���ϼ������
	int nRegionNum; //�������
	std::list<s_ModelRegionPro> lstAlgModRegion;
	_CAMERA_GROUP_ALG_MOD()
	{
		nGroupID = 0;
		nUnionType = 0;
		nRegionNum = 0;
		lstAlgModRegion.clear();
	}
	~_CAMERA_GROUP_ALG_MOD()
	{

	}
	_CAMERA_GROUP_ALG_MOD(const _CAMERA_GROUP_ALG_MOD& sEI)
	{
		*this = sEI;
	}
	_CAMERA_GROUP_ALG_MOD& operator=(const _CAMERA_GROUP_ALG_MOD& sA)
	{
		nGroupID = sA.nGroupID;
		nUnionType = sA.nUnionType;
		nRegionNum = sA.nRegionNum;
		lstAlgModRegion = sA.lstAlgModRegion;

		return *this;
	}
	void lst_vec_clear()
	{
		if (lstAlgModRegion.size() > 0)
		{
			std::list<s_ModelRegionPro>::iterator it;
			for (it = lstAlgModRegion.begin(); it != lstAlgModRegion.end(); it++)
			{
				it->lst_vec_clear();
			}
			lstAlgModRegion.clear();
		}
	}
}s_CameraGroupAlgMod;//ϵͳ��λ����

typedef struct _SYSTEM_ALG_MOD
{
	int nCamGroupNum;//������������й�λ���������ϼ������
	std::vector<s_CameraGroupAlgMod> vcAlgCamGroup;
	_SYSTEM_ALG_MOD()
	{
		nCamGroupNum = 0;

		vcAlgCamGroup.clear();
	}
	~_SYSTEM_ALG_MOD()
	{

	}
	_SYSTEM_ALG_MOD(const _SYSTEM_ALG_MOD& sEI)
	{
		*this = sEI;
	}
	_SYSTEM_ALG_MOD& operator=(const _SYSTEM_ALG_MOD& sA)
	{
		nCamGroupNum = sA.nCamGroupNum;

		vcAlgCamGroup = sA.vcAlgCamGroup;

		return *this;
	}
	void lst_vec_clear()
	{
		if (vcAlgCamGroup.size() > 0)
		{
			std::vector<s_CameraGroupAlgMod>::iterator it;
			for (it = vcAlgCamGroup.begin(); it != vcAlgCamGroup.end(); it++)
			{
				it->lst_vec_clear();
			}
			vcAlgCamGroup.clear();
		}
	}
}s_SystemAlgModes;//���ϵͳ����

#endif//_AlgVirtualBaseClass
