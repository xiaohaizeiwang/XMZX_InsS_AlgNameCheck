/************************************************************************
/* Copyright (c) 2008, 北京大恒图像视觉有限公司视觉算法部    
/* All rights reserved.             
/*                  
/* 文件名称：  PropDlg.h
/* 摘要： 属性对话框。
/*
/* 当前
/* 创建者：   邓红丽
/* 版本：     1.0
/* 完成日期： 2010年8月8日
/* 说明：     属性对话框中，修改参数、保存参数、参数分析和实现函数
/* 修改人：   田仁赞
/* 版本：     V2.0
/* 修改时间： 2015-06-04 
/* 修改记录： 1. 格式化框架代码，增强可读性
        2. 删除原先的图像显示方法，更换成HALCON方法进行图像显示
		3. 修改GetHWindowID()函数，添加传出图像控件宽高的结果
/* 修改人：  高丽萍
/* 版本：    V1.2.0.2
/* 修改时间：2017-06-29 
/* 修改记录：修改halcon13相关算子 

/* 修改人：  张波
/* 版本：    V3.0
/* 修改时间：2018-11-07
/* 修改记录：1、参数控件改为BCG链表形式、新加参数无需添加控件
			 2、添加自动建模函数，适应halcon例程直接导入
************************************************************************/

#if !defined(AFX_PROPDLG_H__8D2BD3EF_2765_4084_BF87_9E390245E887__INCLUDED_)
#define AFX_PROPDLG_H__8D2BD3EF_2765_4084_BF87_9E390245E887__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Check.h"
#include "MyEditListCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"

/////////////////////////////////////////////////////////////////////////////

class CPropDlg : public CBCGPDialog/*CDialog*/
{
public:
	CPropDlg(CWnd* pParent = NULL);  

	enum { IDD = IDD_DIALOG_PROP };
	
	s_InputTimeImage   m_sAlgImageInfo;       // 图像信息
	s_ModelRegionPro m_sDstRegionProperty;  // 区域信息
	s_AlgInitParam   m_sAlgInitParam;       // 全局路径
	
	CCheck           *m_pChecker;           // 外部指针	
	CInnerCheck      *m_pTempChecker;       // 内部指针，临时测试用
    std::list<s_ModelRegionPro> *m_regions;//存储需要检测的区域
	
	int         m_nRangV;          // 滚动条垂直范围
	int         m_nRangH;          // 滚动条水平范围
	int         m_nCurPosx;        // 起始点
	int         m_nCurPosy;        // 终止点
	int         m_nWindowHeight;   // 窗口高度
	int         m_nWindowWidth;    // 窗口宽度
	float	    m_fScaleX;         // 显示比率x
	float	    m_fScaleY;         // 显示比率y
	Hlong       m_lHWindowID;      // HALCON窗口ID
	Hlong       m_lImgWid;         // 图像宽度
	Hlong       m_lImgHei;         // 图像高度
	HObject     m_hImgSrcBig;         // 检测大图
	HObject     m_hImgSrc;         // 待处理的Halcon图像
    CString   	m_strCheckResult;  // 检测结果

	CScrollBar	m_scrollVer;       //垂直滚动条的控制变量
	CScrollBar	m_scrollHor;       //水平滚动条的控制变量
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	//{{AFX_MSG(CPropDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnButtonTest();
	afx_msg void OnButtonSave();
	afx_msg void OnRadioAuto();
	afx_msg void OnRadioNorm();
	afx_msg void OnRadioSetrate();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKillfocusEditShowrateX();
	afx_msg void OnKillfocusEditShowrateY();
	afx_msg void OnButtonReaddefault();
	afx_msg void OnButtonWritedefault();
	//}}AFX_MSG
	afx_msg LRESULT UpdatadataParam(WPARAM wParam, LPARAM LParam);
	DECLARE_MESSAGE_MAP()
public:
	// 获得控件大小
	void GetDlgItemRect(int nID,CWnd *wndParent,LPRECT Client);

	// 获得HALCON的窗口ID和窗口的宽高
	bool GetHWindowID(int nControlID, int& nDlgWidth, int& nDlgHeight, Hlong& lHWindowID);

	afx_msg void OnBnClickedButtonAtuocreatmodel();
	//int m_nChannel;
	BOOL m_bAllowModify;
	//BOOL m_bModelTest;
	BOOL m_bChangePropParam;
    BOOL m_bMatchTem;
	HObject m_hShowReg;
	HObject m_hErrorReg;

	//参数
	HTuple m_htResolution;

	HTuple m_htParamProp;
	HTuple m_htParamName;
	HTuple m_htParamVaule;
	HTuple m_htParamMeaning;
	int m_nParamNum;
    HTuple m_htParamType;
	HTuple m_htParamChildNames;

	//BCG
	CBCGPPropList  m_wndPropList;
	CBCGPProp*	   m_pGroupGeneral;
	BOOL        m_bPropertyModify;
	bool InitListBCGControl();
	int InitCommand();
	int ClearPropertyCtl();
	int AddPropertyData();
	int PropertyCommitModify();

	CMyEditListCtrl m_lstParam;
	bool InitLstControl();
	bool ShowLstControlContent();
	

	CBCGPStatic m_wndPropListLocation;
	afx_msg LRESULT OnPropertyCommandClicked(WPARAM, LPARAM);
	afx_msg LRESULT OnPropChanged(WPARAM uProListID, LPARAM pPro);
    afx_msg void OnBnClickedGenModelMatch();
};
#endif // !defined(AFX_PROPDLG_H__8D2BD3EF_2765_4084_BF87_9E390245E887__INCLUDED_)
