#pragma once

#include "Include/Common.h"
#include "Parts/ShareMemory/ShareMemory.h"
#include "Parts/MappingData/AreaMap.h"
#include "Parts/MappingData/MoveMap.h"

/* �����ƕ��p���Ǘ����邽�߂̃N���X */
/* ��ʑw�ł́u�O�㍶�E�v�Ŏw�����o���������A���{�b�g�̈ʒu�͐�ΓI�ȃf�[�^ (= ���p) �Ŕ��f���������߁A���̃N���X�����܂��� */
/* �Ȃ��A�{�N���X�Ŏ擾�ł�����W�́A�u���͈͓̔����v�u���s�֎~���v�Ƃ��������f�͓x�O�����Ă���_�ɒ��ӂ��K�v */
/*  �����F�u�O�㍶�E�v����Ƃ����A���{�b�g�̌����̏��B���{�b�g�̎p���ɂ���ĎQ�Ƃ�����p���ς�� */
/*  ���p�F�u������k�v����Ƃ����A���{�b�g�������Ă�����p�̏��B*/
class PositionData
{
public :

    /* ���W�\���� */
    typedef struct
    {
        long X;    /* �� */
        long Y;    /* �c */
    } RectStr;

    PositionData(const CompassEnum compass, RectStr* const position);
    virtual ~PositionData();

    /* ���݂̍��W���擾���� */
    void GetPosition(RectStr* const position);

    /* ���݌����Ă�����p���擾���� */
    CompassEnum GetCompass();

    /* �����Ă�����p�ɐi�񂾎��̍��W���擾���� */
    void GetNextPosition(RectStr* const position);

    /* �w�肵�������ɐi�񂾎��̍��W���擾���� */
    void GetNextPosition(const DirectionEnum direction, RectStr* const position);

    /* �����Ă��������ς��� */
    void SetDirection(DirectionEnum const direction);

    /* �����Ă�������ɐi�� */
    void Move();

protected :

private :

    static const RectStr m_ReferenceMap[CompassEnum::E_COMPASS_MAX];    /* ���W�ړ������Z�o�}�b�v */

    RectStr     m_Position;     /* ���݈ʒu */
    CompassEnum m_Compass;      /* ���݌����Ă�����p */

    /* �w�肳�ꂽ���p�Ɍ��������������V���ȕ��p��Ԃ� */
    CompassEnum convertDirectionToCompass(const CompassEnum compass, const DirectionEnum direction);

};
