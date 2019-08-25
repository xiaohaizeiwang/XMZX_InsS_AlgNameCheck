/* 作者：    张波
/* 版本：    V1.0
/************************************************************************/
#pragma once
// halcon检测算法实现类
// Chapter: 建模
void InnerHalconAutoCreateAlgMode(HObject ho_hCheckImg, HObject *ho_hShowReg, HObject *ho_hModelImg,
	HObject *ho_hModelReg, HTuple hv_htParamVaule, HTuple hv_htResolution, HTuple hv_htHalconHandle,
	HTuple *hv_htParamVauleNew, HTuple *hv_htCheckResultCode, HTuple *hv_htHalconHandleNew, HTuple *hv_htState);
//生成3D相机参数
void GenCamParAreaScanDivision(HTuple hv_Focus, HTuple hv_Kappa, HTuple hv_Sx,
    HTuple hv_Sy, HTuple hv_Cx, HTuple hv_Cy, HTuple hv_ImageWidth, HTuple hv_ImageHeight,
    HTuple *hv_CameraParam);
// Chapter: 检测
void InnerHalconSingleCheck(HTuple hv_SurfaceModelIDLarge, const HObject& hImgSrc, HTuple *hv_htState);

// Chapter: 检测
void InnerHalconSingleCheck(HObject ho_hCheckImg, HObject ho_hModelImg, HObject ho_hModelReg,
    HObject *ho_hShowReg, HObject *ho_hErrorReg, HTuple hv_htParamVaule, HTuple hv_htResolution,
    HTuple hv_htHalconHandle, HTuple hv_htExpectedCode, HTuple *hv_htErrorInfor, HTuple *hv_htCheckResultCode, HTuple *hv_htState);

// Chapter: 读参
void InnerHalconReadHtParam(HTuple hv_ImgChannels, HTuple *hv_ParamProp, HTuple *hv_ParamName, HTuple *hv_ParamVaule,
	HTuple *hv_ParamMeaning, HTuple *hv_ParamType, HTuple *hv_ParamMinVaule, HTuple *hv_ParamMaxVaule,
	HTuple *hv_ParamStep, HTuple *hv_ParamChildNames, HTuple *hv_htState);
// Chapter: halcon句柄初始化
void InnerHalconInitHandle(HTuple hv_ParamVaule, HTuple hv_HalconHandle, HTuple *hv_HalconHandleNew, HTuple *hv_htState);
// Chapter: 误报学习
void InnerHalconAutoLearnAlgMode(HObject ho_hModelImg, HObject ho_hCheckImg, HObject *ho_hModelImgNew, HTuple *hv_htState);