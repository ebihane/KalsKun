#pragma once

#include "Parts/Setting/SettingManager.h"
#include "Parts/MappingData/AreaMap.h"
#include "Parts/MappingData/MoveMap.h"
#include "Parts/PositionData/PositionData.h"
#include "Task/Main/DriveDecider/DriveDecider.h"
#include "Task/Main/Sequencer/SequencerBase.h"

class YakeiKun : public SequencerBase
{
public:

    YakeiKun();
    virtual ~YakeiKun();

protected:


private:

    /* 状態情報構造体 */
    typedef struct
    {
        MoveTypeEnum        MoveType;       /* from 前方カメラ : 動作指示 */
        DetectTypeEnum      Human;          /* from 動物カメラ : 人検知状態 */
        DetectTypeEnum      Animal;         /* from 動物カメラ : 動物検知状態 */
        DetectTypeEnum      Redwave;        /* from 周辺カメラ : 赤外線検知状態 */
        MotorCommandEnum    CurrentMove;    /* from モータ : 現在の動作状態 */
        ControlModeEnum     ControlMode;    /* from モータ : コントロールモード */
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
    DriveDecider        m_DriveDecider;     /* 駆動動作決定クラスインスタンス */

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

};
