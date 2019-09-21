#pragma once

#include "MappingData.h"

class MoveMap : public MappingData
{
public:

    static const unsigned long  MOVE_FAILED_NO = 0;         /* 走行禁止エリア */
    static const unsigned long  NOT_MOVE_INIT_VALUE = 1;    /* 未走行 初期値 */
    static const unsigned long  MOVED_INIT_VALUE = 2;       /* 走行済 初期値 */

    /* インスタンス取得 */
    static MoveMap* const GetInstance();

    /* 初期データ生成 */
    void SetInitialData();

    /* 指定した座標が移動可能か判断する */
    bool IsMovable(RectStr* const position);
    bool IsMovable(const unsigned long x, const unsigned long y);

    /* 指定した座標が未移動か判断する */
    bool IsNotMove(RectStr* const position);
    bool IsNotMove(const unsigned long x, const unsigned long y);

    /* 走行済みデータを更新する */
    void UpdateMovedValue();

protected:

private:

    static MoveMap* m_This;

    unsigned long   m_MovedValue;                       /* 走行済時にマップに記録する値 */
    unsigned long   m_NotMoveValue;                     /* 未走行時にマップに記録する値 */

    MoveMap();
    virtual ~MoveMap();

};
