#include "filters.h"

// =====================================================================

static Mat createBoxFilterKernel(int ksize) {
    Mat kernel(ksize, ksize, CV_32F);
    for(int i = 0 ; i < ksize ; i++) {
        for(int j = 0 ; j < ksize ; j++) {
            kernel.at <float> (i, j) = 1;
        }
    }
    return kernel;
}

static Mat createGaussKernel(int ksize, float sigmaX, float sigmaY = 0) {

    Mat kernel(ksize, ksize, CV_32F);

    for (int i = 0 ; i < ksize ; i++) {
        for (int j = 0 ; j < ksize ; j++) {
            int x = i - ksize/2;
            int y = j - ksize/2;
            kernel.at<float>(Point(i, j)) =  exp(- (x*x + y*y) / (2 * sigmaX) );
        }
    }

    return kernel;
}

static Mat createLaplaceKernel(int ksize, float sigma) {

    Mat kernel(ksize, ksize, CV_32F);

    for (int i = 0 ; i < ksize ; i++) {
        for (int j = 0 ; j < ksize ; j++) {
            int x = i - ksize/2;
            int y = j - ksize/2;
            float LoG = exp(- (x*x + y*y) / (2 * sigma * sigma) );
            LoG *= (1 - ((x*x + y*y) / (2 * sigma * sigma)) );
            LoG *= - 1 / (CV_PI * pow(sigma, 4));
            kernel.at<float>(Point(i, j)) =  LoG;
        }
    }

    return kernel;
}

// =====================================================================

static float applyKernelFilter(Mat roi, Mat kernel, int ksize) {

    float sum_ = 0;
    for(int j = 0; j < ksize; j++ ) {
        for(int i = 0; i < ksize; i++) {
            sum_ += (float)roi.at<uchar>(Point(i,j)) * kernel.at<float>(j, i);
        }
    }
    return sum_ / sum(kernel)[0];
}

// =====================================================================

void boxFilter_c( Mat & src, Mat & dst, int ksize) {

    if(ksize % 2 == 0) {
        std::cout << "The kernel size must be odd. Please try again";
        return;
    }

    dst = src.clone();
    Mat kernel = createBoxFilterKernel(ksize);

    for(int y = 0; y < src.rows - ksize; y++) {
        for(int x = 0; x < src.cols - ksize; x++) {
            Rect roiRect(x, y, ksize, ksize);
            Mat roi = src(roiRect);
            dst.at<uchar>(Point(x, y)) = (uchar)applyKernelFilter(roi, kernel, ksize);
        }
    }
}

void GaussianBlur_c( Mat src, Mat & dst, int ksize, float sigmaX, float sigmaY) {

    if(ksize % 2 == 0) {
        std::cout << "The kernel size must be odd. Please try again";
        return;
    }

    dst = src.clone();
    Mat kernel = createGaussKernel(ksize, sigmaX);

    for(int y = 0; y < src.rows - ksize; y++) {
        for(int x = 0; x < src.cols - ksize; x++) {
            Rect roiRect(x, y, ksize, ksize);
            Mat roi = src(roiRect);
            dst.at<uchar>(Point(x, y)) = (uchar)applyKernelFilter(roi, kernel, ksize);
        }
    }
}

void Laplacian_c(Mat src, Mat & dst, int ksize, float scale, float sigma) {
    if(ksize % 2 == 0) {
        std::cout << "The kernel size must be odd. Please try again";
        return;
    }

    dst = src.clone();
    Mat kernel = createLaplaceKernel(ksize, sigma);

    for(int y = 0; y < src.rows - ksize; y++) {
        for(int x = 0; x < src.cols - ksize; x++) {
            Rect roiRect(x, y, ksize, ksize);
            Mat roi = src(roiRect);
            //dst.at<uchar>(Point(x, y)) = src.at<uchar>(Point(x, y)) - (uchar)(scale * applyKernelFilter(roi, kernel, ksize));
            dst.at<uchar>(Point(x, y)) = (uchar)applyKernelFilter(roi, kernel, ksize);
        }
    }
}

void unsharpMaskGaussian(Mat src, Mat & dst, int ksize, float sigma, float scale) {
    if(ksize % 2 == 0) {
        std::cout << "The kernel size must be odd. Please try again";
        return;
    }
    Mat smoothSrc = src.clone();
    GaussianBlur_c(src, smoothSrc, ksize, sigma);

    for(int y = 0; y < src.rows - ksize; y++) {
        for(int x = 0; x < src.cols - ksize; x++) {
            Rect roiRect(x, y, ksize, ksize);
            Mat roi = src(roiRect);
            dst.at<uchar>(Point(x, y)) = src.at<uchar>(Point(x, y))
                    - (uchar)(scale * (src.at<uchar>(Point(x, y)) - smoothSrc.at<uchar>(Point(x, y))));
        }
    }
}

void unsharpMaskBox(Mat src, Mat & dst, int ksize, float scale) {
    if(ksize % 2 == 0) {
        std::cout << "The kernel size must be odd. Please try again";
        return;
    }
    Mat smoothSrc = src.clone();
    boxFilter_c(src, smoothSrc, ksize);

    for(int y = 0; y < src.rows - ksize; y++) {
        for(int x = 0; x < src.cols - ksize; x++) {
            Rect roiRect(x, y, ksize, ksize);
            Mat roi = src(roiRect);
            dst.at<uchar>(Point(x, y)) = src.at<uchar>(Point(x, y))
                    - (uchar)(scale * (src.at<uchar>(Point(x, y)) - smoothSrc.at<uchar>(Point(x, y))));
        }
    }
}

void unsharpMaskLaplacian(Mat src, Mat & dst, int ksize, float scale, float sigma) {
    if(ksize % 2 == 0) {
        std::cout << "The kernel size must be odd. Please try again";
        return;
    }

    dst = src.clone();
    Mat kernel = createLaplaceKernel(ksize, sigma);

    for(int y = 0; y < src.rows - ksize; y++) {
        for(int x = 0; x < src.cols - ksize; x++) {
            Rect roiRect(x, y, ksize, ksize);
            Mat roi = src(roiRect);
            dst.at<uchar>(Point(x, y)) = src.at<uchar>(Point(x, y)) - (uchar)(scale * applyKernelFilter(roi, kernel, ksize));
        }
    }
}
