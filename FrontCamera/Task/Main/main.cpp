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

#include "Include/Common.h"
#include "Parts/FrontCameraCommon.h"
#include "Logger/Logger.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "MasterMain.h"
#include "SlaveMain.h"
#include "KinectMain.h"

ResultEnum initialize(const char controllerType);
void finalize();

/* ./FrontCamera.out (�g�p�f�o�C�X�ԍ�) */
/* Kinect ��p���Đ[�x�f�[�^���狗�����킩�邩�v���� */
/* Kinect �����O�Ŏg�p�ł��Ȃ��ꍇ�A2��̃J�����ɂ��X�e���I�}�b�`���O���s���K�v���� */
/* �g�p�f�o�C�X�ԍ��� 0 ���w�肷��� Kinect �p�̏��������s����O���J�����}�C�R���Ƃ��ċN������ */
/* �g�p�f�o�C�X�ԍ��� 1 ���w�肷��ƃX�e���I�}�b�`���O�����{���鑤�̑O���J�����}�C�R���Ƃ��ċN������ (Master) */
/* �g�p�f�o�C�X�ԍ��� 2 ���w�肷��ƃJ�����摜���擾���}�X�^�[�ɑ��M����O���J�����}�C�R���Ƃ��ċN������ (Slave) */
int main(int argc, char* argv[])
{
    bool isMainLoopExit = false;
        
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
			masterMain(cameraNo);
            break;

        case 1 :
            slaveMain(cameraNo);
            break;

        default :
            printf("[FrontCamera::main] invalid deviceNo. [%d]\n", deviceNo);
            break;
    }

    isMainLoopExit = true;

FINISH:
    finalize();

    if (isMainLoopExit == true)
    {
        printf("shutdown execute.");
        // system("sudo shutdown -h now &");
    }

    return 0;
}

ResultEnum initialize(const char controllerType)
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;

    /* I/O ������ */
    wiringPiSetupSys();
    {
        for (long lCnt = 0; lCnt < GPIO_USE_PIN_COUNT; lCnt++)
        {
            /* ���o�͕����w�� */
            pinMode(GPIO_INFO_TABLE[lCnt].PinNo, GPIO_INFO_TABLE[lCnt].Mode);
        }
    }

    pShareMemory = new ShareMemoryStr();
    if (pShareMemory == NULL)
    {
        goto FINISH;
    }

    if (controllerType == 0)
    {
        StartLoggerProcess((char*)"FrontCameraM");
    }
    else if (controllerType == 1)
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
