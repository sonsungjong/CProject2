#include <stdio.h>
#include <math.h>
#include <Windows.h>

int main() {
    float A = 0.0f, B = 0.0f;
    float i, j;
    int k;
    float z[1760];
    char b[1760];
    printf("\x1b[2J");
    while (1)
    {
        memset(b, 32, 1760);
        memset(z, 0, 7040);
        for (j = 0.0f; j < 6.28f; j += 0.07f) {
            for (i = 0.0f; i < 6.28f; i += 0.02f) {
                float c = sin(i);
                float d = cos(j);
                float e = sin(A);
                float f = sin(j);
                float g = cos(A);
                float h = d + 2;
                float D = 1.0f / (c * h * e + f * g + 5);
                float l = cos(i);
                float m = cos(B);
                float n = sin(B);
                float t = c * h * g - f * e;
                int x = 40 + 30.0f * D * (l * h * m - t * n);
                int y = 12 + 15.0f * D * (l * h * n + t * m);
                int o = x + 80 * y;
                int N = 8.0f * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
                if (22 > y && y > 0 && x > 0 && 80 > x && D > z[o]) {
                    z[o] = D;
                    b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
                }
            }
        }
        printf("\x1b[H");
        for (k = 0; k < 1761; k++) {
            putchar(k % 80 ? b[k] : 10);
            A += 0.00004f;
            B += 0.00002f;
        }
        Sleep(30);
    }
    return 0;
}
