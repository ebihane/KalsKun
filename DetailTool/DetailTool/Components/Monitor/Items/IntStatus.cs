using System;

namespace DetailTool.Components.Monitor.Items
{
    public class IntStatus : StatusItemBase
    {
        /// <summary>
        /// デフォルトコンストラクタ
        /// </summary>
        public IntStatus()
        {
        }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns>サイズ</returns>
        public override int GetSize()
        {
            return sizeof(int);
        }

        /// <summary>
        /// Gets or sets 値
        /// </summary>
        public int Value { get; set; }

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

            this.Value = BitConverter.ToInt32(data, index);
            index += sizeof(int);

            retVal = index;

            return retVal;
        }

        /// <summary>
        /// 文字列取得
        /// </summary>
        /// <returns>文字列</returns>
        public override string ToString()
        {
            return this.Value.ToString();
        }
    }
}
