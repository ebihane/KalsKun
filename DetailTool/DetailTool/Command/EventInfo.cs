using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DetailTool.Command
{
    public class EventInfo
    {
        /// <summary>
        /// コンストラクタ
        /// </summary>
        public EventInfo()
        {
            this.LParam = new List<int>();
            this.FParam = new List<float>();

            for (int index = 0; index < PublicConstants.LParamCount; index++)
            {
                this.LParam.Add(0);
            }

            for (int index = 0; index < PublicConstants.FParamCount; index++)
            {
                this.FParam.Add(0);
            }
        }

        public int Code { get; set; }
        public ResultEnum Result { get; set; }
        public List<int> LParam { get; set; }
        public List<float> FParam { get; set; }

        /// <summary>
        /// 公開定数
        /// </summary>
        public static class PublicConstants
        {
            public const int LParamCount = 5;
            public const int FParamCount = 10;
        }

        /// <summary>
        /// 結果定数
        /// </summary>
        public enum ResultEnum
        {
            /// <summary>
            /// 正常終了
            /// </summary>
            NormalEnd,
            
            /// <summary>
            /// 異常終了
            /// </summary>
            AbnormalEnd,

            /// <summary>
            /// 再接続
            /// </summary>
            Reconnect,
        }

        /// <summary>
        /// バイト配列を取得する
        /// </summary>
        /// <returns>イベントデータのバイト配列</returns>
        public byte[] GetByteData()
        {
            byte[] retVal = null;

            // サイズ計算
            int size = 0;
            size += sizeof(int);
            size += sizeof(ResultEnum);
            size += sizeof(int) * PublicConstants.LParamCount;
            size += sizeof(float) * PublicConstants.FParamCount;

            int index = 0;
            byte[] byteArray = new byte[size];

            byte[] codeBytes = BitConverter.GetBytes(this.Code);
            codeBytes.CopyTo(byteArray, index);
            index += codeBytes.Length;

            byte[] resultBytes = BitConverter.GetBytes((int)this.Result);
            resultBytes.CopyTo(byteArray, index);
            index += resultBytes.Length;

            for (int i = 0; i < PublicConstants.LParamCount; i++)
            {
                byte[] lParamBytes = BitConverter.GetBytes(this.LParam[i]);
                lParamBytes.CopyTo(byteArray, index);
                index += lParamBytes.Length;
            }

            for (int i = 0; i < PublicConstants.FParamCount; i++)
            {
                byte[] fParamBytes = BitConverter.GetBytes(this.FParam[i]);
                fParamBytes.CopyTo(byteArray, index);
                index += fParamBytes.Length;
            }

            retVal = byteArray;

            return retVal;
        }
    }
}
