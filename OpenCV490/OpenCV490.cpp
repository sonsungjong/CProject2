/*
dll ������ ����
Project -> Properties -> Configuration Properties -> Debugging -> Environment ���ٰ�

PATH=C:\opencv\build\x64\vc16\bin
;%PATH%;

�� �߰�
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

    // ���ó��
    cv::Mat grayMat;
    cv::cvtColor(mat, grayMat, cv::COLOR_BGR2GRAY);

    // ȸ�� �̹����� ���
    //cv::Mat brightGrayMat;
    //grayMat.convertTo(brightGrayMat, -1, 1.2, 0);               // ���İ��� 1.4��

    // ȸ�� �̹����� ��Ӱ�
    //cv::Mat darkGrayMat;
    //grayMat.convertTo(darkGrayMat, -1, 0.8, 0);               // ���İ��� 0.6��

    cv::Mat brighterGrayMat = grayMat.clone();
    cv::Mat darkerGrayMat = grayMat.clone();

    // �̹����� ��� �����
    for (int y = 0; y < brighterGrayMat.rows; y++) {
        for (int x = 0; x < brighterGrayMat.cols; x++) {
            int temp = brighterGrayMat.data[y* brighterGrayMat.cols + x] + 60;           // �ȼ��� ����
            if (temp > 255) {
                brighterGrayMat.data[y* brighterGrayMat.cols + x] = 255;              // 255 �ʰ� ����
            }
            else {
                brighterGrayMat.data[y * brighterGrayMat.cols + x] = (uchar)temp;
            }
        }
    }

    // �̹����� ��Ӱ� �����
    for (int y = 0; y < darkerGrayMat.rows; y++) {
        for (int x = 0; x < darkerGrayMat.cols; x++) {
            int temp = darkerGrayMat.data[y * darkerGrayMat.cols + x] - 60;           // �ȼ��� ����
            if (temp < 0) {
                darkerGrayMat.data[y * darkerGrayMat.cols + x] = 0;              // 0 �̸� ����
            }
            else {
                darkerGrayMat.data[y * darkerGrayMat.cols + x] = (uchar)temp;
            }
        }
    }

    cv::imshow("����", mat);
    cv::imshow("ȸ��", grayMat);
    //cv::imshow("���� ȸ��", brightGrayMat);
    //cv::imshow("��ο� ȸ��", darkGrayMat);
    cv::imshow("���", brighterGrayMat);
    cv::imshow("��Ӱ�", darkerGrayMat);

    cv::waitKey(0);
    
    return 0;
}
