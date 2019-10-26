using System;

namespace DetailTool.Components.Monitor
{
    public class AroundCameraMonitor
    {
        public AroundCameraMonitor()
        {
        }

        public int ReceiveCount { get; private set; }
        public int SystemError { get; private set; }
        public DetectTypeEnum Detect { get; private set; }

        /// <summary>
        /// 検知状態定数
        /// </summary>
        public enum DetectTypeEnum
        {
            NOT_DETECT = 0,     // 未検知
            DETECTED,           // 検知
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
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.Detect = (DetectTypeEnum)intValue;
            dataIndex += sizeof(int);

            retVal = dataIndex;

            return retVal;
        }
    }
}