/************************************************************************
/* Copyright (c) 2015, 北京大恒图像视觉有限公司印钞事业部    
/* All rights reserved.             
/*                  
/* 文件名称：  AlgStitchCheck.cpp
/* 简要说明：  检测算法库接口类和算法属性设置接口类。
/*
/* 创建者：  邓红丽
/* 版本：    V1.0
/* 修改人：  田仁赞
/* 版本：    V2.0
/* 修改时间：2015-06-04 
/* 修改记录：1. 格式化框架代码，增强可读性
              2. 完善了每个函数的注释说明
/* 修改人：  高丽萍
/* 版本：    V1.2.0.2
/* 修改时间：2017-06-29 
/* 修改记录：1.修改halcon13相关算子  
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
#include "stdafx.h"
#include "AlgStitchCheck.h"
#include "Check.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//	Note!
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//
/////////////////////////////////////////////////////////////////////////////
// CAlgStitchCheckApp
BEGIN_MESSAGE_MAP(CAlgStitchCheckApp, CWinApp)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CAlgStitchCheckApp construction
CAlgStitchCheckApp::CAlgStitchCheckApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}
CAlgStitchCheckApp theApp;

/////////////////////////////////////////////////////////////////////////////
// 检测算法库接口类实现
/////////////////////////////////////////////////////////////////////////////

// 检测算法库接口类构造函数和析构函数
//******************************************************************************//
CDHAlgStitchCheck::CDHAlgStitchCheck()
{
	m_pAlgStitchCheck = new CCheck;
}
CDHAlgStitchCheck::~CDHAlgStitchCheck()
{
	if (m_pAlgStitchCheck != NULL)
	{
		delete m_pAlgStitchCheck;
		m_pAlgStitchCheck = NULL;
	}
}

// 检测类初始化
//******************************************************************************//
//函数:检测类初始化
//参数:
//	sAlgInitParam    输入，全局路径
//  nAlgTypID        输入，算法ID
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgStitchCheck::Init(const s_AlgInitParam &sAlgLayerInitParam,int nAlgTypID, s_TypeCheckParam* psCheckParams)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_sStatus = m_pAlgStitchCheck->Init(sAlgLayerInitParam,nAlgTypID,psCheckParams);
	
	return m_sStatus;
}

// 释放空间 
//******************************************************************************//
//函数:释放空间
//参数: NULL
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgStitchCheck::Free()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	
	m_sStatus = m_pAlgStitchCheck->Free();	
	
	return m_sStatus;
}

//加载任务
//******************************************************************************//
//函数:加载任务
//参数:
//	strTaskName      输入，任务名
//	lstRegion        输入，任务区域结构体链表
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgStitchCheck::LoadTask(const CString strTaskName,const list<s_ModelRegionPro> lstRegion)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	
	m_sStatus = m_pAlgStitchCheck->LoadTask(strTaskName,lstRegion);	
	
	////测试
	/*s_ErrorTpye sErrorTpye;
	s_TypeCheckParam psCheckParams;*/

	/*m_sStatus = m_pAlgStitchCheck->GetAlgParam(sErrorTpye, &psCheckParams);
	m_sStatus = m_pAlgStitchCheck->SetAlgParam(sErrorTpye, psCheckParams);*/

	/*s_ModelRegionPro sSrcRegion;
	sSrcRegion.nROIIndex = 1;
	BOOL bAllowModify;
	bAllowModify = TRUE;
	GetAlgParam(sErrorTpye,&psCheckParams, sSrcRegion, bAllowModify);
	SetAlgParam(sErrorTpye,psCheckParams, sSrcRegion);*/

	return m_sStatus;
}

//获取检测参数
s_Status CDHAlgStitchCheck::GetAlgParam(const s_ErrorTpye sErrorTpye,s_TypeCheckParam  * psCheckParams)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	

	m_sStatus = m_pAlgStitchCheck->GetAlgParam(sErrorTpye,psCheckParams);	

	return m_sStatus;
}

//获取界面参数
s_Status CDHAlgStitchCheck::GetAlgParam(const s_ErrorTpye sErrorTpye, s_TypeCheckParam  * psCheckParams, const s_ModelRegionPro& sSrcRegion, const BOOL bAllowModify)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_sStatus = m_pAlgStitchCheck->GetAlgParam(sErrorTpye, psCheckParams, sSrcRegion, bAllowModify);

	return m_sStatus;
}

//设置检测参数
s_Status CDHAlgStitchCheck::SetAlgParam(const s_ErrorTpye sErrorTpye,const s_TypeCheckParam  sCheckParams)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	

	m_sStatus = m_pAlgStitchCheck->SetAlgParam(sErrorTpye,sCheckParams);	

	return m_sStatus;
}

//设置界面参数
s_Status CDHAlgStitchCheck::SetAlgParam(const s_ErrorTpye sErrorTpye, const s_TypeCheckParam  sCheckParams, const s_ModelRegionPro& sSrcRegion)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_sStatus = m_pAlgStitchCheck->SetAlgParam(sErrorTpye, sCheckParams, sSrcRegion);

	return m_sStatus;
}
//自动建模
s_Status CDHAlgStitchCheck::AutoCreateAlgModel(const s_AlgCheckInputParam& sAlgInputParam, const s_ModelRegionPro& sSrcRegion,
	CString strAlgModeName, s_TypeCheckParam  sCheckParams, BOOL bIsModelImg, s_Status &CreateOrLearnState)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_sStatus = m_pAlgStitchCheck->AutoCreateAlgModel(sAlgInputParam, sSrcRegion,strAlgModeName, sCheckParams, bIsModelImg, CreateOrLearnState);

	return m_sStatus;

}
////设置需要的中间检测结果（如所有的定位结果）,每次检测函数之前调用
//******************************************************************************//
//函数:设置需要的中间检测结果
//参数:
//	sMiddleResult    输入，中间结果结构体
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgStitchCheck::SetMiddleResult(const s_MiddleResult &sMiddleResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	
	m_sStatus = m_pAlgStitchCheck->SetMiddleResult(sMiddleResult);	
		
	return m_sStatus;
}
// 检测函数主函数
//******************************************************************************//
//函数:检测主函数
//参数:
//	sAlgCheckInputParam      输入，检测输入结构体
//	sAlgCheckOutputResult    输出，返回检测结果输出结构体
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgStitchCheck::Check(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParallel,
	s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	try
	{
		m_sStatus = m_pAlgStitchCheck->Check(sAlgCheckInputParam, bParallel, sAlgCheckOutputResult);
	}
	catch (...)
	{
		;// 写异常日志，这里需要添加代码		
	}

	return m_sStatus;
}

//定位算法调用重载检测函数
//******************************************************************************//
//函数:定位算法调用重载检测函数
//参数:
//	sAlgCheckInputParam    输入，检测输入结构体
//	sMiddleResult          输出，返回定位算法结果链表
//  sAlgCheckOutputResult  输出，返回检测结果输出结构体
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgStitchCheck::Check(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParallel,
	s_MiddleResult *sMiddleResult,
	s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	try
	{
		m_sStatus = m_pAlgStitchCheck->Check(sAlgCheckInputParam, bParallel, sMiddleResult, sAlgCheckOutputResult);
	}
	catch (...)
	{
		;// 写异常日志，这里需要添加代码		
	}

	return m_sStatus;
}
//Join检测纯虚函数
s_Status CDHAlgStitchCheck::JoinCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParallel,
	s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	try
	{
		m_sStatus = m_pAlgStitchCheck->JoinCheck(sAlgCheckInputParam, bParallel, sAlgCheckOutputResult);
	}
	catch (...)
	{
		;// 写异常日志，这里需要添加代码		
	}

	return m_sStatus;
}

//MutiCam检测纯虚函数
s_Status CDHAlgStitchCheck::MutiCheck(const s_AlgCheckInputParam& sAlgCheckInputParam, BOOL bParallel,
	s_AlgCheckOutputParam *sAlgCheckOutputResult)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	try
	{
		m_sStatus = m_pAlgStitchCheck->MutiCheck(sAlgCheckInputParam, bParallel,  sAlgCheckOutputResult);
	}
	catch (...)
	{
		;// 写异常日志，这里需要添加代码		
	}

	return m_sStatus;
}
//更新检测参数
//******************************************************************************//
//函数:更新检测参数
//参数:NULL
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgStitchCheck::UpdateParam()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	
	m_sStatus = m_pAlgStitchCheck->UpdateParam();	
	
	return m_sStatus;
}

//保存检测模板
//******************************************************************************//
//函数:保存检测模板
//参数:NULL
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgStitchCheck::SaveCheckModel()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	
	m_sStatus = m_pAlgStitchCheck->SaveCheckModel();	
	
	return m_sStatus;
}

/////////////////////////////////////////////////////////////////////////////
// 算法属性设置接口类实现
CDHAlgStitchCheckPROPDLG::CDHAlgStitchCheckPROPDLG()
{
	m_pPROPDLG = new CSetPROP;
}
CDHAlgStitchCheckPROPDLG::~CDHAlgStitchCheckPROPDLG()
{
	if (m_pPROPDLG != NULL)
	{
		/*CSetPROP *pPROPDLG;
		pPROPDLG = (CSetPROP*)m_pPROPDLG;*/

		delete m_pPROPDLG;
		//pPROPDLG = NULL;
		m_pPROPDLG = NULL;
	}
}

//属性设置初始化
//******************************************************************************//
//函数:属性设置初始化
//参数:
//	strTaskName      输入，任务名
//	sAlgInitParam    输入，全局路径
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgStitchCheckPROPDLG::Init(const CString strTaskName,const s_AlgInitParam& sAlgLayerInitParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_pPROPDLG->pChecker = pChecker;
	m_sStatus = m_pPROPDLG->Init(strTaskName, sAlgLayerInitParam);	
	return m_sStatus;
}

//属性设置主函数
//******************************************************************************//
//函数:属性设置主函数
//参数:
//	sAlgImageInfo     输入，图像结构体   
//	sRegionProperty   输入，区域属性
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgStitchCheckPROPDLG::SetAlgParamDlg(const s_AlgCheckInputParam& sAlgInputParam, \
	const s_ModelRegionPro& sRegion, BOOL bAllowModify)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_sStatus = m_pPROPDLG->SetAlgParamDlg(sAlgInputParam,sRegion,bAllowModify);	
	return m_sStatus;
}

//复制区域
//******************************************************************************//
//函数:复制区域
//参数:
//	sAlgImageInfo        输入，图像结构体   
//	sDstRegionProperty   输出，目的区域属性
//  sSrcRegionProperty   输入，源区域属性
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgStitchCheckPROPDLG::AlgCopyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
	s_ModelRegionPro& sNewRegion, const s_ModelRegionPro& sSrcRegion)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	
	m_sStatus = m_pPROPDLG->AlgCopyRegion(sAlgInputParam,sNewRegion,sSrcRegion);
	
	return m_sStatus;
}

//修改区域大小
//******************************************************************************//
//函数:修改区域大小
//功能：修改区域大小后，调用函数（可重新设置或生成模板等）
//参数:
//	sAlgImageInfo     输入，图像结构体   
//	hNewRegion        输入，区域属性
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgStitchCheckPROPDLG::AlgModifyRegion(const s_AlgCheckInputParam& sAlgInputParam, \
	const s_ModelRegionPro& sSrcRegion, const s_ModelRegionPro& sNewRegion)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_sStatus = m_pPROPDLG->AlgModifyRegion(sAlgInputParam,sSrcRegion,sNewRegion);
	
	return m_sStatus;
}

//删除区域
//******************************************************************************//
//函数:删除区域
//参数:
//	lstRegion     输入，区域属性链表   
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgStitchCheckPROPDLG::AlgDeleteRegion(const list<s_ModelRegionPro>& lstRegion)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_sStatus = m_pPROPDLG->AlgDeleteRegion(lstRegion);
	
	return m_sStatus;
}

//得到算法内部类型
//******************************************************************************//
//函数:得到算法内部类型
//参数:
//	lstInnerAlgType     输出，算法内部支持算法类型信息链表
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CDHAlgStitchCheckPROPDLG::GetAlgorithmInfo(list<s_InnerAlgType>& lstInnerAlgType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());	
	m_sStatus = m_pPROPDLG->GetAlgorithmInfo(lstInnerAlgType);

	return m_sStatus;
}

//动态调用检测类
extern "C" __declspec(dllexport) CDHAlgStitchCheck* CreateDHAlgCheck()
{
	CDHAlgStitchCheck *A;
	A  = new CDHAlgStitchCheck;
	return A;
}

//动态调用属性类
extern "C" __declspec(dllexport) CDHAlgStitchCheckPROPDLG* CreateDHAlgCheckPROPDLG()
{
	CDHAlgStitchCheckPROPDLG *A;
	A  = new CDHAlgStitchCheckPROPDLG;
	return A;
}

//释放检测类和属性类
extern "C" __declspec(dllexport) BOOL FreeAlgCheckDll(CDHAlgStitchCheck *pCheck,
													  CDHAlgStitchCheckPROPDLG *pPropDlg)
{
	try
	{
		if (pCheck)
		{
			delete pCheck;
			pCheck = NULL;
		}
		if (pPropDlg)
		{
			delete pPropDlg;
			pPropDlg = NULL;
		}
	}
	catch (...)
	{
		return TRUE;
	}
	return FALSE;
}
