//AlgPriInterface.h   ģ����Ϣ�ṹ
//////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Copyright (c) 2015, �������ͼ���Ӿ����޹�˾ӡ����ҵ��
/* All rights reserved.
/*
/* �ļ����ƣ� AlgPriInterface.h
/* ��Ҫ˵���� �㷨���ӿڽṹ��

/* ���ߣ�    �˺���
/* �汾��    V1.0

/* �޸��ˣ�  �˺������Ų�������ɽ
/* �汾��    V5.0
/* �޸�ʱ�䣺2018-11-07
/* �޸ļ�¼��1.������ʼ���������������ṹ��
             2.�㷨�����ṹ������

/************************************************************************/
#pragma once

#if !defined(_AlgControlInterface)
#define _AlgControInterface

#define  DH_MAX_NAMESIZE		64
#define  DH_MAX_TEXTSIZE		255
#define  DH_MAX_PATH			512

#include <list>
#include <vector>
#include <algorithm>

#include "Halcon.h"
#include "HalconCpp.h"
using namespace HalconCpp;
using namespace std;
#pragma pack(1)

//������־
enum eOperateFlag
{
	OF_DoNothing = 0,       //Ĭ��״̬  ����ⲻѧϰ
	OF_Check, 	    		//���
	OF_Train,				//ѧϰ
	OF_CheckAndTrain		//�߼���ѧϰ
};

//ѧϰ��־
enum eTrainFlag
{
	TF_DisTrain = 0,		//����ѧϰ
	TF_AddTrain			//���ѧϰ
};

//ѧϰ���
enum eTrainResultFlag
{
	TRF_AllSuss = 0,		//ȫ���ɹ�
	TRF_PartSuss,	    //���ֳɹ�
	TRF_Fail            //ʧ��
};

typedef struct _CAMERA_POS_TIMEID
{
	UINT nCameraID;
	UINT nPosID;	        //λ�����
	int  nOffsetX;         //Xƫ��λ��??
	int  nOffsetY;         //Yƫ��λ��??
	UINT nTimeID;	       //ʱ�����
	_CAMERA_POS_TIMEID()
	{
		nCameraID = 0;
		nPosID = 0;
		nOffsetX = 0;
		nOffsetY = 0;
		nTimeID = 0;
	}
	~_CAMERA_POS_TIMEID()
	{
		nCameraID = 0;
		nPosID = 0;
		nOffsetX = 0;
		nOffsetY = 0;
		nTimeID = 0;
	}
	_CAMERA_POS_TIMEID(const _CAMERA_POS_TIMEID& sEI)
	{
		*this = sEI;
	}
	_CAMERA_POS_TIMEID& operator=(const _CAMERA_POS_TIMEID& sA)
	{
		nCameraID = sA.nCameraID;
		nPosID = sA.nPosID;
		nOffsetX = sA.nOffsetX;
		nOffsetY = sA.nOffsetY;
		nTimeID = sA.nTimeID;
		return (*this);
	}
	BOOL operator==(const _CAMERA_POS_TIMEID& sA)
	{
		if (nCameraID != sA.nCameraID||
		nPosID != sA.nPosID||
		nOffsetX != sA.nOffsetX||
		nOffsetY != sA.nOffsetY||
		nTimeID != sA.nTimeID)
		{
			return FALSE;
		}
		return TRUE;
	}

}s_CamPosTimeID;

typedef struct _TIME_IMAGE_CON
{
	s_CamPosTimeID sCamPosTime;
	CString strTimeDesc;    //ʱ������
	LONG        cx;					//ͼ����
	LONG        cy;					//ͼ��߶�
	UINT        nBit;				//��ͨ��λ��
	UINT        nChannel;			//ͨ����
	float    fResolution_H;         //����ֱ���
	float    fResolution_V;         //����ֱ���
									// ��ӳ�ʼ�������������
	_TIME_IMAGE_CON()
	{
		strTimeDesc.Format(_T(""));
		cx = 0;
		cy = 0;
		nBit = 0;
		nChannel = 0;
		fResolution_H = 0;
		fResolution_V = 0;
	}
	~_TIME_IMAGE_CON()
	{
		strTimeDesc.Format(_T(""));
		cx = 0;
		cy = 0;
		nBit = 0;
		nChannel = 0;
		fResolution_H = 0;
		fResolution_V = 0;
	}
	_TIME_IMAGE_CON(const _TIME_IMAGE_CON& sEI)
	{
		*this = sEI;
	}
	_TIME_IMAGE_CON& operator=(const _TIME_IMAGE_CON& sA)
	{
		sCamPosTime = sA.sCamPosTime;	
		strTimeDesc.Format(_T("%s"), sA.strTimeDesc);
		cx = sA.cx;
		cy = sA.cy;
		nBit = sA.nBit;
		nChannel = sA.nChannel;
		fResolution_H = sA.fResolution_H;
		fResolution_V = sA.fResolution_V;

		return (*this);
	}
}s_TimeImageCon;// ʵ��ͼ������

typedef struct _CAMERA_TIME_IMAGE
{
    UINT nMeterIndex;		//ͼ����ĺ�

    int nGroupID; //��λID	
    s_TimeImageCon sTimeImgConf;
    HObject *pChannelHImg; // ���е�ͨ��ͼ����������ͼ��ָ�룬���������ͨ����һ��
    HObject *pMulHImage;   // ��ͨ���ں�ͼ��һ����ʾ��ʱ���ã�������Ϊ1��

    CString strExpCode;	             //Ԥ�ں���
    eOperateFlag      OperateFlag;               //���Ų�����־
    eTrainFlag        TrainFlag;                 //����ѧϰ״��
    _CAMERA_TIME_IMAGE()
    {
        nMeterIndex = 0;
        nGroupID = 0;
        pChannelHImg = NULL;
        pMulHImage = NULL;
        strExpCode.Format(_T(""));
        OperateFlag = OF_DoNothing;
        TrainFlag = TF_DisTrain;

    }
    ~_CAMERA_TIME_IMAGE()
    {
        nMeterIndex = 0;
        nGroupID = 0;

        pChannelHImg = NULL;
        pMulHImage = NULL;
        strExpCode.Format(_T(""));
        OperateFlag = OF_DoNothing;
        TrainFlag = TF_DisTrain;
    }
    _CAMERA_TIME_IMAGE(const _CAMERA_TIME_IMAGE& sEI)
    {
        *this = sEI;
    }
    _CAMERA_TIME_IMAGE& operator=(const _CAMERA_TIME_IMAGE& sA)
    {
        nMeterIndex = sA.nMeterIndex;
        nGroupID = sA.nGroupID;
        sTimeImgConf = sA.sTimeImgConf;
        pChannelHImg = sA.pChannelHImg;
        pMulHImage = sA.pMulHImage;
        strExpCode.Format(_T("%s"), sA.strExpCode);
        OperateFlag = sA.OperateFlag;
        TrainFlag = sA.TrainFlag;

        return (*this);
    }

}s_InputTimeImage;// ʵ��ͼ�����붨��

typedef struct _CAMERA_POS_CONFIG
{
	UINT nPosID;
	CString strPosDesc;  //λ������
	int  nOffsetX;         //Xƫ��λ��??
	int  nOffsetY;         //Yƫ��λ��??
	int  nTimeNum;
	std::vector<s_TimeImageCon> vcTimeImage;//��λ������ʱ��ͼ������

	_CAMERA_POS_CONFIG()
	{
		nPosID = 0;
		strPosDesc.Format(_T(""));
		nOffsetX = 0;
		nOffsetY = 0;
		nTimeNum = 0;
		vcTimeImage.clear();
	}
	~_CAMERA_POS_CONFIG()
	{
		lst_vec_clear();
	}
	_CAMERA_POS_CONFIG(const _CAMERA_POS_CONFIG& sEI)
	{
		*this = sEI;
	}
	_CAMERA_POS_CONFIG& operator=(const _CAMERA_POS_CONFIG& sA)
	{
		nPosID = sA.nPosID;
		strPosDesc.Format(_T("%s"), sA.strPosDesc);
		nOffsetX = sA.nOffsetX;
		nOffsetY = sA.nOffsetY;
		nTimeNum = sA.nTimeNum;
		vcTimeImage.assign(sA.vcTimeImage.begin(), sA.vcTimeImage.end());
		return *this;
	}
	void lst_vec_clear()
	{
		if (vcTimeImage.size() > 0)
		{
			vcTimeImage.clear();
		}

	}
}s_CameraPosTimeConfig;//���ĳλ������

typedef struct _CAMERA_CONFIG
{
	UINT nCameraID;//������
	CString strCameraDesc;  //�������
	int nPositionNum;//��Ҫ�����λ������
	std::vector<s_CameraPosTimeConfig> vcCamPos;//����λ��ͼ������
	_CAMERA_CONFIG()
	{
		nCameraID = 0;
		strCameraDesc.Format(_T(""));
		nPositionNum = 0;
		vcCamPos.clear();
	}
	~_CAMERA_CONFIG()
	{
		lst_vec_clear();
	}
	_CAMERA_CONFIG(const _CAMERA_CONFIG& sEI)
	{
		*this = sEI;
	}
	_CAMERA_CONFIG& operator=(const _CAMERA_CONFIG& sA)
	{
		nCameraID = sA.nCameraID;
		nPositionNum = sA.nPositionNum;
		vcCamPos.assign(sA.vcCamPos.begin(), sA.vcCamPos.end());
		strCameraDesc.Format(_T("%s"), sA.strCameraDesc);
		return *this;
	}
	void lst_vec_clear()
	{
		if (vcCamPos.size() > 0)
		{
			vcCamPos.clear();
		}
	}
}s_CameraConfig;//ʵ���������
typedef struct _CAMERA_GROUP_CONFIG
{
	int nGroupID; //��λID	
	int nUnionType;//���ϼ������
	int nCamNum; //�������
	CString strGroupDesc;   //��λ����
	std::vector<s_CameraConfig> vcCamera;//�����������
	_CAMERA_GROUP_CONFIG()
	{
		nGroupID = 0;
		nUnionType = 0;
		nCamNum = 0;
		vcCamera.clear();
		strGroupDesc.Format(_T(""));
	}
	~_CAMERA_GROUP_CONFIG()
	{
		lst_vec_clear();
	}
	_CAMERA_GROUP_CONFIG(const _CAMERA_GROUP_CONFIG& sEI)
	{
		*this = sEI;
	}
	_CAMERA_GROUP_CONFIG& operator=(const _CAMERA_GROUP_CONFIG& sA)
	{
		nGroupID = sA.nGroupID;
		nUnionType = sA.nUnionType;
		nCamNum = sA.nCamNum;
		vcCamera.assign(sA.vcCamera.begin(), sA.vcCamera.end());
		strGroupDesc.Format(_T("%s"), sA.strGroupDesc);
		return *this;
	}
	void lst_vec_clear()
	{
		if (vcCamera.size() > 0)
		{
			vcCamera.clear();
		}
	}
}s_CameraGroupConfig;//ϵͳ��λ����

typedef struct _SYSTEM_CONFIG
{
	int nCamGroupNum;//������������й�λ���������ϼ������
	std::vector<s_CameraGroupConfig> vcCamGroup;
	_SYSTEM_CONFIG()
	{
		nCamGroupNum = 0;

		vcCamGroup.clear();
	}
	~_SYSTEM_CONFIG()
	{
		lst_vec_clear();
	}
	_SYSTEM_CONFIG(const _SYSTEM_CONFIG& sEI)
	{
		*this = sEI;
	}
	_SYSTEM_CONFIG& operator=(const _SYSTEM_CONFIG& sA)
	{
		nCamGroupNum = sA.nCamGroupNum;

		vcCamGroup.assign(sA.vcCamGroup.begin(), sA.vcCamGroup.end());

		return *this;
	}
	void lst_vec_clear()
	{
		if (vcCamGroup.size() > 0)
		{
			/*std::vector<s_CameraGroupConfig>::iterator it;
			for (it = vcCamGroup.begin(); it != vcCamGroup.end(); it++)
			{
				it->lst_vec_clear();
			}*/
			vcCamGroup.clear();
		}
	}
}s_SystemConfig;//���ϵͳ����

//�ص�����ָ������ Add [5/4/2018 WZS]
typedef void (WINAPI *PCHECKCALLBACK)(void* sCheckInputOutputParam);

typedef struct _ALG_INIT_PARAM
{
	CString    strAlgPath;        //�㷨ϵͳ����·����Ĭ�ϲ���·��
	CString    strAlglogPath;     //�㷨��־·��
	CString    strAlgModePath;    //�㷨ģ��·��
	s_SystemConfig sSystemConfig;//ϵͳ�������
	PCHECKCALLBACK pCallBackFun;    //�ص�����ָ�� 
								 // ��ӳ�ʼ�������������
	_ALG_INIT_PARAM()
	{
		strAlgPath.Format(_T(""));
		strAlglogPath.Format(_T(""));
		strAlgModePath.Format(_T(""));
	}
	~_ALG_INIT_PARAM()
	{
		//TRACE("�ͷ�s_AlgInitParamָ��%ld\n",this);
		strAlgPath.Format(_T(""));
		strAlglogPath.Format(_T(""));
		strAlgModePath.Format(_T(""));
		lst_vec_clear();
	}
	_ALG_INIT_PARAM(const _ALG_INIT_PARAM& sAIP)
	{
		*this = sAIP;
	}
	_ALG_INIT_PARAM& operator=(const _ALG_INIT_PARAM& sAIParam)
	{
		strAlgPath.Format(_T("%s"), sAIParam.strAlgPath);
		strAlglogPath.Format(_T("%s"), sAIParam.strAlglogPath);
		strAlgModePath.Format(_T("%s"), sAIParam.strAlgModePath);
		sSystemConfig = sAIParam.sSystemConfig;
		pCallBackFun = sAIParam.pCallBackFun;
		return (*this);
	}
	void lst_vec_clear()
	{
		sSystemConfig.lst_vec_clear();
	}
}s_AlgInitParam;

//�����������ṹ��
//typedef struct _ALG_CHECK_INPUT_PARAM
//{
//	std::list<s_InputTimeImage> lstInputImage;		
//}s_AlgCheckInputParam;
typedef struct _REGION_SIGN
{
	UINT nMeterIndex;		//ͼ����ĺ�
	int nGroupID; //��λID
	UINT nCameraID;
	UINT nPosID;            //λ�����
	int  nOffsetX;         //Xƫ��λ��??
	int  nOffsetY;         //Yƫ��λ��??
	UINT nTimeID;	       //ʱ�����
	UINT   nROIIndex;	   //ROI���
	_REGION_SIGN()
	{
		nMeterIndex = 0;
		nGroupID = 0;
		nCameraID = 0;
		nPosID = 0;            //λ�����
		nOffsetX = 0;         //Xƫ��λ��??
		nOffsetY = 0;         //Yƫ��λ��??
		nTimeID = 0;	       //ʱ�����
		nROIIndex = 0;								//ROI���
	}
	_REGION_SIGN(const _ALG_INIT_PARAM& sAIP)
	{
		*this = sAIP;
	}
	_REGION_SIGN& operator=(const _REGION_SIGN& sAIParam)
	{
		nMeterIndex = sAIParam.nMeterIndex;
		nGroupID = sAIParam.nGroupID;
		nCameraID = sAIParam.nCameraID;
		nPosID = sAIParam.nPosID;            //λ�����
		nOffsetX = sAIParam.nOffsetX;         //Xƫ��λ��??
		nOffsetY = sAIParam.nOffsetY;         //Yƫ��λ��??
		nTimeID = sAIParam.nTimeID;	       //ʱ�����
		nROIIndex = sAIParam.nROIIndex;								//ROI���
		return (*this);
	}
}s_sRegionSign;

typedef struct _UNION_ERRORINFO
{
	std::list<s_sRegionSign> lstRegionSign;
	UINT   nAlgType;                                //�㷨���ͱ��
	BYTE   bytErrorType;							//�������ͱ��
	BYTE   bytErrorGrade;							//���󼶱�
	CString strErrorDescription;
	RECT   RectErrorReg;							//����������ʵ
	CString strCheckResultCode;
	INT   nResvered[256];							//����
	HObject hErrorRegs;                             //����λ�ã�����ڱ���ͼ��V3.0
	HObject hStandRegs;                             //��׼λ��
	int    nErrorArea;                              //�������
	eTrainResultFlag TrainResultFlag;           //ѧϰ�Ƿ�ɹ���־
												// ��ӳ�ʼ�������������
	_UNION_ERRORINFO()
	{
		nAlgType = 0;
		bytErrorType = 0;
		bytErrorGrade = 0;
		lstRegionSign.clear();
		nErrorArea = 0;
		GenEmptyObj(&hErrorRegs);
		GenEmptyObj(&hStandRegs);

		SetRect(&RectErrorReg, 0, 0, 0, 0);
		strErrorDescription.Format(_T(""));
		memset(&nResvered, 0, 256 * sizeof(UINT));
		strCheckResultCode.Format(_T(""));

	}
	~_UNION_ERRORINFO()
	{
		nAlgType = 0;
		bytErrorType = 0;
		bytErrorGrade = 0;

		SetRect(&RectErrorReg, 0, 0, 0, 0);
		strErrorDescription.Format(_T(""));
		memset(&nResvered, 0, 256 * sizeof(UINT));
		strCheckResultCode.Format(_T(""));

	}
	_UNION_ERRORINFO(const _UNION_ERRORINFO& sEI)
	{
		*this = sEI;
	}
	_UNION_ERRORINFO& operator=(const _UNION_ERRORINFO& sEInfo)
	{

		nAlgType = sEInfo.nAlgType;
		lstRegionSign = sEInfo.lstRegionSign;
		bytErrorType = sEInfo.bytErrorType;
		bytErrorGrade = sEInfo.bytErrorGrade;
		RectErrorReg = sEInfo.RectErrorReg;
		strCheckResultCode.Format(_T("%s"), sEInfo.strCheckResultCode);

		strErrorDescription.Format(_T("%s"), sEInfo.strErrorDescription);
		memcpy(&nResvered, sEInfo.nResvered, 256 * sizeof(INT));
		hErrorRegs = sEInfo.hErrorRegs;
		hStandRegs = sEInfo.hStandRegs;
		nErrorArea = sEInfo.nErrorArea;
		TrainResultFlag = sEInfo.TrainResultFlag;
		

		return (*this);
	}
	void lst_vec_clear()
	{

		if (lstRegionSign.size() > 0)
		{
			lstRegionSign.clear();
		}
	}

}s_UnionErrorInfo;

//������Ϣ
typedef struct _SINGLE_ERRORINFO
{
	s_sRegionSign sRegionSign;

	UINT   nAlgType;                                //�㷨���ͱ��
	BYTE   bytErrorType;							//�������ͱ��
	BYTE   bytErrorGrade;							//���󼶱�
	CString strErrorDescription;
	RECT   RectErrorReg;							//����������ʵ
	CString strCheckResultCode;
	INT   nResvered[256];							//����
	HObject hErrorRegs;                             //����λ�ã�����ڱ���ͼ��V3.0
	HObject hStandRegs;                             //��׼λ��
	int    nErrorArea;                              //�������

	HTuple htOutputMidResult;                       //����м���

	eTrainResultFlag TrainResultFlag;           //ѧϰ�Ƿ�ɹ���־

    double meanValue;
    double DeviationValue;
	// ��ӳ�ʼ�������������
	_SINGLE_ERRORINFO()
	{
		nAlgType = 0;
		bytErrorType = 0;
		bytErrorGrade = 0;

		nErrorArea = 0;
		GenEmptyObj(&hErrorRegs);
		GenEmptyObj(&hStandRegs);

		SetRect(&RectErrorReg, 0, 0, 0, 0);

		strErrorDescription.Format(_T(""));
		memset(&nResvered, 0, 256 * sizeof(INT));
		strCheckResultCode.Format(_T(""));
	}
	~_SINGLE_ERRORINFO()
	{
		nAlgType = 0;
		bytErrorType = 0;
		bytErrorGrade = 0;

		SetRect(&RectErrorReg, 0, 0, 0, 0);
		strCheckResultCode.Format(_T(""));

		strErrorDescription.Format(_T(""));
		memset(&nResvered, 0, 256 * sizeof(INT));

	}
	_SINGLE_ERRORINFO(const _SINGLE_ERRORINFO& sEI)
	{
		*this = sEI;
	}
	_SINGLE_ERRORINFO& operator=(const _SINGLE_ERRORINFO& sEInfo)
	{
		nAlgType = sEInfo.nAlgType;
		sRegionSign = sEInfo.sRegionSign;
		bytErrorType = sEInfo.bytErrorType;
		bytErrorGrade = sEInfo.bytErrorGrade;
		strCheckResultCode.Format(_T("%s"), sEInfo.strCheckResultCode);

		strErrorDescription.Format(_T("%s"), sEInfo.strErrorDescription);
		memcpy(&nResvered, sEInfo.nResvered, 256 * sizeof(INT));
		hErrorRegs = sEInfo.hErrorRegs;
		hStandRegs = sEInfo.hStandRegs;
		nErrorArea = sEInfo.nErrorArea;
		RectErrorReg = sEInfo.RectErrorReg;

		return (*this);
	}
	bool operator<(const _SINGLE_ERRORINFO& sEInfo)const
	{
		return (bytErrorGrade <= sEInfo.bytErrorGrade);
	}
	bool operator>(const _SINGLE_ERRORINFO& sEInfo)const
	{
		return (bytErrorGrade > sEInfo.bytErrorGrade);
	}
	void lst_vec_clear()
	{
		strCheckResultCode.Format(_T(""));

		strErrorDescription.Format(_T(""));
		memset(&nResvered, 0, 256 * sizeof(INT));
	}
} s_sSingleErrorInfo;

//��λ�����ṹ��
typedef  struct _LOC_GROUP_RESULT_INTERFACE
{
	UINT   nLocGroupID;      //��λ���
	float  fLineShiftLoc;    //����ƫ��λ��
	float  fColShiftLoc;     //����ƫ��λ��
	float  fRoundAngle;      //��ת�Ƕȣ�������
	HTuple htHomMat;         //�������
	HTuple htHomMatInv;      //���������
	int    nLocCount;        //�ö�λ������Ķ�λ����
	BOOL   bHaveError;       //True����ʾ��λ���д�

							 // ��ӳ�ʼ���͵Ⱥ���������� 
	_LOC_GROUP_RESULT_INTERFACE()
	{
		nLocGroupID = 0;
		fLineShiftLoc = 0.0;
		fColShiftLoc = 0.0;
		fRoundAngle = 0.0;
		nLocCount = 0;
		bHaveError = FALSE;

		htHomMat[0] = HTuple(1);
		htHomMat[1] = HTuple(0);
		htHomMat[2] = HTuple(0);
		htHomMat[3] = HTuple(0);
		htHomMat[4] = HTuple(1);
		htHomMat[5] = HTuple(0);
		HomMat2dInvert(htHomMat, &htHomMatInv);

	}
	_LOC_GROUP_RESULT_INTERFACE(const _LOC_GROUP_RESULT_INTERFACE& sLGR)
	{
		*this = sLGR;
	}
	_LOC_GROUP_RESULT_INTERFACE& operator=(const _LOC_GROUP_RESULT_INTERFACE& sLocGroReuslt)
	{
		nLocGroupID = sLocGroReuslt.nLocGroupID;
		fLineShiftLoc = sLocGroReuslt.fLineShiftLoc;
		fColShiftLoc = sLocGroReuslt.fColShiftLoc;
		fRoundAngle = sLocGroReuslt.fRoundAngle;
		nLocCount = sLocGroReuslt.nLocCount;
		bHaveError = sLocGroReuslt.bHaveError;

		htHomMat = sLocGroReuslt.htHomMat;
		htHomMatInv = sLocGroReuslt.htHomMatInv;

		return (*this);
	}
}s_LocGroupResultInterface;

//�м����ṹ��[�������ж�λ��Ϣ]
typedef struct _MIDDLE_RESULT_INTERFACE
{
	std::vector<s_LocGroupResultInterface> vsLocGroupResultnterface;  //���еĶ�λ��Ϣ
	UINT   nSingleErrorCount;						 //���������Ч�������������������ļ���ȡ��
	std::list<s_sSingleErrorInfo> lstSingleErrorInfo;	 //����������������Ϣ����
	_MIDDLE_RESULT_INTERFACE()
	{
		vsLocGroupResultnterface.clear();
		nSingleErrorCount = 0;
		lstSingleErrorInfo.clear();
	}
	_MIDDLE_RESULT_INTERFACE(const _MIDDLE_RESULT_INTERFACE& sMr)
	{
		*this = sMr;
	}
	~_MIDDLE_RESULT_INTERFACE()
	{
		vsLocGroupResultnterface.clear();
		nSingleErrorCount = 0;
		lstSingleErrorInfo.clear();
	}
	_MIDDLE_RESULT_INTERFACE& operator=(const _MIDDLE_RESULT_INTERFACE& sMr)
	{
		vsLocGroupResultnterface.assign(sMr.vsLocGroupResultnterface.begin(), sMr.vsLocGroupResultnterface.end());
		nSingleErrorCount = sMr.nSingleErrorCount;
		lstSingleErrorInfo.assign(sMr.lstSingleErrorInfo.begin(), sMr.lstSingleErrorInfo.end());
		return (*this);
	}
	void lst_vec_clear()
	{
		vsLocGroupResultnterface.clear();
		lstSingleErrorInfo.clear();
		nSingleErrorCount = 0;
	}
}s_MiddleResultInterface;

//�㷨������ṹ��
typedef struct _CHECK_INPUT_OUTPUT
{
	//�����������ṹ��
	//�㷨������ṹ��   //�㷨�����������������ṹ��ϲ����������������ݣ���
	PVOID pAlgManageContext;   //�㷨����ָ�����ڻص����ó�Ա����  ���ύ�̳߳�ǰ��ֵ
	PVOID pFrameDataInfo;     //Ԫ��ָ��  ����ϵͳ�ص���������Ԫ��
	PVOID pVoid;     //���ջص��ĳ�Ա�������ڶ���ָ��  ���ڻص����������ⲿ��Ա����
	int iChecked;        //�Ѽ��������־��   �����㷨���Ƴ���˳��

	std::list<s_InputTimeImage> lstInputImage;		  //���������һ������???  Ϊ�ļ���ͼ����Ϊһ������ͨ������������ļ�ȷ���� �����������ֻ��һ��

	s_MiddleResultInterface  sMiddleResultInterface;     //��λ������м��� Add [2/1/2019 WZS]

	UINT   nSingleErrorCount;						 //���������Ч�������������������ļ���ȡ��
	std::list<s_sSingleErrorInfo> lstSingleErrorInfo;	 //����������������Ϣ����
	UINT   nUnionErrorCount;						 //���������Ч�������������������ļ���ȡ��
	std::list<s_UnionErrorInfo> lstUnionErrorInfo;	 //����������������Ϣ����
	// ��ӳ�ʼ�������������
	_CHECK_INPUT_OUTPUT()
	{
		pAlgManageContext = NULL;   //�㷨����ָ�����ڻص����ó�Ա����  ���ύ�̳߳�ǰ��ֵ
		pFrameDataInfo = NULL;     //Ԫ��ָ��  ����ϵͳ�ص���������Ԫ��
		pVoid = NULL;     //���ջص��ĳ�Ա�������ڶ���ָ��  ���ڻص����������ⲿ��Ա����
		iChecked = 0;        //�Ѽ��������־��   �����㷨���Ƴ���˳��
		lstInputImage.clear();
		nSingleErrorCount = 0;
		lstSingleErrorInfo.clear();
		nUnionErrorCount = 0;
		lstUnionErrorInfo.clear();
	}
	~_CHECK_INPUT_OUTPUT()
	{
		nSingleErrorCount = 0;
		lstSingleErrorInfo.clear();
		nUnionErrorCount = 0;
		lstUnionErrorInfo.clear();
	}
	_CHECK_INPUT_OUTPUT(const _CHECK_INPUT_OUTPUT& sSCCR)
	{
		*this = sSCCR;
	}
	_CHECK_INPUT_OUTPUT& operator=(const _CHECK_INPUT_OUTPUT& sSCCResult)
	{
		pAlgManageContext = sSCCResult.pAlgManageContext;
		pFrameDataInfo = sSCCResult.pFrameDataInfo;
		pVoid = sSCCResult.pVoid;
		iChecked = sSCCResult.iChecked;
		lstInputImage = sSCCResult.lstInputImage;
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

}s_CheckInputOutput;


//������������  0Ϊ������1����NΪ�Լ����������
typedef struct _STATUS
{
	UINT nErrorType;		              //���غ���״̬ 0:���� 1:�쳣
	CString strErrorInfo;	  //״̬����
	CString strErrorContext; //��ϸ��Ϣ

	//////////////////////////////////////////////////////////////////////////
	_STATUS()
	{
		nErrorType = 0;

		strErrorInfo.Format(_T(""));
		strErrorContext.Format(_T(""));
	}
	_STATUS(const _STATUS& sS)
	{
		*this = sS;
	}
	_STATUS& operator=(const _STATUS& sStatus)
	{
		nErrorType = sStatus.nErrorType;

		strErrorInfo.Format(_T("%s"), sStatus.strErrorInfo);
		strErrorContext.Format(_T("%s"), sStatus.strErrorContext);
		return (*this);
	}
}s_Status;
//ÿһ�������ṹ��
typedef struct _TYPE_PARAM
{
	int nParamProp;      // 0������ 1���Բ���
	CString strParamName;//��������
	CString strParamUnit;//������λ��
	int nTypeFlag;//�������ͣ�0ΪBOOL�Ϳ�������1Ϊint�ͣ�2Ϊdouble��
	double dCurValue;//��ǰֵ
	double dMinValue;//��Χ��Сֵ 
	double dMaxValue;//��Χ���ֵ
	double dMinStep;//��С��������
	std::vector<CString> vsParamEnumValues;

	_TYPE_PARAM()
	{
		nParamProp = 1;// 0������ 1���Բ���
		strParamName.Format(_T(""));
		strParamUnit.Format(_T(""));
		nTypeFlag = 1;//�������ͣ�0ΪBOOL�Ϳ�������1Ϊint�ͣ�2Ϊdouble�� 3Ϊ���� ö��
		dCurValue = 0.0;//��ǰֵ
		dMinValue = 0.0;//��Χ��Сֵ 
		dMaxValue = 0.0;//��Χ���ֵ
		dMinStep = 0.000001;//��С��������
		vsParamEnumValues.clear();
	}
	~_TYPE_PARAM()
	{
		vsParamEnumValues.clear();
	}
	_TYPE_PARAM(const _TYPE_PARAM& sAII)
	{
		*this = sAII;
	}
	_TYPE_PARAM& operator=(const _TYPE_PARAM& sAIInfo)
	{
		nParamProp = sAIInfo.nParamProp;
		strParamName.Format(_T("%s"), sAIInfo.strParamName);
		strParamUnit.Format(_T("%s"), sAIInfo.strParamUnit);
		nTypeFlag = sAIInfo.nTypeFlag;//�������ͣ�0ΪBOOL�Ϳ�������1Ϊint�ͣ�2Ϊdouble�� 3Ϊ���� ö��
		dCurValue = sAIInfo.dCurValue;//��ǰֵ
		dMinValue = sAIInfo.dMinValue;//��Χ��Сֵ 
		dMaxValue = sAIInfo.dMaxValue;//��Χ���ֵ
		dMinStep = sAIInfo.dMinStep;//��С��������		
		vsParamEnumValues.assign(sAIInfo.vsParamEnumValues.begin(), sAIInfo.vsParamEnumValues.end());
		return *this;
	}
	BOOL operator==(const _TYPE_PARAM& sAIInfo)
	{
		BOOL bFlag = FALSE;
		if (nParamProp == sAIInfo.nParamProp
			&&nTypeFlag == sAIInfo.nTypeFlag
			&&dCurValue == sAIInfo.dCurValue
			&&dMinValue == sAIInfo.dMinValue
			&&dMaxValue == sAIInfo.dMaxValue
			&&dMinStep == sAIInfo.dMinStep)
		{
			if (strParamName == sAIInfo.strParamName)
			{

				if (strParamUnit == sAIInfo.strParamUnit)
				{
					bFlag = TRUE;
				}
			}
		}
		return bFlag;
	}
	void lst_vec_clear()
	{
		if (vsParamEnumValues.size() > 0)
		{
			vsParamEnumValues.clear();
		}
	}
}s_TypeParam;

//ÿ�����Ͳ���
typedef struct _TYPE_CHECK_PARAM
{
	UINT nALGTypeID;//�㷨���ͱ��
	CString strAlgTypeName;//�㷨��������
	std::list<s_TypeParam> lstTypeParam;	//�������£�������Ҫ�����Ĳ�������
	_TYPE_CHECK_PARAM()
	{
		nALGTypeID = 0;
		strAlgTypeName.Format(_T(""));
		lstTypeParam.clear();
	}
	~_TYPE_CHECK_PARAM()
	{
		lstTypeParam.clear();
	}
	_TYPE_CHECK_PARAM(const _TYPE_CHECK_PARAM& sAII)
	{
		*this = sAII;
	}
	_TYPE_CHECK_PARAM& operator=(const _TYPE_CHECK_PARAM& sAIInfo)
	{
		lstTypeParam = sAIInfo.lstTypeParam;
		nALGTypeID = sAIInfo.nALGTypeID;
		strAlgTypeName.Format(_T("%s"), sAIInfo.strAlgTypeName);
		return *this;
	}
	BOOL operator==(const _TYPE_CHECK_PARAM& sAIInfo)
	{
		BOOL bFlag = TRUE;
		if (nALGTypeID == sAIInfo.nALGTypeID
			&&lstTypeParam.size() == sAIInfo.lstTypeParam.size()
			&& lstTypeParam.size() > 0)
		{
			std::list<s_TypeParam>::iterator it, itEnd;
			std::list<s_TypeParam>::const_iterator itAI;
			it = lstTypeParam.begin();
			itEnd = lstTypeParam.end();
			itAI = sAIInfo.lstTypeParam.begin();
			for (; it != itEnd; ++it, ++itAI)
			{
				if (*it == *itAI)
				{
					;
				}
				else
				{
					bFlag = FALSE;
				}
			}

		}
		else
		{
			bFlag = FALSE;
		}
		return bFlag;
	}
	void lst_vec_clear()
	{
		if (lstTypeParam.size() > 0)
		{
			lstTypeParam.clear();
		}
	}
}s_TypeCheckParam;

typedef struct _CHECK_PARAMS
{
	std::list<s_TypeCheckParam> lstAlgCheckParam;	//�����㷨��������
	_CHECK_PARAMS()
	{
		lstAlgCheckParam.clear();
	}
	~_CHECK_PARAMS()
	{
		lstAlgCheckParam.clear();
	}
	_CHECK_PARAMS(const _CHECK_PARAMS& sAII)
	{
		*this = sAII;
	}
	_CHECK_PARAMS& operator=(const _CHECK_PARAMS& sAIInfo)
	{
		lstAlgCheckParam = sAIInfo.lstAlgCheckParam;
		return *this;
	}
	void lst_vec_clear()
	{
		if (lstAlgCheckParam.size() > 0)
		{
			std::list<s_TypeCheckParam>::iterator it, itEnd;
			it = lstAlgCheckParam.begin();
			itEnd = lstAlgCheckParam.end();
			for (; it != itEnd; ++it)
			{
				it->lst_vec_clear();
			}
			lstAlgCheckParam.clear();
		}
	}
}s_CheckParams;
typedef struct _ERROR_TYPE
{
	UINT nALGTypeID;//�㷨���ͱ��
	int  nErrorType;//��������
	_ERROR_TYPE()
	{
		nALGTypeID = 0;
		nErrorType = 0;
	}
	~_ERROR_TYPE()
	{

	}
	_ERROR_TYPE(const _ERROR_TYPE& sAII)
	{
		*this = sAII;
	}
	_ERROR_TYPE& operator=(const _ERROR_TYPE& sAIInfo)
	{
		nALGTypeID = sAIInfo.nALGTypeID;
		nErrorType = sAIInfo.nErrorType;
		return *this;
	}
}s_ErrorTpye;

#pragma pack()//�ļ�������ǿ�Ƶ��ֽڶ����޸�Ϊ�Զ����뷽ʽ [3/6/2012 Denghl]

#endif//_AlgControInterface.h