#pragma once


#include <opencv2/calib3d/calib3d.hpp>
#include "Cfg.h"

//class for image points container
class PtObj{
public:
	std::vector<cv::Point2d> pt3dVecMin;
	std::vector<cv::Point2d> pt3dVecMax;
	std::vector<cv::Point2d> randPt3dVec;

	bool isVectorReady = false;

	cv::Mat HomoMat;

	double ReprojErr;
	double ProjErr;
	double dReprojErr;

	int getSize(){
		if(isVectorReady) 
			return pt3dVecMin.size();
		else 
			return -1;
	}
	cv::Point2d getRand3dPt(int i){return randPt3dVec[i];}
	double getReprojErr(){return ReprojErr;}
	double getProjErr(){return ProjErr;}
	double getDistReprojErr(){return dReprojErr;}
	cv::Mat getHomoMat(){return HomoMat;}

	void setVectorReady(){isVectorReady = true;}
	void setHomoMat(cv::Mat mat){HomoMat= mat;}
	void setReprojErr(double reprojErr){ReprojErr=reprojErr;}
	void setProjErr(double projErr){ProjErr=projErr;}
	void setDistReprojErr(double distReprojErr){dReprojErr=distReprojErr;}

	void initPts();


};

// camera calibrator
class CCamCal
{
public:
	CCamCal(void);
	~CCamCal(void);

	//! initializes the calibrator
	void initialize(CCfg oCfg, cv::Mat oImgFrm);
	//! perform camera calibration
	void process(void);
	//! output homography matrix and display image
	void output(void);

private:
	//! runs all calibration types
	void runAllCalTyp(std::vector<cv::Point2d> vo3dPt, std::vector<cv::Point2d> vo2dPt);
	//! calculates reprojection error
	double calcReprojErr2D(std::vector<cv::Point2d> vo3dPt, std::vector<cv::Point2d> vo2dPt, cv::Mat oHomoMat, int nCalTyp, double fCalRansacReprojThld);
	//
	double calcReprojErr3D(std::vector<cv::Point2d> vo3dPt, std::vector<cv::Point2d> vo2dPt, cv::Mat oHomoMat, int nCalTyp, double fCalRansacReprojThld);
	//
	double calcReprojErr(std::vector<cv::Point2d> vo3dPt, std::vector<cv::Point2d> vo2dPt, cv::Mat oHomoMat, int nCalTyp, double fCalRansacReprojThld, std::string mode = "2D3D");
	//! outputs text file of homography matrix
	void outTxt(void);
	//! plots a display grid on the ground plane
	void pltDispGrd(void);
	//
	PtObj initEdaParamRng(std::vector<cv::Point2d> m_vo3dPt);
	//
	void calCamEdaOpt(void);
	//
	PtObj estEdaParamRng(std::vector<PtObj>* pvoPtParams);
	//
	double calcDistReprojErr(std::vector<cv::Point2d> vo3dPt, std::vector<cv::Point2d> vo2dPt, cv::Mat oHomoMat, int nCalTyp, double fCalRansacReprojThld);
	//
	std::vector<cv::Point2d> initPts(PtObj sPtParamsObj);

	//! configuration parameters
	CCfg m_oCfg;
	//! frame image
	cv::Mat m_oImgFrm;
	//! list of 3D points for PnP
	std::vector<cv::Point2d> m_vo3dPt;
	//! list of 2D points for PnP
	std::vector<cv::Point2d> m_vo2dPt;
	//! homography matrix
	cv::Mat m_oHomoMat;
	//! reprojection error
	double m_fReprojErr;
	//! Distance Error
	double dReprojErr;
	//! projection error
	double m_projErr;
};

// selector of 2D points for PnP
class C2dPtSel
{
public:
	C2dPtSel(void);
	~C2dPtSel(void);

	//! initializes the 2D point selector
	void initialize(CCfg oCfg, cv::Mat oImgFrm);
	//! selects 2D points
	std::vector<cv::Point> process(void);
	//! pushes a node to the list and draw the circle
	void addNd(int nX, int nY);
	//! checks if the background image is loaded
	inline bool chkImgLd(void)
	{
		if (!m_oImgFrm.empty())
			return true;
		else
			return false;
	}

private:
	//! configuration parameters
	CCfg m_oCfg;
	//! frame image for plotting results
	cv::Mat m_oImgFrm;
	//! list of nodes
	std::vector<cv::Point> m_voNd;
};

extern C2dPtSel o2dPtSel;
