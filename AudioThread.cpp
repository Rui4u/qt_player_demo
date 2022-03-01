#include "AudioThread.h"
#include <QPushButton>
#include <QDebug>
#include <QFile>

extern "C" {
// 设备
#include <libavdevice/avdevice.h>
// 格式
#include <libavformat/avformat.h>
// 工具
#include <libavutil/avutil.h>
}

// 格式名称、设备名称目前暂时使用宏定义固定死
#ifdef Q_OS_WIN
    // 格式名称
    #define FMT_NAME "dshow"
    // 设备名称
    #define DEVICE_NAME "audio=麦克风阵列 (Realtek(R) Audio)"
    // PCM文件的文件名
    #define FILENAME "F:/out.pcm"
#else
    #define FMT_NAME "avfoundation"
    #define DEVICE_NAME ":2"
     #define FILENAME "/Users/lagou/Desktop/out.pcm"
#endif

AudioThead::AudioThead(QObject *parent) : QThread(parent)
{
    // 在线程结束时自动回收线程的内存
    connect(this, &AudioThead::finished,
            this, &AudioThead::deleteLater);
}

AudioThead::~AudioThead() {
    // 线程对象的内存回收时，正常结束线程
    requestInterruption();
    quit();
    wait();
}

void AudioThead::run() {
    avdevice_register_all();
    AVInputFormat *fmt = av_find_input_format(FMT_NAME);
    if (!fmt) {
        // 如果找不到输入格式
        qDebug() << "找不到输入格式" << FMT_NAME;
        return;
    }

   AVFormatContext *context = nullptr;
   int result = avformat_open_input(&context,DEVICE_NAME,fmt,nullptr);
   if (result < 0) {
        char errorbuf[1024];
        qDebug() << av_strerror(result,errorbuf,sizeof(errorbuf));
   }

   QFile file(FILENAME);
   if (!file.open(QFile::WriteOnly)) {
        qDebug() << "文件打开失败";
        avformat_close_input(&context);
        return;
    }

   //暂定只采集50个包
   int count = 1000000000;
   int ret;
   AVPacket *pkt = av_packet_alloc();

   while (count-- > 0) {
       // 从设备中采集数据，返回值为0，代表采集数据成功
       ret = av_read_frame(context, pkt);
       qDebug() << "count =" << count;
       if (ret == 0) { // 读取成功
           // 将数据写入文件
           int a = file.write((const char *) pkt->data, pkt->size);
           qDebug() << "写入文件" << a;
           // 释放资源
           av_packet_unref(pkt);
       } else if (ret == AVERROR(EAGAIN)) { // 资源临时不可用
           qDebug() << " 资源临时不可用";
           char errbuf[1024];
           av_strerror(ret, errbuf, sizeof (errbuf));
           qDebug() << "av_read_frame error" << errbuf << ret;
           continue;
       } else { // 其他错误
           char errbuf[1024];
           av_strerror(ret, errbuf, sizeof (errbuf));
           qDebug() << "av_read_frame error" << errbuf << ret;
           break;
       }
   }
    qDebug() << "完成";
}
