using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DetailTool.Components.Setting
{
    public class DateTimeSetting
    {
        public DateTimeSetting()
        {
        }

        public DayOfWeekEnum DayOfWeek { get; set; }
        public int Hour { get; set; }
        public int Minulte { get; set; }

        /// <summary>
        /// 曜日定数
        /// </summary>
        public enum DayOfWeekEnum
        {
            Sunday,         // 日曜日
            Monday,         // 月曜日
            Tuesday,        // 火曜日
            Wednesday,      // 水曜日
            Tharsday,       // 木曜日
            Friday,         // 金曜日
            Saturday,       // 土曜日
            Everyday,       // 毎日
            Nothing,        // 無し
        }

        /// <summary>
        /// サイズ取得
        /// </summary>
        /// <returns>サイズ</returns>
        public int GetSize()
        {
            return sizeof(int) + sizeof(short) + sizeof(short);
        }

        /// <summary>
        /// 解析処理
        /// </summary>
        /// <param name="data">解析対象データ</param>
        /// <param name="startIndex">解析開始インデックス</param>
        /// <returns>解析済みインデックス</returns>
        public int Analyze(byte[] data, int startIndex)
        {
            int retVal = 0;
            int dataIndex = startIndex;

            // 曜日
            int intValue = BitConverter.ToInt32(data, dataIndex);
            this.DayOfWeek = (DayOfWeekEnum)intValue;
            dataIndex += sizeof(int);

            // 時
            this.Hour = BitConverter.ToInt16(data, dataIndex);
            dataIndex += sizeof(short);

            // 分
            this.Minulte = BitConverter.ToInt16(data, dataIndex);
            dataIndex += sizeof(short);

            retVal = dataIndex;

            return retVal;
        }
    }
}
