#ifndef FILTERS_H
#define FILTERS_H

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>

using namespace cv;

void boxFilter_c( Mat & src, Mat & dst, int ksize);
void GaussianBlur_c( Mat src, Mat & dst, int ksize, float sigmaX, float sigmaY = 0);
void Laplacian_c(Mat src, Mat & dst, int ksize, float scale, float sigma);

void unsharpMaskGaussian(Mat src, Mat & dst, int ksize, float sigma, float scale);
void unsharpMaskBox(Mat src, Mat & dst, int ksize, float scale);
void unsharpMaskLaplacian(Mat src, Mat & dst, int ksize, float scale, float sigma);

float absdiff_c(Mat src1, Mat src2, Mat & dst);
void displayResult(Mat img, std::string winName, float resizeRatio);

#endif // FILTERS_H
