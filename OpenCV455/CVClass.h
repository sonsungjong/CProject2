#ifndef _CV_CLASS_H_
#define _CV_CLASS_H_

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

class CVClass
{
protected:
    std::string title;
    cv::Mat m_mat;

public:
    CVClass();
    ~CVClass();
    enum class VertexEnumClass
    {
        circle = 0,
        triangle = 3,
        rectangle = 4,
    };

    void SetTitle(std::string a_title);

    // 기본 함수
    void ReadImg(std::string a_path);
    int ReadVideo(std::string a_path);
    int WebCam(int a_cam_id);

    // 이미지 함수
    void GrayImg(std::string a_path);
    void BlurImg(std::string a_path);
    void CannyImg(std::string a_path);
    void DilateImg(std::string a_path);
    void ErodeImg(std::string a_path);
    
    // 크기변경
    void ReadImg(std::string a_path, int a_width, int a_height);
    void HalfImg(std::string a_path);
    void QuarterImg(std::string a_path);
    // 자르기
    void CropImg(std::string a_path, int a_x, int a_y, int a_width, int a_height);
    void WhiteBackground();
    void DrawShapes();

};

#endif

// 프로젝트 dll 환경변수 설정
// PATH=D:\opencv\build\x64\vc15\bin;%PATH%;$(LocalDebuggerEnvironment)