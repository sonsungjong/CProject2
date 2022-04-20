#include "pch.h"
#include "CVClass.h"

int main()
{
    // cmd창 제거를 위해
    HWND hConsole = GetConsoleWindow();
    ShowWindow(hConsole, SW_HIDE);

    CVClass cvc;

    // cvc.ReadImg("res/test.png");
    // cvc.ReadVideo("res/test_video.mp4");
    // cvc.ReadVideo("D:/foreman.mp4");
    cvc.WebCam(0, 1280, 960);

    // 이미지처리 함수
    // cvc.GrayImg("res/test.png");
    // cvc.BlurImg("res/test.png");
    // cvc.CannyImg("res/test.png");
    // cvc.DilateImg("res/test.png");
    // cvc.ErodeImg("res/test.png");

    //cvc.ReadImg("res/photo.jpg", 480, 640);
    //cvc.HalfImg("res/test.png");
    //cvc.QuarterImg("res/test.png");

    // cvc.CropImg("res/test.png", 100, 100, 250, 400);

    //cvc.WhiteBackground();
    //cvc.DrawShapes();

    // 그림판으로 해당 픽셀의 위치를 찾아서 입력
    //float king[4][2] = { {529.0f, 142.0f}, {771.0f, 190.0f}, {405.0f, 395.0f}, {674.0f, 457.0f} };
    //cvc.WarpImg("res/cards.jpg", (float*)king, 250, 350);

    //float queen[4][2] = { {64.0f, 322.0f}, {341.0f, 278.0f}, {90.0f, 635.0f}, {405.0f, 575.0f} };
    //cvc.WarpImg("res/cards.jpg", (float*)queen, 350, 350);



    //ShowWindow(hConsole, SW_NORMAL);
    PostMessage(hConsole, WM_CLOSE, 0, 0);

    return 0;
}
