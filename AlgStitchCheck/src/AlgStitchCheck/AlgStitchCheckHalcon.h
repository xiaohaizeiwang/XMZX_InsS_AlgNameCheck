/* ���ߣ�    �Ų�
/* �汾��    V1.0
/************************************************************************/
#pragma once
// halcon����㷨ʵ����
// Chapter: ��ģ
void InnerHalconAutoCreateAlgMode(HObject ho_hCheckImg, HObject *ho_hShowReg, HObject *ho_hModelImg,
	HObject *ho_hModelReg, HTuple hv_htParamVaule, HTuple hv_htResolution, HTuple hv_htHalconHandle,
	HTuple *hv_htParamVauleNew, HTuple *hv_htCheckResultCode, HTuple *hv_htHalconHandleNew, HTuple *hv_htState);
//����3D�������
void GenCamParAreaScanDivision(HTuple hv_Focus, HTuple hv_Kappa, HTuple hv_Sx,
    HTuple hv_Sy, HTuple hv_Cx, HTuple hv_Cy, HTuple hv_ImageWidth, HTuple hv_ImageHeight,
    HTuple *hv_CameraParam);
// Chapter: ���
void InnerHalconSingleCheck(HTuple hv_SurfaceModelIDLarge, const HObject& hImgSrc, HTuple *hv_htState);

// Chapter: ���
void InnerHalconSingleCheck(HObject ho_hCheckImg, HObject ho_hModelImg, HObject ho_hModelReg,
    HObject *ho_hShowReg, HObject *ho_hErrorReg, HTuple hv_htParamVaule, HTuple hv_htResolution,
    HTuple hv_htHalconHandle, HTuple hv_htExpectedCode, HTuple *hv_htErrorInfor, HTuple *hv_htCheckResultCode, HTuple *hv_htState);

// Chapter: ����
void InnerHalconReadHtParam(HTuple hv_ImgChannels, HTuple *hv_ParamProp, HTuple *hv_ParamName, HTuple *hv_ParamVaule,
	HTuple *hv_ParamMeaning, HTuple *hv_ParamType, HTuple *hv_ParamMinVaule, HTuple *hv_ParamMaxVaule,
	HTuple *hv_ParamStep, HTuple *hv_ParamChildNames, HTuple *hv_htState);
// Chapter: halcon�����ʼ��
void InnerHalconInitHandle(HTuple hv_ParamVaule, HTuple hv_HalconHandle, HTuple *hv_HalconHandleNew, HTuple *hv_htState);
// Chapter: ��ѧϰ
void InnerHalconAutoLearnAlgMode(HObject ho_hModelImg, HObject ho_hCheckImg, HObject *ho_hModelImgNew, HTuple *hv_htState);