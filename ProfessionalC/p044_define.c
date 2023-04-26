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
// 전문가를 위한 C
int main_044()
{
    // 매크로
    int x = 2;
    int y = ABC;
    int z = ADD(x, y);

    printf("%d\n", z);

    LOOP(counter, 1, 10)
        PRINT(counter)
    ENDLOOP

    CMD(copy)           // char copy_cmd[256] = ""; strcpy(copy_cmd, "copy");
    CMD(paste)          // char paste_cmd[256] = ""; strcpy(paste_cmd, "paste");
    CMD(cut)                // char cut_cmd[256] = ""; strcpy(cut_cmd, "cut");

    char cmd[256] = { 0, };
    (void)scanf("%s", cmd);

    if (strcmp(cmd, copy_cmd) == 0) {

    }
    if (strcmp(cmd, paste_cmd) == 0) {

    }
    if (strcmp(cmd, cut_cmd) == 0) {

    }

    return 0;
}
