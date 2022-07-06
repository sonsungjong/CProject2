#include <cstdio>
#include <tchar.h>

extern "C"{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>
}

int main()
{
    av_log(NULL, AV_LOG_INFO, "FFFFFFFF\n");

    return 0;
}
