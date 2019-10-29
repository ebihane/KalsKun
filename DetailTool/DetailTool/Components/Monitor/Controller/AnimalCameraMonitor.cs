using DetailTool.Components.Monitor.Items;
using System;

namespace DetailTool.Components.Monitor.Controller
{
    public class AnimalCameraMonitor
    {
        /// <summary>
        /// デフォルトコンストラクタ
        /// </summary>
        public AnimalCameraMonitor()
        {
            this.ReceiveCount = new IntStatus();
            this.SystemError = new ErrorState();
            this.Human = new DetectType();
            this.Animal = new DetectType();
        }

        public IntStatus ReceiveCount { get; private set; }
        public ErrorState SystemError { get; private set; }
        public DetectType Human { get; private set; }
        public DetectType Animal { get; private set; }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns>サイズ</returns>
        public int GetSize()
        {
            int retVal = 0;

            retVal += this.ReceiveCount.GetSize();
            retVal += this.SystemError.GetSize();
            retVal += this.Human.GetSize();
            retVal += this.Animal.GetSize();

            return retVal;
        }

        /// <summary>
        /// 受信データ解析・格納
        /// </summary>
        /// <param name="data">解析対象バイト配列</param>
        /// <param name="startIndex">解析開始インデックス</param>
        /// <returns>解析済みインデックス</returns>
        public int Analyze(byte[] data, int startIndex)
        {
            int retVal = 0;
            int dataIndex = startIndex;

            // 受信回数
            dataIndex = this.ReceiveCount.Analyze(data, dataIndex);

            // システムエラー状態
            dataIndex = this.SystemError.Analyze(data, dataIndex);

            // 人間検知状態
            dataIndex = this.Human.Analyze(data, dataIndex);

            // 動物検知状態
            dataIndex = this.Animal.Analyze(data, dataIndex);

            retVal = dataIndex;

            return retVal;
        }
    }
}
