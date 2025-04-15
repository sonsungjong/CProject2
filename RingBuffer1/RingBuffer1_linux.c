#ifdef __linux
#include "RingBuffer1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

static pthread_mutex_t g_mtx[256];
static pthread_cond_t  g_cv[256];
static int g_using[256] = { 0, };  // [0] not using, [1] using


#endif		__linux