using System;
using DetailTool.Components.Monitor.Items;

namespace DetailTool.Components.Monitor.Controller
{
    public class AroundCameraMonitor
    {
        /// <summary>
        /// デフォルトコンストラクタ
        /// </summary>
        public AroundCameraMonitor()
        {
            this.ReceiveCount = new IntStatus();
            this.SystemError = new ErrorState();
            this.Detect = new DetectType();
        }

        public IntStatus ReceiveCount { get; private set; }
        public ErrorState SystemError { get; private set; }
        public DetectType Detect { get; private set; }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns>サイズ</returns>
        public int GetSize()
        {
            int retVal = 0;

            retVal += this.ReceiveCount.GetSize();
            retVal += this.SystemError.GetSize();
            retVal += this.Detect.GetSize();

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

            // 動体検知状態
            dataIndex = this.Detect.Analyze(data, dataIndex);

            retVal = dataIndex;

            return retVal;
        }
    }
}