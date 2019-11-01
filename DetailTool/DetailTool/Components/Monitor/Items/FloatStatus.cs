using System;

namespace DetailTool.Components.Monitor.Items
{
    public class FloatStatus : StatusItemBase
    {
        /// <summary>
        /// 小数点以下の桁数
        /// </summary>
        private int m_Digit = 1;

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name="digit">小数点以下の桁数</param>
        public FloatStatus(int digit)
        {
            m_Digit = digit;
        }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns></returns>
        public override int GetSize()
        {
            return sizeof(float);
        }

        /// <summary>
        /// Gets or sets 値
        /// </summary>
        public float Value { get; set; }

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

            this.Value = BitConverter.ToSingle(data, index);
            index += sizeof(float);

            retVal = index;

            return retVal;
        }

        /// <summary>
        /// 文字列取得
        /// </summary>
        /// <returns>文字列</returns>
        public override string ToString()
        {
            return this.Value.ToString(string.Format("F{0}", m_Digit));
        }
    }
}
