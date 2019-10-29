namespace DetailTool.Components.Monitor.Items
{
    public abstract class StatusItemBase
    {
        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns>サイズ</returns>
        public abstract int GetSize();

        /// <summary>
        /// 解析処理
        /// </summary>
        /// <param name="data">解析対象バイト配列</param>
        /// <param name="startIndex">解析開始インデックス</param>
        /// <returns>解析済みインデックス</returns>
        public abstract int Analyze(byte[] data, int startIndex);

        /// <summary>
        /// 背景色取得
        /// </summary>
        /// <returns>背景色</returns>
        public virtual System.Drawing.Color GetBackColor()
        {
            return System.Drawing.SystemColors.Control;
        }
    }
}
