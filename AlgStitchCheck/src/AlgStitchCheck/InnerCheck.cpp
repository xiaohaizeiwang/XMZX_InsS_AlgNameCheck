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
/* 修改记录：1. 格式化框架代码，增强可读性。
	   2. 添加初始化函数Init()处理的代码，生成相关的参数保存目录以
		  及参数读取ReadPara()。
	   3. 添加待测图像生成函数GenCheckImage()处理的代码，已添加图像
		  数据校验以及按区域信息生成检测图像(hCheckImage)。
	   4. 添加检测主函数Check()处理的代码，已添加图像生成、区域仿射
		  后的坐标信息以及检测结果初始化。
	   5. 添加写参数和读参数操作的示例代码。
	   6. 为GenCheckImage()函数和Check()函数添加异常捕获，并在每一个
		  捕获节点输出详细异常信息。
	   7. 完善了每个函数的注释说明
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
// 内部检测类实现
/////////////////////////////////////////////////////////////////////////////////

// 内部检测类构造函数和析构函数
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

//初始化函数
//******************************************************************************//
//函数:初始化
//参数:
//  strTaskName        输入，任务名
//	sAlgInitParam      输入，全局路径
//  sDstRegionProperty 输入，区域属性
//  bIsNewRegion       输入，新区域标识
//返回:
//	算法执行状态结构体
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

		//根据区域号创建本区域目录 
		m_strRegionPath.Format(_T("%s\\%s\\%d"),
			m_strTaskPath, m_strAlgName, m_sDstRegionProperty.nROIIndex);
		if (m_sDstRegionProperty.nROIIndex != 0)
		{
			m_FileOperate->CreateMultiLevelPath(m_strRegionPath);
		}

		//公共库的目录
		m_strAlgPublicLib.Format(_T("%s\\AlgPublicLib\\%s"),
			m_sAlgInitParam.strAlgPath, m_strAlgName);
		m_FileOperate->CreateMultiLevelPath(m_strAlgPublicLib);

		//默认参数的目录
		m_strDefaultParaPath.Format(_T("%s\\DefaultParameter\\%s"),
			m_sAlgInitParam.strAlgPath, m_strAlgName);
		m_FileOperate->CreateMultiLevelPath(m_strDefaultParaPath);

		// 分辨率 [10/10/2018 zb]
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
		//图像通道
		m_nChannel = vcTimeImage->nChannel;
		////临时赋值
		//m_htResolution[0] = 0.1;
		//m_htResolution[1] = 0.1;
		//读取检测参数
		ReadPara();
		//读取模板区域
		ReadModelRegion(m_strRegionPath, m_sDstRegionProperty);

		CString strContent;
		strContent.Format(_T("内部检测:区域-%d初始化"), m_sDstRegionProperty.nROIIndex);
		m_FileOperate->WriteAlgOperateLog(m_strLogPath, _T("操作日志"), _T("操作日志"), strContent);
	}
	catch (HException &hException)
	{
		sStatus.nErrorType = 1;
		sStatus.strErrorContext = _T("Halcon初始化异常");

		CString strError;
		strError.Format(_T("区域ID = %d Halcon初始化异常- "), m_sDstRegionProperty.nROIIndex);
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck()类"), strError);
	}
	catch (...)
	{
		sStatus.nErrorType = 1;
		sStatus.strErrorContext = _T("Init()异常");
		// 写异常日志
		CString strErr;
		strErr.Format(_T("区域ID = %d Init()异常"), m_sDstRegionProperty.nROIIndex);
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck()类"), strErr);
	}
	return sStatus;
}

//释放空间
//******************************************************************************//
//函数:释放空间
//参数:NULL
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CInnerCheck::Free()
{
	s_Status sStatus;

	FreeTMPSpace();
	CString strContent;
	strContent.Format(_T("内部检测:区域-%d释放完成"), m_sDstRegionProperty.nROIIndex);
	m_FileOperate->WriteAlgOperateLog(m_strLogPath, _T("操作日志"), _T("操作日志"), strContent);

	return sStatus;
}
//检测图像生成函数
//******************************************************************************//
//函数:检测图像生成函数
//参数:
//	sAlgCheckInputParam    输入，检测输入结构体
//	hImgSrc				   输入，检测图像
//	hCheckImage            输出，返回HALCON类型的检测图像
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CInnerCheck::GenCheckImage(const s_InnerCheckInputParam& sAlgInnerCheckInputParam, const HObject hImgSrc, HObject& hCheckImage)
{
	s_Status sStatus;
	try
	{
		HObject hImgSrcNew;
		HObject hRegion;
		//根据定位信息进行图像的仿射变换，以矫正检测图像
		hRegion = sAlgInnerCheckInputParam.sRegionProperty.hRegion;
		if (sAlgInnerCheckInputParam.sLocGroupResult.nLocCount > 0)
		{
			if ((!sAlgInnerCheckInputParam.sLocGroupResult.bHaveError)
				&& (!m_bAllowModify))//m_bAllowModify=1,表示模板图像,不作仿射[管理层规定]
			{
				AffineTransImage(hImgSrc, &hImgSrcNew, sAlgInnerCheckInputParam.sLocGroupResult.htHomMatInv,
					"constant", "false");
			}
			else if (sAlgInnerCheckInputParam.sLocGroupResult.bHaveError)
			{
				sStatus.nErrorType = 2;
				sStatus.strErrorContext = _T("定位失败");
				CString strErr;
				strErr.Format(_T("区域ID=%d 定位失败"),
					sAlgInnerCheckInputParam.sRegionProperty.nROIIndex);
				m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck()类"), strErr);

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
		sStatus.strErrorContext = _T("Halcon生成图像异常");

		CString strError;
		strError.Format(_T("区域ID = %d Halcon生成图像异常- "), sAlgInnerCheckInputParam.sRegionProperty.nROIIndex);
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck()类"), strError);
	}
	catch (...)
	{
		sStatus.nErrorType = 1;
		sStatus.strErrorContext = _T("GenCheckImage()异常");
		// 写异常日志
		CString strErr;
		strErr.Format(_T("区域ID = %d GenCheckImage()异常"),
			sAlgInnerCheckInputParam.sRegionProperty.nROIIndex);
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck()类"), strErr);
	}

	return sStatus;
}

//检测主函数
//******************************************************************************//
//函数:检测主函数
//参数:
//	hv_ImgSrcBig      输入，待检测原图
//	hv_SurfaceModelID1    输出，返回匹配模板ID
//返回:
//	算法执行状态结构体
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
        sStatus.strErrorContext= _T("Halcon生成图像异常");

        CString strError;
        strError.Format(_T("Check-Halcon-Check异常- "));
        strError += hException.ProcName().Text();
        strError += _T("异常");
        strError += hException.ErrorMessage().Text();
        m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck()类"), strError);
    }
    return sStatus;
}




//检测主函数
//******************************************************************************//
//函数:检测主函数
//参数:
//	sAlgCheckInputParam      输入，检测输入结构体
//	sAlgCheckOutputResult    输出，返回检测结果输出结构体
//返回:
//	算法执行状态结构体
//******************************************************************************//
s_Status CInnerCheck::SingleCheck(const s_InnerCheckInputParam& sAlgInnerCheckInputParam, const HObject hImgSrc, s_InnerSingleCheckOutputParam *sAlgInnerSingleCheckOutputResult)
{
	s_Status sStatus;
	try
	{
		//  获取HObject类型的图像数据hCheckImage
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
            strError.Format(_T("CInnerCheck()-InnerCheck-halcon异常 输入图像通道数错误"));
            m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck类"), strError);
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
                    //生成区域
                    FillUp(ho_Z1, &ho_RegionFillUp);
                    Connection(ho_Z1, &ho_ConnectedRegions);
                    SelectShape(ho_ConnectedRegions, &ho_SelectedRegions, "area", "and", 150, 99999);
                    SmallestRectangle1(ho_SelectedRegions, &hv_Row1, &hv_Column1, &hv_Row2, &hv_Column2);
                    GenRectangle1(&ho_Rectangle, hv_Row1, hv_Column1, hv_Row2, hv_Column2);
                    //去掉边缘部分区域
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
                strError.Format(_T("CInnerCheck()-InnerCheck异常-HALCON- "));
                strError += hException.ProcName().Text();
                strError += _T("异常");
                strError += hException.ErrorMessage().Text();
                m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("检测类"), strError);
                nState = 1;
            }
            catch (...)
            {
                m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck()-InnerCheck函数"), _T("算法异常"));
                nState = 2;
            }
        }	

		if (nState != 0)
		{
			//内部检测函数异常
			sStatus.strErrorContext = _T("算法异常");
			sAlgInnerSingleCheckOutputResult->sSingleErrorInfo.strErrorDescription = _T("检测失败");
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

		strErr.Format(_T("CInnerCheck()异常-SingleCheck-HALCON-%s异常-%s"), strProcName, strErrorMessage);
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath,
			_T("异常日志"), _T("检测类"), strErr);
		sStatus.strErrorContext = _T("算法异常");
		sAlgInnerSingleCheckOutputResult->sSingleErrorInfo.strErrorDescription = _T("检测失败");
		sAlgInnerSingleCheckOutputResult->sSingleErrorInfo.bytErrorType = ABNORMAL;
	}
	catch (...)
	{
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck()类"), _T("算法异常"));
		sStatus.strErrorContext = _T("算法异常");
		sAlgInnerSingleCheckOutputResult->sSingleErrorInfo.strErrorDescription = _T("检测失败");
		sAlgInnerSingleCheckOutputResult->sSingleErrorInfo.bytErrorType = ABNORMAL;
	}

	return sStatus;
}

//保存检测模板
//******************************************************************************//
//函数:保存检测模板
//参数:NULL
//返回:NULL
//	算法执行状态结构体
//******************************************************************************//
s_Status CInnerCheck::SaveCheckModel()
{
	s_Status sStatus;
	try
	{
		// 根据实际情况，添加代码
		WritePara();

		CString strModelImg;
		strModelImg.Format(_T("%s\\hModelImg.hobj"), m_strRegionPath);
		const char* cModelImg;
		CStringA strPathAEn(strModelImg);
		cModelImg = strPathAEn.GetString();
		WriteObject(m_hModelImg, (HTuple)cModelImg);

		//模板区域
		CString strModelReg;
		strModelReg.Format(_T("%s\\hModelReg.hobj"), m_strRegionPath);
		const char* cModelReg;
		CStringA strPathAEnReg(strModelReg);
		cModelReg = strPathAEnReg.GetString();
		WriteObject(m_hModelReg, (HTuple)cModelReg);

        //检测区域
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
		sStatus.strErrorContext = _T("Halcon-SaveCheckModel异常");

		CString strError;
		strError.Format(_T("区域ID = %d Halcon-SaveCheckModel异常- "), m_sDstRegionProperty.nROIIndex);
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck()类"), strError);
	}
	catch (...)
	{
		sStatus.nErrorType = 1;
		sStatus.strErrorContext = _T("SaveCheckModel()异常");
		// 写异常日志
		CString strErr;
		strErr.Format(_T("区域ID = %d SaveCheckModel()异常"), m_sDstRegionProperty.nROIIndex);
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck()类"), strErr);
	}
	return sStatus;
}

// 写参数文件
//******************************************************************************//
//函数:写参数文件
//参数:NULL
//返回:NULL
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

		//标记参数修改
		SaveTempPara();
	}
	catch (HException &hException)
	{
		CString strError;
		strError.Format(_T("区域ID = %d Halcon初始化异常- "), m_sDstRegionProperty.nROIIndex);
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck()类"), strError);
	}
	catch (...)
	{
		// 写异常日志
		CString strErr;
		strErr.Format(_T("区域ID = %d Init()异常"), m_sDstRegionProperty.nROIIndex);
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck()类"), strErr);
	}
}

// 读参数文件
//******************************************************************************//
//函数:读参数文件
//参数:NULL
//返回:NULL
//******************************************************************************//
void CInnerCheck::ReadPara()
{
	char strTemp[100];
	memset(strTemp, 0, 100);
	try
	{
		HTuple htState;
		//参数赋值
		InnerHalconReadHtParam(m_nChannel, &m_htParamProp, &m_htParamName, &m_htParamVaule, &m_htParamMeaning,
			&m_htParamType, &m_htParamMinVaule, &m_htParamMaxVaule, &m_htParamStep, &m_htParamChildNames, &htState);
		if (htState > 0)
		{
			CString strError;
			strError.Format(_T("InnerHalconReadHtParam读参数失败halcon异常"));
			m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck类"), strError);
		}

		//halcon句柄初始化
		InnerHalconInitHandle(m_htParamVaule, m_htHalconHandle, &m_htHalconHandle, &htState);
		if (htState > 0)
		{
			CString strError;
			strError.Format(_T("InnerHalconInitHandle初始化句柄halcon异常"));
			m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck类"), strError);
		}
		//读模板下参数
		CString strParamVaule;
		strParamVaule.Format(_T("%s\\htParamVaule.tup"), m_strRegionPath);
		const char* cParamVaule;
		if (!m_FileOperate->IsFileExist(strParamVaule))
		{
			//读默认参数
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
			//读模板下参数
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
		strError.Format(_T("CInnerCheck读参数失败halcon异常-"));
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck类"), strError);
	}

}

// 保存已经修改区域的临时文件 
//******************************************************************************//
//函数:保存已经修改区域的临时文件 
//参数:NULL
//返回:
//  整型值
//******************************************************************************//
int CInnerCheck::SaveTempPara()
{
	CString sFileName, str;
	sFileName.Format(_T("%s\\%s\\GCIAlgStitchCheckUpdateFile.ini"), // 注意修改更新文件名
		m_strTaskPath,
		m_strAlgName);
	str.Format(_T("第%d个区域修改标志"), m_sDstRegionProperty.nROIIndex);
	WritePrivateProfileString(_T("修改标志"), str, _T("1"), sFileName);

	// 根据实际情况，添加代码
	// ......

	return 0;
}

// 读取已经修改区域的临时文件
//******************************************************************************//
//函数:读取已经修改区域的临时文件
//参数:NULL
//返回:
//  整型值
//******************************************************************************//	
int CInnerCheck::ReadTempPara()
{
	CString sFileName, str;
	sFileName.Format(_T("%s\\%s\\GCIAlgStitchCheckUpdateFile.ini"), // 注意修改更新文件名
		m_strTaskPath,
		m_strAlgName);
	str.Format(_T("第%d个区域修改标志"), m_sDstRegionProperty.nROIIndex);
	int nStatus = GetPrivateProfileInt(_T("修改标志"), str, 0, sFileName);
	// 需要修改，则读入参数
	if (nStatus == 1)
	{
		ReadPara();
		WritePrivateProfileString(_T("修改标志"), str, _T("0"), sFileName);
	}
	return 0;
}

// 申请内存空间
//******************************************************************************//
//函数:申请内存空间
//参数:NULL
//返回:NULL
//******************************************************************************//
void CInnerCheck::RequistSpace()
{
	// 根据实际情况，添加代码
	// ......
}
// 释放内存空间
//******************************************************************************//
//函数:释放内存空间
//参数:NULL
//返回:NULL
//******************************************************************************//	
void CInnerCheck::FreeTMPSpace()
{
	// 根据实际情况，添加代码
	// ......
}

// 复制区域
//******************************************************************************//
//函数:复制区域
//参数:
//  sAlgImageInfo         输入，图像信息
//  sDstRegionProperty    输出，复制后区域
//  sSrcRegionProperty    输入，复制前区域
//返回:NULL
//******************************************************************************//
s_Status CInnerCheck::AlgCopyRegion(const s_InputTimeImage& sAlgImageInfo,
	s_ModelRegionPro &sDstRegionProperty, const s_ModelRegionPro &sSrcRegionProperty)
{
	s_Status sStatus;

	// 根据实际情况，添加代码
	// ......

	return sStatus;
}

//读模板区域文件
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
            //读模板图像
			CStringA strPathAEn(strModelImg);
			cModelImg = strPathAEn.GetString();
			ReadObject(&m_hModelImg, (HTuple)cModelImg);

            //读模板区域
            CStringA strPathAEnReg(strModelReg);
            cModelReg = strPathAEnReg.GetString();
            ReadObject(&m_hModelReg, (HTuple)cModelReg);

            //读取检测模板ID
            CStringA strPathMathId(strMatchId);
            cMatchId = strPathMathId.GetString();
            ReadSurfaceModel((HTuple)cMatchId, &m_hMathId);
            

            //读取检测区域
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
		strError.Format(_T("区域ID = %d Halcon-读模板区域异常- "), m_sDstRegionProperty.nROIIndex);
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck()类"), strError);
	}
	catch (...)
	{
		// 写异常日志
		CString strErr;
		strErr.Format(_T("区域ID = %d ReadModelRegion()异常"), m_sDstRegionProperty.nROIIndex);
		m_FileOperate->WriteAlgExceptionLog(m_strLogPath, _T("异常日志"), _T("CInnerCheck()类"), strErr);
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

// Short Description: 自动建模 
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
			SaveCheckModel();//根据实际情况，确定是否打开
			CreateOrLearnState.nErrorType = 0;
			CreateOrLearnState.strErrorContext = _T("创建模板成功");
			//AfxMessageBox(_T("创建模板成功"));
			return 0;
		}
		else
		{
			CreateOrLearnState.nErrorType = 1;
			CreateOrLearnState.strErrorContext = _T("创建模板失败");
			//AfxMessageBox(_T("创建模板失败"));
			return 1;
		}

	}
	catch (HException &hException)
	{
		CString strError;
		strError.Format(_T("创建模板失败！,halcon- "));
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		
		CreateOrLearnState.nErrorType = 1;
		CreateOrLearnState.strErrorContext = strError;
		//AfxMessageBox(strError);
		return 1;
	}
	catch (...)
	{
		CreateOrLearnState.nErrorType = 2;
		CreateOrLearnState.strErrorContext = _T("创建模板失败");
		//AfxMessageBox(_T("创建模板失败！"));
		return 2;
	}

}
// Short Description: 模板学习
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
			CreateOrLearnState.strErrorContext = _T("学习模板成功");
			//AfxMessageBox(_T("学习模板成功"));
			return 0;
		}
		else if (hv_htState == 2)
		{
			CreateOrLearnState.nErrorType = 0;
			CreateOrLearnState.strErrorContext = _T("此算法不支持学习");
			//AfxMessageBox(_T("此算法不支持学习"));
			return 0;
		}
		else
		{
			CreateOrLearnState.nErrorType = 1;
			CreateOrLearnState.strErrorContext = _T("学习模板失败");
			//AfxMessageBox(_T("学习模板失败"));
			return 1;
		}
	}
	catch (HException &hException)
	{
		CString strError;
		strError.Format(_T("学习模板失败！,halcon- "));
		strError += hException.ProcName().Text();
		strError += _T("异常");
		strError += hException.ErrorMessage().Text();
		CreateOrLearnState.nErrorType = 1;
		CreateOrLearnState.strErrorContext = strError;
		//AfxMessageBox(strError);
		return 1;
	}
	catch (...)
	{
		CreateOrLearnState.nErrorType = 2;
		CreateOrLearnState.strErrorContext = _T("学习模板失败！");
		//AfxMessageBox(_T("学习模板失败！"));
		return 2;
	}
}