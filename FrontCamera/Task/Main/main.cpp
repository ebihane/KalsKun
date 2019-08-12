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
#include "Logger/Logger.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "MasterMain.h"
#include "SlaveMain.h"
#include "KinectMain.h"

ResultEnum initialize(const char controllerType);
void finalize();

/* ./FrontCamera.out (使用デバイス番号) */
int main(int argc, char* argv[])
{
    char deviceNo = 0;
    char cameraNo = 0;

    if (2 < argc)
    {
        deviceNo = atoi(argv[1]);
    }

    if (3 < argc)
    {
        cameraNo = atoi(argv[2]);
    }

    if (initialize(deviceNo) != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    switch (deviceNo)
    {
        case 0 :
            kinectMain(cameraNo);
            break;

        case 1 :
            masterMain(cameraNo);
            break;

        case 2 :
            slaveMain(cameraNo);
            break;

        default :
            printf("[FrontCamera::main] invalid deviceNo. [%d]\n", deviceNo);
            break;
    }

FINISH:
    finalize();
    return 0;
}

ResultEnum initialize(const char controllerType)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    wiringPiSetupSys();

    pShareMemory = new ShareMemoryStr();
    if (pShareMemory == NULL)
    {
        goto FINISH;
    }

    if (controllerType == 0)
    {
        StartLoggerProcess((char *)"KinectCamera");
    }
    else if (controllerType == 1)
    {
        StartLoggerProcess((char *)"FrontCameraM");
    }
    else if (controllerType == 2)
    {
        StartLoggerProcess((char*)"FrontCameraS");
    }
    else
    {
        goto FINISH;
    }

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

void finalize()
{
    if (pShareMemory != NULL)
    {
        delete pShareMemory;
        pShareMemory = NULL;
    }
}
