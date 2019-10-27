using System;

namespace DetailTool.Components.Monitor.Items
{
    public class LightMode : StatusItemBase
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public LightMode()
        {
        }

        /// <summary>
        /// Gets or sets 指向性ライトモード
        /// </summary>
        public LightModeEnum Mode { get; set; }

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
            this.Mode = (LightModeEnum)value;
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
                case LightModeEnum.E_LIGHT_OFF:
                    retVal = "OFF";
                    break;

                case LightModeEnum.E_LIGHT_ON:
                    retVal = "ON";
                    break;

                case LightModeEnum.E_LIGHT_BLINK:
                    retVal = "点滅";
                    break;

                default:
                    retVal = string.Format("不明 ({0})", this.Mode);
                    break;
            }

            return retVal;
        }
    }
}
