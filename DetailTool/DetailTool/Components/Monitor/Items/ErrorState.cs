using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DetailTool.Components.Monitor.Items
{
    public class ErrorState : StatusItemBase
    {
        /// <summary>
        /// デフォルトコンストラクタ
        /// </summary>
        public ErrorState()
        {
        }

        /// <summary>
        /// Gets or sets 異常発生状態
        /// </summary>
        public bool IsErrorState { get; set; }

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

            int systemError = BitConverter.ToInt32(data, index);
            if (systemError != 0)
            {
                this.IsErrorState = true;
            }
            else
            {
                this.IsErrorState = false;
            }
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

            if (this.IsErrorState != false)
            {
                retVal = "異常発生";
            }
            else
            {
                retVal = "正常";
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

            if (this.IsErrorState == true)
            {
                retVal = System.Drawing.Color.Pink;
            }

            return retVal;
        }
    }
}
