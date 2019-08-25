/************************************************************************
/* Copyright (c) 2015, 北京大恒图像视觉有限公司印钞事业部    
/* All rights reserved.             
/*                  
/* 文件名称：  AlgStitchCheck.h
/* 简要说明：  检测算法库接口类和算法属性设置接口类。
/*
/* 创建者：  邓红丽
/* 版本：    V1.0
/* 修改人：  田仁赞
/* 版本：    V2.0
/* 修改时间：2015-06-04 
/* 修改记录：1. 格式化框架代码，增强可读性
      2. 将CreateDHAlgStitchCheck()修改成CreateDHAlgCheck()
      3. 将CreateDHAlgStitchCheckPROPDLG()修改成CreateDHAlgCheckPROPDLG()
	  说明：作2和3的调整，避免修改工程名时将该处进行修改
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
// 检测算法库接口类
class DLL_EXT CDHAlgStitchCheck : public CAlgVirtualCheck
{
public:
	
	//构造函数和析构函数
	CDHAlgStitchCheck();
	virtual ~CDHAlgStitchCheck();

	//初始化函数
	s_Status Init(const s_AlgInitParam& sAlgLayerInitParam, int nAlgTypID, s_TypeCheckParam* psCheckParams);

	//加载任务调用
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
	s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult) ;

	//独立定位模块检测纯虚函数
	s_Status Check(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_MiddleResult* sMiddleResult,
		s_AlgCheckOutputParam *sAlgCheckOutputResult) ;

	//Join检测纯虚函数
	s_Status JoinCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult) ;

	//MutiCam检测纯虚函数
	s_Status MutiCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParall,
		s_AlgCheckOutputParam *sAlgCheckOutputResult) ;

	//更新检测函数
	s_Status UpdateParam();

	//保存检测模板函数
	s_Status SaveCheckModel();
public:
	CAlgVirtualCheck *m_pAlgStitchCheck;
	s_Status m_sStatus;

};

//////////////////////////////////////////////////////////////////////////
// 算法属性设置接口类
class DLL_EXT CDHAlgStitchCheckPROPDLG : public CAlgVirtualPROPDLG
{
public:
	
	// 检测类指针，用于更新对话框上设置的数据
	CDHAlgStitchCheck *m_pChecker;

	//构造函数和析构函数
	CDHAlgStitchCheckPROPDLG();
	virtual ~CDHAlgStitchCheckPROPDLG();

	//初始化纯虚函数
	s_Status Init(CString strTaskName, const s_AlgInitParam& sAlgInitParam) ;

	//设置区域算法属性对话框纯虚函数;
	s_Status SetAlgParamDlg(const s_AlgCheckInputParam& sAlgInputParam, \
		const s_ModelRegionPro& sRegion, BOOL bAllowModify) ;

	//复制区域纯虚函数
	s_Status AlgCopyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
		s_ModelRegionPro& sNewRegion, const s_ModelRegionPro& sSrcRegion) ;

	//修改区域纯虚函数
	s_Status AlgModifyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
		const s_ModelRegionPro& sSrcRegion, const s_ModelRegionPro& sNewRegion) ;

	//删除区域纯虚函数
	s_Status AlgDeleteRegion(const list<s_ModelRegionPro>& lstRegion) ;

	//得到算法类型纯虚函数
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
