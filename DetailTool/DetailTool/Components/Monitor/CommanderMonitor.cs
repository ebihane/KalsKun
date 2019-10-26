using System;
using System.Collections.Generic;

namespace DetailTool.Components.Monitor
{
    /// <summary>
    /// 司令塔マイコン モニタデータクラス
    /// </summary>
    public class CommanderMonitor
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public CommanderMonitor()
        {
            this.Beacon = new List<BeaconData>();
            for (int index = 0; index < 3; index++)
            {
                BeaconData beacon = new BeaconData();
                this.Beacon.Add(beacon);
            }
        }

        public List<BeaconData> Beacon { get; private set; }
        public MelodyModeEnum MelodyMode { get; private set; }
        public LightModeEnum LightMode { get; private set; }
        public SequenceTypeEnum CurrentSequence { get; private set; }
        public bool SystemError { get; private set; }

        /// <summary>
        /// 方向定数
        /// </summary>
        public enum DirectionEnum
        {
            E_DIR_FRONT,        // 前進
            E_DIR_FRONT_R,      // 右斜め前
            E_DIR_RIGHT,        // 右
            E_DIR_BACK_R,       // 右斜め後ろ
            E_DIR_BACK,         // 後ろ
            E_DIR_BACK_L,       // 左斜め後ろ
            E_DIR_LEFT,         // 左
            E_DIR_FRONT_L,      // 左斜め前
            E_DIR_TYPE_MAX,     // 方向種別数
        }

        /// <summary>
        /// ブザー吹鳴モード
        /// </summary>
        public enum MelodyModeEnum
        {
            E_MELODY_SILENT,    // 停止
            E_MELODY_SOUND_1,   // 曲 1
            E_MELODY_SOUND_2,   // 曲 2
            E_MELODY_WARNING,   // 警告
            E_MELODY_ERROR,     // 異常発生
        }

        /// <summary>
        /// 指向性ライトモード
        /// </summary>
        public enum LightModeEnum
        {
            E_LIGHT_OFF,        // OFF
            E_LIGHT_ON,         // ON
            E_LIGHT_BLINK,      // 点滅
        }

        /// <summary>
        /// 動作中シーケンス
        /// </summary>
        public enum SequenceTypeEnum
        {
            E_SEQ_IDLE = 0,
            E_SEQ_KUSAKARI,
            E_SEQ_YAKEI,
            E_SEQ_ERROR,
        }
        
        /// <summary>
        /// 電波強度関連情報クラス
        /// </summary>
        public class BeaconData
        {
            public BeaconData()
            {
            }

            public float RssiAverage { get; private set; }
            public float TxPowerAverage { get; private set; }
            public float Distance { get; private set; }

            /// <summary>
            /// 解析処理
            /// </summary>
            /// <param name="data">解析対象バイト配列</param>
            /// <param name="startIndex">解析開始インデックス</param>
            /// <returns>解析済みインデックス</returns>
            public int Analyze(byte[] data, int startIndex)
            {
                int retVal = 0;
                int index = startIndex;

                this.RssiAverage = BitConverter.ToSingle(data, index);
                index += sizeof(float);

                this.TxPowerAverage = BitConverter.ToSingle(data, index);
                index += sizeof(float);

                this.Distance = BitConverter.ToSingle(data, index);
                index += sizeof(float);

                retVal = index;

                return retVal;
            }
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

            // ビーコンデータ解析
            for (int index = 0; index < PrivateConstants.BeaconCount; index++)
            {
                int analyzedIndex = this.Beacon[index].Analyze(data, dataIndex);
                dataIndex += analyzedIndex;
            }

            // 音楽モード
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.MelodyMode = (MelodyModeEnum)intValue;
            dataIndex += sizeof(int);

            // 指向性ライト
            intValue = BitConverter.ToInt32(data, dataIndex);
            this.LightMode = (LightModeEnum)intValue;
            dataIndex += sizeof(int);

            intValue = BitConverter.ToInt32(data, dataIndex);
            this.CurrentSequence = (SequenceTypeEnum)intValue;
            dataIndex += sizeof(int);

            // システムエラー
            this.SystemError = BitConverter.ToBoolean(data, dataIndex);
            dataIndex += sizeof(bool);

            retVal = dataIndex;

            return retVal;
        }

        /// <summary>
        /// クラス内定数
        /// </summary>
        private static class PrivateConstants
        {
            /// <summary>
            /// ビーコン個数
            /// </summary>
            public const int BeaconCount = 3;
        }
    }
}
