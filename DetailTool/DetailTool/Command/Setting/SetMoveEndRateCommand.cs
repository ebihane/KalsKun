using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DetailTool.Command.Setting
{
    public class SetMoveEndRateCommand : CommandBase
    {
        /// <summary>
        /// 動作完了判定閾値
        /// </summary>
        private float m_MoveEndRate = 0.0F;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="moveEndRate">動作完了判定閾値</param>
        public SetMoveEndRateCommand(float moveEndRate)
        {
            m_MoveEndRate = moveEndRate;
        }

        /// <summary>
        /// イベント生成
        /// </summary>
        /// <param name="expectedSize">期待する受信サイズ</param>
        /// <returns>送信イベント</returns>
        protected override EventInfo createEvent(out int expectedSize)
        {
            EventInfo retVal = new EventInfo();
            expectedSize = sizeof(int);

            retVal.Code = 1001;
            retVal.FParam[0] = m_MoveEndRate;

            return retVal;
        }

        /// <summary>
        /// 解析処理
        /// </summary>
        /// <param name="data">解析対象データ</param>
        protected override void analyzeCore(byte[] data)
        {
        }
    }
}
