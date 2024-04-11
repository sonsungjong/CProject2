/*
dll 설정을 위해
Project -> Properties -> Configuration Properties -> Debugging -> Environment 에다가

PATH=C:\opencv\build\x64\vc16\bin
;%PATH%;

를 추가
*/

#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#ifdef _DEBUG
#pragma comment(lib, "opencv_world490d.lib")
#else
#pragma comment(lib, "opencv_world490.lib")
#endif


int main()
{
    cv::Mat mat;
    mat = cv::imread("../img/Lenna.png");

    // 흑백처리
    cv::Mat grayMat;
    cv::cvtColor(mat, grayMat, cv::COLOR_BGR2GRAY);

    // 회색 이미지를 밝게
    //cv::Mat brightGrayMat;
    //grayMat.convertTo(brightGrayMat, -1, 1.2, 0);               // 알파값을 1.4로

    // 회색 이미지를 어둡게
    //cv::Mat darkGrayMat;
    //grayMat.convertTo(darkGrayMat, -1, 0.8, 0);               // 알파값을 0.6로

    cv::Mat brighterGrayMat = grayMat.clone();
    cv::Mat darkerGrayMat = grayMat.clone();

    // 이미지를 밝게 만들기
    for (int y = 0; y < brighterGrayMat.rows; y++) {
        for (int x = 0; x < brighterGrayMat.cols; x++) {
            int temp = brighterGrayMat.data[y* brighterGrayMat.cols + x] + 60;           // 픽셀값 조정
            if (temp > 255) {
                brighterGrayMat.data[y* brighterGrayMat.cols + x] = 255;              // 255 초과 방지
            }
            else {
                brighterGrayMat.data[y * brighterGrayMat.cols + x] = (uchar)temp;
            }
        }
    }

    // 이미지를 어둡게 만들기
    for (int y = 0; y < darkerGrayMat.rows; y++) {
        for (int x = 0; x < darkerGrayMat.cols; x++) {
            int temp = darkerGrayMat.data[y * darkerGrayMat.cols + x] - 60;           // 픽셀값 조정
            if (temp < 0) {
                darkerGrayMat.data[y * darkerGrayMat.cols + x] = 0;              // 0 미만 방지
            }
            else {
                darkerGrayMat.data[y * darkerGrayMat.cols + x] = (uchar)temp;
            }
        }
    }

    cv::imshow("원본", mat);
    cv::imshow("회색", grayMat);
    //cv::imshow("밝은 회색", brightGrayMat);
    //cv::imshow("어두운 회색", darkGrayMat);
    cv::imshow("밝게", brighterGrayMat);
    cv::imshow("어둡게", darkerGrayMat);

    cv::waitKey(0);
    
    return 0;
}
