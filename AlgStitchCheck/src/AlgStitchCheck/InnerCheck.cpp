/************************************************************************
/* Copyright (c) 2015, �������ͼ���Ӿ����޹�˾ӡ����ҵ��
/* All rights reserved.
/*
/* �ļ����ƣ�  InnerCheck.h
/* ��Ҫ˵����  �ڲ������
/*
/* �����ߣ�  �˺���
/* �汾��    V1.0
/* �޸��ˣ�  ������
/* �汾��    V2.0
/* �޸�ʱ�䣺2015-06-04
/* �޸ļ�¼��1. ��ʽ����ܴ��룬��ǿ�ɶ��ԡ�
	   2. ��ӳ�ʼ������Init()����Ĵ��룬������صĲ�������Ŀ¼��
		  ��������ȡReadPara()��
	   3. ��Ӵ���ͼ�����ɺ���GenCheckImage()����Ĵ��룬�����ͼ��
		  ����У���Լ���������Ϣ���ɼ��ͼ��(hCheckImage)��
	   4. ��Ӽ��������Check()����Ĵ��룬�����ͼ�����ɡ��������
		  ���������Ϣ�Լ��������ʼ����
	   5. ���д�����Ͷ�����������ʾ�����롣
	   6. ΪGenCheckImage()������Check()��������쳣���񣬲���ÿһ��
		  ����ڵ������ϸ�쳣��Ϣ��
	   7. ������ÿ��������ע��˵��
/* �޸��ˣ�  ����Ƽ
/* �汾��    V1.2.0.2
/* �޸�ʱ�䣺2017-06-29
/* �޸ļ�¼���޸�halcon13�������

/* �޸��ˣ�  �Ų�
/* �汾��    V3.0
/* �޸�ʱ�䣺2018-11-07
/* �޸ļ�¼��1.�����ӿ��ļ�
			 2.���ӽ�ģ�����ȡ��������GetAlgParam
			 3.���ӽ�ģ�������ò�������SetAlgParam
			 4.�����Զ���ģ����AutoCreateAlgModel
			 5.֧��halcon����ֱ�ӵ���,�������Ρ���ģ�����
			 6.���ϸ��Ϊ���������Ϻ��ںϼ��
************************************************************************/
#include "stdafx.h"
#include "InnerCheck.h"
#include <algorithm>
#include <math.h>
#include <io.h>
#include "AlgStitchCheckHalcon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////////
// �ڲ������ʵ��
/////////////////////////////////////////////////////////////////////////////////

// �ڲ�����๹�캯������������
//******************************************************************************//
CInnerCheck::CInnerCheck()
{
	//HException::InstallHHandler(&MyHalconExceptionHandler);
	m_bAllowModify = FALSE;
	m_nChannel = 0;
	m_htHalconHandle = -1;

	m_bFileExist = TRUE;
	GenEmptyObj(&m_hModelImg);
	GenEmptyObj(&m_hModelReg);
	GenEmptyObj(&m_hShowReg);
	GenEmptyObj(&m_hErrorReg);

	TupleGenConst(0, 0, &m_htResolution);

	TupleGenConst(0, 0, &m_htParamProp);
	TupleGenConst(0, 0, &m_htParamName);
	TupleGenConst(0, 0, &m_htParamVaule);
	TupleGenConst(0, 0, &m_htParamMeaning);

	TupleGenConst(0, 0, &m_htParamType);
	TupleGenConst(0, 0, &m_htParamMinVaule);
	TupleGenConst(0, 0, &m_htParamMaxVaule);
	TupleGenConst(0, 0, &m_htParamStep);
	TupleGenConst(0, 0, &m_htParamChildNames);

	TupleGenConst(0, 0, &m_htParam);
}
CInnerCheck::~CInnerCheck()
{

}

//��ʼ������
//******************************************************************************//
//����:��ʼ��
//����:
//  strTaskName        ���룬������
//	sAlgInitParam      ���룬ȫ��·��
//  sDstRegionProperty ���룬��������
//  bIsNewRegion       ���룬�������ʶ
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CInnerCheck::Init(const CString strTaskName, const s_AlgInitParam &sAlgInitParam, const s_ModelRegionPro &sDstRegionProperty, BOOL bIsNewRegion)
{
	s_Status sStatus;
	try
	{
		m_sAlgInitParam = sAlgInitParam;
		m_sDstRegionProperty = sDstRegionProperty;

		m_strAlgName.Format(_T("AlgStitchCheck"));
		m_strTaskPath.Format(_T("%s\\%s\\AlgTask"), m_sAlgInitParam.strAlgModePath, strTaskName);

		//��������Ŵ���������Ŀ¼ 
		m_strRegionPath.Format(_T("%s\\%s\\%d"),
			m_strTaskPath, m_strAlgName, m_sDstRegionProperty.nROIIndex);
		if (m_sDstRegionProperty.nROIIndex != 0)
		{
			m_FileOperate->CreateMultiLevelPath(m_strRegionPath);
		}

		//�������Ŀ¼
		m_strAlgPublicLib.Format(_T("%s\\AlgPublicLib\\%s"),
			m_sAlgInitParam.strAlgPath, m_strAlgName);
		m_FileOperate->CreateMultiLevelPath(m_strAlgPublicLib);

		//Ĭ�ϲ�����Ŀ¼
		m_strDefaultParaPath.Format(_T("%s\\DefaultParameter\\%s"),
			m_sAlgInitParam.strAlgPath, m_strAlgName);
		m_FileOperate->CreateMultiLevelPath(m_strDefaultParaPath);

		// �ֱ��� [10/10/2018 zb]
		m_htResolution.Clear();

		std::vector<s_CameraGroupConfig> ::const_iterator vcCamGroup;
		vcCamGroup = m_sAlgInitParam.sSystemConfig.vcCamGroup.begin();
		std::vector<s_CameraConfig> ::const_iterator vcCamera;
		vcCamera = vcCamGroup->vcCamera.begin();
		std::vector<s_CameraPosTimeConfig> ::const_iterator vcCamPos;
		vcCamPos = vcCamera->vcCamPos.begin();
		std::vector<s_TimeImageCon> ::const_iterator vcTimeImage;
		vcTimeImage = vcCamPos->vcTimeImage.begin();
		m_htResolution[0] = vcTimeImage->fResolution_H;
		m_htResolution[1] = vcTimeImage->fResolution_V;
		//ͼ��ͨ��
		m_nChannel = vcTimeImage->nChannel;
		////��ʱ��ֵ
		//m_htResolution[0] = 0.1;
		//m_htResolution[1] = 0.1;
		//��ȡ������
		ReadPara();
		//��ȡģ������
		ReadModelRegion(m_strRegionPath, m_sDstRegionProperty);

		CString strContent;
		strContent.Format(_T("�ڲ����:����-%d��ʼ��"), m_sDstRegionProperty.nROIIndex);
		m_FileOperate->WriteAlgOperateLog(m_strLogPath, _T("������־"), _T("������־"), strContent);
	}
	catch (HException &hException)
	{
		sStatus.nErrorType = 1;
		sStatus.strErrorContext = _T("Halcon��ʼ���쳣");

		CString strError;
		strError.Format(_T("����ID = %d Halcon��ʼ���쳣- "), m_sDstRegionProperty.nROIIndex);
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck()��"), strError);
	}
	catch (...)
	{
		sStatus.nErrorType = 1;
		sStatus.strErrorContext = _T("Init()�쳣");
		// д�쳣��־
		CString strErr;
		strErr.Format(_T("����ID = %d Init()�쳣"), m_sDstRegionProperty.nROIIndex);
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck()��"), strErr);
	}
	return sStatus;
}

//�ͷſռ�
//******************************************************************************//
//����:�ͷſռ�
//����:NULL
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CInnerCheck::Free()
{
	s_Status sStatus;

	FreeTMPSpace();
	CString strContent;
	strContent.Format(_T("�ڲ����:����-%d�ͷ����"), m_sDstRegionProperty.nROIIndex);
	m_FileOperate->WriteAlgOperateLog(m_strLogPath, _T("������־"), _T("������־"), strContent);

	return sStatus;
}
//���ͼ�����ɺ���
//******************************************************************************//
//����:���ͼ�����ɺ���
//����:
//	sAlgCheckInputParam    ���룬�������ṹ��
//	hImgSrc				   ���룬���ͼ��
//	hCheckImage            ���������HALCON���͵ļ��ͼ��
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CInnerCheck::GenCheckImage(const s_InnerCheckInputParam& sAlgInnerCheckInputParam, const HObject hImgSrc, HObject& hCheckImage)
{
	s_Status sStatus;
	try
	{
		HObject hImgSrcNew;
		HObject hRegion;
		//���ݶ�λ��Ϣ����ͼ��ķ���任���Խ������ͼ��
		hRegion = sAlgInnerCheckInputParam.sRegionProperty.hRegion;
		if (sAlgInnerCheckInputParam.sLocGroupResult.nLocCount > 0)
		{
			if ((!sAlgInnerCheckInputParam.sLocGroupResult.bHaveError)
				&& (!m_bAllowModify))//m_bAllowModify=1,��ʾģ��ͼ��,��������[�����涨]
			{
				AffineTransImage(hImgSrc, &hImgSrcNew, sAlgInnerCheckInputParam.sLocGroupResult.htHomMatInv,
					"constant", "false");
			}
			else if (sAlgInnerCheckInputParam.sLocGroupResult.bHaveError)
			{
				sStatus.nErrorType = 2;
				sStatus.strErrorContext = _T("��λʧ��");
				CString strErr;
				strErr.Format(_T("����ID=%d ��λʧ��"),
					sAlgInnerCheckInputParam.sRegionProperty.nROIIndex);
				m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck()��"), strErr);

				return sStatus;
			}
			else
			{
				CopyObj(hImgSrc, &hImgSrcNew, 1, -1);
			}
		}
		else
		{
			CopyObj(hImgSrc, &hImgSrcNew, 1, -1);
		}
		ReduceDomain(hImgSrcNew, hRegion, &hCheckImage);
		CropDomain(hCheckImage, &hCheckImage);
		//MirrorImage(hCheckImage, &hCheckImage, "row");
	}
	catch (HException &hException)
	{
		sStatus.nErrorType = 1;
		sStatus.strErrorContext = _T("Halcon����ͼ���쳣");

		CString strError;
		strError.Format(_T("����ID = %d Halcon����ͼ���쳣- "), sAlgInnerCheckInputParam.sRegionProperty.nROIIndex);
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck()��"), strError);
	}
	catch (...)
	{
		sStatus.nErrorType = 1;
		sStatus.strErrorContext = _T("GenCheckImage()�쳣");
		// д�쳣��־
		CString strErr;
		strErr.Format(_T("����ID = %d GenCheckImage()�쳣"),
			sAlgInnerCheckInputParam.sRegionProperty.nROIIndex);
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck()��"), strErr);
	}

	return sStatus;
}

//���������
//******************************************************************************//
//����:���������
//����:
//	hv_ImgSrcBig      ���룬�����ԭͼ
//	hv_SurfaceModelID1    ���������ƥ��ģ��ID
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CInnerCheck::GenMatchTem(HObject &hv_ImgSrcBig,HObject& hv_ShowReg)
{
    s_Status sStatus;
    try
    {
        HObject hv_X, hv_Y, hv_Z, hv_ImageDevice;
        HTuple hv_ObjectModel3DDev;
        Decompose3(hv_ImgSrcBig, &hv_X, &hv_Y, &hv_Z);
        ReduceDomain(hv_Z, hv_ShowReg, &hv_ImageDevice);
        XyzToObjectModel3d(hv_X, hv_Y, hv_ImageDevice, &hv_ObjectModel3DDev);
        CreateSurfaceModel(hv_ObjectModel3DDev, 0.03, HTuple(), HTuple(), &m_hMathId);
        CString strMatchId;
        strMatchId.Format(_T("%s\\%s\\hModelMatchId.sfm"), m_strTaskPath, m_strAlgName);
        CStringA strPathMId(strMatchId);
        const char * cMatchId = strPathMId.GetString();
        WriteSurfaceModel(m_hMathId, (HTuple)cMatchId);
    }
    catch (HException &hException)
    {
        sStatus.nErrorType = 1;
        sStatus.strErrorContext= _T("Halcon����ͼ���쳣");

        CString strError;
        strError.Format(_T("Check-Halcon-Check�쳣- "));
        strError += hException.ProcName().Text();
        strError += _T("�쳣");
        strError += hException.ErrorMessage().Text();
        m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck()��"), strError);
    }
    return sStatus;
}




//���������
//******************************************************************************//
//����:���������
//����:
//	sAlgCheckInputParam      ���룬�������ṹ��
//	sAlgCheckOutputResult    ��������ؼ��������ṹ��
//����:
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CInnerCheck::SingleCheck(const s_InnerCheckInputParam& sAlgInnerCheckInputParam, const HObject hImgSrc, s_InnerSingleCheckOutputParam *sAlgInnerSingleCheckOutputResult)
{
	s_Status sStatus;
	try
	{
		//  ��ȡHObject���͵�ͼ������hCheckImage
        //m_sDstRegionProperty->hRegion
        HObject hCheckImage;
        GenEmptyObj(&hCheckImage);
        sStatus = GenCheckImage(sAlgInnerCheckInputParam, hImgSrc, hCheckImage);

        HTuple htState, hv_ObjectModel3D, hv_Pose, hv_CameraParam, hv_Pose2, hv_Score1;
        HTuple hv_SurfaceMatchingResultID, channels, hv_Model3DStitchTmp;
        HObject  ho_X, ho_Y, ho_Z, ho_ImageStitchTmp;
        HTuple hv_Mean1, hv_Deviation1, hv_Min1, hv_Max1;
        CreatePose(-0.00, 0.00, 0.0, 0.0, 0.0, 0.0, "Rp+T", "gba", "point", &hv_Pose);
        GenCamParAreaScanDivision(0.016, 0, 5e-6, 5e-6, 1280, 960, 2560, 1920, &hv_CameraParam);

        CountChannels(hImgSrc, &channels);
        int nState = 0;

        if (channels != 3)
        {
            CString strError;
            strError.Format(_T("CInnerCheck()-InnerCheck-halcon�쳣 ����ͼ��ͨ��������"));
            m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck��"), strError);
            nState = htState;
        }
        else
        {
            try
            {
                Decompose3(hImgSrc, &ho_X, &ho_Y, &ho_Z);
                ReduceDomain(ho_Z, m_RegionDetection, &ho_ImageStitchTmp);
                XyzToObjectModel3d(ho_X, ho_Y, ho_ImageStitchTmp, &hv_Model3DStitchTmp);
                XyzToObjectModel3d(ho_X, ho_Y, ho_Z, &hv_ObjectModel3D);
                FindSurfaceModel(m_hMathId, hv_ObjectModel3D, 0.05, 1, 0, "true", "num_matches",
                    1, &hv_Pose2, &hv_Score1, &hv_SurfaceMatchingResultID);
                HTuple hv_i, hv_CPose, hv_HomMat3D, hv_ObjectModel3DAffineTrans;
                HObject ho_X1, ho_Y1, ho_Z1, ho_ObjectSelect, ho_RegionFillUp, ho_ConnectedRegions;
                HObject ho_SelectedRegions, ho_Rectangle, ho_RegionErosion, ho_StitchRegion, ho_StitchImage2D;
                HTuple hv_ObjectModel3DStitch, hv_ObjectModel3DOut, hv_Primitive_param;
                HTuple  hv_ObjectModel3DPlane, hv_GenParamValue;
                HTuple hv_Row1, hv_Column1, hv_Row2, hv_Column2;
                HTuple end_val87 = (hv_Score1.TupleLength()) - 1;
                HTuple step_val87 = 1;
                for (hv_i = 0; hv_i.Continue(end_val87, step_val87); hv_i += step_val87)
                {
                    hv_CPose = hv_Pose2.TupleSelectRange(hv_i * 7, (hv_i * 7) + 6);
                    PoseToHomMat3d(hv_CPose, &hv_HomMat3D);
                    AffineTransObjectModel3d(hv_Model3DStitchTmp, hv_HomMat3D, &hv_ObjectModel3DAffineTrans);
    /*                CString strModelImg;
                    strModelImg.Format(_T("D:/a.om3"), m_strRegionPath);
                    const char* cModelImg;
                    CStringA strPathAEn(strModelImg);
                    cModelImg = strPathAEn.GetString();
                    WriteObjectModel3d(hv_ObjectModel3DAffineTrans, 'om3', (HTuple)cModelImg,HTuple(),HTuple());*/
                    ObjectModel3dToXyz(&ho_X1, &ho_Y1, &ho_Z1, hv_ObjectModel3DAffineTrans, "cartesian",
                        hv_CameraParam, hv_CPose);
                    //��������
                    FillUp(ho_Z1, &ho_RegionFillUp);
                    Connection(ho_Z1, &ho_ConnectedRegions);
                    SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 150, 99999);
                    SmallestRectangle1(ho_SelectedRegions, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
                    GenRectangle1(&ho_Rectangle, hv_Row1, hv_Column1, hv_Row2, hv_Column2);
                    //ȥ����Ե��������
                    ErosionCircle(ho_Rectangle, &ho_RegionErosion, 1.5);
                    Union1(ho_RegionErosion, &ho_StitchRegion);

                    ReduceDomain(ho_Z, ho_StitchRegion, &ho_StitchImage2D);

                    ReduceObjectModel3dByView(ho_StitchImage2D, hv_ObjectModel3D, hv_CameraParam, hv_CPose,
                        &hv_ObjectModel3DStitch);



                    FitPrimitivesObjectModel3d(hv_ObjectModel3DStitch, (HTuple("primitive_type").Append("fitting_algorithm")),
                        (HTuple("plane").Append("least_squares_tukey")), &hv_ObjectModel3DOut);
                    GetObjectModel3dParams(hv_ObjectModel3DOut, "primitive_parameter_pose", &hv_Primitive_param);
                    GenPlaneObjectModel3d(hv_Primitive_param, HTuple(), HTuple(), &hv_ObjectModel3DPlane);
                    DistanceObjectModel3d(hv_ObjectModel3DStitch, hv_ObjectModel3DPlane, HTuple(), 0,
                        "distance_to", "primitive");
                    GetObjectModel3dParams(hv_ObjectModel3DStitch, "&distance", &hv_GenParamValue);
                    TupleMean(hv_GenParamValue, &hv_Mean1);
                    TupleDeviation(hv_GenParamValue, &hv_Deviation1);
                    TupleMin(hv_GenParamValue, &hv_Min1);
                    TupleMax(hv_GenParamValue, &hv_Max1);

                }
            }
            catch (HException &hException)
            {
                CString strError;
                strError.Format(_T("CInnerCheck()-InnerCheck�쳣-HALCON- "));
                strError += hException.ProcName().Text();
                strError += _T("�쳣");
                strError += hException.ErrorMessage().Text();
                m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("�����"), strError);
                nState = 1;
            }
            catch (...)
            {
                m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck()-InnerCheck����"), _T("�㷨�쳣"));
                nState = 2;
            }
        }	

		if (nState != 0)
		{
			//�ڲ���⺯���쳣
			sStatus.strErrorContext = _T("�㷨�쳣");
			sAlgInnerSingleCheckOutputResult->sSingleErrorInfo.strErrorDescription = _T("���ʧ��");
			sAlgInnerSingleCheckOutputResult->sSingleErrorInfo.bytErrorType = ABNORMAL;
		}
		else
		{

            sAlgInnerSingleCheckOutputResult->sSingleErrorInfo.meanValue = hv_Mean1.D();
            sAlgInnerSingleCheckOutputResult->sSingleErrorInfo.DeviationValue = hv_Deviation1.D();
		}
	}
	catch (HException &hException)
	{
		CString strErr;
		CString strProcName, strErrorMessage;
		strProcName = hException.ProcName().Text();
		strErrorMessage = hException.ErrorMessage().Text();

		strErr.Format(_T("CInnerCheck()�쳣-SingleCheck-HALCON-%s�쳣-%s"), strProcName, strErrorMessage);
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath,
			_T("�쳣��־"), _T("�����"), strErr);
		sStatus.strErrorContext = _T("�㷨�쳣");
		sAlgInnerSingleCheckOutputResult->sSingleErrorInfo.strErrorDescription = _T("���ʧ��");
		sAlgInnerSingleCheckOutputResult->sSingleErrorInfo.bytErrorType = ABNORMAL;
	}
	catch (...)
	{
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck()��"), _T("�㷨�쳣"));
		sStatus.strErrorContext = _T("�㷨�쳣");
		sAlgInnerSingleCheckOutputResult->sSingleErrorInfo.strErrorDescription = _T("���ʧ��");
		sAlgInnerSingleCheckOutputResult->sSingleErrorInfo.bytErrorType = ABNORMAL;
	}

	return sStatus;
}

//������ģ��
//******************************************************************************//
//����:������ģ��
//����:NULL
//����:NULL
//	�㷨ִ��״̬�ṹ��
//******************************************************************************//
s_Status CInnerCheck::SaveCheckModel()
{
	s_Status sStatus;
	try
	{
		// ����ʵ���������Ӵ���
		WritePara();

		CString strModelImg;
		strModelImg.Format(_T("%s\\hModelImg.hobj"), m_strRegionPath);
		const char* cModelImg;
		CStringA strPathAEn(strModelImg);
		cModelImg = strPathAEn.GetString();
		WriteObject(m_hModelImg, (HTuple)cModelImg);

		//ģ������
		CString strModelReg;
		strModelReg.Format(_T("%s\\hModelReg.hobj"), m_strRegionPath);
		const char* cModelReg;
		CStringA strPathAEnReg(strModelReg);
		cModelReg = strPathAEnReg.GetString();
		WriteObject(m_hModelReg, (HTuple)cModelReg);

        //�������
        if (m_sDstRegionProperty.nROIIndex>1)
        {
            CString strModelRegs;
            strModelRegs.Format(_T("%s\\%s\\hModelDetectRegion.hobj"), m_strTaskPath, m_strAlgName);
            CStringA strPathRegs(strModelRegs);
            const char * cModelRegs = strPathRegs.GetString();
            WriteObject(m_RegionDetection, (HTuple)cModelRegs);
        }
	}
	catch (HException &hException)
	{
		sStatus.nErrorType = 1;
		sStatus.strErrorContext = _T("Halcon-SaveCheckModel�쳣");

		CString strError;
		strError.Format(_T("����ID = %d Halcon-SaveCheckModel�쳣- "), m_sDstRegionProperty.nROIIndex);
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck()��"), strError);
	}
	catch (...)
	{
		sStatus.nErrorType = 1;
		sStatus.strErrorContext = _T("SaveCheckModel()�쳣");
		// д�쳣��־
		CString strErr;
		strErr.Format(_T("����ID = %d SaveCheckModel()�쳣"), m_sDstRegionProperty.nROIIndex);
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck()��"), strErr);
	}
	return sStatus;
}

// д�����ļ�
//******************************************************************************//
//����:д�����ļ�
//����:NULL
//����:NULL
//******************************************************************************//
void CInnerCheck::WritePara()
{
	try
	{
		CString strParamVaule;
		strParamVaule.Format(_T("%s\\htParamVaule.tup"), m_strRegionPath);
		const char* cParamVaule;
		CStringA strPathAEn(strParamVaule);
		cParamVaule = strPathAEn.GetString();
		WriteTuple(m_htParamVaule, (HTuple)cParamVaule);

		//��ǲ����޸�
		SaveTempPara();
	}
	catch (HException &hException)
	{
		CString strError;
		strError.Format(_T("����ID = %d Halcon��ʼ���쳣- "), m_sDstRegionProperty.nROIIndex);
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck()��"), strError);
	}
	catch (...)
	{
		// д�쳣��־
		CString strErr;
		strErr.Format(_T("����ID = %d Init()�쳣"), m_sDstRegionProperty.nROIIndex);
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck()��"), strErr);
	}
}

// �������ļ�
//******************************************************************************//
//����:�������ļ�
//����:NULL
//����:NULL
//******************************************************************************//
void CInnerCheck::ReadPara()
{
	char strTemp[100];
	memset(strTemp, 0, 100);
	try
	{
		HTuple htState;
		//������ֵ
		InnerHalconReadHtParam(m_nChannel, &m_htParamProp, &m_htParamName, &m_htParamVaule, &m_htParamMeaning,
			&m_htParamType, &m_htParamMinVaule, &m_htParamMaxVaule, &m_htParamStep, &m_htParamChildNames, &htState);
		if (htState > 0)
		{
			CString strError;
			strError.Format(_T("InnerHalconReadHtParam������ʧ��halcon�쳣"));
			m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck��"), strError);
		}

		//halcon�����ʼ��
		InnerHalconInitHandle(m_htParamVaule, m_htHalconHandle, &m_htHalconHandle, &htState);
		if (htState > 0)
		{
			CString strError;
			strError.Format(_T("InnerHalconInitHandle��ʼ�����halcon�쳣"));
			m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck��"), strError);
		}
		//��ģ���²���
		CString strParamVaule;
		strParamVaule.Format(_T("%s\\htParamVaule.tup"), m_strRegionPath);
		const char* cParamVaule;
		if (!m_FileOperate->IsFileExist(strParamVaule))
		{
			//��Ĭ�ϲ���
			strParamVaule.Format(_T("%s\\htParamVaule.tup"), m_strDefaultParaPath);
			if (m_FileOperate->IsFileExist(strParamVaule))
			{
				CStringA strPathAEn(strParamVaule);
				cParamVaule = strPathAEn.GetString();
				ReadTuple((HTuple)cParamVaule, &m_htParamVaule);
			}
		}
		else
		{
			//��ģ���²���
			CStringA strPathAEn(strParamVaule);
			cParamVaule = strPathAEn.GetString();
			ReadTuple((HTuple)cParamVaule, &m_htParamVaule);
		}

		m_htParam.Clear();
		m_htParam.Append(m_htParamProp);
		m_htParam.Append(m_htParamName);
		m_htParam.Append(m_htParamVaule);
		m_htParam.Append(m_htParamMeaning);
		m_htParam.Append(m_htParamType);
		m_htParam.Append(m_htParamMinVaule);
		m_htParam.Append(m_htParamMinVaule);
		m_htParam.Append(m_htParamStep);
	}
	catch (HException& hException)
	{
		CString strError;
		strError.Format(_T("CInnerCheck������ʧ��halcon�쳣-"));
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck��"), strError);
	}

}

// �����Ѿ��޸��������ʱ�ļ� 
//******************************************************************************//
//����:�����Ѿ��޸��������ʱ�ļ� 
//����:NULL
//����:
//  ����ֵ
//******************************************************************************//
int CInnerCheck::SaveTempPara()
{
	CString sFileName, str;
	sFileName.Format(_T("%s\\%s\\GCIAlgStitchCheckUpdateFile.ini"), // ע���޸ĸ����ļ���
		m_strTaskPath,
		m_strAlgName);
	str.Format(_T("��%d�������޸ı�־"), m_sDstRegionProperty.nROIIndex);
	WritePrivateProfileString(_T("�޸ı�־"), str, _T("1"), sFileName);

	// ����ʵ���������Ӵ���
	// ......

	return 0;
}

// ��ȡ�Ѿ��޸��������ʱ�ļ�
//******************************************************************************//
//����:��ȡ�Ѿ��޸��������ʱ�ļ�
//����:NULL
//����:
//  ����ֵ
//******************************************************************************//	
int CInnerCheck::ReadTempPara()
{
	CString sFileName, str;
	sFileName.Format(_T("%s\\%s\\GCIAlgStitchCheckUpdateFile.ini"), // ע���޸ĸ����ļ���
		m_strTaskPath,
		m_strAlgName);
	str.Format(_T("��%d�������޸ı�־"), m_sDstRegionProperty.nROIIndex);
	int nStatus = GetPrivateProfileInt(_T("�޸ı�־"), str, 0, sFileName);
	// ��Ҫ�޸ģ���������
	if (nStatus == 1)
	{
		ReadPara();
		WritePrivateProfileString(_T("�޸ı�־"), str, _T("0"), sFileName);
	}
	return 0;
}

// �����ڴ�ռ�
//******************************************************************************//
//����:�����ڴ�ռ�
//����:NULL
//����:NULL
//******************************************************************************//
void CInnerCheck::RequistSpace()
{
	// ����ʵ���������Ӵ���
	// ......
}
// �ͷ��ڴ�ռ�
//******************************************************************************//
//����:�ͷ��ڴ�ռ�
//����:NULL
//����:NULL
//******************************************************************************//	
void CInnerCheck::FreeTMPSpace()
{
	// ����ʵ���������Ӵ���
	// ......
}

// ��������
//******************************************************************************//
//����:��������
//����:
//  sAlgImageInfo         ���룬ͼ����Ϣ
//  sDstRegionProperty    ��������ƺ�����
//  sSrcRegionProperty    ���룬����ǰ����
//����:NULL
//******************************************************************************//
s_Status CInnerCheck::AlgCopyRegion(const s_InputTimeImage& sAlgImageInfo,
	s_ModelRegionPro &sDstRegionProperty, const s_ModelRegionPro &sSrcRegionProperty)
{
	s_Status sStatus;

	// ����ʵ���������Ӵ���
	// ......

	return sStatus;
}

//��ģ�������ļ�
void CInnerCheck::ReadModelRegion(CString strRegionPath, const s_ModelRegionPro &sDstRegionProperty)
{
	try
	{
		CString strModelImg, strModelReg, strMatchId, strModelRegs;
		strModelImg.Format(_T("%s\\%s\\%s\\hModelImg.hobj"), m_strTaskPath, m_strAlgName, _T("1"));
        strModelReg.Format(_T("%s\\%s\\%s\\hModelReg.hobj"), m_strTaskPath, m_strAlgName, _T("1"));
        strMatchId.Format(_T("%s\\%s\\hModelMatchId.sfm"), m_strTaskPath, m_strAlgName);
        strModelRegs.Format(_T("%s\\%s\\hModelDetectRegion.hobj"), m_strTaskPath, m_strAlgName);

		
		const char *cModelImg, *cModelReg,*cMatchId,*cModelRegs;
		if (m_FileOperate->IsFileExist(strModelImg)
            && m_FileOperate->IsFileExist(strModelReg)
            &&m_FileOperate->IsFileExist(strMatchId)
            && m_FileOperate->IsFileExist(strModelRegs))
		{
            //��ģ��ͼ��
			CStringA strPathAEn(strModelImg);
			cModelImg = strPathAEn.GetString();
			ReadObject(&m_hModelImg, (HTuple)cModelImg);

            //��ģ������
            CStringA strPathAEnReg(strModelReg);
            cModelReg = strPathAEnReg.GetString();
            ReadObject(&m_hModelReg, (HTuple)cModelReg);

            //��ȡ���ģ��ID
            CStringA strPathMathId(strMatchId);
            cMatchId = strPathMathId.GetString();
            ReadSurfaceModel((HTuple)cMatchId, &m_hMathId);
            

            //��ȡ�������
            CStringA strPathModelRegs(strModelRegs);
            cModelRegs = strPathModelRegs.GetString();
            ReadObject(&m_RegionDetection, (HTuple)cModelRegs);

            m_bFileExist = TRUE;
		}
		else
		{
			m_bFileExist = FALSE;
		}
		
		
		
		
	}
	catch (HException &hException)
	{
		CString strError;
		strError.Format(_T("����ID = %d Halcon-��ģ�������쳣- "), m_sDstRegionProperty.nROIIndex);
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck()��"), strError);
	}
	catch (...)
	{
		// д�쳣��־
		CString strErr;
		strErr.Format(_T("����ID = %d ReadModelRegion()�쳣"), m_sDstRegionProperty.nROIIndex);
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("�쳣��־"), _T("CInnerCheck()��"), strErr);
	}
}

void CInnerCheck::getDetectionRegion()
{
    HObject hv_Regions;
    GenEmptyObj(&hv_Regions);
    if (m_sDstRegionProperty.nROIIndex > 1)
    {
        BOOL bExist = FALSE;
        if (m_regions->size() > 0)
        {
            std::list<s_ModelRegionPro>::iterator it = m_regions->begin();
            for (; it != m_regions->end(); it++)
            {
                if (m_sDstRegionProperty.nROIIndex == it->nROIIndex)
                {
                    CopyObj(m_hModelReg, &(it->hRegion), 1, 1);
                    bExist = TRUE;
                }
                ConcatObj(hv_Regions, it->hRegion, &hv_Regions);
            }
        }
        if (FALSE == bExist)
        {
            s_ModelRegionPro regModel;
            regModel.nROIIndex = m_sDstRegionProperty.nROIIndex;
            CopyObj(m_hModelReg, &(regModel.hRegion), 1, 1);
            m_regions->push_back(regModel);
            ConcatObj(hv_Regions, regModel.hRegion, &hv_Regions);
        }
        Union1(hv_Regions, &m_RegionDetection);
    }

}

// Short Description: �Զ���ģ 
int CInnerCheck::AutoCreateAlgMode(HObject ho_hModelImg, HObject *ho_hShowReg, const HTuple hv_htParamVaule,
	const HTuple hv_htResolution, HTuple *hv_htParamVauleNew, s_Status &CreateOrLearnState)
{
	try
	{
		HTuple hv_htCheckResultCode;
		HTuple hv_htState;
		InnerHalconAutoCreateAlgMode(ho_hModelImg, ho_hShowReg, &m_hModelImg, &m_hModelReg, hv_htParamVaule,
			hv_htResolution, m_htHalconHandle, &m_htParamVaule, &hv_htCheckResultCode,&m_htHalconHandle, &hv_htState);
		if (hv_htState == 0)
		{
            
			//WritePara();
			*hv_htParamVauleNew = m_htParamVaule;
            getDetectionRegion();
			SaveCheckModel();//����ʵ�������ȷ���Ƿ��
			CreateOrLearnState.nErrorType = 0;
			CreateOrLearnState.strErrorContext = _T("����ģ��ɹ�");
			//AfxMessageBox(_T("����ģ��ɹ�"));
			return 0;
		}
		else
		{
			CreateOrLearnState.nErrorType = 1;
			CreateOrLearnState.strErrorContext = _T("����ģ��ʧ��");
			//AfxMessageBox(_T("����ģ��ʧ��"));
			return 1;
		}

	}
	catch (HException &hException)
	{
		CString strError;
		strError.Format(_T("����ģ��ʧ�ܣ�,halcon- "));
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		
		CreateOrLearnState.nErrorType = 1;
		CreateOrLearnState.strErrorContext = strError;
		//AfxMessageBox(strError);
		return 1;
	}
	catch (...)
	{
		CreateOrLearnState.nErrorType = 2;
		CreateOrLearnState.strErrorContext = _T("����ģ��ʧ��");
		//AfxMessageBox(_T("����ģ��ʧ�ܣ�"));
		return 2;
	}

}
// Short Description: ģ��ѧϰ
int CInnerCheck::AutoLearnAlgMode(HObject ho_hCheckImg, s_Status &CreateOrLearnState)
{
	try
	{
		HTuple hv_htState;
		InnerHalconAutoLearnAlgMode(m_hModelImg, ho_hCheckImg, &m_hModelImg, &hv_htState);
		if (hv_htState == 0)
		{
			SaveCheckModel();
			CreateOrLearnState.nErrorType = 0;
			CreateOrLearnState.strErrorContext = _T("ѧϰģ��ɹ�");
			//AfxMessageBox(_T("ѧϰģ��ɹ�"));
			return 0;
		}
		else if (hv_htState == 2)
		{
			CreateOrLearnState.nErrorType = 0;
			CreateOrLearnState.strErrorContext = _T("���㷨��֧��ѧϰ");
			//AfxMessageBox(_T("���㷨��֧��ѧϰ"));
			return 0;
		}
		else
		{
			CreateOrLearnState.nErrorType = 1;
			CreateOrLearnState.strErrorContext = _T("ѧϰģ��ʧ��");
			//AfxMessageBox(_T("ѧϰģ��ʧ��"));
			return 1;
		}
	}
	catch (HException &hException)
	{
		CString strError;
		strError.Format(_T("ѧϰģ��ʧ�ܣ�,halcon- "));
		strError += hException.ProcName().Text();
		strError += _T("�쳣");
		strError += hException.ErrorMessage().Text();
		CreateOrLearnState.nErrorType = 1;
		CreateOrLearnState.strErrorContext = strError;
		//AfxMessageBox(strError);
		return 1;
	}
	catch (...)
	{
		CreateOrLearnState.nErrorType = 2;
		CreateOrLearnState.strErrorContext = _T("ѧϰģ��ʧ�ܣ�");
		//AfxMessageBox(_T("ѧϰģ��ʧ�ܣ�"));
		return 2;
	}
}