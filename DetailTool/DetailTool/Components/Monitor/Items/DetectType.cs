using System;

namespace DetailTool.Components.Monitor.Items
{
    public class DetectType : StatusItemBase
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public DetectType()
        {
        }

        /// <summary>
        /// Gets or sets 検知状態
        /// </summary>
        public DetectTypeEnum Detection { get; set; }

        /// <summary>
        /// 検知状態定数
        /// </summary>
        public enum DetectTypeEnum
        {
            NOT_DETECT = 0,     // 未検知
            DETECTED,           // 検知
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
            this.Detection = (DetectTypeEnum)value;
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

            switch (this.Detection)
            {
                case DetectTypeEnum.NOT_DETECT:
                    retVal = "未検知";
                    break;

                case DetectTypeEnum.DETECTED:
                    retVal = "検知";
                    break;

                default:
                    retVal = string.Format("不明 ({0})", this.Detection);
                    break;
            }

            return retVal;
        }
    }
}