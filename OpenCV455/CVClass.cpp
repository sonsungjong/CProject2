#include "pch.h"
#include "CVClass.h"

using namespace cv;

CVClass::CVClass()
{
    SetTitle("Image");
}

CVClass::~CVClass()
{

}

void CVClass::SetTitle(std::string a_title)
{
    this->title = a_title;
}

void CVClass::ReadImg(std::string a_path)
{
    m_mat = imread(a_path);
    imshow(title, m_mat);
    waitKey(0);
}

// ���� ����� ����ó�� �۾� �ʿ�
void CVClass::ReadVideo(std::string a_path)
{
    VideoCapture cap(a_path);

    while (1) {
        cap.read(m_mat);
        imshow(title, m_mat);
        waitKey(20);
    }
}

void CVClass::WebCam(int a_cam_id)
{
    VideoCapture cap(a_cam_id);

    while (1) {
        cap.read(m_mat);
        imshow(title, m_mat);
        waitKey(1);
    }
}


void CVClass::GrayImg(std::string a_path)
{
    m_mat = imread(a_path);
    Mat gray;
    cvtColor(m_mat, gray, COLOR_BGR2GRAY);

    imshow(title, gray);
    waitKey(0);
}

void CVClass::BlurImg(std::string a_path)
{
    m_mat = imread(a_path);
    Mat blur;
    GaussianBlur(m_mat, blur, Size(7, 7), 5, 0);

    imshow(title, blur);
    waitKey(0);
}

void CVClass::CannyImg(std::string a_path)
{
    m_mat = imread(a_path);
    Mat canny;
    Canny(m_mat, canny, 50, 150);

    imshow(title, canny);
    waitKey(0);
}

void CVClass::DilateImg(std::string a_path)
{
    m_mat = imread(a_path);
    Mat canny;
    Canny(m_mat, canny, 50, 150);
    Mat dilation, kernel = cv::getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(canny, dilation, kernel);

    imshow(title, dilation);
    waitKey(0);
}

void CVClass::ErodeImg(std::string a_path)
{
    m_mat = imread(a_path);
    Mat canny;
    Canny(m_mat, canny, 50, 150);
    Mat dilation, imgErode, kernel = cv::getStructuringElement(MORPH_RECT, Size(3, 3));
    dilate(canny, dilation, kernel);
    erode(dilation, imgErode, kernel);

    imshow(title, imgErode);
    waitKey(0);
}

void CVClass::ReadImg(std::string a_path, int a_width, int a_height)
{
    m_mat = imread(a_path);
    Mat img_resize;
    resize(m_mat, img_resize, Size(a_width, a_height));
    imshow(title, img_resize);
    waitKey(0);
}

void CVClass::HalfImg(std::string a_path)
{
    m_mat = imread(a_path);
    Mat img_resize;
    resize(m_mat, img_resize, Size(), 0.5, 0.5);
    imshow(title, img_resize);
    waitKey(0);
}

void CVClass::QuarterImg(std::string a_path)
{
    m_mat = imread(a_path);
    Mat img_resize;
    resize(m_mat, img_resize, Size(), 0.25, 0.25);
    imshow(title, img_resize);
    waitKey(0);
}

void CVClass::CropImg(std::string a_path, int a_x, int a_y, int a_width, int a_height)
{
    m_mat = imread(a_path);
    Mat img_crop;

    cv::Rect crop_range(a_x, a_y, a_width, a_height);
    img_crop = m_mat(crop_range);
    imshow(title, img_crop);
    waitKey(0);
}