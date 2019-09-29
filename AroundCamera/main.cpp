#if 0
// Notify : Use OpenCV Version : 4.1.0-20190415.1
// Compile options (�\���v���p�e�B - C/C++ - �R�}���h���C�� - �ǉ��̃I�v�V����)
`pkg-config opencv --cflags` `pkg-config opencv --libs`
// Library Link (�\���v���p�e�B - �����J�[ - ���� - ���C�u�����̈ˑ��t�@�C��)
wiringPi; pthread; dl; rt;  opencv_core; opencv_video; opencv_videoio; opencv_highgui; opencv_imgproc; opencv_imgcodecs; Library
// Memo:
//  wifi �A�N�Z�X�X�L�����Fsudo iwlist wlan0 scan
#endif

#define MEMORY_MAIN

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "Include/Common.h"
#include "Parts/AroundCameraCommon.h"

/* Parts */
#include "Logger/Logger.h"
#include "Socket/TcpClient/TcpClient.h"
#include "Parts/ShareMemory/ShareMemory.h"

/* Task */
#include "Task/CameraCapture/CameraCapture.h"
#include "Task/StateSender/StateSender.h"
#include "Task/RedwavePatrol/RedwavePatrol.h"
#include "Task/HeartBeat/HeartBeatManager.h"
#include "Task/ErrorLed/ErrorLedManager.h"

static Logger* g_pLogger = NULL;
static CameraCapture* g_pCameraCapture = NULL;
static StateSender* g_pStateSender = NULL;
static RedwavePatrol* g_pRedwavePatrol = NULL;
static HeartBeatManager* g_pHeartBeat = NULL;
static ErrorLedManager* g_pErrorLedManager = NULL;

ResultEnum initialize(const char cameraNo);
void procMain(const char isShow);
void finalize();
void signalHandler(int signum);

int main(int argc, char* argv[])
{
    bool isMainLoopExit = false;
    char cameraNo = 0;
    char isShow = 0;
    
    signal(SIGKILL, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGABRT, signalHandler);

    if (2 <= argc)
    {
        cameraNo = atoi(argv[1]);
    }

    if (3 <= argc)
    {
        isShow = atoi(argv[2]);
    }

    if (initialize(cameraNo) != ResultEnum::NormalEnd)
    {
        goto FINISH;
    }

    procMain(isShow);

    isMainLoopExit = true;

FINISH :
    finalize();

    if (isMainLoopExit == true)
    {
        system("sudo shutdown -h now &");
    }

    return 0;
}

ResultEnum initialize(const char cameraNo)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    TcpClient* client = NULL;
    long lCnt = 0;

    /* I/O ������ */
    wiringPiSetupSys();
    for (lCnt = 0; lCnt < GPIO_USE_PIN_COUNT; lCnt++)
    {
        /* ���o�͕����w�� */
        pinMode(GPIO_INFO_TABLE[lCnt].PinNo, GPIO_INFO_TABLE[lCnt].Mode);
    }

    /* ���L���������� */
    pShareMemory = new ShareMemoryStr;
    if (pShareMemory == NULL)
    {
        goto FINISH;
    }

    /* �f�[�^������ */
    pShareMemory->SystemError = false;
    pShareMemory->Detect = DetectTypeEnum::NOT_DETECT;

    /* Log Process �N�� */
    StartLoggerProcess((char*)"AroundCamera");

    /* Log Process �����҂� */
    delay(100);

    /* Log Accessor ���� */
    g_pLogger = new Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::BOTH_OUT);
    if (g_pLogger == NULL)
    {
        goto FINISH;
    }

    /* �n�[�g�r�[�g����X���b�h ������ */
    g_pHeartBeat = new HeartBeatManager();
    if (g_pHeartBeat == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] g_pHeartBeat allocation failed.\n");
        goto FINISH;
    }

    /* 360���J������荞�݃X���b�h ������ */
    g_pCameraCapture = new CameraCapture(cameraNo);
    if (g_pCameraCapture == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] g_pCameraCapture allocation failed.\n");
        goto FINISH;
    }

    /* �i�ߓ��}�C�R���ւ̏�񑗐M�X���b�h ������ */
    client = new TcpClient((char*)COMMANDER_IP_ADDRESS, AC_TO_COMMANDER_PORT);
    g_pStateSender = new StateSender(client);
    if (g_pStateSender == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] g_pStateSender allocation failed.\n");
        goto FINISH;
    }

    /* �ُ��� LED ����X���b�h ������ */
    g_pErrorLedManager = new ErrorLedManager();
    if (g_pErrorLedManager == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] g_pErrorLedManager allocation failed.\n");
        goto FINISH;
    }

    g_pHeartBeat->Run();
    g_pStateSender->Run();
    g_pCameraCapture->Run();
    g_pErrorLedManager->Run();

    retVal = ResultEnum::NormalEnd;

FINISH:
    return retVal;
}

void finalize()
{
    if (g_pErrorLedManager != NULL)
    {
        g_pErrorLedManager->Stop(5);
        delete g_pErrorLedManager;
        g_pErrorLedManager = NULL;
    }

    if (g_pRedwavePatrol != NULL)
    {
        g_pRedwavePatrol->Stop(10);
        delete g_pRedwavePatrol;
        g_pRedwavePatrol = NULL;
    }

    if (g_pStateSender != NULL)
    {
        g_pStateSender->Stop(10);
        delete g_pStateSender;
        g_pStateSender = NULL;
    }

    if (g_pCameraCapture != NULL)
    {
        g_pCameraCapture->Stop(10);
        delete g_pCameraCapture;
        g_pCameraCapture = NULL;
    }

    if (g_pHeartBeat != NULL)
    {
        g_pHeartBeat->Stop(10);
        delete g_pHeartBeat;
        g_pHeartBeat = NULL;
    }

    if (pShareMemory != NULL)
    {
        delete pShareMemory;
        pShareMemory = NULL;
    }

    StopLoggerProcess();

    if (g_pLogger != NULL)
    {
        delete g_pLogger;
        g_pLogger = NULL;
    }
}

void procMain(const char isShow)
{
    Stopwatch watch;
    Stopwatch yakeiWatch;
    Stopwatch returnWatch;
    int kusakari = 0;
    int yakei = 0;
    int key = 0;
    bool isYakei = false;
    char log[80] = { 0 };

    snprintf(&log[0], sizeof(log), "[procMain] Main Loop enter. isShow[%d]\n", isShow);
    g_pLogger->LOG_INFO(log);

    watch.Start();
    while (1)
    {
        if (60.0f <= watch.GetSplit())
        {
            int shutdown1 = digitalRead(IO_SHUTDOWN_1);
            int shutdown2 = digitalRead(IO_SHUTDOWN_2);
            if ((shutdown1 == LOW) && (shutdown2 == LOW))
            {
                g_pLogger->LOG_INFO("[procMain] Shutdown!!!!\n");
                break;
            }
        }

        kusakari = digitalRead(IO_KUSAKARI_MODE);
        yakei = digitalRead(IO_YAKEI_MODE);

        if ((kusakari == LOW) && (yakei == HIGH))
        {
            returnWatch.Stop();

            if (isYakei == false)
            {
                if (yakeiWatch.IsRunninng() == false)
                {
                    yakeiWatch.Start();
                }

                if (1.0 <= yakeiWatch.GetSplit())
                {
                    isYakei = true;
                    yakeiWatch.Stop();
                    g_pLogger->LOG_INFO("[procMain] Yakei Start.\n");
                }
            }
        }
        else
        {
            yakeiWatch.Stop();

            if (isYakei == true)
            {
                if (returnWatch.IsRunninng() == false)
                {
                    returnWatch.Start();
                }

                if (1.0f <= returnWatch.GetSplit())
                {
                    isYakei = false;
                    returnWatch.Stop();
                    g_pLogger->LOG_INFO("[procMain] Yakei Finish.\n");
                }
            }
        }

        if (isYakei == true)
        {
            /* ��x�J�n�ɂ��ԊO�����j�^�X���b�h���N�� */
            if (g_pRedwavePatrol == NULL)
            {
                g_pRedwavePatrol = new RedwavePatrol();
                g_pRedwavePatrol->Run();
            }
        }
        else
        {
            /* ��x�I���ɂ��ԊO�����j�^�X���b�h���~�E�j�� */
            if (g_pRedwavePatrol != NULL)
            {
                g_pRedwavePatrol->Stop(1);
                delete g_pRedwavePatrol;
                g_pRedwavePatrol = NULL;
            }
        }

        if (isShow == 1)
        {
            if (g_pCameraCapture->IsCaptureStart() == true)
            {
                cv::imshow("camera", pShareMemory->Capture.Data[pShareMemory->Capture.Index]);
                key = cv::waitKey(1);
                if (key == 'q')
                {
                    break;
                }
            }
        }
    }

    if (isShow == 1)
    {
        cv::destroyAllWindows();
    }
}

void signalHandler(int signum)
{
    printf("ERR! [AroundCamera] Signal Catched. signum[%d]\n", signum);
    if (g_pLogger != NULL)
    {
        g_pLogger->LOG_ERROR("[AroundCamera] Signal Catched!!\n");
    }

    finalize();

    exit(0);
}
