using System;
using System.Collections.Generic;

namespace DetailTool.Components.Monitor
{
    /// <summary>
    /// 前方カメラマイコン モニタデータクラス
    /// </summary>
    public class FrontCameraMonitor
    {
        public FrontCameraMonitor()
        {
            this.Distance = new List<float>();

            for (int index = 0; index < PrivateConstants.SonicSensorCount; index++)
            {
                float value = 0.0f;
                this.Distance.Add(value);
            }
        }

        public int ReceiveCount { get; private set; }
        public int SystemError { get; private set; }
        public MoveTypeEnum MoveType { get; private set; }
        public DetectTypeEnum RedTape { get; private set; }
        public DetectTypeEnum BlueObject { get; private set; }
        public List<float> Distance { get; private set; }

        /// <summary>
        /// 動作指示定数
        /// </summary>
        public enum MoveTypeEnum
        {
            NOT_REQUEST = 0,    // 要求無し
            AVOIDANCE,          // 回避指示
            TURN,               // ターン指示
        }

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

            // 回避・転回 指示状態
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.MoveType = (MoveTypeEnum)intValue;
            dataIndex += sizeof(int);

            // 赤テープ検知状態
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.RedTape = (DetectTypeEnum)intValue;
            dataIndex += sizeof(int);

            // 障害物検知状態
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.BlueObject = (DetectTypeEnum)intValue;
            dataIndex += sizeof(int);

            // 超音波センサ 距離
            for (int index = 0; index < PrivateConstants.SonicSensorCount; index++)
            {
                float floatValue = BitConverter.ToSingle(data, dataIndex);
                this.Distance[index] = floatValue;
                dataIndex += sizeof(float);
            }

            retVal = dataIndex;

            return retVal;
        }

        private static class PrivateConstants
        {
            /// <summary>
            /// 超音波センサ 個数
            /// </summary>
            public const int SonicSensorCount = 2;
        }
    }
}