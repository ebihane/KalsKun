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

    /* ジャイロのデータは cm 単位のため、mm に変換 */
    /* X は最初に向いている方向が - 側のため、符号を反転する必要あり */
    double baseXmm = (baseX * -1) * 10.0;
    double baseYmm = baseY * 10.0;

    /* ロボットの大きさを取得 */
    SizeStr robotSize = { 0 };
    m_SettingManager->GetRobotSize(&robotSize);

    /* 1 マス分の大きさは、ロボットサイズの半分 */
    robotSize.Length /= 2;
    robotSize.Width /= 2;

    /* 座標はロボットサイズで割った値 */
    retVal.X = (long)(baseXmm / robotSize.Length);
    retVal.Y = (long)(baseYmm / robotSize.Width);

    /* ただし周辺にわざと進入禁止エリアを設けているため、+1 */
    retVal.X += 1;
    retVal.Y += 1;

    return retVal;
}
