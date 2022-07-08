#define SDL_MAIN_HANDLED
extern "C"
{
#include <libavformat/avformat.h>
#include <libavdevice/avdevice.h>
}
//#include <SDL.h> 16:10 https://youtu.be/MHRmi-n79W4
#include <iostream>

int main()
{
	avdevice_register_all();
	//av_register_all();
	AVFormatContext* formatContext = NULL;
	const char* fileName = "D:\\video\\zigzag.mp4";

	//https://cpp.hotexamples.com/examples/-/-/av_find_input_format/cpp-av_find_input_format-function-examples.html
	//const char* format = "gdigrab";			// µ¥½ºÅ©Å¾ Ä¸Ã³
	const char* format = "dshow";			// À¥Ä·
	auto inputFormat = av_find_input_format(format);
	if (!inputFormat)
	{
		std::cerr << "Unknown input format: " << format << std::endl;
		return 1;
	}

	AVDictionary* format_opts = NULL;
	av_dict_set(&format_opts, "framerate", "1000", 0);
	const char* resolution = "320x240";
	av_dict_set(&format_opts, "video_size", resolution, 0);
	int err = avformat_open_input(&formatContext, fileName, NULL, NULL);
	if (err != 0)
	{
		std::cout << "Could not open input " << fileName << std::endl;
		return 2;
	}
	av_dump_format(formatContext, 0, fileName, 0);
	av_dict_free(&format_opts);



	bool done = false;
	while (!done)
	{
		AVPacket packet;
		err = av_read_frame(formatContext, &packet);
		if (err) 
		{
			done = true;
			break;
		}
		std::cout << packet.pts << std::endl;
		av_free(&packet);
	}
	avformat_free_context(formatContext);
}