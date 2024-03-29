﻿using System;
using System.Collections.Generic;
using DetailTool.Components.Monitor.Items;

namespace DetailTool.Components.Monitor.Controller
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

            this.MelodyMode = new MelodyMode();
            this.LightMode = new LightMode();
            this.LastStartDate = new DateTimeData();
            this.CurrentSequence = new SequenceType();
            this.MapMovedValue = new IntStatus();
            this.SystemError = new ErrorState();
        }

        public List<BeaconData> Beacon { get; private set; }
        public MelodyMode MelodyMode { get; private set; }
        public LightMode LightMode { get; private set; }
        public DateTimeData LastStartDate { get; private set; }
        public SequenceType CurrentSequence { get; private set; }
        public IntStatus MapMovedValue { get; private set; }
        public ErrorState SystemError { get; private set; }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns></returns>
        public int GetSize()
        {
            int retVal = 0;

            foreach (BeaconData data in this.Beacon)
            {
                retVal += data.GetSize();
            }

            retVal += this.MelodyMode.GetSize();
            retVal += this.LightMode.GetSize();
            retVal += this.LastStartDate.GetSize();
            retVal += this.CurrentSequence.GetSize();
            retVal += this.MapMovedValue.GetSize();
            retVal += this.SystemError.GetSize();

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

            // ビーコンデータ解析
            for (int index = 0; index < PrivateConstants.BeaconCount; index++)
            {
                int analyzedIndex = this.Beacon[index].Analyze(data, dataIndex);
                dataIndex += this.Beacon[index].GetSize();
            }

            // 音楽モード
            dataIndex = this.MelodyMode.Analyze(data, dataIndex);

            // 指向性ライト
            dataIndex = this.LightMode.Analyze(data, dataIndex);

            // 採集開始日時
            dataIndex = this.LastStartDate.Analyze(data, dataIndex);

            // シーケンス
            dataIndex = this.CurrentSequence.Analyze(data, dataIndex);

            // 動作マップ 移動完了データ
            dataIndex = this.MapMovedValue.Analyze(data, dataIndex);

            // システムエラー
            dataIndex = this.SystemError.Analyze(data, dataIndex);

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
