using System;

namespace DetailTool.Components.Monitor
{
    /// <summary>
    /// モータマイコン モニタデータクラス
    /// </summary>
    public class MotorMonitor
    {
        public MotorMonitor()
        {
        }

        public int CommunicationCount { get; private set; }
        public MotorCommandEnum Command { get; private set; }
        public CutterDriveEnum Cutter { get; private set; }
        public int PointX { get; private set; }
        public int PointY { get; private set; }
        public DetectTypeEnum ErrorStatus { get; private set; }
        public ControlModeEnum RemoteMode { get; private set; }
        public DetectTypeEnum CommunicationError { get; private set; }

        /// <summary>
        /// 動作指示
        /// </summary>
        public enum MotorCommandEnum
        {
            E_COMMAND_STOP,     // 停止
            E_COMMAND_FRONT,    // 前進
            E_COMMAND_R_TURN,   // 右ターン
            E_COMMAND_AVOID,    // 回避
            E_COMMAND_REMOTE,   // 遠隔動作
            E_COMMAND_MONITOR,  // モニタ
            E_COMMAND_L_TURN,   // 左ターン
            E_COMMAND_MAX,      // コマンド数
        }

        /// <summary>
        /// 草刈り刃動作指示
        /// </summary>
        public enum CutterDriveEnum
        {
            E_CUTTER_STOP,      // 停止
            E_CUTTER_DRIVE,     // 駆動
            E_CUTTER_TYPE_MAX,  // 制御数
        }

        /// <summary>
        /// 遠隔操作モード
        /// </summary>
        public enum ControlModeEnum
        {
            E_MODE_MANUAL,      // 手動
            E_MODE_AUTO,        // 自動
        }

        /// <summary>
        /// 検知状態定数
        /// </summary>
        public enum DetectTypeEnum
        {
            NOT_DETECT,         // 未検知
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

            // 通信回数
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.CommunicationCount = intValue;
            dataIndex += sizeof(int);

            // モータコマンド
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.Command = (MotorCommandEnum)intValue;
            dataIndex += sizeof(int);

            // 草刈り刃動作指示
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.Cutter = (CutterDriveEnum)intValue;
            dataIndex += sizeof(int);

            // 現在位置 X 座標
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.PointX = intValue;
            dataIndex += sizeof(int);

            // 現在位置 Y 座標
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.PointY = intValue;
            dataIndex += sizeof(int);

            // 異常検知状態
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.ErrorStatus = (DetectTypeEnum)intValue;
            dataIndex += sizeof(int);

            // 遠隔操作モード
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.RemoteMode = (ControlModeEnum)intValue;
            dataIndex += sizeof(int);

            // 通信エラー状態
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.CommunicationError = (DetectTypeEnum)intValue;
            dataIndex += sizeof(int);

            retVal = dataIndex;

            return retVal;
        }

    }
}
