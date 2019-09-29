#pragma once

#include "Parts/Setting/SettingManager.h"
#include "Parts/MappingData/AreaMap.h"
#include "Parts/MappingData/MoveMap.h"
#include "Task/Main/Sequencer/SequencerBase.h"

class KusakariKun : public SequencerBase
{
public:

    KusakariKun();
    virtual ~KusakariKun();

protected:


private:

    /* 状態情報構造体 */
    typedef struct
    {
        MoveTypeEnum        MoveType;       /* from 前方カメラ : 動作指示 */
        DetectTypeEnum      Animal;         /* from 動物カメラ : 動物検知状態 */
        SizeStr             RealPosition;   /* from モータ : ロボット実現在位置 */
        RectStr             Position;       /* from モータ : ロボット位置座標 */
        MotorCommandEnum    CurrentMove;    /* from モータ : 現在の動作状態 */
        ControlModeEnum     ControlMode;    /* from モータ : コントロールモード */
        long                TurnCount;      /* from 司令塔 : ターン回数 */
    } StateInfoStr;
    
    /* 動作指示構造体 */
    typedef struct
    {
        MelodyModeEnum      Melody;         /* to ブザー : ブザー出力指示 */
        LightModeEnum       Light;          /* to ライト : ライト出力指示 */
        MotorCommandEnum    MotorCommand;   /* to モータ : 動作指示 */
        CutterDriveEnum     Cutter;         /* to モータ : 草刈り刃回転指示 */
    } DriveInfoStr;

    StateInfoStr        m_PreviewState;     /* 前回周期の状態情報 */
    DriveInfoStr        m_PreviewDrive;     /* 前回周期の動作指示 */

    SettingManager*     m_Setting;          /* 設定管理クラスインスタンス */
    AreaMap*            m_AreaMap;          /* エリアマップクラスインスタンス */
    MoveMap*            m_MoveMap;          /* 動作マップクラスインスタンス */

    ResultEnum initializeCore();
    void destroyCore();
    SequenceTypeEnum processCore();

    /* 状態収集 */
    void correctCurrentState(StateInfoStr* const state);

    /* 出力決定 */
    MelodyModeEnum decideMelodyMode(StateInfoStr* const state);
    LightModeEnum decideLightMode(StateInfoStr* const state);
    MotorCommandEnum decideMotorCommand(StateInfoStr* const state);
    CutterDriveEnum decideCutterDrive(StateInfoStr* const state);

    /* 指示 */
    void sendMotorMessage(DriveInfoStr* const drive);

    /* 次回動作の決定 */
    SequenceTypeEnum decideNextSequence(StateInfoStr* const state);

    /* ユーティリティ */
    RectStr convertRealPointToMapPoint(SizeStr* const pRealPoint);
};
