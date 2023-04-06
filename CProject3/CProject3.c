#define ABC     5
#define ADD(a,b)        a+b
#define PRINT(a)        printf("%d\n", a);
#define LOOP(v, s, e) for (int v=s;v<=e;v++){
#define ENDLOOP     }

#define CMD(NAME) \
    char NAME ## _cmd[256] = "";    \
    strcpy(NAME ## _cmd, #NAME);

#include <stdio.h>
#include <string.h>

int main()
{
    // 전문가를 위한 C
    int x = 2;
    int y = ABC;
    int z = ADD(x,y);

    printf("%d\n",z);

    LOOP(counter, 1, 10)
        PRINT(counter)
    ENDLOOP

    CMD(copy)
    CMD(paste)
    CMD(cut)

    char cmd[256];
    scanf("%s", cmd);

    if (strcmp(cmd, copy_cmd) == 0) {

    }
    if (strcmp(cmd, paste_cmd) == 0) {

    }
    if (strcmp(cmd, cut_cmd) == 0) {

    }

    return 0;
}
