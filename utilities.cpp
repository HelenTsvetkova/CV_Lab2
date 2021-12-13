#include "filters.h"

float absdiff_c(Mat src1, Mat src2, Mat & dst) {

    if(src1.size() != src2.size()) {
        std::cout << "Arrays aren't equal size. Please try again";
        return -1.;
    }

    std::vector<float> diffProcents;

    for(int j = 0; j < src1.rows; j++ ) {
        for(int i = 0; i < src1.cols; i++) {
            int diff = abs((int)src1.at<uchar>(Point(i, j)) - (int)src2.at<uchar>(Point(i, j)));
            dst.at<uchar>(Point(i, j)) = (uchar)diff;

            float diffProcent = 0.;
            if(src1.at<uchar>(Point(i, j))) {
                diffProcent = (float)dst.at<uchar>(Point(i, j)) / (float)src1.at<uchar>(Point(i, j));
            } else if (src2.at<uchar>(Point(i, j))) {
                diffProcent = (float)dst.at<uchar>(Point(i, j)) / (float)src2.at<uchar>(Point(i, j));
            } else diffProcent = 0.;

            diffProcents.push_back(diffProcent);
        }
    }

    std::sort(diffProcents.begin(), diffProcents.end());
    return 100. * diffProcents[diffProcents.size() / 2];
}

void displayResult(Mat img, std::string winName, float resizeRatio) {
    Mat outImg;
    outImg = img.clone();
    resize(outImg, outImg, Size(), resizeRatio, resizeRatio);
    imshow(winName, outImg);
}
