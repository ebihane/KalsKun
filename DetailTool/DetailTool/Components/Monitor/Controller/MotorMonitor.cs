using DetailTool.Components.Monitor.Items;
using System;

namespace DetailTool.Components.Monitor.Controller
{
    /// <summary>
    /// モータマイコン モニタデータクラス
    /// </summary>
    public class MotorMonitor
    {
        /// <summary>
        /// デフォルトコンストラクタ
        /// </summary>
        public MotorMonitor()
        {
            this.CommunicationCount = new IntStatus();
            this.Command = new MotorMoveState();
            this.Cutter = new CutterMoveState();
            this.GyroX = new IntStatus();
            this.GyroY = new IntStatus();
            this.PointX = new IntStatus();
            this.PointY = new IntStatus();
            this.ErrorStatus = new DetectType();
            this.RemoteMode = new ControlMode();
            this.CommunicationError = new DetectType();
        }

        public IntStatus CommunicationCount { get; private set; }
        public MotorMoveState Command { get; private set; }
        public CutterMoveState Cutter { get; private set; }
        public IntStatus GyroX { get; private set; }
        public IntStatus GyroY { get; private set; }
        public IntStatus PointX { get; private set; }
        public IntStatus PointY { get; private set; }
        public DetectType ErrorStatus { get; private set; }
        public ControlMode RemoteMode { get; private set; }
        public DetectType CommunicationError { get; private set; }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns>サイズ</returns>
        public int GetSize()
        {
            int retVal = 0;

            retVal += this.CommunicationCount.GetSize();
            retVal += this.Command.GetSize();
            retVal += this.Cutter.GetSize();
            retVal += this.GyroX.GetSize();
            retVal += this.GyroY.GetSize();
            retVal += this.PointX.GetSize();
            retVal += this.PointY.GetSize();
            retVal += this.ErrorStatus.GetSize();
            retVal += this.RemoteMode.GetSize();
            retVal += this.CommunicationError.GetSize();

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

            // 通信回数
            dataIndex = this.CommunicationCount.Analyze(data, dataIndex);

            // モータコマンド
            dataIndex = this.Command.Analyze(data, dataIndex);

            // 草刈り刃動作指示
            dataIndex = this.Cutter.Analyze(data, dataIndex);

            // ジャイロデータ
            dataIndex = this.GyroX.Analyze(data, dataIndex);
            dataIndex = this.GyroY.Analyze(data, dataIndex);

            // 現在位置
            dataIndex = this.PointX.Analyze(data, dataIndex);
            dataIndex = this.PointY.Analyze(data, dataIndex);

            // 異常検知状態
            dataIndex = this.ErrorStatus.Analyze(data, dataIndex);

            // 遠隔操作モード
            dataIndex = this.RemoteMode.Analyze(data, dataIndex);

            // 通信エラー状態
            dataIndex = this.CommunicationError.Analyze(data, dataIndex);

            retVal = dataIndex;

            return retVal;
        }
    }
}
