/************************************************************************
/* Copyright (c) 2015, 北京大恒图像视觉有限公司印钞事业部  
/* All rights reserved.             
/*                  
/* 文件名称：  InnerCheck.h
/* 简要说明：  内部检测类
/*
/* 创建者：  邓红丽
/* 版本：    V1.0
/* 修改人：  田仁赞
/* 版本：    V2.0
/* 修改时间：2015-06-04 
/* 修改记录：1. 格式化框架代码，增强可读性
     2. 添加halcon异常抛出声明
	 3. 修改GenCheckImage()函数，添加形参变量Hobject& hCheckImage，
	    提高函数的可读性。
/* 修改人：  高丽萍
/* 版本：    V1.2.0.2
/* 修改时间：2017-06-29 
/* 修改记录：修改halcon13相关算子 

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
//内部参数设置输入参数
typedef struct 
{
	s_ModelRegionPro   sRegionProperty;   //区域属性
	s_LocGroupResult   sLocGroupResult;   //定位组结果
	s_InputTimeImage   sInputTimeImage;	  //图像信息
	//CString strExpCode;	                  //预期号码
}s_InnerCheckInputParam;

//内部输出结果-独立检测
typedef struct 
{
	s_sSingleErrorInfo sSingleErrorInfo;   //检测结果信息
	UINT         nFlagStudy;   //学习成功0，学习失败1
}s_InnerSingleCheckOutputParam;

//内部输出结果-联合检测
typedef struct
{
	s_UnionErrorInfo sUnionErrorInfo;   //检测结果信息
	UINT         nFlagStudy;   //学习成功0，学习失败1
}s_InnerUnionCheckOutputParam;

//halcon异常抛出声明
void inline MyHalconExceptionHandler(const HException& hException)
{
	throw hException;
}
//////////////////////////////////////////////////////////////////////////
// 内部检测算法实现类
class CInnerCheck 
{
public:
	CInnerCheck();
	virtual ~CInnerCheck();

	CString  m_strTaskPath;                //任务路径
	CString  m_strParaIni;                 //参数配置文件路径
	CString  m_strDefaultParaIni;          //默认参数配置文件路径
	CString  m_strRegionPath;              //区域路径
	CString  m_strAlgName;                 //算法名
	CString  m_strAlgPublicLib;            //算法公用库路径
	CString  m_strDefaultParaPath;         //算法默认参数路径
	CString  m_strLogPath;                 //日志记录路径
	s_AlgInitParam   m_sAlgInitParam;      //全局算法路径
    s_ModelRegionPro m_sDstRegionProperty; //区域信息
	
	CFileOperate *m_FileOperate;              //文件操作
	BOOL m_bAllowModify; //是否为模板图像
	int m_nChannel;     //图像颜色通道

	BOOL m_bFileExist; //文件是否存在

	HObject m_hModelImg;//含大小模板
    HObject m_hModelReg;

    HObject m_hShowReg;
    HObject m_hErrorReg;

    HTuple m_hMathId;//模板匹配Id
    HObject m_RegionDetection;//实际检测区域
    std::list<s_ModelRegionPro> *m_regions;//存储需要检测的区域

    HTuple m_htHalconHandle;
    //参数
	HTuple m_htResolution;

	HTuple m_htParamProp;
	HTuple m_htParamName;
	HTuple m_htParamVaule;
	HTuple m_htParamMeaning;
	
	//外部显示用
	HTuple m_htParamType;
	HTuple m_htParamMinVaule;
	HTuple m_htParamMaxVaule;
	HTuple m_htParamStep; 
	HTuple m_htParamChildNames;

	//数组
	HTuple m_htParam;

public:
	
	//初始化函数
	s_Status Init(const CString strTaskName, const s_AlgInitParam &sAlgInitParam,\
		const s_ModelRegionPro &sDstRegionProperty,BOOL bIsNewRegion);
	
	// 释放函数
	s_Status Free();

    
    // 检测函数
	s_Status SingleCheck(const s_InnerCheckInputParam& sAlgInnerCheckInputParam, const HObject hImgSrc,\
		s_InnerSingleCheckOutputParam *sAlgInnerCheckOutputResult);
	
	//保存检测模板函数
	s_Status SaveCheckModel();

	//复制区域
	s_Status AlgCopyRegion(const s_InputTimeImage& sAlgImageInfo,
		s_ModelRegionPro &sDstRegionProperty,const s_ModelRegionPro &sSrcRegionProperty);

	//生成用于检测的HALCON图像
	s_Status GenCheckImage(const s_InnerCheckInputParam& sAlgInnerCheckInputParam, const HObject hImgSrc, HObject& hCheckImage);

    //创建模板匹配模板
    s_Status GenMatchTem(HObject &hv_ImgSrcBig, HObject& hv_ShowReg);

public:
	
	// 写参数文件
	void WritePara();

	// 读参数文件
	void ReadPara();

	// 保存已经修改区域的临时文件 
	int SaveTempPara();

	// 读取已经修改区域的临时文件 	
	int ReadTempPara();
    
	// 申请内存空间
	void RequistSpace();
	
	// 释放内存空间
	void FreeTMPSpace();

	//读模板区域文件
	void ReadModelRegion(CString strRegionPath,const s_ModelRegionPro &sDstRegionProperty);

    void getDetectionRegion();
	
	// Short Description: 自动建模 
	int AutoCreateAlgMode(HObject ho_hModelImg, HObject *ho_hShowReg, const HTuple hv_htParamVaule,
		const HTuple hv_htResolution, HTuple *hv_htParamVauleNew, s_Status &CreateOrLearnState);
	
	// Short Description: 自动学习
	int AutoLearnAlgMode( HObject ho_hCheckImg, s_Status &CreateOrLearnState);
};
#endif // !defined(AFX_CINNERCHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_)


