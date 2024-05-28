/* --------------------------------------------------------------------------- *
 * TMOQueiroz06.cpp: implementation of the TMOQueiroz06 class.   *
 * --------------------------------------------------------------------------- */

#include "TMOQueiroz06.h"

/* --------------------------------------------------------------------------- *
 * Constructor serves for describing a technique and input parameters          *
 * --------------------------------------------------------------------------- */
TMOQueiroz06::TMOQueiroz06()
{
	SetName(L"Queiroz06");
	SetDescription(L"Queiroz et al. 2006: Color to Gray and Back: Color Embedding Into Textured Gray Images");
}

TMOQueiroz06::~TMOQueiroz06()
{
}

/* --------------------------------------------------------------------------- *
 * This function converts RGB to YCbCr color space                             *
 * --------------------------------------------------------------------------- */
YCbCr TMOQueiroz06::RGBToYCbCr(double fr, double fg, double fb)
{
	double Y = (0.2989 * fr + 0.5866 * fg + 0.1145 * fb);
	double Cb = (-0.1687 * fr - 0.3313 * fg + 0.5000 * fb);
	double Cr = (0.5000 * fr - 0.4184 * fg - 0.0816 * fb);

	return YCbCr{Y, Cb, Cr};
}

/* --------------------------------------------------------------------------- *
 * This overloaded function is an implementation of your tone mapping operator *
 * --------------------------------------------------------------------------- */
int TMOQueiroz06::Transform()
{
	pSrc->Convert(TMO_RGB);
	pDst->Convert(TMO_RGB);

	double *pSourceData = pSrc->GetData();
	double *pDestinationData = pDst->GetData();

	int height = pSrc->GetWidth();
	int width = pSrc->GetHeight();
	int N = height * width;

	double *Y = (double*)calloc(N, sizeof(double));
	double *Cb = (double*)calloc(N, sizeof(double));
	double *Cr = (double*)calloc(N, sizeof(double));
	
	// Convert RGB to YCbCr
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			YCbCr ycbcrData = RGBToYCbCr(pSourceData[0], pSourceData[1], pSourceData[2]);
			Y[i * width + j] = ycbcrData.Y;
			Cb[i * width + j] = ycbcrData.Cb;
			Cr[i * width + j] = ycbcrData.Cr;

			pSourceData += 3;
		}
	}

	// Convert Cb, Cr to cv::Mat
	cv::Mat CbMat(width, height, CV_64F, Cb);
	cv::Mat CrMat(width, height, CV_64F, Cr);
	// Resize Cb and Cr to half size
	cv::Mat CbMatResized, CrMatResized;


	// Wavelet transform
	wave_object wave_obj;
	wt2_object wt;
	double *wavecoeffs;
	double *cH, *cV;
	int ir, ic;
	int level = 1; // Level of wavelet transform

	wave_obj = wave_init("db2");
	wt = wt2_init(wave_obj, "dwt", width, height, level);

	wavecoeffs = dwt2(wt, Y);

	// Replace the Horizontal coefficients with Cb
	cH = getWT2Coeffs(wt, wavecoeffs, level, "H", &ir, &ic);
	// Resize Cb to half size
	cv::resize(CbMat, CbMatResized, cv::Size(ic, ir), cv::INTER_LINEAR);
	int cHSize = ic * ir;
	for (int i = 0; i < cHSize; i++) {
		cH[i] = CbMatResized.at<double>(i);
	}

	// Replace the Vertical coefficients with Cr
	cV = getWT2Coeffs(wt, wavecoeffs, level, "V", &ir, &ic);
	// Resize Cr to half size
	cv::resize(CrMat, CrMatResized, cv::Size(ic, ir), cv::INTER_LINEAR);
	int cVSize = ic * ir;
	for (int i = 0; i <cVSize; i++) {
		cV[i] = CrMatResized.at<double>(i);
	}

	// Inverse wavelet transform
	double *outputY = (double*)calloc(N, sizeof(double));
	idwt2(wt, wavecoeffs, outputY);

	
	// Save the output to the destination image
	for (int i = 0; i < N; i++) {
		pDestinationData[0] = outputY[i];
		pDestinationData[1] = outputY[i];
		pDestinationData[2] = outputY[i];
		pDestinationData += 3;
	}


	wave_free(wave_obj);
	wt2_free(wt);
	free(wavecoeffs);
	free(outputY);
	free(Cb);
	free(Cr);
	free(Y);

	return 0;
}
