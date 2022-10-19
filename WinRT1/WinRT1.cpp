#include "pch.h"
#include <iostream>

using namespace Windows::AI::MachineLearning;
using namespace Windows::Foundation::Collections;
using namespace Windows::Graphics::Imaging;
using namespace Windows::Media;
using namespace Windows::Storage;

using namespace std;

// 전역변수
hstring modelPath;
string deviceName = "default";
hstring imagePath;
LearningModel model = nullptr;
LearningModelSession session = nullptr;
LearningModelBinding binding = nullptr;
VideoFrame imageFrame = nullptr;
string labelsFilePath;
vector<string> labels;

// forward 선언
void LoadModel();
VideoFrame LoadImageFile(hstring filePath);
void BindModel();
void EvaluateModel();
void PrintResults(IVectorView<float> results);
void LoadLabels();

int main()
{
    cout << "hello" << endl;
}
