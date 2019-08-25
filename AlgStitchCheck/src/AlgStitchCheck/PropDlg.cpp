// PropDlg.cpp : implementation file
//
/************************************************************************
/* Copyright (c) 2008, �������ͼ���Ӿ����޹�˾�Ӿ��㷨��
/* All rights reserved.
/*
/* �ļ����ƣ�  PropDlg.cpp
/* ժҪ�� ���ԶԻ���
/*
/* ��ǰ�汾�� 1.0
/* �����ߣ�   �˺���
/* ������ڣ� 2010��8��8��
/* ˵����     ���ԶԻ����У��޸Ĳ������������������������ʵ�ֺ���
/* �޸��ˣ�   ������
/* �汾��     V2.0
/* �޸�ʱ�䣺 2015-06-04
/* �޸ļ�¼�� 1. ��ʽ����ܴ��룬��ǿ�ɶ��ԡ�
		2. ����HALCON��ʾͼ��ķ������޸���OnRadioAuto()��OnRadioNorm()
		   �Լ�OnRadioSetrate()�����е���ʾ�������롣
		3. �Ի����ʼ������OnInitDialog()������Ӱ�������Ϣ���г�����
		   ����ͼ�񣬲�������ʾ��ͼ��ؼ��С�
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
//�ַ����̶��ָ�
CString * SplitString1(CString str, char split, int& iSubStrs)
{
	int iPos = 0; //�ָ��λ��
	int iNums = 0; //�ָ��������
	CString strTemp = str;
	CString strRight;
	//�ȼ������ַ���������
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
	if (iNums == 0) //û���ҵ��ָ��
	{
		//���ַ����������ַ�������
		iSubStrs = 1;
		return NULL;
	}
	//���ַ�������
	iSubStrs = iNums + 1; //�Ӵ������� = �ָ������ + 1
	CString* pStrSplit;
	pStrSplit = new CString[iSubStrs];
	strTemp = str;
	CString strLeft;
	for (int i = 0; i < iNums; i++)
	{
		iPos = strTemp.Find(split);
		//���Ӵ�
		strLeft = strTemp.Left(iPos);
		//���Ӵ�
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

// ��ͼ
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

// �Ի����ʼ��
BOOL CPropDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();
	try
	{
		//int cx, cy;
		//cx = GetSystemMetrics(SM_CXSCREEN);
		//cy = GetSystemMetrics(SM_CYSCREEN);
		////����MoveWindow
		//CRect rcTemp;
		//rcTemp.BottomRight() = CPoint(cx, cy);
		//rcTemp.TopLeft() = CPoint(0, 0);
		//MoveWindow(&rcTemp);

		CheckRadioButton(IDC_RADIO_AUTO, //��һ������Ϊ����ĵ�һ����ѡ��ť��ID  		
			IDC_RADIO_SETRATE,           //�ڶ�������Ϊ��������һ����ѡ��ť��ID  		
			IDC_RADIO_AUTO);

		//��ȡHALCON����ID
		GetHWindowID(IDC_PIC, m_nWindowWidth, m_nWindowHeight, m_lHWindowID);

		//��ȡHALCONͼ��
		HTuple hv_ImgWid, hv_ImgHei;
		//CopyObj(*m_sAlgImageInfo.p3DHImg, &m_hImgSrcBig, 1, -1);

		//// ��ʱ���� [9/12/2018 zb]
		//CString strPath;
		//const char * cPath;
		//strPath = _T("D:/InspectionSystem/����ͼ/0925-006/Camera0/1.bmp");
		////cPath =  (char*)strPath.GetBuffer(0);
		//CStringA strPathAEn(strPath);
		//cPath = strPathAEn.GetString();
		//ReadImage(&m_hImgSrcBig, (HTuple)cPath);
		//GetDomain(m_hImgSrcBig, &m_sDstRegionProperty.hRegion);

		//��ȡ����ͼ��
		
        /*HObject hv_X, hv_Y, hv_Z;
        Decompose3(m_hImgSrcBig, &hv_X, &hv_Y, &hv_Z);*/
        HObject HImg;

        ReduceDomain(m_hImgSrcBig, m_sDstRegionProperty.hRegion, &m_hImgSrc);
		//CropDomain(HImg, &m_hImgSrc);
        
		GetImageSize(m_hImgSrc, &hv_ImgWid, &hv_ImgHei);
		m_lImgWid = hv_ImgWid[0].L();
		m_lImgHei = hv_ImgHei[0].L();
		//��ʾͼ��
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

		// ���������ʼ��
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
		strError.Format(_T("��ʼ��ʧ�ܣ�,halcon- "));
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		AfxMessageBox(strError);
		return FALSE;
	}
	catch (...)
	{
		AfxMessageBox(_T("��ʼ��ʧ�ܣ�"));
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
// �Ի����˳��ͷſռ�
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

// ���
void CPropDlg::OnButtonTest()
{
	if (m_bChangePropParam)
	{
		AfxMessageBox(_T("���Բ����޸ģ����ȴ���ģ�壡"));
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
		AfxMessageBox(_T("ȱ��ģ�壬�봴��ģ��"));
		return;
	}
	m_pTempChecker->SingleCheck(sAlgInnerCheckInputParam, m_hImgSrcBig, &sInnerSingleCheckOutputResult);

	m_hShowReg = m_pTempChecker->m_hShowReg;
	m_hErrorReg = m_pTempChecker->m_hErrorReg;
	CString strResultCode;
	strResultCode = sInnerSingleCheckOutputResult.sSingleErrorInfo.strCheckResultCode;

	m_strCheckResult.Format(_T("\n1. ��ֵ = %d , ���� = %d \n"),
		sInnerSingleCheckOutputResult.sSingleErrorInfo.meanValue,
		sInnerSingleCheckOutputResult.sSingleErrorInfo.DeviationValue);
	UpdateData(FALSE);
	OnPaint();
}

// ����
void CPropDlg::OnButtonSave()
{
	if (m_bChangePropParam)
	{
		AfxMessageBox(_T("���Բ����޸ģ����ȴ���ģ�壡"));
		return;
	}
    if (1==m_sDstRegionProperty.nROIIndex && !m_bMatchTem)
    {
        AfxMessageBox(_T("ƥ��ģ����δ������"));
        return;
    }
	m_pTempChecker->WritePara();
	OnOK();
}

// ����������ʾ
void CPropDlg::OnRadioAuto() // ����ʱ��ȫ��������ʾ
{
	// TODO: Add your control notification handler code here
	m_nRangH = 0;
	m_nRangV = 0;
	m_nCurPosy = 0;
	m_nCurPosx = 0;
	m_fScaleX = (float)((m_nWindowWidth) / m_lImgWid);
	m_fScaleY = (float)((m_nWindowHeight) / m_lImgHei);
	UpdateData(FALSE);

	// ���ι�����
	GetDlgItem(IDC_SCROLLBAR_HOR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SCROLLBAR_VER)->ShowWindow(SW_HIDE);

	// ��ʾ����
	GetDlgItem(IDC_EDIT_SHOWRATE_X)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SHOWRATE_Y)->EnableWindow(FALSE);

	SetPart(m_lHWindowID, 0, 0, m_lImgHei - 1, m_lImgWid - 1);

	CRect rect;
	GetDlgItemRect(IDC_PIC, this, &rect);
	InvalidateRect(rect, TRUE);// ����	
}

// ԭʼ��ʾ
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

	// �ж��Ƿ�Ӧ�ô��ڹ�����
	BOOL bHorFlag = FALSE;// ��������Ƿ����
	BOOL bVerFlag = FALSE;// ���������Ƿ����
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
	// ��ʾ����
	GetDlgItem(IDC_EDIT_SHOWRATE_X)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SHOWRATE_Y)->EnableWindow(FALSE);
	SetWindowExtents(m_lHWindowID, 0, 0, m_nWindowWidth, m_nWindowHeight);
	SetPart(m_lHWindowID, 0, 0, (m_nWindowHeight - 1) / m_fScaleY, (m_nWindowWidth - 1) / m_fScaleX);
	CRect rect;
	GetDlgItemRect(IDC_PIC, this, &rect);
	InvalidateRect(rect, TRUE);// ����
}

// �趨������ʾ
void CPropDlg::OnRadioSetrate()
{
	// TODO: Add your control notification handler code here
	m_nRangH = 0;
	m_nRangV = 0;
	m_nCurPosy = 0;
	m_nCurPosx = 0;
	UpdateData(TRUE);

	// �ж��Ƿ�Ӧ�ô��ڹ�����
	BOOL bHorFlag = FALSE;// ��������Ƿ����
	BOOL bVerFlag = FALSE;// ���������Ƿ����
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
	// ��ʾ����
	GetDlgItem(IDC_EDIT_SHOWRATE_X)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_SHOWRATE_Y)->EnableWindow(TRUE);
	SetWindowExtents(m_lHWindowID, 0, 0, m_nWindowWidth, m_nWindowHeight);
	SetPart(m_lHWindowID, 0, 0, (m_nWindowHeight - 1) / m_fScaleY, (m_nWindowWidth - 1) / m_fScaleX);
	CRect rect;
	GetDlgItemRect(IDC_PIC, this, &rect);
	InvalidateRect(rect, TRUE);// ����	
}

// ˮƽ��������Ӧ
void CPropDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nCurPos;
	int nID = pScrollBar->GetDlgCtrlID();
	CString str;

	// �϶�ͼ��ˮƽ������
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
		// ����
		CRect rect;
		GetDlgItemRect(IDC_PIC, this, &rect);
		InvalidateRect(rect, TRUE);
	}

	CBCGPDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

// ��ֱ��������Ӧ
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
		// ����
		CRect rect;
		GetDlgItemRect(IDC_PIC, this, &rect);
		InvalidateRect(rect, TRUE);
	}
	CBCGPDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

// �õ��ؼ���С
//******************************************************************************//
//����:�õ��ؼ���С
//����:
//	nID         ���룬�ؼ�ID
//	wndParent   ���룬����
//	Client      ������ؼ���������
//����:
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
// �Ի���س������˳�
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!m_bAllowModify)
	{
		AfxMessageBox(_T("��ģ��ͼ�񣬲��ܴ���ģ�壡"));
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
	AfxMessageBox(CreateOrLearnState.strErrorContext);//�ⲿ��ʾ״̬
	GetDlgItem(IDC_BUTTON_ATUOCREATMODEL)->EnableWindow(TRUE);
	if (nState != 0)
	{
		//��ģʧ��
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
	lvc.pszText = _T("����");
	lvc.cx = 40;
	m_lstParam.InsertColumn(++i, &lvc);

	lvc.iSubItem = i;
	lvc.pszText = _T("����");
	lvc.cx = 65;
	m_lstParam.InsertColumn(++i, &lvc);

	lvc.iSubItem = i++;
	lvc.pszText = _T("��ֵ");
	lvc.cx = 50;
	m_lstParam.InsertColumn(++i, &lvc);

	lvc.iSubItem = i++;
	lvc.pszText = _T("����");
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

		//��������
		int nParamProp;
		nParamProp = m_htParamProp[n].I();
		strItemTxt.Format(_T("%d"), nParamProp);
		m_lstParam.InsertItem(n, strItemTxt);

		//��������
		CString strParamName;
		strParamName = m_htParamName[n].C();
		strItemTxt.Format(_T("%s"), strParamName);
		m_lstParam.SetItemText(n, ++j, strParamName);

		//����ֵ
		double dParamVaule;
		dParamVaule = m_htParamVaule[n].D();
		double dRemainder;
		dRemainder = dParamVaule - floor(dParamVaule);
		if (dRemainder<0.1 && dRemainder>-0.1)//�ж����λ��Ǹ�����
		{
			strItemTxt.Format(_T("%.0f"), dParamVaule);
		}
		else
		{
			strItemTxt.Format(_T("%.1f"), dParamVaule);
		}

		m_lstParam.SetItemText(n, ++j, strItemTxt);

		//��������
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
				AfxMessageBox(_T("��ģ��ͼʱ�����Բ��������޸�"));
				double dRemainder;
				CString strItemTxt;
				dRemainder = dOldParamVaule - floor(dOldParamVaule);
				if (dRemainder<0.1 && dRemainder>-0.1)//�ж����λ��Ǹ�����
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
//BCG ��ʼ��
bool CPropDlg::InitListBCGControl()
{
	bool ret = true;

	CRect rectPropList;
	m_wndPropListLocation.GetClientRect(&rectPropList);
	m_wndPropListLocation.MapWindowPoints(this, &rectPropList);
	m_wndPropList.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER, rectPropList, this, (UINT)-1);

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDesciptionArea(TRUE);		//��ʾ��������
	m_wndPropList.SetVSDotNetLook(TRUE);		//Visual Studio Look
	m_wndPropList.MarkModifiedProperties(FALSE);		//Mar&k Changed Properties
	m_wndPropList.SetNameAlign(DT_LEFT);	//��������뻹���Ҷ���
	m_wndPropList.SetAlphabeticMode(FALSE);		//�Ƿ�����ĸ˳������
	m_wndPropList.SetShowDragContext(TRUE/*m_bShowDragContext*/);
	m_wndPropList.SetCommandsVisible(TRUE);		//��ʾ����
	//m_wndPropList.EnableToolBar(TRUE);			//����������(��������ť)
	//m_wndPropList.EnableSearchBox(TRUE, _T("Search"));			//����������


	//LOGFONT lf;
	//CFont* font = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	//font->GetLogFont(&lf);
	//lstrcpy(lf.lfFaceName, _T("Arial"));

	InitCommand();
	m_wndPropList.AddProperty(new CBCGPProp(_T("û��ѡ������")));
	return ret;
}
int CPropDlg::InitCommand()
{
	CStringList lstCommands;
	//lstCommands.AddTail(_T("[��������]"));
	//lstCommands.AddTail(_T("[չ��]"));
	//lstCommands.AddTail(_T("[����]"));
	lstCommands.AddTail(_T("[�ύ����]"));

	m_wndPropList.SetCommands(lstCommands);
	return 0;
}
int CPropDlg::ClearPropertyCtl()
{
	m_wndPropList.RemoveAll();
	m_pGroupGeneral = NULL;
	////m_pGroupGeneral->RemoveAllOptions();
	//delete m_pGroupGeneral;		//m_wndPropList.RemoveAll(); �Ѿ��ͷ���

	InitCommand();
	m_wndPropList.Invalidate();  //�ػ�
	return 0;
}
int CPropDlg::AddPropertyData()
{
	ClearPropertyCtl();
	m_pGroupGeneral = new CBCGPProp(_T("�����������"));

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

		//0ΪBOOL�Ϳ�������1Ϊint�ͣ�2Ϊdouble�� 3.ö��ѡ��
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
				else if (iSubStrs == 1 && (strChildParamName.Find(' ') == -1))//ֻ��һ������
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
	m_wndPropList.Invalidate();  //�ػ�

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
			MessageBox(_T("���ݸ��³ɹ�!"), _T("��ʾ"), MB_OK);
		}
		else
		{
			MessageBox(_T("���ݸ���ʧ��!"), _T("����"), MB_OK);
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
		//m_sCheckParams���ݴ�GetRegionParamToRegionPropertyCtl()�����⵽�Ƿ��޸�
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
	//0ΪBOOL�Ϳ�������1Ϊint�ͣ�2Ϊdouble�� 3.ö��ѡ��
	for (int i = 0; i < m_nParamNum; i++)
	{
		CBCGPProp* pSubProp = m_pGroupGeneral->FindSubItemByData(i);

		nParamType = m_htParamType[i];
		strParamName = m_htParamName[i];
		strParamMeaning = m_htParamMeaning[i];
		dParamVaule = m_htParamVaule[i];
		strChildParamName = m_htParamChildNames[i];

		CString strChildParamName0;
		//0ΪBOOL�Ϳ�������1Ϊint�ͣ�2Ϊdouble�� 3.ö��ѡ��
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
				else if (iSubStrs == 1 && (strChildParamName.Find(' ') == -1))//ֻ��һ������
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
        AfxMessageBox(_T("��ѡ������1����ƥ��ģ�崴����"));
        return;
    }
    if (!m_bAllowModify)
    {
        AfxMessageBox(_T("��ģ��ͼ�񣬲��ܴ���ƥ��ģ�壡"));
        return;
    }
    
    s_Status sStatus = m_pTempChecker->GenMatchTem(m_hImgSrcBig, m_hShowReg);
    if (1==sStatus.nErrorType)AfxMessageBox(_T("���ģ�崴��ʧ�ܣ�"));
    else
    {
        AfxMessageBox(_T("���ģ�崴���ɹ���"));
        m_bMatchTem = TRUE;
    }
    
}
