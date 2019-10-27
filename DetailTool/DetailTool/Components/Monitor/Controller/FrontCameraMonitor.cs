using System;
using System.Collections.Generic;
using DetailTool.Components.Monitor.Items;

namespace DetailTool.Components.Monitor.Controller
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

            this.MoveType = new MotorMoveType();
            this.RedTape = new DetectType();
            this.BlueObject = new DetectType();
        }

        public int ReceiveCount { get; private set; }
        public int SystemError { get; private set; }
        public MotorMoveType MoveType { get; private set; }
        public DetectType RedTape { get; private set; }
        public DetectType BlueObject { get; private set; }
        public List<float> Distance { get; private set; }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns>サイズ</returns>
        public int GetSize()
        {
            int retVal = 0;

            retVal += sizeof(int);
            retVal += sizeof(int);
            retVal += this.MoveType.GetSize();
            retVal += this.RedTape.GetSize();
            retVal += this.BlueObject.GetSize();

            foreach (float val in this.Distance)
            {
                retVal += sizeof(float);
            }

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

            // 回避・転回 指示状態
            dataIndex = this.MoveType.Analyze(data, dataIndex);

            // 赤テープ検知状態
            dataIndex = this.RedTape.Analyze(data, dataIndex);

            // 障害物検知状態
            dataIndex = this.BlueObject.Analyze(data, dataIndex);

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