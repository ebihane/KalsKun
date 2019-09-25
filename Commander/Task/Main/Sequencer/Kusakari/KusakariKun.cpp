#include <stdio.h>
#include "Logger/Logger.h"
#include "Parts/CommanderCommon.h"
#include "Parts/Setting/SettingManager.h"
#include "Parts/MappingData/AreaMap.h"
#include "Parts/MappingData/MoveMap.h"
#include "KusakariKun.h"

KusakariKun::KusakariKun()
 : SequencerBase(SequencerBase::SequenceTypeEnum::E_SEQ_KUSAKARI)
 , m_TapeDetect(0)
 , m_PrevAvoidance(0)
 , m_BuzzerThread(NULL)
 , m_LightThread(NULL)
{

}

KusakariKun::~KusakariKun()
{
    /* nop. */
}

ResultEnum KusakariKun::initializeCore()
{
    ResultEnum retVal = ResultEnum::AbnormalEnd;
    EventInfo ev = { 0 };

    /* ブザー吹鳴スレッド生成 */
    m_BuzzerThread = new BuzzerController();
    if (m_BuzzerThread == NULL)
    {
        m_Logger.LOG_ERROR("[initializeCore] BuzzerThread allocation failed.\n");
        goto FINISH;
    }

    /* 指向性ライト制御スレッド生成 */
    m_LightThread = new LightController();
    if (m_LightThread == NULL)
    {
        m_Logger.LOG_ERROR("[initializeCore] LightThread allocation failed.\n");
        goto FINISH;
    }

    /* 前進・草刈り刃回転指示 */
    ev.Code = 2;
    ev.lParam[0] = MotorCommandEnum::E_COMMAND_FRONT;
    ev.lParam[1] = CutterDriveEnum::E_CUTTER_DRIVE;

    if (m_SendQueue.Send((char*)"MotorComm", &ev, sizeof(EventInfo)) != ResultEnum::NormalEnd)
    {
        char log[40] = { 0 };
        snprintf(&log[0], sizeof(log), "[initializeCore] Queue Send failed. errno[%d]\n", m_SendQueue.GetLastError());
        m_Logger.LOG_ERROR(log);
        goto FINISH;
    }

    m_PrevAvoidance = 0;
    m_PrevRect.X = -1;
    m_PrevRect.Y = -1;

    /* スレッド起動 */
    m_BuzzerThread->Run();
    m_LightThread->Run();

    retVal = ResultEnum::NormalEnd;

FINISH :
    return retVal;
}

void KusakariKun::destroyCore()
{
    pShareMemory->Commander.LightMode = LightModeEnum::E_LIGHT_OFF;
    pShareMemory->Commander.MelodyMode = MelodyModeEnum::E_MELODY_SILENT;

    if (m_LightThread != NULL)
    {
        m_LightThread->Stop(10);
        delete m_LightThread;
        m_LightThread = NULL;
    }

    if (m_BuzzerThread != NULL)
    {
        m_BuzzerThread->Stop(10);
        delete m_BuzzerThread;
        m_BuzzerThread = NULL;
    }
}

SequencerBase::SequenceTypeEnum KusakariKun::processCore()
{
    SequenceTypeEnum retVal = SequenceTypeEnum::E_SEQ_ERROR;
    bool sendExist = false;
    long avoidance = pShareMemory->FrontCamera.Avoidance;
    EventInfo ev = { 0 };
    MoveMap* moveMap = MoveMap::GetInstance();

    /* 草刈り中であることを示す出力 */
    digitalWrite(IO_KUSATORI_MODE, HIGH);
    digitalWrite(IO_YAKEI_MODE, LOW);

    /* 音楽を鳴らす指示 */
    pShareMemory->Commander.MelodyMode = MelodyModeEnum::E_MELODY_SOUND_1;

#if 0
    /* 動作マップ更新 */
    SizeStr realPoint;
    realPoint.Width = (double)pShareMemory->Motor.PointX;
    realPoint.Length = (double)pShareMemory->Motor.PointY;
    RectStr point = convertRealPointToMapPoint(&realPoint);
    moveMap->ChangeMoved(&point);
#endif

    /* 全網羅完了の場合は IDLE */
    if (moveMap->IsComplete() == true)
    {
        m_Logger.LOG_INFO("[processCore] Kusakari Finish!!!\n");
        retVal = SequenceTypeEnum::E_SEQ_IDLE;
        goto FINISH;
    }

    /* 前方カメラからの回避指示 */
    if (avoidance != m_PrevAvoidance)
    {
        /* 何もなし */
        if (avoidance == 0)
        {
            sendExist = true;
            ev.Code = 0;
            ev.lParam[0] = (long)MotorCommandEnum::E_COMMAND_FRONT;
        }
        /* 障害物を検知 */
        else if (avoidance == 1)
        {
            sendExist = true;
            ev.Code = 0;
            ev.lParam[0] = (long)MotorCommandEnum::E_COMMAND_AVOID;
            m_Logger.LOG_INFO("[processCore] Avoid from FrontCamera.\n");
        }
        /* 赤テープを検知 */
        else if (avoidance == 2)
        {
            if (m_TapeDetect == 0)
            {
                /* 1 回目は U ターン */
                sendExist = true;
                ev.Code = 0;
                ev.lParam[0] = (long)MotorCommandEnum::E_COMMAND_U_TURN;
                m_Logger.LOG_INFO("[processCore] 1st RedTape detect from FrontCamera.\n");
                m_TapeDetect = 1;
            }
            else if (pShareMemory->Motor.Command == MotorCommandEnum::E_COMMAND_AVOID)
            {
                /* 回避中の場合は何もしない */
                sendExist = false;
            }
            else
            {
                /* 2 回目は IDLE */
                m_Logger.LOG_INFO("[processCore] Red Tape 2nd Detected!!!\n");
                retVal = SequenceTypeEnum::E_SEQ_IDLE;
                goto FINISH;
            }
        }
        else
        {
            /* nop. */
            sendExist = false;
        }

        if (sendExist == true)
        {
            m_SendQueue.Send((char*)"MotorComm", &ev, sizeof(ev));
        }
    }

#if 0
    if ((m_PrevRect.X != point.X)
    ||  (m_PrevRect.Y != point.Y))
    {
        char log[64] = { 0 };
        snprintf(&log[0], sizeof(log), "[processCore] Move. Array[%ld, %ld] Real[%f, %f]\n", point.X, point.Y);
        m_Logger.LOG_INFO(log);
    }

    m_PrevRect.X = point.X;
    m_PrevRect.Y = point.Y;
#endif
    m_PrevAvoidance = avoidance;

    retVal = MY_SEQUENCE_TYPE;

FINISH :
    return retVal;
}

RectStr KusakariKun::convertRealPointToMapPoint(SizeStr* const pRealPoint)
{
    RectStr retVal = { 0 };
    SettingManager* setting = SettingManager::GetInstance();

    /* ロボットの大きさを取得 */
    SizeStr robotSize = { 0 };
    setting->GetRobotSize(&robotSize);

    /* 1 マス分の大きさは、ロボットサイズの半分 */
    robotSize.Length /= 2;
    robotSize.Width /= 2;

    /* 座標はロボットサイズで割った値 */
    retVal.X = (long)(pRealPoint->Width / robotSize.Width);
    retVal.Y = (long)(pRealPoint->Length / robotSize.Length);

    /* ただし周辺にわざと進入禁止エリアを設けているため、+1 */
    retVal.X += 1;
    retVal.Y += 1;

    return retVal;
}
