//AlgPriInterface.h   模版信息结构
//////////////////////////////////////////////////////////////////////
/************************************************************************/
/* Copyright (c) 2015, 北京大恒图像视觉有限公司印钞事业部
/* All rights reserved.
/*
/* 文件名称： AlgPriInterface.h
/* 简要说明： 算法主接口结构体

/* 作者：    邓红丽
/* 版本：    V1.0

/* 修改人：  邓红丽、张波、吴忠山
/* 版本：    V5.0
/* 修改时间：2018-11-07
/* 修改记录：1.调整初始化参数、错误结果结构体
             2.算法参数结构体升级

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

//操作标志
enum eOperateFlag
{
	OF_DoNothing = 0,       //默认状态  不检测不学习
	OF_Check, 	    		//检测
	OF_Train,				//学习
	OF_CheckAndTrain		//边检测边学习
};

//学习标志
enum eTrainFlag
{
	TF_DisTrain = 0,		//丢弃学习
	TF_AddTrain			//添加学习
};

//学习结果
enum eTrainResultFlag
{
	TRF_AllSuss = 0,		//全部成功
	TRF_PartSuss,	    //部分成功
	TRF_Fail            //失败
};

typedef struct _CAMERA_POS_TIMEID
{
	UINT nCameraID;
	UINT nPosID;	        //位置序号
	int  nOffsetX;         //X偏移位置??
	int  nOffsetY;         //Y偏移位置??
	UINT nTimeID;	       //时间序号
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
	CString strTimeDesc;    //时序描述
	LONG        cx;					//图像宽度
	LONG        cy;					//图像高度
	UINT        nBit;				//单通道位深
	UINT        nChannel;			//通道数
	float    fResolution_H;         //横向分辨率
	float    fResolution_V;         //纵向分辨率
									// 添加初始化和运算符重载
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
}s_TimeImageCon;// 实际图像配置

typedef struct _CAMERA_TIME_IMAGE
{
    UINT nMeterIndex;		//图像节拍号

    int nGroupID; //工位ID	
    s_TimeImageCon sTimeImgConf;
    HObject *pChannelHImg; // 所有单通道图像连起来的图像指针，个数必须和通道数一致
    HObject *pMulHImage;   // 多通道融合图（一般显示的时候用），个数为1个

    CString strExpCode;	             //预期号码
    eOperateFlag      OperateFlag;               //整张操作标志
    eTrainFlag        TrainFlag;                 //整张学习状况
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

}s_InputTimeImage;// 实际图像及输入定义

typedef struct _CAMERA_POS_CONFIG
{
	UINT nPosID;
	CString strPosDesc;  //位置描述
	int  nOffsetX;         //X偏移位置??
	int  nOffsetY;         //Y偏移位置??
	int  nTimeNum;
	std::vector<s_TimeImageCon> vcTimeImage;//该位置所有时序图像链表

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
}s_CameraPosTimeConfig;//相机某位置配置

typedef struct _CAMERA_CONFIG
{
	UINT nCameraID;//相机序号
	CString strCameraDesc;  //相机描述
	int nPositionNum;//需要拍摄的位置总数
	std::vector<s_CameraPosTimeConfig> vcCamPos;//所有位置图像链表
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
}s_CameraConfig;//实体相机配置
typedef struct _CAMERA_GROUP_CONFIG
{
	int nGroupID; //工位ID	
	int nUnionType;//联合检测类型
	int nCamNum; //相机个数
	CString strGroupDesc;   //工位描述
	std::vector<s_CameraConfig> vcCamera;//所有相机链表
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
}s_CameraGroupConfig;//系统工位配置

typedef struct _SYSTEM_CONFIG
{
	int nCamGroupNum;//相机组个数，或叫工位组数或联合检测组数
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
}s_SystemConfig;//相机系统配置

//回调函数指针声明 Add [5/4/2018 WZS]
typedef void (WINAPI *PCHECKCALLBACK)(void* sCheckInputOutputParam);

typedef struct _ALG_INIT_PARAM
{
	CString    strAlgPath;        //算法系统配置路径及默认参数路径
	CString    strAlglogPath;     //算法日志路径
	CString    strAlgModePath;    //算法模板路径
	s_SystemConfig sSystemConfig;//系统相机配置
	PCHECKCALLBACK pCallBackFun;    //回调函数指针 
								 // 添加初始化和运算符重载
	_ALG_INIT_PARAM()
	{
		strAlgPath.Format(_T(""));
		strAlglogPath.Format(_T(""));
		strAlgModePath.Format(_T(""));
	}
	~_ALG_INIT_PARAM()
	{
		//TRACE("释放s_AlgInitParam指针%ld\n",this);
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

//检测输入参数结构体
//typedef struct _ALG_CHECK_INPUT_PARAM
//{
//	std::list<s_InputTimeImage> lstInputImage;		
//}s_AlgCheckInputParam;
typedef struct _REGION_SIGN
{
	UINT nMeterIndex;		//图像节拍号
	int nGroupID; //工位ID
	UINT nCameraID;
	UINT nPosID;            //位置序号
	int  nOffsetX;         //X偏移位置??
	int  nOffsetY;         //Y偏移位置??
	UINT nTimeID;	       //时间序号
	UINT   nROIIndex;	   //ROI序号
	_REGION_SIGN()
	{
		nMeterIndex = 0;
		nGroupID = 0;
		nCameraID = 0;
		nPosID = 0;            //位置序号
		nOffsetX = 0;         //X偏移位置??
		nOffsetY = 0;         //Y偏移位置??
		nTimeID = 0;	       //时间序号
		nROIIndex = 0;								//ROI序号
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
		nPosID = sAIParam.nPosID;            //位置序号
		nOffsetX = sAIParam.nOffsetX;         //X偏移位置??
		nOffsetY = sAIParam.nOffsetY;         //Y偏移位置??
		nTimeID = sAIParam.nTimeID;	       //时间序号
		nROIIndex = sAIParam.nROIIndex;								//ROI序号
		return (*this);
	}
}s_sRegionSign;

typedef struct _UNION_ERRORINFO
{
	std::list<s_sRegionSign> lstRegionSign;
	UINT   nAlgType;                                //算法类型编号
	BYTE   bytErrorType;							//错误类型编号
	BYTE   bytErrorGrade;							//错误级别
	CString strErrorDescription;
	RECT   RectErrorReg;							//错误区域（在实
	CString strCheckResultCode;
	INT   nResvered[256];							//保留
	HObject hErrorRegs;                             //错误位置（相对于报错图像）V3.0
	HObject hStandRegs;                             //标准位置
	int    nErrorArea;                              //错误面积
	eTrainResultFlag TrainResultFlag;           //学习是否成功标志
												// 添加初始化和运算符重载
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

//错误信息
typedef struct _SINGLE_ERRORINFO
{
	s_sRegionSign sRegionSign;

	UINT   nAlgType;                                //算法类型编号
	BYTE   bytErrorType;							//错误类型编号
	BYTE   bytErrorGrade;							//错误级别
	CString strErrorDescription;
	RECT   RectErrorReg;							//错误区域（在实
	CString strCheckResultCode;
	INT   nResvered[256];							//保留
	HObject hErrorRegs;                             //错误位置（相对于报错图像）V3.0
	HObject hStandRegs;                             //标准位置
	int    nErrorArea;                              //错误面积

	HTuple htOutputMidResult;                       //输出中间结果

	eTrainResultFlag TrainResultFlag;           //学习是否成功标志

    double meanValue;
    double DeviationValue;
	// 添加初始化和运算符重载
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

//定位组结果结构体
typedef  struct _LOC_GROUP_RESULT_INTERFACE
{
	UINT   nLocGroupID;      //定位组号
	float  fLineShiftLoc;    //横向偏移位置
	float  fColShiftLoc;     //横向偏移位置
	float  fRoundAngle;      //旋转角度，弧度制
	HTuple htHomMat;         //仿射矩阵
	HTuple htHomMatInv;      //仿射逆矩阵
	int    nLocCount;        //该定位组包含的定位个数
	BOOL   bHaveError;       //True，表示定位组有错

							 // 添加初始化和等号运算符重载 
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

//中间结果结构体[包含所有定位信息]
typedef struct _MIDDLE_RESULT_INTERFACE
{
	std::vector<s_LocGroupResultInterface> vsLocGroupResultnterface;  //所有的定位信息
	UINT   nSingleErrorCount;						 //结果向量有效个数（最大个数从配置文件获取）
	std::list<s_sSingleErrorInfo> lstSingleErrorInfo;	 //独立检测区域错误信息链表
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

//算法检测结果结构体
typedef struct _CHECK_INPUT_OUTPUT
{
	//检测输入参数结构体
	//算法检测结果结构体   //算法检测结果与检测输入参数结构体合并，并增加如下内容？？
	PVOID pAlgManageContext;   //算法对象指针用于回调调用成员函数  在提交线程池前赋值
	PVOID pFrameDataInfo;     //元素指针  用于系统回调结束回收元素
	PVOID pVoid;     //接收回调的成员函数所在对象指针  用于回调函数调用外部成员函数
	int iChecked;        //已检测出结果标志或   用于算法控制出队顺序

	std::list<s_InputTimeImage> lstInputImage;		  //输入参数是一个队列???  为哪几个图像作为一个队列通过任务或配置文件确定？ 正常情况队列只有一个

	s_MiddleResultInterface  sMiddleResultInterface;     //定位输出的中间结果 Add [2/1/2019 WZS]

	UINT   nSingleErrorCount;						 //结果向量有效个数（最大个数从配置文件获取）
	std::list<s_sSingleErrorInfo> lstSingleErrorInfo;	 //独立检测区域错误信息链表
	UINT   nUnionErrorCount;						 //结果向量有效个数（最大个数从配置文件获取）
	std::list<s_UnionErrorInfo> lstUnionErrorInfo;	 //独立检测区域错误信息链表
	// 添加初始化和运算符重载
	_CHECK_INPUT_OUTPUT()
	{
		pAlgManageContext = NULL;   //算法对象指针用于回调调用成员函数  在提交线程池前赋值
		pFrameDataInfo = NULL;     //元素指针  用于系统回调结束回收元素
		pVoid = NULL;     //接收回调的成员函数所在对象指针  用于回调函数调用外部成员函数
		iChecked = 0;        //已检测出结果标志或   用于算法控制出队顺序
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


//函数返回类型  0为正常，1－－N为自己定义的类型
typedef struct _STATUS
{
	UINT nErrorType;		              //返回函数状态 0:正常 1:异常
	CString strErrorInfo;	  //状态描述
	CString strErrorContext; //详细信息

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
//每一个参数结构体
typedef struct _TYPE_PARAM
{
	int nParamProp;      // 0检测参数 1属性参数
	CString strParamName;//参数名称
	CString strParamUnit;//参数单位名
	int nTypeFlag;//参数类型：0为BOOL型开关量，1为int型，2为double型
	double dCurValue;//当前值
	double dMinValue;//范围最小值 
	double dMaxValue;//范围最大值
	double dMinStep;//最小调整幅度
	std::vector<CString> vsParamEnumValues;

	_TYPE_PARAM()
	{
		nParamProp = 1;// 0检测参数 1属性参数
		strParamName.Format(_T(""));
		strParamUnit.Format(_T(""));
		nTypeFlag = 1;//参数类型：0为BOOL型开关量，1为int型，2为double型 3为下拉 枚举
		dCurValue = 0.0;//当前值
		dMinValue = 0.0;//范围最小值 
		dMaxValue = 0.0;//范围最大值
		dMinStep = 0.000001;//最小调整幅度
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
		nTypeFlag = sAIInfo.nTypeFlag;//参数类型：0为BOOL型开关量，1为int型，2为double型 3为下拉 枚举
		dCurValue = sAIInfo.dCurValue;//当前值
		dMinValue = sAIInfo.dMinValue;//范围最小值 
		dMaxValue = sAIInfo.dMaxValue;//范围最大值
		dMinStep = sAIInfo.dMinStep;//最小调整幅度		
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

//每种类型参数
typedef struct _TYPE_CHECK_PARAM
{
	UINT nALGTypeID;//算法类型编号
	CString strAlgTypeName;//算法类型名称
	std::list<s_TypeParam> lstTypeParam;	//该类型下，所有需要调整的参数链表
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
	std::list<s_TypeCheckParam> lstAlgCheckParam;	//所有算法参数链表
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
	UINT nALGTypeID;//算法类型编号
	int  nErrorType;//报错类型
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

#pragma pack()//文件结束将强制单字节对齐修改为自动对齐方式 [3/6/2012 Denghl]

#endif//_AlgControInterface.h