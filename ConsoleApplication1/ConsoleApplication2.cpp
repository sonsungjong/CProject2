#include <cstdio>

int main()
{
    int nx = 36;
    int ny = 0;
    int* pn = &nx;

    printf("Addr:%p\n", &nx);
    printf("Addr:%p\n", &ny);
    printf("Addr:%p\n", &pn);
    printf("Size:%d\n", sizeof(pn));
    printf("Addr:%p\n", &pn);
    
    ny = *pn;
    *pn = ny+1;
    nx = *pn + *(&ny);

    printf("%d, %d, %d\n", nx, ny, *pn);

    return 0;
}

/*
    라이브러리 구매
    
    
*/