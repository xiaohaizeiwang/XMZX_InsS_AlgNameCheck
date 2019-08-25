/************************************************************************
/* Copyright (c) 2015, 北京大恒图像视觉有限公司印钞事业部    
/* All rights reserved.             
/*                  
/* 文件名称：  Check.h
/* 简要说明：  检测算法库实现类和算法属性设置实现类。
/*
/* 创建者：  邓红丽
/* 版本：    V1.0
/* 修改人：  田仁赞关于我爱你。
/* 修改记录：1. 格式化框架代码，增强可读性
              2. 添加内部检测链表list<CInnerCheck> m_listInnerCheck
/* 修改人：  高丽萍
/* 版本：    V1.2.0.2
/* 修改时间：2017-06-29 
/* 修改记录：1.halcon13全面升级  
      2. 修改初始化函数，增加一个检测参数类的输出
      3. 增加获取参数函数GetAlgParam(const s_ErrorTpye sErrorTpye,s_TypeCheckParam  * psCheckParams)
	  4. 增加设置参数函数SetAlgParam(const s_ErrorTpye sErrorTpye,const s_TypeCheckParam  sCheckParams)

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

#if !defined(AFX_CHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_)
#define AFX_CHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InnerCheck.h"
#include "FileOperate.h"
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

//////////////////////////////////////////////////////////////////////////
// 检测算法库实现类
class CCheck : public CAlgVirtualCheck 
{
public:
	CCheck();
	virtual ~CCheck();

	s_Status m_sStatus;             // 执行状态
	CString  m_strTaskPath;         // 任务全路径
	CString  m_strTaskName;         // 任务名
	CString  m_strDefaultParaIni;   // 全局默认参数路径
	CString  m_strAlgName;          // 算法名
    s_MiddleResult m_sMiddleResult; // 中间结果
	s_AlgInitParam m_sAlgInitParam; // 全局算法路径

	std::list<CInnerCheck> m_listInnerCheck; // 生成的检测链表
	CFileOperate           m_FileOperate;    //文件操作
	CString                m_strLogPath;     //日志路径
	
	//int m_nRegionNum;     //区域个数
	//int m_nStudyRegNum;   //每大张学习成功区域数
	//int m_nAlgTypID;      //算法ID
	//s_AlgImageInfo m_sAlgImageInfo;//算法检测需要的图像信息

	HTuple m_htParamProp;   //参数属性 1-属性参数，0-检测参数
	HTuple m_htParamName;   //参数名称
	HTuple m_htParamVaule;  //参数大小
	HTuple m_htParamMeaning;//参数含义

	//显示用
	HTuple m_htParamType;     //参数类型 0-bool 1-int 2-double
	HTuple m_htParamMinVaule; //参数最小值
	HTuple m_htParamMaxVaule; //参数最大值
	HTuple m_htParamStep;     //参数步长
	HTuple m_htParamChildNames;//多子参数名

public:
	//初始化函数
	s_Status Init(const s_AlgInitParam& sAlgLayerInitParam, int nAlgTypID, s_TypeCheckParam* psCheckParams);

	//加载任务函数
	s_Status LoadTask(const CString strTaskName, const list<s_ModelRegionPro> lstRegion);

	//获取检测参数
	s_Status GetAlgParam(const s_ErrorTpye sErrorTpye,s_TypeCheckParam  * psCheckParams);

	//建模界面获取参数
	s_Status GetAlgParam(const s_ErrorTpye sErrorTpye, s_TypeCheckParam  * psCheckParams, const s_ModelRegionPro& sSrcRegion, const BOOL bAllowModify);

	//设置检测参数
	s_Status SetAlgParam(const s_ErrorTpye sErrorTpye,const s_TypeCheckParam  sCheckParams);
	
	//设置建模界面参数
	s_Status SetAlgParam(const s_ErrorTpye sErrorTpye, const s_TypeCheckParam  sCheckParams, const s_ModelRegionPro& sSrcRegion);

	//自动建模
	s_Status AutoCreateAlgModel(const s_AlgCheckInputParam& sAlgInputParam,
		const s_ModelRegionPro& sSrcRegion,
		CString strAlgModeName,
		s_TypeCheckParam  sCheckParams,
		BOOL bIsModelImg,
		s_Status &CreateOrLearnState);

	//释放函数
	s_Status Free();

	//设置需要的中间检测结果（如所有的定位结果）,每次检测函数之前调用
	s_Status SetMiddleResult(const s_MiddleResult& sMiddleResult);

	//独立检测纯虚函数
	s_Status Check(const s_AlgCheckInputParam& sAlgSingleCheckInputParam, BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult);

	//独立定位模块检测纯虚函数
	s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_MiddleResult* sMiddleResult,
		s_AlgCheckOutputParam *sAlgCheckOutputResult);

	//Join检测纯虚函数
	s_Status JoinCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult);

	//MutiCam检测纯虚函数
	s_Status MutiCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult);
	//更新检测参数函数
	s_Status UpdateParam();

	//保存检测模板函数
	s_Status SaveCheckModel();
};

//////////////////////////////////////////////////////////////////////////
// 算法属性设置实现类
class CSetPROP :public CAlgVirtualPROPDLG
{
public:
	CSetPROP();
	virtual ~CSetPROP();
	s_Status m_sStatus;              //执行状态
	CString  m_strTaskName;          //任务名
	CString  m_strAlgName;           //算法名
	CFileOperate    m_FileOperate;   //文件操作
	s_AlgInitParam  m_sAlgInitParam; //全局算法路径
	CCheck          *m_pChecker;     //检测类对象指针
    std::list<s_ModelRegionPro> m_regions;//存储需要检测的区域

public:
	//初始化
	s_Status Init(CString strTaskName, const s_AlgInitParam& sAlgInitParam);

	//设置区域算法属性对话框纯虚函数;
	s_Status SetAlgParamDlg(const s_AlgCheckInputParam& sAlgInputParam, \
		const s_ModelRegionPro& sRegion, BOOL bAllowModify);

	//复制区域
	s_Status AlgCopyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
		s_ModelRegionPro& sNewRegion, const s_ModelRegionPro& sSrcRegion);

	//修改区域
	s_Status AlgModifyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
		const s_ModelRegionPro& sSrcRegion, const s_ModelRegionPro& sNewRegion);

	//删除区域
	s_Status AlgDeleteRegion(const list<s_ModelRegionPro>& lstRegion);

	//得到算法类型
	s_Status GetAlgorithmInfo(list<s_InnerAlgType>& lstInnerAlgType);
};
#endif // !defined(AFX_CHECK_H__5E5A2CA4_AD34_48EC_A361_7AD2563B9366__INCLUDED_)


