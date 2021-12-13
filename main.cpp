#include <iostream>

#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

#include "filters.h"

using std::endl;
using std::cout;
using std::cin;
using std::string;
using std::stoi;
using namespace cv;


int main(int argc, char *argv[]) {

    // 1. prepare lenna  ================================================

    Mat lennaImg;

    CommandLineParser parser(argc, argv,"{@file||}");
    string filePath = parser.get<string>("@file");
    if (filePath.empty()) {
        cout << "failed to open file " << filePath << endl;
        return 1;
    }

    lennaImg = imread(samples::findFile(filePath));
    if (lennaImg.empty()) {
        cout << "--(!) Can't read this image :\n" << filePath << endl;
        return 1;
    }

    cvtColor(lennaImg, lennaImg, CV_BGR2GRAY);

    // 2. choose part of the work =========================================

    enum {
        BOXFILTER,
        GAUSSIAN,
        LAPLACIAN,
        UNSHARPMASK_G_BOX,
        UNSHARPMASK_l
    };

    int labPart = UNSHARPMASK_l;

    switch (labPart) {

    // =====================================================================

    case BOXFILTER : {

        int ksize = 5;

        cout << "-- Apply box filter." << endl;

        TickMeter tm;

        Mat lennaBlur(lennaImg.size(), lennaImg.type());
        tm.start();
        blur(lennaImg, lennaBlur, Size(ksize, ksize));
        tm.stop();
        cout << "-- Total time of opencv blur : " << tm.getTimeSec() << endl;
        tm.reset();
        displayResult(lennaBlur, "Lenna blur (opencv)", 0.3);
        waitKey();

        Mat lennaBlur_(lennaImg.size(), lennaImg.type());
        tm.start();
        boxFilter_c(lennaImg, lennaBlur_, ksize);
        tm.stop();
        cout << "-- Total time of custom box filter : " << tm.getTimeSec() << endl;
        tm.reset();
        displayResult(lennaBlur_, "Lenna box filter (custom)", 0.3);
        waitKey();

        Mat diff(lennaImg.size(), lennaImg.type());
        float diffProcent = absdiff_c(lennaBlur, lennaBlur_, diff);
        cout << "-- Median difference of custon box filter function and blur function from openCV : " << diffProcent << " %." << endl;
        displayResult(diff, "Difference of blurring", 0.3);
        waitKey();
        destroyAllWindows();

        break;

    }

    // =====================================================================

    case GAUSSIAN : {

        int ksize = 5;
        float sigmaX = 5.;

        cout << "-- Gaussian blur." << endl;

        Mat lennaGauss(lennaImg.size(), lennaImg.type());
        GaussianBlur(lennaImg, lennaGauss, Size(ksize, ksize), sigmaX);
        displayResult(lennaGauss, "Lenna Gaussian blur (opencv)", 0.3);
        waitKey();

        Mat lennaGauss_(lennaImg.size(), lennaImg.type());
        GaussianBlur_c(lennaImg, lennaGauss_, ksize, sigmaX);
        displayResult(lennaGauss_, "Lenna Gaussian blur (custom)", 0.3);
        waitKey();

        Mat diff(lennaImg.size(), lennaImg.type());
        float diffProcent = absdiff_c(lennaGauss, lennaGauss_, diff);
        cout << "-- Median difference of custon Gaussian blur function and Gaussian blur function from openCV : " << diffProcent << " %." << endl;
        displayResult(diff, "Difference of blurring", 0.3);
        waitKey();

        destroyAllWindows();

        break;
    }

    // =====================================================================

    case UNSHARPMASK_G_BOX : {

        int ksize = 5;
        float sigmaX = 5.;
        float scale = 5.;

        cout << "-- Unsharp mask with Gaussian blur." << endl;

        Mat lennaUMGauss(lennaImg.size(), lennaImg.type());
        unsharpMaskGaussian(lennaImg, lennaUMGauss, ksize, sigmaX, scale);
        displayResult(lennaUMGauss, "Unsharp mask with Gaussian blur", 0.3);
        waitKey();

        cout << "-- Unsharp mask with Box filter." << endl;

        Mat lennaUMBox(lennaImg.size(), lennaImg.type());
        unsharpMaskBox(lennaImg, lennaUMBox, ksize, scale);
        displayResult(lennaUMGauss, "Unsharp mask with Box filter", 0.3);
        waitKey();

        Mat diff(lennaImg.size(), lennaImg.type());
        float diffProcent = absdiff_c(lennaUMGauss, lennaUMBox, diff);
        cout << "-- Median difference of unsharp mask with Gaussian blur and box filter : " << diffProcent << " %." << endl;

        diff.convertTo(diff, CV_32F);
        diff = 1 + diff;
        log(diff, diff);
        cv::convertScaleAbs(diff, diff);
        cv::normalize(diff, diff, 0, 255, cv::NORM_MINMAX);

        displayResult(diff, "Difference of blurring (log)", 0.3);
        waitKey();

        destroyAllWindows();

        break;
    }

    // =====================================================================

    case LAPLACIAN : {

        int ksize = 3;
        int scale = 1;

        cout << "-- Laplacian filter." << endl;

//        Mat lennaLaplace(lennaImg.size(), lennaImg.type());
//        Laplacian(lennaImg, lennaLaplace, CV_16S, ksize, scale);
//        convertScaleAbs( lennaLaplace, lennaLaplace );
//        displayResult(lennaLaplace, "Lenna Laplacian (opencv)", 0.3);
//        waitKey();

        float sigma = 0.75;
        float scale_ = 0.9;
        Mat lennaLaplace_(lennaImg.size(), lennaImg.type());
        Laplacian_c(lennaImg, lennaLaplace_, ksize, scale_, sigma);
        displayResult(lennaLaplace_, "Lenna Laplacian (custom)", 0.3);
        waitKey();

        destroyAllWindows();

        break;
    }

    // =====================================================================

    case UNSHARPMASK_l : {

        int ksize = 3;
        float sigma = 0.75;
        float scale = 0.9;

        cout << "-- Increase sharpening with Laplace." << endl;

        Mat lennaUMLaplace(lennaImg.size(), lennaImg.type());
        Laplacian(lennaImg, lennaUMLaplace, CV_16S, ksize, scale);
        convertScaleAbs( lennaUMLaplace, lennaUMLaplace );
        displayResult(lennaUMLaplace, "Lenna Laplacian (opencv)", 0.3);
        waitKey();

        Mat lennaUMLaplace_(lennaImg.size(), lennaImg.type());
        unsharpMaskLaplacian(lennaImg, lennaUMLaplace_, ksize, scale, sigma);
        displayResult(lennaUMLaplace_, "Usharp mask Laplacian (custom)", 0.3);
        waitKey();

        break;
    }

    // =====================================================================

    default: {
        cout << "Please set the part of lab" << endl;
    }
    }

    return 0;
}
