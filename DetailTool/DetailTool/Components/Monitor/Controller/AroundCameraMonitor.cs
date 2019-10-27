using System;
using DetailTool.Components.Monitor.Items;

namespace DetailTool.Components.Monitor.Controller
{
    public class AroundCameraMonitor
    {
        public AroundCameraMonitor()
        {
            this.Detect = new DetectType();
        }

        public int ReceiveCount { get; private set; }
        public int SystemError { get; private set; }
        public DetectType Detect { get; private set; }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns>サイズ</returns>
        public int GetSize()
        {
            int retVal = 0;

            retVal += sizeof(int);
            retVal += sizeof(int);
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
            int intValue = 0;
            int dataIndex = startIndex;

            // 受信回数
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.ReceiveCount = intValue;
            dataIndex += sizeof(int);

            // システムエラー状態
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.SystemError = intValue;
            dataIndex += sizeof(int);

            // 動体検知状態
            dataIndex = this.Detect.Analyze(data, dataIndex);

            retVal = dataIndex;

            return retVal;
        }
    }
}