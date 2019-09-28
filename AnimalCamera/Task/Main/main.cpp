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

#include <signal.h>

/* Common */
#include "Include/Common.h"
#include "Parts/AnimalCameraCommon.h"

/* Parts */
#include "Logger/Logger.h"
#include "Socket/TcpClient/TcpClient.h"
#include "Parts/ShareMemory/ShareMemory.h"

/* Task */
#include "Task/CameraCapture/CameraCapture.h"
#include "Task/StateSender/StateSender.h"
#include "Task/HeartBeat/HeartBeatManager.h"

/* Parts */
static Logger* g_pLogger = NULL;

/* Task */
static CameraCapture* g_pCameraCapture = NULL;
static StateSender* g_pStateSender = NULL;
static HeartBeatManager* g_pHeartBeatManager = NULL;


ResultEnum initialize(const char cameraNo);
void mainProcedure(const char isShow);
void finalize();
void signalHandler(int signum);

/* ./AnimalCamera.out (�J�����ԍ�) */
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

    mainProcedure(isShow);
    isMainLoopExit = true;

FINISH:
    finalize();

    if (isMainLoopExit == true)
    {
        printf("shutdown execute.");
        system("sudo shutdown -h now &");
    }

    return 0;
}

ResultEnum initialize(const char cameraNo)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    TcpClient* client = NULL;

    /* I/O ������ */
    wiringPiSetupSys();
    {
        for (long lCnt = 0; lCnt < GPIO_USE_PIN_COUNT; lCnt++)
        {
            /* ���o�͕����w�� */
            pinMode(GPIO_INFO_TABLE[lCnt].PinNo, GPIO_INFO_TABLE[lCnt].Mode);
        }
    }

    /* ���L������ �C���X�^���X���� */
    pShareMemory = new ShareMemoryStr();
    if (pShareMemory == NULL)
    {
        goto FINISH;
    }

    /* �f�[�^������ */
    pShareMemory->SystemError = false;
    pShareMemory->Human = DetectTypeEnum::NOT_DETECT;
    pShareMemory->Animal = DetectTypeEnum::NOT_DETECT;

    /* Log Process �N�� */
    StartLoggerProcess((char*)"AnimalCamera");

    /* Log Process �����҂� */
    delay(100);

    /* Log Accessor ���� */
    g_pLogger = new Logger(Logger::LOG_ERROR | Logger::LOG_INFO, Logger::LogTypeEnum::BOTH_OUT);
    if (g_pLogger == NULL)
    {
        goto FINISH;
    }

    /* �n�[�g�r�[�g����X���b�h ������ */
    g_pHeartBeatManager = new HeartBeatManager();
    if (g_pHeartBeatManager == NULL)
    {
        g_pLogger->LOG_ERROR("[initialize] HeartBeatManager allocation failed.\n");
        goto FINISH;
    }

    /* �J�����擾�X���b�h �N�� */
    g_pCameraCapture = new CameraCapture(cameraNo);
    if (g_pCameraCapture == NULL)
    {
        g_pLogger->LOG_ERROR("[slaveInitialize] g_pCameraCapture allocation failed.\n");
        goto FINISH;
    }

    /* ��ԑ��M�X���b�h �N�� */
    client = new TcpClient((char*)COMMANDER_IP_ADDRESS, FC2_TO_COMMANDER_PORT);
    g_pStateSender = new StateSender(client);
    if (g_pStateSender == NULL)
    {
        g_pLogger->LOG_ERROR("[masterInitialize] g_pStateSender allocation failed.\n");
        goto FINISH;
    }

    g_pHeartBeatManager->Run();
    g_pCameraCapture->Run();
    g_pStateSender->Run();

    retVal = ResultEnum::NormalEnd;

FINISH:
    return retVal;
}

void mainProcedure(const char isShow)
{
    int key = 0;
    char logStr[80] = { 0 };
    Stopwatch watch;

    snprintf(&logStr[0], sizeof(logStr), "[mainProcedure] Main loop start. isShow[%d]\n", isShow);
    g_pLogger->LOG_INFO(logStr);

    watch.Start();
    while (1)
    {
        if (isShow == 1)
        {
            if (g_pCameraCapture->IsCaptureStart() == true)
            {
                Mat slaveCapture = pShareMemory->Capture.Data[pShareMemory->Capture.Index];
                cv::imshow("camera", slaveCapture);
                key = cv::waitKey(1);
                if (key == 'q')
                {
                    break;
                }
            }
        }

        if (60.0f <= watch.GetSplit())
        {
            int shutdown1 = digitalRead(IO_SHUTDOWN_1);
            int shutdown2 = digitalRead(IO_SHUTDOWN_2);
            if ((shutdown1 == LOW) && (shutdown2 == LOW))
            {
                g_pLogger->LOG_INFO("[mainProcedure] Shutdown!!!\n");
                break;
            }
        }

        delay(10);
    }

    if (isShow == 1)
    {
        cv::destroyAllWindows();
    }
}

void finalize()
{
    if (g_pStateSender != NULL)
    {
        g_pStateSender->Stop(5);
        delete g_pStateSender;
        g_pStateSender = NULL;
    }

    if (g_pCameraCapture != NULL)
    {
        g_pCameraCapture->Stop(5);
        delete g_pCameraCapture;
        g_pCameraCapture = NULL;
    }

    if (g_pHeartBeatManager != NULL)
    {
        g_pHeartBeatManager->Stop(5);
        delete g_pHeartBeatManager;
        g_pHeartBeatManager = NULL;
    }

    if (g_pLogger != NULL)
    {
        delete g_pLogger;
        g_pLogger = NULL;
    }

    if (pShareMemory != NULL)
    {
        delete pShareMemory;
        pShareMemory = NULL;
    }

    StopLoggerProcess();
}

void signalHandler(int signum)
{
    printf("ERR! [AnimalCamera] Signal Catched. signum[%d]\n", signum);

    if (g_pLogger != NULL)
    {
        g_pLogger->LOG_ERROR("[AnimalCamera] Signal Catched!!\n");
    }

    finalize();

    exit(0);
}
