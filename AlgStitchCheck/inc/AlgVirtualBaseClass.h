//AlgVirtualBaseClass.h   算法各层虚拟基类
/************************************************************************/
/* Copyright (c) 2015, 北京大恒图像视觉有限公司印钞事业部  
/* All rights reserved.													
/*																		
/* 文件名称： AlgVirtualBaseClass.h												
/* 摘要：算法各层虚拟基类
/*
/* 作者：    邓红丽
/* 版本：    V1.0
/* 修改人：  田仁赞
/* 版本：    V2.0
/* 修改时间：2015-06-04 
/* 修改记录：1. 格式化框架代码，增强可读性 
              2. 完善中间结果结构体_MIDDLE_RESULT

/* 修改人：  邓红丽、张波
/* 版本：    V3.0
/* 修改时间：2018-11-07
/* 修改记录：1.调整区域结构体、检测结果结构体
			 2.增加建模界面自动建模、获取与设置参数结构体
/************************************************************************/
#if !defined(_AlgVirtualBaseClass)
#define _AlgVirtualBaseClass
#include "AlgControlInterface.h"
#include "afxmt.h"   // [2018/09/10 WZS]
//大算法中子算法信息结构体
typedef struct _INNER_ALG_TYPE
{
	UINT nInnerTypeID;					//算法类型ID(各算法层唯一标识)
	BOOL bSupportLearn;					//是否支持学习 TRUE：支持 FASLE：不支持 
	BOOL bPopUpDlg;						//学习是否弹出对话框 TRUE：弹出 FASLE：不弹出
	CString strTypeName;	//算法类型名称

	// 添加初始化和等号运算符重载
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

//大算法信息结构体
typedef struct _ALG_TYPE
{
	UINT nAlgTypeID;                             //大算法类型ID唯一标识
	UINT nCheckType;                             //算法联合检测类型，0为Single，1为Join，2为Muti
	CString strTypeName;             //大算法类型名称
	CString strDllName;             //大算法类型名称
	std::list< s_InnerAlgType > lstInnerAlgType; //子算法信息链表

	// 添加初始化和等号运算符重载
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

//定位组结果结构体
typedef  struct _LOC_GROUP_RESULT
{
	UINT   nLocGroupID;      //定位组号
	float  fLineShiftLoc;    //横向偏移位置
	float  fColShiftLoc;     //横向偏移位置
	float  fRoundAngle;      //旋转角度，弧度制
	HTuple htHomMat;         //仿射矩阵
	HTuple htHomMatInv;      //仿射逆矩阵
	int    nLocCount;        //该定位组包含的定位个数
	BOOL   bHaveError;       //True，表示定位组有错

	// 添加初始化和等号运算符重载 [5/17/2011 ChenLiang]
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

////区域属性结构体
//typedef struct _REGION_PROPERTY
//{
//	UINT    nROIIndex;                   //区域序号
//	UINT    nAlgorithmType;              //大算法编号
//	UINT    nInnerAlgType;               //小算法编号
//	UINT    nLocGroupID;                 //定位组号	
//	HObject hRegion;                     //具体区域信息
//	int     nResvered[DH_MAX_NAMESIZE];  //保留
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
//		nROIIndex      = sReg.nROIIndex;       //区域序号
//		nAlgorithmType = sReg.nAlgorithmType;  //大算法编号
//		nInnerAlgType  = sReg.nInnerAlgType;   //小算法编号
//		nLocGroupID    = sReg.nLocGroupID;     //定位组号
//		hRegion        = sReg.hRegion;         //具体区域信息
//		memcpy(nResvered,sReg.nResvered,DH_MAX_NAMESIZE*sizeof(int));	
//		return (*this);
//	}
//}s_RegionProperty;


//模板区域属性结构体
typedef struct _MODEl_REGION_PROPERTY
{
	UINT nCamGroupID;//相机组号
	int nTimeImgNum; //需要关联的时序图像数	
	std::list<s_CamPosTimeID> lstCamPosTime;
	UINT nROIIndex;     //区域序号
	UINT nCheckType;    //检测类型
	UINT nAlgorithmType;//大算法编号
	UINT nInnerAlgType; //小算法编号
	UINT nLocGroupID;   //定位组号	
	int  nResvered[DH_MAX_NAMESIZE];//保留	
	HObject hRegion;
	CString strDescription;    // 描述[2018/12/16 WZS]
	

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
		strDescription.Format(_T(""));    // 描述[2018/12/16 WZS]
	}

	_MODEl_REGION_PROPERTY(const _MODEl_REGION_PROPERTY& sReg)
	{
		*this = sReg;
	}
	_MODEl_REGION_PROPERTY& operator=(const _MODEl_REGION_PROPERTY& sReg)
	{
		nROIIndex = sReg.nROIIndex;     //区域序号
		nCheckType = sReg.nCheckType;
		nAlgorithmType = sReg.nAlgorithmType;//大算法编号
		nInnerAlgType = sReg.nInnerAlgType; //小算法编号
		nLocGroupID = sReg.nLocGroupID;   //定位组号
		memcpy(nResvered, sReg.nResvered, DH_MAX_NAMESIZE * sizeof(int));
		lstCamPosTime.assign(sReg.lstCamPosTime.begin(), sReg.lstCamPosTime.end());
		nTimeImgNum = sReg.nTimeImgNum;
		// [2018/09/20 WZS]hRegion = sReg.hRegion;
		CopyObj(sReg.hRegion, &hRegion, 1, -1);
		nCamGroupID = sReg.nCamGroupID;
		strDescription.Format(_T("%s"), sReg.strDescription);    // 描述[2018/12/16 WZS]
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

//中间结果结构体[包含所有定位信息]
typedef struct _MIDDLE_RESULT
{
    std::vector<s_LocGroupResult> vsLocGroupResult;  //所有的定位信息
	UINT   nSingleErrorCount;						 //结果向量有效个数（最大个数从配置文件获取）
	std::list<s_sSingleErrorInfo> lstSingleErrorInfo;	 //独立检测区域错误信息链表
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
	UINT   nSingleErrorCount;						 //结果向量有效个数（最大个数从配置文件获取）
	std::list<s_sSingleErrorInfo> lstSingleErrorInfo;	 //独立检测区域错误信息链表
	UINT   nUnionErrorCount;						 //结果向量有效个数（最大个数从配置文件获取）
	std::list<s_UnionErrorInfo> lstUnionErrorInfo;	 //独立检测区域错误信息链表
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
	std::list<s_InputTimeImage> lstInputImage;		  //输入参数是一个队列???  为哪几个图像作为一个队列通过任务或配置文件确定？ 正常情况队列只有一个
}s_AlgCheckInputParam;


//////////////////////////////////////////////////////////////////////////
//检测类虚拟基类[抽象类]
class CAlgVirtualCheck  
{
public:
	//构造函数
	CAlgVirtualCheck()
	{
	}	
	//析构函数
	virtual ~CAlgVirtualCheck()
	{
	}
	//初始化纯虚函数
	virtual s_Status Init(const s_AlgInitParam& sAlgInitParam,int nAlgTypID,s_TypeCheckParam* psCheckParams)=0;
	
	//加载任务纯虚函数
	virtual s_Status LoadTask(const CString strTaskName,const list<s_ModelRegionPro> lstRegion)=0;

	//获取检测参数
	virtual s_Status GetAlgParam(const s_ErrorTpye sErrorTpye,s_TypeCheckParam  * psCheckParams)=0;

	//建模界面获取参数
	virtual s_Status GetAlgParam(const s_ErrorTpye sErrorTpye, s_TypeCheckParam  * psCheckParams,const s_ModelRegionPro& sSrcRegion, const BOOL bAllowModify) = 0;

	//设置检测参数
	virtual s_Status SetAlgParam(const s_ErrorTpye sErrorTpye,const s_TypeCheckParam  sCheckParams)=0;
	
	//设置建模界面参数
	virtual s_Status SetAlgParam(const s_ErrorTpye sErrorTpye, const s_TypeCheckParam  sCheckParams, const s_ModelRegionPro& sSrcRegion) = 0;
	
	//自动建模
	virtual s_Status AutoCreateAlgModel(const s_AlgCheckInputParam& sAlgInputParam,
		const s_ModelRegionPro& sSrcRegion,
		CString strAlgModeName,
		s_TypeCheckParam  sCheckParams,
		BOOL bIsModelImg,
		s_Status &CreateOrLearnState)=0;

	//释放纯虚函数
	virtual s_Status Free()=0;

	//设置需要的中间检测结果（如所有的定位结果）,每次检测函数之前调用
 	virtual s_Status SetMiddleResult(const s_MiddleResult& sMiddleResult)=0;

	//独立检测纯虚函数,bParall是否允许内部并行
	virtual s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam,BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult)=0;
	
	//独立定位模块检测纯虚函数
	virtual s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam,  BOOL bParall,
		s_MiddleResult* sMiddleResult,
		s_AlgCheckOutputParam *sAlgCheckOutputResult) = 0;
	
	//Join检测纯虚函数
	virtual s_Status JoinCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult)=0;

	//MutiCam检测纯虚函数
	virtual s_Status MutiCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult)=0;
	
	//更新纯虚函数
	virtual s_Status UpdateParam()=0;

	//保存检测模板纯虚函数
	virtual s_Status SaveCheckModel()=0;	
};

//////////////////////////////////////////////////////////////////////////
// 属性类虚拟基类
class CAlgVirtualPROPDLG 
{
public:
	CAlgVirtualPROPDLG()
	{
	}
	virtual ~CAlgVirtualPROPDLG()
	{
	}

	//初始化纯虚函数
	virtual s_Status Init(CString strTaskName, const s_AlgInitParam& sAlgInitParam)=0;
	
	//设置区域算法属性对话框纯虚函数;
	virtual s_Status SetAlgParamDlg(const s_AlgCheckInputParam& sAlgInputParam, \
		const s_ModelRegionPro& sRegion, BOOL bAllowModify)=0;
	
	//复制区域纯虚函数
	virtual s_Status AlgCopyRegion(const s_AlgCheckInputParam& sAlgInputParam,\
		s_ModelRegionPro& sNewRegion, const s_ModelRegionPro& sSrcRegion)=0;

	//修改区域纯虚函数
	virtual s_Status AlgModifyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
		const s_ModelRegionPro& sSrcRegion, const s_ModelRegionPro& sNewRegion)=0;

	//删除区域纯虚函数
	virtual s_Status AlgDeleteRegion(const list<s_ModelRegionPro>& lstRegion)=0;

	//得到算法类型纯虚函数
	virtual s_Status GetAlgorithmInfo(list<s_InnerAlgType>& lstInnerAlgType)=0;

	//声明检测类虚基类的对象
	CAlgVirtualCheck * pChecker; 

};

//综合层各算法总信息结构
typedef   struct _ALG_CHECK_INFO
{
	s_AlgType           sAlgType;//算法类型结构体
	CAlgVirtualCheck*   AlgCheck;
	CAlgVirtualPROPDLG* AlgProDlg;
	BOOL                bPropInit;
	list<s_ModelRegionPro> lstAlgRegion;        //每个算法对应的区域链表
												//	s_AlgCheckOutputParam  sAlgCheckOutputParam;// 算法异常或者不检测时，填写算法结果链表 [2/27/2012 Denghl]
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
}s_AlgPosTimCheck;//某时序图独立检测算法和当前相机联合检测

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

}s_AlgMutiJoinCheck;//某时序图独立检测算法和当前相机联合检测

typedef struct _CAMERA_GROUP_ALG_MOD
{
	int nGroupID; //工位ID	
	int nUnionType;//联合检测类型
	int nRegionNum; //相机个数
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
}s_CameraGroupAlgMod;//系统工位配置

typedef struct _SYSTEM_ALG_MOD
{
	int nCamGroupNum;//相机组个数，或叫工位组数或联合检测组数
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
}s_SystemAlgModes;//相机系统配置

#endif//_AlgVirtualBaseClass
