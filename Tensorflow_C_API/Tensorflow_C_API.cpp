/*
https://www.tensorflow.org/install/lang_c ���� ���̺귯���� �ٿ�ε� �Ѵ� (������������)
C:\libtensorflow-gpu-windows-x86_64-2.6.0\include �� ��Ŭ��� ���丮�� �Է��Ѵ�
C:\libtensorflow-gpu-windows-x86_64-2.6.0\lib �� ���̺귯�� ���丮�� �Է��Ѵ�
Linker�� tensorflow.lib �� �Է��Ѵ�

Project -> Properties -> Configuration Properties -> Debugging -> Environment ���ٰ�
PATH=C:\libtensorflow-gpu-windows-x86_64-2.6.0\lib;
%PATH%;

�� �Է��Ѵ�.
*/

#include <stdio.h>
#include <tensorflow/c/c_api.h>

int main()
{
    printf("Hello from Tensorflow C library version %s\n", TF_Version());
    printf("Hello from Tensorflow C library version %s\n", TF_Version());


}
