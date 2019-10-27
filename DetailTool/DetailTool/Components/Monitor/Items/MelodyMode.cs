using System;

namespace DetailTool.Components.Monitor.Items
{
    public class MelodyMode : StatusItemBase
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public MelodyMode()
        {
            this.Mode = MelodyModeEnum.E_MELODY_SILENT;
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
        /// Gets or sets ブザー吹鳴モード
        /// </summary>
        public MelodyModeEnum Mode { get; set; }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns>サイズ</returns>
        public override int GetSize()
        {
            return sizeof(int);
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

            int value = BitConverter.ToInt32(data, index);
            this.Mode = (MelodyModeEnum)value;
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
            string retVal = string.Empty;

            switch (this.Mode)
            {
                case MelodyModeEnum.E_MELODY_SILENT:
                    retVal = "無し";
                    break;

                case MelodyModeEnum.E_MELODY_SOUND_1:
                    retVal = "かえるの歌";
                    break;

                case MelodyModeEnum.E_MELODY_SOUND_2:
                    retVal = "社歌";
                    break;

                case MelodyModeEnum.E_MELODY_WARNING:
                    retVal = "警告";
                    break;

                case MelodyModeEnum.E_MELODY_ERROR:
                    retVal = "エラー";
                    break;

                default:
                    retVal = string.Format("不明 ({0})", this.Mode);
                    break;
            }

            return retVal;
        }
    }
}
