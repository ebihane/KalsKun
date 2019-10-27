using System;

namespace DetailTool.Components.Monitor.Items
{
    public class BeaconData : StatusItemBase
    {
        public BeaconData()
        {
        }

        public float RssiAverage { get; private set; }
        public float TxPowerAverage { get; private set; }
        public float Distance { get; private set; }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns>サイズ</returns>
        public override int GetSize()
        {
            int retVal = sizeof(float) * 3;
            return retVal;
        }

        /// <summary>
        /// 解析処理
        /// </summary>
        /// <param name="data">解析対象バイト配列</param>
        /// <param name="startIndex">解析開始インデックス</param>
        /// <returns>解析済みインデックス</returns>
        public override int Analyze(byte[] data, int startIndex)
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
}
