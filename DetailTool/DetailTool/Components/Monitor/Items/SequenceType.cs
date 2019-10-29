using System;

namespace DetailTool.Components.Monitor.Items
{
    public class SequenceType : StatusItemBase
    {
        /// <summary>
        /// デフォルトコンストラクタ
        /// </summary>
        public SequenceType()
        {
        }

        /// <summary>
        /// シーケンス種別
        /// </summary>
        public SequenceTypeEnum Sequence { get; set; }

        /// <summary>
        /// 動作中シーケンス
        /// </summary>
        public enum SequenceTypeEnum
        {
            E_SEQ_IDLE = 0,
            E_SEQ_KUSAKARI,
            E_SEQ_YAKEI,
            E_SEQ_ERROR,
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
            this.Sequence = (SequenceTypeEnum)value;
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

            switch (this.Sequence)
            {
                case SequenceTypeEnum.E_SEQ_IDLE:
                    retVal = "待機中";
                    break;

                case SequenceTypeEnum.E_SEQ_KUSAKARI:
                    retVal = "草刈り";
                    break;

                case SequenceTypeEnum.E_SEQ_YAKEI:
                    retVal = "夜警";
                    break;

                default:
                    retVal = string.Format("不明 ({0})", this.Sequence);
                    break;
            }

            return retVal;
        }

        /// <summary>
        /// 背景色取得
        /// </summary>
        /// <returns>背景色</returns>
        public override System.Drawing.Color GetBackColor()
        {
            System.Drawing.Color retVal = base.GetBackColor();

            if (this.Sequence == SequenceTypeEnum.E_SEQ_KUSAKARI)
            {
                retVal = System.Drawing.Color.LightGreen;
            }
            else if (this.Sequence == SequenceTypeEnum.E_SEQ_YAKEI)
            {
                retVal = System.Drawing.Color.Cyan;
            }
            else
            {
                // nop.
            }

            return retVal;
        }
    }
}
