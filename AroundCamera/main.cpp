#if 0
// Notify : Use OpenCV Version : 4.1.0-20190415.1
// Compile options (構成プロパティ - C/C++ - コマンドライン - 追加のオプション)
`pkg-config opencv --cflags` `pkg-config opencv --libs`
// Library Link (構成プロパティ - リンカー - 入力 - ライブラリの依存ファイル)
wiringPi; pthread; dl; rt;  opencv_core; opencv_video; opencv_videoio; opencv_highgui; opencv_imgproc; opencv_imgcodecs; Library
// Memo:
//  wifi アクセススキャン：sudo iwlist wlan0 scan
#endif

#define MEMORY_MAIN

#include "Include/Common.h"
#include "Logger/Thread/LogWriter.h"

static LogWriter* g_pLogWriter = NULL;

ResultEnum initialize();
void procMain();
void finalize();

int main(void)
{
    if (initialize() != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    procMain();

FINISH :
    finalize();
    return 0;
}

ResultEnum initialize()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    wiringPiSetupSys();

    g_pLogWriter = new LogWriter((char *)"AroundCamera");
    if (g_pLogWriter == NULL)
    {
        goto FINISH;
    }

    g_pLogWriter->Run();

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

void procMain()
{

}

void finalize()
{
    if (g_pLogWriter != NULL)
    {
        delete g_pLogWriter;
        g_pLogWriter = NULL;
    }
}
