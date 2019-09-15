#include <string.h>
#include "PositionData.h"

/* �����Ă�����������ƂɁA�O�i�����ꍇ�̍��W���Z�o���邽�߂̃e�[�u�� */
/* �}�b�v�� [0][0] ���k�������Ƃ��Ă��� */
const PositionData::RectStr PositionData::m_ReferenceMap[CompassEnum::E_COMPASS_MAX]
= {
    /*   X       Y      */
    /*-------+----------*/
    {    0,     -1  },  /* �k */
    {    1,     -1  },  /* �k�� */
    {    1,      0  },  /* �� */
    {    1,      1  },  /* �쓌 */
    {    0,      1  },  /* �� */
    {   -1,      1  },  /* �쐼 */
    {   -1,      0  },  /* �� */
    {   -1,     -1  },  /* �k�� */
};

PositionData::PositionData(const CompassEnum compass, RectStr* const position)
 : m_Compass(compass)
{
    memcpy(&m_Position, position, sizeof(RectStr));
}

PositionData::~PositionData()
{
    /* nop. */
}

void PositionData::GetPosition(RectStr* const position)
{
    memcpy(position, &m_Position, sizeof(RectStr));
}

CompassEnum PositionData::GetCompass()
{
    return m_Compass;
}

/* �����Ă�����p�ɐi�񂾎��̍��W���擾���� */
void PositionData::GetNextPosition(RectStr* const position)
{
    position->X = m_Position.X + m_ReferenceMap[m_Compass].X;
    position->Y = m_Position.Y + m_ReferenceMap[m_Compass].Y;
}

/* �w�肵�������ɐi�񂾎��̍��W���擾���� */
void PositionData::GetNextPosition(const DirectionEnum direction, RectStr* const position)
{
    CompassEnum nextCompass = convertDirectionToCompass(m_Compass, direction);
    position->X = m_Position.X + m_ReferenceMap[nextCompass].X;
    position->Y = m_Position.X + m_ReferenceMap[nextCompass].Y;
}

/* �����Ă��������ς��� */
void PositionData::SetDirection(DirectionEnum const direction)
{
    m_Compass = convertDirectionToCompass(m_Compass, direction);
}

/* �����Ă�������ɐi�� */
void PositionData::Move()
{
    m_Position.X += m_ReferenceMap[m_Compass].X;
    m_Position.Y += m_ReferenceMap[m_Compass].Y;
}

/* �w�肳�ꂽ���p�Ɍ��������������V���ȕ��p��Ԃ� */
CompassEnum PositionData::convertDirectionToCompass(const CompassEnum compass, const DirectionEnum direction)
{
    CompassEnum retVal = CompassEnum::E_COMPASS_MAX;
    long tempCompass = (long)compass;

    tempCompass += (long)direction;
    tempCompass %= (long)CompassEnum::E_COMPASS_MAX;
    retVal = (CompassEnum)tempCompass;

    return retVal;
}
