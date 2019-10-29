using System;
using System.Drawing;

namespace DetailTool.Components.Monitor.Items
{
    public class ControlMode : StatusItemBase
    {
        /// <summary>
        /// デフォルトコンストラクタ
        /// </summary>
        public ControlMode()
        {
        }

        /// <summary>
        /// Gets or sets 遠隔操作状態
        /// </summary>
        public ControlModeEnum State { get; set; }

        /// <summary>
        /// 遠隔操作モード
        /// </summary>
        public enum ControlModeEnum
        {
            E_MODE_MANUAL,      // 手動
            E_MODE_AUTO,        // 自動
        }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns></returns>
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
            this.State = (ControlModeEnum)value;
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

            switch (this.State)
            {
                case ControlModeEnum.E_MODE_MANUAL:
                    retVal = "手動";
                    break;

                case ControlModeEnum.E_MODE_AUTO:
                    retVal = "自動";
                    break;

                default:
                    retVal = string.Format("不明 ({0})", this.State);
                    break;
            }

            return retVal;
        }

        /// <summary>
        /// 背景色取得
        /// </summary>
        /// <returns>背景色</returns>
        public override Color GetBackColor()
        {
            Color retVal = base.GetBackColor();

            if (this.State == ControlModeEnum.E_MODE_MANUAL)
            {
                retVal = Color.Yellow;
            }

            return retVal;
        }
    }
}
