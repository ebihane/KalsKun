#pragma once

#include "MappingData.h"

class MoveMap : public MappingData
{
public:

    static const unsigned long  MOVE_FAILED_NO = 0;         /* ���s�֎~�G���A */
    static const unsigned long  NOT_MOVE_INIT_VALUE = 1;    /* �����s �����l */
    static const unsigned long  MOVED_INIT_VALUE = 2;       /* ���s�� �����l */

    /* �C���X�^���X�擾 */
    static MoveMap* const GetInstance();

    /* �����f�[�^���� */
    void SetInitialData();

    /* �w�肵�����W���ړ��\�����f���� */
    bool IsMovable(RectStr* const position);
    bool IsMovable(const unsigned long x, const unsigned long y);

    /* �w�肵�����W�����ړ������f���� */
    bool IsNotMove(RectStr* const position);
    bool IsNotMove(const unsigned long x, const unsigned long y);

    /* ���s�ς݃f�[�^���X�V���� */
    void UpdateMovedValue();

protected:

private:

    static MoveMap* m_This;

    unsigned long   m_MovedValue;                       /* ���s�ώ��Ƀ}�b�v�ɋL�^����l */
    unsigned long   m_NotMoveValue;                     /* �����s���Ƀ}�b�v�ɋL�^����l */

    MoveMap();
    virtual ~MoveMap();

};
