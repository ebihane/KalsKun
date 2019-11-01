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
            this.ReceiveCount = new IntStatus();
            this.SystemError = new ErrorState();
            this.MoveType = new MotorMoveType();
            this.RedTape = new DetectType();
            this.BlueObject = new DetectType();
            this.Distance = new List<FloatStatus>();
            for (int index = 0; index < PrivateConstants.SonicSensorCount; index++)
            {
                FloatStatus value = new FloatStatus(3);
                this.Distance.Add(value);
            }
        }

        public IntStatus ReceiveCount { get; private set; }
        public ErrorState SystemError { get; private set; }
        public MotorMoveType MoveType { get; private set; }
        public DetectType RedTape { get; private set; }
        public DetectType BlueObject { get; private set; }
        public List<FloatStatus> Distance { get; private set; }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns>サイズ</returns>
        public int GetSize()
        {
            int retVal = 0;

            retVal += this.ReceiveCount.GetSize();
            retVal += this.SystemError.GetSize();
            retVal += this.MoveType.GetSize();
            retVal += this.RedTape.GetSize();
            retVal += this.BlueObject.GetSize();

            foreach (FloatStatus val in this.Distance)
            {
                retVal += val.GetSize();
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
            int dataIndex = startIndex;

            // 受信回数
            dataIndex = this.ReceiveCount.Analyze(data, dataIndex);

            // エラー状態
            dataIndex = this.SystemError.Analyze(data, dataIndex);

            // 回避・転回 指示状態
            dataIndex = this.MoveType.Analyze(data, dataIndex);

            // 赤テープ検知状態
            dataIndex = this.RedTape.Analyze(data, dataIndex);

            // 障害物検知状態
            dataIndex = this.BlueObject.Analyze(data, dataIndex);

            // 超音波センサ 距離
            for (int index = 0; index < PrivateConstants.SonicSensorCount; index++)
            {
                dataIndex = this.Distance[index].Analyze(data, dataIndex);
            }

            retVal = dataIndex;

            return retVal;
        }

        /// <summary>
        /// クラス内定数
        /// </summary>
        private static class PrivateConstants
        {
            /// <summary>
            /// 超音波センサ 個数
            /// </summary>
            public const int SonicSensorCount = 2;
        }
    }
}