/*
https://www.tensorflow.org/install/lang_c 에서 라이브러리를 다운로드 한다 (압축해제까지)
C:\libtensorflow-gpu-windows-x86_64-2.6.0\include 를 인클루드 디렉토리에 입력한다
C:\libtensorflow-gpu-windows-x86_64-2.6.0\lib 를 라이브러리 디렉토리에 입력한다
Linker에 tensorflow.lib 를 입력한다

Project -> Properties -> Configuration Properties -> Debugging -> Environment 에다가
PATH=C:\libtensorflow-gpu-windows-x86_64-2.6.0\lib;
%PATH%;

를 입력한다.
*/

#include <stdio.h>
#include <tensorflow/c/c_api.h>

int main()
{
    printf("Hello from Tensorflow C library version %s\n", TF_Version());
    printf("Hello from Tensorflow C library version %s\n", TF_Version());


}
