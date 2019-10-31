using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DetailTool.Components.Monitor.Items
{
    public class DateTimeData : StatusItemBase
    {
        private int m_Year = 0;
        private int m_Month = 0;
        private int m_Day = 0;
        private int m_Hour = 0;
        private int m_Minute = 0;
        private int m_Second = 0;

        /// <summary>
        /// デフォルトコンストラクタ
        /// </summary>
        public DateTimeData()
        {
        }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns>サイズ</returns>
        public override int GetSize()
        {
            // Y/M/D h/m/s の 6 つ
            return sizeof(int) * 6;
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

            m_Year = BitConverter.ToInt32(data, index);
            index += sizeof(int);

            m_Month = BitConverter.ToInt32(data, index);
            index += sizeof(int);

            m_Day = BitConverter.ToInt32(data, index);
            index += sizeof(int);

            m_Hour = BitConverter.ToInt32(data, index);
            index += sizeof(int);

            m_Minute = BitConverter.ToInt32(data, index);
            index += sizeof(int);

            m_Second = BitConverter.ToInt32(data, index);
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
            string retVal = string.Format("{0:0000}/{1:00}/{2:00} {3:00}:{4:00}:{5:00}", m_Year, m_Month, m_Day, m_Hour, m_Minute, m_Second);
            return retVal;
        }
    }
}
