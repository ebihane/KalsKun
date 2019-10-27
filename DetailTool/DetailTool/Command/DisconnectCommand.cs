namespace DetailTool.Command
{
    public class DisconnectCommand : CommandBase
    {
        /// <summary>
        /// デフォルトコンストラクタ
        /// </summary>
        public DisconnectCommand()
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

            retVal.Code = 9999;
            expectedSize = sizeof(int);
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
