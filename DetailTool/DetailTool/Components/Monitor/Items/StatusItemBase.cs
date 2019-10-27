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
    }
}
