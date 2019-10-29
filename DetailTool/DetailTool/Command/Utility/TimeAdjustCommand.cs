using System;

namespace DetailTool.Command.Utility
{
    public class TimeAdjustCommand : CommandBase
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public TimeAdjustCommand()
        {
        }

        /// <summary>
        /// イベント生成
        /// </summary>
        /// <param name="expectedSize">期待する応答データサイズ</param>
        /// <returns>送信イベント</returns>
        protected override EventInfo createEvent(out int expectedSize)
        {
            EventInfo retVal = new EventInfo();
            expectedSize = sizeof(int);
            DateTime dt = DateTime.Now;
            int date = (dt.Year * 10000) + (dt.Month * 100) + dt.Day;
            int time = (dt.Hour * 10000) + (dt.Minute * 100) + dt.Second;

            retVal.Code = 3001;
            retVal.LParam[0] = date;
            retVal.LParam[1] = time;

            return retVal;
        }

        /// <summary>
        /// 解析処理
        /// </summary>
        /// <param name="data">解析対象データ</param>
        protected override void analyzeCore(byte[] data)
        {
            int retCode = BitConverter.ToInt32(data, 0);
        }
    }
}
