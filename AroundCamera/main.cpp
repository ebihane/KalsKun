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

#include <stdio.h>
#include <stdlib.h>
#include "Include/Common.h"
#include "Logger/Logger.h"
#include "Socket/TcpServer/TcpServer.h"
#include "Socket/TcpClient/TcpClient.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Task/CameraCapture/CameraCapture.h"
#include "Task/StateSender/StateSender.h"

static Logger* g_pLogger = NULL;
static CameraCapture* g_pCameraCapture = NULL;
static StateSender* g_pStateSender = NULL;
static TcpServer* g_pCommanderSocket = NULL;

ResultEnum initialize(const char cameraNo);
void procMain();
void finalize();

int main(int argc, char* argv[])
{
    char cameraNo = 0;
    
    if (2 < argc)
    {
        cameraNo = atoi(argv[2]);
    }

    if (initialize(cameraNo) != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    procMain();

FINISH :
    finalize();
    return 0;
}

ResultEnum initialize(const char cameraNo)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    TcpClient* client = NULL;

    wiringPiSetupSys();

    g_pLogger = new Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::BOTH_OUT);
    if (g_pLogger == NULL)
    {
        goto FINISH;
    }

    pShareMemory = new ShareMemoryStr;
    if (pShareMemory == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] pShareMemory allocation failed.\n");
        goto FINISH;
    }

    StartLoggerProcess((char*)"AroundCamera");

    g_pCameraCapture = new CameraCapture(cameraNo);
    if (g_pCameraCapture == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] g_pCameraCapture allocation failed.\n");
        goto FINISH;
    }

    client = new TcpClient((char*)COMMANDER_IP_ADDRESS, AC_TO_COMMANDER_PORT);
    g_pStateSender = new StateSender(client);
    if (g_pStateSender == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] g_pStateSender allocation failed.\n");
        goto FINISH;
    }

    g_pCommanderSocket = new TcpServer(COMMANDER_TO_AC_PORT);
    if (g_pCommanderSocket == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] g_pCommanderSocket allocation failed.\n");
        goto FINISH;
    }

    if (g_pCommanderSocket->Open() != ResultEnum::NormalEnd)
    {
        char logStr[80] = { 0 };
        snprintf(&logStr[0], sizeof(logStr), "[initialize] g_pCommanderSocket Open failed. errno[%d]\n", g_pCommanderSocket->GetLastError());
        g_pLogger->LOG_ERROR(logStr);
        goto FINISH;
    }

    g_pStateSender->Run();

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

void finalize()
{
    if (g_pStateSender != NULL)
    {
        delete g_pStateSender;
        g_pStateSender = NULL;
    }

    if (g_pCameraCapture != NULL)
    {
        delete g_pCameraCapture;
        g_pCameraCapture = NULL;
    }

    if (pShareMemory != NULL)
    {
        delete pShareMemory;
        pShareMemory = NULL;
    }

    system("sudo shutdown -h now &");
}

void procMain()
{
    ResultEnum result = ResultEnum::AbnormalEnd;
    EventInfo ev = { 0 };

RECONNECT :

    g_pCommanderSocket->Disconnection();

    result = g_pCommanderSocket->Connection();
    if (result != ResultEnum::NormalEnd)
    {
        char logStr[80] = { 0 };
        snprintf(&logStr[0], sizeof(logStr), "[procMain] Socket connect failed. errno[%d]\n", g_pCommanderSocket->GetLastError());
        g_pLogger->LOG_ERROR(logStr);

        if (result == ResultEnum::Reconnect)
        {
            goto RECONNECT;
        }
        else
        {
            goto FINISH;
        }
    }

    while (1)
    {
        g_pCommanderSocket->Receive(&ev, sizeof(EventInfo));
        if (result != ResultEnum::NormalEnd)
        {
            char logStr[80] = { 0 };
            snprintf(&logStr[0], sizeof(logStr), "[procMain] Socket receive failed. errno[%d]\n", g_pCommanderSocket->GetLastError());
            g_pLogger->LOG_ERROR(logStr);

            if (result == ResultEnum::Reconnect)
            {
                goto RECONNECT;
            }
            else
            {
                goto FINISH;
            }
        }

        if (ev.Code == 666)
        {
            g_pLogger->LOG_INFO("[procMain] Finish Request receive.\n");

            g_pStateSender->Stop(10000);
            g_pCameraCapture->Stop(10000);
            ev.Result = ResultEnum::NormalEnd;
            g_pCommanderSocket->Send(&ev, sizeof(EventInfo));

            break;
        }
    }

FINISH :
    return;
}
