/************************************************************************
/* Copyright (c) 2008, �������ͼ���Ӿ����޹�˾�Ӿ��㷨��    
/* All rights reserved.             
/*                  
/* �ļ����ƣ�  PropDlg.h
/* ժҪ�� ���ԶԻ���
/*
/* ��ǰ
/* �����ߣ�   �˺���
/* �汾��     1.0
/* ������ڣ� 2010��8��8��
/* ˵����     ���ԶԻ����У��޸Ĳ������������������������ʵ�ֺ���
/* �޸��ˣ�   ������
/* �汾��     V2.0
/* �޸�ʱ�䣺 2015-06-04 
/* �޸ļ�¼�� 1. ��ʽ����ܴ��룬��ǿ�ɶ���
        2. ɾ��ԭ�ȵ�ͼ����ʾ������������HALCON��������ͼ����ʾ
		3. �޸�GetHWindowID()��������Ӵ���ͼ��ؼ���ߵĽ��
/* �޸��ˣ�  ����Ƽ
/* �汾��    V1.2.0.2
/* �޸�ʱ�䣺2017-06-29 
/* �޸ļ�¼���޸�halcon13������� 

/* �޸��ˣ�  �Ų�
/* �汾��    V3.0
/* �޸�ʱ�䣺2018-11-07
/* �޸ļ�¼��1�������ؼ���ΪBCG������ʽ���¼Ӳ���������ӿؼ�
			 2������Զ���ģ��������Ӧhalcon����ֱ�ӵ���
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
	
	s_InputTimeImage   m_sAlgImageInfo;       // ͼ����Ϣ
	s_ModelRegionPro m_sDstRegionProperty;  // ������Ϣ
	s_AlgInitParam   m_sAlgInitParam;       // ȫ��·��
	
	CCheck           *m_pChecker;           // �ⲿָ��	
	CInnerCheck      *m_pTempChecker;       // �ڲ�ָ�룬��ʱ������
    std::list<s_ModelRegionPro> *m_regions;//�洢��Ҫ��������
	
	int         m_nRangV;          // ��������ֱ��Χ
	int         m_nRangH;          // ������ˮƽ��Χ
	int         m_nCurPosx;        // ��ʼ��
	int         m_nCurPosy;        // ��ֹ��
	int         m_nWindowHeight;   // ���ڸ߶�
	int         m_nWindowWidth;    // ���ڿ��
	float	    m_fScaleX;         // ��ʾ����x
	float	    m_fScaleY;         // ��ʾ����y
	Hlong       m_lHWindowID;      // HALCON����ID
	Hlong       m_lImgWid;         // ͼ����
	Hlong       m_lImgHei;         // ͼ��߶�
	HObject     m_hImgSrcBig;         // ����ͼ
	HObject     m_hImgSrc;         // �������Halconͼ��
    CString   	m_strCheckResult;  // �����

	CScrollBar	m_scrollVer;       //��ֱ�������Ŀ��Ʊ���
	CScrollBar	m_scrollHor;       //ˮƽ�������Ŀ��Ʊ���
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
	// ��ÿؼ���С
	void GetDlgItemRect(int nID,CWnd *wndParent,LPRECT Client);

	// ���HALCON�Ĵ���ID�ʹ��ڵĿ��
	bool GetHWindowID(int nControlID, int& nDlgWidth, int& nDlgHeight, Hlong& lHWindowID);

	afx_msg void OnBnClickedButtonAtuocreatmodel();
	//int m_nChannel;
	BOOL m_bAllowModify;
	//BOOL m_bModelTest;
	BOOL m_bChangePropParam;
    BOOL m_bMatchTem;
	HObject m_hShowReg;
	HObject m_hErrorReg;

	//����
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
