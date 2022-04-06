#include "pch.h"
#include "CVClass.h"

using namespace cv;

CVClass::CVClass()
{
    SetTitle("by SungjongSon");
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

int CVClass::ReadVideo(std::string a_path)
{
    VideoCapture cap(a_path);

    if (!cap.isOpened()) {
        return -1;
    }

    while (1) {
        cap.read(m_mat);
        if (m_mat.empty()) {
            //return 1;                     // 한번만하고 종료하고 싶을 때
            cap.open(a_path);
            continue;
        }
        imshow(title, m_mat);
        if (waitKey(25) == 27) {
            break;
        }
        else if (getWindowProperty(title, WND_PROP_VISIBLE) < 1) {
            break;
        }
    }
    return 0;
}

int CVClass::WebCam(int a_cam_id, int a_width, int a_height)
{
    VideoCapture cap(a_cam_id);

    if (!cap.isOpened()) {
        return -1;
    }

    while (1) {
        cap.read(m_mat);
        //printf("%d, %d\n", cvRound(cap.get(CAP_PROP_FRAME_WIDTH)), cvRound(cap.get(CAP_PROP_FRAME_HEIGHT)));
        
        flip(m_mat, m_mat, 1);                                                         // 1: 좌우반전
        resize(m_mat, m_mat, cv::Size(a_width, a_height));              // 크기변경

        imshow(title, m_mat);
        if (waitKey(1) == 27) {
            break;
        }
        else if (getWindowProperty(title, WND_PROP_VISIBLE) < 1) {
            break;
        }
    }
    return 0;
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

void CVClass::WhiteBackground()
{
    Mat blank(512, 512, CV_8UC3, Scalar(255, 255, 255));

    imshow(title, blank);
    waitKey(0);
}

void CVClass::DrawShapes()
{
    Mat background(512, 512, CV_8UC3, Scalar(255, 255, 255));

    circle(background, Point(256, 256), 155, Scalar(0, 69, 255), FILLED);
    rectangle(background, Point(130, 226), Point(382, 286), Scalar(255, 255, 255), 3);
    line(background, Point(130, 296), Point(382, 296), Scalar(255, 255, 255), 2);

    putText(background, "sonsungjong", Point(137, 262), FONT_HERSHEY_PLAIN, 2.25, Scalar(255, 0, 0), 2);

    imshow(title, background);
    waitKey(0);
}

void CVClass::WarpImg(std::string a_path, float* a_four_point, float a_width, float a_height)
{
    cv::Mat matrix, imgWarp;
    m_mat = cv::imread(a_path);
    cv::Point2f destination[4] = { {0.0f, 0.0f}, {a_width, 0.0f}, {0.0f, a_height}, {a_width, a_height} };
    matrix = cv::getPerspectiveTransform((cv::Point2f*)(a_four_point), destination);
    cv::warpPerspective(m_mat, imgWarp, matrix, cv::Point(a_width, a_height));

    imshow("Image Warp", imgWarp);
    cv::waitKey(0);
}