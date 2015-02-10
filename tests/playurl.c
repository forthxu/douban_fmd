#include <curl/curl.h>
#include <mpg123.h>
#include <ao/ao.h>

#define BITS 8

mpg123_handle *mh = NULL;
ao_device *dev = NULL;


size_t play_stream(void *buffer, size_t size, size_t nmemb, void *userp)
{
    int err;
    off_t frame_offset;
    unsigned char *audio;
    size_t done;
    ao_sample_format format;
    int channels, encoding;
    long rate;

    mpg123_feed(mh, (const unsigned char*) buffer, size * nmemb);
    do {
        err = mpg123_decode_frame(mh, &frame_offset, &audio, &done);
        switch(err) {
            case MPG123_NEW_FORMAT:
                mpg123_getformat(mh, &rate, &channels, &encoding);
                format.bits = mpg123_encsize(encoding) * BITS;
                format.rate = rate;
                format.channels = channels;
                format.byte_format = AO_FMT_NATIVE;
                format.matrix = 0;//新版mpg123 结构体ao_sample_format去掉matrix：错误 (核心已转储)
                dev = ao_open_live(ao_default_driver_id(), &format, NULL);
                break;
            case MPG123_OK:
				ao_play(dev, audio, done);
                break;
            case MPG123_NEED_MORE:
				break;
            default:
				break;
        }
    } while(done > 0);

    return size * nmemb;
}
// gcc -O2 -o playurl playurl.c -lmpg123 -lao -lcurl
// ./playurl http://mr3.douban.com/201501091414/b8b9e66a7acbfd9058d93b6469d9fca7/view/song/small/p574399.mp3
int main(int argc, char *argv[])
{
    if(argc < 2)
        return 0;

    ao_initialize();
    
    mpg123_init();
    mh = mpg123_new(NULL, NULL);
    mpg123_open_feed(mh);

    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, play_stream);
    curl_easy_setopt(curl, CURLOPT_URL, argv[1]);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();

    ao_close(dev);
    ao_shutdown();

    return 0;
}