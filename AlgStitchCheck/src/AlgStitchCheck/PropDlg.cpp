// PropDlg.cpp : implementation file
//
/************************************************************************
/* Copyright (c) 2008, 北京大恒图像视觉有限公司视觉算法部
/* All rights reserved.
/*
/* 文件名称：  PropDlg.cpp
/* 摘要： 属性对话框。
/*
/* 当前版本： 1.0
/* 创建者：   邓红丽
/* 完成日期： 2010年8月8日
/* 说明：     属性对话框中，修改参数、保存参数、参数分析和实现函数
/* 修改人：   田仁赞
/* 版本：     V2.0
/* 修改时间： 2015-06-04
/* 修改记录： 1. 格式化框架代码，增强可读性。
		2. 根据HALCON显示图像的方法，修改了OnRadioAuto()、OnRadioNorm()
		   以及OnRadioSetrate()函数中的显示操作代码。
		3. 对话框初始化函数OnInitDialog()中已添加按区域信息裁切出待测
		   区域图像，并将其显示在图像控件中。
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
#include "stdafx.h"
#include "AlgStitchCheck.h"
#include "PropDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
HWND g_hMessageHwnd;
/////////////////////////////////////////////////////////////////////////////
CPropDlg::CPropDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(CPropDlg::IDD, pParent)
	/*, m_nChannel(0)*/
{
	EnableVisualManagerStyle(TRUE, TRUE);
	m_fScaleX = 1.0;
	m_fScaleY = 1.0;
	m_nRangV = 0;
	m_nRangH = 0;
	m_nCurPosx = 0;
	m_nCurPosy = 0;
	m_nWindowHeight = 0;
	m_nWindowWidth = 0;
	m_lHWindowID = -1;
	m_lImgWid = 0;
	m_lImgHei = 0;
	GenEmptyObj(&m_hImgSrcBig);
	GenEmptyObj(&m_hImgSrc);
	GenEmptyObj(&m_hShowReg);
	GenEmptyObj(&m_hErrorReg);

	TupleGenConst(0, 0, &m_htResolution);

	TupleGenConst(0, 0, &m_htParamProp);
	TupleGenConst(0, 0, &m_htParamName);
	TupleGenConst(0, 0, &m_htParamVaule);
	TupleGenConst(0, 0, &m_htParamMeaning);
	TupleGenConst(0, 0, &m_htParamType);
	TupleGenConst(0, 0, &m_htParamChildNames);

	m_nParamNum = 0;
	m_strCheckResult = _T("");

	m_pChecker = NULL;
	m_pTempChecker = NULL;

	m_bAllowModify = FALSE;
	m_bChangePropParam = FALSE;
    m_bMatchTem = FALSE;
	//m_bModelTest = FALSE;
	m_pGroupGeneral = FALSE;
}
//字符串固定分割
CString * SplitString1(CString str, char split, int& iSubStrs)
{
	int iPos = 0; //分割符位置
	int iNums = 0; //分割符的总数
	CString strTemp = str;
	CString strRight;
	//先计算子字符串的数量
	while (iPos != -1)
	{
		iPos = strTemp.Find(split);
		if (iPos == -1)
		{
			break;
		}
		strRight = strTemp.Mid(iPos + 1, str.GetLength());
		strTemp = strRight;
		iNums++;
	}
	if (iNums == 0) //没有找到分割符
	{
		//子字符串数就是字符串本身
		iSubStrs = 1;
		return NULL;
	}
	//子字符串数组
	iSubStrs = iNums + 1; //子串的数量 = 分割符数量 + 1
	CString* pStrSplit;
	pStrSplit = new CString[iSubStrs];
	strTemp = str;
	CString strLeft;
	for (int i = 0; i < iNums; i++)
	{
		iPos = strTemp.Find(split);
		//左子串
		strLeft = strTemp.Left(iPos);
		//右子串
		strRight = strTemp.Mid(iPos + 1, strTemp.GetLength());
		strTemp = strRight;
		pStrSplit[i] = strLeft;
	}
	pStrSplit[iNums] = strTemp;
	return pStrSplit;
}

void CPropDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropDlg)
	DDX_Control(pDX, IDC_SCROLLBAR_VER, m_scrollVer);
	DDX_Control(pDX, IDC_SCROLLBAR_HOR, m_scrollHor);
	DDX_Text(pDX, IDC_STATIC_CHECKRESULT, m_strCheckResult);
	DDX_Text(pDX, IDC_EDIT_SHOWRATE_X, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT_SHOWRATE_Y, m_fScaleY);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_PARAM, m_lstParam);
	DDX_Control(pDX, IDC_PARAMLIST, m_wndPropListLocation);
}

BEGIN_MESSAGE_MAP(CPropDlg, CBCGPDialog)
	//{{AFX_MSG_MAP(CPropDlg)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_RADIO_AUTO, OnRadioAuto)
	ON_BN_CLICKED(IDC_RADIO_NORM, OnRadioNorm)
	ON_BN_CLICKED(IDC_RADIO_SETRATE, OnRadioSetrate)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_EN_KILLFOCUS(IDC_EDIT_SHOWRATE_X, OnKillfocusEditShowrateX)
	ON_EN_KILLFOCUS(IDC_EDIT_SHOWRATE_Y, OnKillfocusEditShowrateY)
	ON_BN_CLICKED(IDC_BUTTON_READDEFAULT, OnButtonReaddefault)
	ON_BN_CLICKED(IDC_BUTTON_WRITEDEFAULT, OnButtonWritedefault)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ATUOCREATMODEL, &CPropDlg::OnBnClickedButtonAtuocreatmodel)
	ON_MESSAGE(WM_UPDATADATA, UpdatadataParam)

	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_COMMAND_CLICKED, OnPropertyCommandClicked)
	ON_REGISTERED_MESSAGE(BCGM_PROPERTY_CHANGED, OnPropChanged)
    ON_BN_CLICKED(IDC_GEN_MODEL_MATCH, &CPropDlg::OnBnClickedGenModelMatch)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropDlg message handlers

void CPropDlg::OnOK()
{
	// TODO: Add extra validation here

	CBCGPDialog::OnOK();
}

// 画图
void CPropDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	ClearWindow(m_lHWindowID);
	HObject hImgShow;
	HTuple htChannelNum;
	int nChannelNum;
	CountObj(m_hImgSrc, &htChannelNum);
	nChannelNum = htChannelNum[0].I();
	if (nChannelNum == 3)
	{
		HObject hImageR, hImageG, hImageB;
		SelectObj(m_hImgSrc, &hImageR, 1);
		SelectObj(m_hImgSrc, &hImageG, 2);
		SelectObj(m_hImgSrc, &hImageB, 3);
		Compose3(hImageR, hImageG, hImageB, &hImgShow);
	}
	else if (nChannelNum == 1)
	{
		MirrorImage(m_hImgSrc, &hImgShow, "row");
	}

	DispObj(hImgShow, m_lHWindowID);
	SetColor(m_lHWindowID, "green");
	HObject hShowReg;
	MirrorRegion(m_hShowReg, &hShowReg, "row", m_lImgHei);
	DispObj(hShowReg, m_lHWindowID);

	SetColor(m_lHWindowID, "red");
	HObject hErrorReg;
	MirrorRegion(m_hErrorReg, &hErrorReg, "row", m_lImgHei);
	DispObj(hErrorReg, m_lHWindowID);
}

// 对话框初始化
BOOL CPropDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	try
	{
		//int cx, cy;
		//cx = GetSystemMetrics(SM_CXSCREEN);
		//cy = GetSystemMetrics(SM_CYSCREEN);
		////再用MoveWindow
		//CRect rcTemp;
		//rcTemp.BottomRight() = CPoint(cx, cy);
		//rcTemp.TopLeft() = CPoint(0, 0);
		//MoveWindow(&rcTemp);

		CheckRadioButton(IDC_RADIO_AUTO, //第一个参数为该组的第一个单选按钮的ID  		
			IDC_RADIO_SETRATE,           //第二个参数为该组的最后一个单选按钮的ID  		
			IDC_RADIO_AUTO);

		//获取HALCON窗口ID
		GetHWindowID(IDC_PIC, m_nWindowWidth, m_nWindowHeight, m_lHWindowID);

		//获取HALCON图像
		HTuple hv_ImgWid, hv_ImgHei;
		//CopyObj(*m_sAlgImageInfo.p3DHImg, &m_hImgSrcBig, 1, -1);

		//// 临时测试 [9/12/2018 zb]
		//CString strPath;
		//const char * cPath;
		//strPath = _T("D:/InspectionSystem/测试图/0925-006/Camera0/1.bmp");
		////cPath =  (char*)strPath.GetBuffer(0);
		//CStringA strPathAEn(strPath);
		//cPath = strPathAEn.GetString();
		//ReadImage(&m_hImgSrcBig, (HTuple)cPath);
		//GetDomain(m_hImgSrcBig, &m_sDstRegionProperty.hRegion);

		//提取区域图像
		
        /*HObject hv_X, hv_Y, hv_Z;
        Decompose3(m_hImgSrcBig, &hv_X, &hv_Y, &hv_Z);*/
        HObject HImg;

        ReduceDomain(m_hImgSrcBig, m_sDstRegionProperty.hRegion, &m_hImgSrc);
		//CropDomain(HImg, &m_hImgSrc);
        
		GetImageSize(m_hImgSrc, &hv_ImgWid, &hv_ImgHei);
		m_lImgWid = hv_ImgWid[0].L();
		m_lImgHei = hv_ImgHei[0].L();
		//显示图像
		OnRadioAuto();

		m_pTempChecker = new CInnerCheck;
		m_pTempChecker->m_strLogPath = m_pChecker->m_strLogPath;
		m_pTempChecker->m_FileOperate = &m_pChecker->m_FileOperate;
		m_pTempChecker->Init(m_pChecker->m_strTaskName, m_pChecker->m_sAlgInitParam, m_sDstRegionProperty, TRUE);

		m_htParamProp = m_pTempChecker->m_htParamProp;
		m_htParamName = m_pTempChecker->m_htParamName;
		m_htParamMeaning = m_pTempChecker->m_htParamMeaning;
		m_htParamVaule = m_pTempChecker->m_htParamVaule;
		m_htParamType = m_pTempChecker->m_htParamType;
		m_htParamChildNames = m_pTempChecker->m_htParamChildNames;

		m_htResolution = m_pTempChecker->m_htResolution;

		HTuple htParamNum;
		TupleLength(m_htParamProp, &htParamNum);
		m_nParamNum = htParamNum[0].I();
		//BCG
		InitListBCGControl();
		int ret = AddPropertyData();

		// 参数链表初始化
		//InitLstControl();
		//ShowLstControlContent();
		//g_hMessageHwnd = m_hWnd;

		if (!m_bAllowModify)
		{
			GetDlgItem(IDC_BUTTON_ATUOCREATMODEL)->EnableWindow(FALSE);
		}

		UpdateData(FALSE);
		return TRUE;
	}
	catch (HException &hException)
	{
		CString strError;
		strError.Format(_T("初始化失败！,halcon- "));
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		AfxMessageBox(strError);
		return FALSE;
	}
	catch (...)
	{
		AfxMessageBox(_T("初始化失败！"));
		return FALSE;
	}
}
bool CPropDlg::GetHWindowID(int nControlID, int& nDlgWidth, int& nDlgHeight, Hlong& lHWindowID)
{
	CRect rect;
	CWnd  *pWnd;
	Hlong lWndID;
	HTuple hv_HWindowID;
	lWndID = (Hlong)GetDlgItem(nControlID)->m_hWnd;

	pWnd = GetDlgItem(nControlID);
	pWnd->GetClientRect(&rect);
	pWnd->MapWindowPoints(this, &rect);

	nDlgWidth = rect.Width();
	nDlgHeight = rect.Height();

	SetWindowAttr("border_width", 0);
	SetCheck("~father");
	OpenWindow(0, 0, nDlgWidth, nDlgHeight, lWndID, "visible", "", &hv_HWindowID);
	lHWindowID = hv_HWindowID[0].L();
	SetWindowType("WIN32-Window");
	SetCheck("father");
	SetPart(lHWindowID, 0, 0, nDlgHeight - 1, nDlgWidth - 1);
	SetPartStyle(lHWindowID, 0);
	SetDraw(lHWindowID, "margin");
	return true;
}
// 对话框退出释放空间
void CPropDlg::OnDestroy()
{
	CBCGPDialog::OnDestroy();

	if (m_pTempChecker)
	{
		m_pTempChecker->Free();
		delete m_pTempChecker;
		m_pTempChecker = NULL;
	}
}

// 检测
void CPropDlg::OnButtonTest()
{
	if (m_bChangePropParam)
	{
		AfxMessageBox(_T("属性参数修改，请先创建模板！"));
		return;
	}

	s_InnerCheckInputParam sAlgInnerCheckInputParam;
	sAlgInnerCheckInputParam.sInputTimeImage = m_sAlgImageInfo;
	sAlgInnerCheckInputParam.sRegionProperty = m_sDstRegionProperty;

	s_InnerSingleCheckOutputParam sInnerSingleCheckOutputResult;
	m_pTempChecker->m_bAllowModify = m_bAllowModify;
	
	m_pTempChecker->ReadModelRegion(m_pTempChecker->m_strRegionPath, m_sDstRegionProperty);
	if (m_pTempChecker->m_bFileExist == FALSE)
	{
		AfxMessageBox(_T("缺少模板，请创建模板"));
		return;
	}
	m_pTempChecker->SingleCheck(sAlgInnerCheckInputParam, m_hImgSrcBig, &sInnerSingleCheckOutputResult);

	m_hShowReg = m_pTempChecker->m_hShowReg;
	m_hErrorReg = m_pTempChecker->m_hErrorReg;
	CString strResultCode;
	strResultCode = sInnerSingleCheckOutputResult.sSingleErrorInfo.strCheckResultCode;

	m_strCheckResult.Format(_T("\n1. 均值 = %d , 方差 = %d \n"),
		sInnerSingleCheckOutputResult.sSingleErrorInfo.meanValue,
		sInnerSingleCheckOutputResult.sSingleErrorInfo.DeviationValue);
	UpdateData(FALSE);
	OnPaint();
}

// 保存
void CPropDlg::OnButtonSave()
{
	if (m_bChangePropParam)
	{
		AfxMessageBox(_T("属性参数修改，请先创建模板！"));
		return;
	}
    if (1==m_sDstRegionProperty.nROIIndex && !m_bMatchTem)
    {
        AfxMessageBox(_T("匹配模板尚未创建！"));
        return;
    }
	m_pTempChecker->WritePara();
	OnOK();
}

// 合适缩放显示
void CPropDlg::OnRadioAuto() // 合适时，全屏缩放显示
{
	// TODO: Add your control notification handler code here
	m_nRangH = 0;
	m_nRangV = 0;
	m_nCurPosy = 0;
	m_nCurPosx = 0;
	m_fScaleX = (float)((m_nWindowWidth) / m_lImgWid);
	m_fScaleY = (float)((m_nWindowHeight) / m_lImgHei);
	UpdateData(FALSE);

	// 屏蔽滚动条
	GetDlgItem(IDC_SCROLLBAR_HOR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SCROLLBAR_VER)->ShowWindow(SW_HIDE);

	// 显示比率
	GetDlgItem(IDC_EDIT_SHOWRATE_X)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SHOWRATE_Y)->EnableWindow(FALSE);

	SetPart(m_lHWindowID, 0, 0, m_lImgHei - 1, m_lImgWid - 1);

	CRect rect;
	GetDlgItemRect(IDC_PIC, this, &rect);
	InvalidateRect(rect, TRUE);// 清屏	
}

// 原始显示
void CPropDlg::OnRadioNorm()
{
	// TODO: Add your control notification handler code here
	m_nRangH = 0;
	m_nRangV = 0;
	m_nCurPosy = 0;
	m_nCurPosx = 0;
	m_fScaleX = (float)1.0;
	m_fScaleY = (float)1.0;
	UpdateData(FALSE);

	// 判断是否应该存在滚动条
	BOOL bHorFlag = FALSE;// 横滚动条是否存在
	BOOL bVerFlag = FALSE;// 竖滚动条是否存在
	if (m_nWindowHeight < m_lImgHei)
	{
		bVerFlag = TRUE;
		m_nRangV = (int)m_lImgHei - m_nWindowHeight;

		m_scrollVer.SetScrollRange(0, m_nRangV);
		m_scrollVer.SetScrollPos(0);
		m_nCurPosy = m_nRangV;
	}
	if (m_nWindowWidth < m_lImgWid)
	{
		bHorFlag = TRUE;
		m_nRangH = (int)m_lImgWid - m_nWindowWidth;
		m_scrollHor.SetScrollRange(0, m_nRangH);
		m_scrollHor.SetScrollPos(m_nCurPosx);
	}
	if (bHorFlag)
	{
		GetDlgItem(IDC_SCROLLBAR_HOR)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_SCROLLBAR_HOR)->ShowWindow(SW_HIDE);
	}
	if (bVerFlag)
	{
		GetDlgItem(IDC_SCROLLBAR_VER)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_SCROLLBAR_VER)->ShowWindow(SW_HIDE);
	}
	// 显示比率
	GetDlgItem(IDC_EDIT_SHOWRATE_X)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SHOWRATE_Y)->EnableWindow(FALSE);
	SetWindowExtents(m_lHWindowID, 0, 0, m_nWindowWidth, m_nWindowHeight);
	SetPart(m_lHWindowID, 0, 0, (m_nWindowHeight - 1) / m_fScaleY, (m_nWindowWidth - 1) / m_fScaleX);
	CRect rect;
	GetDlgItemRect(IDC_PIC, this, &rect);
	InvalidateRect(rect, TRUE);// 清屏
}

// 设定比率显示
void CPropDlg::OnRadioSetrate()
{
	// TODO: Add your control notification handler code here
	m_nRangH = 0;
	m_nRangV = 0;
	m_nCurPosy = 0;
	m_nCurPosx = 0;
	UpdateData(TRUE);

	// 判断是否应该存在滚动条
	BOOL bHorFlag = FALSE;// 横滚动条是否存在
	BOOL bVerFlag = FALSE;// 竖滚动条是否存在
	if (m_nWindowHeight<int(m_lImgHei*m_fScaleY))
	{
		bVerFlag = TRUE;
		m_nRangV = int(m_lImgHei*m_fScaleY) - m_nWindowHeight;

		m_scrollVer.SetScrollRange(0, m_nRangV);
		m_scrollVer.SetScrollPos(0);
		m_nCurPosy = m_nRangV;
	}
	if (m_nWindowWidth<int(m_lImgWid*m_fScaleX))
	{
		bHorFlag = TRUE;
		m_nRangH = int(m_lImgWid*m_fScaleX) - m_nWindowWidth;
		m_scrollHor.SetScrollRange(0, m_nRangH);
		m_scrollHor.SetScrollPos(m_nCurPosx);
	}
	if (bHorFlag)
	{
		GetDlgItem(IDC_SCROLLBAR_HOR)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_SCROLLBAR_HOR)->ShowWindow(SW_HIDE);
	}
	if (bVerFlag)
	{
		GetDlgItem(IDC_SCROLLBAR_VER)->ShowWindow(SW_SHOW);
	}
	else
	{
		GetDlgItem(IDC_SCROLLBAR_VER)->ShowWindow(SW_HIDE);
	}
	// 显示比率
	GetDlgItem(IDC_EDIT_SHOWRATE_X)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_SHOWRATE_Y)->EnableWindow(TRUE);
	SetWindowExtents(m_lHWindowID, 0, 0, m_nWindowWidth, m_nWindowHeight);
	SetPart(m_lHWindowID, 0, 0, (m_nWindowHeight - 1) / m_fScaleY, (m_nWindowWidth - 1) / m_fScaleX);
	CRect rect;
	GetDlgItemRect(IDC_PIC, this, &rect);
	InvalidateRect(rect, TRUE);// 清屏	
}

// 水平滚动条响应
void CPropDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nCurPos;
	int nID = pScrollBar->GetDlgCtrlID();
	CString str;

	// 拖动图像水平滚动条
	if (nID == IDC_SCROLLBAR_HOR)
	{
		switch (nSBCode)
		{
		case SB_THUMBPOSITION:
			pScrollBar->SetScrollPos(nPos);
			break;
		case SB_LINERIGHT:
			nCurPos = pScrollBar->GetScrollPos();
			nCurPos++;
			if (nCurPos > m_nRangH)
				nCurPos = m_nRangH;
			pScrollBar->SetScrollPos(nCurPos);
			break;
		case SB_LINELEFT:
			nCurPos = pScrollBar->GetScrollPos();
			nCurPos--;
			if (nCurPos < 0)
				nCurPos = 0;
			pScrollBar->SetScrollPos(nCurPos);
			break;
		case SB_PAGERIGHT:
			nCurPos = pScrollBar->GetScrollPos();
			nCurPos += 10;
			if (nCurPos > m_nRangH)
				nCurPos = m_nRangH;
			pScrollBar->SetScrollPos(nCurPos);
			break;
		case SB_PAGELEFT:
			nCurPos = pScrollBar->GetScrollPos();
			nCurPos -= 10;
			if (nCurPos < 0)
				nCurPos = 0;
			pScrollBar->SetScrollPos(nCurPos);
			break;
		case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			break;
		}
		nCurPos = pScrollBar->GetScrollPos();
		m_nCurPosx = nCurPos;
		SetPart(m_lHWindowID, m_nCurPosy / m_fScaleY, m_nCurPosx / m_fScaleX,
			(m_nWindowHeight - 1 + m_nCurPosy) / m_fScaleY,
			(m_nWindowWidth - 1 + m_nCurPosx) / m_fScaleX);
		// 清屏
		CRect rect;
		GetDlgItemRect(IDC_PIC, this, &rect);
		InvalidateRect(rect, TRUE);
	}

	CBCGPDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

// 垂直滚动条响应
void CPropDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nCurPos;
	int nID = pScrollBar->GetDlgCtrlID();
	CString str;
	if (nID == IDC_SCROLLBAR_VER)
	{
		switch (nSBCode)
		{
		case SB_THUMBPOSITION:
			pScrollBar->SetScrollPos(nPos);
			break;
		case SB_LINERIGHT:
			nCurPos = pScrollBar->GetScrollPos();
			nCurPos++;
			if (nCurPos > m_nRangV)
				nCurPos = m_nRangV;
			pScrollBar->SetScrollPos(nCurPos);
			break;
		case SB_LINELEFT:
			nCurPos = pScrollBar->GetScrollPos();
			nCurPos--;
			if (nCurPos < 0)
				nCurPos = 0;
			pScrollBar->SetScrollPos(nCurPos);
			break;
		case SB_PAGERIGHT:
			nCurPos = pScrollBar->GetScrollPos();
			nCurPos += 10;
			if (nCurPos > m_nRangV)
				nCurPos = m_nRangV;
			pScrollBar->SetScrollPos(nCurPos);
			break;
		case SB_PAGELEFT:
			nCurPos = pScrollBar->GetScrollPos();
			nCurPos -= 10;
			if (nCurPos < 0)
				nCurPos = 0;
			pScrollBar->SetScrollPos(nCurPos);
			break;
		case SB_THUMBTRACK:
			pScrollBar->SetScrollPos(nPos);
			break;
		}
		nCurPos = pScrollBar->GetScrollPos();
		m_nCurPosy = nCurPos;
		SetPart(m_lHWindowID, m_nCurPosy / m_fScaleY, m_nCurPosx / m_fScaleX,
			(m_nWindowHeight - 1 + m_nCurPosy) / m_fScaleY,
			(m_nWindowWidth - 1 + m_nCurPosx) / m_fScaleX);
		// 清屏
		CRect rect;
		GetDlgItemRect(IDC_PIC, this, &rect);
		InvalidateRect(rect, TRUE);
	}
	CBCGPDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

// 得到控件大小
//******************************************************************************//
//函数:得到控件大小
//参数:
//	nID         输入，控件ID
//	wndParent   输入，窗口
//	Client      输出，控件区域区域
//返回:
//	
//******************************************************************************//
void  CPropDlg::GetDlgItemRect(int nID, CWnd *wndParent, LPRECT Client)
{
	CRect WinRect, ClientRect;
	wndParent->GetClientRect(&WinRect);
	wndParent->ClientToScreen(&WinRect);
	wndParent->GetDlgItem(nID)->GetWindowRect(&ClientRect);
	Client->left = ClientRect.left - WinRect.left;
	Client->top = ClientRect.top - WinRect.top;
	Client->right = ClientRect.right - WinRect.left;
	Client->bottom = ClientRect.bottom - WinRect.top;
}

void CPropDlg::OnKillfocusEditShowrateX()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	OnRadioSetrate();
}

void CPropDlg::OnKillfocusEditShowrateY()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	OnRadioSetrate();
}

void CPropDlg::OnButtonReaddefault()
{
	// TODO: Add your control notification handler code here
	//m_pTempChecker->ReadPara(m_pTempChecker->m_strDefaultParaIni);
}

void CPropDlg::OnButtonWritedefault()
{
	// TODO: Add your control notification handler code here
	//m_pTempChecker->WritePara();
}
// 对话框回车键不退出
BOOL CPropDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			return TRUE;
			break;
		case VK_RETURN:
			return TRUE;
		default:
			break;
		}
	}
	return CBCGPDialog::PreTranslateMessage(pMsg);
}

void CPropDlg::OnBnClickedButtonAtuocreatmodel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bAllowModify)
	{
		AfxMessageBox(_T("非模板图像，不能创建模板！"));
		return;
	}
	m_bChangePropParam = FALSE;
	int nState;
	s_Status CreateOrLearnState;
	GetDlgItem(IDC_BUTTON_ATUOCREATMODEL)->EnableWindow(FALSE);
	if (m_bAllowModify)
	{
        if (m_sDstRegionProperty.nROIIndex > 1)m_pTempChecker->m_regions = m_regions;
		nState = m_pTempChecker->AutoCreateAlgMode(m_hImgSrc, &m_hShowReg, m_htParamVaule, m_htResolution, &m_htParamVaule, CreateOrLearnState);
	}
	else
	{
		nState = m_pTempChecker->AutoLearnAlgMode(m_hImgSrc, CreateOrLearnState);
	}
	AfxMessageBox(CreateOrLearnState.strErrorContext);//外部显示状态
	GetDlgItem(IDC_BUTTON_ATUOCREATMODEL)->EnableWindow(TRUE);
	if (nState != 0)
	{
		//建模失败
		return;
	}

	int ret = AddPropertyData();

	//ShowLstControlContent();
	/*m_pTempChecker->m_htParamVaule = m_htParamVaule;
	m_pTempChecker->WritePara();*/
	OnPaint();
}

bool CPropDlg::InitLstControl()
{
	bool ret = true;
	LRESULT lStyle = m_lstParam.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	lStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP/*|LVS_EX_CHECKBOXES*/;
	m_lstParam.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)lStyle);
	LV_COLUMN lvc;
	lvc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;
	m_lstParam.DeleteAllItems();

	int i = 0;
	lvc.iSubItem = i;
	lvc.pszText = _T("属性");
	lvc.cx = 40;
	m_lstParam.InsertColumn(++i, &lvc);

	lvc.iSubItem = i;
	lvc.pszText = _T("名称");
	lvc.cx = 65;
	m_lstParam.InsertColumn(++i, &lvc);

	lvc.iSubItem = i++;
	lvc.pszText = _T("数值");
	lvc.cx = 50;
	m_lstParam.InsertColumn(++i, &lvc);

	lvc.iSubItem = i++;
	lvc.pszText = _T("含义");
	lvc.cx = 200;
	m_lstParam.InsertColumn(++i, &lvc);

	m_lstParam.SetEditColomn(0, FALSE);
	m_lstParam.SetEditColomn(1, FALSE);
	m_lstParam.SetEditColomn(2, TRUE);
	m_lstParam.SetEditColomn(3, FALSE);

	return ret;
}
bool CPropDlg::ShowLstControlContent()
{
	bool ret = true;
	CString strItemTxt;
	m_lstParam.DeleteAllItems();

	if (m_nParamNum == 0)
	{
		return FALSE;
	}

	for (int n = 0; n < m_nParamNum; n++)
	{
		CString strItemTxt;
		int j = 0;

		//参数属性
		int nParamProp;
		nParamProp = m_htParamProp[n].I();
		strItemTxt.Format(_T("%d"), nParamProp);
		m_lstParam.InsertItem(n, strItemTxt);

		//参数名称
		CString strParamName;
		strParamName = m_htParamName[n].C();
		strItemTxt.Format(_T("%s"), strParamName);
		m_lstParam.SetItemText(n, ++j, strParamName);

		//参数值
		double dParamVaule;
		dParamVaule = m_htParamVaule[n].D();
		double dRemainder;
		dRemainder = dParamVaule - floor(dParamVaule);
		if (dRemainder<0.1 && dRemainder>-0.1)//判断整形还是浮点型
		{
			strItemTxt.Format(_T("%.0f"), dParamVaule);
		}
		else
		{
			strItemTxt.Format(_T("%.1f"), dParamVaule);
		}

		m_lstParam.SetItemText(n, ++j, strItemTxt);

		//参数含义
		CString strParamMeaning;
		strParamMeaning = m_htParamMeaning[n].C();
		strItemTxt.Format(_T("%s"), strParamMeaning);
		m_lstParam.SetItemText(n, ++j, strItemTxt);
	}
	return ret;
}
LRESULT CPropDlg::UpdatadataParam(WPARAM wParam, LPARAM LParam)
{
	for (int i = 0; i < m_nParamNum; i++)
	{
		CString strParamVaule;
		strParamVaule = m_lstParam.GetItemText(i, 2);
		double dParamVaule;
		dParamVaule = _tstof(strParamVaule);
		int nParamProp;
		nParamProp = m_htParamProp[i].I();
		double dOldParamVaule;
		dOldParamVaule = m_htParamVaule[i].D();
		if (nParamProp == 1 && abs(dOldParamVaule - dParamVaule) > 0.05)
		{
			if (m_bAllowModify)
			{
				m_bChangePropParam = TRUE;
			}
			else
			{
				AfxMessageBox(_T("非模板图时，属性参数不能修改"));
				double dRemainder;
				CString strItemTxt;
				dRemainder = dOldParamVaule - floor(dOldParamVaule);
				if (dRemainder<0.1 && dRemainder>-0.1)//判断整形还是浮点型
				{
					strItemTxt.Format(_T("%.0f"), dOldParamVaule);
				}
				else
				{
					strItemTxt.Format(_T("%.1f"), dOldParamVaule);
				}
				m_lstParam.SetItemText(i, 2, strItemTxt);
				UpdateData(FALSE);
				return 0;
			}
		}

		TupleReplace(m_htParamVaule, i, dParamVaule, &m_htParamVaule);
	}
	m_pTempChecker->m_htParamVaule = m_htParamVaule;
	return 0;
}
//BCG 初始化
bool CPropDlg::InitListBCGControl()
{
	bool ret = true;

	CRect rectPropList;
	m_wndPropListLocation.GetClientRect(&rectPropList);
	m_wndPropListLocation.MapWindowPoints(this, &rectPropList);
	m_wndPropList.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectPropList, this, (UINT)-1);

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDesciptionArea(TRUE);		//显示描述区域
	m_wndPropList.SetVSDotNetLook(TRUE);		//Visual Studio Look
	m_wndPropList.MarkModifiedProperties(FALSE);		//Mar&k Changed Properties
	m_wndPropList.SetNameAlign(DT_LEFT);	//文字左对齐还是右对齐
	m_wndPropList.SetAlphabeticMode(FALSE);		//是否按照字母顺序排列
	m_wndPropList.SetShowDragContext(TRUE/*m_bShowDragContext*/);
	m_wndPropList.SetCommandsVisible(TRUE);		//显示命令
	//m_wndPropList.EnableToolBar(TRUE);			//开启工具栏(两个排序按钮)
	//m_wndPropList.EnableSearchBox(TRUE, _T("Search"));			//开启搜索框


	//LOGFONT lf;
	//CFont* font = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	//font->GetLogFont(&lf);
	//lstrcpy(lf.lfFaceName, _T("Arial"));

	InitCommand();
	m_wndPropList.AddProperty(new CBCGPProp(_T("没有选择区域")));
	return ret;
}
int CPropDlg::InitCommand()
{
	CStringList lstCommands;
	//lstCommands.AddTail(_T("[重置数据]"));
	//lstCommands.AddTail(_T("[展开]"));
	//lstCommands.AddTail(_T("[收起]"));
	lstCommands.AddTail(_T("[提交更改]"));

	m_wndPropList.SetCommands(lstCommands);
	return 0;
}
int CPropDlg::ClearPropertyCtl()
{
	m_wndPropList.RemoveAll();
	m_pGroupGeneral = NULL;
	////m_pGroupGeneral->RemoveAllOptions();
	//delete m_pGroupGeneral;		//m_wndPropList.RemoveAll(); 已经释放了

	InitCommand();
	m_wndPropList.Invalidate();  //重绘
	return 0;
}
int CPropDlg::AddPropertyData()
{
	ClearPropertyCtl();
	m_pGroupGeneral = new CBCGPProp(_T("『区域参数』"));

	int nParamProp;
	int nParamType;
	CString strParamName;
	CString strParamMeaning;
	double  dParamVaule;

	bool bValue;
	int nValue;
	double dValue;
	CString   strChildParamName;
	CString info;
	CBCGPProp* pProp;

	for (int i = 0; i < m_nParamNum; i++)
	{
		nParamProp = m_htParamProp[i];
		nParamType = m_htParamType[i];
		strParamName = m_htParamName[i];
		strParamMeaning = m_htParamMeaning[i];
		dParamVaule = m_htParamVaule[i];

		//0为BOOL型开关量，1为int型，2为double型 3.枚举选择
		switch (nParamType)
		{
		case 0:
			bValue = dParamVaule != 0 ? true : false;
			info.Format(_T("%s"), strParamMeaning);
			pProp = new CBCGPProp(strParamName, (_variant_t)bValue, info);
			if (nParamProp == 1 && !m_bAllowModify)
			{
				pProp->Enable(FALSE, FALSE);
			}
			m_pGroupGeneral->AddSubItem(pProp);
			pProp->SetData(i);
			break;
		case 1:
			nValue = (int)dParamVaule;
			info.Format(_T("%s"), strParamMeaning);
			pProp = new CBCGPProp(strParamName, (_variant_t)nValue, info);
			if (nParamProp == 1 && !m_bAllowModify)
			{
				pProp->Enable(FALSE, FALSE);
			}
			m_pGroupGeneral->AddSubItem(pProp);
			pProp->SetData(i);
			break;
		case 2:
			dValue = (double)dParamVaule;
			info.Format(_T("%s"), strParamMeaning);
			pProp = new CBCGPProp(strParamName, (_variant_t)dValue, info);
			if (nParamProp == 1 && !m_bAllowModify)
			{
				pProp->Enable(FALSE, FALSE);
			}
			m_pGroupGeneral->AddSubItem(pProp);
			pProp->SetData(i);
			break;
		case 3:
			strChildParamName = m_htParamChildNames[i];
			if (strChildParamName.GetLength() > 0)
			{
				CString* pStr;
				int iSubStrs;
				pStr = SplitString1(strChildParamName, ';', iSubStrs);
				if (iSubStrs > 1)
				{
					for (int j = 0; j < iSubStrs; j++)
					{
						if (j == ((int)dParamVaule))
						{
							pProp = new CBCGPProp(strParamName, pStr[j].GetBuffer(), strParamMeaning);
						}
					}
					for (int j = 0; j < iSubStrs; j++)
					{
						pProp->AddOption(pStr[j]);
					}

				}
				else if (iSubStrs == 1 && (strChildParamName.Find(' ') == -1))//只有一个参数
				{
					pProp = new CBCGPProp(strParamName, strChildParamName.GetBuffer(), strParamMeaning);
					pProp->AddOption(strChildParamName);
				}
				delete[]pStr;
			}
			if (nParamProp == 1 && !m_bAllowModify)
			{
				pProp->Enable(FALSE, FALSE);
			}

			pProp->AllowEdit(FALSE);
			m_pGroupGeneral->AddSubItem(pProp);
			pProp->SetData(i);
			break;

		default:
			break;
		}

	}
	m_wndPropList.AddProperty(m_pGroupGeneral);
	m_wndPropList.Invalidate();  //重绘

	return 0;
}
LRESULT CPropDlg::OnPropertyCommandClicked(WPARAM, LPARAM lp)
{
	int TriggerCommandIndex = (int)lp;

	//if (TriggerCommandIndex == 0)
	//{
	//	//ResetValues
	//	m_wndPropList.ResetOriginalValues();
	//}
	//else if (TriggerCommandIndex == 1)
	//{
	//	//ExpandAll
	//	m_wndPropList.ExpandAll(TRUE);
	//}
	//else if (TriggerCommandIndex == 2)
	//{
	//	//CollapseAll
	//	m_wndPropList.ExpandAll(FALSE);
	//}
	/*else*/ if (TriggerCommandIndex == 0)
	{
		if (m_pGroupGeneral == NULL || m_pGroupGeneral->GetSubItemsCount() < 1)
		{
			return 0;
		}

		//CommitModify
		if (PropertyCommitModify() == 0)
		{
			m_wndPropList.CommitModifiedValues();
			MessageBox(_T("数据更新成功!"), _T("提示"), MB_OK);
		}
		else
		{
			MessageBox(_T("数据更新失败!"), _T("警告"), MB_OK);
		}

	}
	else
	{
		return -1;
	}
	return 0;
}

LRESULT CPropDlg::OnPropChanged(WPARAM uProListID, LPARAM pPro)
{

	m_bPropertyModify = TRUE;
	return 0;
}


int CPropDlg::PropertyCommitModify()
{
	if (!m_bPropertyModify)
	{
		return 0;
	}

	if (m_pGroupGeneral->GetSubItemsCount() != m_nParamNum)
	{
		//m_sCheckParams内容从GetRegionParamToRegionPropertyCtl()到此遭到非法修改
		return -1;
	}

	int nParamType;
	CString strParamName;
	CString strParamMeaning;
	double  dParamVaule;
	CString   strChildParamName;

	bool bValue;
	int nValue;
	double dValue;
	//0为BOOL型开关量，1为int型，2为double型 3.枚举选择
	for (int i = 0; i < m_nParamNum; i++)
	{
		CBCGPProp* pSubProp = m_pGroupGeneral->FindSubItemByData(i);

		nParamType = m_htParamType[i];
		strParamName = m_htParamName[i];
		strParamMeaning = m_htParamMeaning[i];
		dParamVaule = m_htParamVaule[i];
		strChildParamName = m_htParamChildNames[i];

		CString strChildParamName0;
		//0为BOOL型开关量，1为int型，2为double型 3.枚举选择
		switch (nParamType)
		{
		case 0:
			bValue = (bool)pSubProp->GetValue();
			m_htParamVaule[i] = bValue ? 1 : 0;
			break;
		case 1:
			nValue = (int)pSubProp->GetValue();
			m_htParamVaule[i] = nValue;
			break;
		case 2:
			dValue = (double)pSubProp->GetValue();
			m_htParamVaule[i] = dValue;
			break;
		case 3:
			strChildParamName0 = /*(CString)*/pSubProp->GetValue();

			if (strChildParamName.GetLength() > 0)
			{
				CString* pStr;
				int iSubStrs;
				pStr = SplitString1(strChildParamName, ';', iSubStrs);
				if (iSubStrs > 1)
				{
					for (int j = 0; j < iSubStrs; j++)
					{
						if (pStr[j] == strChildParamName0)
						{
							m_htParamVaule[i] = j;
						}
					}

				}
				else if (iSubStrs == 1 && (strChildParamName.Find(' ') == -1))//只有一个参数
				{
					if (strChildParamName == strChildParamName0)
					{
						m_htParamVaule[i] = 0;
					}
				}
				delete[]pStr;
			}
			break;

		default:
			break;
		}

	}
	m_pTempChecker->m_htParamVaule = m_htParamVaule;
	m_pTempChecker->WritePara();
	return 0;
}

void CPropDlg::OnBnClickedGenModelMatch()
{
    if (m_sDstRegionProperty.nROIIndex>1)
    {
        AfxMessageBox(_T("请选择区域1进行匹配模板创建！"));
        return;
    }
    if (!m_bAllowModify)
    {
        AfxMessageBox(_T("非模板图像，不能创建匹配模板！"));
        return;
    }
    
    s_Status sStatus = m_pTempChecker->GenMatchTem(m_hImgSrcBig, m_hShowReg);
    if (1==sStatus.nErrorType)AfxMessageBox(_T("检测模板创建失败！"));
    else
    {
        AfxMessageBox(_T("检测模板创建成功！"));
        m_bMatchTem = TRUE;
    }
    
}
