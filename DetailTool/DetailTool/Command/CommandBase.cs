using System;

namespace DetailTool.Command
{
    public abstract class CommandBase
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CommandBase()
        {
        }

        /// <summary>
        /// 解析完了イベント
        /// </summary>
        public event EventHandler<EventArgs> OnAnalyzed;

        /// <summary>
        /// コマンド生成
        /// </summary>
        /// <param name="expectedSize">期待する応答データサイズ</param>
        /// <returns>送信コマンド</returns>
        public byte[] CreateCommand(out int expectedSize)
        {
            byte[] retVal = null;

            EventInfo sendEvent = createEvent(out expectedSize);
            retVal = sendEvent.GetByteData();
            
            return retVal;
        }

        /// <summary>
        /// 解析
        /// </summary>
        /// <param name="data"></param>
        public void Analyze(byte[] data)
        {
            analyzeCore(data);

            if (this.OnAnalyzed != null)
            {
                EventArgs e = new EventArgs();
                this.OnAnalyzed(this, e);
            }
        }

        /// <summary>
        /// イベント生成
        /// </summary>
        /// <param name="expectedSize">期待する応答データサイズ</param>
        /// <returns>送信イベント</returns>
        protected abstract EventInfo createEvent(out int expectedSize);

        /// <summary>
        /// 解析処理
        /// </summary>
        /// <param name="data">受信データ</param>
        protected abstract void analyzeCore(byte[] data);

    }
}
