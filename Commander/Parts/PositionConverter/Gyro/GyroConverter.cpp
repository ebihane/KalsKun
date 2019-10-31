#include <stdlib.h>
#include "Parts/Setting/SettingManager.h"
#include "GyroConverter.h"

GyroConverter::GyroConverter()
 : m_SettingManager(NULL)
{
    m_SettingManager = SettingManager::GetInstance();
}

GyroConverter::~GyroConverter()
{
    /* nop. */
}

RectStr GyroConverter::Convert(const double baseX, const double baseY)
{
    RectStr retVal = { 0 };

    /* �W���C���̃f�[�^�� cm �P�ʂ̂��߁Amm �ɕϊ� */
    /* X �͍ŏ��Ɍ����Ă�������� - ���̂��߁A�����𔽓]����K�v���� */
    double baseXmm = (baseX * -1) * 10.0;
    double baseYmm = baseY * 10.0;

    /* ���{�b�g�̑傫�����擾 */
    SizeStr robotSize = { 0 };
    m_SettingManager->GetRobotSize(&robotSize);

    /* 1 �}�X���̑傫���́A���{�b�g�T�C�Y�̔��� */
    robotSize.Vertical /= 2;
    robotSize.Horizontal /= 2;

    /* ���W�̓��{�b�g�T�C�Y�Ŋ������l */
    retVal.X = (long)((baseXmm / robotSize.Horizontal) + 0.5);
    retVal.Y = (long)((baseYmm / robotSize.Vertical) + 0.5);

    /* ���������ӂɂ킴�Ɛi���֎~�G���A��݂��Ă��邽�߁A+1 */
    retVal.X += 1;
    retVal.Y += 1;

    return retVal;
}
