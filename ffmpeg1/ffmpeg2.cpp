#include <cstdio>
#include <tchar.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswresample/swresample.h>
#include <libswscale/swscale.h>
#include <libavutil/avutil.h>

}

int main2()
{
	const char* filePath = "D:\\video\\zigzag.mp4";

	//av_register_all();

	avformat_network_init();

	int ret;
	AVFormatContext* pFmtCtx = NULL;

	ret = avformat_open_input(&pFmtCtx, filePath, NULL, NULL);
	if (ret != 0)
	{
		av_log(NULL, AV_LOG_ERROR, "File [%s] Open Fail (ret : %d)\n", filePath, ret);
		exit(-1);
	}
	av_log(NULL, AV_LOG_INFO, "File [%s] Open Success\n", filePath);

	ret = avformat_find_stream_info(pFmtCtx, NULL);
	if (ret < 0)
	{
		av_log(NULL, AV_LOG_ERROR, "Fail to get Stream Information \n");
		exit(-1);
	}
	av_log(NULL, AV_LOG_INFO, "Get Stream Information Success \n");

	avformat_close_input(&pFmtCtx);
	avformat_network_deinit();

	return 0;
}